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
 * @file    osal.c
 * @brief   OSAL module code.
 *
 * @addtogroup OSAL
 * @{
 */

#include "osal.h"

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
 * @brief   OSAL module initialization.
 *
 * @api
 */
void osalInit(void) {

  /* Executing configured initialization code.*/
  osal_cfg_init();

  /* INTC initialization, software vector mode, 4 bytes vectors, starting
     at priority 0.*/
  INTC_BCR      = 0;
  INTC_CPR      = 0;
  INTC_IACKR    = (uint32_t)_vectors;
}

/**
 * @brief   Sends the current thread sleeping and sets a reference variable.
 * @note    This function must reschedule, it can only be called from thread
 *          context.
 *
 * @param[in] trp       a pointer to a thread reference object
 * @return              The wake up message.
 *
 * @sclass
 */
msg_t osalThreadSuspendS(thread_reference_t *trp) {

  chDbgAssert(*trp == NULL, "osalThreadSuspendS(), #1", "not NULL");

  *trp = (thread_reference_t)chThdSelf();
  chSchGoSleepS(THD_STATE_SUSPENDED);
  return chThdSelf()->p_msg;
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
void osalThreadResumeI(thread_reference_t *trp, msg_t msg) {

  if (*trp != NULL) {

    chDbgAssert((*trp)->p_state == THD_STATE_SUSPENDED,
                "osalThreadResumeI(), #1", "not THD_STATE_SUSPENDED");

    (*trp)->p_u.rdymsg = msg;
    chSchReadyI((Thread *)*trp);
    *trp = NULL;
  }
}

/**
 * @brief   Wakes up a thread waiting on a thread reference object.
 * @note    This function must reschedule, it can only be called from thread
 *          context.
 *
 * @param[in] trp       a pointer to a thread reference object
 * @param[in] msg       the message code
 *
 * @iclass
 */
void osalThreadResumeS(thread_reference_t *trp, msg_t msg) {

  if (*trp != NULL) {
    Thread *tp = (Thread *)*trp;

    chDbgAssert(tp->p_state == THD_STATE_SUSPENDED,
                "osalThreadResumeS(), #1", "not THD_STATE_SUSPENDED");

    *trp = NULL;
    chSchWakeupS(tp, msg);
  }
}

/**
 * @brief   Enqueues the caller thread.
 * @details The caller thread is enqueued and put to sleep until it is
 *          dequeued or the specified timeouts expires.
 *
 * @param[in] tqp       pointer to the threads queue object
 * @param[in] time      the timeout in system ticks, the special values are
 *                      handled as follow:
 *                      - @a TIME_INFINITE the thread enters an infinite sleep
 *                        state.
 *                      - @a TIME_IMMEDIATE the thread is not enqueued and
 *                        the function returns @p MSG_TIMEOUT as if a timeout
 *                        occurred.
 *                      .
 * @return              The message from @p osalQueueWakeupOneI() or
 *                      @p osalQueueWakeupAllI() functions.
 * @retval RDY_TIMEOUT  if the thread has not been dequeued within the
 *                      specified timeout or if the function has been
 *                      invoked with @p TIME_IMMEDIATE as timeout
 *                      specification.
 *
 * @sclass
 */
msg_t osalThreadEnqueueTimeoutS(threads_queue_t *tqp, systime_t time) {

  void wakeup(void *p) {
    Thread *tp = (Thread *)p;

    chSysUnlockFromIsr();
    tp->p_u.rdymsg = RDY_TIMEOUT;
    chSchReadyI(dequeue(tp));
    chSysUnlockFromIsr();
  }

  if (TIME_IMMEDIATE == time)
    return MSG_TIMEOUT;

  queue_insert(currp, tqp);
  if (TIME_INFINITE == time)
    chSchGoSleepS(THD_STATE_SUSPENDED);
  else {
    VirtualTimer vt;

    chVTSetI(&vt, time, wakeup, currp);
    chSchGoSleepS(THD_STATE_SUSPENDED);
    if (chVTIsArmedI(&vt))
      chVTResetI(&vt);
  }
  return currp->p_u.rdymsg;
}

/**
 * @brief   Dequeues and wakes up one thread from the queue, if any.
 *
 * @param[in] tqp       pointer to the threads queue object
 * @param[in] msg       the message code
 *
 * @iclass
 */
void osalThreadDequeueNextI(threads_queue_t *tqp, msg_t msg) {

  if (notempty(tqp)) {
    Thread *tp = fifo_remove(tqp);
    tp->p_u.rdymsg = msg;
    chSchReadyI(tp);
  }
}

/**
 * @brief   Dequeues and wakes up all threads from the queue.
 *
 * @param[in] tqp       pointer to the threads queue object
 * @param[in] msg       the message code
 *
 * @iclass
 */
void osalThreadDequeueAllI(threads_queue_t *tqp, msg_t msg) {

  while (notempty(tqp)) {
    Thread *tp = fifo_remove(tqp);
    tp->p_u.rdymsg = msg;
    chSchReadyI(tp);
  }
}

/** @} */
