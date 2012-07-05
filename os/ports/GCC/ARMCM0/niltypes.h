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
 * @file    niltypes.h
 * @brief   Nil RTOS types header file.
 *
 * @addtogroup ARMCM0_TYPES
 * @{
 */

#ifndef _NILTYPES_H_
#define _NILTYPES_H_

#define __need_NULL
#define __need_size_t
#define __need_ptrdiff_t
#include <stddef.h>
#include <stdint.h>

/**
 * @name Kernel types
 * @{
 */
typedef int32_t         bool_t;     /**< @brief Fast boolean type.          */
typedef uint8_t         tmode_t;    /**< @brief Thread flags.               */
typedef int32_t         msg_t;      /**< @brief Inter-thread message.       */
typedef uint32_t        systime_t;  /**< @brief System time.                */
typedef int32_t         cnt_t;      /**< @brief Signed resources counter.   */
/** @} */

#endif /* _NILTYPES_H_ */

/** @} */
