/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

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
 * @file    templates/hal_lld.h
 * @brief   HAL subsystem low level driver header template.
 *
 * @addtogroup HAL
 * @{
 */

#ifndef _HAL_LLD_H_
#define _HAL_LLD_H_

#include "bcm2835.h"

#define UNUSED(x) (void)(x)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Platform name.
 */
#define PLATFORM_NAME   "BCM2835"

/**
 * @brief Extension of ChibiOS's PAL output modes.
 *
 */
#define PAL_MODE_OUTPUT 0xFF

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

#define HAL_IMPLEMENTS_COUNTERS 1

typedef uint32_t halrtcnt_t;

#define hal_lld_get_counter_value() SYSTIMER_CLO
#define hal_lld_get_counter_frequency() 1000002

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void hal_lld_init(void);

  void delayMicroseconds(uint32_t n);

  void watchdog_start ( uint32_t timeout );
  void watchdog_stop ( void );
  uint32_t watchdog_get_remaining ( void );

#ifdef __cplusplus
}
#endif

#endif /* _HAL_LLD_H_ */

/** @} */
