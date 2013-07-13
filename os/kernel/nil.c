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
 * @file    nil.c
 * @brief   Nil RTOS main source file.
 *
 * @defgroup nil
 * @details Nil RTOS services.
 * @{
 */

#include "nil.h"

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   System variables.
 */
nil_system_t nil;

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Initializes the kernel.
 * @details Initializes the kernel structures, the current instructions flow
 *          becomes the idle thread upon return. The idle thread must not
 *          invoke any kernel primitive able to change state to not runnable.
 *
 * @special
 */
void nilSysInit(void) {
  thread_ref_t tr;
  const thread_config_t *tcp;

  /* Port layer initialization.*/
  port_init();

  /* Iterates through the list of defined threads.*/
  for (tr = &nil.threads[0], tcp = nil_thd_configs;
       tr < &nil.threads[NIL_CFG_NUM_THREADS];
       tr++, tcp++) {
    tr->state = NIL_THD_READY;

    /* Port dependent thread initialization.*/
    SETUP_CONTEXT(tr, tcp->wap, tcp->size, tcp->funcp, tcp->arg);

    /* Initialization hook.*/
    NIL_CFG_THREAD_EXT_INIT_HOOK(tr);
  }

  /* Runs the highest priority thread, the current one becomes the null
     thread.*/
  nil.current = nil.next = nil.threads;
  port_switch(nil.threads, &nil.threads[NIL_CFG_NUM_THREADS]);

  /* Interrupts enabled for the idle thread.*/
  nilSysEnable();
}

/**
 * @brief   Time management handler.
 * @note    This handler has to be invoked by a periodic ISR in order to
 *          reschedule the waiting threads.
 *
 * @iclass
 */
void nilSysTimerHandlerI(void) {
  thread_ref_t tr;

  nil.systime++;
  tr = &nil.threads[0];
  do {
    /* If a thread is not ready and its timeout counter is greater than
       zero then the timeout handling must be performed.*/
    if (!NIL_THD_IS_READY(tr) && (tr->timeout > 0)) {
      /* Did the timer reach zero?*/
      if (--tr->timeout == 0) {
        /* Timeout on semaphores requires a special handling because the
           semaphore counter must be incremented.*/
        if (NIL_THD_IS_WTSEM(tr))
          tr->u1.semp->cnt++;
        else if (NIL_THD_IS_SUSP(tr))
          tr->u1.trp = NULL;
        nilSchReadyI(tr, NIL_MSG_TMO);
      }
    }
    /* Lock released in order to give a preemption chance on those
       architectures supporting IRQ preemption.*/
    nilSysUnlockFromISR();
    tr++;
    nilSysLockFromISR();
  } while (tr < &nil.threads[NIL_CFG_NUM_THREADS]);
}

/**
 * @brief   Makes the specified thread ready for execution.
 *
 * @param[in] tr        reference to the @p thread_t object
 * @param[in] msg       the wakeup message
 *
 * @return              The same reference passed as parameter.
 */
thread_ref_t nilSchReadyI(thread_ref_t tr, msg_t msg) {

  nilDbgAssert((tr >= nil.threads) &&
               (tr < &nil.threads[NIL_CFG_NUM_THREADS]),
               "nilSchReadyI(), #1", "pointer out of range");
  nilDbgAssert(!NIL_THD_IS_READY(tr),
               "nilSchReadyI(), #2", "already ready");
  nilDbgAssert(nil.next <= nil.current,
               "nilSchReadyI(), #3", "priority ordering");

  tr->u1.msg = msg;
  tr->state = NIL_THD_READY;
  tr->timeout = 0;
  if (tr < nil.next)
    nil.next = tr;
  return tr;
}

/**
 * @brief   Reschedules.
 *
 * @sclass
 */
void nilSchRescheduleS() {
  thread_ref_t otr = nil.current;
  thread_ref_t ntr = nil.next;

  if (ntr != otr) {
    nil.current = ntr;
    port_switch(ntr, otr);
  }
}

/**
 * @brief   Puts the current thread to sleep into the specified state with
 *          timeout specification.
 * @details The thread goes into a sleeping state, if it is not awakened
 *          explicitly within the specified system time then it is forcibly
 *          awakened with a @p NIL_MSG_TMO low level message.
 *
 * @param[in] newstate  the new thread state or a semaphore pointer
 * @param[in] timeout   the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return              The wakeup message.
 * @retval NIL_MSG_TMO  if a timeout occurred.
 *
 * @sclass
 */
msg_t nilSchGoSleepTimeoutS(tstate_t newstate, systime_t timeout) {
  thread_ref_t ntr, otr = nil.current;

  /* Storing the wait object for the current thread.*/
  otr->state = newstate;

  /* Timeout settings.*/
  otr->timeout = timeout;

  /* Scanning the whole threads array.*/
  ntr = nil.threads;
  while (true) {
    /* Is this thread ready to execute?*/
    if (NIL_THD_IS_READY(ntr)) {
      nil.current = nil.next = ntr;
      port_switch(ntr, otr);
      return nil.current->u1.msg;
    }

    /* Points to the next thread in lowering priority order.*/
    ntr++;
    nilDbgAssert(ntr <= &nil.threads[NIL_CFG_NUM_THREADS],
                 "nilSchGoSleepTimeoutS(), #1", "pointer out of range");
  }
}

/**
 * @brief   Sends the current thread sleeping and sets a reference variable.
 * @note    This function must reschedule, it can only be called from thread
 *          context.
 *
 * @param[in] trp       a pointer to a thread reference object
 * @param[in] timeout   the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return              The wake up message.
 *
 * @sclass
 */
msg_t nilThdSuspendTimeoutS(thread_ref_t *trp, systime_t timeout) {

  nilDbgAssert(*trp == NULL, "nilThdSuspendTimeoutS(), #1", "not NULL");

  *trp = nil.current;
  nil.current->u1.trp = trp;
  return nilSchGoSleepTimeoutS(NIL_THD_SUSP, timeout);
}

/**
 * @brief   Wakes up a thread waiting on a thread reference object.
 * @note    This function must not reschedule because it can be called from
 *          ISR context.
 *
 * @param[in] trp       a pointer to a thread reference object
 * @param[in] msg       the message code
 *
 * @iclass
 */
void nilThdResumeI(thread_ref_t *trp, msg_t msg) {

  if (*trp != NULL) {
    thread_ref_t tr = *trp;

    nilDbgAssert(NIL_THD_IS_SUSP(tr), "nilThdResumeI(), #1", "not suspended");

    *trp = NULL;
    nilSchReadyI(tr, msg);
  }
}

/**
 * @brief   Suspends the invoking thread for the specified time.
 *
 * @param[in] time      the delay in system ticks
 *
 * @api
 */
void nilThdSleep(systime_t time) {

  nilSysLock();
  nilThdSleepS(time);
  nilSysUnlock();
}

/**
 * @brief   Suspends the invoking thread until the system time arrives to the
 *          specified value.
 *
 * @param[in] time      absolute system time
 *
 * @api
 */
void nilThdSleepUntil(systime_t time) {

  nilSysLock();
  nilThdSleepUntilS(time);
  nilSysUnlock();
}

/**
 * @brief   Checks if the current system time is within the specified time
 *          window.
 * @note    When start==end then the function returns always true because the
 *          whole time range is specified.
 *
 * @param[in] start     the start of the time window (inclusive)
 * @param[in] end       the end of the time window (non inclusive)
 *
 * @retval true         current time within the specified time window.
 * @retval false        current time not within the specified time window.
 *
 * @api
 */
bool nilTimeIsWithin(systime_t start, systime_t end) {

  systime_t time = nilTimeNow();
  return end > start ? (time >= start) && (time < end) :
                       (time >= start) || (time < end);
}

/**
 * @brief   Performs a wait operation on a semaphore with timeout specification.
 *
 * @param[in] sp        pointer to a @p semaphore_t structure
 * @param[in] timeout   the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_IMMEDIATE immediate timeout.
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return              A message specifying how the invoking thread has been
 *                      released from the semaphore.
 * @retval NIL_MSG_OK   if the thread has not stopped on the semaphore or the
 *                      semaphore has been signaled.
 * @retval NIL_MSG_RST  if the semaphore has been reset using @p nilSemReset().
 * @retval NIL_MSG_TMO  if the semaphore has not been signaled or reset within
 *                      the specified timeout.
 *
 * @api
 */
msg_t nilSemWaitTimeout(semaphore_t *sp, systime_t timeout) {
  msg_t msg;

  nilSysLock();
  msg = nilSemWaitTimeoutS(sp, timeout);
  nilSysUnlock();
  return msg;
}

/**
 * @brief   Performs a wait operation on a semaphore with timeout specification.
 *
 * @param[in] sp        pointer to a @p semaphore_t structure
 * @param[in] timeout   the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_IMMEDIATE immediate timeout.
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return              A message specifying how the invoking thread has been
 *                      released from the semaphore.
 * @retval NIL_MSG_OK   if the thread has not stopped on the semaphore or the
 *                      semaphore has been signaled.
 * @retval NIL_MSG_RST  if the semaphore has been reset using @p nilSemReset().
 * @retval NIL_MSG_TMO  if the semaphore has not been signaled or reset within
 *                      the specified timeout.
 *
 * @sclass
 */
msg_t nilSemWaitTimeoutS(semaphore_t *sp, systime_t timeout) {

  /* Note, the semaphore counter is a volatile variable so accesses are
     manually optimized.*/
  cnt_t cnt = sp->cnt;
  if (cnt <= 0) {
    if (TIME_IMMEDIATE == timeout)
      return NIL_MSG_TMO;
    sp->cnt = cnt - 1;
    nil.current->u1.semp = sp;
    return nilSchGoSleepTimeoutS(NIL_THD_WTSEM, timeout);
  }
  return NIL_MSG_OK;
}

/**
 * @brief   Performs a signal operation on a semaphore.
 * @post    This function does not reschedule so a call to a rescheduling
 *          function must be performed before unlocking the kernel. Note that
 *          interrupt handlers always reschedule on exit so an explicit
 *          reschedule must not be performed in ISRs.
 *
 * @param[in] sp    pointer to a @p semaphore_t structure
 *
 * @api
 */
void nilSemSignal(semaphore_t *sp) {

  nilSysLock();
  nilSemSignalI(sp);
  nilSchRescheduleS();
  nilSysUnlock();
}

/**
 * @brief   Performs a signal operation on a semaphore.
 * @post    This function does not reschedule so a call to a rescheduling
 *          function must be performed before unlocking the kernel. Note that
 *          interrupt handlers always reschedule on exit so an explicit
 *          reschedule must not be performed in ISRs.
 *
 * @param[in] sp    pointer to a @p semaphore_t structure
 *
 * @iclass
 */
void nilSemSignalI(semaphore_t *sp) {

  if (++sp->cnt <= 0) {
    thread_ref_t tr = nil.threads;
    while (true) {
      /* Is this thread waiting on this semaphore?*/
      if (tr->u1.semp == sp) {

        nilDbgAssert(NIL_THD_IS_WTSEM(tr),
                     "nilSemSignalI(), #1", "not waiting");

        nilSchReadyI(tr, NIL_MSG_OK);
        return;
      }
      tr++;
    }
  }
}

/**
 * @brief   Performs a reset operation on the semaphore.
 * @post    After invoking this function all the threads waiting on the
 *          semaphore, if any, are released and the semaphore counter is set
 *          to the specified, non negative, value.
 * @post    This function does not reschedule so a call to a rescheduling
 *          function must be performed before unlocking the kernel. Note that
 *          interrupt handlers always reschedule on exit so an explicit
 *          reschedule must not be performed in ISRs.
 *
 * @param[in] sp        pointer to a @p semaphore_t structure
 * @param[in] n         the new value of the semaphore counter. The value must
 *                      be non-negative.
 *
 * @api
 */
void nilSemReset(semaphore_t *sp, cnt_t n) {

  nilSysLock();
  nilSemResetI(sp, n);
  nilSchRescheduleS();
  nilSysUnlock();
}

/**
 * @brief   Performs a reset operation on the semaphore.
 * @post    After invoking this function all the threads waiting on the
 *          semaphore, if any, are released and the semaphore counter is set
 *          to the specified, non negative, value.
 * @post    This function does not reschedule so a call to a rescheduling
 *          function must be performed before unlocking the kernel. Note that
 *          interrupt handlers always reschedule on exit so an explicit
 *          reschedule must not be performed in ISRs.
 *
 * @param[in] sp        pointer to a @p semaphore_t structure
 * @param[in] n         the new value of the semaphore counter. The value must
 *                      be non-negative.
 *
 * @iclass
 */
void nilSemResetI(semaphore_t *sp, cnt_t n) {
  thread_ref_t tr;
  cnt_t cnt;

  cnt = sp->cnt;
  sp->cnt = n;
  tr = nil.threads;
  while (cnt < 0) {
    /* Is this thread waiting on this semaphore?*/
    if (tr->u1.semp == sp) {

      nilDbgAssert(NIL_THD_IS_WTSEM(tr),
                   "nilSemResetI(), #1", "not waiting");

      cnt++;
      nilSchReadyI(tr, NIL_MSG_RST);
    }
    tr++;
  }
}

/** @} */
