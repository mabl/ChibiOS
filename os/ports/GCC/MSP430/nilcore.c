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
 * @file    GCC/MSP430/nilcore.c
 * @brief   Nil RTOS MSP430 port main source file.
 *
 * @defgroup MSP430_CORE
 * @{
 */

#include "nil.h"

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

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Performs a context switch between two threads.
 * @details This is the most critical code in any port, this function
 *          is responsible for the context switch between 2 threads.
 * @note    The implementation of this code affects <b>directly</b> the context
 *          switch performance so optimize here as much as you can.
 *
 * @param[in] ntp       the thread to be switched in
 * @param[in] otp       the thread to be switched out
 */
#if !defined(__DOXYGEN__)
__attribute__((naked))
#endif
void _port_switch(Thread *ntp, Thread *otp) {
  register struct intctx *sp asm("r1");

  asm volatile ("push    r11                                    \n\t"
                "push    r10                                    \n\t"
                "push    r9                                     \n\t"
                "push    r8                                     \n\t"
                "push    r7                                     \n\t"
                "push    r6                                     \n\t"
                "push    r5                                     \n\t"
                "push    r4" : : : "memory");

  otp->ctxp = r1;
  r1 = ntp->ctxp;

  asm volatile ("pop     r4                                     \n\t"
                "pop     r5                                     \n\t"
                "pop     r6                                     \n\t"
                "pop     r7                                     \n\t"
                "pop     r8                                     \n\t"
                "pop     r9                                     \n\t"
                "pop     r10                                    \n\t"
                "pop     r11                                    \n\t"
                "ret" : : "r" (sp) : "memory");
}

/**
 * @brief   Start a thread by invoking its work function.
 * @details If the work function returns @p chThdExit() is automatically
 *          invoked.
 */
#if !defined(__DOXYGEN__)
__attribute__((naked))
#endif
void _port_thread_start(void) {

  nilSysUnlock();
  asm volatile ("mov     r11, r15                               \n\t"
                "call    r10");

  nilDbgAssert(TRUE, "_port_thread_start(), #1", "thread returned");
}

/**
 * @brief   Halts the system.
 * @details This function is invoked by the operating system when an
 *          unrecoverable error is detected (for example because a programming
 *          error in the application code that triggers an assertion while in
 *          debug mode).
 * @note    The function is declared as a weak symbol, it is possible to
 *          redefine it in your application code.
 */
#if !defined(__DOXYGEN__)
__attribute__((naked, weak))
#endif
void port_halt(void) {

  port_disable();
  while (TRUE) {
  }
}

/** @} */
