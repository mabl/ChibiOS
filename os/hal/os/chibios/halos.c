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
 * @file    halos.c
 * @brief   HAL OS abstraction code.
 *
 * @addtogroup HALOS
 * @{
 */

#include "halos.h"

/*===========================================================================*/
/* Initialization.                                                           */
/*===========================================================================*/

/**
 * @brief   HAL OS abstraction initialization.
 * @note    This function is implicitly invoked by @p halInit(), there is
 *          no need to explicitly initialize the module.
 *
 * @init
 */
void halOsInit(void) {

}

/*===========================================================================*/
/* Thread references abstraction.                                            */
/*===========================================================================*/

hal_os_message_t halOsThreadSuspendS(hal_os_thread_reference_t *trp) {

  *trp = NULL;
  return HAL_OS_MSG_OK;
}

void halOsThreadResumeS(hal_os_thread_reference_t tr, hal_os_message_t msg) {

}

void halOsThreadResumeI(hal_os_thread_reference_t tr, hal_os_message_t msg) {

}

/** @} */
