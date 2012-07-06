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
 * @file    nil.h
 * @brief   Nil RTOS main header file.
 * @details This header includes all the required kernel headers so it is the
 *          only kernel header you usually want to include in your application.
 *
 * @addtogroup nil
 * @{
 */

#ifndef _NIL_H_
#define _NIL_H_

#include "nilconf.h"
#include "niltypes.h"

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    Nil RTOS identification
 * @{
 */
#define _NIL_                       /**< @brief Nil RTOS identification.    */

#define NIL_KERNEL_VERSION  "0.0.1" /**< @brief Kernel version string.      */

#define NIL_KERNEL_MAJOR    0       /**< @brief Kernel version major number.*/
#define NIL_KERNEL_MINOR    0       /**< @brief Kernel version minor number.*/
#define NIL_KERNEL_PATCH    1       /**< @brief Kernel version patch number.*/
/** @} */

/**
 * @name    Common constants
 */
#if !defined(FALSE) || defined(__DOXYGEN__)
#define FALSE               0       /**< @brief Generic 'false' constant.   */
#endif

#if !defined(TRUE) || defined(__DOXYGEN__)
#define TRUE                (!FALSE)/**< @brief Generic 'true' constant.    */
#endif
/** @} */

/**
 * @name    Wakeup status codes
 * @{
 */
#define NIL_MSG_OK          0       /**< @brief Normal wakeup message.      */
#define NIL_MSG_TMO         -1      /**< @brief Wake-up caused by a timeout
                                         condition.                         */
#define NIL_MSG_RST         -2      /**< @brief Wake-up caused by a reset
                                         condition.                         */
/** @} */

/**
 * @name    Special time constants
 * @{
 */
/**
 * @brief   Zero time specification for some functions with a timeout
 *          specification.
 * @note    Not all functions accept @p TIME_IMMEDIATE as timeout parameter,
 *          see the specific function documentation.
 */
#define TIME_IMMEDIATE  ((systime_t)0)

/**
 * @brief   Infinite time specification for all functions with a timeout
 *          specification.
 */
#define TIME_INFINITE   ((systime_t)-1)
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   Number of threads in the application.
 * @note    This number is not inclusive of the idle thread which is
 *          implicitely handled.
 */
#if !defined(NIL_CFG_NUM_THREADS) || defined(__DOXYGEN__)
#define NIL_CFG_NUM_THREADS             2
#endif

/**
 * @brief   System tick frequency.
 */
#if !defined(NIL_FREQUENCY) || defined(__DOXYGEN__)
#define NIL_CFG_FREQUENCY               100
#endif

/**
 * @brief   System assertions.
 */
#if !defined(NIL_CFG_ENABLE_ASSERTS) || defined(__DOXYGEN__)
#define NIL_CFG_ENABLE_ASSERTS          TRUE
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if NIL_CFG_NUM_THREADS < 1
#error "at least one thread must be defined"
#endif

#if NIL_CFG_NUM_THREADS > 12
#error "nil is not recommended for thread-intensive applications, consider" \
       "ChibiOS/RT instead"
#endif

#if NIL_CFG_FREQUENCY <= 0
#error "invalid NIL_CFG_FREQUENCY specified"
#endif

#if NIL_CFG_ENABLE_ASSERTS
#define NIL_DBG_ENABLED                 TRUE
#else
#define NIL_DBG_ENABLED                 FALSE
#endif

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of internal context structure.
 */
typedef struct port_intctx intctx;

/**
 * @brief   Type of a structure representing a counting semaphore.
 */
typedef struct {
  volatile cnt_t    cnt;        /**< @brief Semaphore counter.              */
} Semaphore;

/**
 * @brief Thread function.
 */
typedef msg_t (*tfunc_t)(void *);

/**
 * @brief   Type of a structure representing a thread static configuration.
 */
typedef struct nil_thread_cfg ThreadConfig;

/**
 * @brief   Type of a structure representing a thread.
 */
typedef struct nil_thread Thread;

/**
 * @brief   Structure representing a thread static configuration.
 */
struct nil_thread_cfg {
  const char        *namep;     /**< @brief Thread name, for debugging.     */
  tfunc_t           funcp;      /**< @brief Thread function.                */
  void              *arg;       /**< @brief Thread function argument.       */
  void              *wap;       /**< @brief Thread working area base.       */
  size_t            size;       /**< @brief Thread working area size.       */
};

/**
 * @brief   Structure representing a thread.
 */
struct nil_thread {
  intctx            *ctxp;      /**< @brief Pointer to internal context.    */
  bool_t            timeout;    /**< @brief Timeout flags.                  */
  union {
    void            *p;         /**< @brief Generic pointer to object.      */
    Semaphore       *semp;      /**< @brief Pointer to semaphore.           */
    Thread          *thdp;      /**< @brief Pointer to thread.              */
  } waitobj;                    /**< @brief Pointer to the wake-up object.  */
  union {
    systime_t       time;       /**< @brief Timeout deadline, if enabled.   */
    msg_t           msg;        /**< @brief Timeout message.                */
  } wakeup;                     /**< @brief Wake-up related info.           */
};

/**
 * @brief   System data structure.
 * @note    This structure contain all the data areas used by the OS except
 *          stacks.
 */
typedef struct {
  /**
   * @brief   Pointer to the running thread.
   */
  Thread            *currp;
  /**
   * @brief   Pointer to the next thread to be executed.
   * @note    This pointer must point at the same thread pointed by @p currp
   *          or to an higher priority thread in a switch is required.
   */
  Thread            *nextp;
  /**
   * @brief   System time.
   */
  systime_t         systime;
  /**
   * @brief   Thread structures for all the defined threads.
   */
  Thread            threads[NIL_CFG_NUM_THREADS + 1];
#if NIL_DBG_ENABLED || defined(__DOXYGEN__)
  /**
   * @brief   Panic message.
   * @note    This field is only present if some debug option has been
   *          activated.
   */
  const char        *dbg_msg;
#endif
} NilSystem;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @name    Threads tables definition macros
 * @{
 */
/**
 * @brief   Start of user threads table.
 */
#define NIL_THREADS_TABLE_BEGIN()                                           \
  const ThreadConfig nil_thd_configs[NIL_CFG_NUM_THREADS + 1] = {

/**
 * @brief   Entry of user threads table
 */
#define NIL_THREADS_TABLE_ENTRY(name, funcp, arg, wap, size)                \
  {name, funcp, arg, wap, size},

/**
 * @brief   End of user threads table.
 */
#define NIL_THREADS_TABLE_END(iwap, isize)                                  \
  {"idle", 0, NULL, iwap, isize}                                            \
};

/** @} */

/**
 * @name    Macro Functions
 * @{
 */
/**
 * @brief   System halt state.
 */
#define nilSysHalt() port_halt()

/**
 * @brief   Enters the kernel lock mode.
 *
 * @special
 */
#define nilSysDisable() port_disable()

/**
 * @brief   Enters the kernel lock mode.
 *
 * @special
 */
#define nilSysEnable() port_enable()

/**
 * @brief   Enters the kernel lock mode.
 *
 * @special
 */
#define nilSysLock() port_lock()

/**
 * @brief   Leaves the kernel lock mode.
 *
 * @special
 */
#define nilSysUnlock() port_unlock()

/**
 * @brief   Enters the kernel lock mode from within an interrupt handler.
 * @note    This API may do nothing on some architectures, it is required
 *          because on ports that support preemptable interrupt handlers
 *          it is required to raise the interrupt mask to the same level of
 *          the system mutual exclusion zone.<br>
 *          It is good practice to invoke this API before invoking any I-class
 *          syscall from an interrupt handler.
 * @note    This API must be invoked exclusively from interrupt handlers.
 *
 * @special
 */
#define nilSysLockFromIsr() port_lock_from_isr()

/**
 * @brief   Leaves the kernel lock mode from within an interrupt handler.
 *
 * @note    This API may do nothing on some architectures, it is required
 *          because on ports that support preemptable interrupt handlers
 *          it is required to raise the interrupt mask to the same level of
 *          the system mutual exclusion zone.<br>
 *          It is good practice to invoke this API after invoking any I-class
 *          syscall from an interrupt handler.
 * @note    This API must be invoked exclusively from interrupt handlers.
 *
 * @special
 */
#define nilSysUnlockFromIsr() port_unlock_from_isr()

/**
 * @brief   Suspends the invoking thread for the specified time.
 *
 * @param[in] time      the delay in system ticks, the special values are
 *                      handled as follow:
 *                      - @a TIME_INFINITE the thread enters an infinite sleep
 *                        state.
 *                      - @a TIME_IMMEDIATE this value is not allowed.
 *                      .
 * @api
 */
#define nilThdSleep(time) nilThdSleepUntil(nilTimeNow() + (time))

/**
 * @brief   Suspends the invoking thread for the specified time.
 *
 * @param[in] time      the delay in system ticks, the special values are
 *                      handled as follow:
 *                      - @a TIME_INFINITE the thread enters an infinite sleep
 *                        state.
 *                      - @a TIME_IMMEDIATE this value is not allowed.
 *                      .
 * @sclass
 */
#define nilThdSleepS(time) nilThdSleepUntilS(nilTimeNow() + (time))

/**
 * @brief   Suspends the invoking thread until the system time arrives to the
 *          specified value.
 *
 * @param[in] time      absolute system time
 *
 * @sclass
 */
#define nilThdSleepUntilS(time) nilSchGoSleepTimeoutS(nil.currp, (time))

/**
 * @brief   Initializes a semaphore with the specified counter value.
 *
 * @param[out] sp       pointer to a @p Semaphore structure
 * @param[in] n         initial value of the semaphore counter. Must be
 *                      non-negative.
 *
 * @init
 */
#define nilSemInit(sp, n) ((sp)->cnt = n)

/**
 * @brief   Performs a wait operation on a semaphore.
 *
 * @param[in] sp        pointer to a @p Semaphore structure
 * @return              A message specifying how the invoking thread has been
 *                      released from the semaphore.
 * @retval NIL_MSG_OK   if the thread has not stopped on the semaphore or the
 *                      semaphore has been signaled.
 * @retval NIL_MSG_RST  if the semaphore has been reset using @p nilSemReset().
 *
 * @api
 */
#define nilSemWait(sp) nilSemWaitTimeout(sp, TIME_INFINITE)

/**
 * @brief   Performs a wait operation on a semaphore.
 *
 * @param[in] sp        pointer to a @p Semaphore structure
 * @return              A message specifying how the invoking thread has been
 *                      released from the semaphore.
 * @retval NIL_MSG_OK   if the thread has not stopped on the semaphore or the
 *                      semaphore has been signaled.
 * @retval NIL_MSG_RST  if the semaphore has been reset using @p nilSemReset().
 *
 * @sclass
 */
#define nilSemWaitS(sp) nilSemWaitTimeoutS(sp, TIME_INFINITE)

/**
 * @brief   Current system time.
 * @details Returns the number of system ticks since the @p nilSysInit()
 *          invocation.
 * @note    The counter can reach its maximum and then restart from zero.
 * @note    This function is designed to work with the @p nilThdSleepUntil().
 *
 * @return              The system time in ticks.
 *
 * @api
 */
#define nilTimeNow() (nil.systime)
/** @} */

/**
 * @name    Threads abstraction macros
 */
/**
 * @brief   Thread declaration macro.
 * @note    Thread declarations should be performed using this macro because
 *          the port layer could define optimizations for thread functions.
 */
#if !defined(PORT_THREAD) || defined(__DOXYGEN__)
#define NIL_THREAD(tname, arg) msg_t tname(void *arg)
#else
#define NIL_THREAD(tname, arg) PORT_THREAD(tname, arg)
#endif

#if NIL_CFG_ENABLE_ASSERTS || defined(__DOXYGEN__)
/**
 * @brief   Condition assertion.
 * @details If the condition check fails then the kernel panics with the
 *          specified message and halts.
 * @note    The condition is tested only if the @p NIL_CFG_ENABLE_ASSERTS
 *          switch is specified in @p nilconf.h else the macro does nothing.
 * @note    The convention for the message is the following:<br>
 *          @<function_name@>(), #@<assert_number@>
 * @note    The remark string is not currently used except for putting a
 *          comment in the code about the assertion.
 *
 * @param[in] c         the condition to be verified to be true
 * @param[in] m         the text message
 * @param[in] r         a remark string
 *
 * @api
 */
#if !defined(nilDbgAssert)
#define nilDbgAssert(c, m, r) {                                             \
  if (!(c)) {                                                               \
    nil.dbg_msg = (m);                                                      \
    nilSysHalt();                                                           \
  }                                                                         \
}
#endif /* !defined(chDbgAssert) */
#else /* !NIL_CFG_ENABLE_ASSERTS */
#define nilDbgAssert(c, m, r) {(void)(c);}
#endif /* !NIL_CFG_ENABLE_ASSERTS */
/** @} */

/**
 * @name    ISRs abstraction macros
 */
/**
 * @brief   IRQ handler enter code.
 * @note    Usually IRQ handlers functions are also declared naked.
 * @note    On some architectures this macro can be empty.
 *
 * @special
 */
#define NIL_IRQ_PROLOGUE() PORT_IRQ_PROLOGUE()

/**
 * @brief   IRQ handler exit code.
 * @note    Usually IRQ handlers function are also declared naked.
 *
 * @special
 */
#define NIL_IRQ_EPILOGUE() PORT_IRQ_EPILOGUE()

/**
 * @brief   Standard normal IRQ handler declaration.
 * @note    @p id can be a function name or a vector number depending on the
 *          port implementation.
 *
 * @special
 */
#define NIL_IRQ_HANDLER(id) PORT_IRQ_HANDLER(id)
/** @} */

/**
 * @name    Fast ISRs abstraction macros
 */
/**
 * @brief   Standard fast IRQ handler declaration.
 * @note    @p id can be a function name or a vector number depending on the
 *          port implementation.
 * @note    Not all architectures support fast interrupts.
 *
 * @special
 */
#define NIL_FAST_IRQ_HANDLER(id) PORT_FAST_IRQ_HANDLER(id)
/** @} */

/**
 * @name    Time conversion utilities
 * @{
 */
/**
 * @brief   Seconds to system ticks.
 * @details Converts from seconds to system ticks number.
 * @note    The result is rounded upward to the next tick boundary.
 *
 * @param[in] sec       number of seconds
 * @return              The number of ticks.
 *
 * @api
 */
#define S2ST(sec)   ((systime_t)((sec) * NIL_CFG_FREQUENCY))

/**
 * @brief   Milliseconds to system ticks.
 * @details Converts from milliseconds to system ticks number.
 * @note    The result is rounded upward to the next tick boundary.
 *
 * @param[in] msec      number of milliseconds
 * @return              The number of ticks.
 *
 * @api
 */
#define MS2ST(msec) ((systime_t)(((((msec) - 1L) * NIL_CFG_FREQUENCY) /     \
                                  1000L) + 1L))

/**
 * @brief   Microseconds to system ticks.
 * @details Converts from microseconds to system ticks number.
 * @note    The result is rounded upward to the next tick boundary.
 *
 * @param[in] usec      number of microseconds
 * @return              The number of ticks.
 *
 * @api
 */
#define US2ST(usec) ((systime_t)(((((usec) - 1L) * NIL_CFG_FREQUENCY) /     \
                                  1000000L) + 1L))
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#include "nilcore.h"

#if !defined(__DOXYGEN__)
extern NilSystem nil;
extern const ThreadConfig nil_thd_configs[NIL_CFG_NUM_THREADS + 1];
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void nilSysInit(void);
  void nilSysTimerHandlerI(void);
  Thread *nilSchReadyI(Thread *tp);
  msg_t nilSchGoSleepTimeoutS(void *waitobj, systime_t time);
  void nilSchRescheduleS(void);
  void nilThdSleepUntil(systime_t time);
  bool_t nilTimeIsWithin(systime_t start, systime_t end);
  msg_t nilSemWaitTimeout(Semaphore *sp, systime_t time);
  msg_t nilSemWaitTimeoutS(Semaphore *sp, systime_t time);
  void nilSemSignal(Semaphore *sp);
  void nilSemSignalI(Semaphore *sp);
  void nilSemReset(Semaphore *sp, cnt_t n);
  void nilSemResetI(Semaphore *sp, cnt_t n);
#ifdef __cplusplus
}
#endif

#endif /* _NIL_H_ */

/** @} */
