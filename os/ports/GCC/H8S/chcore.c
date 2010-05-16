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
 * @file    H8S/chcore.c
 * @brief   H8S architecture port code.
 * @details This file is a template of the system driver functions provided by
 *          a port. Some of the following functions may be implemented as
 *          macros in chcore.h if the implementer decides that there is an
 *          advantage in doing so, as example because performance concerns.
 *
 * @addtogroup H8S_CORE
 * @{
 */

#include <ch.h>

/**
 * @brief   Halts the system.
 * @details This function is invoked by the operating system when an
 *          unrecoverable error is detected (as example because a programming
 *          error in the application code that triggers an assertion while in
 *          debug mode).
 */
void port_halt(void) {

  port_disable();
  while (TRUE) {
  }
}

/**
 * @brief   Performs a context switch between two threads.
 * @details This is the most critical code in any port, this function
 *          is responsible for the context switch between 2 threads.
 * @note    The implementation of this code affects <b>directly</b> the context
 *          switch performance so optimize here as much as you can.
 *
 * @param otp   the thread to be switched out
 * @param ntp   the thread to be switched in
 */
//void __attribute__((optimize("-Os"))) port_switch(Thread *otp, Thread *ntp) {
void port_switch(Thread *otp, Thread *ntp) {
/*
// It doesn't work, optimizer cut the code "sp = ntp->p_ctx.sp"
  register struct intctx *sp asm("er7");

  asm volatile (
    "stm.l    er4-er6,@-sp                    \n\t"
  );
  otp->p_ctx.sp = sp;
  sp = ntp->p_ctx.sp;
  asm volatile (
    "ldm.l    @sp+,er4-er6                    \n\t"
  );
*/

#ifndef CH_CURRP_REGISTER_CACHE
  asm volatile (
    "stm.l    er4-er6,@-sp                    \n\t"
    "mov.l    er7,@(12,er0)                   \n\t"
    "mov.l    @(12,er1),er7                   \n\t"
    "ldm.l    @sp+,er4-er6                    \n\t"
  );
#else
  asm volatile (
    "stm.l    er5-er6,@-sp                    \n\t"
    "mov.l    er7,@(12,er0)                   \n\t"
    "mov.l    @(12,er1),er7                   \n\t"
    "ldm.l    @sp+,er5-er6                    \n\t"
  );
#endif // CH_CURRP_REGISTER_CACHE

}

/**
 * @brief   Starts a thread by invoking its work function.
 * @details If the work function returns @p chThdExit() is automatically
 *          invoked.
 */
void threadstart(void) {

  asm volatile (
    "andc.b   #0x7F,ccr                       \n\t"
    "mov.l    er6,er0                         \n\t"
    "jsr      @er5                            \n\t"
    "jsr      @_chThdExit                     \n\t"
  );

}

/** @} */
