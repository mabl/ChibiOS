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
 * @file    RX600/chcore.h
 * @brief   RX600 architecture port macros and structures.
 *
 * @addtogroup RX600_CORE
 * @{
 */

#ifndef _CHCORE_H_
#define _CHCORE_H_

/*===========================================================================*/
/* Port constants.                                                           */
/*===========================================================================*/

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
 * @brief   If enabled allows the idle thread to enter a low power mode.
 */
#ifndef RX600_ENABLE_WFI_IDLE
#define RX600_ENABLE_WFI_IDLE         FALSE
#endif

/*===========================================================================*/
/* Port exported info.                                                       */
/*===========================================================================*/

/**
 * @brief   Unique macro for the implemented architecture.
 */
#define CH_ARCHITECTURE_RX600

/**
 * @brief   Name of the implemented architecture.
 */
#define CH_ARCHITECTURE_NAME    "RX600"

/*===========================================================================*/
/* Port implementation part (common).                                        */
/*===========================================================================*/

/**
 * @brief   32 bits stack and memory alignment enforcement.
 */
typedef uint32_t stkalign_t;

/** Thread function.*/
typedef msg_t (*port_tfunc_t)(void *args);

/** Exit function.*/
typedef void (*exitfunc_t)();


/**
 * @brief   Generic RX600 register.
 */
typedef void *regrx600_t;

/**
 * @brief   Generic dsp RX600 accumulator register.
 */
typedef long long regrx600acc_t;

/**
 * @brief   Generic fp RX600 register.
 */
typedef float regfprx_t;

/**
 * Status register 
 */
struct regrx600_status_t{
	unsigned int C : 1; 		// carry flag
	unsigned int Z : 1; 		// zero flag
	unsigned int S : 1; 		// sign flag
	unsigned int O : 1; 		// overflow flag
	unsigned int nu1 : 12; 		// spare
	unsigned int I : 1; 		// irq enable flag (1=enabled)
	unsigned int U : 1; 		// user stack select (1=user stack, 0=supv stack)
	unsigned int nu2 : 2; 		// spare
	unsigned int PM : 1; 		// Processor mode select bit (0: Supervisor mode is selected)
	unsigned int nu3 : 3; 		// spare
	unsigned int IPL : 4; 		// Processor interrupt priority level
								// Priority level 0 (lowest)
								// Priority level 15 (highest)
};

typedef struct regrx600_status_t regrx600_status_t;


/**
 * @brief   Interrupt saved context.
 * @details This structure represents the stack frame saved during a
 *          preemption-capable interrupt handler.
 */
struct extctx {
  regrx600_t      		r1;
  regrx600_t      		r2;
  regrx600_t      		r3;
  regrx600_t      		r4;
  regrx600_t      		r14;
  regrx600_status_t   psw;
  regrx600_t      		pc;
};

/**
 * @brief   System saved context.
 * @details This structure represents the inner stack frame during a context
 *          switching.
 */
struct intctx {
  regrx600_t      r5;
  regrx600_t      r6;
  regrx600_t      r7;
  regrx600_t      r8;
  regrx600_t      r9;
  regrx600_t      r10;
  regrx600_t      r11;
  regrx600_t      r12;
  regrx600_t      r13;
#ifndef CH_CURRP_REGISTER_CACHE
  regrx600_t      r15;
#endif
  regrx600_status_t   psw;
  regrx600_t      	 pc;		// the thread start function
  exitfunc_t		 lr;		// the exit function
};

/**
 * @brief   Platform dependent part of the @p Thread structure.
 * @details In this port the structure just holds a pointer to the @p intctx
 *          structure representing the stack pointer at context switch time.
 */
struct context {
  struct intctx *r0;
};

/**
 * @brief   Platform dependent part of the @p chThdInit() API.
 * @details This code usually setup the context switching frame represented
 *          by an @p intctx structure.
 */
#define SETUP_CONTEXT(workspace, wsize, pf, arg) {                          \
  tp->p_ctx.r0 = (struct intctx *)((uint8_t *)workspace +                  \
                                     wsize -                                \
                                     sizeof(struct intctx));                \
  tp->p_ctx.r0->r5 = pf;	 \
  tp->p_ctx.r0->r6 = arg;	 \
  tp->p_ctx.r0->r7 = (regrx600_t) 0x7777;	 \
  tp->p_ctx.r0->r8 = (regrx600_t) 0x8888;	 \
  tp->p_ctx.r0->r9 = (regrx600_t) 0x9999;	 \
  tp->p_ctx.r0->r10 = (regrx600_t) 0xAAAA;	 \
  tp->p_ctx.r0->r11 = (regrx600_t) 0xBBBB;	 \
  tp->p_ctx.r0->r12 = (regrx600_t) 0xCCCC;	 \
  tp->p_ctx.r0->r13 = (regrx600_t) 0xDDDD;	 \
  tp->p_ctx.r0->r15 = (regrx600_t) 0xFFFF;	 \
  tp->p_ctx.r0->psw.IPL = 0x07;	\
  tp->p_ctx.r0->psw.PM = 0;	 \
  tp->p_ctx.r0->psw.I = 1;	 \
  tp->p_ctx.r0->psw.U = 0;	 \
  tp->p_ctx.r0->pc = &_port_thread_start;	\
  tp->p_ctx.r0->lr = &_port_thread_off; \
};

/**
 * @brief   Stack size for the system idle thread.
 * @details This size depends on the idle thread implementation, usually
 *          the idle thread should take no more space than those reserved
 *          by @p INT_REQUIRED_STACK.
 * @note    In this port it is set to 4 because the idle thread does have
 *          a stack frame when compiling without optimizations.
 */
#ifndef IDLE_THREAD_STACK_SIZE
#define IDLE_THREAD_STACK_SIZE      16
#endif

/**
 * @brief   Per-thread stack overhead for interrupts servicing.
 * @details This constant is used in the calculation of the correct working
 *          area size.
 *          This value can be zero on those architecture where there is a
 *          separate interrupt stack and the stack space between @p intctx and
 *          @p extctx is known to be zero.
 * @note    In this port 0x20 is a safe value, it can be reduced after careful
 *          analysis of the generated code.
 */
#ifndef INT_REQUIRED_STACK
#define INT_REQUIRED_STACK          0x20
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
#define WORKING_AREA(s, n) stkalign_t s[THD_WA_SIZE(n) / sizeof(stkalign_t)]

/**
 * @brief   IRQ prologue code.
 * @details This macro must be inserted at the start of all IRQ handlers
 *          enabled to invoke system APIs.
 */
#define PORT_IRQ_PROLOGUE() {                                               \
  asm volatile ("pushm    r14			\n\t" \
                "pushm    r1-r4");           \
}

/**
 * @brief   IRQ epilogue code.
 * @details This macro must be inserted at the end of all IRQ handlers
 *          enabled to invoke system APIs.
 */
#define PORT_IRQ_EPILOGUE() {                   \
  if (chSchIsRescRequiredExI())                 \
    chSchDoRescheduleI();                       \
  asm volatile ("popm    r14			\n\t" 	\
                "popm    r1-r4");           \
}

/**
 * @brief   IRQ handler function declaration.
 * @note    @p id can be a function name or a vector number depending on the
 *          port implementation.
 */
#define PORT_IRQ_HANDLER(id) __attribute__((naked)) void id(void)

/**
 * @brief   Fast IRQ handler function declaration.
 * @note    @p id can be a function name or a vector number depending on the
 *          port implementation.
 */
#define PORT_FAST_IRQ_HANDLER(id)                                           \
  __attribute__((interrupt("Fast"))) void id(void)

/**
 * @brief   Port-related initialization code.
 * @note    This function is empty in this port.
 */
#define port_init()

/**
 * @brief   Kernel-lock action.
 * @details Usually this function just disables interrupts but may perform
 *          more actions.
 * @note    In this port it disables the IRQ sources and keeps FIQ sources
 *          enabled.
 */
#define port_lock() asm volatile ("CLRPSW   I")

/**
 * @brief   Kernel-unlock action.
 * @details Usually this function just disables interrupts but may perform
 *          more actions.
 * @note    In this port it enables both the IRQ and FIQ sources.
 */
#define port_unlock() asm volatile ("SETPSW  I")

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
 * @note    In this port it disables both the IRQ and FIQ sources.
 */
#define port_disable() {                                                    \
  asm volatile ("CLRPSW   I");                 \
}

/**
 * @brief   Disables the interrupt sources below kernel-level priority.
 * @note    Interrupt sources above kernel level remains enabled.
 * @note    In this port it disables the IRQ sources and enables the
 *          FIQ sources.
 */
#define port_suspend() asm volatile ("CLRPSW   I")

/**
 * @brief   Enables all the interrupt sources.
 * @note    In this port it enables both the IRQ and FIQ sources.
 */
#define port_enable() asm volatile ("SETPSW   I")

/**
 * This port function is implemented as inlined code for performance reasons.
 * @note The port code does not define a low power mode, this macro has to be
 *       defined externally. The default implementation is a "nop", not a
 *       real low power mode.
 */
#if ENABLE_WFI_IDLE != 0
#ifndef port_wait_for_interrupt
#define port_wait_for_interrupt() {                                     \
  asm volatile ("nop");                                                 \
}
#endif
#else
#define port_wait_for_interrupt()
#endif


/**
 * @brief   Performs a context switch between two threads.
 * @details This is the most critical code in any port, this function
 *          is responsible for the context switch between 2 threads.
 * @note    The implementation of this code affects <b>directly</b> the context
 *          switch performance so optimize here as much as you can.
 * @note    Implemented as inlined code for performance reasons.
 *
 * @param[in] ntp       the thread to be switched in
 * @param[in] otp       the thread to be switched out
 */
#if CH_DBG_ENABLE_STACK_CHECK
#define port_switch(ntp, otp) {                                             \
  register Thread *_ntp asm ("r1") = (ntp);                                 \
  register Thread *_otp asm ("r2") = (otp);                                 \
  register char *sp asm ("sp");                                             \
  if (sp - sizeof(struct intctx) - sizeof(Thread) < (char *)_otp)           \
    asm volatile ("mov     #0, r1                               \n\t"       \
                  "bsr.a    chDbgPanic");                                    \
  _port_switch_rx600(_ntp, _otp);                                           \
}
#else /* !CH_DBG_ENABLE_STACK_CHECK */
#define port_switch(ntp, otp) _port_switch_rx600(ntp, otp)
#endif /* !CH_DBG_ENABLE_STACK_CHECK */

#ifdef __cplusplus
extern "C" {
#endif
  void port_halt(void);
  void _port_switch(Thread *ntp, Thread *otp);
  void _port_thread_start(port_tfunc_t pf, void *args);
  void _port_thread_off(void);  
#ifdef __cplusplus
}
#endif

#endif /* _CHCORE_H_ */

/** @} */
