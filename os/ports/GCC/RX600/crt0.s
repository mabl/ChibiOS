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
 * @file    RX600/crt0.s
 * @brief   Generic RX600 startup file for ChibiOS/RT.
 *
 * @addtogroup RX600_CORE
 * @{
 */

#if !defined(__DOXYGEN__)

.section .text
.balign 4

	.extern _data
	.extern _mdata
	.extern _ebss
	.extern _bss
	.extern _edata
	.extern _main 
	.extern _ustack
	.extern _istack
	.extern _rvectors


#ifdef CPPAPP	
___dso_handle:
	.global ___dso_handle
#endif	


/*
 * Reset handler.
 */
.global ResetHandler
.global _PowerON_Reset


ResetHandler:
_PowerON_Reset:

        /*
         * Stack pointers initialization.
         */

		/* initialise user stack pointer */
		mvtc	#_ustack,USP

		/* initialise interrupt stack pointer */
		mvtc	#_istack,ISP

		/* setup intb */
		mvtc	#_rvectors_start, intb	/* INTERRUPT VECTOR ADDRESS  definition	*/

		/* setup FPSW */
		mvtc    #100h, fpsw	

        /*
         * Early initialization.
         */
        bsr.a   hwinit0

        /*
         * Data initialization.
         * NOTE: It assumes that the DATA size is a multiple of 4.
         */
		mov     #_mdata,r2      /* src ROM address of data section in R2 */
		mov     #_data,r1       /* dest start RAM address of data section in R1 */
		mov     #_edata,r3      /* end RAM address of data section in R3 */
		sub    	r1,r3            /* size of data section in R3 (R3=R3-R1) */
		smovf                   /* block copy R3 bytes from R2 to R1 */

        /*
         * BSS initialization.
         * NOTE: It assumes that the BSS size is a multiple of 4.
         */

/* bss initialisation : zero out bss */

		mov		#00h,r2  	/* load R2 reg with zero */
		mov		#_ebss, r3  /* store the end address of bss in R3 */
		mov		#_bss, r1 	/* store the start address of bss in R1 */
		sub   	r1,r3	   	/* size of bss section in R3 (R3=R3-R1) */
		sstr.b

        /*
         * Late initialization.
         */
        bsr.a   hwinit1

		/* setup PSW */
		mvtc	#10000h, psw			/* Set Ubit & Ibit for PSW */

		/* change PSW PM to user-mode 
		mvfc   	psw,r1
		or     	#00100000h,r1
		push.l 	r1
		mvfc   	pc,r1
		add    	#10,r1
		push.l 	r1
		rte
		nop
		nop */

		/* start user program */
        mov.l    #0, r1 		/* argc	 */
        mov.l    #0, r2			/* argv */
		bsr.a	_main		
        bra     MainExitHandler

/*
 * Default main function exit handler.
 */
.weak MainExitHandler
.globl MainExitHandler
MainExitHandler:

.loop:  bra       .loop

/*
 * Default early initialization code. It is declared weak in order to be
 * replaced by the real initialization code.
 * Early initialization is performed just after reset before BSS and DATA
 * segments initialization.
 */

.weak hwinit0
hwinit0:
        rts

/*
 * Default late initialization code. It is declared weak in order to be
 * replaced by the real initialization code.
 * Late initialization is performed after BSS and DATA segments initialization
 * and before invoking the main() function.
 */
.weak hwinit1
hwinit1:
        rts

#endif

/** @} */
