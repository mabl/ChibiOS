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
 * @file    usb.c
 * @brief   USB Driver code.
 *
 * @addtogroup USB
 * @{
 */

#include "ch.h"
#include "hal.h"
#include "usb.h"

#if HAL_USE_USB || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   USB Driver initialization.
 * @note    This function is implicitly invoked by @p halInit(), there is
 *          no need to explicitly initialize the driver.
 *
 * @init
 */
void usbInit(void) {

  usb_lld_init();
}

/**
 * @brief   Initializes the standard part of a @p USBDriver structure.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 *
 * @init
 */
void usbObjectInit(USBDriver *usbp) {

  usbp->usb_state    = USB_STOP;
  usbp->usb_config   = NULL;
}

/**
 * @brief   Configures and activates the USB peripheral.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] config    pointer to the @p USBConfig object
 *
 * @api
 */
void usbStart(USBDriver *usbp, const USBConfig *config) {

  chDbgCheck((usbp != NULL) && (config != NULL), "usbStart");

  chSysLock();
  chDbgAssert((usbp->usb_state == USB_STOP) || (usbp->usb_state == USB_READY),
              "usbStart(), #1",
              "invalid state");
  usbp->usb_config = config;
  usb_lld_start(usbp);
  usbp->usb_state = USB_READY;
  chSysUnlock();
}

/**
 * @brief   Deactivates the USB peripheral.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 *
 * @api
 */
void usbStop(USBDriver *usbp) {

  chDbgCheck(usbp != NULL, "usbStop");

  chSysLock();
  chDbgAssert((usbp->usb_state == USB_STOP) || (usbp->usb_state == USB_READY),
              "usbStop(), #1",
              "invalid state");
  usb_lld_stop(usbp);
  usbp->usb_state = USB_STOP;
  chSysUnlock();
}

/**
 * @brief   USB reset routine.
 *
 * @param[in] usbp      pointer to the @p USBDriver object triggering the
 *                      callback
 *
 * @notapi
 */
void usb_reset(USBDriver *usbp) {
  int32_t i;

  /* Invalidates all endpoints into the USBDriver structure.*/
  for (i = 0; i < USB_ENDOPOINTS_NUMBER + 1; i++)
    usbp->usb_epc[i] = NULL;
  
  /* EP0 state machine initialization.*/
  usbp->usb_ep0state = USB_EP0_SETUP_DATA;
  usbp->usb_ep0next = usbp->usb_ep0buf;

  /* Low level reset.*/
  usb_lld_reset(usbp);
}

const USBDescriptor *usb_get_descriptor(USBDriver *usbp,
                                        uint8_t dtype,
                                        uint8_t dindex) {
  
  /* The device descriptor is a special case because it is statically
     assigned to the USB driver configuration. Just returning the
     pointer.*/
  if (dtype == USB_DESCRIPTOR_DEVICE)
    return &usbp->usb_config->uc_dev_descriptor;
  
  /* Any other descriptor must be provided by the application in
     order to provide an interface for complicated devices with
     multiple configurations or languages.*/
  return usbp->usb_config->uc_get_descriptor(usbp, dtype, dindex);
}

#endif /* HAL_USE_USB */

/** @} */
