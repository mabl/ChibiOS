#include "board.h"
#define MSD_RW_LED_ON()   palSetPad(GPIOI, GPIOI_LED4);
#define MSD_RW_LED_OFF()  palClearPad(GPIOI, GPIOI_LED4);




#define USB_MS_DATA_EP 1

#if STM32_USE_USB_OTG2_HS
#  define USB_MS_EP_SIZE 512
#else
#  define USB_MS_EP_SIZE 64
#endif

#define MSD_REQ_RESET		0xFF
#define MSD_GET_MAX_LUN		0xFE
#define MSD_CBW_SIGNATURE                               0x43425355
#define MSD_CSW_SIGNATURE                               0x53425355
#define MSD_COMMAND_DIR_DATA_OUT                        (0 << 7)
#define MSD_COMMAND_DIR_DATA_IN                         (1 << 7)

#define MSD_SETUP_WORD(setup, index) (uint16_t)(((uint16_t)setup[index+1] << 8) | (setup[index] & 0x00FF))

#define MSD_SETUP_VALUE(setup) MSD_SETUP_WORD(setup, 2)
#define MSD_SETUP_INDEX(setup) MSD_SETUP_WORD(setup, 4)
#define MSD_SETUP_LENGTH(setup) MSD_SETUP_WORD(setup, 6)

#define SCSI_CMD_INQUIRY 						0x12
#define SCSI_CMD_REQUEST_SENSE 					0x03
#define SCSI_CMD_READ_CAPACITY_10				0x25
#define SCSI_CMD_READ_10 						0x28
#define SCSI_CMD_WRITE_10						0x2A
#define SCSI_CMD_TEST_UNIT_READY				0x00
#define SCSI_CMD_PREVENT_ALLOW_MEDIUM_REMOVAL	0x1E
#define SCSI_CMD_VERIFY_10						0x2F
#define SCSI_CMD_SEND_DIAGNOSTIC				0x1D
#define SCSI_CMD_MODE_SENSE_6                   0x1A
#define SCSI_CMD_START_STOP_UNIT				0x1B

#define MSD_COMMAND_PASSED 0x00
#define MSD_COMMAND_FAILED 0x01
#define MSD_COMMAND_PHASE_ERROR 0x02

#define SCSI_SENSE_KEY_GOOD                     0x00
#define SCSI_SENSE_KEY_RECOVERED_ERROR          0x01
#define SCSI_SENSE_KEY_NOT_READY                0x02
#define SCSI_SENSE_KEY_MEDIUM_ERROR             0x03
#define SCSI_SENSE_KEY_HARDWARE_ERROR           0x04
#define SCSI_SENSE_KEY_ILLEGAL_REQUEST          0x05
#define SCSI_SENSE_KEY_UNIT_ATTENTION           0x06
#define SCSI_SENSE_KEY_DATA_PROTECT             0x07
#define SCSI_SENSE_KEY_BLANK_CHECK              0x08
#define SCSI_SENSE_KEY_VENDOR_SPECIFIC          0x09
#define SCSI_SENSE_KEY_COPY_ABORTED             0x0A
#define SCSI_SENSE_KEY_ABORTED_COMMAND          0x0B
#define SCSI_SENSE_KEY_VOLUME_OVERFLOW          0x0D
#define SCSI_SENSE_KEY_MISCOMPARE               0x0E
#define SCSI_ASENSE_NO_ADDITIONAL_INFORMATION   0x00
#define SCSI_ASENSE_LOGICAL_UNIT_NOT_READY      0x04
#define SCSI_ASENSE_INVALID_FIELD_IN_CDB        0x24
#define SCSI_ASENSE_NOT_READY_TO_READY_CHANGE   0x28
#define SCSI_ASENSE_WRITE_PROTECTED             0x27
#define SCSI_ASENSE_FORMAT_ERROR                0x31
#define SCSI_ASENSE_INVALID_COMMAND             0x20
#define SCSI_ASENSE_LOGICAL_BLOCK_ADDRESS_OUT_OF_RANGE 0x21
#define SCSI_ASENSE_MEDIUM_NOT_PRESENT                 0x3A
#define SCSI_ASENSEQ_NO_QUALIFIER                      0x00
#define SCSI_ASENSEQ_FORMAT_COMMAND_FAILED             0x01
#define SCSI_ASENSEQ_INITIALIZING_COMMAND_REQUIRED     0x02
#define SCSI_ASENSEQ_OPERATION_IN_PROGRESS             0x07

PACK_STRUCT_BEGIN typedef struct {
	uint32_t signature;
	uint32_t tag;
	uint32_t data_len;
	uint8_t flags;
	uint8_t lun;
	uint8_t scsi_cmd_len;
	uint8_t scsi_cmd_data[16];
} PACK_STRUCT_STRUCT msd_cbw_t PACK_STRUCT_END;

PACK_STRUCT_BEGIN typedef struct {
	uint32_t signature;
	uint32_t tag;
	uint32_t data_residue;
	uint8_t status;
} PACK_STRUCT_STRUCT msd_csw_t PACK_STRUCT_END;

typedef struct {
		uint8_t byte[18];
} __attribute__ ((packed)) scsi_sense_response_t;

PACK_STRUCT_BEGIN typedef struct
{
	uint8_t peripheral;
	uint8_t removable;
	uint8_t version;
	uint8_t response_data_format;
	uint8_t additional_length;
	uint8_t sccstp;
	uint8_t bqueetc;
	uint8_t cmdque;
	uint8_t vendorID[8];
	uint8_t productID[16];
	uint8_t productRev[4];
} PACK_STRUCT_STRUCT scsi_inquiry_response_t PACK_STRUCT_END;

PACK_STRUCT_BEGIN typedef struct {
	uint32_t last_block_addr;
	uint32_t block_size;
} PACK_STRUCT_STRUCT SCSIReadCapacity10Response_t PACK_STRUCT_END;

PACK_STRUCT_BEGIN typedef struct {
	uint8_t op_code;
	uint8_t lun_immed;
	uint8_t res1;
	uint8_t res2;
	uint8_t loej_start;
	uint8_t control;
} PACK_STRUCT_STRUCT SCSIStartStopUnitRequest_t;

typedef struct USBMassStorageDriver USBMassStorageDriver;

typedef enum { idle, read_cmd_block, ejected} msd_state_t;

struct USBMassStorageDriver {
	USBDriver                 *usbp;
	BinarySemaphore bsem;
	BinarySemaphore usb_transfer_thread_bsem;
	BinarySemaphore mass_sorage_thd_bsem;
	BaseBlockDevice *bbdp;
	EventSource evt_connected, evt_ejected;
	BlockDeviceInfo block_dev_info;
	msd_state_t state;
	msd_cbw_t cbw;
	msd_csw_t csw;
	scsi_sense_response_t sense;
	bool_t result;
};

#define MSD_CONNECTED			0
#define MSD_EJECTED				1

#ifdef __cplusplus
extern "C" {
#endif
void msdInit(USBDriver *usbp, BaseBlockDevice *bdp, USBMassStorageDriver *msdp);
#ifdef __cplusplus
}
#endif
