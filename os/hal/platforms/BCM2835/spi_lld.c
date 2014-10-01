/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

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
 * @file    templates/spi_lld.c
 * @brief   SPI Driver subsystem low level driver source template.
 *
 * @addtogroup SPI
 * @{
 */

#include "ch.h"
#include "hal.h"
#include "bcm2835.h"
#include "chprintf.h"

#if HAL_USE_SPI || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

SPIDriver SPI0;

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#define read_fifo(spip) {                       \
  while (SPI0_CS & SPI_CS_RXD) {                \
    uint32_t rx = SPI0_FIFO;                    \
    uint8_t *rxbuf = (uint8_t *)(spip)->rxbuf;	\
    if (rxbuf)                                  \
      *rxbuf++ = rx;                            \
  }                                             \
}


/**
 * @brief   SPI IRQ handler.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
void spi_lld_serve_interrupt(SPIDriver *spip) {
  if (IRQ_PEND2 & SPI_IRQ) {
    if (SPI0_CS & SPI_CS_DONE) {
      size_t *count = &(spip->txcnt);
      if (*count > 0) {
	/* Fill FIFO */
	if (spip->config->lossiEnabled) {
	  /* LoSSI sends 9 bits (2 bytes) at a time.*/
	  uint16_t *txbuf = (uint16_t *)(spip)->txbuf;
	  while ((SPI0_CS & SPI_CS_TXD) && *count > 0) {
	    SPI0_FIFO = spip->txbuf != NULL ? *(txbuf)++ : 0;
	    *count -= 2;
	  }
	}
	else {
	  uint8_t *txbuf = (uint8_t *)(spip)->txbuf;
	  while ((SPI0_CS & SPI_CS_TXD) && *count > 0) {
	    SPI0_FIFO = spip->txbuf != NULL ? *(txbuf)++ : 0;
	    --*count;
	  }
	}
      }
      else {
	/* LoSSI-mode recv not supported yet.*/
	read_fifo(spip);
	/* Deactivate Transfer and disable SPI interrupts.*/
	SPI0_CS &= ~(SPI_CS_INTD | SPI_CS_INTR | SPI_CS_TA);
	_spi_isr_code(spip);
      }
    }
  
    if (SPI0_CS & SPI_CS_RXR) {
      read_fifo(spip);
    }
  }
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level SPI driver initialization.
 *
 * @notapi
 */
void spi_lld_init(void) {
  spiObjectInit(&SPI0);
}

/**
 * @brief   Configures and activates the SPI peripheral.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
void spi_lld_start(SPIDriver *spip) {
  UNUSED(spip);

  IRQ_DISABLE2 |= BIT(22);

  /* This can be optimized to setting two masks.*/
  bcm2835_gpio_fnsel(7, GPFN_ALT0);   /* SPI0_CE1_N.*/
  bcm2835_gpio_fnsel(8, GPFN_ALT0);   /* SPI0_CE0_N.*/
  bcm2835_gpio_fnsel(9, GPFN_ALT0);   /* SPI0_MOSI.*/
  bcm2835_gpio_fnsel(10, GPFN_ALT0);  /* SPI0_MISO.*/
  bcm2835_gpio_fnsel(11, GPFN_ALT0);  /* SPIO_SCLK.*/

  uint32_t control = 0;

  /* Not using bidirectional mode.*/
  control &= ~SPI_CS_REN;

  const SPIConfig *cfg = spip->config;

  if (cfg->clock_polarity)
    control |= SPI_CS_CPOL;

  if (cfg->clock_phase)
    control |= SPI_CS_CPHA;

  if (cfg->chip_select_polarity)
    control |= SPI_CS_CSPOL;

  if (cfg->chip_select_polarity0)
    control |= SPI_CS_CSPOL0;

  if (cfg->chip_select_polarity1)
    control |= SPI_CS_CSPOL1;

  if (cfg->lossiEnabled)
    control |= SPI_CS_LEN;

  /* Optimization: precompute control mask? Could be externally changed.*/
  SPI0_CS = control | SPI_CS_CLEAR_TX | SPI_CS_CLEAR_RX;

  SPI0_CLK = cfg->clock_divider;
}

/**
 * @brief   Deactivates the SPI peripheral.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
void spi_lld_stop(SPIDriver *spip) {
  UNUSED(spip);

  IRQ_DISABLE2 |= BIT(22);

  /* This can be optimized to setting two masks.*/
  bcm2835_gpio_fnsel(7, GPFN_IN);
  bcm2835_gpio_fnsel(8, GPFN_IN);
  bcm2835_gpio_fnsel(9, GPFN_IN);
  bcm2835_gpio_fnsel(10, GPFN_IN);
  bcm2835_gpio_fnsel(11, GPFN_IN);
}

/**
 * @brief   Asserts the slave select signal and prepares for transfers.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
void spi_lld_select(SPIDriver *spip) {
  uint32_t cs = SPI0_CS;
  cs &= ~SPI_CS_CS;
  cs |= (spip->config->chip_select & SPI_CS_CS);
  SPI0_CS = cs;
}

/**
 * @brief   Deasserts the slave select signal.
 * @details The previously selected peripheral is unselected.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
void spi_lld_unselect(SPIDriver *spip) {
  UNUSED(spip);
  /* This sets the CS back to CS0.
   * There's no way to turn off all chip selects.
   * However, the CS is only active during transfers.
   */
  SPI0_CS &= ~SPI_CS_CS;
}

/**
 * @brief   Ignores data on the SPI bus.
 * @details This asynchronous function starts the transmission of a series of
 *          idle words on the SPI bus and ignores the received data.
 * @post    At the end of the operation the configured callback is invoked.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be ignored
 *
 * @notapi
 */
void spi_lld_ignore(SPIDriver *spip, size_t n) {
  spi_lld_exchange(spip, n, NULL, NULL);
}

/**
 * @brief   Exchanges data on the SPI bus.
 * @details This asynchronous function starts a simultaneous transmit/receive
 *          operation.
 * @post    At the end of the operation the configured callback is invoked.
 * @note    The buffers are organized as uint8_t arrays for data sizes below or
 *          equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be exchanged
 * @param[in] txbuf     the pointer to the transmit buffer
 * @param[out] rxbuf    the pointer to the receive buffer
 *
 * @notapi
 */
void spi_lld_exchange(SPIDriver *spip, size_t n,
                      const void *txbuf, void *rxbuf) {
  /* Clear TX and RX fifos.*/
  SPI0_CS |= SPI_CS_CLEAR_TX | SPI_CS_CLEAR_RX;

  spip->txbuf = txbuf;
  spip->txcnt = n;
  spip->rxbuf = rxbuf;

  /* Enable SPI interrupts and activate transfer.*/
  SPI0_CS |=  SPI_CS_INTD | SPI_CS_INTR |  SPI_CS_TA;
  IRQ_ENABLE2 |= BIT(22);
}

/**
 * @brief   Sends data over the SPI bus.
 * @details This asynchronous function starts a transmit operation.
 * @post    At the end of the operation the configured callback is invoked.
 * @note    The buffers are organized as uint8_t arrays for data sizes below or
 *          equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to send
 * @param[in] txbuf     the pointer to the transmit buffer
 *
 * @notapi
 */
void spi_lld_send(SPIDriver *spip, size_t n, const void *txbuf) {
  spi_lld_exchange(spip, n, txbuf, NULL);
}

/**
 * @brief   Receives data from the SPI bus.
 * @details This asynchronous function starts a receive operation.
 * @post    At the end of the operation the configured callback is invoked.
 * @note    The buffers are organized as uint8_t arrays for data sizes below or
 *          equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to receive
 * @param[out] rxbuf    the pointer to the receive buffer
 *
 * @notapi
 */
void spi_lld_receive(SPIDriver *spip, size_t n, void *rxbuf) {
  spi_lld_exchange(spip, n, NULL, rxbuf);
}

/**
 * @brief   Exchanges one frame using a polled wait.
 * @details This synchronous function exchanges one frame using a polled
 *          synchronization method. This function is useful when exchanging
 *          small amount of data on high speed channels, usually in this
 *          situation is much more efficient just wait for completion using
 *          polling than suspending the thread waiting for an interrupt.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] frame     the data frame to send over the SPI bus
 * @return              The received data frame from the SPI bus.
 */
uint16_t spi_lld_polled_exchange(SPIDriver *spip, uint16_t frame) {
  UNUSED(spip);

  /* Clear TX and RX fifos. Start transfer.*/
  SPI0_CS |= SPI_CS_CLEAR_TX | SPI_CS_CLEAR_RX | SPI_CS_TA;

  /* Wait for space in TX FIFO.*/
  while (!(SPI0_CS & SPI_CS_TXD));

  /* Write to FIFO.*/
  SPI0_FIFO = frame;

  /* Wait for DONE to be set.*/
  while (!(SPI0_CS & SPI_CS_DONE));

  /* Read any bytes that were sent back by the slave.*/
  uint16_t rxdata = SPI0_FIFO;

  /* Set TA = 0.*/
  SPI0_CS &= ~SPI_CS_TA;

  return rxdata;
}

#endif /* HAL_USE_SPI */

/** @} */
