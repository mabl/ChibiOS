#include "ch.h"
#include "hal.h"
#include "usb_msd_cfg.h"
#include "usb_msd.h"
#include "chprintf.h"

#define MSD_ENABLE_PERF_DEBUG_GPIOS    FALSE

#define MSD_DEBUG   FALSE
#define msd_debug_print(args ...) if (MSD_DEBUG) { chprintf(args); }



#if HAL_USE_SERIAL_USB
#  ifndef MSD_USB_DRIVER_EXT_FIELDS_NAME
#    error "The serial usb driver and the mass storage driver both use the USBD->param variable and they conflict, you must define MSD_USB_DRIVER_EXT_FIELDS_NAME for these to work concurently."
#  endif
#endif


extern const USBConfig msd_usb_config;



static BaseSequentialStream *chp = (BaseSequentialStream *)&SD2;

static WORKING_AREA(waMassStorage, 1024);
static WORKING_AREA(waMassStorageUSBTransfer, 1024);

static msg_t MassStorageUSBTransferThd(void *arg);
static msg_t MassStorageThd(void *arg);

static Thread *msdThd = NULL;
static Thread *msdUSBTransferThd = NULL;

#define WAIT_ISR_SUCCESS                     0
#define WAIT_ISR_BUSS_RESET_OR_RECONNECT     1
static uint8_t WaitForISR(USBMassStorageDriver *msdp, const bool_t check_reset);

#define BLOCK_SIZE_INCREMENT        512
#define BLOCK_WRITE_ITTERATION_COUNT    32

typedef struct {
  uint8_t is_transfer_done;
  /*Number of blocks actually read from USB IN endpont that should be written to SD card*/
  int num_blocks_to_write;
  /*Number of blocks to read from USB IN endpoint*/
  int max_blocks_to_read;
  uint8_t buf[(BLOCK_SIZE_INCREMENT * BLOCK_WRITE_ITTERATION_COUNT)];
} rw_usb_sd_buffer_t;

static volatile rw_usb_sd_buffer_t rw_ping_pong_buffer[2];
static uint8_t read_buffer[2][BLOCK_SIZE_INCREMENT];

inline uint32_t swap_uint32( uint32_t val ) {
    val = ((val << 8) & 0xFF00FF00 ) | ((val >> 8) & 0xFF00FF );
    return ((val << 16) & 0xFFFF0000) | ((val >> 16) & 0x0000FFFF);
}

#define swap_uint16(x) ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8))

#if !defined(MSD_RW_LED_ON)
#define MSD_RW_LED_ON()
#endif

#if !defined(MSD_RW_LED_OFF)
#define MSD_RW_LED_OFF()
#endif



/*===========================================================================*/
/* USB related stuff.                                                        */
/*===========================================================================*/


/**
 * @brief   Default requests hook.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @return              The hook status.
 * @retval TRUE         Message handled internally.
 * @retval FALSE        Message not handled.
 */
bool_t msdRequestsHook(USBDriver *usbp) {

	if (((usbp->setup[0] & USB_RTYPE_TYPE_MASK) == USB_RTYPE_TYPE_CLASS) &&
		((usbp->setup[0] & USB_RTYPE_RECIPIENT_MASK) == USB_RTYPE_RECIPIENT_INTERFACE)) {
		/* check that the request is for interface 0.*/
		if(MSD_SETUP_INDEX(usbp->setup) != 0)
			return FALSE;

		/* act depending on bRequest = setup[1] */
		switch(usbp->setup[1]) {
		case MSD_REQ_RESET:
			/* check that it is a HOST2DEV request */
			if(((usbp->setup[0] & USB_RTYPE_DIR_MASK) != USB_RTYPE_DIR_HOST2DEV) ||
			   (MSD_SETUP_LENGTH(usbp->setup) != 0) ||
			   (MSD_SETUP_VALUE(usbp->setup) != 0))
				return FALSE;

			/* reset all endpoints */
			/* TODO!*/
			/* The device shall NAK the status stage of the device request until
			 * the Bulk-Only Mass Storage Reset is complete.
			 */
			return TRUE;
		case MSD_GET_MAX_LUN:
			/* check that it is a DEV2HOST request */
			if(((usbp->setup[0] & USB_RTYPE_DIR_MASK) != USB_RTYPE_DIR_DEV2HOST) ||
			   (MSD_SETUP_LENGTH(usbp->setup) != 1) ||
			   (MSD_SETUP_VALUE(usbp->setup) != 0))
				return FALSE;

			static uint8_t len_buf[1] = {0};
			/* stall to indicate that we don't support LUN */
			usbSetupTransfer(usbp, len_buf, 1, NULL);
			return TRUE;
		default:
			return FALSE;
			break;
		}
	}
  return FALSE;
}

static uint8_t WaitForISR(USBMassStorageDriver *msdp, const bool_t check_reset) {
  uint8_t ret = WAIT_ISR_SUCCESS;
	/* sleep until it completes */
	chSysLock();
	if( check_reset ) {
	  for(;;) {
	    const msg_t m = chBSemWaitTimeoutS(&msdp->bsem, 1);
	    if( m == RDY_OK ) {
	      break;
	    }

	    if( msdp->reconfigured_or_reset_event ) {
	      ret = WAIT_ISR_BUSS_RESET_OR_RECONNECT;
	      break;
	    }
	  }
	} else {
	  chBSemWaitS(&msdp->bsem);
	}

	chSysUnlock();
	return(ret);
}

void msdUsbEvent(USBDriver *usbp, usbep_t ep) {
	(void)usbp;
	(void)ep;

	chSysLockFromIsr();
	chBSemSignalI(&((USBMassStorageDriver *)usbp->USBD_PARAM_NAME)->bsem);
	chSysUnlockFromIsr();
}





static inline void SCSISetSense(USBMassStorageDriver *msdp, uint8_t key, uint8_t acode, uint8_t aqual) {
	msdp->sense.byte[2] = key;
	msdp->sense.byte[12] = acode;
	msdp->sense.byte[13] = aqual;
}

bool_t SCSICommandInquiry(USBMassStorageDriver *msdp) {
	msd_cbw_t *cbw = &(msdp->cbw);

	static const scsi_inquiry_response_t inquiry = {
		0x00,						// direct access block device
		0x80,						// removable
		0x04, 						// SPC-2
		0x02,						// response data format
		0x20,						// response has 0x20 + 4 bytes
		0x00,
		0x00,
		0x00,
		"Chibios",
		"Mass Storage",
		{'v',CH_KERNEL_MAJOR+'0','.',CH_KERNEL_MINOR+'0'},
	};

	if((cbw->scsi_cmd_data[1] & ((1 << 0) | (1 << 1))) ||
			cbw->scsi_cmd_data[2]) {
		/* Optional but unsupported bits set - update the SENSE key and fail the request */
		SCSISetSense(	msdp,
						SCSI_SENSE_KEY_ILLEGAL_REQUEST,
		               	SCSI_ASENSE_INVALID_FIELD_IN_CDB,
		               	SCSI_ASENSEQ_NO_QUALIFIER);

		return FALSE;
	}

	usbPrepareTransmit(msdp->usbp, USB_MS_DATA_EP, (uint8_t *)&inquiry,
			sizeof(scsi_inquiry_response_t));

	chSysLock();
	usbStartTransmitI(msdp->usbp, USB_MS_DATA_EP);
	chSysUnlock();

	msdp->result = TRUE;

	/* wait for ISR */
	return TRUE;
}

bool_t SCSICommandRequestSense(USBMassStorageDriver *msdp) {
	usbPrepareTransmit(msdp->usbp, USB_MS_DATA_EP, (uint8_t *)&msdp->sense,
			sizeof(scsi_sense_response_t));

	chSysLock();
	usbStartTransmitI(msdp->usbp, USB_MS_DATA_EP);
	chSysUnlock();

	msdp->result = TRUE;

	/* wait for ISR */
	return TRUE;
}

bool_t SCSICommandReadCapacity10(USBMassStorageDriver *msdp) {
	static SCSIReadCapacity10Response_t response;

	response.block_size = swap_uint32(msdp->block_dev_info.blk_size);
	response.last_block_addr = swap_uint32(msdp->block_dev_info.blk_num-1);

	usbPrepareTransmit(msdp->usbp, USB_MS_DATA_EP, (uint8_t *)&response,
			sizeof(SCSIReadCapacity10Response_t));

	chSysLock();
	usbStartTransmitI(msdp->usbp, USB_MS_DATA_EP);
	chSysUnlock();

	msdp->result = TRUE;

	/* wait for ISR */
	return TRUE;
}

bool_t SCSICommandSendDiagnostic(USBMassStorageDriver *msdp) {
	msd_cbw_t *cbw = &(msdp->cbw);

	if( (!cbw->scsi_cmd_data[1]) & (1 << 2)) {
		/* Only self-test supported - update SENSE key and fail the command */
		SCSISetSense(	msdp,
						SCSI_SENSE_KEY_ILLEGAL_REQUEST,
		                SCSI_ASENSE_INVALID_FIELD_IN_CDB,
		                SCSI_ASENSEQ_NO_QUALIFIER);

		return FALSE;
	}

	/* TODO: actually perform the test */
	msdp->result = TRUE;

	/* don't wait for ISR */
	return FALSE;
}

void SCSIWriteTransferPingPong(USBMassStorageDriver *msdp, volatile rw_usb_sd_buffer_t *dest_buffer) {
  int cnt;
  dest_buffer->is_transfer_done = FALSE;
  dest_buffer->num_blocks_to_write = 0;

#if MSD_ENABLE_PERF_DEBUG_GPIOS
  palSetPad(GPIOH, GPIOH_LED2);
#endif
  for(cnt = 0; cnt < BLOCK_WRITE_ITTERATION_COUNT && cnt < dest_buffer->max_blocks_to_read; cnt++ ) {
    usbPrepareReceive(msdp->usbp, USB_MS_DATA_EP, (uint8_t*)&dest_buffer->buf[cnt * BLOCK_SIZE_INCREMENT], (msdp->block_dev_info.blk_size));

    chSysLock();
    usbStartReceiveI(msdp->usbp, USB_MS_DATA_EP);
    chSysUnlock();

    WaitForISR(msdp, FALSE);
    dest_buffer->num_blocks_to_write++;
  }
  dest_buffer->is_transfer_done = TRUE;
#if MSD_ENABLE_PERF_DEBUG_GPIOS
  palClearPad(GPIOH, GPIOH_LED2);
#endif
}

static void WaitForUSBTransferComplete(USBMassStorageDriver *msdp, const int ping_pong_buffer_index) {
  while(TRUE) {
    chBSemWaitTimeout(&msdp->mass_sorage_thd_bsem, MS2ST(1));

    if( rw_ping_pong_buffer[ping_pong_buffer_index].is_transfer_done ) {
      break;
    } else {
      chThdSleepMilliseconds(1);
    }
  }
}

bool_t SCSICommandStartReadWrite10(USBMassStorageDriver *msdp) {
	msd_cbw_t *cbw = &(msdp->cbw);
	int read_success;
	int retry_count;

	if((cbw->scsi_cmd_data[0] == SCSI_CMD_WRITE_10) &&
			blkIsWriteProtected(msdp->bbdp)) {
		/* device is write protected and a write has been issued */
		/* Block address is invalid, update SENSE key and return command fail */
		SCSISetSense(	msdp,
						SCSI_SENSE_KEY_DATA_PROTECT,
		               	SCSI_ASENSE_WRITE_PROTECTED,
		                SCSI_ASENSEQ_NO_QUALIFIER);
		msdp->result = FALSE;
		return FALSE;
	}

	uint32_t rw_block_address = swap_uint32(*(uint32_t *)&cbw->scsi_cmd_data[2]);
	const uint16_t total_blocks = swap_uint16(*(uint16_t *)&cbw->scsi_cmd_data[7]);
	uint16_t i = 0;

	if(rw_block_address >= msdp->block_dev_info.blk_num) {
		/* Block address is invalid, update SENSE key and return command fail */
		SCSISetSense(	msdp,
						SCSI_SENSE_KEY_DATA_PROTECT,
	               	    SCSI_ASENSE_WRITE_PROTECTED,
	               	    SCSI_ASENSEQ_NO_QUALIFIER);
		msdp->result = FALSE;

		/* don't wait for ISR */
		return FALSE;
	}

	for(i = 0; i < 2; i++ ) {
	  //initialized ping pong buffer
	  rw_ping_pong_buffer[i].max_blocks_to_read = 0;
	  rw_ping_pong_buffer[i].num_blocks_to_write = 0;
	  rw_ping_pong_buffer[i].is_transfer_done = FALSE;
	}


	if(cbw->scsi_cmd_data[0] == SCSI_CMD_WRITE_10) {
      /* loop over each block */

	  int ping_pong_buffer_index = 0;
	  //initiate a transfer
	  rw_ping_pong_buffer[ping_pong_buffer_index].is_transfer_done = FALSE;
	  rw_ping_pong_buffer[ping_pong_buffer_index].max_blocks_to_read = total_blocks;
	  msdp->trigger_transfer_index = ping_pong_buffer_index;//Trigger the transfer in the other thread

	  //wake other thread on semaphore to trigger the transfer
	  chBSemSignal(&msdp->usb_transfer_thread_bsem);

	  WaitForUSBTransferComplete(msdp, ping_pong_buffer_index);

      for(i = 0; i < total_blocks; ) {
        const int done_buffer_index = ping_pong_buffer_index;
        const int empty_buffer_index = ((ping_pong_buffer_index + 1) % 2);

        //initiate another transfer in the other ping pong buffer
        int queue_another_transfer = FALSE;
        if( (i + BLOCK_WRITE_ITTERATION_COUNT) < total_blocks ) {
          queue_another_transfer = TRUE;
        }

        if( queue_another_transfer ) {
          while( TRUE ) {
            if(msdp->trigger_transfer_index == UINT32_MAX) {
              rw_ping_pong_buffer[empty_buffer_index].max_blocks_to_read = total_blocks - i - BLOCK_WRITE_ITTERATION_COUNT;

              msdp->trigger_transfer_index = empty_buffer_index;

              //wake other thread on semaphore to trigger the transfer
              chBSemSignal(&msdp->usb_transfer_thread_bsem);
              break;
            } else {
              chThdSleepMilliseconds(1);
            }
          }
        }




         if( rw_ping_pong_buffer[done_buffer_index].num_blocks_to_write <= 0 ) {
           /*This should never happen!!! Something is seriously wrong!*/
           msd_debug_print(chp, "\r\nCant write 0 blocks, this should not happen, halting\r\n");
           chThdSleepMilliseconds(50);
           chSysHalt();
         }

         /* now write the block to the block device */
         if(blkWrite(msdp->bbdp, rw_block_address, (uint8_t*)rw_ping_pong_buffer[done_buffer_index].buf,
                     rw_ping_pong_buffer[done_buffer_index].num_blocks_to_write) == CH_FAILED ) {

             msd_debug_print(chp, "\r\nSD Block Write Error, halting\r\n");
             chThdSleepMilliseconds(50);
             msdp->result = FALSE;
             SCSISetSense(   msdp,
                             SCSI_SENSE_KEY_MEDIUM_ERROR,
                             SCSI_ASENSE_NO_ADDITIONAL_INFORMATION,
                             SCSI_ASENSEQ_NO_QUALIFIER);

             if( queue_another_transfer ) {
               /*Let the previous queued transfer finish and ignore it.*/
               WaitForUSBTransferComplete(msdp, empty_buffer_index);
             }

             return(FALSE);
         }
         rw_block_address += rw_ping_pong_buffer[done_buffer_index].num_blocks_to_write;
         i += rw_ping_pong_buffer[done_buffer_index].num_blocks_to_write;
         rw_ping_pong_buffer[done_buffer_index].is_transfer_done = FALSE;
         rw_ping_pong_buffer[done_buffer_index].num_blocks_to_write = 0;


         if( queue_another_transfer ) {
           WaitForUSBTransferComplete(msdp, empty_buffer_index);
         }

         //Swap the ping pong buffers
         ping_pong_buffer_index = empty_buffer_index;
      }

	} else {
		i = 0;
		/* read the first block from block device */
		read_success = FALSE;
		for(retry_count = 0; retry_count < 3; retry_count++ ) {
          if(blkRead(msdp->bbdp, rw_block_address, read_buffer[i % 2], 1) == CH_FAILED) {
              msd_debug_print(chp, "\r\nSD Block Read Error\r\n");
          } else {
            read_success = TRUE;
            break;
          }
		}
		if( (!read_success) ) {
		  msd_debug_print(chp, "\r\nSD Block Read Error 1, halting\r\n");
		  chThdSleepMilliseconds(70);//wait for printing to finish
		  msdp->result = FALSE;

		  msd_debug_print(chp, "\r\nSetting sense code %u\r\n", SCSI_SENSE_KEY_MEDIUM_ERROR);
          SCSISetSense(   msdp,
                          SCSI_SENSE_KEY_MEDIUM_ERROR,
                          SCSI_ASENSE_NO_ADDITIONAL_INFORMATION,
                          SCSI_ASENSEQ_NO_QUALIFIER);
          return FALSE;
		}
		rw_block_address++;

		/* loop over each block */
		for(i = 0; i < total_blocks; i++) {
			/* transmit the block */
			usbPrepareTransmit(msdp->usbp, USB_MS_DATA_EP, read_buffer[i % 2],
					msdp->block_dev_info.blk_size);

			chSysLock();
			usbStartTransmitI(msdp->usbp, USB_MS_DATA_EP);
			chSysUnlock();

			if(i < (total_blocks - 1)) {
				/* there is at least one more block to be read from device */
				/* so read that while the USB transfer takes place */
			    read_success = FALSE;
                for(retry_count = 0; retry_count < 3; retry_count++ ) {
                  if(blkRead(msdp->bbdp, rw_block_address, read_buffer[(i+1) % 2], 1) == CH_FAILED ) {
                      msd_debug_print(chp, "\r\nSD Block Read Error 2\r\n");
                  } else {
                    read_success = TRUE;
                    break;
                  }
                }

                if( (! read_success) ) {
                  msd_debug_print(chp, "\r\nSD Block Read Error 22, addr=%d, halting\r\n", rw_block_address);
                  chThdSleepMilliseconds(70);//wait for printing to finish
                  msdp->result = FALSE;
                  msd_debug_print(chp, "\r\nSetting sense code %u\r\n", SCSI_SENSE_KEY_MEDIUM_ERROR);
                  SCSISetSense(   msdp,
                                  SCSI_SENSE_KEY_MEDIUM_ERROR,
                                  SCSI_ASENSE_NO_ADDITIONAL_INFORMATION,
                                  SCSI_ASENSEQ_NO_QUALIFIER);
                  return FALSE;
                }

                rw_block_address++;
			}

			/*FIXME In the event that the USB connection is unplugged while we're waiting for a bulk
			 * endpoint ISR, this will never return, and when re-plugged into the host, the drive will
			 * not show back up on the host. We need a way to break out of this loop when disconnected from the bus.
			 */
			if( WaitForISR(msdp, TRUE) == WAIT_ISR_BUSS_RESET_OR_RECONNECT ) {
              msdp->result = FALSE;
              return FALSE;
			}
		}
	}

	msdp->result = TRUE;

	/* don't wait for ISR */
	return FALSE;
}

bool_t SCSICommandStartStopUnit(USBMassStorageDriver *msdp) {
	SCSIStartStopUnitRequest_t *ssu = (SCSIStartStopUnitRequest_t *)&(msdp->cbw.scsi_cmd_data);

	if((ssu->loej_start & 0b00000011) == 0b00000010) {
		/* device has been ejected */
		chEvtBroadcast(&msdp->evt_ejected);

		msdp->state = ejected;
	}

	msdp->result = TRUE;

	/* don't wait for ISR */
	return FALSE;
}

bool_t SCSICommandModeSense6(USBMassStorageDriver *msdp) {
	/* Send an empty header response with the Write Protect flag status */
	/* TODO set byte3 to 0x80 if disk is read only */
	static uint8_t response[4] = {0x00, 0x00, 0x00, 0x00};

	usbPrepareTransmit(msdp->usbp, USB_MS_DATA_EP, response, 4);

	chSysLock();
	usbStartTransmitI(msdp->usbp, USB_MS_DATA_EP);
	chSysUnlock();

	msdp->result = TRUE;

	/* wait for ISR */
	return TRUE;
}

bool_t msdWaitForCommandBlock(USBMassStorageDriver *msdp) {
    usbPrepareReceive(msdp->usbp, USB_MS_DATA_EP,
            (uint8_t *)&msdp->cbw, sizeof(msd_cbw_t));

    chSysLock();
    usbStartReceiveI(msdp->usbp, USB_MS_DATA_EP);
    chSysUnlock();

    msdp->state = read_cmd_block;

    /* wait for ISR */
    return TRUE;
}



/* A command block has been received */
bool_t msdReadCommandBlock(USBMassStorageDriver *msdp) {
	msd_cbw_t *cbw = &(msdp->cbw);

	/* by default transition back to the idle state */
	msdp->state = idle;

	/* check the command */
	if((cbw->signature != MSD_CBW_SIGNATURE) ||
		(cbw->lun > 0) ||
		((cbw->data_len > 0) && (cbw->flags & 0x1F)) ||
		(cbw->scsi_cmd_len == 0) ||
		(cbw->scsi_cmd_len > 16)) {

		/* stall both IN and OUT endpoints */
		chSysLockFromIsr();
		usbStallReceiveI(msdp->usbp, USB_MS_DATA_EP);
		chSysUnlockFromIsr();

		/* don't wait for ISR */
		return FALSE;
	}

	bool_t sleep = FALSE;
	switch(cbw->scsi_cmd_data[0]) {
	case SCSI_CMD_INQUIRY:
	    msd_debug_print(chp, "CMD_INQ\r\n");
		sleep = SCSICommandInquiry(msdp);
		break;
	case SCSI_CMD_REQUEST_SENSE:
	    msd_debug_print(chp, "\r\nCMD_RS\r\n");
		sleep = SCSICommandRequestSense(msdp);
		break;
	case SCSI_CMD_READ_CAPACITY_10:
	    msd_debug_print(chp, "CMD_RC10\r\n");
		sleep = SCSICommandReadCapacity10(msdp);
		break;
	case SCSI_CMD_READ_10:
	case SCSI_CMD_WRITE_10:
	    msd_debug_print(chp, "CMD_RW\r\n");
		MSD_RW_LED_ON();
		sleep = SCSICommandStartReadWrite10(msdp);
		MSD_RW_LED_OFF();
		break;
	case SCSI_CMD_SEND_DIAGNOSTIC:
	  msd_debug_print(chp, "CMD_DIA\r\n");
		sleep = SCSICommandSendDiagnostic(msdp);
		break;
	case SCSI_CMD_TEST_UNIT_READY:
	case SCSI_CMD_PREVENT_ALLOW_MEDIUM_REMOVAL:
	case SCSI_CMD_VERIFY_10:
		/* don't handle */
		msdp->result = TRUE;
		break;
	case SCSI_CMD_MODE_SENSE_6:
	  msd_debug_print(chp, "\r\nCMD_S6\r\n");
		sleep = SCSICommandModeSense6(msdp);
		break;
	case SCSI_CMD_START_STOP_UNIT:
	    msd_debug_print(chp, "CMD_STOP\r\n");
		sleep = SCSICommandStartStopUnit(msdp);
		break;
	default:
		SCSISetSense(	msdp,
						SCSI_SENSE_KEY_ILLEGAL_REQUEST,
						SCSI_ASENSE_INVALID_COMMAND,
						SCSI_ASENSEQ_NO_QUALIFIER);

		/* stall IN endpoint */
		chSysLockFromIsr();
		usbStallTransmitI(msdp->usbp, USB_MS_DATA_EP);
		chSysUnlockFromIsr();

		cbw->data_len = 0;
		return FALSE;
	}

	cbw->data_len = 0;

	if(msdp->result) {
        /* update sense with success status */
        SCSISetSense(   msdp,
                        SCSI_SENSE_KEY_GOOD,
                        SCSI_ASENSE_NO_ADDITIONAL_INFORMATION,
                        SCSI_ASENSEQ_NO_QUALIFIER);
	} else {
        /* stall IN endpoint */
        chSysLockFromIsr();
        usbStallTransmitI(msdp->usbp, USB_MS_DATA_EP);
        chSysUnlockFromIsr();

        cbw->data_len = 0;
        return FALSE;
	}

	if(sleep) {
        if( WaitForISR(msdp, TRUE) == WAIT_ISR_BUSS_RESET_OR_RECONNECT ) {
          return(FALSE);
        }
	}

	msd_csw_t *csw = &(msdp->csw);

	if( (!msdp->result) && cbw->data_len ) {
		/* still bytes left to send, this is too early to send CSW? */
		chSysLockFromIsr();
		usbStallReceiveI(msdp->usbp, USB_MS_DATA_EP);
		usbStallTransmitI(msdp->usbp, USB_MS_DATA_EP);
		chSysUnlockFromIsr();

	    return FALSE;
	}

	csw->status = (msdp->result) ? MSD_COMMAND_PASSED : MSD_COMMAND_FAILED;
	csw->signature = MSD_CSW_SIGNATURE;
	csw->data_residue = cbw->data_len;
	csw->tag = cbw->tag;

	usbPrepareTransmit(msdp->usbp, USB_MS_DATA_EP, (uint8_t *)csw,
			sizeof(msd_csw_t));

	chSysLock();
	usbStartTransmitI(msdp->usbp, USB_MS_DATA_EP);
	chSysUnlock();

	/* wait on ISR */
	return TRUE;
}

static msg_t MassStorageUSBTransferThd(void *arg) {
  USBMassStorageDriver *msdp = (USBMassStorageDriver *)arg;

  for(;;) {
    if( msdp->trigger_transfer_index != UINT32_MAX ) {
      SCSIWriteTransferPingPong(msdp, &rw_ping_pong_buffer[msdp->trigger_transfer_index]);
      msdp->trigger_transfer_index = UINT32_MAX;
      //notify other thread
      chBSemSignal(&msdp->mass_sorage_thd_bsem);
    }

    chBSemWaitTimeout(&msdp->usb_transfer_thread_bsem, MS2ST(1));
  }

  return(0);
}

static msg_t MassStorageThd(void *arg) {
	USBMassStorageDriver *msdp = (USBMassStorageDriver *)arg;

	chRegSetThreadName("USB-MSD");

	bool_t wait_for_isr = FALSE;

	/* wait for the usb to be initialized */
	msd_debug_print(chp, "Y");
	WaitForISR(msdp, FALSE);
	msd_debug_print(chp, "y");

	while (TRUE) {
		wait_for_isr = FALSE;

		if( msdp->reconfigured_or_reset_event ) {
		  /*If the devices is unplugged and re-plugged but did not have a CPU reset,
		   * we must set the state back to idle.*/
		  msdp->reconfigured_or_reset_event = FALSE;
		  msdp->state = idle;
		}

		msd_debug_print(chp, "state=%d\r\n", msdp->state);
		/* wait on data depending on the current state */
		switch(msdp->state) {
		case idle:
		    msd_debug_print(chp, "IDL");
			wait_for_isr = msdWaitForCommandBlock(msdp);
			msd_debug_print(chp, "x\r\n");
		    break;
		case read_cmd_block:
		    msd_debug_print(chp, "RCB");
			wait_for_isr = msdReadCommandBlock(msdp);
			msd_debug_print(chp, "x\r\n");
			break;
		case ejected:
			/* disconnect usb device */
		    msd_debug_print(chp, "ejected\r\n");
		    chThdSleepMilliseconds(70);
			usbDisconnectBus(msdp->usbp);
			usbStop(msdp->usbp);
			chThdExit(0);
			return 0;
		}

		/* wait until the ISR wakes thread */
		if( wait_for_isr && (!msdp->reconfigured_or_reset_event) ) {
		    msd_debug_print(chp, "W");
			WaitForISR(msdp, FALSE);
			msd_debug_print(chp, "w");
		}
	}

	return 0;
}

void msdInit(USBDriver *usbp, BaseBlockDevice *bbdp, USBMassStorageDriver *msdp) {
	uint8_t i;

	msdp->usbp = usbp;
	msdp->state = idle;
	msdp->trigger_transfer_index = UINT32_MAX;
	msdp->bbdp = bbdp;

	chEvtInit(&msdp->evt_connected);
	chEvtInit(&msdp->evt_ejected);

	/* Initialize binary semaphore as taken, will cause the thread to initially wait on the  */
	chBSemInit(&msdp->bsem, TRUE);
	/* Initialize binary semaphore as NOT taken */
	chBSemInit(&msdp->usb_transfer_thread_bsem, FALSE);
	chBSemInit(&msdp->mass_sorage_thd_bsem, FALSE);

	/* Initialize sense values to zero */
	for(i = 0; i < sizeof(scsi_sense_response_t); i++)
		msdp->sense.byte[i] = 0x00;

	/* Response code = 0x70, additional sense length = 0x0A */
	msdp->sense.byte[0] = 0x70;
	msdp->sense.byte[7] = 0x0A;

	/* make sure block device is working and get info */
	while(TRUE) {
		blkstate_t state = blkGetDriverState(bbdp);
		if(state == BLK_READY)
			break;

		chThdSleepMilliseconds(50);
	}

	blkGetInfo(bbdp, &msdp->block_dev_info);

	usbDisconnectBus(usbp);
	chThdSleepMilliseconds(200);
	usbp->USBD_PARAM_NAME = (void *)msdp;

	usbStart(usbp, &msd_usb_config);
	usbConnectBus(usbp);

	if(msdThd == NULL) {
		msdThd = chThdCreateStatic(waMassStorage, sizeof(waMassStorage),
				NORMALPRIO, MassStorageThd, msdp);
	}

	if(msdUSBTransferThd == NULL) {
	    msdUSBTransferThd = chThdCreateStatic(waMassStorageUSBTransfer, sizeof(waMassStorageUSBTransfer),
	            NORMALPRIO, MassStorageUSBTransferThd, msdp);
	}
}
