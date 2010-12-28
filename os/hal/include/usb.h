/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

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

/**
 * @file    usb.h
 * @brief   USB Driver macros and structures.
 *
 * @addtogroup USB
 * @{
 */

#ifndef _USB_H_
#define _USB_H_

#if HAL_USE_USB || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

#define USB_REQ_GET_STATUS                  0
#define USB_REQ_CLEAR_FEATURE               1
#define USB_REQ_SET_FEATURE                 3
#define USB_REQ_SET_ADDRESS                 5
#define USB_REQ_GET_DESCRIPTOR              6
#define USB_REQ_SET_DESCRIPTOR              7
#define USB_REQ_GET_CONFIGURATION           8
#define USB_REQ_SET_CONFIGURATION           9
#define USB_REQ_GET_INTERFACE               10
#define USB_REQ_SET_INTERFACE               11
#define USB_REQ_SYNCH_FRAME                 12

#define USB_REQ_TYPE_DEVICE                 0
#define USB_REQ_TYPE_INTERFACE              1
#define USB_REQ_TYPE_ENDPOINT               3

#define USB_DESCRIPTOR_DEVICE               1
#define USB_DESCRIPTOR_CONFIGURATION        2
#define USB_DESCRIPTOR_STRING               3
#define USB_DESCRIPTOR_INTERFACE            4
#define USB_DESCRIPTOR_ENDPOINT             5
#define USB_DESCRIPTOR_DEVICE_QUALIFIER     6
#define USB_DESCRIPTOR_OTHER_SPEED_CFG      7
#define USB_DESCRIPTOR_INTERFACE_POWER      8

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   USB endpoint zero buffer size.
 * @note    The default is 64.
 */
#if !defined(USB_EP0_BUFSIZE) || defined(__DOXYGEN__)
#define USB_EP0_BUFSIZE                     64
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a structure representing an USB driver.
 */
typedef struct USBDriver USBDriver;

/**
 * @brief   Driver state machine possible states.
 */
typedef enum {
  USB_UNINIT = 0,                       /**< Not initialized.               */
  USB_STOP   = 1,                       /**< Stopped.                       */
  USB_READY  = 2,                       /**< Ready, before enumeration.     */
  USB_ACTIVE = 3,                       /**< Active, after enumeration.     */
} usbstate_t;

/**
 * @brief   Endpoint type.
 */
typedef enum {
  EP_TYPE_CTRL = 0,                     /**< Control endpoint.              */
  EP_TYPE_ISOC = 1,                     /**< Isochronous endpoint.          */
  EP_TYPE_BULK = 2,                     /**< Bulk endpoint.                 */
  EP_TYPE_INTR = 3                      /**< Interrupt endpoint.            */
} usbeptype_t;

/**
 * @brief   Endpoint zero state machine states.
 */
typedef enum {
  USB_EP0_WAITING_SETUP,                /**< Waiting for SETUP data.        */
  USB_EP0_TX,                           /**< Trasmitting.                   */
  USB_EP0_WAITING_STS,                  /**< Waiting status.                */
  USB_EP0_RX,                           /**< Receiving.                     */
  USB_EP0_SENDING_STS,                  /**< Sending status.                */
  USB_EP0_FATAL                         /**< Final state because fatal
                                             error.                         */
} usbep0state_t;

/**
 * @brief   Enumeration of the possible USB events.
 */
typedef enum {
  USB_EVENT_RESET = 0,                  /**< Driver has been reset by host. */
  USB_EVENT_ADDRESS = 1,                /**< Address assigned.              */
  USB_EVENT_CONFIGURED = 2,             /**< Configuration selected.        */
  USB_EVENT_SUSPEND = 3,                /**< Entering suspend mode.         */
  USB_EVENT_RESUME = 4,                 /**< Leaving suspend mode.          */
} usbevent_t;

/**
 * @brief   USB descriptor.
 */
typedef struct {
  /**
   * @brief   Descriptor size in unicode characters.
   */
  size_t                        ud_size;
  /**
   * @brief   Pointer to the descriptor.
   */
  const uint8_t                 *ud_string;
} USBDescriptor;

/**
 * @brief   Type of an USB notification callback.
 *
 * @param[in] usbp      pointer to the @p USBDriver object triggering the
 *                      callback
 * @param[in] event     event type
 */
typedef void (*usbcallback_t)(USBDriver *usbp, usbevent_t event);

/**
 * @brief   Type of an USB descriptor-retrieving callback.
 */
typedef const USBDescriptor * (*usbgetdescriptor_t)(USBDriver *usbp,
                                                    uint8_t dtype,
                                                    uint8_t dindex,
                                                    uint16_t lang);

#include "usb_lld.h"

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void usbInit(void);
  void usbObjectInit(USBDriver *usbp);
  void usbStart(USBDriver *usbp, const USBConfig *config);
  void usbStop(USBDriver *usbp);
  void usb_reset(USBDriver *usbp);
  const USBDescriptor *usb_get_descriptor(USBDriver *usbp,
                                          uint8_t dtype,
                                          uint8_t dindex,
                                          uint16_t lang);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_USB */

#endif /* _USB_H_ */

/** @} */
