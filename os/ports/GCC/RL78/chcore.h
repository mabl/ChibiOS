/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013,2014 Giovanni Di Sirio.

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
 * @file    RL78/chcore.h
 * @brief   Renesas RL78 port macros and structures.
 *
 * @addtogroup RL78_CORE
 * @{
 */

#ifndef _CHCORE_H_
#define _CHCORE_H_

/*===========================================================================*/
/* Port constants.                                                           */
/*===========================================================================*/

/* Inclusion of the RL78 implementation specific parameters.*/

/* RL78 model check, only RL78G13 is supported right now.*/
#if (RL78_MODEL == RL78G13)
#else
/*#error "unknown or unsupported RL78 model"*/
#endif

/*===========================================================================*/
/* Port statically derived parameters.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Port macros.                                                              */
/*===========================================================================*/

/*===========================================================================*/
/* Port configurable parameters.                                             */
/*===========================================================================*/

/**
 * If enabled allows the idle thread to enter a low power mode.
 */
#ifndef RL78_ENABLE_WFI_IDLE
#define RL78_ENABLE_WFI_IDLE            FALSE
#endif

/*===========================================================================*/
/* Port exported info.                                                       */
/*===========================================================================*/

/**
 * Macro defining a generic RL78 architecture.
 */
#define CH_ARCHITECTURE_RL78

/**
 * @brief   Name of the implemented architecture.
 * @note    The value is for documentation only, the real value changes
 *          depending on the selected architecture, the possible values are:
 *          - "ARM7".
 *          - "ARM9".
 *          .
 */
#define CH_ARCHITECTURE_NAME            "RL78"

/**
 * @brief   Name of the compiler supported by this port.
 */
#define CH_COMPILER_NAME                "GCC " __VERSION__

/*===========================================================================*/
/* Port implementation part (common).                                        */
/*===========================================================================*/

/**
 * @brief   16 bits stack and memory alignment enforcement.
 */
typedef uint16_t stkalign_t;

/**
 * @brief Interrupt saved context.
 * @details This structure represents the stack frame saved during a
 * preemption-capable interrupt handler. These registers are caller-saved in GCC.
 */
struct extctx {
    /* Bank 0 and 1 registers */
    uint16_t    cs_es;
    uint16_t    hl_0;
    uint16_t    de_0;
    uint16_t    bc_0;
    uint16_t    ax_0;
    uint16_t    hl_1;
    uint16_t    de_1;
    uint16_t    bc_1;
    uint16_t    ax_1;
    uint32_t    psw_pc;
};

/**
 * @brief System saved context.
 * @details This structure represents the inner stack frame during a context
 * switching. These registers are callee-saved in GCC.
 */
struct intctx {
    /* Bank 2 registers */
    uint16_t    hl_2;
    uint16_t    de_2;
    uint16_t    bc_2;
    uint16_t    ax_2;
#ifdef CH_CURRP_REGISTER_CACHE
#error CH_CURRP_REGISTER_CACHE feature is not supported.
#endif
    uint32_t    pc;
};

/**
 * @brief   Platform dependent part of the @p Thread structure.
 * @details In this port the structure just holds a pointer to the @p intctx
 *          structure representing the stack pointer at context switch time.
 */
struct context {
  struct intctx *sp;
};

#define APUSH(p, a) (p) -= sizeof(void *), *(void **)(p) = (void*)(a)

/**
 * @brief   Platform dependent part of the @p chThdCreateI() API.
 * @details This code usually setup the context switching frame represented
 *          by an @p intctx structure.
 */
#define SETUP_CONTEXT(workspace, wsize, pf, arg) {                      \
  uint8_t *sp = (uint8_t *)workspace + wsize;                           \
  APUSH(sp, arg);                                                       \
  APUSH(sp, pf);                                                        \
  APUSH(sp, 0);                                                         \
  APUSH(sp, 0);                                                         \
  sp -= sizeof(struct intctx);                                          \
  ((struct intctx *)sp)->pc = (uint16_t)_port_thread_start;             \
  ((struct intctx *)sp)->ax_2 = 0x0000;                                 \
  ((struct intctx *)sp)->bc_2 = 0x1111;                                 \
  ((struct intctx *)sp)->de_2 = 0x2222;                                 \
  ((struct intctx *)sp)->hl_2 = 0x3333;                                 \
  tp->p_ctx.sp = (struct intctx *)sp;                                   \
}

/**
 * @brief   Stack size for the system idle thread.
 * @details This size depends on the idle thread implementation, usually
 *          the idle thread should take no more space than those reserved
 *          by @p PORT_INT_REQUIRED_STACK.
 */
#ifndef PORT_IDLE_THREAD_STACK_SIZE
#define PORT_IDLE_THREAD_STACK_SIZE     20
#endif

/**
 * @brief   Per-thread stack overhead for interrupts servicing.
 * @details This constant is used in the calculation of the correct working
 *          area size.
 *          This value can be zero on those architecture where there is a
 *          separate interrupt stack and the stack space between @p intctx and
 *          @p extctx is known to be zero.
 */
#ifndef PORT_INT_REQUIRED_STACK
#define PORT_INT_REQUIRED_STACK 64
#endif

/**
 * @brief   Enforces a correct alignment for a stack area size value.
 */
#define STACK_ALIGN(n) ((((n) - 1) | (sizeof(stkalign_t) - 1)) + 1)

/**
 * @brief   Computes the thread working area global size.
 */
#define THD_WA_SIZE(n) STACK_ALIGN(sizeof(Thread) +                           \
                                   sizeof(struct intctx) +                    \
                                   sizeof(struct extctx) +                    \
                                  (n) + (PORT_INT_REQUIRED_STACK))

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
 */
#define PORT_IRQ_PROLOGUE() {                                                 \
    asm volatile ("sel     rb1                                      \n\t"     \
                  "push    ax                                       \n\t"     \
                  "push    bc                                       \n\t"     \
                  "push    de                                       \n\t"     \
                  "push    hl                                       \n\t"     \
                  "sel     rb0                                      \n\t"     \
                  "push    ax                                       \n\t"     \
                  "push    bc                                       \n\t"     \
                  "push    de                                       \n\t"     \
                  "push    hl                                       \n\t"     \
                  "mov     a,cs                                     \n\t"     \
                  "push    ax                                       \n\t");   \
}

/**
 * @brief   IRQ epilogue code.
 * @details This macro must be inserted at the end of all IRQ handlers
 *          enabled to invoke system APIs.
 */
#define PORT_IRQ_EPILOGUE() {                                                 \
    dbg_check_lock();                                                         \
    if (chSchIsPreemptionRequired()) {                                        \
      asm volatile ("mov   a,PSW                                    \n\t");   \
      asm volatile ("or    a,#6                                     \n\t");   \
      asm volatile ("mov   PSW,a                                    \n\t");   \
      chSchDoReschedule();                                                    \
    }                                                                         \
    dbg_check_unlock();                                                       \
    asm volatile ("sel     rb0                                      \n\t"     \
                  "pop     ax                                       \n\t"     \
                  "mov     cs,a                                     \n\t"     \
                  "pop     hl                                       \n\t"     \
                  "pop     de                                       \n\t"     \
                  "pop     bc                                       \n\t"     \
                  "pop     ax                                       \n\t"     \
                  "sel     rb1                                      \n\t"     \
                  "pop     hl                                       \n\t"     \
                  "pop     de                                       \n\t"     \
                  "pop     bc                                       \n\t"     \
                  "pop     ax                                       \n\t"     \
                  "reti                                             \n\t");   \
}

/**
 * IRQ handler function declaration.
 * @note @p id can be a function name or a vector number depending on the
 *       port implementation.
 */
#define PORT_IRQ_HANDLER(id) void __attribute__((naked)) id(void)

/**
 * @brief Kernel-unlock action.
 * @details Usually this function just disables interrupts but may perform more
 * actions.
 */
#define port_lock() asm volatile ("di                               \n\t")

/**
 * @brief Kernel-unlock action.
 * @details Usually this function just disables interrupts but may perform more
 * actions.
 */
#define port_unlock() asm volatile ("ei                             \n\t")

/**
 * @brief Kernel-lock action from an interrupt handler.
 * @details This function is invoked before invoking I-class APIs from
 * interrupt handlers. The implementation is architecture dependent, in its
 * simplest form it is void.
 */
#define port_lock_from_isr()

/**
 * @brief Kernel-unlock action from an interrupt handler.
 * @details This function is invoked after invoking I-class APIs from interrupt
 * handlers. The implementation is architecture dependent, in its simplest form
 * it is void.
 */
#define port_unlock_from_isr()

/**
 * @brief   Enters an architecture-dependent IRQ-waiting mode.
 * @details The function is meant to return when an interrupt becomes pending.
 *          The simplest implementation is an empty function or macro but this
 *          would not take advantage of architecture-specific power saving
 *          modes.
 * @note    This port function is implemented as inlined code for performance
 *          reasons.
 */
#if RL78_ENABLE_WFI_IDLE == TRUE
#define port_wait_for_interrupt() {                                           \
  asm volatile ("halt");                                                      \
}
#else
#define port_wait_for_interrupt()
#endif


#ifdef __cplusplus
extern "C" {
#endif
  void port_init(void);
  void port_disable(void);
  void port_suspend(void);
  void port_enable(void);
  void port_halt(void);
  void port_switch(Thread *ntp, Thread *otp);
  void _port_thread_start(msg_t (*pf)(void *), void *p);
#ifdef __cplusplus
}
#endif

#endif /* _CHCORE_H_ */

/** @} */
