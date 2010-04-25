/*
    ChibiOS/RT - Copyright (C) 2009 Giovanni Di Sirio.

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

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file ColdFire/serial_lld.h
 * @brief COLDFIRE low level serial driver header.
 * @addtogroup COLDFIRE_SERIAL
 * @{
 */

#ifndef _SERIAL_LLD_H_
#define _SERIAL_LLD_H_

#if CH_HAL_USE_SERIAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief UART1 driver enable switch.
 * @details If set to @p TRUE the support for UART1 is included.
 * @note The default is @p TRUE.
 */
#if !defined(USE_COLDFIRE_UART1) || defined(__DOXYGEN__)
#define USE_COLDFIRE_USART1 TRUE
#endif

/**
 * @brief UART2 driver enable switch.
 * @details If set to @p TRUE the support for UART2 is included.
 * @note The default is @p FALSE.
 */
#if !defined(USE_COLDFIRE_UART2) || defined(__DOXYGEN__)
#define USE_COLDFIRE_USART2 FALSE
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief Serial Driver condition flags type.
 */
typedef uint8_t sdflags_t;

/**
 * @brief AVR Serial Driver configuration structure.
 * @details An instance of this structure must be passed to @p sdStart()
 *          in order to configure and start a serial driver operations.
 */
typedef struct {
  /**
   * @brief Initialization value for the UBG register.
   */
  uint32_t                  sc_speed;
  /**
   * @brief Initialization value for the USR register.
   */
  uint8_t                   sc_usr;
  /**
   * @brief Initialization value for the UMR register.
   */
  uint8_t                   sc_umr;
} SerialConfig;

/**
 * @brief @p SerialDriver specific data.
 */
#define _serial_driver_data                                                 \
  _base_asynchronous_channel_data                                           \
  /* Driver state.*/                                                        \
  sdstate_t                 state;                                          \
  /* Current configuration data.*/                                          \
  const SerialConfig        *config;                                        \
  /* Input queue.*/                                                         \
  InputQueue                iqueue;                                         \
  /* Output queue.*/                                                        \
  OutputQueue               oqueue;                                         \
  /* Status Change @p EventSource.*/                                        \
  EventSource               sevent;                                         \
  /* I/O driver status flags.*/                                             \
  sdflags_t                 flags;                                          \
  /* Input circular buffer.*/                                               \
  uint8_t                   ib[SERIAL_BUFFERS_SIZE];                        \
  /* Output circular buffer.*/                                              \
  uint8_t                   ob[SERIAL_BUFFERS_SIZE];                        \
  /* End of the mandatory fields.*/                                         \
  /* Pointer to the USART registers block.*/                                \
  mcf5206e_UART1            *uart1;                                         \
  mcf5206e_UART2            *uart2;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/


/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/** @cond never*/
#if USE_COLDFIRE_UART1
extern SerialDriver SD1;
#endif
#if USE_COLDFIRE_UART2
extern SerialDriver SD2;
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void sd_lld_init(void);
  void sd_lld_start(SerialDriver *sdp);
  void sd_lld_stop(SerialDriver *sdp);
#ifdef __cplusplus
}
#endif
/** @endcond*/

#endif /* _SERIAL_LLD_H_ */

/** @} */
