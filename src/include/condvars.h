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
 * @addtogroup Conditional Variables
 * @{
 */

#ifndef _CONDVARS_H_
#define _CONDVARS_H_

#ifdef CH_USE_CONDVARS

typedef struct CondVar {
  ThreadsQueue  c_queue;        /* Threads sleeping on this CondVar.    */
  Mutex         c_mutex;        /* Mutext protecting the condvar.       */
} CondVar;

#ifdef __cplusplus
extern "C" {
#endif
  void chCondInit(CondVar *cp);
  void chCondSignal(CondVar *cp);
  void chCondSignalI(CondVar *cp);
  void chCondBroadcast(CondVar *cp);
  void chCondBroadcastI(CondVar *cp);
  msg_t chCondWaitTimeout(CondVar *cp, systime_t time);
  msg_t chCondWaitTimeoutS(CondVar *cp, systime_t time);
#ifdef __cplusplus
}
#endif

#define chCondLock(cp) chMtxLock(&(cp)->c_mutex)
#define chCondUnlock() chMtxUnlock()
#define chCondWait(cp) chCondWaitTimeout(cp, TIME_INFINITE)
#define chCondWaitS(cp) chCondWaitTimeoutS(cp, TIME_INFINITE)

#endif /* CH_USE_CONDVARS */

#endif /* _CONDVARS_H_ */

/** @} */
