/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    serial_usb.c
 * @brief   Serial over USB Driver code.
 *
 * @addtogroup SERIAL_USB
 * @{
 */

#include "hal.h"

#if HAL_USE_SERIAL_USB || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*
 * Current Line Coding.
 */
static cdc_linecoding_t linecoding = {
  {0x00, 0x96, 0x00, 0x00},             /* 38400.                           */
  LC_STOP_1, LC_PARITY_NONE, 8
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*
 * Interface implementation.
 */

static size_t write(void *ip, const uint8_t *bp, size_t n) {

  return oqWriteTimeout(&((SerialUSBDriver *)ip)->oqueue, bp,
                        n, TIME_INFINITE);
}

static size_t read(void *ip, uint8_t *bp, size_t n) {

  return iqReadTimeout(&((SerialUSBDriver *)ip)->iqueue, bp,
                       n, TIME_INFINITE);
}

static msg_t put(void *ip, uint8_t b) {

  return oqPutTimeout(&((SerialUSBDriver *)ip)->oqueue, b, TIME_INFINITE);
}

static msg_t get(void *ip) {

  return iqGetTimeout(&((SerialUSBDriver *)ip)->iqueue, TIME_INFINITE);
}

static msg_t putt(void *ip, uint8_t b, systime_t timeout) {

  return oqPutTimeout(&((SerialUSBDriver *)ip)->oqueue, b, timeout);
}

static msg_t gett(void *ip, systime_t timeout) {

  return iqGetTimeout(&((SerialUSBDriver *)ip)->iqueue, timeout);
}

static size_t writet(void *ip, const uint8_t *bp, size_t n, systime_t time) {

  return oqWriteTimeout(&((SerialUSBDriver *)ip)->oqueue, bp, n, time);
}

static size_t readt(void *ip, uint8_t *bp, size_t n, systime_t time) {

  return iqReadTimeout(&((SerialUSBDriver *)ip)->iqueue, bp, n, time);
}

static const struct SerialUSBDriverVMT vmt = {
  write, read, put, get,
  putt, gett, writet, readt
};

/**
 * @brief   Notification of data removed from the input queue.
 *
 * @param[in] qp        the queue pointer.
 */
static void inotify(io_queue_t *qp) {
  size_t n, maxsize;
  SerialUSBDriver *sdup = qGetLink(qp);

  /* If the USB driver is not in the appropriate state then transactions
     must not be started.*/
  if ((usbGetDriverStateI(sdup->config->usbp) != USB_ACTIVE) ||
      (sdup->state != SDU_READY))
    return;

  /* If there is in the queue enough space to hold at least one packet and
     a transaction is not yet started then a new transaction is started for
     the available space.*/
  maxsize = sdup->config->usbp->epc[sdup->config->bulk_out]->out_maxsize;
  if (!usbGetReceiveStatusI(sdup->config->usbp, sdup->config->bulk_out) &&
      ((n = iqGetEmptyI(&sdup->iqueue)) >= maxsize)) {
    osalSysUnlock();

    n = (n / maxsize) * maxsize;
    usbPrepareQueuedReceive(sdup->config->usbp,
                            sdup->config->bulk_out,
                            &sdup->iqueue, n);

    osalSysLock();
    usbStartReceiveI(sdup->config->usbp, sdup->config->bulk_out);
  }
}

/**
 * @brief   Notification of data inserted into the output queue.
 *
 * @param[in] qp        the queue pointer.
 */
static void onotify(io_queue_t *qp) {
  size_t n;
  SerialUSBDriver *sdup = qGetLink(qp);

  /* If the USB driver is not in the appropriate state then transactions
     must not be started.*/
  if ((usbGetDriverStateI(sdup->config->usbp) != USB_ACTIVE) ||
      (sdup->state != SDU_READY))
    return;

  /* If there is not an ongoing transaction and the output queue contains
     data then a new transaction is started.*/
  if (!usbGetTransmitStatusI(sdup->config->usbp, sdup->config->bulk_in) &&
      ((n = oqGetFullI(&sdup->oqueue)) > 0)) {
    osalSysUnlock();

    usbPrepareQueuedTransmit(sdup->config->usbp,
                             sdup->config->bulk_in,
                             &sdup->oqueue, n);

    osalSysLock();
    usbStartTransmitI(sdup->config->usbp, sdup->config->bulk_in);
  }
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Serial Driver initialization.
 * @note    This function is implicitly invoked by @p halInit(), there is
 *          no need to explicitly initialize the driver.
 *
 * @init
 */
void sduInit(void) {
}

/**
 * @brief   Initializes a generic full duplex driver object.
 * @details The HW dependent part of the initialization has to be performed
 *          outside, usually in the hardware initialization code.
 *
 * @param[out] sdup     pointer to a @p SerialUSBDriver structure
 *
 * @init
 */
void sduObjectInit(SerialUSBDriver *sdup) {

  sdup->vmt = &vmt;
  osalEventObjectInit(&sdup->event);
  sdup->state = SDU_STOP;
  iqObjectInit(&sdup->iqueue, sdup->ib, SERIAL_USB_BUFFERS_SIZE, inotify, sdup);
  oqObjectInit(&sdup->oqueue, sdup->ob, SERIAL_USB_BUFFERS_SIZE, onotify, sdup);
}

/**
 * @brief   Configures and starts the driver.
 *
 * @param[in] sdup      pointer to a @p SerialUSBDriver object
 * @param[in] config    the serial over USB driver configuration
 *
 * @api
 */
void sduStart(SerialUSBDriver *sdup, const SerialUSBConfig *config) {
  USBDriver *usbp = config->usbp;

  osalDbgCheck(sdup != NULL);

  osalSysLock();
  osalDbgAssert((sdup->state == SDU_STOP) || (sdup->state == SDU_READY),
                "invalid state");
  usbp->in_params[config->bulk_in - 1]   = sdup;
  usbp->out_params[config->bulk_out - 1] = sdup;
  usbp->in_params[config->int_in - 1]    = sdup;
  sdup->config = config;
  sdup->state = SDU_READY;
  osalSysUnlock();
}

/**
 * @brief   Stops the driver.
 * @details Any thread waiting on the driver's queues will be awakened with
 *          the message @p Q_RESET.
 *
 * @param[in] sdup      pointer to a @p SerialUSBDriver object
 *
 * @api
 */
void sduStop(SerialUSBDriver *sdup) {
  USBDriver *usbp = sdup->config->usbp;

  osalDbgCheck(sdup != NULL);

  osalSysLock();

  osalDbgAssert((sdup->state == SDU_STOP) || (sdup->state == SDU_READY),
                "invalid state");

  /* Driver in stopped state.*/
  usbp->in_params[sdup->config->bulk_in - 1]   = NULL;
  usbp->out_params[sdup->config->bulk_out - 1] = NULL;
  usbp->in_params[sdup->config->int_in - 1]    = NULL;
  sdup->state = SDU_STOP;

  /* Queues reset in order to signal the driver stop to the application.*/
  chnAddFlagsI(sdup, CHN_DISCONNECTED);
  iqResetI(&sdup->iqueue);
  iqResetI(&sdup->oqueue);
  osalOsRescheduleS();

  osalSysUnlock();
}

/**
 * @brief   USB device configured handler.
 *
 * @param[in] sdup      pointer to a @p SerialUSBDriver object
 *
 * @iclass
 */
void sduConfigureHookI(SerialUSBDriver *sdup) {
  USBDriver *usbp = sdup->config->usbp;

  iqResetI(&sdup->iqueue);
  oqResetI(&sdup->oqueue);
  chnAddFlagsI(sdup, CHN_CONNECTED);

  /* Starts the first OUT transaction immediately.*/
  usbPrepareQueuedReceive(usbp, sdup->config->bulk_out, &sdup->iqueue,
                          usbp->epc[sdup->config->bulk_out]->out_maxsize);
  usbStartReceiveI(usbp, sdup->config->bulk_out);
}

/**
 * @brief   Default requests hook.
 * @details Applications wanting to use the Serial over USB driver can use
 *          this function as requests hook in the USB configuration.
 *          The following requests are emulated:
 *          - CDC_GET_LINE_CODING.
 *          - CDC_SET_LINE_CODING.
 *          - CDC_SET_CONTROL_LINE_STATE.
 *          .
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @return              The hook status.
 * @retval TRUE         Message handled internally.
 * @retval FALSE        Message not handled.
 */
bool sduRequestsHook(USBDriver *usbp) {

  if ((usbp->setup[0] & USB_RTYPE_TYPE_MASK) == USB_RTYPE_TYPE_CLASS) {
    switch (usbp->setup[1]) {
    case CDC_GET_LINE_CODING:
      usbSetupTransfer(usbp, (uint8_t *)&linecoding, sizeof(linecoding), NULL);
      return TRUE;
    case CDC_SET_LINE_CODING:
      usbSetupTransfer(usbp, (uint8_t *)&linecoding, sizeof(linecoding), NULL);
      return TRUE;
    case CDC_SET_CONTROL_LINE_STATE:
      /* Nothing to do, there are no control lines.*/
      usbSetupTransfer(usbp, NULL, 0, NULL);
      return TRUE;
    default:
      return FALSE;
    }
  }
  return FALSE;
}

/**
 * @brief   Default data transmitted callback.
 * @details The application must use this function as callback for the IN
 *          data endpoint.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 */
void sduDataTransmitted(USBDriver *usbp, usbep_t ep) {
  size_t n;
  SerialUSBDriver *sdup = usbp->in_params[ep - 1];

  if (sdup == NULL)
    return;

  osalSysLockFromISR();
  chnAddFlagsI(sdup, CHN_OUTPUT_EMPTY);

  if ((n = oqGetFullI(&sdup->oqueue)) > 0) {
    /* The endpoint cannot be busy, we are in the context of the callback,
       so it is safe to transmit without a check.*/
    osalSysUnlockFromISR();

    usbPrepareQueuedTransmit(usbp, ep, &sdup->oqueue, n);

    osalSysLockFromISR();
    usbStartTransmitI(usbp, ep);
  }
  else if ((usbp->epc[ep]->in_state->txsize > 0) &&
           !(usbp->epc[ep]->in_state->txsize &
             (usbp->epc[ep]->in_maxsize - 1))) {
    /* Transmit zero sized packet in case the last one has maximum allowed
       size. Otherwise the recipient may expect more data coming soon and
       not return buffered data to app. See section 5.8.3 Bulk Transfer
       Packet Size Constraints of the USB Specification document.*/
    osalSysUnlockFromISR();

    usbPrepareQueuedTransmit(usbp, ep, &sdup->oqueue, 0);

    osalSysLockFromISR();
    usbStartTransmitI(usbp, ep);
  }

  osalSysUnlockFromISR();
}

/**
 * @brief   Default data received callback.
 * @details The application must use this function as callback for the OUT
 *          data endpoint.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 */
void sduDataReceived(USBDriver *usbp, usbep_t ep) {
  size_t n, maxsize;
  SerialUSBDriver *sdup = usbp->out_params[ep - 1];

  if (sdup == NULL)
    return;

  osalSysLockFromISR();
  chnAddFlagsI(sdup, CHN_INPUT_AVAILABLE);

  /* Writes to the input queue can only happen when there is enough space
     to hold at least one packet.*/
  maxsize = usbp->epc[ep]->out_maxsize;
  if ((n = iqGetEmptyI(&sdup->iqueue)) >= maxsize) {
    /* The endpoint cannot be busy, we are in the context of the callback,
       so a packet is in the buffer for sure.*/
    osalSysUnlockFromISR();

    n = (n / maxsize) * maxsize;
    usbPrepareQueuedReceive(usbp, ep, &sdup->iqueue, n);

    osalSysLockFromISR();
    usbStartReceiveI(usbp, ep);
  }

  osalSysUnlockFromISR();
}

/**
 * @brief   Default data received callback.
 * @details The application must use this function as callback for the IN
 *          interrupt endpoint.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 */
void sduInterruptTransmitted(USBDriver *usbp, usbep_t ep) {

  (void)usbp;
  (void)ep;
}

#endif /* HAL_USE_SERIAL */

/** @} */
