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
 * @file    templates/chcore.h
 * @brief   Port related template macros and structures.
 * @details This file is a template of the system driver macros provided by
 *          a port.
 *
 * @addtogroup core
 * @{
 */

#ifndef _CHCORE_H_
#define _CHCORE_H_

#include <intrins.h>

/**
 * @brief   Unique macro for the implemented architecture.
 */
#define CH_ARCHITECTURE_STM8S

/**
 * @brief   Name of the implemented architecture.
 */
#define CH_ARCHITECTURE_NAME "STM8S"

/**
 * @brief   Base type for stack alignment.
 * @note    No alignment constraints so uint8_t.
 */
typedef uint8_t stkalign_t;

/**
 * @brief   Interrupt saved context.
 * @details This structure represents the stack frame saved during a
 *          preemption-capable interrupt handler.
 * @note    The structure requires one dummy field at its start because the
 *          stack is handled as preincremented/postdecremented.
 */
struct extctx {
  uint8_t       _next;
  uint8_t       cl;
  uint8_t       ch;
  uint8_t       bl;
  uint8_t       bh;
  uint8_t       cc;
  uint8_t       a;
  uint8_t       xh;
  uint8_t       xl;
  uint8_t       yh;
  uint8_t       yl;
  uint8_t       pce;
  uint8_t       pch;
  uint8_t       pcl;
};

/**
 * @brief   System saved context.
 * @details This structure represents the inner stack frame during a context
 *          switching.
 * @note    Only PCL and PCH are defined because the kernel code requires
 *          the small memory mode (the application code can use the extended
 *          address space).
 * @note    The structure requires one dummy field at its start because the
 *          stack is handled as preincremented/postdecremented.
 */
struct intctx {
  uint8_t       _next;
  uint8_t       pch;
  uint8_t       pcl;
};

/**
 * @brief   Start context.
 * @details This context is the stack organization for the trampoline code
 *          @p _port_thread_start().
 */
struct startctx {
  uint8_t     tsh;              /* Trampoline address high byte.            */
  uint8_t     tsl;              /* Trampoline address low byte.             */
  uint8_t     argh;             /* Thread argument high byte.               */
  uint8_t     argl;             /* Thread argument low byte.                */
  uint8_t     pch;              /* Thread function address high byte.       */
  uint8_t     pcl;              /* Thread function address low byte.        */
  uint8_t     reth;             /* chThdExit() address high byte.           */
  uint8_t     tetl;             /* chThdExit() address low byte.            */
};

/**
 * @brief   Platform dependent part of the @p Thread structure.
 * @details This structure usually contains just the saved stack pointer
 *          defined as a pointer to a @p intctx structure.
 */
struct context {
  struct intctx *sp;
};

/**
 * @brief   Platform dependent part of the @p chThdInit() API.
 * @details This code usually setup the context switching frame represented
 *          by an @p intctx structure.
 */
#define SETUP_CONTEXT(workspace, wsize, pf, arg) {                      \
  struct startctx *scp;                                                 \
  scp = (struct startctx *)((uint8_t *)workspace + wsize  -             \
                            sizeof(struct startctx));                   \
  scp->tsh  = (uint8_t)((uint16_t)_port_thread_start >> 8);             \
  scp->tsl  = (uint8_t)_port_thread_start;                              \
  scp->argh = (uint8_t)((uint16_t)arg >> 8);                            \
  scp->argl = (uint8_t)arg;                                             \
  scp->pch  = (uint8_t)((uint16_t)pf >> 8);                             \
  scp->pcl  = (uint8_t)pf;                                              \
  scp->reth  = (uint8_t)((uint16_t)chThdExit >> 8);                     \
  scp->tetl  = (uint8_t)chThdExit;                                      \
  tp->p_ctx.sp = (struct intctx *)scp;                                  \
}

/**
 * @brief   Stack size for the system idle thread.
 * @details This size depends on the idle thread implementation, usually
 *          the idle thread should take no more space than those reserved
 *          by @p INT_REQUIRED_STACK.
 */
#ifndef IDLE_THREAD_STACK_SIZE
#define IDLE_THREAD_STACK_SIZE  0
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
#define INT_REQUIRED_STACK      32
#endif

/**
 * @brief   Enforces a correct alignment for a stack area size value.
 */
#define STACK_ALIGN(n) ((((n) - 1) | (sizeof(stkalign_t) - 1)) + 1)

/**
 * @brief   Computes the thread working area global size.
 */
#define THD_WA_SIZE(n) STACK_ALIGN(sizeof(Thread) +                     \
                                   sizeof(struct intctx) +              \
                                   sizeof(struct extctx) +              \
                                  (n) + (INT_REQUIRED_STACK))



/**
 * @brief   Static working area allocation.
 * @details This macro is used to allocate a static thread working area
 *          aligned as both position and size.
 */
#define WORKING_AREA(s, n) stkalign_t s[THD_WA_SIZE(n) / sizeof(stkalign_t)]

/**
 * @brief   IRQ prologue code.
 * @details This macro must be inserted at the start of all IRQ handlers
 *          enabled to invoke system APIs.
 */
#define PORT_IRQ_PROLOGUE()

/**
 * @brief   IRQ epilogue code.
 * @details This macro must be inserted at the end of all IRQ handlers
 *          enabled to invoke system APIs.
 */
#define PORT_IRQ_EPILOGUE() {   \
  if (chSchIsRescRequiredExI()) \
      chSchDoRescheduleI();     \
}

/**
 * @brief   IRQ handler function declaration.
 * @note    @p id can be a function name or a vector number depending on the
 *          port implementation.
 */
#define PORT_IRQ_HANDLER(id) void irq##id(void) interrupt id

/**
 * @brief   Port-related initialization code.
 * @note    None in this port.
 */
#define port_init()

/**
 * @brief   Kernel-lock action.
 * @note    Implemented as global interrupts disable.
 */
#define port_lock() _sim_()

/**
 * @brief   Kernel-unlock action.
 * @note    Implemented as global interrupts enable.
 */
#define port_unlock() _rim_()

/**
 * @brief   Kernel-lock action from an interrupt handler.
 * @note    This function is empty in this port.
 */
#define port_lock_from_isr()

/**
 * @brief   Kernel-unlock action from an interrupt handler.
 * @note    This function is empty in this port.
 */
#define port_unlock_from_isr()

/**
 * @brief   Disables all the interrupt sources.
 * @note    Implemented as global interrupts disable.
 * @note    Of course non maskable interrupt sources are not included.
 */
#define port_disable() _sim_()

/**
 * @brief   Disables the interrupt sources that are not supposed to preempt
 *          the kernel.
 * @note    Same as @p port_disable() in this port, there is no difference
 *          between the two states.
 */
#define port_suspend() _sim_()

/**
 * @brief   Enables all the interrupt sources.
 * @note    Implemented as global interrupt enable.
 */
#define port_enable() _rim_()

/**
 * @brief   Enters an architecture-dependent halt mode.
 * @note    Implemented with the specific "wfi" instruction.
 */
#define port_wait_for_interrupt() _wfi_()

#ifdef __cplusplus
extern "C" {
#endif
  void port_halt(void);
  void port_switch(Thread *otp, Thread *ntp);
  void _port_thread_start(void);
#ifdef __cplusplus
}
#endif

#endif /* _CHCORE_H_ */

/** @} */
