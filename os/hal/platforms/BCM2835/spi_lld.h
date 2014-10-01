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
 * @file    templates/spi_lld.h
 * @brief   SPI Driver subsystem low level driver header template.
 *
 * @addtogroup SPI
 * @{
 */

#ifndef _SPI_LLD_H_
#define _SPI_LLD_H_

#if HAL_USE_SPI || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a structure representing an SPI driver.
 */
typedef struct SPIDriver SPIDriver;

/**
 * @brief   SPI notification callback type.
 *
 * @param[in] spip      pointer to the @p SPIDriver object triggering the
 *                      callback
 */
typedef void (*spicallback_t)(SPIDriver *spip);

/**
 * @brief   Driver configuration structure.
 * @note    Implementations may extend this structure to contain more,
 *          architecture dependent, fields.
 */
typedef struct {
  /**
   * @brief Operation complete callback.
   */
  spicallback_t         end_cb;
  /* End of the mandatory fields.*/

  /* @brief LoSSI enable
   * 0 = The serial interface will behave as an SPI master.
   * 1 = The serial interface will behave as a LoSSI master.
   */
  uint8_t               lossiEnabled;

  /* @brief 00=Chip select 0, 01=Chip select 1 */
  uint8_t               chip_select;

  /* @brief Chip Select Polarity
   *    0 = Chip select lines are active low 
   *    1 = Chip select lines are active high
   */
  uint8_t               chip_select_polarity;

  /* @brief Chip Select 0 Polarity
   *    0 = Chip select lines are active low 
   *    1 = Chip select lines are active high
   */
  uint8_t               chip_select_polarity0;

  /* @brief Chip Select 1 Polarity
   *    0 = Chip select lines are active low 
   *    1 = Chip select lines are active high
   */
  uint8_t               chip_select_polarity1;

  /* @brief Clock Polarity 
   *    0 = Rest state of clock = low. 
   *    1 = Rest state of clock = high.
   */
  uint8_t               clock_polarity;

  /* @brief Clock Phase
   *    0 = First SCLK transition at middle of data bit.
   *    1 = First SCLK transition at beginning of data bit.
   */
  uint8_t               clock_phase;

  /* @brief Clock Divider.*/
  uint32_t              clock_divider;

} SPIConfig;

/**
 * @brief   Structure representing an SPI driver.
 * @note    Implementations may extend this structure to contain more,
 *          architecture dependent, fields.
 */
struct SPIDriver {
  /**
   * @brief Driver state.
   */
  spistate_t            state;
  /**
   * @brief Current configuration data.
   */
  const SPIConfig       *config;
#if SPI_USE_WAIT || defined(__DOXYGEN__)
  /**
   * @brief Waiting thread.
   */
  Thread                *thread;
#endif /* SPI_USE_WAIT */
#if SPI_USE_MUTUAL_EXCLUSION || defined(__DOXYGEN__)
#if CH_USE_MUTEXES || defined(__DOXYGEN__)
  /**
   * @brief Mutex protecting the bus.
   */
  Mutex                 mutex;
#elif CH_USE_SEMAPHORES
  Semaphore             semaphore;
#endif

  /**
   * @brief Receive buffer
   */
  void                  *rxbuf;

  /**
   * @brief Transmit buffer
   */
  const void            *txbuf;

  /**
   * @brief Pending transfer count
   */
  size_t              txcnt;

#endif /* SPI_USE_MUTUAL_EXCLUSION */
#if defined(SPI_DRIVER_EXT_FIELDS)
  SPI_DRIVER_EXT_FIELDS
#endif
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
  extern SPIDriver SPI0;

  void spi_lld_init(void);
  void spi_lld_start(SPIDriver *spip);
  void spi_lld_stop(SPIDriver *spip);
  void spi_lld_select(SPIDriver *spip);
  void spi_lld_unselect(SPIDriver *spip);
  void spi_lld_ignore(SPIDriver *spip, size_t n);
  void spi_lld_exchange(SPIDriver *spip, size_t n,
                        const void *txbuf, void *rxbuf);
  void spi_lld_send(SPIDriver *spip, size_t n, const void *txbuf);
  void spi_lld_receive(SPIDriver *spip, size_t n, void *rxbuf);
  uint16_t spi_lld_polled_exchange(SPIDriver *spip, uint16_t frame);

  void spi_lld_serve_interrupt(SPIDriver *spip);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_SPI */

#endif /* _SPI_LLD_H_ */

/** @} */
