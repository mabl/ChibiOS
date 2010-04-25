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
 * @file    CF/chcore.c
 * @brief   ColdFire architecture port code.
 * @details This file is a template of the system driver functions provided by
 *          a port. Some of the following functions may be implemented as
 *          macros in chcore.h if the implementer decides that there is an
 *          advantage in doing so, as example because performance concerns.
 *
 * @addtogroup CF_CORE
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
//void __attribute__((optimize("-fno-toplevel-reorder"))) port_switch(Thread *otp, Thread *ntp) {
void port_switch(Thread *otp, Thread *ntp) {
/* It doesn't work, optimizer makes incorrect code */
/*
  register struct intctx *sp asm("sp");

  asm volatile (
    "lea     %%sp@(-44), %%sp                   \n\t"
    "movem.l %%d2-%%d7/%%a2-%%a6, %%sp@         \n\t" : : "r" (sp));

  otp->p_ctx.sp = sp;
  sp = ntp->p_ctx.sp;

  asm volatile (
    "movem.l %%sp@, %%d2-%%d7/%%a2-%%a6         \n\t"
    "lea     %%sp@(44), %%sp                    \n\t" : : "r" (sp));
*/
/* It is the same but not dependent on internal offsets.*/

/* It doesn't work too, optimizer makes incorrect code */
/*
  register struct intctx *sp asm("sp");
  register struct intctx *ctx asm("a0");

  otp->p_ctx.sp = sp;
  ctx = ntp->p_ctx.sp;
  asm volatile (
    "lea     %%sp@(-44), %%sp                   \n\t"
    "movem.l %%d2-%%d7/%%a2-%%a6, %%sp@         \n\t" : : "r" (sp));

  sp = ctx;

  asm volatile (
    "movem.l %%sp@, %%d2-%%d7/%%a2-%%a6         \n\t"
    "lea     %%sp@(44), %%sp                    \n\t" : : "r" (sp));

// 3001080a <port_switch>:
// 3001080a:	206f 0004      	moveal %sp@(4),%a0
// 3001080e:	214f 000c      	movel %sp,%a0@(12)
// 30010812:	4fef ffd4      	lea %sp@(-44),%sp
// 30010816:	48d7 7cfc      	moveml %d2-%d7/%a2-%fp,%sp@
// 3001081a:	206f 0008      	moveal %sp@(8),%a0
// 3001081e:	2e68 000c      	moveal %a0@(12),%sp
// 30010822:	4cd7 7cfc      	moveml %sp@,%d2-%d7/%a2-%fp
// 30010826:	4fef 002c      	lea %sp@(44),%sp
// 3001082a:	4e75           	rts

*/

  asm volatile (
    "movel   %sp@(4), %a0                       \n\t"
    "movel   %sp@(8), %a1                       \n\t"
    "lea     %sp@(-44), %sp                     \n\t"
    "moveml  %d2-%d7/%a2-%a6, %sp@              \n\t"
    "movel   %sp, %a0@(12)                      \n\t"
    "movel   %a1@(12), %sp                      \n\t"
    "moveml  %sp@, %d2-%d7/%a2-%a6              \n\t"
    "lea     %sp@(44), %sp                      \n\t"
  );

}

/**
 * @brief   Starts a thread by invoking its work function.
 * @details If the work function returns @p chThdExit() is automatically
 *          invoked.
 */
void _port_thread_start(void) {

  asm volatile (
    "move.w  #0x2000,%sr                        \n\t"
    "movel   %a6,-(%sp)                         \n\t"
    "jsr     %a5@                               \n\t"
    "addql   #4,%sp                             \n\t"
    "jsr     chThdExit                          \n\t"
  );
}

/** @} */
