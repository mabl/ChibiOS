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
   * @brief   Descriptor string size in unicode characters.
   */
  size_t                        ud_size;
  /**
   * @brief   Pointer to the descriptor unicode string.
   */
  uint8_t                       ud_string;
} USBDescriptor;

/**
 * @brief   Type of a structure representing an USB driver.
 */
typedef struct USBDriver USBDriver;

/**
 * @brief   Type of a structure representing an USB endpoint.
 */
typedef struct USBEndpoint USBEndpoint;

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
 * @param[in] uepp      pointer to the @p USBEndpoint object triggering the
 *                      callback
 */
typedef void (*usbepcallback_t)(USBEndpoint *uepp);

/**
 * @brief   Type of an USB device request handling callback.
 */
typedef void (*usbdevrequest_t)(void);

/**
 * @brief   Type of an USB Endpoint configuration structure.
 * @note    Platform specific restrictions may apply to endpoints.
 */
typedef struct {
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
   * @note    Only the EP_TYPE and EP_KIND bits can be specified in this
   *          field.
   */
  uint16_t                      uepc_epr;
  /**
   * @brief   EPxR STAT_TX initialization value.
   */
  uint16_t                      uepc_stattx;
  /**
   * @brief   EPxR STAT_RX initialization value.
   */
  uint16_t                      uepc_statrx;
  /**
   * @brief   Endpoint buffer address as offset in the PMA.
   */
  uint16_t                      uepc_offset;
} USBEndpointConfig;

/**
 * @brief   Endpoint descriptor.
 */
struct USBEndpoint {
  /**
   * @brief   Endpoint notification callback.
   */
  usbepcallback_t               uep_callback;
  /* End of the mandatory fields.*/
};

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
   * @brief   Device descriptor for this USB device configuration.
   */
  USBDescriptor                 uc_dev_descriptor;
  /**
   * @brief   Device requests callback.
   */
  usbdevrequest_t               uc_dev_rq_callback;
  /* End of the mandatory fields.*/
} USBConfig;

/**
 * @brief   Structure representing an USB driver.
 */
struct USBDriver {
  /**
   * @brief   Driver state.
   */
  usbstate_t                usb_state;
  /**
   * @brief   Current configuration data.
   */
  const USBConfig           *usb_config;
  /**
   * @brief   Active endpoint descriptors.
   */
  USBEndpoint               *usb_ep[USB_ENDOPOINTS_NUMBER + 1];
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
  void usb_lld_ep_open(USBDriver *usbp, const USBEndpointConfig *epcp,
                       USBEndpoint *epp, usbepcallback_t epcb);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_USB */

#endif /* _USB_LLD_H_ */

/** @} */
