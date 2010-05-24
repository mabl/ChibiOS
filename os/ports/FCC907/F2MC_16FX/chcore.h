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
 * @file ports/F2MC_16FX/chcore.h
 * @brief 16FX architecture port macros and structures.
 * @addtogroup 16FX_CORE
 * @{
 */

#ifndef _CHCORE_H_
#define _CHCORE_H_


/**
 * If enabled allows the idle thread to enter a low power mode.
 */
#ifndef ENABLE_WFI_IDLE
#define ENABLE_WFI_IDLE 0
#endif

/**
 * Macro defining the 16FX architecture.
 */
#define CH_ARCHITECTURE_F2MC_16FX

/**
 * Name of the implemented architecture.
 */
#define CH_ARCHITECTURE_NAME "F2MC_16FX"

/**
 * 16 bit stack alignment.
 */
typedef uint16_t stkalign_t;

/** Thread function.*/
typedef msg_t (*port_tfunc_t)(void *args);

/** Exit function.*/
typedef void (*exitfunc_t)();

/** Thread start function.(only 16bit value, also for medium model, the prog. counter bank need to be saved into the context) */
typedef __near void * tstart_func_t;

/**
 * Fujitsu 16FX basic registers
 */
typedef void *reg16fx_t;

/**
 * Accumulators 
 */
typedef uint16_t reg16fx_acc_t;

/**
 * Page extension register or direct page register
 */
typedef uint8_t reg16fx_page_t;

typedef uint8_t reg16fx_dpr_t;
typedef uint8_t reg16fx_adb_t;
typedef uint8_t reg16fx_dtb_t;
typedef uint8_t reg16fx_pcb_t;

/**
 * Status register 
 */
struct reg16fx_status_t{
	unsigned int ccr : 8; 		// condition code register
	unsigned int rp : 5; 		// register bank pointer
	unsigned int ilm: 3; 		// interrupt level mask register
};

typedef struct reg16fx_status_t reg16fx_status_t;

/** @cond never */
/**
 * Interrupt saved context.
 */
struct extctx {
/**
 * Interrupt saved context, not empty in this architecture.
 */
  reg16fx_status_t 	ps;
  tstart_func_t	     pc;		// just the irq return prog. counter
  reg16fx_page_t     pcb;
  reg16fx_page_t     dtb;
  reg16fx_page_t     adb;
  reg16fx_page_t     dpr;
  reg16fx_acc_t      al;
  reg16fx_acc_t      ah;
};
/** @endcond */

/** @cond never */
/**
 * This structure represents the inner stack frame during a context switching.
 */
struct intctx {
  reg16fx_acc_t      rw0;
  reg16fx_acc_t      rw1;
  reg16fx_acc_t      rw2;
  reg16fx_acc_t      rw3;
  reg16fx_acc_t      rw4;
  reg16fx_acc_t      rw5;
  reg16fx_acc_t      rw6;
  reg16fx_acc_t      rw7;

  reg16fx_status_t 	ps;
  tstart_func_t	     pc;		// just the start thread prog. counter
  reg16fx_page_t     pcb;
  reg16fx_page_t     dtb;
  reg16fx_page_t     adb;
  reg16fx_page_t     dpr;
  reg16fx_acc_t      al;
  reg16fx_acc_t      ah;

  exitfunc_t		 lr;		// the exit function
  port_tfunc_t		 pf;		// the thread main function
  void      	 	*arg;		// the thread arguments
};
/** @endcond */

/** @cond never */
/**
 * In the 16FX port this structure just holds a pointer to the @p intctx
 * structure representing the stack pointer at the time of the context switch.
 */
struct context {
  struct intctx *sp;
};
/** @endcond */


/**
 * Platform dependent part of the @p chThdInit() API.
 */
#define SETUP_CONTEXT(workspace, wsize, pf, arg) {             \
  tp->p_ctx.sp = (struct intctx *)((uint8_t *)workspace +     \
                                   wsize -                     \
                                   sizeof(struct intctx));    \
  tp->p_ctx.sp->lr = &threadoff;                               \
  tp->p_ctx.sp->pf = pf;                                       \
  tp->p_ctx.sp->arg = arg;                                     \
  tp->p_ctx.sp->ah = 0;                                        \
  tp->p_ctx.sp->al = 0;                                        \
  tp->p_ctx.sp->dpr = read_reg_dpr(); 							\
  tp->p_ctx.sp->adb = read_reg_adb(); 							\
  tp->p_ctx.sp->pcb = read_reg_pcb( (uint32_t) &threadstart);	\
  tp->p_ctx.sp->dtb = read_reg_dtb(); 							\
  tp->p_ctx.sp->pc = (tstart_func_t) threadstart;      			\
  tp->p_ctx.sp->ps.ilm = 0x07;		   							\
  tp->p_ctx.sp->ps.rp = 0;		   								\
  tp->p_ctx.sp->ps.ccr = 0x60;	 \
  tp->p_ctx.sp->rw0 = 0x0000;	 \
  tp->p_ctx.sp->rw1 = 0x1111;	 \
  tp->p_ctx.sp->rw2 = 0x2222;	 \
  tp->p_ctx.sp->rw3 = 0x3333;	 \
  tp->p_ctx.sp->rw4 = 0x4444;	 \
  tp->p_ctx.sp->rw5 = 0x5555;	 \
  tp->p_ctx.sp->rw6 = 0x6666;	 \
  tp->p_ctx.sp->rw7 = 0x7777;	 \
}// ILM = 0x07 => all irq on, RP=0 => always bank 0, CCR=0x60 (supv stack + irq en); 

/**
 * The default idle thread implementation requires no extra stack space in
 * this port.
 */
#ifndef IDLE_THREAD_STACK_SIZE
#define IDLE_THREAD_STACK_SIZE 32
#endif

/**
 * Per-thread stack overhead for interrupts servicing, it is used in the
 * calculation of the correct working area size. In this port the default is
 * 32 bytes per thread.
 */
#ifndef INT_REQUIRED_STACK
#define INT_REQUIRED_STACK 32
#endif

/**
 * Enforces a correct alignment for a stack area size value.
 */
#define STACK_ALIGN(n) ((((n) - 1) | (sizeof(stkalign_t) - 1)) + 1)


/**
 * Computes the thread working area global size.
 */
#define THD_WA_SIZE(n) STACK_ALIGN(sizeof(Thread) +                     \
                                   sizeof(struct intctx) +              \
                                   sizeof(struct extctx) +              \
                                  (n) + (INT_REQUIRED_STACK))

/**
 * Macro used to allocate a thread working area aligned as both position and
 * size.
 */
#define WORKING_AREA(s, n) stkalign_t s[THD_WA_SIZE(n) / sizeof(stkalign_t)]

/**
 * Macro used to calculate the thread space area aligned as both position and
 * size.
 */
#define WORKING_SIZE(n) (THD_WA_SIZE(n) / sizeof(stkalign_t))


/**
 * IRQ prologue code, inserted at the start of all IRQ handlers enabled to
 * invoke system APIs.
 */
#define PORT_IRQ_PROLOGUE()

/**
 * Irq priority of the system timer that drives the system ticks. This also
 * defines the software interrupt priority.
 */
#if !defined(CH_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define CH_IRQ_PRIORITY                    4
#endif

/**
 * IRQ epilogue code, inserted at the end of all IRQ handlers enabled to
 * invoke system APIs.
 */
#define PORT_IRQ_EPILOGUE() {                                           \
  if (chSchIsRescRequiredExI())                                         \
    chSchDoRescheduleI();                                               \
}

/**
 * IRQ handler function modifier.
 */
#define PORT_IRQ_HANDLER(id)  __interrupt void id(void)

/**
 * This function is empty in this port.
 */
#define port_init()

/**
 * Implemented as interrupt level mask.
 */
#define port_lock() __set_il(CH_IRQ_PRIORITY)

/**
 * Implemented as interrupt level mask.
 */
#define port_unlock() __set_il(0x07)

/**
 * This function is empty in this port.
 */
#define port_lock_from_isr() { asm(" 	PUSHW PS");\
							 __set_il(CH_IRQ_PRIORITY);\
							 }

/**
 * This function is empty in this port.
 */
#define port_unlock_from_isr()	{ asm(" 	POPW PS"); }

/**
 * Implemented as global interrupt disable.
 */
#define port_disable() __DI()

/**
 * Same as @p port_disable() in this port, there is no difference between the
 * two states.
 */
#define port_suspend() __DI()

/**
 * Implemented as global interrupt enable.
 */
#define port_enable() __EI()

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

#ifdef __cplusplus
extern "C" {
#endif
  void port_switch(Thread *otp, Thread *ntp);

  void port_halt(void);
  void threadstart(port_tfunc_t pf, void *args);
  void threadoff(void);

  reg16fx_page_t read_reg_dpr(void);
  reg16fx_page_t read_reg_adb(void);
  reg16fx_page_t read_reg_pcb(uint32_t f);
  reg16fx_page_t read_reg_dtb(void);
  
#ifdef __cplusplus
}
#endif

#endif /* _CHCORE_H_ */

/** @} */
