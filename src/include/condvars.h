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
  /* mutex protecting access to the condition */
  Mutex c_mutex;
  /** Queue of the threads sleeping on this CondVar. */
  ThreadsQueue c_queue;
} CondVar;

#ifdef __cplusplus
extern "C" {
#endif
  void chCondInit(CondVar *cp);
  void chCondLock(CondVar *cp);
  void chCondUnlock(void);
  void chCondSignal(void);
  void chCondSignalS(void);
  void chCondBroadcast(void);
  void chCondBroadcastS(void);
  msg_t chCondWait(void);
  msg_t chCondWaitS(void);
#ifdef __cplusplus
}
#endif

#endif /* CH_USE_CONDVARS */

#endif /* _CONDVARS_H_ */

/** @} */
