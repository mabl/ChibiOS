#include "ch.h"
#include "hal.h"
#include "usb_msd.h"
#include "chprintf.h"

static BaseSequentialStream *chp = (BaseSequentialStream *)&SD2;

static WORKING_AREA(waMassStorage, 1024);
static WORKING_AREA(waMassStorageUSBTransfer, 1024);

static msg_t MassStorageUSBTransferThd(void *arg);
static msg_t MassStorageThd(void *arg);

static Thread *msdThd = NULL;
static Thread *msdUSBTransferThd = NULL;

static void WaitForISR(USBMassStorageDriver *msdp, const int max_ms);

static volatile uint32_t trigger_transfer_index = UINT32_MAX;

#define BLOCK_SIZE_INCREMENT        512
#define BLOCK_WRITE_ITTERATION_COUNT    32

static uint8_t rw_buf[2][BLOCK_SIZE_INCREMENT];

typedef struct {
  uint8_t is_transfer_done;
  /*Number of blocks actually read from USB IN endpont that should be written to SD card*/
  int num_blocks_to_write;
  /*Number of blocks to read from USB IN endpoint*/
  int max_blocks_to_read;
  uint8_t buf[(BLOCK_SIZE_INCREMENT * BLOCK_WRITE_ITTERATION_COUNT)];
} rw_usb_sd_buffer_t;

static volatile rw_usb_sd_buffer_t rw_ping_pong_buffer[2];

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

/*
 * USB Device Descriptor.
 */
static const uint8_t msd_device_descriptor_data[18] = {
  USB_DESC_DEVICE       (0x0200,        /* bcdUSB (2.0).                    */
                         0x00,          /* bDeviceClass (None).             */
                         0x00,          /* bDeviceSubClass.                 */
                         0x00,          /* bDeviceProtocol.                 */
                         0x40,          /* Control Endpoint Size.           */
                         0x0483,        /* idVendor (ST).                   */
                         0x5742,        /* idProduct.                       */
                         0x0100,        /* bcdDevice.                       */
                         1,             /* iManufacturer.                   */
                         2,             /* iProduct.                        */
                         3,             /* iSerialNumber.                   */
                         1)             /* bNumConfigurations.              */
};

/*
 * Device Descriptor wrapper.
 */
static const USBDescriptor msd_device_descriptor = {
  sizeof msd_device_descriptor_data,
  msd_device_descriptor_data
};

/* Configuration Descriptor tree for a CDC.*/
static const uint8_t msd_configuration_descriptor_data[] = {
  /* Configuration Descriptor.*/
  USB_DESC_CONFIGURATION(0x0020,            /* wTotalLength.                    */
                         0x01,          /* bNumInterfaces.                  */
                         0x01,          /* bConfigurationValue.             */
                         0,             /* iConfiguration.                  */
                         0xC0,          /* bmAttributes (self powered).     */
                         0x32),           /* bMaxPower (100mA).               */
  /* Interface Descriptor.*/
  USB_DESC_INTERFACE    (0x00,          /* bInterfaceNumber.                */
                         0x00,          /* bAlternateSetting.               */
                         0x02,          /* bNumEndpoints.                   */
                         0x08,          /* bInterfaceClass (Mass Storage)   */
                         0x06,          /* bInterfaceSubClass (SCSI
                         	 	 	 	   Transparent storage class)       */
                         0x50,          /* bInterfaceProtocol (Bulk Only)   */
                         0),            /* iInterface. (none)               */
  /* Mass Storage Data In Endpoint Descriptor.*/
  USB_DESC_ENDPOINT     (USB_MS_DATA_EP|0x80,
                         0x02,          /* bmAttributes (Bulk).             */
                         USB_MS_EP_SIZE,/* wMaxPacketSize.                  */
                         0x05),         /* bInterval. 1ms                   */
  /* Mass Storage Data In Endpoint Descriptor.*/
  USB_DESC_ENDPOINT     (USB_MS_DATA_EP,
						 0x02,          /* bmAttributes (Bulk).             */
						 USB_MS_EP_SIZE,/* wMaxPacketSize.                  */
						 0x05)          /* bInterval. 1ms                   */
};

/*
 * Configuration Descriptor wrapper.
 */
static const USBDescriptor msd_configuration_descriptor = {
  sizeof msd_configuration_descriptor_data,
  msd_configuration_descriptor_data
};

/*
 * U.S. English language identifier.
 */
static const uint8_t msd_string0[] = {
  USB_DESC_BYTE(4),                     /* bLength.                         */
  USB_DESC_BYTE(USB_DESCRIPTOR_STRING), /* bDescriptorType.                 */
  USB_DESC_WORD(0x0409)                 /* wLANGID (U.S. English).          */
};

/*
 * Vendor string.
 */
static const uint8_t msd_string1[] = {
  USB_DESC_BYTE(38),                    /* bLength.                         */
  USB_DESC_BYTE(USB_DESCRIPTOR_STRING), /* bDescriptorType.                 */
  'S', 0, 'T', 0, 'M', 0, 'i', 0, 'c', 0, 'r', 0, 'o', 0, 'e', 0,
  'l', 0, 'e', 0, 'c', 0, 't', 0, 'r', 0, 'o', 0, 'n', 0, 'i', 0,
  'c', 0, 's', 0
};

/*
 * Device Description string.
 */
static const uint8_t msd_string2[] = {
  USB_DESC_BYTE(62),                    /* bLength.                         */
  USB_DESC_BYTE(USB_DESCRIPTOR_STRING), /* bDescriptorType.                 */
  'C', 0, 'h', 0, 'i', 0, 'b', 0, 'i', 0, 'O', 0, 'S', 0, '/', 0,
  'R', 0, 'T', 0, ' ', 0, 'M', 0, 'a', 0, 's', 0, 's', 0, ' ', 0,
  'S', 0, 't', 0, 'o', 0, 'r', 0, 'a', 0, 'g', 0, 'e', 0, ' ', 0,
  'D', 0, 'e', 0, 'v', 0, 'i', 0, 'c', 0, 'e'
};

static const uint8_t msd_string3[] = {
  USB_DESC_BYTE(26),                     /* bLength.                         */
  USB_DESC_BYTE(USB_DESCRIPTOR_STRING), /* bDescriptorType.                 */
  'A', 0, 'E', 0, 'C', 0, 'C', 0, 'E', 0, 'C', 0, 'C', 0, 'C', 0, 'C', 0,
  '0' + CH_KERNEL_MAJOR, 0,
  '0' + CH_KERNEL_MINOR, 0,
  '0' + CH_KERNEL_PATCH, 0
};

/*
 * Strings wrappers array.
 */
static const USBDescriptor msd_strings[] = {
  {sizeof msd_string0, msd_string0},
  {sizeof msd_string1, msd_string1},
  {sizeof msd_string2, msd_string2},
  {sizeof msd_string3, msd_string3}
};

/*
 * Handles the GET_DESCRIPTOR callback. All required descriptors must be
 * handled here.
 */
static const USBDescriptor *get_descriptor(USBDriver *usbp,
                                           uint8_t dtype,
                                           uint8_t dindex,
                                           uint16_t lang) {

  (void)usbp;
  (void)lang;
  switch (dtype) {
  case USB_DESCRIPTOR_DEVICE:
    return &msd_device_descriptor;
  case USB_DESCRIPTOR_CONFIGURATION:
    return &msd_configuration_descriptor;
  case USB_DESCRIPTOR_STRING:
    if (dindex < 4)
      return &msd_strings[dindex];
  }
  return NULL;
}

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

static void WaitForISR(USBMassStorageDriver *msdp, const int max_ms) {
	/* sleep until it completes */
	chSysLock();
	if( max_ms <= 0 ) {
	  chBSemWaitS(&msdp->bsem);
	} else {
	  chBSemWaitTimeoutS(&msdp->bsem, max_ms);
	}
	chSysUnlock();
}

void msdUsbEvent(USBDriver *usbp, usbep_t ep) {
	(void)usbp;
	(void)ep;

	chSysLockFromIsr();
	chBSemSignalI(&((USBMassStorageDriver *)usbp->param)->bsem);
	chSysUnlockFromIsr();
}

/**
 * @brief   IN EP1 state.
 */
static USBInEndpointState ep1InState;
static USBOutEndpointState ep1OutState;

/**
 * @brief   EP1 initialization structure (IN only).
 */
static const USBEndpointConfig epDataConfig = {
  USB_EP_MODE_TYPE_BULK,
  NULL,
  msdUsbEvent,
  msdUsbEvent,
  USB_MS_EP_SIZE,
  USB_MS_EP_SIZE,
  &ep1InState,
  &ep1OutState,
  1,
  NULL
};

/*
 * Handles the USB driver global events.
 */
static void usb_event(USBDriver *usbp, usbevent_t event) {
	USBMassStorageDriver *msdp = (USBMassStorageDriver *)usbp->param;
  switch (event) {
  case USB_EVENT_RESET:
    msdp->reconfigured_or_reset_event = TRUE;
    return;
  case USB_EVENT_ADDRESS:
    return;
  case USB_EVENT_CONFIGURED:
    chSysLockFromIsr();
    msdp->reconfigured_or_reset_event = TRUE;
    usbInitEndpointI(usbp, USB_MS_DATA_EP, &epDataConfig);
	/* initialise the thread */
	chBSemSignalI(&msdp->bsem);

	/* signal that the device is connected */
	chEvtBroadcastI(&msdp->evt_connected);
	chSysUnlockFromIsr();

    return;
  case USB_EVENT_SUSPEND:
    return;
  case USB_EVENT_WAKEUP:
    return;
  case USB_EVENT_STALLED:
    return;
  }
  return;
}

static const USBConfig msd_usb_config = {
	usb_event,
	get_descriptor,
	msdRequestsHook,
	NULL
};

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

  palSetPad(GPIOH, GPIOH_LED2);
  for(cnt = 0; cnt < BLOCK_WRITE_ITTERATION_COUNT && cnt < dest_buffer->max_blocks_to_read; cnt++ ) {
    usbPrepareReceive(msdp->usbp, USB_MS_DATA_EP, (uint8_t*)&dest_buffer->buf[cnt * BLOCK_SIZE_INCREMENT], (msdp->block_dev_info.blk_size));

    chSysLock();
    usbStartReceiveI(msdp->usbp, USB_MS_DATA_EP);
    chSysUnlock();

    WaitForISR(msdp, 0);
    dest_buffer->num_blocks_to_write++;
  }
  dest_buffer->is_transfer_done = TRUE;
  palClearPad(GPIOH, GPIOH_LED2);
}

static void WaitForUSBTransferComplete(USBMassStorageDriver *msdp, const int ping_pong_buffer_index) {
  while(TRUE) {
    const msg_t m = chBSemWaitTimeout(&msdp->mass_sorage_thd_bsem, MS2ST(1));

    if( rw_ping_pong_buffer[ping_pong_buffer_index].is_transfer_done ) {
      break;
    } else {
      chThdSleepMilliseconds(1);
    }
  }
}

bool_t SCSICommandStartReadWrite10(USBMassStorageDriver *msdp) {
	msd_cbw_t *cbw = &(msdp->cbw);
	bool_t data_cached = FALSE;
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
	  trigger_transfer_index = ping_pong_buffer_index;//Trigger the transfer in the other thread

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
            if(trigger_transfer_index == UINT32_MAX) {
              rw_ping_pong_buffer[empty_buffer_index].max_blocks_to_read = total_blocks - i - BLOCK_WRITE_ITTERATION_COUNT;

              trigger_transfer_index = empty_buffer_index;

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
           chprintf(chp, "\r\nCant write 0 blocks, this should not happen, halting\r\n");
           chThdSleepMilliseconds(50);
           chSysHalt();
         }

         /* now write the block to the block device */
         if(blkWrite(msdp->bbdp, rw_block_address, (uint8_t*)rw_ping_pong_buffer[done_buffer_index].buf,
                     rw_ping_pong_buffer[done_buffer_index].num_blocks_to_write) == CH_FAILED || (!palReadPad(GPIOF, GPIOF_USER_BUTTON)) ) {

             chprintf(chp, "\r\nSD Block Write Error, halting\r\n");
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

             /* TODO: handle this */
             //chSysHalt();
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
          if(blkRead(msdp->bbdp, rw_block_address, rw_buf[i % 2], 1) == CH_FAILED) {
              /* TODO: handle this */
              chprintf(chp, "\r\nSD Block Read Error\r\n");
          } else {
            read_success = TRUE;
            break;
          }
		}
		if( (!read_success) || (!palReadPad(GPIOF, GPIOF_USER_BUTTON)) ) {
		  chprintf(chp, "\r\nSD Block Read Error 1, halting\r\n");
		  chThdSleepMilliseconds(70);//wait for printing to finish
		  msdp->result = FALSE;

		  chprintf(chp, "\r\nSetting sense code %u\r\n", SCSI_SENSE_KEY_MEDIUM_ERROR);
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
			usbPrepareTransmit(msdp->usbp, USB_MS_DATA_EP, rw_buf[i % 2],
					msdp->block_dev_info.blk_size);

			chSysLock();
			usbStartTransmitI(msdp->usbp, USB_MS_DATA_EP);
			chSysUnlock();

			if(i < (total_blocks - 1)) {
				/* there is at least one more block to be read from device */
				/* so read that while the USB transfer takes place */
			    read_success = FALSE;
                for(retry_count = 0; retry_count < 3; retry_count++ ) {
                  if(blkRead(msdp->bbdp, rw_block_address, rw_buf[(i+1) % 2], 1) == CH_FAILED ) {
                      chprintf(chp, "\r\nSD Block Read Error 2\r\n");
                  } else {
                    read_success = TRUE;
                    break;
                  }
                }

                if( (! read_success) || (!palReadPad(GPIOF, GPIOF_USER_BUTTON)) ) {
                  chprintf(chp, "\r\nSD Block Read Error 22, addr=%d, halting\r\n", rw_block_address);
                  chThdSleepMilliseconds(70);//wait for printing to finish
                  msdp->result = FALSE;
                  chprintf(chp, "\r\nSetting sense code %u\r\n", SCSI_SENSE_KEY_MEDIUM_ERROR);
                  SCSISetSense(   msdp,
                                  SCSI_SENSE_KEY_MEDIUM_ERROR,
                                  SCSI_ASENSE_NO_ADDITIONAL_INFORMATION,
                                  SCSI_ASENSEQ_NO_QUALIFIER);
                  return FALSE;
                }

                rw_block_address++;
			}

			WaitForISR(msdp, 0);
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
	    chprintf(chp, "CMD_INQ\r\n");
		sleep = SCSICommandInquiry(msdp);
		break;
	case SCSI_CMD_REQUEST_SENSE:
	    chprintf(chp, "\r\nCMD_RS\r\n");
		sleep = SCSICommandRequestSense(msdp);
		break;
	case SCSI_CMD_READ_CAPACITY_10:
	    chprintf(chp, "CMD_RC10\r\n");
		sleep = SCSICommandReadCapacity10(msdp);
		break;
	case SCSI_CMD_READ_10:
	case SCSI_CMD_WRITE_10:
	    chprintf(chp, "CMD_RW\r\n");
		MSD_RW_LED_ON();
		sleep = SCSICommandStartReadWrite10(msdp);
		MSD_RW_LED_OFF();
		break;
	case SCSI_CMD_SEND_DIAGNOSTIC:
	  chprintf(chp, "CMD_DIA\r\n");
		sleep = SCSICommandSendDiagnostic(msdp);
		break;
	case SCSI_CMD_TEST_UNIT_READY:
	case SCSI_CMD_PREVENT_ALLOW_MEDIUM_REMOVAL:
	case SCSI_CMD_VERIFY_10:
		/* don't handle */
		msdp->result = TRUE;
		break;
	case SCSI_CMD_MODE_SENSE_6:
	  chprintf(chp, "\r\nCMD_S6\r\n");
		sleep = SCSICommandModeSense6(msdp);
		break;
	case SCSI_CMD_START_STOP_UNIT:
	    chprintf(chp, "CMD_STOP\r\n");
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
		WaitForISR(msdp, 0);
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
    if( trigger_transfer_index != UINT32_MAX ) {
      SCSIWriteTransferPingPong(msdp, &rw_ping_pong_buffer[trigger_transfer_index]);
      trigger_transfer_index = UINT32_MAX;
      //notify other thread
      chBSemSignal(&msdp->mass_sorage_thd_bsem);
    }

    chBSemWaitTimeout(&msdp->usb_transfer_thread_bsem, MS2ST(1));
  }
}

static msg_t MassStorageThd(void *arg) {
	USBMassStorageDriver *msdp = (USBMassStorageDriver *)arg;

	chRegSetThreadName("USB-MSD");

	bool_t wait_for_isr = FALSE;

	/* wait for the usb to be initialised */
	chprintf(chp, "Y");
	WaitForISR(msdp, 0);
	chprintf(chp, "y");

	while (TRUE) {
		wait_for_isr = FALSE;

		if( msdp->reconfigured_or_reset_event ) {
		  /*If the devices is unplugged and re-plugged but did not have a CPU reset,
		   * we must set the state back to idle.*/
		  msdp->reconfigured_or_reset_event = FALSE;
		  msdp->state = idle;
		}

		chprintf(chp, "state=%d\r\n", msdp->state);
		/* wait on data depending on the current state */
		switch(msdp->state) {
		case idle:
		    chprintf(chp, "IDL");
			wait_for_isr = msdWaitForCommandBlock(msdp);
			chprintf(chp, "x\r\n");
		    break;
		case read_cmd_block:
		    chprintf(chp, "RCB");
			wait_for_isr = msdReadCommandBlock(msdp);
			chprintf(chp, "x\r\n");
			break;
		case ejected:
			/* disconnect usb device */
		    chprintf(chp, "ejected\r\n");
		    chThdSleepMilliseconds(70);
			usbDisconnectBus(msdp->usbp);
			usbStop(msdp->usbp);
			chThdExit(0);
			return 0;
		}

		/* wait until the ISR wakes thread */
		if(wait_for_isr) {
		    chprintf(chp, "W");
			WaitForISR(msdp, 0);
			chprintf(chp, "w");
		}
	}

	return 0;
}

void msdInit(USBDriver *usbp, BaseBlockDevice *bbdp, USBMassStorageDriver *msdp) {
	uint8_t i;

	msdp->usbp = usbp;
	msdp->state = idle;
	msdp->bbdp = bbdp;

	chEvtInit(&msdp->evt_connected);
	chEvtInit(&msdp->evt_ejected);

	/* initialise binary semaphore as taken */
	chBSemInit(&msdp->bsem, TRUE);
	/* initialise binary semaphore as NOT taken */
	chBSemInit(&msdp->usb_transfer_thread_bsem, FALSE);
	chBSemInit(&msdp->mass_sorage_thd_bsem, FALSE);

	/* initialise sense values to zero */
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
	usbp->param = (void *)msdp;

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
