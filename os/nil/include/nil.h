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
 * @file    nil.h
 * @brief   Nil RTOS main header file.
 * @details This header includes all the required kernel headers so it is the
 *          only header you usually need to include in your application.
 *
 * @addtogroup NIL_KERNEL
 * @{
 */

#ifndef _NIL_H_
#define _NIL_H_

typedef struct nil_thread thread_t;

#include "nilconf.h"
#include "niltypes.h"
#include "nilcore.h"

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    Nil RTOS identification
 * @{
 */
#define _NIL_                           /**< @brief Nil RTOS identification.*/

#define CH_KERNEL_VERSION      "1.0.0dev"

#define CH_KERNEL_MAJOR        1
#define CH_KERNEL_MINOR        0
#define CH_KERNEL_PATCH        0
/** @} */

/**
 * @name    Wakeup messages
 * @{
 */
#define MSG_OK                  0   /**< @brief Normal wakeup message.      */
#define MSG_TIMEOUT             -1  /**< @brief Wake-up caused by a timeout
                                         condition.                         */
#define MSG_RESET               -2  /**< @brief Wake-up caused by a reset
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
#define TIME_IMMEDIATE          ((systime_t)-1)

/**
 * @brief   Infinite time specification for all functions with a timeout
 *          specification.
 */
#define TIME_INFINITE           ((systime_t)0)
/** @} */

/**
 * @name    Thread state related macros
 * @{
 */
#define NIL_STATE_READY         0   /**< @brief Thread ready or executing.  */
#define NIL_STATE_SLEEPING      1   /**< @brief Thread sleeping.            */
#define NIL_STATE_SUSP          2   /**< @brief Thread suspended.           */
#define NIL_STATE_WTSEM         3   /**< @brief Thread waiting on semaphore.*/
#define NIL_STATE_WTOREVT       4   /**< @brief Thread waiting for events.  */
#define NIL_THD_IS_READY(tr)    ((tr)->state == NIL_STATE_READY)
#define NIL_THD_IS_SLEEPING(tr) ((tr)->state == NIL_STATE_SLEEPING)
#define NIL_THD_IS_SUSP(tr)     ((tr)->state == NIL_STATE_SUSP)
#define NIL_THD_IS_WTSEM(tr)    ((tr)->state == NIL_STATE_WTSEM)
#define NIL_THD_IS_WTOREVT(tr)  ((tr)->state == NIL_STATE_WTOREVT)
/** @} */

/**
 * @name    Events related macros
 * @{
 */
/**
 * @brief   All events allowed mask.
 */
#define ALL_EVENTS              ((eventmask_t)-1)

/**
 * @brief   Returns an event mask from an event identifier.
 */
#define EVENT_MASK(eid)         ((eventmask_t)(1 << (eid)))
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   Number of user threads in the application.
 * @note    This number is not inclusive of the idle thread which is
 *          implicitly handled.
 */
#if !defined(NIL_CFG_NUM_THREADS) || defined(__DOXYGEN__)
#define NIL_CFG_NUM_THREADS                 2
#endif

/**
 * @brief   System time counter resolution.
 * @note    Allowed values are 16 or 32 bits.
 */
#if !defined(NIL_CFG_ST_RESOLUTION) || defined(__DOXYGEN__)
#define NIL_CFG_ST_RESOLUTION               32
#endif

/**
 * @brief   System tick frequency.
 * @note    This value together with the @p NIL_CFG_ST_RESOLUTION
 *          option defines the maximum amount of time allowed for
 *          timeouts.
 */
#if !defined(NIL_CFG_ST_FREQUENCY) || defined(__DOXYGEN__)
#define NIL_CFG_ST_FREQUENCY                100
#endif

/**
 * @brief   Time delta constant for the tick-less mode.
 * @note    If this value is zero then the system uses the classic
 *          periodic tick. This value represents the minimum number
 *          of ticks that is safe to specify in a timeout directive.
 *          The value one is not valid, timeouts are rounded up to
 *          this value.
 */
#if !defined(NIL_CFG_ST_TIMEDELTA) || defined(__DOXYGEN__)
#define NIL_CFG_ST_TIMEDELTA                0
#endif

/**
 * @brief   Events Flags APIs.
 * @details If enabled then the event flags APIs are included in the kernel.
 *
 * @note    The default is @p TRUE.
 */
#if !defined(NIL_CFG_USE_EVENTS) || defined(__DOXYGEN__)
#define NIL_CFG_USE_EVENTS                  TRUE
#endif

/**
 * @brief   System assertions.
 */
#if !defined(NIL_CFG_ENABLE_ASSERTS) || defined(__DOXYGEN__)
#define NIL_CFG_ENABLE_ASSERTS              FALSE
#endif

/**
 * @brief   Stack check.
 */
#if !defined(NIL_CFG_ENABLE_STACK_CHECK) || defined(__DOXYGEN__)
#define NIL_CFG_ENABLE_STACK_CHECK          FALSE
#endif

/**
 * @brief   System initialization hook.
 */
#if !defined(NIL_CFG_SYSTEM_INIT_HOOK) || defined(__DOXYGEN__)
#define NIL_CFG_SYSTEM_INIT_HOOK() {}
#endif

/**
 * @brief   Threads descriptor structure extension.
 * @details User fields added to the end of the @p thread_t structure.
 */
#if !defined(NIL_CFG_THREAD_EXT_FIELDS) || defined(__DOXYGEN__)
#define NIL_CFG_THREAD_EXT_FIELDS
#endif

/**
 * @brief   Threads initialization hook.
 */
#if !defined(NIL_CFG_THREAD_EXT_INIT_HOOK) || defined(__DOXYGEN__)
#define NIL_CFG_THREAD_EXT_INIT_HOOK(tr) {}
#endif

/**
 * @brief   Idle thread enter hook.
 * @note    This hook is invoked within a critical zone, no OS functions
 *          should be invoked from here.
 * @note    This macro can be used to activate a power saving mode.
 */
#if !defined(NIL_CFG_IDLE_ENTER_HOOK) || defined(__DOXYGEN__)
#define NIL_CFG_IDLE_ENTER_HOOK() {}
#endif

/**
 * @brief   Idle thread leave hook.
 * @note    This hook is invoked within a critical zone, no OS functions
 *          should be invoked from here.
 * @note    This macro can be used to deactivate a power saving mode.
 */
#if !defined(NIL_CFG_IDLE_LEAVE_HOOK) || defined(__DOXYGEN__)
#define NIL_CFG_IDLE_LEAVE_HOOK() {}
#endif

/**
 * @brief   System halt hook.
 */
#if !defined(NIL_CFG_SYSTEM_HALT_HOOK) || defined(__DOXYGEN__)
#define NIL_CFG_SYSTEM_HALT_HOOK(reason) {}
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if NIL_CFG_NUM_THREADS < 1
#error "at least one thread must be defined"
#endif

#if NIL_CFG_NUM_THREADS > 12
#error "Nil is not recommended for thread-intensive applications, consider" \
       "ChibiOS/RT instead"
#endif

#if (NIL_CFG_ST_RESOLUTION != 16) && (NIL_CFG_ST_RESOLUTION != 32)
#error "invalid NIL_CFG_ST_RESOLUTION specified, must be 16 or 32"
#endif

#if NIL_CFG_ST_FREQUENCY <= 0
#error "invalid NIL_CFG_ST_FREQUENCY specified, must be greated than zero"
#endif

#if (NIL_CFG_ST_TIMEDELTA < 0) || (NIL_CFG_ST_TIMEDELTA == 1)
#error "invalid NIL_CFG_ST_TIMEDELTA specified, must "                      \
       "be zero or greater than one"
#endif

#if NIL_CFG_ENABLE_ASSERTS || NIL_CFG_ENABLE_STACK_CHECK
#define NIL_DBG_ENABLED                 TRUE
#else
#define NIL_DBG_ENABLED                 FALSE
#endif

/** Boundaries of the idle thread boundaries, only required if stack checking
    is enabled.*/
#if NIL_CFG_ENABLE_STACK_CHECK || defined(__DOXYGEN__)
extern stkalign_t __main_thread_stack_base__, __main_thread_stack_end__;

#define THD_IDLE_BASE                   (&__main_thread_stack_base__)
#define THD_IDLE_END                    (&__main_thread_stack_end__)
#else
#define THD_IDLE_BASE                   NULL
#define THD_IDLE_END                    NULL
#endif

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of internal context structure.
 */
typedef struct port_intctx intctx_t;

/**
 * @brief   Type of a structure representing a semaphore.
 */
typedef struct nil_semaphore semaphore_t;

/**
 * @brief   Structure representing a counting semaphore.
 */
struct nil_semaphore {
  volatile cnt_t    cnt;        /**< @brief Semaphore counter.              */
};

/**
 * @brief Thread function.
 */
typedef void (*tfunc_t)(void *);

/**
 * @brief   Type of a structure representing a thread static configuration.
 */
typedef struct nil_thread_cfg thread_config_t;

/**
 * @brief   Structure representing a thread static configuration.
 */
struct nil_thread_cfg {
  stkalign_t        *wbase;     /**< @brief Thread working area base.       */
  stkalign_t        *wend;      /**< @brief Thread working area end.        */
  const char        *namep;     /**< @brief Thread name, for debugging.     */
  tfunc_t           funcp;      /**< @brief Thread function.                */
  void              *arg;       /**< @brief Thread function argument.       */
};

/**
 * @brief   Type of a structure representing a thread.
 */
typedef struct nil_thread thread_t;

/**
 * @brief   Type of a thread reference.
 */
typedef thread_t * thread_reference_t;

/**
 * @brief   Structure representing a thread.
 */
struct nil_thread {
  intctx_t              *ctxp;  /**< @brief Pointer to internal context.    */
  tstate_t              state;  /**< @brief Thread state.                   */
  /* Note, the following union contains a pointer while the thread is in a
     sleeping state (!NIL_THD_IS_READY()) else contains the wake-up message.*/
  union {
    msg_t               msg;    /**< @brief Wake-up message.                */
    void                *p;     /**< @brief Generic pointer.                */
    thread_reference_t  *trp;   /**< @brief Pointer to thread reference.    */
    semaphore_t         *semp;  /**< @brief Pointer to semaphore.           */
#if NIL_CFG_USE_EVENTS || defined(__DOXYGEN__)
    eventmask_t         ewmask; /**< @brief Enabled events mask.            */
#endif
  } u1;
  volatile systime_t    timeout;/**< @brief Timeout counter, zero
                                            if disabled.                    */
#if NIL_CFG_USE_EVENTS || defined(__DOXYGEN__)
  eventmask_t           epmask; /**< @brief Pending events mask.            */
#endif
#if NIL_CFG_ENABLE_STACK_CHECK || defined(__DOXYGEN__)
  stkalign_t            *stklim;/**< @brief Thread stack boundary.          */
#endif
  /* Optional extra fields.*/
  NIL_CFG_THREAD_EXT_FIELDS
};

/**
 * @brief   Type of a structure representing the system.
 */
typedef struct nil_system nil_system_t;

/**
 * @brief   System data structure.
 * @note    This structure contain all the data areas used by the OS except
 *          stacks.
 */
struct nil_system {
  /**
   * @brief   Pointer to the running thread.
   */
  thread_t              *current;
  /**
   * @brief   Pointer to the next thread to be executed.
   * @note    This pointer must point at the same thread pointed by @p current
   *          or to an higher priority thread if a switch is required.
   */
  thread_t              *next;
#if NIL_CFG_ST_TIMEDELTA == 0 || defined(__DOXYGEN__)
  /**
   * @brief   System time.
   */
  systime_t             systime;
#endif
#if NIL_CFG_ST_TIMEDELTA > 0 || defined(__DOXYGEN__)
  /**
   * @brief   System time of the last tick event.
   */
  systime_t             lasttime;
  /**
   * @brief   Time of the next scheduled tick event.
   */
  systime_t             nexttime;
#endif
  /**
   * @brief   Thread structures for all the defined threads.
   */
  thread_t              threads[NIL_CFG_NUM_THREADS + 1];
#if NIL_DBG_ENABLED || defined(__DOXYGEN__)
  /**
   * @brief   Panic message.
   * @note    This field is only present if some debug options have been
   *          activated.
   * @note    Accesses to this pointer must never be optimized out so the
   *          field itself is declared volatile.
   */
  const char            * volatile dbg_panic_msg;
#endif
};

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
#define THD_TABLE_BEGIN                                                     \
  const thread_config_t nil_thd_configs[NIL_CFG_NUM_THREADS + 1] = {

/**
 * @brief   Entry of user threads table
 */
#define THD_TABLE_ENTRY(wap, name, funcp, arg)                              \
  {wap, ((stkalign_t *)(wap)) + (sizeof (wap) / sizeof(stkalign_t)),        \
   name, funcp, arg},

/**
 * @brief   End of user threads table.
 */
#define THD_TABLE_END                                                       \
  {THD_IDLE_BASE, THD_IDLE_END, "idle", 0, NULL}                            \
};
/** @} */

/**
 * @name    Working Areas and Alignment
 */
/**
 * @brief   Enforces a correct alignment for a stack area size value.
 *
 * @param[in] n         the stack size to be aligned to the next stack
 *                      alignment boundary
 * @return              The aligned stack size.
 *
 * @api
 */
#define THD_ALIGN_STACK_SIZE(n)                                             \
  ((((n) - 1) | (sizeof(stkalign_t) - 1)) + 1)

/**
 * @brief   Calculates the total Working Area size.
 *
 * @param[in] n         the stack size to be assigned to the thread
 * @return              The total used memory in bytes.
 *
 * @api
 */
#define THD_WORKING_AREA_SIZE(n)                                            \
  THD_ALIGN_STACK_SIZE(PORT_WA_SIZE(n))

/**
 * @brief   Static working area allocation.
 * @details This macro is used to allocate a static thread working area
 *          aligned as both position and size.
 *
 * @param[in] s         the name to be assigned to the stack array
 * @param[in] n         the stack size to be assigned to the thread
 *
 * @api
 */
#define THD_WORKING_AREA(s, n)                                              \
  stkalign_t s[THD_WORKING_AREA_SIZE(n) / sizeof(stkalign_t)]
/** @} */

/**
 * @name    Threads abstraction macros
 */
/**
 * @brief   Thread declaration macro.
 * @note    Thread declarations should be performed using this macro because
 *          the port layer could define optimizations for thread functions.
 */
#define THD_FUNCTION(tname, arg) PORT_THD_FUNCTION(tname, arg)
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
#define CH_IRQ_PROLOGUE() PORT_IRQ_PROLOGUE()

/**
 * @brief   IRQ handler exit code.
 * @note    Usually IRQ handlers function are also declared naked.
 *
 * @special
 */
#define CH_IRQ_EPILOGUE() PORT_IRQ_EPILOGUE()

/**
 * @brief   Standard normal IRQ handler declaration.
 * @note    @p id can be a function name or a vector number depending on the
 *          port implementation.
 *
 * @special
 */
#define CH_IRQ_HANDLER(id) PORT_IRQ_HANDLER(id)
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
#define CH_FAST_IRQ_HANDLER(id) PORT_FAST_IRQ_HANDLER(id)
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
#define S2ST(sec)                                                           \
  ((systime_t)((sec) * NIL_CFG_ST_FREQUENCY))

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
#define MS2ST(msec)                                                         \
  ((systime_t)(((((uint32_t)(msec)) *                                       \
                 ((uint32_t)NIL_CFG_ST_FREQUENCY) - 1UL) / 1000UL) + 1UL))

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
#define US2ST(usec)                                                         \
  ((systime_t)(((((uint32_t)(usec)) *                                       \
                 ((uint32_t)NIL_CFG_ST_FREQUENCY) - 1UL) / 1000000UL) + 1UL))
/** @} */

/**
 * @name    Macro Functions
 * @{
 */
/**
 * @brief   Enters the kernel lock mode.
 *
 * @special
 */
#define chSysDisable() port_disable()

/**
 * @brief   Enters the kernel lock mode.
 *
 * @special
 */
#define chSysEnable() port_enable()

/**
 * @brief   Enters the kernel lock state.
 *
 * @special
 */
#define chSysLock() port_lock()

/**
 * @brief   Leaves the kernel lock state.
 *
 * @special
 */
#define chSysUnlock() port_unlock()

/**
 * @brief   Enters the kernel lock state from within an interrupt handler.
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
#define chSysLockFromISR() port_lock_from_isr()

/**
 * @brief   Leaves the kernel lock state from within an interrupt handler.
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
#define chSysUnlockFromISR() port_unlock_from_isr()

/**
 * @brief   Evaluates if a reschedule is required.
 *
 * @retval true         if there is a thread that must go in running state
 *                      immediately.
 * @retval false        if preemption is not required.
 *
 * @iclass
 */
#define chSchIsRescRequiredI() ((bool)(nil.current != nil.next))

/**
 * @brief   Returns a pointer to the current @p thread_t.
 *
 * @xclass
 */
#define chThdGetSelfX() nil.current

/**
 * @brief   Delays the invoking thread for the specified number of seconds.
 * @note    The specified time is rounded up to a value allowed by the real
 *          system clock.
 * @note    The maximum specified value is implementation dependent.
 *
 * @param[in] sec       time in seconds, must be different from zero
 *
 * @api
 */
#define chThdSleepSeconds(sec) chThdSleep(S2ST(sec))

/**
 * @brief   Delays the invoking thread for the specified number of
 *          milliseconds.
 * @note    The specified time is rounded up to a value allowed by the real
 *          system clock.
 * @note    The maximum specified value is implementation dependent.
 *
 * @param[in] msec      time in milliseconds, must be different from zero
 *
 * @api
 */
#define chThdSleepMilliseconds(msec) chThdSleep(MS2ST(msec))

/**
 * @brief   Delays the invoking thread for the specified number of
 *          microseconds.
 * @note    The specified time is rounded up to a value allowed by the real
 *          system clock.
 * @note    The maximum specified value is implementation dependent.
 *
 * @param[in] usec      time in microseconds, must be different from zero
 *
 * @api
 */
#define chThdSleepMicroseconds(usec) chThdSleep(US2ST(usec))

/**
 * @brief   Suspends the invoking thread for the specified time.
 *
 * @param[in] timeout   the delay in system ticks
 *
 * @sclass
 */
#define chThdSleepS(timeout) chSchGoSleepTimeoutS(NIL_STATE_SLEEPING, timeout)

/**
 * @brief   Suspends the invoking thread until the system time arrives to the
 *          specified value.
 *
 * @param[in] time      absolute system time
 *
 * @sclass
 */
#define chThdSleepUntilS(time)                                             \
  chSchGoSleepTimeoutS(NIL_STATE_SLEEPING, (time) - chVTGetSystemTimeX())

/**
 * @brief   Initializes a semaphore with the specified counter value.
 *
 * @param[out] sp       pointer to a @p semaphore_t structure
 * @param[in] n         initial value of the semaphore counter. Must be
 *                      non-negative.
 *
 * @init
 */
#define chSemObjectInit(sp, n) ((sp)->cnt = n)

/**
 * @brief   Performs a wait operation on a semaphore.
 *
 * @param[in] sp        pointer to a @p semaphore_t structure
 * @return              A message specifying how the invoking thread has been
 *                      released from the semaphore.
 * @retval CH_MSG_OK   if the thread has not stopped on the semaphore or the
 *                      semaphore has been signaled.
 * @retval CH_MSG_RST  if the semaphore has been reset using @p chSemReset().
 *
 * @api
 */
#define chSemWait(sp) chSemWaitTimeout(sp, TIME_INFINITE)

/**
 * @brief   Performs a wait operation on a semaphore.
 *
 * @param[in] sp        pointer to a @p semaphore_t structure
 * @return              A message specifying how the invoking thread has been
 *                      released from the semaphore.
 * @retval CH_MSG_OK   if the thread has not stopped on the semaphore or the
 *                      semaphore has been signaled.
 * @retval CH_MSG_RST  if the semaphore has been reset using @p chSemReset().
 *
 * @sclass
 */
#define chSemWaitS(sp) chSemWaitTimeoutS(sp, TIME_INFINITE)

/**
 * @brief   Returns the semaphore counter current value.
 *
 * @iclass
 */
#define chSemGetCounterI(sp) ((sp)->cnt)

/**
 * @brief   Current system time.
 * @details Returns the number of system ticks since the @p chSysInit()
 *          invocation.
 * @note    The counter can reach its maximum and then restart from zero.
 * @note    This function can be called from any context but its atomicity
 *          is not guaranteed on architectures whose word size is less than
 *          @p systime_t size.
 *
 * @return              The system time in ticks.
 *
 * @xclass
 */
#if NIL_CFG_ST_TIMEDELTA == 0 || defined(__DOXYGEN__)
#define chVTGetSystemTimeX() (nil.systime)
#else
#define chVTGetSystemTimeX() port_timer_get_time()
#endif

/**
 * @brief   Returns the elapsed time since the specified start time.
 *
 * @param[in] start     start time
 * @return              The elapsed time.
 *
 * @xclass
 */
#define chVTTimeElapsedSinceX(start)                                        \
  ((systime_t)(chVTGetSystemTimeX() - (start)))

/**
 * @brief   Checks if the specified time is within the specified time window.
 * @note    When start==end then the function returns always true because the
 *          whole time range is specified.
 * @note    This function can be called from any context.
 *
 * @param[in] time      the time to be verified
 * @param[in] start     the start of the time window (inclusive)
 * @param[in] end       the end of the time window (non inclusive)
 * @retval true         current time within the specified time window.
 * @retval false        current time not within the specified time window.
 *
 * @xclass
 */
#define chVTIsTimeWithinX(time, start, end)                                 \
  ((bool)((systime_t)((time) - (start)) < (systime_t)((end) - (start))))

/**
 * @brief   Condition assertion.
 * @details If the condition check fails then the kernel panics with a
 *          message and halts.
 * @note    The condition is tested only if the @p NIL_CFG_ENABLE_ASSERTS
 *          switch is specified in @p nilconf.h else the macro does nothing.
 * @note    The remark string is not currently used except for putting a
 *          comment in the code about the assertion.
 *
 * @param[in] c         the condition to be verified to be true
 * @param[in] r         a remark string
 *
 * @api
 */
#if !defined(chDbgAssert)
#define chDbgAssert(c, r) do {                                              \
  if (NIL_CFG_ENABLE_ASSERTS && !(c))                                       \
    chSysHalt(__func__);                                                    \
} while (0)
#endif /* !defined(chDbgAssert) */
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(__DOXYGEN__)
extern nil_system_t nil;
extern const thread_config_t nil_thd_configs[NIL_CFG_NUM_THREADS + 1];
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void chSysInit(void);
  void chSysHalt(const char *reason);
  void chSysTimerHandlerI(void);
  void chSysUnconditionalLock(void);
  void chSysUnconditionalUnlock(void);
  syssts_t chSysGetStatusAndLockX(void);
  void chSysRestoreStatusX(syssts_t sts);
  thread_t *chSchReadyI(thread_t *tp, msg_t msg);
  void chSchRescheduleS(void);
  msg_t chSchGoSleepTimeoutS(tstate_t newstate, systime_t timeout);
  msg_t chThdSuspendTimeoutS(thread_reference_t *trp, systime_t timeout);
  void chThdResumeI(thread_reference_t *trp, msg_t msg);
  void chThdSleep(systime_t time);
  void chThdSleepUntil(systime_t time);
  msg_t chSemWaitTimeout(semaphore_t *sp, systime_t time);
  msg_t chSemWaitTimeoutS(semaphore_t *sp, systime_t time);
  void chSemSignal(semaphore_t *sp);
  void chSemSignalI(semaphore_t *sp);
  void chSemReset(semaphore_t *sp, cnt_t n);
  void chSemResetI(semaphore_t *sp, cnt_t n);
  void chEvtSignal(thread_t *tp, eventmask_t mask);
  void chEvtSignalI(thread_t *tp, eventmask_t mask);
  eventmask_t chEvtWaitAnyTimeout(eventmask_t mask, systime_t timeout);
  eventmask_t chEvtWaitAnyTimeoutS(eventmask_t mask, systime_t timeout);
#ifdef __cplusplus
}
#endif

#endif /* _NIL_H_ */

/** @} */
