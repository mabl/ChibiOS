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
 * @param cp pointer to a \p CondVar structure
 */
void chCondInit(CondVar *cp) {

  fifo_init(&cp->c_queue);
}

/**
 * Signals one thread that is waiting on the condition variable.
 *
 * @param mp pointer to the \p CondVar structure
 */
void chCondSignal(CondVar *cp) {

  chSysLock();

  chCondSignalS(cp);

  chSysUnlock();
}

/**
 * Signals one thread that is waiting on the condition variable.
 *
 * @param cp pointer to the \p CondVar structure
 * @note This function must be called within a \p chSysLock() / \p chSysUnlock()
 *       block.
 */
void chCondSignalS(CondVar *cp) {
  /* any thread waiting on the condition variable? */
  if (notempty(&cp->c_queue))
    /* wake up the first thread */
    chSchWakeupS(fifo_remove(&cp->c_queue), RDY_OK);
}

/**
 * Signal all threads that are waiting on the condition variable.
 *
 * @param mp pointer to the \p CondVar structure
 */
void chCondBroadcast(CondVar *cp) {

  chSysLock();

  chCondBroadcastS(cp);
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
void chCondBroadcastS(CondVar *cp) {

  /* empties the condition variable queue and inserts all the Threads into the
   * ready list in FIFO order. The wakeup message is set to \p RDY_RESET in
   * order to make a chCondBroadcast() detectable from a chCondSignal(). */
  while (cp->c_queue.p_next != (void *)&cp->c_queue)
    chSchReadyI(fifo_remove(&cp->c_queue))->p_rdymsg = RDY_RESET;
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
msg_t chCondWait(CondVar *cp) {
  msg_t msg;

  chSysLock();

  msg = chCondWaitS(cp);

  chSysUnlock();
  return msg;
}

msg_t chCondWaitS(CondVar *cp) {
  Mutex *mp = currp->p_mtxlist; /* Last locked mutex. */

  chDbgAssert(mp != NULL, "chcond.c, chCondWaitS()");

  chMtxUnlockS();                       /* Unlocks it */
  prio_insert(currp, &cp->c_queue);     /* enters the condvar queue */
  currp->p_wtcondp = cp;
  chSchGoSleepS(PRWTCOND);
  chMtxLockS(mp);                       /* atomically relocks the same mutex */
  return currp->p_rdymsg;
}

#endif /* CH_USE_CONDVARS */

/** @} */
