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
 * @file    GCC/ARMCM0/nilcore.c
 * @brief   Nil RTOS Cortex-M0 port main source file.
 *
 * @defgroup ARMCM0_CORE
 * @{
 */

#include "nil.h"

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module interrupt handlers.                                                */
/*===========================================================================*/

/**
 * @brief   System Timer vector.
 * @details This interrupt is used as system tick.
 * @note    The timer must be initialized in the startup code.
 */
NIL_IRQ_HANDLER(SysTickVector) {

  NIL_IRQ_PROLOGUE();

  nilSysTimerHandler();

  NIL_IRQ_EPILOGUE();
}

#if !CORTEX_ALTERNATE_SWITCH || defined(__DOXYGEN__)
/**
 * @brief   NMI vector.
 * @details The NMI vector is used for exception mode re-entering after a
 *          context switch.
 */
void NMIVector(void) {
  register struct port_extctx *ctxp;

  /* Discarding the current exception context and positioning the stack to
     point to the real one.*/
  asm volatile ("mrs     %0, PSP" : "=r" (ctxp) : : "memory");
  ctxp++;
  asm volatile ("msr     PSP, %0" : : "r" (ctxp) : "memory");
  port_unlock_from_isr();
}
#endif /* !CORTEX_ALTERNATE_SWITCH */

#if CORTEX_ALTERNATE_SWITCH || defined(__DOXYGEN__)
/**
 * @brief   PendSV vector.
 * @details The PendSV vector is used for exception mode re-entering after a
 *          context switch.
 */
void PendSVVector(void) {
  register struct extctx *ctxp;

  /* Discarding the current exception context and positioning the stack to
     point to the real one.*/
  asm volatile ("mrs     %0, PSP" : "=r" (ctxp) : : "memory");
  ctxp++;
  asm volatile ("msr     PSP, %0" : : "r" (ctxp) : "memory");
}
#endif /* CORTEX_ALTERNATE_SWITCH */

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   IRQ epilogue code.
 *
 * @param[in] lr        value of the @p LR register on ISR entry
 */
void _port_irq_epilogue(regarm_t lr) {

  if (lr != (regarm_t)0xFFFFFFF1) {
    register struct port_extctx *ctxp;

    port_lock_from_isr();
    /* Adding an artificial exception return context, there is no need to
       populate it fully.*/
    asm volatile ("mrs     %0, PSP" : "=r" (ctxp) : : "memory");
    ctxp--;
    asm volatile ("msr     PSP, %0" : : "r" (ctxp) : "memory");
    ctxp->xpsr = (regarm_t)0x01000000;

    /* The context switch is handled outside the ISR context..*/
    ctxp->pc = (void *)_port_switch_from_isr;

    /* Note, returning without unlocking is intentional, this is done in
       order to keep the rest of the context switching atomic.*/
  }
}

/**
 * @brief   Post-IRQ switch code.
 * @details The switch is performed in thread context then an NMI exception
 *          is enforced in order to return to the exact point before the
 *          preemption.
 */
#if !defined(__DOXYGEN__)
__attribute__((naked))
#endif
void _port_switch_from_isr(void) {

  nilSchRescheduleS();
#if CORTEX_ALTERNATE_SWITCH
  SCB_ICSR = ICSR_PENDSVSET;
  port_unlock();
#else
  SCB_ICSR = ICSR_NMIPENDSET;
#endif
  /* The following loop should never be executed, the exception will kick in
     immediately.*/
  while (TRUE)
    ;
}

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
#if !defined(__DOXYGEN__)
__attribute__((naked))
#endif
void _port_switch(Thread *ntp, Thread *otp) {
  register struct port_intctx *r13 asm ("r13");

  asm volatile ("push    {r4, r5, r6, r7, lr}                   \n\t"
                "mov     r4, r8                                 \n\t"
                "mov     r5, r9                                 \n\t"
                "mov     r6, r10                                \n\t"
                "mov     r7, r11                                \n\t"
                "push    {r4, r5, r6, r7}" : : : "memory");

  otp->ctxp = r13;
  r13 = ntp->ctxp;

  asm volatile ("pop     {r4, r5, r6, r7}                       \n\t"
                "mov     r8, r4                                 \n\t"
                "mov     r9, r5                                 \n\t"
                "mov     r10, r6                                \n\t"
                "mov     r11, r7                                \n\t"
                "pop     {r4, r5, r6, r7, pc}" : : "r" (r13) : "memory");
}

/**
 * @brief   Start a thread by invoking its work function.
 */
#if !defined(__DOXYGEN__)
__attribute__((naked))
#endif
void _port_thread_start(void) {

  nilSysUnlock();
  asm volatile ("mov     r0, r5                                 \n\t"
                "blx     r4");
  nilDbgAssert(TRUE, "_port_thread_start(), #1", "thread returned");
}

/**
 * @brief   Halts the system.
 * @note    The function is declared as a weak symbol, it is possible
 *          to redefine it in your application code.
 */
#if !defined(__DOXYGEN__)
__attribute__((naked, weak))
#endif
void port_halt(void) {

  port_disable();
  while (TRUE) {
  }
}

/** @} */
