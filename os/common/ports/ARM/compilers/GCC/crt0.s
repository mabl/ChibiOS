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
 * @file    crt0.s
 * @brief   Generic ARM startup file.
 *
 * @addtogroup ARM_GCC_STARTUP
 * @{
 */

#if !defined(__DOXYGEN__)

                .set    MODE_USR, 0x10
                .set    MODE_FIQ, 0x11
                .set    MODE_IRQ, 0x12
                .set    MODE_SVC, 0x13
                .set    MODE_ABT, 0x17
                .set    MODE_UND, 0x1B
                .set    MODE_SYS, 0x1F

                .set    I_BIT, 0x80
                .set    F_BIT, 0x40

                .text
                .code   32
                .balign 4

/*
 * Reset handler.
 */
                .global Reset_Handler
Reset_Handler:
                /*
                 * Stack pointers initialization.
                 */
                ldr     r0, =___stacks_end__
                /* Undefined */
                msr     CPSR_c, #MODE_UND | I_BIT | F_BIT
                mov     sp, r0
                ldr     r1, =__und_stack_size__
                sub     r0, r0, r1
                /* Abort */
                msr     CPSR_c, #MODE_ABT | I_BIT | F_BIT
                mov     sp, r0
                ldr     r1, =__abt_stack_size__
                sub     r0, r0, r1
                /* FIQ */
                msr     CPSR_c, #MODE_FIQ | I_BIT | F_BIT
                mov     sp, r0
                ldr     r1, =__fiq_stack_size__
                sub     r0, r0, r1
                /* IRQ */
                msr     CPSR_c, #MODE_IRQ | I_BIT | F_BIT
                mov     sp, r0
                ldr     r1, =__irq_stack_size__
                sub     r0, r0, r1
                /* Supervisor */
                msr     CPSR_c, #MODE_SVC | I_BIT | F_BIT
                mov     sp, r0
                ldr     r1, =__svc_stack_size__
                sub     r0, r0, r1
                /* System */
                msr     CPSR_c, #MODE_SYS | I_BIT | F_BIT
                mov     sp, r0
//                ldr     r1, =__sys_stack_size__
//                sub     r0, r0, r1
                /*
                 * Early initialization.
                 */
#if !defined(THUMB_NO_INTERWORKING)
                bl      __early_init
#else /* defined(THUMB_NO_INTERWORKING) */
                add     r0, pc, #1
                bx      r0
                .code   16
                bl      __early_init
                mov     r0, pc
                bx      r0
                .code   32
#endif /* defined(THUMB_NO_INTERWORKING) */

                /*
                 * Data initialization.
                 * NOTE: It assumes that the DATA size is a multiple of 4.
                 */
                ldr     r1, =_textdata
                ldr     r2, =_data
                ldr     r3, =_edata
dataloop:
                cmp     r2, r3
                ldrlo   r0, [r1], #4
                strlo   r0, [r2], #4
                blo     dataloop
                /*
                 * BSS initialization.
                 * NOTE: It assumes that the BSS size is a multiple of 4.
                 */
                mov     r0, #0
                ldr     r1, =_bss_start
                ldr     r2, =_bss_end
bssloop:
                cmp     r1, r2
                strlo   r0, [r1], #4
                blo     bssloop
                /*
                 * Late initialization.
                 */
#if !defined(THUMB_NO_INTERWORKING)
                bl      __late_init
#else /* defined(THUMB_NO_INTERWORKING) */
                add     r0, pc, #1
                bx      r0
                .code   16
                bl      __late_init
                mov     r0, pc
                bx      r0
                .code   32
#endif /* defined(THUMB_NO_INTERWORKING) */

                /*
                 * Main program invocation.
                 */
#if defined(THUMB_NO_INTERWORKING)
                add     r0, pc, #1
                bx      r0
                .code   16
                bl      main
                ldr     r1, =_main_exit_handler
                bx      r1
                .code   32
#else /* !defined(THUMB_NO_INTERWORKING) */
                bl      main
                b       _main_exit_handler
#endif /* !defined(THUMB_NO_INTERWORKING) */

/*
 * Default main function exit handler.
 */
                .weak   _main_exit_handler
_main_exit_handler:
.loop:          b       .loop

/*
 * Default early initialization code. It is declared weak in order to be
 * replaced by the real initialization code.
 * Early initialization is performed just before reset before BSS and DATA
 * segments initialization.
 */
#if defined(THUMB_NO_INTERWORKING)
                .thumb_func
                .code   16
#endif
                .weak   __early_init
__early_init:
                bx      lr
                .code   32

/*
 * Default late initialization code. It is declared weak in order to be
 * replaced by the real initialization code.
 * Early initialization is performed just after reset before BSS and DATA
 * segments initialization.
 */
#if defined(THUMB_NO_INTERWORKING)
                .thumb_func
                .code   16
#endif
                .weak   __late_init
__late_init:
                bx      lr
                .code   32

#endif /* !defined(__DOXYGEN__) */

/** @} */
