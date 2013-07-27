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
 * @file    RX62N/hal_lld.h
 * @brief   HAL subsystem low level driver header template.
 *
 * @addtogroup HAL
 * @{
 */

#ifndef _HAL_LLD_H_
#define _HAL_LLD_H_

#include "iodefine_gcc62n.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Defines the support for realtime counters in the HAL.
 */
#define HAL_IMPLEMENTS_COUNTERS FALSE

/**
 * @brief   Platform name.
 */
#define PLATFORM_NAME             "RX62N"

#define IWDTCLK                   125000      /**< Watchdog internal clock.   */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   System PLL multiplier.
 * @note    The value is hardware fixed at 8.
 */
#if defined(__DOXYGEN__)
#define RX62N_SYSPLL_MUL                  8
#endif

/**
 * @brief   System PLL divider.
 * @note    The value must be chosen between (1, 2, 4, 8).
 */
#if defined(__DOXYGEN__)
#define RX62N_SYSPLL_DIV                  1
#endif

/**
 * @brief   Enables or disables the SDCLK clock output.
 */
#if !defined(RX62N_SDCLK_OUTPUT_ENABLED) || defined(__DOXYGEN__)
#define RX62N_SDCLK_OUTPUT_ENABLED        FALSE
#endif

/**
 * @brief   Enables or disables the BCLK clock output.
 */
#if !defined(RX62N_BCLK_OUTPUT_ENABLED) || defined(__DOXYGEN__)
#define RX62N_BCLK_OUTPUT_ENABLED         FALSE
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/**
 * @brief   ICK mask in SCKCR register.
 */
#if (RX62N_ICLK_MUL == 1) || defined(__DOXYGEN__)
#define RX62N_SCKCR_ICK (3 << 24)
#elif RX62N_ICLK_MUL == 2
#define RX62N_SCKCR_ICK (2 << 24)
#elif RX62N_ICLK_MUL == 4
#define RX62N_SCKCR_ICK (1 << 24)
#elif RX62N_ICLK_MUL == 8
#define RX62N_SCKCR_ICK (0 << 24)
#else
#error "invalid RX62N_ICLK_MUL value (1,2,4,8), check mcuconf.h"
#endif

/**
 * @brief   BCK mask in SCKCR register.
 */
#if (RX62N_BCLK_MUL == 1) || defined(__DOXYGEN__)
#define RX62N_SCKCR_BCK (3 << 16)
#elif RX62N_BCLK_MUL == 2
#define RX62N_SCKCR_BCK (2 << 16)
#elif RX62N_BCLK_MUL == 4
#define RX62N_SCKCR_BCK (1 << 16)
#elif RX62N_BCLK_MUL == 8
#define RX62N_SCKCR_BCK (0 << 16)
#else
#error "invalid RX62N_BCLK_MUL value (1,2,4,8), check mcuconf.h"
#endif

/**
 * @brief   PCK mask in SCKCR register.
 */
#if (RX62N_PCLK_MUL == 1) || defined(__DOXYGEN__)
#define RX62N_SCKCR_PCK (3 << 8)
#elif RX62N_PCLK_MUL == 2
#define RX62N_SCKCR_PCK (2 << 8)
#elif RX62N_PCLK_MUL == 4
#define RX62N_SCKCR_PCK (1 << 8)
#elif RX62N_PCLK_MUL == 8
#define RX62N_SCKCR_PCK (0 << 8)
#else
#error "invalid RX62N_PCLK_MUL value (1,2,4,8), check mcuconf.h"
#endif

/**
 * @brief   System clock.
 */
#define  RX62N_SYSCLK     (EXTALCLK * RX62N_ICLK_MUL)
#if (RX62N_SYSCLK < 8000000) || (RX62N_SYSCLK > 100000000)
#error "System clock frequency out of the acceptable range (8..100MHz max)"
#endif

/**
 * @brief   Peripheral clock.
 */
#define  RX62N_PERCLK     (EXTALCLK * RX62N_PCLK_MUL)
#if (RX62N_PERCLK < 8000000) || (RX62N_PERCLK > 100000000)
#error "Peripheral clock frequency out of the acceptable range (8..50MHz max)"
#endif

#if (RX62N_PERCLK > RX62N_SYSCLK)
#error "Peripheral clock frequency higher than system clock frequency"
#endif

#if (EXTALCLK < 8000000) || (EXTALCLK > 14000000)
#error "External crystal frequency out of the acceptable range (8..14MHz max)"
#endif

/*
 * TODO: check SYSCLK (ICLK) when ethernet controller is used (SYSCLK > 12.5MHz)
 * TODO: check UCLK when USB is used
 * TODO: oscilation stop detection
 */

/*
Clock frequencies (from manual):
ICLK: 8..100MHz
PCLK: 8..50MHz
BCLK: 8..50MHz or 8..100MHz
BCLK_OUT: 8..25MHz or 8..50MHz
SDCLK: 8..50MHz
UCLK: 48MHz (only when EXTAL = 12MHz)
SUBCLK: 32768Hz
IWDTCLK: 125kHz
ICLK >= PCLK
ICLK >= BCLK
*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void hal_lld_init(void);
  void rx62n_clock_init(void);
#ifdef __cplusplus
}
#endif

#endif /* _HAL_LLD_H_ */

/** @} */
