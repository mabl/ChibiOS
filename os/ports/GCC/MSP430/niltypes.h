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
 * @file    GCC/MSP430/niltypes.h
 * @brief   Nil RTOS types header file.
 *
 * @addtogroup MSP430_TYPES
 * @{
 */

#ifndef _NILTYPES_H_
#define _NILTYPES_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * @name Kernel types
 * @{
 */
typedef uint8_t         tstate_t;   /**< @brief Type of a thread state.     */
typedef int16_t         msg_t;      /**< @brief Type of a message.          */
typedef uint16_t        systime_t;  /**< @brief Type of system time.        */
typedef int16_t         cnt_t;      /**< @brief Type of signed counter.     */
/** @} */

#endif /* _NILTYPES_H_ */

/** @} */
