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
 * @file    nilconf.h
 * @brief   Configuration file template.
 * @details A copy of this file must be placed in each project directory, it
 *          contains the application specific kernel settings.
 *
 * @addtogroup config
 * @details Kernel related settings and hooks.
 * @{
 */

#ifndef _NILCONF_H_
#define _NILCONF_H_

/**
 * @brief   Number of threads in the application.
 * @note    This number is not inclusive of the idle thread which is
 *          implicitely handled.
 */
#if !defined(NIL_CFG_NUM_THREADS) || defined(__DOXYGEN__)
#define NIL_CFG_NUM_THREADS             2
#endif

/**
 * @brief   System tick frequency.
 */
#if !defined(NIL_FREQUENCY) || defined(__DOXYGEN__)
#define NIL_CFG_FREQUENCY               100
#endif

/**
 * @brief   System assertions.
 */
#if !defined(NIL_CFG_ENABLE_ASSERTS) || defined(__DOXYGEN__)
#define NIL_CFG_ENABLE_ASSERTS          TRUE
#endif

/*===========================================================================*/
/* Port-specific settings (override port settings defaulted in nilcore.h).   */
/*===========================================================================*/

#endif  /* _NILCONF_H_ */

/** @} */
