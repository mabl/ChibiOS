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
 * @file    chdynamic.h
 * @brief   Dynamic threads macros and structures.
 *
 * @addtogroup dynamic_threads
 * @{
 */

#ifndef _CHDYNAMIC_H_
#define _CHDYNAMIC_H_

#if CH_CFG_USE_DYNAMIC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*
 * Module dependencies check.
 */
#if CH_CFG_USE_DYNAMIC && !CH_CFG_USE_WAITEXIT
#error "CH_CFG_USE_DYNAMIC requires CH_CFG_USE_WAITEXIT"
#endif
#if CH_CFG_USE_DYNAMIC && !CH_CFG_USE_HEAP && !CH_CFG_USE_MEMPOOLS
#error "CH_CFG_USE_DYNAMIC requires CH_CFG_USE_HEAP and/or CH_CFG_USE_MEMPOOLS"
#endif

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/*
 * Dynamic threads APIs.
 */
#ifdef __cplusplus
extern "C" {
#endif
  thread_t *chThdAddRef(thread_t *tp);
  void chThdRelease(thread_t *tp);
#if CH_CFG_USE_HEAP
  thread_t *chThdCreateFromHeap(memory_heap_t *heapp, size_t size,
                                tprio_t prio, tfunc_t pf, void *arg);
#endif
#if CH_CFG_USE_MEMPOOLS
  thread_t *chThdCreateFromMemoryPool(memory_pool_t *mp, tprio_t prio,
                                      tfunc_t pf, void *arg);
#endif
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* CH_CFG_USE_DYNAMIC */

#endif /* _CHDYNAMIC_H_ */

/** @} */
