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
 * @file    templates/usb_lld.h
 * @brief   USB Driver subsystem low level driver header template.
 *
 * @addtogroup USB
 * @{
 */

#ifndef _USB_LLD_H_
#define _USB_LLD_H_

#if HAL_USE_USB || defined(__DOXYGEN__)

#include "stm32_usb.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   USB1 driver enable switch.
 * @details If set to @p TRUE the support for USB1 is included.
 * @note    The default is @p TRUE.
 */
#if !defined(STM32_USB_USE_USB1) || defined(__DOXYGEN__)
#define STM32_USB_USE_USB1                  TRUE
#endif

/**
 * @brief   USB1 interrupt priority level setting.
 */
#if !defined(STM32_USB_USB1_HP_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_USB_USB1_HP_IRQ_PRIORITY      6
#endif

/**
 * @brief   USB1 interrupt priority level setting.
 */
#if !defined(STM32_USB_USB1_LP_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_USB_USB1_LP_IRQ_PRIORITY      14
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if STM32_USB_USE_USB1 && !STM32_HAS_USB
#error "USB not present in the selected device"
#endif

#if !STM32_USB_USE_USB1
#error "USB driver activated but no USB peripheral assigned"
#endif

#if STM32_USBCLK != 48000000
#error "the USB driver requires a 48MHz clock"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

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
 * @brief   Endpoint type.
 */
typedef enum {
  EP_TYPE_CTRL = 0,                     /**< Control endpoint.              */
  EP_TYPE_ISOC = 1,                     /**< Isochronous endpoint.          */
  EP_TYPE_BULK = 2,                     /**< Bulk endpoint.                 */
  EP_TYPE_INTR = 3                      /**< Interrupt endpoint.            */
} usb_ep_type_t;

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
 * @brief   Type of a structure representing an USB driver.
 */
typedef struct USBDriver USBDriver;

/**
 * @brief   Type of an USB notification callback.
 *
 * @param[in] usbp      pointer to the @p USBDriver object triggering the
 *                      callback
 * @param[in] event     event type
 */
typedef void (*usbcallback_t)(USBDriver *usbp, usbevent_t event);

/**
 * @brief   Type of an USB endpoint callback.
 *
 * @param[in] usbp      pointer to the @p USBDriver object triggering the
 *                      callback
 * @param[in] ep        endpoint number
 */
typedef void (*usbepcallback_t)(USBDriver *usbp, uint32_t ep);

/**
 * @brief   Type of an USB descriptor-retrieving callback.
 */
typedef const USBDescriptor * (*usbgetdescriptor_t)(USBDriver *usbp,
                                                    uint8_t dtype,
                                                    uint8_t dindex);

/**
 * @brief   Type of an USB Endpoint configuration structure.
 * @note    Platform specific restrictions may apply to endpoints.
 */
typedef struct {
  /**
   * @brief   IN endpoint notification callback.
   */
  usbepcallback_t               uepc_in_cb;
  /**
   * @brief   OUT endpoint notification callback.
   */
  usbepcallback_t               uepc_out_cb;
  /**
   * @brief   Endpoint address.
   */
  uint32_t                      uepc_addr;
  /**
   * @brief   Endpoint buffer size.
   * @note    This size is allocated twice for:
   *          - Isochronous endpoints.
   *          - Double buffered bulk endpoint.
   *          - Endpoints with both IN and OUT directions enabled.
   *          .
   */
  uint32_t                      uepc_size;
  /* End of the mandatory fields.*/
  /**
   * @brief   EPxR register initialization value.
   * @note    Do not specify the EA field, leave it to zero.
   */
  uint16_t                      uepc_epr;
  /**
   * @brief   Endpoint buffer address as offset in the PMA.
   */
  uint16_t                      uepc_offset;
} USBEndpointConfig;

/**
 * @brief   Type of an USB driver configuration structure.
 * @note    It could be empty on some architectures.
 */
typedef struct {
  /**
   * @brief   USB events callback.
   * @details This callback is invoked when the USB driver changes state
   *          because an external event.
   */
  usbcallback_t                 uc_callback;
  /**
   * @brief   Device descriptor for this USB device.
   */
  USBDescriptor                 uc_dev_descriptor;
  /**
   * @brief   Device GET_DESCRIPTOR request callback.
   * @note    This callback is mandatory and cannot be set to @p NULL.
   */
  usbgetdescriptor_t            uc_get_descriptor;
  /* End of the mandatory fields.*/
} USBConfig;

/**
 * @brief   Structure representing an USB driver.
 */
struct USBDriver {
  /**
   * @brief   Driver state.
   */
  usbstate_t                    usb_state;
  /**
   * @brief   Current configuration data.
   */
  const USBConfig               *usb_config;
  /**
   * @brief   Active endpoint descriptors.
   */
  const USBEndpointConfig       *usb_epc[USB_ENDOPOINTS_NUMBER + 1];
  /**
   * @brief   Endpoint 0 state.
   */
  usbep0state_t                 usb_ep0state;
  /**
   * @brief   Next position in endpoint 0 buffer.
   */
  uint8_t                       *usb_ep0next;
  /**
   * @brief   Buffer for endpoint 0 transactions.
   */
  uint8_t                       usb_ep0buf[USB_EP0_BUFSIZE];
  /* End of the mandatory fields.*/
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if STM32_USB_USE_USB1 && !defined(__DOXYGEN__)
extern USBDriver USBD1;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void usb_lld_init(void);
  void usb_lld_start(USBDriver *usbp);
  void usb_lld_stop(USBDriver *usbp);
  void usb_lld_reset(USBDriver *usbp);
  void usb_lld_ep_open(USBDriver *usbp, const USBEndpointConfig *epcp);
  size_t usb_lld_read(USBDriver *usbp, uint32_t ep, uint8_t *buf, size_t n);
  void usb_lld_write(USBDriver *usbp, uint32_t ep, const uint8_t *buf, size_t n);
  void usb_lld_stall_out(USBDriver *usbp, uint32_t ep);
  void usb_lld_stall_in(USBDriver *usbp, uint32_t ep);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_USB */

#endif /* _USB_LLD_H_ */

/** @} */
