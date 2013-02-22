/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ch.h"
#include "hal.h"
#include "usb_msd_cfg.h"
#include "usb_msd.h"
#include "usb_cdc.h"



#if STM32_USB_USE_OTG2 && STM32_USE_USB_OTG2_HS
#define USB_MAX_PACKET_SIZE         512
#define USB_CDC_INTERUPT_INTERVAL   0x10
#else
#define USB_MAX_PACKET_SIZE         64
#define USB_CDC_INTERUPT_INTERVAL   0xFF
#endif


/*see www.usb.org/developers/whitepapers/iadclasscode_r10.pdf*/
#define MULTI_FUNCTION_DEVICE_CLASS       0xEF
#define MULTI_FUNCTION_SUB_CLASS          0x02
#define MULTI_FUNCTION_DEVICE_PROTOCOL    0x01
#define VENDOR_ID                         0x0483
#define PRODUCT_ID                        0x5742

#define MASS_STORAGE_INTERFACE_CLASS            0x08
#define MASS_STORAGE_INTERFACE_SCSI_SUB_CLASS   0x06

#define COMMUNICATIONS_INTERFACE_CLASS    0x02
#define ABSTRACT_CONTROL_SUB_CLASS        0x02
#define AT_CDC_INTERFACE_PROTOCOL         0x01



#if USB_CDC_DATA_REQUEST_EP == USB_MS_DATA_EP
#  error "CDC and MSD Endpoints must be different"
#endif
#if USB_CDC_DATA_AVAILABLE_EP == USB_MS_DATA_EP
#  error "CDC and MSD Endpoints must be different"
#endif
#if USB_CDC_INTERRUPT_REQUEST_EP == USB_MS_DATA_EP
#  error "CDC and MSD Endpoints must be different"
#endif


/*
 * USB Device Descriptor.
 */
static const uint8_t msd_device_descriptor_data[18] = {
  USB_DESC_DEVICE       (0x0200,        /* bcdUSB (2.0).                    */
                         MULTI_FUNCTION_DEVICE_CLASS,       /* bDeviceClass (None).             */
                         MULTI_FUNCTION_SUB_CLASS,          /* bDeviceSubClass.                 */
                         MULTI_FUNCTION_DEVICE_PROTOCOL,    /* bDeviceProtocol.                 */
                         0x40,          /* Control Endpoint Size.           */
                         VENDOR_ID,     /* idVendor                         */
                         PRODUCT_ID,    /* idProduct.              */
                         0x0100,        /* bcdDevice.                       */
                         1,             /* iManufacturer.                   */
                         2,             /* iProduct.                        */
                         3,             /* iSerialNumber.                   */
                         1)             /* bNumConfigurations.              */
};

/*
 * Device Descriptor wrapper.
 */
const USBDescriptor msd_device_descriptor = {
  sizeof(msd_device_descriptor_data),
  msd_device_descriptor_data
};


/* Configuration Descriptor tree for a CDC.*/
static const uint8_t msd_configuration_descriptor_data[] = {
  /* Configuration Descriptor.*/
  USB_DESC_CONFIGURATION(98,            /* wTotalLength.                    */
                         0x03,          /* bNumInterfaces.                  */
                         0x01,          /* bConfigurationValue.             */
                         0,             /* iConfiguration.                  */
                         0xC0,          /* bmAttributes (self powered).     */
                         0x32),         /* bMaxPower (100mA).               */

    // Interface Association descriptor (IAD)
    0x08, //Length of IAD
    USB_DESCRIPTOR_INTERFACE_ASSOCIATION,
    0x00, // bFirstInterface
    0x02, // bInterfaceCount
    0xFF,//COMMUNICATIONS_INTERFACE_CLASS, //0xFF, // bFunctionClass
    0x00,//ABSTRACT_CONTROL_SUB_CLASS, //0x00, // bFunctionSubClass
    0x00,//AT_CDC_INTERFACE_PROTOCOL, //0x00, // bFunctionProcotol
    0x00, // iInterface


    /* Interface Descriptor. */
    USB_DESC_INTERFACE (0x00, /* bInterfaceNumber.                */
                0x00, /* bAlternateSetting.               */
                0x01, /* bNumEndpoints.                   */
                COMMUNICATIONS_INTERFACE_CLASS, /* bInterfaceClass (Communications Interface Class, CDC section 4.2).                            */
                ABSTRACT_CONTROL_SUB_CLASS, /* bInterfaceSubClass (Abstract Control Model, CDC section 4.3).   */
                AT_CDC_INTERFACE_PROTOCOL, /* bInterfaceProtocol (AT commands, CDC section 4.4).                */
                0), /* iInterface.                      */
    /* Header Functional Descriptor (CDC section 5.2.3).*/
    USB_DESC_BYTE (5), /* bLength.                         */
    USB_DESC_BYTE (0x24), /* bDescriptorType (CS_INTERFACE).  */
    USB_DESC_BYTE (0x00), /* bDescriptorSubtype (Header Functional Descriptor.           */
    USB_DESC_BCD (0x0110), /* bcdCDC.                          */
    /* Call Management Functional Descriptor. */
    USB_DESC_BYTE (5), /* bFunctionLength.                 */
    USB_DESC_BYTE (0x24), /* bDescriptorType (CS_INTERFACE).  */
    USB_DESC_BYTE (0x01), /* bDescriptorSubtype (Call Management Functional Descriptor).          */
    USB_DESC_BYTE (0x00), /* bmCapabilities (D0+D1).          */
    USB_DESC_BYTE (0x02), /* bDataInterface.                  */
    /* ACM Functional Descriptor.*/
    USB_DESC_BYTE (4), /* bFunctionLength.                 */
    USB_DESC_BYTE (0x24), /* bDescriptorType (CS_INTERFACE).  */
    USB_DESC_BYTE (0x02), /* bDescriptorSubtype (Abstract Control Management Descriptor).  */
    USB_DESC_BYTE (0x02), /* bmCapabilities.                  */
    /* Union Functional Descriptor.*/
    USB_DESC_BYTE (5), /* bFunctionLength.                 */
    USB_DESC_BYTE (0x24), /* bDescriptorType (CS_INTERFACE).  */
    USB_DESC_BYTE (0x06), /* bDescriptorSubtype (Union Functional Descriptor).          */
    USB_DESC_BYTE (0x00), /* bMasterInterface (Communication Class Interface).                */
    USB_DESC_BYTE (0x01), /* bSlaveInterface0 (Data Class Interface).                      */
    /* Endpoint 2 Descriptor. */
    USB_DESC_ENDPOINT (USB_CDC_INTERRUPT_REQUEST_EP|0x80,
            0x03, /* bmAttributes (Interrupt).        */
            0x0008, /* wMaxPacketSize.                  */
            USB_CDC_INTERUPT_INTERVAL), /* bInterval.                       */

    /* Interface Descriptor. */
    USB_DESC_INTERFACE (0x01, /* bInterfaceNumber.                */
            0x00, /* bAlternateSetting.               */
            0x02, /* bNumEndpoints.                   */
            0x0A, /* bInterfaceClass (Data Class Interface, CDC section 4.5).     */
            0x00, /* bInterfaceSubClass (CDC section 4.6).                            */
            0x00, /* bInterfaceProtocol (CDC section 4.7).                            */
            0x00), /* iInterface.                      */
    /* Endpoint 3 Descriptor. */
    USB_DESC_ENDPOINT (USB_CDC_DATA_AVAILABLE_EP, /* bEndpointAddress.*/
            0x02, /* bmAttributes (Bulk).             */
            USB_MAX_PACKET_SIZE, /* wMaxPacketSize.                  */
            0x00), /* bInterval.                       */
    /* Endpoint 1 Descriptor. */
    USB_DESC_ENDPOINT (USB_CDC_DATA_REQUEST_EP|0x80, /* bEndpointAddress.*/
            0x02, /* bmAttributes (Bulk).             */
            USB_MAX_PACKET_SIZE, /* wMaxPacketSize.                  */
            0x00), /* bInterval.                       */


    /* Mass Storage Device */
    /* Interface Descriptor. */
    USB_DESC_INTERFACE    (0x02,          /* bInterfaceNumber.                */
             0x00,          /* bAlternateSetting.               */
             0x02,          /* bNumEndpoints.                   */
             MASS_STORAGE_INTERFACE_CLASS,          /* bInterfaceClass (Mass Storage)   */
             MASS_STORAGE_INTERFACE_SCSI_SUB_CLASS,          /* bInterfaceSubClass (SCSI Transparent storage class)       */
             0x50,          /* bInterfaceProtocol (Bulk Only)   */
             2),            /* iInterface. (none)               */
    /* Mass Storage Data In Endpoint Descriptor. 5*/
    USB_DESC_ENDPOINT     (USB_MS_DATA_EP|0x80,
             0x02,          /* bmAttributes (Bulk).             */
             USB_MAX_PACKET_SIZE,/* wMaxPacketSize.                  */
             0x05),         /* bInterval. 1ms                   */
    /* Mass Storage Data In Endpoint Descriptor. 5*/
    USB_DESC_ENDPOINT     (USB_MS_DATA_EP,
             0x02,          /* bmAttributes (Bulk).             */
             USB_MAX_PACKET_SIZE,/* wMaxPacketSize.                  */
             0x05),          /* bInterval. 1ms                   */
};




/*
 * Configuration Descriptor wrapper.
 */
const USBDescriptor msd_configuration_descriptor = {
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
const USBDescriptor msd_strings[] = {
  {sizeof msd_string0, msd_string0},
  {sizeof msd_string1, msd_string1},
  {sizeof msd_string2, msd_string2},
  {sizeof msd_string3, msd_string3},
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
 * @brief   IN EP1 state.
 */
static USBInEndpointState epMSInState;
static USBOutEndpointState epMSOutState;

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
  &epMSInState,
  &epMSOutState,
  1,
  NULL
};



/**
 * @brief   IN EP1 state.
 */
static USBInEndpointState epCDC1instate;

/**
 * @brief   OUT EP1 state.
 */
static USBOutEndpointState epCDC1outstate;

/**
 * @brief   EP1 initialization structure (both IN and OUT).
 */
static const USBEndpointConfig epCDC1config = {
  USB_EP_MODE_TYPE_BULK,
  NULL,
  sduDataTransmitted,
  sduDataReceived,
  0x0040,
  0x0040,
  &epCDC1instate,
  &epCDC1outstate,
  2,
  NULL
};

/**
 * @brief   IN EP2 state.
 */
static USBInEndpointState epCDC2instate;

/**
 * @brief   EP2 initialization structure (IN only).
 */
static const USBEndpointConfig epCDC2config = {
  USB_EP_MODE_TYPE_INTR,
  NULL,
  sduInterruptTransmitted,
  NULL,
  0x0010,
  0x0000,
  &epCDC2instate,
  NULL,
  1,
  NULL
};




/*
 * Handles the USB driver global events.
 */
static void usb_event(USBDriver *usbp, usbevent_t event) {
    USBMassStorageDriver *msdp = (USBMassStorageDriver *)usbp->USBD_PARAM_NAME;
  switch (event) {
  case USB_EVENT_RESET:
    msdp->reconfigured_or_reset_event = TRUE;
    return;
  case USB_EVENT_ADDRESS:
    return;
  case USB_EVENT_CONFIGURED:
    chSysLockFromIsr();
    msdp->reconfigured_or_reset_event = TRUE;
    usbInitEndpointI(usbp, msdp->ms_ep_number, &epDataConfig);

    /* Enables the endpoints specified into the configuration.
     Note, this callback is invoked from an ISR so I-Class functions
     must be used.*/
    usbInitEndpointI(usbp, USB_CDC_DATA_REQUEST_EP, &epCDC1config);
    usbInitEndpointI(usbp, USB_CDC_INTERRUPT_REQUEST_EP, &epCDC2config);
    /* Resetting the state of the CDC subsystem.*/
    sduConfigureHookI(usbp);

    /* Initialize the thread */
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

bool_t msdCdcRequestsHook(USBDriver *usbp) {
  bool_t r;
  r = msdRequestsHook(usbp);
  if( r ) {
    return(r);
  }

  r = sduRequestsHook(usbp);

  return(r);
}

const USBConfig msd_usb_config = {
    usb_event,
    get_descriptor,
    msdCdcRequestsHook,
    NULL
};




















