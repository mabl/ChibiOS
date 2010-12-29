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

/**
 * @brief  SET ADDRESS transaction callback.
 *
 * @param[in] usbp      pointer to the @p USBDriver object triggering the
 *                      callback
 */
void set_address(USBDriver *usbp) {

  usbp->usb_address = usbp->usb_setup[2];
  usb_lld_set_address(usbp, usbp->usb_address);
  if (usbp->usb_config->uc_state_change_cb)
    usbp->usb_config->uc_state_change_cb(usbp, USB_EVENT_ADDRESS);
  usbp->usb_state = USB_SELECTED;
}

/**
 * @brief   Starts a receive phase on the endpoint zero.
 *
 * @param[in] usbp      pointer to the @p USBDriver object triggering the
 *                      callback
 */
static void start_rx_ep0(USBDriver *usbp) {

  if (usbp->usb_ep0n > 0) {
    /* TO BE IMPLEMENTED.*/
  }
  else {
    /* Sending zero sized status packet.*/
    usb_lld_write(usbp, 0, NULL, 0);
    usbp->usb_ep0state = USB_EP0_SENDING_STS;
  }
}

/**
 * @brief   Starts a transmission phase on the endpoint zero.
 *
 * @param[in] usbp      pointer to the @p USBDriver object triggering the
 *                      callback
 */
static void start_tx_ep0(USBDriver *usbp) {

  if (usbp->usb_ep0n > 0) {
    /* The transmitted data cannot exceed the requested amount.*/
    if (usbp->usb_ep0n > usbp->usb_ep0max)
      usbp->usb_ep0n = usbp->usb_ep0max;

    /* Determines the maximum amount that can be transmitted using a
       single packet.*/
    if (usbp->usb_ep0n > usb_lld_ep0config.uepc_size)
      usbp->usb_ep0lastsize = usb_lld_ep0config.uepc_size;
    else
      usbp->usb_ep0lastsize = usbp->usb_ep0n;

    /* Starts transmission.*/
    usb_lld_write(usbp, 0, usbp->usb_ep0next, usbp->usb_ep0lastsize);
    usbp->usb_ep0state = USB_EP0_TX;
  }
  else
    usbp->usb_ep0state = USB_EP0_WAITING_STS;
}

/**
 * @brief   Standard requests handler.
 */
static bool_t handle_standard_requests(USBDriver *usbp) {

    /* Decoding the request.*/
  switch ((usbp->usb_setup[0] & USB_RTYPE_RECIPIENT_MASK) |
          (usbp->usb_setup[1] << 5)) {
  case USB_REQ_TYPE_DEVICE | (USB_REQ_GET_STATUS << 5):
    return TRUE;
  case USB_REQ_TYPE_DEVICE | (USB_REQ_CLEAR_FEATURE << 5):
    return TRUE;
  case USB_REQ_TYPE_DEVICE | (USB_REQ_SET_FEATURE << 5):
    return TRUE;
  case USB_REQ_TYPE_DEVICE | (USB_REQ_SET_ADDRESS << 5):
    /* The handling is posponed to after the status phase in order to allow
       the proper completion of the transaction.*/
    usbSetupTransfer(usbp, NULL, 0, set_address);
    return FALSE;
  case USB_REQ_TYPE_DEVICE | (USB_REQ_GET_DESCRIPTOR << 5):
    /* Handling descriptor requests from the host.*/
    {
      const USBDescriptor *dp;

      if (usbp->usb_setup[3] == USB_DESCRIPTOR_DEVICE) {
        /* The device descriptor is a special case because it is statically
           assigned to the USB driver configuration. Just returning the
           pointer.*/
        dp = &usbp->usb_config->uc_dev_descriptor;
      }
      else {
        /* Any other descriptor must be provided by the application in
           order to provide an interface for complicated devices with
           multiple configurations or languages.*/
        dp = usbp->usb_config->uc_get_descriptor_cb(usbp,
                                                    usbp->usb_setup[3],
                                                    usbp->usb_setup[2],
                                                    usb_lld_fetch_word(&usbp->usb_setup[4]));
      }
      if (dp == NULL)
        return TRUE;

      usbSetupTransfer(usbp, dp->ud_string, dp->ud_size, NULL);
      return FALSE;
    }
  case USB_REQ_TYPE_DEVICE | (USB_REQ_SET_DESCRIPTOR << 5):
    return TRUE;
  case USB_REQ_TYPE_DEVICE | (USB_REQ_GET_CONFIGURATION << 5):
    usbSetupTransfer(usbp, &usbp->usb_configuration, 1, NULL);
    return FALSE;
  case USB_REQ_TYPE_DEVICE | (USB_REQ_SET_CONFIGURATION << 5):
    /* Handling configuration selection from the host.*/
    usbp->usb_configuration = usbp->usb_setup[2];
    if (usbp->usb_config->uc_state_change_cb)
      usbp->usb_config->uc_state_change_cb(usbp, USB_EVENT_CONFIGURED);
    usbSetupTransfer(usbp, NULL, 0, NULL);
    usbp->usb_state = USB_ACTIVE;
    return FALSE;
  case USB_REQ_TYPE_INTERFACE | (USB_REQ_GET_STATUS << 5):
    return TRUE;
  case USB_REQ_TYPE_INTERFACE | (USB_REQ_CLEAR_FEATURE << 5):
    return TRUE;
  case USB_REQ_TYPE_INTERFACE | (USB_REQ_SET_FEATURE << 5):
    return TRUE;
  case USB_REQ_TYPE_INTERFACE | (USB_REQ_GET_INTERFACE << 5):
    return TRUE;
  case USB_REQ_TYPE_INTERFACE | (USB_REQ_SET_INTERFACE << 5):
    return TRUE;
  case USB_REQ_TYPE_ENDPOINT | (USB_REQ_GET_STATUS << 5):
    return TRUE;
  case USB_REQ_TYPE_ENDPOINT | (USB_REQ_CLEAR_FEATURE << 5):
    return TRUE;
  case USB_REQ_TYPE_ENDPOINT | (USB_REQ_SET_FEATURE << 5):
    return TRUE;
  case USB_REQ_TYPE_ENDPOINT | (USB_REQ_SYNCH_FRAME << 5):
    return TRUE;
  default:
    return TRUE;
  }
}

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
 * @brief   Activates an endpoint.
 *
 * @param[in] usbp      pointer to the @p USBDriver object triggering the
 *                      callback
 * @param[in] epcp      the endpoint configuration
 *
 * @notapi
 */
void usbEPOpen(USBDriver *usbp, const USBEndpointConfig *epcp) {

  chDbgAssert(usbp->usb_state == USB_READY,
              "usbEPOpen(), #1", "invalid state");

  chSysLock();
  usb_lld_ep_open(usbp, epcp);
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
void _usb_reset(USBDriver *usbp) {
  int32_t i;

  usbp->usb_state         = USB_READY;
  usbp->usb_address       = 0;
  usbp->usb_configuration = 0;

  /* Invalidates all endpoints into the USBDriver structure.*/
  for (i = 0; i < USB_ENDOPOINTS_NUMBER + 1; i++)
    usbp->usb_epc[i] = NULL;

  /* EP0 state machine initialization.*/
  usbp->usb_ep0state     = USB_EP0_WAITING_SETUP;

  /* Low level reset.*/
  usb_lld_reset(usbp);
}

/**
 * @brief   Default EP0 IN callback.
 * @details This function is used by the low level driver as default handler
 *          for EP0 IN events.
 *
 * @param[in] usbp      pointer to the @p USBDriver object triggering the
 *                      callback
 * @param[in] ep        endpoint number, always zero
 *
 * @notapi
 */
void _usb_ep0in(USBDriver *usbp, uint32_t ep) {

  switch (usbp->usb_ep0state) {
  case USB_EP0_TX:
    usbp->usb_ep0next      += usbp->usb_ep0lastsize;
    usbp->usb_ep0max       -= usbp->usb_ep0lastsize;
    usbp->usb_ep0n -= usbp->usb_ep0lastsize;

    /* The final condition is when the requested size has been transmitted or when a
       packet has been sent with size less than the maximum packet size.*/
    if ((usbp->usb_ep0max == 0) || (usbp->usb_ep0lastsize < usb_lld_ep0config.uepc_size))
      usbp->usb_ep0state = USB_EP0_WAITING_STS;
    else {
      usbp->usb_ep0lastsize = usbp->usb_ep0n > usb_lld_ep0config.uepc_size ?
                              usb_lld_ep0config.uepc_size : usbp->usb_ep0n;
      usb_lld_write(usbp, 0, usbp->usb_ep0next, usbp->usb_ep0lastsize);
    }
    return;
  case USB_EP0_SENDING_STS:
    if (usbp->usb_ep0endcb)
      usbp->usb_ep0endcb(usbp);
    usbp->usb_ep0state = USB_EP0_WAITING_SETUP;
    return;
  default:
//stall:
    usb_lld_stall_in(usbp, 0);
    usb_lld_stall_out(usbp, 0);
    if (usbp->usb_config->uc_state_change_cb)
      usbp->usb_config->uc_state_change_cb(usbp, USB_EVENT_STALLED);
    usbp->usb_ep0state = USB_EP0_FATAL;
  }
}

/**
 * @brief   Default EP0 OUT callback.
 * @details This function is used by the low level driver as default handler
 *          for EP0 OUT events.
 *
 * @param[in] usbp      pointer to the @p USBDriver object triggering the
 *                      callback
 * @param[in] ep        endpoint number, always zero
 *
 * @notapi
 */
void _usb_ep0out(USBDriver *usbp, uint32_t ep) {
  size_t n;
  uint8_t buf[1];

  switch (usbp->usb_ep0state) {
  case USB_EP0_WAITING_SETUP:
    /*
     * SETUP packet handling.
     */
    n = usb_lld_read(usbp, 0, usbp->usb_setup, 8);
    if (n != 8)
      goto stall;

    /* Invoking the appropriate requests handler (standard or user).*/
    if ((usbp->usb_setup[0] & USB_RTYPE_TYPE_MASK) == USB_RTYPE_TYPE_STANDARD) {
      if (handle_standard_requests(usbp))
        goto stall;
    }
    else {
      if ((usbp->usb_config->uc_user_request_cb) &&
          (usbp->usb_config->uc_user_request_cb(usbp)))
        goto stall;
    }

    /* Transfer preparation. The request handler must have populated
       correctly the fields usb_ep0next, usb_ep0n and usb_ep0endcb using
       the macro usbSetupTransfer().*/
    usbp->usb_ep0max = usb_lld_fetch_word(&usbp->usb_setup[6]);

    if ((usbp->usb_setup[0] & USB_RTYPE_DIR_MASK) == USB_RTYPE_DIR_DEV2HOST)
      start_tx_ep0(usbp);
    else
      start_rx_ep0(usbp);

    return;

  case USB_EP0_WAITING_STS:
    /*
     * STATUS received packet handling, it must be zero sized.
     */
    n = usb_lld_read(usbp, 0, buf, 1);
    if (n != 0)
      goto stall;
    if (usbp->usb_ep0endcb)
      usbp->usb_ep0endcb(usbp);
    usbp->usb_ep0state = USB_EP0_WAITING_SETUP;
    return;
  case USB_EP0_RX:
    goto stall;
  default:
stall:
    /* Stalled because it should never happen.*/
    usb_lld_stall_in(usbp, 0);
    usb_lld_stall_out(usbp, 0);
    if (usbp->usb_config->uc_state_change_cb)
      usbp->usb_config->uc_state_change_cb(usbp, USB_EVENT_STALLED);
    usbp->usb_ep0state = USB_EP0_FATAL;
  }
}

#endif /* HAL_USE_USB */

/** @} */
