/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013,2014 Giovanni Di Sirio.

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
 * @file    GCC/RL78/chcore.c
 * @brief   Renesas RL78 architecture port code.
 *
 * @addtogroup RL78_CORE
 * @{
 */

#include "ch.h"
#include "hal.h"

/**
 * @brief RL78 initialization code.
 *
 * @note This function is usually empty.
 */
void port_init(void){
}

/**
 * @brief Disables all the interrupt sources.
 *
 * @note Of course non maskable interrupt sources are not included.
 */
void port_disable() {

  asm volatile ("di                                                 \n\t");
}

/**
 * @brief Disables the interrupt sources that are not supposed to preempt the kernel.
 */
void port_suspend(void) {

  asm volatile ("di                                                 \n\t");
}

/**
 * @brief Enables all the interrupt sources.
 */
void port_enable(void) {

  asm volatile ("ei                                                 \n\t");
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
__attribute__((weak))
#endif
void port_halt(void) {

  port_disable();
  while (TRUE) {
  }
}

/**
 * @brief Performs a context switch between two threads.
 * @details This is the most critical code in any port, this function
 * is responsible for the context switch between 2 threads.
 *
 * @param ntp the thread to be switched in
 * @param otp the thread to be switched out
 * @note The implementation of this code affects <b>directly</b> the context
 *       switch performance so optimize here as much as you can.
 */
#if !defined(__DOXYGEN__)
__attribute__((naked, weak))
#endif
void port_switch(Thread *ntp, Thread *otp) {
#if !defined(CH_CURRP_REGISTER_CACHE)
/*
  de - ntp
  hl - otp
*/
/*  register struct intctx *sp asm ("sp"); */
  asm volatile ("movw    ax,[sp+4]                                  \n\t"
                "movw    de,ax                                      \n\t"
                "movw    ax,[sp+6]                                  \n\t"
                "movw    hl,ax                                      \n\t");
  asm volatile ("sel     rb2                                        \n\t"
                "push    ax                                         \n\t"
                "push    bc                                         \n\t"
                "push    de                                         \n\t"
                "push    hl                                         \n\t");
/*
  otp->p_ctx.sp = sp;
  sp = ntp->p_ctx.sp;
*/
  asm volatile ("sel     rb0                                        \n\t"
                "movw    ax,sp                                      \n\t"
                "movw    [hl+6],ax                                  \n\t"
                "nop                                                \n\t"
                "movw    ax,[de+6]                                  \n\t"
                "movw    sp,ax                                      \n\t"
                "sel     rb2                                        \n\t");
  asm volatile ("pop     hl                                         \n\t"
                "pop     de                                         \n\t"
                "pop     bc                                         \n\t"
                "pop     ax                                         \n\t"
                "sel     rb0                                        \n\t");
#else
#error "CH_CURRP_REGISTER_CACHE not supported"
#endif
  asm volatile ("ret                                                \n\t");
}

/**
 * @brief   Start a thread by invoking its work function.
 * @details If the work function returns @p chThdExit() is automatically
 *          invoked.
 */
void _port_thread_start(msg_t (*pf)(void *), void *p) {

  chSysUnlock();
  chThdExit(pf(p));
  while(1);
}

/** @} */
