/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

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
 * @file    ARM/BCM2835/vectors.s
 * @brief   Interrupt vectors for the BCM2835 family.
 *
 * @defgroup ARM_BCM2835_VECTORS BCM2835 Interrupt Vectors
 * @ingroup ARM_SPECIFIC
 * @details Interrupt vectors for the BCM2835 family.
 * @{
 */

#if defined(__DOXYGEN__)
/**
 * @brief   Unhandled exceptions handler.
 * @details Any undefined exception vector points to this function by default.
 *          This function simply stops the system into an infinite loop.
 *
 * @notapi
 */
void _unhandled_exception(void) {}
#endif

#if !defined(__DOXYGEN__)

.section vectors
.code 32
.balign 4
/*
 * System entry points.
 */
_start:
        ldr     pc, _reset
        ldr     pc, _undefined
        ldr     pc, _swi
        ldr     pc, _prefetch
        ldr     pc, _abort
        nop
        ldr     pc, _irq
        ldr     pc, _fiq

_reset:
        .word   ResetHandler            /* In crt0.s */
_undefined:
        .word   UndHandler
_swi:
        .word   SwiHandler
_prefetch:
        .word   PrefetchHandler
_abort:
        .word   AbortHandler
        .word   0
_irq:
        .word   IrqHandler
_fiq:
        .word   FiqHandler

.text
.code 32
.balign 4

/*
 * Default exceptions handlers. The handlers are declared weak in order to be
 * replaced by the real handling code. Everything is defaulted to an infinite
 * loop.
 */
.weak UndHandler
UndHandler:

.weak SwiHandler
SwiHandler:

.weak PrefetchHandler
PrefetchHandler:

.weak AbortHandler
AbortHandler:

.weak IrqHandler
IrqHandler:

.weak FiqHandler
FiqHandler:

.global _unhandled_exception
_unhandled_exception:
        b       _unhandled_exception

.global __early_init
__early_init:
        mov r0,#0x8000
        mov r1,#0x0000
        ldmia r0!,{r2,r3,r4,r5,r6,r7,r8,r9}
        stmia r1!,{r2,r3,r4,r5,r6,r7,r8,r9}
        ldmia r0!,{r2,r3,r4,r5,r6,r7,r8,r9}
        stmia r1!,{r2,r3,r4,r5,r6,r7,r8,r9}
        bx      lr

.global arm_enable_irq
arm_enable_irq:
    mrs r0,cpsr
    bic r0,r0,#0x80
    msr cpsr_c,r0
    bx lr

.global get_cpsr
get_cpsr:
  mrs r0,cpsr
  bx lr
 
#endif

/** @} */
