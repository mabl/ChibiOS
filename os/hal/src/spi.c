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
 * @file    spi.c
 * @brief   SPI Driver code.
 *
 * @addtogroup SPI
 * @{
 */

#include "hal.h"

#if HAL_USE_SPI || defined(__DOXYGEN__)

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
 * @brief   SPI Driver initialization.
 * @note    This function is implicitly invoked by @p halInit(), there is
 *          no need to explicitly initialize the driver.
 *
 * @init
 */
void spiInit(void) {

  spi_lld_init();
}

/**
 * @brief   Initializes the standard part of a @p SPIDriver structure.
 *
 * @param[out] spip     pointer to the @p SPIDriver object
 *
 * @init
 */
void spiObjectInit(SPIDriver *spip) {

  spip->state = SPI_STOP;
  spip->config = NULL;
#if SPI_USE_WAIT
  spip->thread = NULL;
#endif /* SPI_USE_WAIT */
#if SPI_USE_MUTUAL_EXCLUSION
  osalMutexObjectInit(&spip->mutex);
#endif /* SPI_USE_MUTUAL_EXCLUSION */
#if defined(SPI_DRIVER_EXT_INIT_HOOK)
  SPI_DRIVER_EXT_INIT_HOOK(spip);
#endif
}

/**
 * @brief   Configures and activates the SPI peripheral.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] config    pointer to the @p SPIConfig object
 *
 * @api
 */
void spiStart(SPIDriver *spip, const SPIConfig *config) {

  osalDbgCheck((spip != NULL) && (config != NULL));

  osalSysLock();
  osalDbgAssert((spip->state == SPI_STOP) || (spip->state == SPI_READY),
                "invalid state");
  spip->config = config;
  spi_lld_start(spip);
  spip->state = SPI_READY;
  osalSysUnlock();
}

/**
 * @brief Deactivates the SPI peripheral.
 * @note  Deactivating the peripheral also enforces a release of the slave
 *        select line.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @api
 */
void spiStop(SPIDriver *spip) {

  osalDbgCheck(spip != NULL);

  osalSysLock();
  osalDbgAssert((spip->state == SPI_STOP) || (spip->state == SPI_READY),
                "invalid state");
  spi_lld_stop(spip);
  spip->state = SPI_STOP;
  osalSysUnlock();
}

/**
 * @brief   Asserts the slave select signal and prepares for transfers.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @api
 */
void spiSelect(SPIDriver *spip) {

  osalDbgCheck(spip != NULL);

  osalSysLock();
  osalDbgAssert(spip->state == SPI_READY, "not ready");
  spiSelectI(spip);
  osalSysUnlock();
}

/**
 * @brief   Deasserts the slave select signal.
 * @details The previously selected peripheral is unselected.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @api
 */
void spiUnselect(SPIDriver *spip) {

  osalDbgCheck(spip != NULL);

  osalSysLock();
  osalDbgAssert(spip->state == SPI_READY, "not ready");
  spiUnselectI(spip);
  osalSysUnlock();
}

/**
 * @brief   Ignores data on the SPI bus.
 * @details This asynchronous function starts the transmission of a series of
 *          idle words on the SPI bus and ignores the received data.
 * @pre     A slave must have been selected using @p spiSelect() or
 *          @p spiSelectI().
 * @post    At the end of the operation the configured callback is invoked.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be ignored
 *
 * @api
 */
void spiStartIgnore(SPIDriver *spip, size_t n) {

  osalDbgCheck((spip != NULL) && (n > 0));

  osalSysLock();
  osalDbgAssert(spip->state == SPI_READY, "not ready");
  spiStartIgnoreI(spip, n);
  osalSysUnlock();
}

/**
 * @brief   Exchanges data on the SPI bus.
 * @details This asynchronous function starts a simultaneous transmit/receive
 *          operation.
 * @pre     A slave must have been selected using @p spiSelect() or
 *          @p spiSelectI().
 * @post    At the end of the operation the configured callback is invoked.
 * @note    The buffers are organized as uint8_t arrays for data sizes below
 *          or equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be exchanged
 * @param[in] txbuf     the pointer to the transmit buffer
 * @param[out] rxbuf    the pointer to the receive buffer
 *
 * @api
 */
void spiStartExchange(SPIDriver *spip, size_t n,
                      const void *txbuf, void *rxbuf) {

  osalDbgCheck((spip != NULL) && (n > 0) && (rxbuf != NULL) && (txbuf != NULL));

  osalSysLock();
  osalDbgAssert(spip->state == SPI_READY, "not ready");
  spiStartExchangeI(spip, n, txbuf, rxbuf);
  osalSysUnlock();
}

/**
 * @brief   Sends data over the SPI bus.
 * @details This asynchronous function starts a transmit operation.
 * @pre     A slave must have been selected using @p spiSelect() or
 *          @p spiSelectI().
 * @post    At the end of the operation the configured callback is invoked.
 * @note    The buffers are organized as uint8_t arrays for data sizes below
 *          or equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to send
 * @param[in] txbuf     the pointer to the transmit buffer
 *
 * @api
 */
void spiStartSend(SPIDriver *spip, size_t n, const void *txbuf) {

  osalDbgCheck((spip != NULL) && (n > 0) && (txbuf != NULL));

  osalSysLock();
  osalDbgAssert(spip->state == SPI_READY, "not ready");
  spiStartSendI(spip, n, txbuf);
  osalSysUnlock();
}

/**
 * @brief   Receives data from the SPI bus.
 * @details This asynchronous function starts a receive operation.
 * @pre     A slave must have been selected using @p spiSelect() or
 *          @p spiSelectI().
 * @post    At the end of the operation the configured callback is invoked.
 * @note    The buffers are organized as uint8_t arrays for data sizes below
 *          or equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to receive
 * @param[out] rxbuf    the pointer to the receive buffer
 *
 * @api
 */
void spiStartReceive(SPIDriver *spip, size_t n, void *rxbuf) {

  osalDbgCheck((spip != NULL) && (n > 0) && (rxbuf != NULL));

  osalSysLock();
  osalDbgAssert(spip->state == SPI_READY, "not ready");
  spiStartReceiveI(spip, n, rxbuf);
  osalSysUnlock();
}

#if SPI_USE_WAIT || defined(__DOXYGEN__)
/**
 * @brief   Ignores data on the SPI bus.
 * @details This synchronous function performs the transmission of a series of
 *          idle words on the SPI bus and ignores the received data.
 * @pre     In order to use this function the option @p SPI_USE_WAIT must be
 *          enabled.
 * @pre     In order to use this function the driver must have been configured
 *          without callbacks (@p end_cb = @p NULL).
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be ignored
 *
 * @api
 */
void spiIgnore(SPIDriver *spip, size_t n) {

  osalDbgCheck((spip != NULL) && (n > 0));

  osalSysLock();
  osalDbgAssert(spip->state == SPI_READY, "not ready");
  osalDbgAssert(spip->config->end_cb == NULL, "has callback");
  spiStartIgnoreI(spip, n);
  _spi_wait_s(spip);
  osalSysUnlock();
}

/**
 * @brief   Exchanges data on the SPI bus.
 * @details This synchronous function performs a simultaneous transmit/receive
 *          operation.
 * @pre     In order to use this function the option @p SPI_USE_WAIT must be
 *          enabled.
 * @pre     In order to use this function the driver must have been configured
 *          without callbacks (@p end_cb = @p NULL).
 * @note    The buffers are organized as uint8_t arrays for data sizes below
 *          or equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be exchanged
 * @param[in] txbuf     the pointer to the transmit buffer
 * @param[out] rxbuf    the pointer to the receive buffer
 *
 * @api
 */
void spiExchange(SPIDriver *spip, size_t n,
                 const void *txbuf, void *rxbuf) {

  osalDbgCheck((spip != NULL) && (n > 0) &&
               (rxbuf != NULL) && (txbuf != NULL));

  osalSysLock();
  osalDbgAssert(spip->state == SPI_READY, "not ready");
  osalDbgAssert(spip->config->end_cb == NULL, "has callback");
  spiStartExchangeI(spip, n, txbuf, rxbuf);
  _spi_wait_s(spip);
  osalSysUnlock();
}

/**
 * @brief   Sends data over the SPI bus.
 * @details This synchronous function performs a transmit operation.
 * @pre     In order to use this function the option @p SPI_USE_WAIT must be
 *          enabled.
 * @pre     In order to use this function the driver must have been configured
 *          without callbacks (@p end_cb = @p NULL).
 * @note    The buffers are organized as uint8_t arrays for data sizes below
 *          or equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to send
 * @param[in] txbuf     the pointer to the transmit buffer
 *
 * @api
 */
void spiSend(SPIDriver *spip, size_t n, const void *txbuf) {

  osalDbgCheck((spip != NULL) && (n > 0) && (txbuf != NULL));

  osalSysLock();
  osalDbgAssert(spip->state == SPI_READY, "not ready");
  osalDbgAssert(spip->config->end_cb == NULL, "has callback");
  spiStartSendI(spip, n, txbuf);
  _spi_wait_s(spip);
  osalSysUnlock();
}

/**
 * @brief   Receives data from the SPI bus.
 * @details This synchronous function performs a receive operation.
 * @pre     In order to use this function the option @p SPI_USE_WAIT must be
 *          enabled.
 * @pre     In order to use this function the driver must have been configured
 *          without callbacks (@p end_cb = @p NULL).
 * @note    The buffers are organized as uint8_t arrays for data sizes below
 *          or equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to receive
 * @param[out] rxbuf    the pointer to the receive buffer
 *
 * @api
 */
void spiReceive(SPIDriver *spip, size_t n, void *rxbuf) {

  osalDbgCheck((spip != NULL) && (n > 0) && (rxbuf != NULL));

  osalSysLock();
  osalDbgAssert(spip->state == SPI_READY, "not ready");
  osalDbgAssert(spip->config->end_cb == NULL, "has callback");
  spiStartReceiveI(spip, n, rxbuf);
  _spi_wait_s(spip);
  osalSysUnlock();
}
#endif /* SPI_USE_WAIT */

#if SPI_USE_MUTUAL_EXCLUSION || defined(__DOXYGEN__)
/**
 * @brief   Gains exclusive access to the SPI bus.
 * @details This function tries to gain ownership to the SPI bus, if the bus
 *          is already being used then the invoking thread is queued.
 * @pre     In order to use this function the option @p SPI_USE_MUTUAL_EXCLUSION
 *          must be enabled.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @api
 */
void spiAcquireBus(SPIDriver *spip) {

  osalDbgCheck(spip != NULL);

  osalMutexLock(&spip->mutex);
}

/**
 * @brief   Releases exclusive access to the SPI bus.
 * @pre     In order to use this function the option @p SPI_USE_MUTUAL_EXCLUSION
 *          must be enabled.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @api
 */
void spiReleaseBus(SPIDriver *spip) {

  osalDbgCheck(spip != NULL);

  osalMutexUnlock(&spip->mutex);
}
#endif /* SPI_USE_MUTUAL_EXCLUSION */

#endif /* HAL_USE_SPI */

/** @} */
