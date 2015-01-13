/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    ARMCMx/compilers/GCC/nilcoreasm_v6m.s
 * @brief   ARMv6-M architecture port low level code.
 *
 * @addtogroup ARMCMx_GCC_CORE
 * @{
 */

#define _FROM_ASM_
#include "nilconf.h"
#include "nilcore.h"

#if !defined(FALSE) || defined(__DOXYGEN__)
#define FALSE   0
#endif

#if !defined(TRUE) || defined(__DOXYGEN__)
#define TRUE    1
#endif

#if !defined(__DOXYGEN__)

                .set    CONTEXT_OFFSET, 0
                .set    SCB_ICSR, 0xE000ED04
                .set    ICSR_PENDSVSET, 0x10000000
                .set    ICSR_NMIPENDSET, 0x80000000

                .cpu    cortex-m0
                .fpu    softvfp

                .thumb
                .text

/*--------------------------------------------------------------------------*
 * Performs a context switch between two threads.
 *--------------------------------------------------------------------------*/
                .thumb_func
                .globl  _port_switch
_port_switch:
                push    {r4, r5, r6, r7, lr}
                mov     r4, r8
                mov     r5, r9
                mov     r6, r10
                mov     r7, r11
                push    {r4, r5, r6, r7}
                mov r3, sp
                str r3, [r1, #CONTEXT_OFFSET]
                ldr r3, [r0, #CONTEXT_OFFSET]
                mov sp, r3
                pop     {r4, r5, r6, r7}
                mov     r8, r4
                mov     r9, r5
                mov     r10, r6
                mov     r11, r7
                pop     {r4, r5, r6, r7, pc}

/*--------------------------------------------------------------------------*
 * Start a thread by invoking its work function.
 *
 * Threads execution starts here, the code leaves the system critical zone
 * and then jumps into the thread function passed in register R4. The
 * register R5 contains the thread parameter. The function chThdExit() is
 * called on thread function return.
 *--------------------------------------------------------------------------*/
                .thumb_func
                .globl  _port_thread_start
_port_thread_start:
                cpsie   i
                mov     r0, r5
                blx     r4
                mov     r3, #0
                bl      chSysHalt

/*--------------------------------------------------------------------------*
 * Post-IRQ switch code.
 *
 * Exception handlers return here for context switching.
 *--------------------------------------------------------------------------*/
                .thumb_func
                .globl  _port_switch_from_isr
_port_switch_from_isr:
                bl      chSchRescheduleS
                .globl  _port_exit_from_isr
_port_exit_from_isr:
                ldr     r2, .L2
                ldr     r3, .L3
                str     r3, [r2, #0]
#if CORTEX_ALTERNATE_SWITCH
                cpsie   i
#endif
.L1:            b       .L1

                .align  2
.L2:            .word   SCB_ICSR
#if CORTEX_ALTERNATE_SWITCH
.L3:            .word   ICSR_PENDSVSET
#else
.L3:            .word   ICSR_NMIPENDSET
#endif

#endif /* !defined(__DOXYGEN__) */

/** @} */
