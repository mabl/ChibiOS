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

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Number of the available endpoints.
 * @details This value does not include the endpoint 0 which is always present.
 */
#define USB_ENDOPOINTS_NUMBER           7

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   Maximum number of strings in an USB configuration.
 */
#if !defined(USB_MAX_STRINGS) || defined(__DOXYGEN__)
#define USB_MAX_STRINGS                 8
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

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
} USB_LLD_ep_type_t;

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
 * @brief   USB notification callback type.
 *
 * @param[in] usbp      pointer to the @p USBDriver object triggering the
 *                      callback
 * @param[in] event     event type
 */
typedef void (*usbcallback_t)(USBDriver *usbp, usbevent_t event);

/**
 * @brief   USB endpoint callback type.
 *
 * @param[in] uepp      pointer to the @p USBEndpoint object triggering the
 *                      callback
 */
typedef void (*usbepcallback_t)(USBEndpoint *uepp);

/**
 * @brief   USB device request handling callback.
 */
typedef void (*usbdevrequest_t(void);

/**
 * @brief   USB Endpoint configuration structure.
 * @note    Platform specific restrictions may apply to endpoints.
 */
typedef struct {
  /**
   * @brief   Endpoint notification callback.
   */
  usbepcallback_t               uepc_callback;
} USBEndpointConfig;

typedef struct {

} USBEndpoint;

/**
 * @brief   Driver configuration structure.
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
  /**
   * @brief Input endpoints configurations.
   */
  USBEndpointConfig             uc_in_ep_cfg[USB_ENDOPOINTS_NUMBER];
  /**
   * @brief Output endpoints configurations.
   */
  USBEndpointConfig             uc_out_ep_cfg[USB_ENDOPOINTS_NUMBER];
} USBConfig;

/**
 * @brief   Structure representing an USB driver.
 */
struct USBDriver {
  /**
   * @brief Driver state.
   */
  usbstate_t                usb_state;
  /**
   * @brief Current configuration data.
   */
  const USBConfig           *usb_config;
  /* End of the mandatory fields.*/
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void usb_lld_init(void);
  void usb_lld_start(USBDriver *usbp);
  void usb_lld_stop(USBDriver *usbp);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_USB */

#endif /* _USB_LLD_H_ */

/** @} */
