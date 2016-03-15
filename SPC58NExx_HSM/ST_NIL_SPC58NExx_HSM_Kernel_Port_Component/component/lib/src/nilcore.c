/*
    HSM RTOS - Copyright (C) 2016 STMicroelectronics.
*/

/* parasoft suppress item MISRA2012-DIR-4_2 "Some inline assembly code is used
   for context switch performance" */
/* parasoft suppress item MISRA2012-DIR-4_3 "Some inline assembly code is used
   for context switch performance" */

/**
 * @file    templates/nilcore.c
 * @brief   Port code.
 *
 * @addtogroup NIL_CORE
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
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Performs a context switch between two threads.
 * @details This is the most critical code in any port, this function
 *          is responsible for the context switch between 2 threads.
 * @note    The implementation of this code affects <b>directly</b> the context
 *          switch performance so optimize here as much as you can.
 */
#if !defined(__DOXYGEN__)
__attribute__((naked))
#endif
void port_dummy1(void) {

  asm (".global _port_switch");
  asm ("_port_switch:");
  asm ("subi        %sp, %sp, 80");     /* Size of the intctx structure.    */
  asm ("mflr        %r0");
  asm ("stw         %r0, 84(%sp)");     /* LR into the caller frame.        */
  asm ("mfcr        %r0");
  asm ("stw         %r0, 0(%sp)");      /* CR.                              */
  asm ("stmw        %r14, 4(%sp)");     /* GPR14...GPR31.                   */

  asm ("stw         %sp, 0(%r4)");      /* Store swapped-out stack.         */
  asm ("lwz         %sp, 0(%r3)");      /* Load swapped-in stack.           */

  asm ("lmw         %r14, 4(%sp)");     /* GPR14...GPR31.                   */
  asm ("lwz         %r0, 0(%sp)");      /* CR.                              */
  asm ("mtcr        %r0");
  asm ("lwz         %r0, 84(%sp)");     /* LR from the caller frame.        */
  asm ("mtlr        %r0");
  asm ("addi        %sp, %sp, 80");     /* Size of the intctx structure.    */
  asm ("blr");
}

/**
 * @brief   Start a thread by invoking its work function.
 * @details If the work function returns @p chThdExit() is automatically
 *          invoked.
 */
#if !defined(__DOXYGEN__)
__attribute__((naked))
#endif
void port_dummy2(void) {

  asm (".global _port_thread_start");
  asm ("_port_thread_start:");
  chSysUnlock();
  asm ("mr          %r3, %r31");        /* Thread parameter.                */
  asm ("mtctr       %r30");
  asm ("bctrl");                        /* Invoke thread function.          */
  asm ("li          %r0, 0");
  asm ("bl          chSysHalt");        /* Thread termination on exit.      */
}

/** @} */
