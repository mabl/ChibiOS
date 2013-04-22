/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    STM8S/spi_lld.h
 * @brief   STM8S low level SPI driver header.
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

/**
 * @brief   SPI1 driver enable switch.
 * @details If set to @p TRUE the support for device SSP0 is included.
 * @note    The default is @p TRUE.
 */
#if !defined(STM8S_SPI_USE_SPI) || defined(__DOXYGEN__)
#define STM8S_SPI_USE_SPI               TRUE
#endif

/**
 * @brief   Overflow error hook.
 * @details The default action is to stop the system.
 */
#if !defined(STM8S_SPI_SPI_ERROR_HOOK) || defined(__DOXYGEN__)
#define STM8S_SPI_ERROR_HOOK(spip)      chSysHalt()
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !STM8S_SPI_USE_SPI
#error "SPI driver activated but no SPI peripheral assigned"
#endif

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
 */
typedef struct {
  /**
   * @brief Operation complete callback or @p NULL.
   */
  spicallback_t         end_cb;
  /* End of the mandatory fields.*/
  /**
   * @brief The chip select line port.
   */
  ioportid_t            ssport;
  /**
   * @brief The chip select line pad number.
   */
  uint16_t              sspad;
  /**
   * @brief SPI initialization data.
   */
  uint8_t               cr1;
} SPIConfig;

/**
 * @brief   Structure representing a SPI driver.
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
#endif /* SPI_USE_MUTUAL_EXCLUSION */
#if defined(SPI_DRIVER_EXT_FIELDS)
  SPI_DRIVER_EXT_FIELDS
#endif
  /* End of the mandatory fields.*/
  /**
   * @brief Number of bytes yet to be received.
   */
  uint16_t              rxcnt;
  /**
   * @brief Receive pointer or @p NULL.
   */
  uint8_t               *rxptr;
  /**
   * @brief Number of bytes yet to be transmitted.
   */
  uint16_t              txcnt;
  /**
   * @brief Transmit pointer or @p NULL.
   */
  const uint8_t         *txptr;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if STM8S_SPI_USE_SPI && !defined(__DOXYGEN__)
extern SPIDriver SPID1;
#endif

#ifdef __cplusplus
extern "C" {
#endif
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
  uint8_t spi_lld_polled_exchange(SPIDriver *spip, uint8_t frame);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_SPI */

#endif /* _SPI_LLD_H_ */

/** @} */
