/*
    ChibiOS/RT - Copyright (C) 2006-2007 Giovanni Di Sirio.

    This file is part of ChibiOS/RT and Copyright (C) 2008 Leon Woestenberg.

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
 * @addtogroup CondVars
 * @{
 */

#include <ch.h>

#ifdef CH_USE_CONDVARS

/**
 * Initializes s \p CondVar structure.
 *
 * Condition variables are synchronization primitives that enable threads
 * to wait until a particular condition occurs. The condition itself is
 * external to the CondVar structure, but access to the condition should
 * always occur inbetween a \p chCondLock() \p chCondUnlock() block.
 *
 * @param cp pointer to a \p CondVar structure
 */
void chCondInit(CondVar *cp) {
  chMtxInit(&cp->c_mutex);
  fifo_init(&cp->c_queue);
}

void chCondLock(CondVar *cp) {
  chSysLock();
  chDbgAssert(currp->p_condvar == NULL, "chcond.c, chCondLock()");
  chMtxLockS(&cp->c_mutex);
  currp->p_condvar = cp;
  chSysUnlock();
}

void chCondUnlock() {
  chSysLock();
  /* verify the condition variable is locked, and its mutex was locked last */
  chDbgAssert(currp->p_mtxlist == &currp->p_condvar->c_mutex, "chcond.c, chCondUnlock()");
  chMtxUnlockS();
  currp->p_condvar = NULL;
  chSysUnlock();
}

/**
 * Signals one thread that is waiting on the condition variable.
 *
 * @param mp pointer to the \p CondVar structure
 */
void chCondSignal(void) {

  chSysLock();
  chCondSignalS();
  chSysUnlock();
}

/**
 * Signals one thread that is waiting on the condition variable.
 *
 * @param cp pointer to the \p CondVar structure
 * @note This function must be called within a \p chSysLock() / \p chSysUnlock()
 *       block.
 */
void chCondSignalS(void) {
  /* verify the condition variable is locked, and its mutex was locked last */
  chDbgAssert(currp->p_mtxlist == &currp->p_condvar->c_mutex, "chcond.c, chCondUnlock()");
  /* any thread waiting on the condition variable? */
  if (notempty(&currp->p_condvar->c_queue))
    /* wake up the first thread */
    chSchWakeupS(fifo_remove(&currp->p_condvar->c_queue), RDY_OK);
}

/**
 * Signal all threads that are waiting on the condition variable.
 *
 * The caller must call chCondLock() before and chCondUnlock() after.
 *  *
 * @param mp pointer to the \p CondVar structure
 */
void chCondBroadcast(void) {

  chSysLock();
  chCondBroadcastS();
  chSchRescheduleS();
  chSysUnlock();
}

/**
 * Signal all threads that are waiting on the condition variable.
 *
 * @param cp pointer to the \p CondVar structure
 * @note This function must be called within a \p chSysLock() / \p chSysUnlock()
 *
 * @todo If priority inheritance is disabled, we want the threads to appear in
 * FIFO order on the ready list. Check if we need to iterate backwards?
 */
void chCondBroadcastS(void) {

  /* verify the condition variable is locked, and its mutex was locked last */
  chDbgAssert(currp->p_mtxlist == &currp->p_condvar->c_mutex, "chcond.c, chCondUnlock()");
  /* empties the condition variable queue and inserts all the Threads into the
   * ready list in FIFO order. The wakeup message is set to \p RDY_RESET in
   * order to make a chCondBroadcast() detectable from a chCondSignal(). */
  while (&currp->p_condvar->c_queue.p_next != (void *)&currp->p_condvar->c_queue)
    chSchReadyI(fifo_remove(&currp->p_condvar->c_queue))->p_rdymsg = RDY_RESET;
}

/**
 * Wait on the condition variable protected by the given mutex.
 *
 * Release the mutex, wait on the condition variable, and lock the mutex. This
 * is done atomically.
 *
 * The thread MUST already have locked the mutex when calling chCondWait().
 *
 * @param cp pointer to the \p CondVar structure
 * @note This function must be called within a \p chSysLock() / \p chSysUnlock()
 */
msg_t chCondWait(void) {
  msg_t msg;
  chSysLock();
  msg = chCondWaitS();
  chSysUnlock();
  return msg;
}

msg_t chCondWaitS(void) {
  /* last locked mutex should be the condition variable mutex */
  chDbgAssert(currp->p_mtxlist == &currp->p_condvar->c_mutex, "chcond.c, chCondWaitS()");
  /* unlock the condition variable mutex but remember the condition variable */
  chMtxUnlockS();
  /* enter the condvar queue */
  prio_insert(currp, &currp->p_condvar->c_queue);
  chSchGoSleepS(PRWTCOND);
  /* atomically relock the condition variable's mutex */
  chMtxLockS(&currp->p_condvar->c_mutex);
  return currp->p_rdymsg;
}

#endif /* CH_USE_CONDVARS */

/** @} */
