/*
    ChibiOS/RT - Copyright (C) 2009 Giovanni Di Sirio.

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
 * @file ports/MSP430/chcore.c
 * @brief MSP430 architecture port code.
 * @addtogroup MSP430_CORE
 * @{
 */

#include <ch.h>
#include <mb96356rw.h>

#pragma inline read_reg_sp


/**
 * Performs a context switch between two threads.
 * @param otp the thread to be switched out
 * @param ntp the thread to be switched in
 * @note The function is declared as a weak symbol, it is possible to redefine
 *       it in your application code.
 */
/** @cond never */

/** @endcond */
//#pragma inline port_switch
void port_switch(Thread *ntp, Thread *otp) 
{
#pragma asm
  	intp	_port_switch_swi_handler	;context save
#pragma endasm
}

__nosavereg __interrupt void port_switch_swi_handler(void)
{
#pragma asm
				    ;save into the system stack task the current context
		PUSHW 	(RW2, RW3, RW4, RW5, RW6, RW7)
		MOVW    A, SP				; plz record the current bottom stack
		MOVW	RW0,A				; RW0 => sp
		MOVW	RW1,@RW0+(6+12+12)	; RW1 => otp (note: only for medium model)
		MOVW	@RW1+6,A			; @(RW1+8) => otp->p_ctx

	  // Then swap to new task 
		MOVW	RW1,@RW0+(4+12+12)	; RW1 => ntp (note: only for medium model)
		MOVW	A,@RW1+6			; RW1+8 => ntp->p_ctx
		MOVW	SP,A				; setup new stack 
		POPW 	(RW2, RW3, RW4, RW5, RW6, RW7)
		RETI						; restore context

/*
				    ;save into the system stack task the current context
		PUSHW 	(RW0, RW1, RW2, RW3, RW4, RW5, RW6, RW7)
		MOVW    A, SP				; plz record the current bottom stack
		MOVW	RW0,A				; RW0 => sp
		MOVW	RW1,@RW0+(6+16+12)	; RW1 => otp (note: only for medium model)
		MOVW	@RW1+6,A			; @(RW1+8) => otp->p_ctx

	  // Then swap to new task 
		MOVW	RW1,@RW0+(4+16+12)	; RW1 => ntp (note: only for medium model)
		MOVW	A,@RW1+6			; RW1+8 => ntp->p_ctx
		MOVW	SP,A				; setup new stack 
		POPW 	(RW0, RW1, RW2, RW3, RW4, RW5, RW6, RW7)
		RETI						; restore context
*/

#pragma endasm

}


static reg16fx_t read_reg_sp(void) 
{
#pragma asm
	MOVW    A, SP				; plz record the current bottom stack
#pragma endasm
}


/**
 * Disables the interrupts and halts the system.
 * @note The function is declared as a weak symbol, it is possible to redefine
 *       it in your application code.
 */
/** @cond never */

/** @endcond */
void port_halt(void) {

  port_disable();
  while (TRUE) 
  {
  }
}

/**
 * Start a thread by invoking its work function.
 * If the work function returns @p chThdExit() is automatically invoked.
 */
void threadstart(tfunc_t pf, void *args) 
{
msg_t thread_msg;

	thread_msg = pf(args);
	chThdExit(thread_msg);
}

void threadoff(void)
{
volatile long cntOff;

	while(1)
	{
		cntOff++;
	}
}

uint8_t read_reg_dpr(void)
{
#pragma asm
      mov A, DPR
#pragma endasm	
}

uint8_t read_reg_adb(void)
{
#pragma asm
      mov A, ADB
#pragma endasm	
}

uint8_t read_reg_pcb(uint32_t f)
{
	f >>= 16;
	f &= 0x00FF;

	return f;
}

uint8_t read_reg_dtb(void)
{
#pragma asm
      mov A, DTB
#pragma endasm	
}



/** @} */
