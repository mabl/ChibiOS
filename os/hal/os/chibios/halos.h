/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

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
 * @file    halos.h
 * @brief   HAL OS abstraction macros and structures.
 *
 * @addtogroup HALOS
 * @{
 */

#ifndef _HALOS_H_
#define _HALOS_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "ch.h"

/*===========================================================================*/
/* Debug.                                                                    */
/*===========================================================================*/

#define halOsDbgPanic(msg) chDbgPanic(msg)
#define halOsDbgAssert(c, msg, remark) chDbgAssert(c, msg, remark)
#define halOsDbgCheck(c, func) chDbgCheck(c, func)
#define halOsDbgCheckClassI() chDbgCheckClassI()
#define halOsDbgCheckClassS() chDbgCheckClassS()

/*===========================================================================*/
/* Low level messages.                                                       */
/*===========================================================================*/

#define HAL_OS_MSG_OK       RDY_OK
#define HAL_OS_MSG_RESET    RDY_RESET
#define HAL_OS_MSG_TIMEOUT  RDY_TIMEOUT

typedef int32_t hal_os_message_t;

/*===========================================================================*/
/* Critical zones abstraction.                                               */
/*===========================================================================*/

/**
 * @brief   Enters a critical zone from thread context.
 *
 * @special
 */
#define halOsLock() chSysLock()

/**
 * @brief   Leaves a critical zone from thread context.
 *
 * @special
 */
#define halOsUnlock() chSysUnlock()

/**
 * @brief   Enters a critical zone from ISR context.
 *
 * @special
 */
#define halOsLockFromISR() chSysLockFromIsr()

/**
 * @brief   Leaves a critical zone from ISR context.
 *
 * @special
 */
#define halOsUnlockFromISR() chSysLockFromIsr()

/*===========================================================================*/
/* Thread references abstraction.                                            */
/*===========================================================================*/

typedef void * hal_os_thread_reference_t;

/*===========================================================================*/
/* Mutexes abstraction.                                                      */
/*===========================================================================*/

#if CH_USE_MUTEXES || defined(__DOXYGEN__)
typedef Mutex hal_os_mutex_t;

#define halOsMutexInit(mp) chMtxInit(mp)
#define halOsMutexLock(mp) chMtxLock(mp)
#define halOsMutexUnlock(mp) {                                              \
  (void)mp;                                                                 \
  chMtxUnlock();                                                            \
}

#elif CH_USE_SEMAPHORES
typedef Semaphore hal_os_mutex_t;

#define halOsMutexInit(mp) chSemInit(mp)
#define halOsMutexLock(mp) chSemWait(mp)
#define halOsMutexUnlock(mp) chSemSignal(mp)

#else
#error "HAL OS abstraction requires CH_USE_MUTEXES or CH_USE_SEMAPHORES"
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void halOsInit(void);
  hal_os_message_t halOsThreadSuspendS(hal_os_thread_reference_t *trp);
  void halOsThreadResumeS(hal_os_thread_reference_t tr, hal_os_message_t msg);
  void halOsThreadResumeI(hal_os_thread_reference_t tr, hal_os_message_t msg);
#ifdef __cplusplus
}
#endif

#endif /* _HALOS_H_ */

/** @} */
