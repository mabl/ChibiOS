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
 * @file    GCC/AVR/niltypes.h
 * @brief   Nil RTOS types header file.
 *
 * @addtogroup AVR_TYPES
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
typedef int8_t          bool_t;     /**< @brief Fast boolean type.          */
typedef int16_t         msg_t;      /**< @brief Inter-thread message.       */
typedef uint16_t        systime_t;  /**< @brief System time.                */
typedef int8_t          cnt_t;      /**< @brief Signed resources counter.   */
/** @} */

#endif /* _NILTYPES_H_ */

/** @} */
