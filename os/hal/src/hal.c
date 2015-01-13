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
 * @file    hal.c
 * @brief   HAL subsystem code.
 *
 * @addtogroup HAL
 * @{
 */

#include "hal.h"

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
 * @brief   HAL initialization.
 * @details This function invokes the low level initialization code then
 *          initializes all the drivers enabled in the HAL. Finally the
 *          board-specific initialization is performed by invoking
 *          @p boardInit() (usually defined in @p board.c).
 *
 * @init
 */
void halInit(void) {

  /* Initializes the OS Abstraction Layer.*/
  osalInit();

  /* Platform low level initializations.*/
  hal_lld_init();

#if HAL_USE_PAL || defined(__DOXYGEN__)
  palInit(&pal_default_config);
#endif
#if HAL_USE_ADC || defined(__DOXYGEN__)
  adcInit();
#endif
#if HAL_USE_CAN || defined(__DOXYGEN__)
  canInit();
#endif
#if HAL_USE_EXT || defined(__DOXYGEN__)
  extInit();
#endif
#if HAL_USE_GPT || defined(__DOXYGEN__)
  gptInit();
#endif
#if HAL_USE_I2C || defined(__DOXYGEN__)
  i2cInit();
#endif
#if HAL_USE_I2S || defined(__DOXYGEN__)
  i2sInit();
#endif
#if HAL_USE_ICU || defined(__DOXYGEN__)
  icuInit();
#endif
#if HAL_USE_MAC || defined(__DOXYGEN__)
  macInit();
#endif
#if HAL_USE_PWM || defined(__DOXYGEN__)
  pwmInit();
#endif
#if HAL_USE_SERIAL || defined(__DOXYGEN__)
  sdInit();
#endif
#if HAL_USE_SDC || defined(__DOXYGEN__)
  sdcInit();
#endif
#if HAL_USE_SPI || defined(__DOXYGEN__)
  spiInit();
#endif
#if HAL_USE_UART || defined(__DOXYGEN__)
  uartInit();
#endif
#if HAL_USE_USB || defined(__DOXYGEN__)
  usbInit();
#endif
#if HAL_USE_MMC_SPI || defined(__DOXYGEN__)
  mmcInit();
#endif
#if HAL_USE_SERIAL_USB || defined(__DOXYGEN__)
  sduInit();
#endif
#if HAL_USE_RTC || defined(__DOXYGEN__)
  rtcInit();
#endif

  /* Community driver overlay initialization.*/
#if HAL_USE_COMMUNITY || defined(__DOXYGEN__)
  halCommunityInit();
#endif

  /* Board specific initialization.*/
  boardInit();

/*
 *  The ST driver is a special case, it is only initialized if the OSAL is
 *  configured to require it.
 */
#if OSAL_ST_MODE != OSAL_ST_MODE_NONE
  stInit();
#endif
}

/** @} */
