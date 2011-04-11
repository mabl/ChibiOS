/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

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
 * @file    RX600/chcore.c
 * @brief   RX600/9 architecture port code.
 *
 * @addtogroup RX600_CORE
 * @{
 */

#include "ch.h"

/**
 * @brief   Performs a context switch between two threads.
 * @details This is the most critical code in any port, this function
 *          is responsible for the context switch between 2 threads.
 * @note    The implementation of this code affects <b>directly</b> the context
 *          switch performance so optimize here as much as you can.
 * @note    The function is declared as a weak symbol, it is possible to
 *          redefine it in your application code.
 *
 * @param[in] ntp       the thread to be switched in
 * @param[in] otp       the thread to be switched out
 */
#if !defined(__DOXYGEN__)
__attribute__((naked, weak))
#endif
void port_switch(Thread *ntp, Thread *otp) {
  register struct intctx *sp asm("r0");

  asm volatile( "PUSHC   PSW                                    \n\t" \
				"PUSH    R15                                    \n\t" \
                "PUSHM   R5-R13                                 \n\t" );
  otp->p_ctx.r0 = sp;
  sp = ntp->p_ctx.r0;
  asm volatile ("POPM    R5-R13                                 \n\t" \
                "POP     R15                                    \n\t" \
                "POPC    PSW                                    \n\t" \
                "RTS                                            ");
}


/*
 * Common exit point for all IRQ routines, it performs the rescheduling if
 * required.
 * System stack frame structure after a context switch in the
 * interrupt handler:
 *
 * High +------------+
 *      |   PSW_IRQ  | -+
 *      |   PC_IRQ   |  |
 *      |     R14    |  |
 *      |     R4     |  |
 *      |     R3     |  | External context: IRQ handler frame
 *      |     R2     |  |
 *      |     R1     |  |
 *      |     PC     |  |   (user code return address)
 *      |   PSW_USR  | -+   (user code status)
 *      |    ....    | <- mk_DoRescheduleI() stack frame, optimize it for space
 *      |     PC     | -+   (system code return address)
 *      |    (R15)   |  |   (optional, see CH_CURRP_REGISTER_CACHE)
 *      |     R13    |  |
 *      |     R12    |  |
 *      |     R11    |  |
 *      |     R10    |  | Internal context: mk_SwitchI() frame
 *      |     R9     |  |
 *      |     R8     |  |
 *      |     R7     |  |
 *      |     R6     |  |
 * SP-> |     R5     | -+
 * Low  +------------+
 */


/**
 * Start a thread by invoking its work function.
 * If the work function returns @p chThdExit() is automatically invoked.
 */
 
void _port_thread_start(tfunc_t pf, void *args) 
{
  asm volatile ("mov     r6, r1                                 \n\t"
                "jsr     r5                                     \n\t"
                "bra.a   __port_thread_off");
}

void _port_thread_off(void)
{
volatile long cntOff;

	while(1)
	{
		cntOff++;
	}
}


/**
 * Halts the system.
 */
#if !defined(__DOXYGEN__)
__attribute__((weak))
#endif
void port_halt(void) {

  port_disable();
  while (TRUE) {
  }
}

/** @} */
