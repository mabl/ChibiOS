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
 * @file    H8S/chcore.h
 * @brief   H8S architecture port macros and structures.
 * @details This file is a template of the system driver macros provided by
 *          a port.
 *
 * @addtogroup H8S_CORE
 * @{
 */

#ifndef _CHCORE_H_
#define _CHCORE_H_

/**
 * @brief   If enabled allows the idle thread to enter a low power mode.
 */
#ifndef ENABLE_WFI_IDLE
#define ENABLE_WFI_IDLE 0
#endif

/**
 * @brief   Unique macro for the implemented architecture.
 */
#define CH_ARCHITECTURE_H8S

/**
 * @brief   Name of the implemented architecture.
 */
#define CH_ARCHITECTURE_NAME "H8S"

/**
 * @brief   32-bit stack alignment.
 */
typedef uint32_t stkalign_t;

/**
 * @brief   Interrupt saved context.
 * @details This structure represents the stack frame saved during a
 *          preemption-capable interrupt handler.
 * @note    Checked for O2 and Os optimizations only.
 */
struct extctx {
  uint32_t	er3;
  uint32_t	er2;
  uint32_t	er1;
  uint32_t	er0;
  uint32_t	pc;
};

/**
 * @brief   System saved context.
 * @details This structure represents the inner stack frame during a context
 *          switching.
 */
struct intctx {
  uint32_t	er6;
  uint32_t	er5;
#ifndef CH_CURRP_REGISTER_CACHE
  uint32_t	er4;
#endif
  uint32_t	pc;
};

/**
 * @brief   Platform dependent part of the @p Thread structure.
 * @details In the H8S port this structure just holds a pointer to the
 *          @p intctx structure representing the stack pointer at the time
 *          of the context switch.
 */
struct context {
  struct intctx *sp;
};

/**
 * @brief   Platform dependent part of the @p chThdInit() API.
 * @details This code usually setup the context switching frame represented
 *          by an @p intctx structure.
 * @note    Initial values for registers d2-d7 and a3-a4 are for test purposes only.
 */
#ifndef CH_CURRP_REGISTER_CACHE
#define SETUP_CONTEXT(workspace, wsize, pf, arg) {          \
  tp->p_ctx.sp = (struct intctx *)((uint8_t *)workspace +   \
                  wsize -	sizeof(struct intctx));         \
  tp->p_ctx.sp->er6 = (uint32_t)arg;                        \
  tp->p_ctx.sp->er5 = (uint32_t)pf;                         \
  tp->p_ctx.sp->er4 = 0xD4D4D4D4;                           \
  tp->p_ctx.sp->pc = (uint32_t)threadstart;                 \
}
#else
#define SETUP_CONTEXT(workspace, wsize, pf, arg) {          \
  tp->p_ctx.sp = (struct intctx *)((uint8_t *)workspace +   \
                  wsize -	sizeof(struct intctx));         \
  tp->p_ctx.sp->er6 = (uint32_t)arg;                        \
  tp->p_ctx.sp->er5 = (uint32_t)pf;                         \
  tp->p_ctx.sp->pc = (uint32_t)threadstart;                 \
}
#endif // CH_CURRP_REGISTER_CACHE

/**
 * @brief   Stack size for the system idle thread.
 * @details This size depends on the idle thread implementation, usually
 *          the idle thread should take no more space than those reserved
 *          by @p INT_REQUIRED_STACK.
 */
#ifndef IDLE_THREAD_STACK_SIZE
#define IDLE_THREAD_STACK_SIZE 20
#endif

/**
 * @brief   Per-thread stack overhead for interrupts servicing.
 * @details This constant is used in the calculation of the correct working
 *          area size.
 *          This value can be zero on those architecture where there is a
 *          separate interrupt stack and the stack space between @p intctx and
 *          @p extctx is known to be zero.
 */
#ifndef INT_REQUIRED_STACK
#define INT_REQUIRED_STACK 64
#endif

/**
 * @brief   Enforces a correct alignment for a stack area size value.
 */
#define STACK_ALIGN(n) ((((n) - 1) | (sizeof(stkalign_t) - 1)) + 1)

/**
 * @brief   Computes the thread working area global size.
 */
#define THD_WA_SIZE(n) STACK_ALIGN(sizeof(Thread) +                         \
                                   sizeof(struct intctx) +                  \
                                   sizeof(struct extctx) +                  \
                                   (n) + (INT_REQUIRED_STACK))

/**
 * @brief   Static working area allocation.
 * @details This macro is used to allocate a static thread working area
 *          aligned as both position and size.
 */
#define WORKING_AREA(s, n) stkalign_t s[THD_WA_SIZE(n) / sizeof(stkalign_t)];

/**
 * @brief   IRQ prologue code.
 * @details This macro must be inserted at the start of all IRQ handlers
 *          enabled to invoke system APIs.
 * @note    Currently the H8S port prevents the IRQ handlers preemption
 *          by raising the mask into the SR register to 7.
 */
#define PORT_IRQ_PROLOGUE() {                               \
  asm volatile (    "stm.l      er0-er3,@-sp         \n\t");\
}

/**
 * @brief   IRQ epilogue code.
 * @details This macro must be inserted at the end of all IRQ handlers
 *          enabled to invoke system APIs.
 */
#define PORT_IRQ_EPILOGUE() {                               \
  if (chSchIsRescRequiredExI())                             \
      chSchDoRescheduleI();                                 \
  asm volatile (    "ldm.l      @sp+,er0-er3         \n\t"  \
                    "rte                             \n\t");\
}

/**
 * @brief   IRQ handler function declaration.
 * @note    @p id can be a function name or a vector number depending on the
 *          port implementation.
 */
#define PORT_IRQ_HANDLER(id) void __attribute__((interrupt_handler)) id(void)

/**
 * @brief   Port-related initialization code.
 * @note    Empty in this port.
 */
#define port_init()

/**
 * @brief   Kernel-lock action.
 * @details Usually this function just disables interrupts but may perform more
 *          actions.
 * @note    In the H8S port this is performed by raising the IRQ mask
 *          in the SR register to 7.
 */
#define port_lock() asm volatile ("orc.b    #0x80,ccr  \n\t")

/**
 * @brief   Kernel-unlock action.
 * @details Usually this function just disables interrupts but may perform more
 *          actions.
 * @note    In the H8S port this is performed by lowering the IRQ mask
 *          in the SR register to 0.
 */
#define port_unlock() asm volatile ("andc.b #0x7F,ccr  \n\t")

/**
 * @brief   Kernel-lock action from an interrupt handler.
 * @details This function is invoked before invoking I-class APIs from
 *          interrupt handlers. The implementation is architecture dependent,
 *          in its simplest form it is void.
 * @note    Empty in this port.
 */
#define port_lock_from_isr()

/**
 * @brief   Kernel-unlock action from an interrupt handler.
 * @details This function is invoked after invoking I-class APIs from interrupt
 *          handlers. The implementation is architecture dependent, in its
 *          simplest form it is void.
 * @note    Empty in this port.
 */
#define port_unlock_from_isr()

/**
 * @brief   Disables all the interrupt sources.
 * @note    Of course non maskable interrupt sources are not included.
 */
#define port_disable() asm volatile ("orc.b  #0x80,ccr      \n\t")

/**
 * @brief   Disables the interrupt sources that are not supposed to preempt
 *          the kernel.
 */
#define port_suspend()

/**
 * @brief   Enables all the interrupt sources.
 */
#define port_enable() asm volatile ("andc.b #0x7F,ccr       \n\t")

/**
 * This port function is implemented as inlined code for performance reasons.
 * @note The port code does not define a low power mode, this macro has to be
 *       defined externally. The default implementation is a "nop", not a
 *       real low power mode.
 */
#if ENABLE_WFI_IDLE != 0
#ifndef port_wait_for_interrupt
#define port_wait_for_interrupt() {                                     \
  asm volatile ("sleep");                                               \
}
#endif
#else
#define port_wait_for_interrupt()
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void port_halt(void);
  void port_switch(Thread *otp, Thread *ntp);
  void threadstart(void);
#ifdef __cplusplus
}
#endif

#endif /* _CHCORE_H_ */

/** @} */
