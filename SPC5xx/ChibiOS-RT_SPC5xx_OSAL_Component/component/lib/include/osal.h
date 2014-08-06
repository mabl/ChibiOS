/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013 Giovanni Di Sirio.

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    osal.h
 * @brief   OSAL module header.
 *
 * @addtogroup OSAL
 * @{
 */

#ifndef _OSAL_H_
#define _OSAL_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "ch.h"
#include "platform.h"
#include "osal_cfg.h"

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    Common constants
 * @{
 */
#if !defined(FALSE) || defined(__DOXYGEN__)
#define FALSE                               false
#endif

#if !defined(TRUE) || defined(__DOXYGEN__)
#define TRUE                                true
#endif

#define OSAL_SUCCESS                        FALSE
#define OSAL_FAILED                         TRUE
/** @} */

/**
 * @name    Messages
 * @{
 */
#define MSG_OK                              RDY_OK
#define MSG_RESET                           RDY_RESET
#define MSG_TIMEOUT                         RDY_TIMEOUT
/** @} */

/**
 * @name    Time related
 */
#define OSAL_ST_FREQUENCY                   CH_FREQUENCY
/** @} */

/**
 * @name    Systick modes.
 * @{
 */
#define OSAL_ST_MODE_NONE                   0
#define OSAL_ST_MODE_PERIODIC               1
#define OSAL_ST_MODE_FREERUNNING            2
/** @} */

/**
 * @name    INTC registers direct access macros
 * @{
 */
#define INTC_BCR            (*((volatile uint32_t *)0xFFF48000))
#define INTC_CPR            (*((volatile uint32_t *)0xFFF48008))
#define INTC_IACKR          (*((volatile uint32_t *)0xFFF48010))
#define INTC_PSR(n)         (*((volatile uint8_t *)(0xFFF48040 + (n))))
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a Virtual Timer structure.
 * @note 	Mapped on ChibiOS/RT's own virtual timers.
 */
typedef VirtualTimer virtual_timer_t;

/**
 * @brief   Type of a thread.
 * @note 	Mapped on ChibiOS/RT's own threads.
 */
typedef Thread thread_t;

/**
 * @brief   Type of a thread reference.
 */
typedef thread_t * thread_reference_t;

/**
 * @brief   Type of an event flags mask.
 * @note 	Mapped on ChibiOS/RT's own flags.
 */
typedef flagsmask_t eventflags_t;

/**
 * @brief   Type of an event flags object.
 * @note 	Mapped on ChibiOS/RT's own event source object.
 */
typedef EventSource event_source_t;

/**
 * @brief   Type of a mutex.
 * @note 	Mapped on ChibiOS/RT's own event mutexes or semaphores.
 */
#if CH_USE_MUTEXES || defined(__DOXYGEN__)
typedef Mutex mutex_t;
#elif CH_USE_SEMAPHORES
typedef Semaphore mutex_t;
#else
typedef uint32_t mutex_t;
#endif

/**
 * @brief   Type of a thread queue.
 * @detail  A thread queue is a queue of sleeping threads, queued threads
 *          can be dequeued one at time or all together.
 */
typedef ThreadsQueue threads_queue_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Condition assertion.
 * @note 	Mapped on ChibiOS/RT's own assertions.
 *
 * @param[in] c         the condition to be verified to be true
 * @param[in] r         a remark string
 *
 * @api
 */
#define osalDbgAssert(c, r) chDbgAssert(c, __func__, remark)

/**
 * @brief   Function parameters check.
 * @note 	Mapped on ChibiOS/RT's own checks.
 *
 * @param[in] c         the condition to be verified to be true
 *
 * @api
 */
#define osalDbgCheck(c) chDbgCheck(c, __func__)

/**
 * @brief   I-Class state check.
 * @note 	Mapped on ChibiOS/RT's own state checker.
 */
#define osalDbgCheckClassI() chDbgCheckClassI()

/**
 * @brief   S-Class state check.
 * @note 	Mapped on ChibiOS/RT's own state checker.
 */
#define osalDbgCheckClassS() chDbgCheckClassS()

/**
 * @brief   IRQ prologue code.
 * @note 	Mapped on ChibiOS/RT's own ISR macros.
 */
#define OSAL_IRQ_PROLOGUE() CH_IRQ_PROLOGUE()

/**
 * @brief   IRQ epilogue code.
 * @note 	Mapped on ChibiOS/RT's own ISR macros.
 */
#define OSAL_IRQ_EPILOGUE() CH_IRQ_EPILOGUE()

/**
 * @brief   IRQ handler function declaration.
 * @note 	Mapped on ChibiOS/RT's own ISR macros.
 *
 * @param[in] id        a vector name as defined in @p vectors.s
 */
#define OSAL_IRQ_HANDLER(id) CH_IRQ_HANDLER(id)

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
#define OSAL_S2ST(sec) S2ST(sec)

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
#define OSAL_MS2ST(msec) MS2ST(msec)

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
#define OSAL_US2ST(usec) US2ST(usec)

/**
 * @brief   Delays the invoking thread for the specified number of seconds.
 * @note    The specified time is rounded up to a value allowed by the real
 *          system tick clock.
 * @note    The maximum specifiable value is implementation dependent.
 *
 * @param[in] sec       time in seconds, must be different from zero
 *
 * @api
 */
#define osalThreadSleepSeconds(sec) chThdSleepSeconds(sec)

/**
 * @brief   Delays the invoking thread for the specified number of
 *          milliseconds.
 * @note    The specified time is rounded up to a value allowed by the real
 *          system tick clock.
 * @note    The maximum specifiable value is implementation dependent.
 *
 * @param[in] msec      time in milliseconds, must be different from zero
 *
 * @api
 */
#define osalThreadSleepMilliseconds(msec) chThdSleepMilliseconds(msec)

/**
 * @brief   Delays the invoking thread for the specified number of
 *          microseconds.
 * @note    The specified time is rounded up to a value allowed by the real
 *          system tick clock.
 * @note    The maximum specifiable value is implementation dependent.
 *
 * @param[in] usec      time in microseconds, must be different from zero
 *
 * @api
 */
#define osalThreadSleepMicroseconds(usec) chThdSleepMicroseconds(usec)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void osalInit(void);
  msg_t osalThreadSuspendS(thread_reference_t *trp);
  void osalThreadResumeI(thread_reference_t *trp, msg_t msg);
  void osalThreadResumeS(thread_reference_t *trp, msg_t msg);
  msg_t osalThreadEnqueueTimeoutS(threads_queue_t *tqp, systime_t time);
  void osalThreadDequeueNextI(threads_queue_t *tqp, msg_t msg);
  void osalThreadDequeueAllI(threads_queue_t *tqp, msg_t msg);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @brief   Globally enables interrupts.
 * @note    Mapped on ChibiOS/RT's own ISR primitives.
 *
 * @special
 */
static inline
void osalIsrEnable(void) {

    chSysEnable();
}

/**
 * @brief   Globally disables interrupts.
 * @note    Mapped on ChibiOS/RT's own ISR primitives.
 *
 * @special
 */
static inline
void osalIsrDisable(void) {

    chSysDisable();
}

/**
 * @brief   Waits for an interrupt to occur.
 * @note    Mapped on ChibiOS/RT's own ISR primitives.
 *
 * @special
 */
static inline
void osalIsrWait(void) {

  asm volatile ("wait" : : : "memory");
}

/**
 * @brief   Enables an interrupt vector.
 *
 * @param[in] vector    the vector number
 * @param[in] priority  the priority level, zero disables the vector
 *
 * @special
 */
static inline
void osalIsrEnableVector(unsigned vector, unsigned priority) {

  osalDbgCheck((vector < VECTORS_NUMBER) &&
               (priority >= 1) && (priority <= 15));

  INTC_PSR(vector) = (uint8_t)priority;
}

/**
 * @brief   Disables an interrupt vector.
 *
 * @param[in] vector    the vector number
 *
 * @special
 */
static inline
void osalIsrDisableVector(unsigned vector) {

  osalDbgCheck(vector < VECTORS_NUMBER);

  INTC_PSR(vector) = (uint8_t)0;
}

/**
 * @brief   System halt with error message.
 *
 * @param[in] reason    the halt message pointer
 *
 * @api
 */
static inline
void osalSysHalt(const char *reason) {

  (void)reason;

  chDbgAssert(0, reason, "");
  chSysHalt();
}

/**
 * @brief   System timer handler.
 * @details The handler is used for scheduling and Virtual Timers management.
 *
 * @iclass
 */
static inline
void osalOsTimerHandlerI(void) {

  chSysTimerHandlerI();
}

/**
 * @brief   Enters a critical zone from thread context.
 * @note    This function cannot be used for reentrant critical zones.
 * @note    Mapped on ChibiOS/RT's own lock primitives.
 *
 * @special
 */
static inline
void osalSysLock(void) {

  chSysLock();
}

/**
 * @brief   Leaves a critical zone from thread context.
 * @note    This function cannot be used for reentrant critical zones.
 * @note    Mapped on ChibiOS/RT's own lock primitives.
 *
 * @special
 */
static inline
void osalSysUnlock(void) {

  chSysUnlock();
}

/**
 * @brief   Enters a critical zone from ISR context.
 * @note    This function cannot be used for reentrant critical zones.
 * @note    Mapped on ChibiOS/RT's own lock primitives.
 *
 * @special
 */
static inline
void osalSysLockFromISR(void) {

  chSysLockFromIsr();
}

/**
 * @brief   Leaves a critical zone from ISR context.
 * @note    This function cannot be used for reentrant critical zones.
 * @note    Mapped on ChibiOS/RT's own lock primitives.
 *
 * @special
 */
static inline
void osalSysUnlockFromISR(void) {

  chSysUnlockFromIsr();
}

/**
 * @brief   Returns @p TRUE if the specified timer is armed.
 * @note    Mapped on ChibiOS/RT's own virtual timers.
 *
 * @param[out] vtp      the @p virtual_timer_t structure pointer
 *
 * @iclass
 */
static inline
bool osalVTIsArmedI(virtual_timer_t *vtp) {

  return (bool)chVTIsArmedI(vtp);
}

/**
 * @brief   Enables a virtual timer.
 * @note    The associated function is invoked from interrupt context.
 * @note    Mapped on ChibiOS/RT's own virtual timers.
 *
 * @param[out] vtp      the @p virtual_timer_t structure pointer
 * @param[in] time      the number of ticks before the operation timeouts, the
 *                      special values are handled as follow:
 *                      - @a TIME_INFINITE is allowed but interpreted as a
 *                        normal time specification.
 *                      - @a TIME_IMMEDIATE this value is not allowed.
 *                      .
 * @param[in] vtfunc    the timer callback function. After invoking the
 *                      callback the timer is disabled and the structure can
 *                      be disposed or reused.
 * @param[in] par       a parameter that will be passed to the callback
 *                      function
 *
 * @iclass
 */
static inline
void osalVTSetI(virtual_timer_t *vtp, systime_t time,
                vtfunc_t vtfunc, void *par) {

  chVTSetI((VirtualTimer *)vtp, time, vtfunc, par);
}

/**
 * @brief   Enables a virtual timer.
 * @note    The associated function is invoked from interrupt context.
 * @note    Mapped on ChibiOS/RT's own virtual timers.
 *
 * @param[out] vtp      the @p virtual_timer_t structure pointer
 * @param[in] time      the number of ticks before the operation timeouts, the
 *                      special values are handled as follow:
 *                      - @a TIME_INFINITE is allowed but interpreted as a
 *                        normal time specification.
 *                      - @a TIME_IMMEDIATE this value is not allowed.
 *                      .
 * @param[in] vtfunc    the timer callback function. After invoking the
 *                      callback the timer is disabled and the structure can
 *                      be disposed or reused.
 * @param[in] par       a parameter that will be passed to the callback
 *                      function
 *
 * @api
 */
static inline
void osalVTSet(virtual_timer_t *vtp, systime_t time,
               vtfunc_t vtfunc, void *par) {

  chVTSet((VirtualTimer *)vtp, time, vtfunc, par);
}

/**
 * @brief   Disables a Virtual Timer.
 * @note    The timer MUST be active when this function is invoked.
 * @note    Mapped on ChibiOS/RT's own virtual timers.
 *
 * @param[in] vtp       the @p virtual_timer_t structure pointer
 *
 * @iclass
 */
static inline
void osalVTResetI(virtual_timer_t *vtp) {

  chVTResetI((VirtualTimer *)vtp);
}

/**
 * @brief   Disables a Virtual Timer.
 * @note    The timer is first checked and disabled only if armed.
 * @note    Mapped on ChibiOS/RT's own virtual timers.
 *
 * @param[in] vtp       the @p virtual_timer_t structure pointer
 *
 * @api
 */
static inline
void osalVTReset(virtual_timer_t *vtp) {

  chVTReset((VirtualTimer *)vtp);
}

/**
 * @brief   Current system time.
 * @details Returns the number of system ticks since the @p osalInit()
 *          invocation.
 * @note    The counter can reach its maximum and then restart from zero.
 * @note    Mapped on ChibiOS/RT's own virtual timers.
 *
 * @return              The system time in ticks.
 *
 * @api
 */
static inline
systime_t osalVTGetSystemTime(void) {

  return chTimeNow();
}

/**
 * @brief   Returns the elapsed time since the specified start time.
 *
 * @param[in] start     start time
 * @return              The elapsed time.
 *
 * @api
 */
static inline
systime_t osalVTTimeElapsedSince(systime_t start) {

  return osalVTGetSystemTime() - start;
}

/**
 * @brief   Checks if the current system time is within the specified time
 *          window.
 * @note    When start==end then the function returns always true because the
 *          whole time range is specified.
 * @note    Mapped on ChibiOS/RT's own virtual timers.
 *
 * @param[in] start     the start of the time window (inclusive)
 * @param[in] end       the end of the time window (non inclusive)
 * @retval TRUE         current time within the specified time window.
 * @retval FALSE        current time not within the specified time window.
 *
 * @api
 */
static inline
bool osalVTIsSystemTimeWithin(systime_t start, systime_t end) {

  return (bool)(osalVTTimeElapsedSince(start) < end - start);
}

/**
 * @brief   Suspends the invoking thread for the specified time.
 *
 * @param[in] time      the delay in system ticks, the special values are
 *                      handled as follow:
 *                      - @a TIME_INFINITE the thread enters an infinite sleep
 *                        state.
 *                      - @a TIME_IMMEDIATE this value is not allowed.
 *                      .
 *
 * @api
 */
static inline
void osalThreadSleep(systime_t time) {

  chThdSleep(time);
}

/**
 * @brief   Suspends the invoking thread for the specified time.
 *
 * @param[in] time      the delay in system ticks, the special values are
 *                      handled as follow:
 *                      - @a TIME_INFINITE is allowed but interpreted as a
 *                        normal time specification.
 *                      - @a TIME_IMMEDIATE this value is not allowed.
 *                      .
 *
 * @sclass
 */
static inline
void osalThreadSleepS(systime_t time) {

  chThdSleepS(time);
}

/**
 * @brief   Checks if a reschedule is required and performs it.
 * @note    I-Class functions invoked from thread context must not reschedule
 *          by themselves, an explicit reschedule using this function is
 *          required in this scenario.
 *
 * @sclass
 */
static inline
void osalOsRescheduleS(void) {

  chSchRescheduleS();
}

/**
 * @brief   Initializes an event flags object.
 *
 * @param[in] esp       pointer to the event flags object
 *
 * @init
 */
static inline
void osalEventObjectInit(event_source_t *esp) {

  chEvtInit((EventSource *)esp);
}

/**
 * @brief   Add flags to an event source object.
 *
 * @param[in] esp       pointer to the event flags object
 * @param[in] flags     flags to be ORed to the flags mask
 *
 * @iclass
 */
static inline
void osalEventBroadcastFlagsI(event_source_t *esp,
                              eventflags_t flags) {

  chEvtBroadcastFlagsI((EventSource *)esp, (flagsmask_t)flags);
}

/**
 * @brief   Add flags to an event source object.
 *
 * @param[in] esp       pointer to the event flags object
 * @param[in] flags     flags to be ORed to the flags mask
 *
 * @api
 */
static inline
void osalEventBroadcastFlags(event_source_t *esp,
                             eventflags_t flags) {

  chEvtBroadcastFlags((EventSource *)esp, (flagsmask_t)flags);
}

/**
 * @brief   Initializes s @p mutex_t object.
 *
 * @param[out] mp       pointer to the @p mutex_t object
 *
 * @init
 */
static inline
void osalMutexInit(mutex_t *mp) {

#if CH_USE_MUTEXES
  chMtxInit((Mutex *)mp);
#elif CH_USE_SEMAPHORES
  chSemInit((Semaphore *)mp, 1);
#else
 *mp = 0;
#endif
}

/*
 * @brief   Locks the specified mutex.
 * @post    The mutex is locked and inserted in the per-thread stack of owned
 *          mutexes.
 *
 * @param[in,out] mp    pointer to the @p mutex_t object
 *
 * @api
 */
static inline
void osalMutexLock(mutex_t *mp) {

#if CH_USE_MUTEXES
  chMtxLock((Mutex *)mp);
#elif CH_USE_SEMAPHORES
  chSemWait((Semaphore *)mp);
#else
  *mp = 1;
#endif
}

/**
 * @brief   Unlocks the specified mutex.
 * @note    The HAL guarantees to release mutex in reverse lock order. The
 *          mutex being unlocked is guaranteed to be the last locked mutex
 *          by the invoking thread.
 *          The implementation can rely on this behavior and eventually
 *          ignore the @p mp parameter which is supplied in order to support
 *          those OSes not supporting a stack of the owned mutexes.
 *
 * @param[in,out] mp    pointer to the @p mutex_t object
 *
 * @api
 */
static inline
void osalMutexUnlock(mutex_t *mp) {

#if CH_USE_MUTEXES
  (void)mp;
  chMtxUnlock();
#elif CH_USE_SEMAPHORES
  chSemSignal((Semaphore *)mp);
#else
  *mp = 0;
#endif
}

/**
 * @brief   Initializes a threads queue object.
 *
 * @param[out] tqp      pointer to the threads queue object
 *
 * @init
 */
static inline
void osalThreadQueueObjectInit(threads_queue_t *tqp) {

  queue_init(tqp);
}

#endif /* _OSAL_H_ */

/** @} */
