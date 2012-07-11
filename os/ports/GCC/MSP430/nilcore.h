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
 * @file    GCC/MSP430/nilcore.h
 * @brief   Nil RTOS MSP430 port main header file.
 *
 * @addtogroup MSP430_CORE
 * @{
 */

#ifndef _NILCORE_H_
#define _NILCORE_H_

#include <avr/io.h>
#include <avr/interrupt.h>

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   Per-thread stack overhead for interrupts servicing.
 * @details This constant is used in the calculation of the correct working
 *          area size.
 */
#if !defined(PORT_INT_REQUIRED_STACK)
#define PORT_INT_REQUIRED_STACK         32
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Stack and memory alignment enforcement.
 */
typedef uint16_t stkalign_t;

/**
 * @brief   Generic MSP430 register.
 */
typedef void *regmsp_t;

/**
 * @brief   Interrupt saved context.
 * @details This structure represents the stack frame saved during a
 *          preemption-capable interrupt handler.
 */
struct port_extctx {
  regmsp_t      r12;
  regmsp_t      r13;
  regmsp_t      r14;
  regmsp_t      r15;
  regmsp_t      sr;
  regmsp_t      pc;
};

/**
 * @brief   System saved context.
 * @details This structure represents the inner stack frame during a context
 *          switching.
 */
struct port_intctx {
  regmsp_t      r4;
  regmsp_t      r5;
  regmsp_t      r6;
  regmsp_t      r7;
  regmsp_t      r8;
  regmsp_t      r9;
  regmsp_t      r10;
  regmsp_t      r11;
  regmsp_t      pc;
};

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Platform dependent context creation for new threads.
 * @details This code usually setup the context switching frame represented
 *          by an @p intctx structure.
 */
#define SETUP_CONTEXT(workspace, wsize, pf, arg) {                          \
  tp->p_ctx.sp = (struct intctx *)((uint8_t *)workspace +                   \
                                   wsize -                                  \
                                   sizeof(struct intctx));                  \
  tp->p_ctx.sp->r10 = (regmsp_t)pf;                                         \
  tp->p_ctx.sp->r11 = (regmsp_t)arg;                                        \
  tp->p_ctx.sp->pc  = (regmsp_t)_port_thread_start;                         \
}

/**
 * @brief   Enforces a correct alignment for a stack area size value.
 */
#define STACK_ALIGN(n) ((((n) - 1) | (sizeof(stkalign_t) - 1)) + 1)

/**
 * @brief   Computes the thread working area global size.
 */
#define THD_WA_SIZE(n) STACK_ALIGN((sizeof(struct port_intctx) - 1) +       \
                                   (sizeof(struct port_extctx) - 1) +       \
                                   (n) + (PORT_INT_REQUIRED_STACK))

/**
 * @brief   Static working area allocation.
 * @details This macro is used to allocate a static thread working area
 *          aligned as both position and size.
 */
#define NIL_WORKING_AREA(s, n)                                              \
  stkalign_t s[THD_WA_SIZE(n) / sizeof(stkalign_t)]

/**
 * @brief   Thread declaration macro optimized for GCC.
 */
#define PORT_THREAD(tname, arg)                                             \
  __attribute__((noreturn)) void tname(void *arg)

/**
 * @brief   IRQ prologue code.
 * @details This macro must be inserted at the start of all IRQ handlers
 *          enabled to invoke system APIs.
 */
#define PORT_IRQ_PROLOGUE()
}

/**
 * @brief   IRQ epilogue code.
 * @details This macro must be inserted at the end of all IRQ handlers
 *          enabled to invoke system APIs.
 */
#define PORT_IRQ_EPILOGUE() {                                               \
  nilSysLockFromIsr();                                                      \
  nilSchRescheduleS();                                                      \
  nilSysUnlockFromIsr();                                                    \
}

/**
 * @brief   ISR name generation macro.
 */
#define MSP430_ISRNAME(pre, id) pre##id

/**
 * @brief   IRQ handler function declaration.
 * @note    @p id can be a function name or a vector number depending on the
 *          port implementation.
 */
#define PORT_IRQ_HANDLER(id) ISR(id, MSP430_ISRNAME(vect, id))

/**
 * @brief   Port-related initialization code.
 * @note    This function is empty in this port.
 */
#define port_init()

/**
 * @brief   Kernel-lock action.
 * @details Usually this function just disables interrupts but may perform
 *          more actions.
 */
#define port_lock() asm volatile ("dint" : : : "memory")

/**
 * @brief   Kernel-unlock action.
 * @details Usually this function just enables interrupts but may perform
 *          more actions.
 */
#define port_unlock() asm volatile ("eint" : : : "memory")

/**
 * @brief   Kernel-lock action from an interrupt handler.
 * @details This function is invoked before invoking I-class APIs from
 *          interrupt handlers. The implementation is architecture dependent,
 *          in its simplest form it is void.
 * @note    Same as @p port_lock() in this port.
 */
#define port_lock_from_isr()

/**
 * @brief   Kernel-unlock action from an interrupt handler.
 * @details This function is invoked after invoking I-class APIs from interrupt
 *          handlers. The implementation is architecture dependent, in its
 *          simplest form it is void.
 * @note    Same as @p port_lock() in this port.
 */
#define port_unlock_from_isr()

/**
 * @brief   Disables all the interrupt sources.
 */
#define port_disable() asm volatile ("dint" : : : "memory")

/**
 * @brief   Disables the interrupt sources below kernel-level priority.
 */
#define port_suspend() asm volatile ("dint" : : : "memory")

/**
 * @brief   Enables all the interrupt sources.
 */
#define port_enable() asm volatile ("eint" : : : "memory")

/**
 * @brief   Enters an architecture-dependent IRQ-waiting mode.
 * @details The function is meant to return when an interrupt becomes pending.
 *          The simplest implementation is an empty function or macro but this
 *          would not take advantage of architecture-specific power saving
 *          modes.
 * @note    Implemented as an inlined @p WFI instruction.
 */
#define port_wait_for_interrupt() asm volatile ("nop" : : : "memory")

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
#define port_switch(ntp, otp) _port_switch(ntp, otp)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void _port_switch(Thread *ntp, Thread *otp);
  void _port_thread_start(void);
  void port_halt(void);
#ifdef __cplusplus
}
#endif

#endif /* _NILCORE_H_ */

/** @} */
