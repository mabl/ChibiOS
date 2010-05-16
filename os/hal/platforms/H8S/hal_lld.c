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
 * @file H8S/hal_lld.c
 * @brief H8S HAL subsystem low level driver source template.
 * @addtogroup H8S_HAL
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

/**
 * @brief PAL setup.
 * @details Digital I/O ports static configuration as defined in @p board.h.
 */
const H8SGPIOConfig pal_default_config =
{
  {VAL_GPIO_OUT, VAL_GPIO_DIR},
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

CH_IRQ_HANDLER(SpuriousIrqHandler) {

  CH_IRQ_PROLOGUE();

  H8S_SPURIOUS_HANDLER_HOOK();

  CH_IRQ_EPILOGUE();
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief Low level HAL driver initialization.
 */
void hal_lld_init(void) {

  /*
   * Spurious interrupt setup
   */
//  *(volatile uint32_t *)(VBR+0x60) = (uint32_t)SpuriousIrqHandler;

}

/** @} */
