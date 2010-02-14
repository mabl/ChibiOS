/*
    ChibiOS/RT - Copyright (C) 2006-2007 Giovanni Di Sirio.

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
 * @file    templates/hal_lld.c
 * @brief   HAL Driver subsystem low level driver source template.
 *
 * @addtogroup HAL_LLD
 * @{
 */

#include "ch.h"
#include "hal.h"

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level HAL driver initialization.
 */
void hal_lld_init(void) {

#if STM8S_CLOCK_SOURCE != CLK_SOURCE_DEFAULT
#if STM8S_CLOCK_SOURCE == CLK_SOURCE_HSI
  CLK_ICKR    = 1;                  /* HSIEN */
  while ((CLK_ICKR & 2) == 0)       /* HSIRDY */
    ;
#elif STM8S_CLOCK_SOURCE == CLK_SOURCE_LSI
  CLK_ICKR    = 8;                  /* LSIEN */             
  while ((CLK_ICKR & 16) == 0)      /* LSIRDY */
    ;
#else /* STM8S_CLOCK_SOURCE == CLK_SOURCE_HSE */
  CLK_ECKR    = 1;                  /* HSEEN */
  while ((CLK_ECKR & 2) == 0)       /* HSERDY */
    ;
#endif
  /* Switching clock (manual switch mode).*/
  CLK_SWCR    = 0;
  CLK_SWR     = STM8S_CLOCK_SOURCE;
  while ((CLK_SWCR & 8) == 0)       /* SWIF */
    ;
  CLK_SWCR    = 4;                  /* SWEN */

  /* Setting up clock dividers.*/
  CLK_CKDIVR  = (STM8S_HSI_DIVIDER << 3) | (STM8S_CPU_DIVIDER << 0);

  /* Clocks initially all disabled.*/
  CLK_PCKENR1 = 0;
  CLK_PCKENR2 = 0;
  
  /* Other clock related initializations.*/
  CLK_CSSR    = 0;
  CLK_CCOR    = 0;
  CLK_CANCCR  = 0;
#endif /* STM8S_CLOCK_SOURCE != CLK_SOURCE_DEFAULT */
}

/** @} */
