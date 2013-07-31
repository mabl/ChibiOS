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
 * @file    dbguart.h
 * @brief   DBGUART Driver macros and structures.
 *
 * @addtogroup DBGUART
 * @{
 */

#ifndef _DBGUART_H_
#define _DBGUART_H_

#if HAL_USE_DBGUART || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/
/**
 * @brief   Default bit rate for debug UART.
 * @details Configuration parameter, this is the baud rate selected for the
 *          default configuration.
 */
#if !defined(DBGUART_BITRATE) || defined(__DOXYGEN__)
#define DBGUART_BITRATE             38400
#endif

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Driver state machine possible states.
 */
typedef enum {
  DBGUART_UNINIT = 0,               /**< Not initialized.                   */
  DBGUART_STOP = 1,                 /**< Stopped.                           */
  DBGUART_READY = 2                 /**< Ready.                             */
} dbguartstate_t;

#include "dbguart_lld.h"

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void dbguartInit(void);
  void dbguartObjectInit(DBGUARTDriver *uartp);
  void dbguartSend(DBGUARTDriver *uartp, size_t n, const void *txbuf);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_DBGUART */

#endif /* _DBGUART_H_ */

/** @} */
