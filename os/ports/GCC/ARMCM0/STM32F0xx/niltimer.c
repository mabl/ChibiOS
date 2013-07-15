/*
    Nil RTOS - Copyright (C) 2012 Giovanni Di Sirio.

    This file is part of Nil RTOS.

    Nil RTOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    Nil RTOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    STM32F0xx/niltimer.c
 * @brief   Nil RTOS STM32F0 system timer code.
 *
 * @addtogroup STM32F0_TIMER
 */

#include "nil.h"

#if NIL_CFG_TIMEDELTA > 0

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module interrupt handlers.                                                */
/*===========================================================================*/

/**
 * @brief   TIM2 interrupt handler.
 *
 * @isr
 */
NIL_IRQ_HANDLER(Vector7C) {

  NIL_IRQ_PROLOGUE();

  STM32F0_TIM2->SR = 0;

  nilSysLockFromISR();
  nilSysTimerHandlerI();
  nilSysUnlockFromISR();

  NIL_IRQ_EPILOGUE();
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

#endif /* NIL_CFG_TIMEDELTA > 0 */

/** @} */
