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
NilSystem nil;

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Switches to the specified thread.
 *
 * @param[in] ntp       pointer to a @p Thread object
 *
 * @notapi
 */
static void nil_switch_to(Thread *ntp) {
  Thread *otp = nil.currp;
  nil.currp = ntp;
//  port_switch(ntp, otp);
}

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
  const ThreadConfig *tcp = nil_thd_configs;
  Thread *tp = nil.threads;

  /* Iterates through the list of defined threads.*/
  while (TRUE) {

    /* The thread is marked as not waiting on anything thus runnable.*/
    tp->waitobj.p = NULL;

    /* Port dependent thread initialization.*/
    SETUP_CONTEXT(tcp->wap, tcp->size, tcp->funcp, tcp->arg);

    if (tcp->funcp == NULL) {
      /* The last thread found in the list is the idle thread and it is
         associated to the current instructions flow.*/
      nil.currp = tp;
      break;
    }

    /* Next thread in the initialization list.*/
    tp++;
  }

  /* Runs the highest priority thread.*/
  nilSchRescheduleS();

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
  Thread *tp = nil.threads;

  nil.systime++;
  do {
    if ((tp->mode & NIL_THD_TIMEOUT) && (tp->wakeup.time == nil.systime)) {
      /* TODO: Assert it is ready because NIL_THD_TIMEOUT is set.*/
      nilSchReadyI(tp);
    }
  } while (tp < &nil.threads[NIL_CFG_NUM_THREADS]);
}

/**
 * @brief   Reschedules.
 *
 * @sclass
 */
void nilSchRescheduleS() {
  Thread *tp = nil.threads;
  while (TRUE) {
    /* Is this thread ready to execute?*/
    if (tp->waitobj.p == NULL) {

      /* If the found thread is the current one then there is nothing
         to reschedule.*/
      if (tp != nil.currp)
        nil_switch_to(tp);
      return;
    }
    tp++;
  }
}

/**
 * @brief   Puts the current thread to sleep into the specified state with
 *          timeout specification.
 * @details The thread goes into a sleeping state, if it is not awakened
 *          explicitly within the specified timeout then it is forcibly
 *          awakened with a @p NIL_MSG_TMO low level message.
 *
 * @param[in] newstate  the new thread state
 * @param[in] time      the number of ticks before the operation timeouts, the
 *                      special values are handled as follow:
 *                      - @a TIME_INFINITE the thread enters an unbounded sleep
 *                        state.
 *                      - @a TIME_IMMEDIATE this value is not allowed.
 *                      .
 * @return              The wakeup message.
 * @retval NIL_MSG_TMO if a timeout occurs.
 *
 * @sclass
 */
msg_t nilSchGoSleepTimeoutS(void *waitobj, systime_t time) {
  Thread *tp = nil.currp;

  if (time != TIME_INFINITE) {
    tp->mode |= NIL_THD_TIMEOUT;
    tp->wakeup.time = time;
  }
  else
    tp->mode &= ~NIL_THD_TIMEOUT;

  /* Storing the wait object for the current thread.*/
  tp->waitobj.p = waitobj;

  /* Starting the scanning from the thread going to sleep because it is
     supposed to have the highest priority among the ready ones.*/
  while (TRUE) {
    /* Points to the next thread in lowering priority order.*/
    tp++;

    /* Is this thread ready to execute?*/
    if (tp->waitobj.p == NULL) {
      nil_switch_to(tp);
      return tp->wakeup.msg;
    }
  }
}

/**
 * @brief   Checks if the current system time is within the specified time
 *          window.
 * @note    When start==end then the function returns always true because the
 *          whole time range is specified.
 *
 * @param[in] start     the start of the time window (inclusive)
 * @param[in] end       the end of the time window (non inclusive)
 * @retval TRUE         current time within the specified time window.
 * @retval FALSE        current time not within the specified time window.
 *
 * @api
 */
bool_t nilTimeIsWithin(systime_t start, systime_t end) {

  systime_t time = nilTimeNow();
  return end > start ? (time >= start) && (time < end) :
                       (time >= start) || (time < end);
}

/**
 * @brief   Performs a wait operation on a semaphore with timeout specification.
 *
 * @param[in] sp        pointer to a @p Semaphore structure
 * @param[in] time      the number of ticks before the operation timeouts,
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
msg_t nilSemWaitTimeout(Semaphore *sp, systime_t time) {
  msg_t msg;

  nilSysLock();
  msg = nilSemWaitTimeoutS(sp, time);
  nilSysUnlock();
  return msg;
}

/**
 * @brief   Performs a wait operation on a semaphore with timeout specification.
 *
 * @param[in] sp        pointer to a @p Semaphore structure
 * @param[in] time      the number of ticks before the operation timeouts,
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
msg_t nilSemWaitTimeoutS(Semaphore *sp, systime_t time) {

  /* Note, the semaphore counter is a volatile variable so accesses are
     manually optimized.*/
  cnt_t cnt = sp->cnt;
  if (cnt <= 0) {
    if (TIME_IMMEDIATE == time)
      return NIL_MSG_TMO;
    sp->cnt = cnt - 1;
    return nilSchGoSleepTimeoutS((void *)sp, time);
  }
  return NIL_MSG_TMO;
}

/**
 * @brief   Performs a signal operation on a semaphore.
 * @post    This function does not reschedule so a call to a rescheduling
 *          function must be performed before unlocking the kernel. Note that
 *          interrupt handlers always reschedule on exit so an explicit
 *          reschedule must not be performed in ISRs.
 *
 * @param[in] sp    pointer to a @p Semaphore structure
 *
 * @api
 */
void nilSemSignal(Semaphore *sp) {

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
 * @param[in] sp    pointer to a @p Semaphore structure
 *
 * @iclass
 */
void nilSemSignalI(Semaphore *sp) {

  if (++sp->cnt <= 0) {
    Thread *tp = nil.threads;
    while (TRUE) {
      /* Is this thread waiting on this semaphore?*/
      if (tp->waitobj.semp == sp) {
        tp->wakeup.msg = NIL_MSG_OK;
        nilSchReadyI(tp);
        return;
      }
      tp++;
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
 * @param[in] sp        pointer to a @p Semaphore structure
 * @param[in] n         the new value of the semaphore counter. The value must
 *                      be non-negative.
 *
 * @api
 */
void nilSemReset(Semaphore *sp, cnt_t n) {

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
 * @param[in] sp        pointer to a @p Semaphore structure
 * @param[in] n         the new value of the semaphore counter. The value must
 *                      be non-negative.
 *
 * @iclass
 */
void nilSemResetI(Semaphore *sp, cnt_t n) {
  Thread *tp;
  cnt_t cnt;

  cnt = sp->cnt;
  sp->cnt = n;
  tp = nil.threads;
  while (cnt < 0) {
    /* Is this thread waiting on this semaphore?*/
    if (tp->waitobj.semp == sp) {
      cnt++;
      tp->wakeup.msg = NIL_MSG_RST;
      nilSchReadyI(tp);
    }
    tp++;
  }
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
 * @api
 */
void nilThdSleep(systime_t time) {

  nilSysLock();
  nilSchGoSleepTimeoutS(nil.currp, nilTimeNow() + time);
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
  nilSchGoSleepTimeoutS(nil.currp, time);
  nilSysUnlock();
}

/**
 * @brief   Stops the current thread.
 *
 * @api
 */
void nilThdExit(void) {

  /* Unbounded waiting for itself is the final state.*/
  nilSysLock();
  nilSchGoSleepTimeoutS(nil.currp, TIME_INFINITE);
}

/** @} */
