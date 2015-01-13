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
 * @file    uart.c
 * @brief   UART Driver code.
 *
 * @addtogroup UART
 * @{
 */

#include "hal.h"

#if HAL_USE_UART || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   UART Driver initialization.
 * @note    This function is implicitly invoked by @p halInit(), there is
 *          no need to explicitly initialize the driver.
 *
 * @init
 */
void uartInit(void) {

  uart_lld_init();
}

/**
 * @brief   Initializes the standard part of a @p UARTDriver structure.
 *
 * @param[out] uartp    pointer to the @p UARTDriver object
 *
 * @init
 */
void uartObjectInit(UARTDriver *uartp) {

  uartp->state   = UART_STOP;
  uartp->txstate = UART_TX_IDLE;
  uartp->rxstate = UART_RX_IDLE;
  uartp->config  = NULL;
  /* Optional, user-defined initializer.*/
#if defined(UART_DRIVER_EXT_INIT_HOOK)
  UART_DRIVER_EXT_INIT_HOOK(uartp);
#endif
}

/**
 * @brief   Configures and activates the UART peripheral.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 * @param[in] config    pointer to the @p UARTConfig object
 *
 * @api
 */
void uartStart(UARTDriver *uartp, const UARTConfig *config) {

  osalDbgCheck((uartp != NULL) && (config != NULL));

  osalSysLock();
  osalDbgAssert((uartp->state == UART_STOP) || (uartp->state == UART_READY),
                "invalid state");

  uartp->config = config;
  uart_lld_start(uartp);
  uartp->state = UART_READY;
  osalSysUnlock();
}

/**
 * @brief   Deactivates the UART peripheral.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 *
 * @api
 */
void uartStop(UARTDriver *uartp) {

  osalDbgCheck(uartp != NULL);

  osalSysLock();
  osalDbgAssert((uartp->state == UART_STOP) || (uartp->state == UART_READY),
                "invalid state");

  uart_lld_stop(uartp);
  uartp->state = UART_STOP;
  uartp->txstate = UART_TX_IDLE;
  uartp->rxstate = UART_RX_IDLE;
  osalSysUnlock();
}

/**
 * @brief   Starts a transmission on the UART peripheral.
 * @note    The buffers are organized as uint8_t arrays for data sizes below
 *          or equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 * @param[in] n         number of data frames to send
 * @param[in] txbuf     the pointer to the transmit buffer
 *
 * @api
 */
void uartStartSend(UARTDriver *uartp, size_t n, const void *txbuf) {

  osalDbgCheck((uartp != NULL) && (n > 0) && (txbuf != NULL));
             
  osalSysLock();
  osalDbgAssert(uartp->state == UART_READY, "is active");
  osalDbgAssert(uartp->txstate != UART_TX_ACTIVE, "tx active");

  uart_lld_start_send(uartp, n, txbuf);
  uartp->txstate = UART_TX_ACTIVE;
  osalSysUnlock();
}

/**
 * @brief   Starts a transmission on the UART peripheral.
 * @note    The buffers are organized as uint8_t arrays for data sizes below
 *          or equal to 8 bits else it is organized as uint16_t arrays.
 * @note    This function has to be invoked from a lock zone.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 * @param[in] n         number of data frames to send
 * @param[in] txbuf     the pointer to the transmit buffer
 *
 * @iclass
 */
void uartStartSendI(UARTDriver *uartp, size_t n, const void *txbuf) {

  osalDbgCheckClassI();
  osalDbgCheck((uartp != NULL) && (n > 0) && (txbuf != NULL));
  osalDbgAssert(uartp->state == UART_READY, "is active");
  osalDbgAssert(uartp->txstate != UART_TX_ACTIVE, "tx active");

  uart_lld_start_send(uartp, n, txbuf);
  uartp->txstate = UART_TX_ACTIVE;
}

/**
 * @brief   Stops any ongoing transmission.
 * @note    Stopping a transmission also suppresses the transmission callbacks.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 *
 * @return              The number of data frames not transmitted by the
 *                      stopped transmit operation.
 * @retval 0            There was no transmit operation in progress.
 *
 * @api
 */
size_t uartStopSend(UARTDriver *uartp) {
  size_t n;

  osalDbgCheck(uartp != NULL);

  osalSysLock();
  osalDbgAssert(uartp->state == UART_READY, "not active");

  if (uartp->txstate == UART_TX_ACTIVE) {
    n = uart_lld_stop_send(uartp);
    uartp->txstate = UART_TX_IDLE;
  }
  else
    n = 0;
  osalSysUnlock();
  return n;
}

/**
 * @brief   Stops any ongoing transmission.
 * @note    Stopping a transmission also suppresses the transmission callbacks.
 * @note    This function has to be invoked from a lock zone.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 *
 * @return              The number of data frames not transmitted by the
 *                      stopped transmit operation.
 * @retval 0            There was no transmit operation in progress.
 *
 * @iclass
 */
size_t uartStopSendI(UARTDriver *uartp) {

  osalDbgCheckClassI();
  osalDbgCheck(uartp != NULL);
  osalDbgAssert(uartp->state == UART_READY, "not active");

  if (uartp->txstate == UART_TX_ACTIVE) {
    size_t n = uart_lld_stop_send(uartp);
    uartp->txstate = UART_TX_IDLE;
    return n;
  }
  return 0;
}

/**
 * @brief   Starts a receive operation on the UART peripheral.
 * @note    The buffers are organized as uint8_t arrays for data sizes below
 *          or equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 * @param[in] n         number of data frames to send
 * @param[in] rxbuf     the pointer to the receive buffer
 *
 * @api
 */
void uartStartReceive(UARTDriver *uartp, size_t n, void *rxbuf) {

  osalDbgCheck((uartp != NULL) && (n > 0) && (rxbuf != NULL));

  osalSysLock();
  osalDbgAssert(uartp->state == UART_READY, "is active");
  osalDbgAssert(uartp->rxstate != UART_RX_ACTIVE, "rx active");

  uart_lld_start_receive(uartp, n, rxbuf);
  uartp->rxstate = UART_RX_ACTIVE;
  osalSysUnlock();
}

/**
 * @brief   Starts a receive operation on the UART peripheral.
 * @note    The buffers are organized as uint8_t arrays for data sizes below
 *          or equal to 8 bits else it is organized as uint16_t arrays.
 * @note    This function has to be invoked from a lock zone.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 * @param[in] n         number of data frames to send
 * @param[out] rxbuf    the pointer to the receive buffer
 *
 * @iclass
 */
void uartStartReceiveI(UARTDriver *uartp, size_t n, void *rxbuf) {

  osalDbgCheckClassI();
  osalDbgCheck((uartp != NULL) && (n > 0) && (rxbuf != NULL));
  osalDbgAssert(uartp->state == UART_READY, "is active");
  osalDbgAssert(uartp->rxstate != UART_RX_ACTIVE, "rx active");

  uart_lld_start_receive(uartp, n, rxbuf);
  uartp->rxstate = UART_RX_ACTIVE;
}

/**
 * @brief   Stops any ongoing receive operation.
 * @note    Stopping a receive operation also suppresses the receive callbacks.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 *
 * @return              The number of data frames not received by the
 *                      stopped receive operation.
 * @retval 0            There was no receive operation in progress.
 *
 * @api
 */
size_t uartStopReceive(UARTDriver *uartp) {
  size_t n;

  osalDbgCheck(uartp != NULL);

  osalSysLock();
  osalDbgAssert(uartp->state == UART_READY, "not active");

  if (uartp->rxstate == UART_RX_ACTIVE) {
    n = uart_lld_stop_receive(uartp);
    uartp->rxstate = UART_RX_IDLE;
  }
  else
    n = 0;
  osalSysUnlock();
  return n;
}

/**
 * @brief   Stops any ongoing receive operation.
 * @note    Stopping a receive operation also suppresses the receive callbacks.
 * @note    This function has to be invoked from a lock zone.
 *
 * @param[in] uartp      pointer to the @p UARTDriver object
 *
 * @return              The number of data frames not received by the
 *                      stopped receive operation.
 * @retval 0            There was no receive operation in progress.
 *
 * @iclass
 */
size_t uartStopReceiveI(UARTDriver *uartp) {

  osalDbgCheckClassI();
  osalDbgCheck(uartp != NULL);
  osalDbgAssert(uartp->state == UART_READY, "not active");

  if (uartp->rxstate == UART_RX_ACTIVE) {
    size_t n = uart_lld_stop_receive(uartp);
    uartp->rxstate = UART_RX_IDLE;
    return n;
  }
  return 0;
}

#endif /* HAL_USE_UART */

/** @} */
