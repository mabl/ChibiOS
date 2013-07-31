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
 * @file    dbguart.c
 * @brief   DBGUART Driver code.
 *
 * @addtogroup DBGUART
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_DBGUART || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   UART Driver initialization.
 * @note    This function is implicitly invoked by @p halInit(), there is
 *          no need to explicitly initialize the driver.
 *
 * @init
 */
void dbguartInit(void) {

  dbguart_lld_init();
}

/**
 * @brief   Initializes the standard part of a @p UARTDriver structure.
 *
 * @param[out] uartp    pointer to the @p UARTDriver object
 *
 * @init
 */
void dbguartObjectInit(DBGUARTDriver *uartp) {

  chDbgCheck(uartp != NULL, "dbguartObjectInit");
  uartp->state  = DBGUART_STOP;
}

/**
 * @brief   Starts a transmission on the UART peripheral.
 * @note    The buffers are organized as uint8_t arrays for data sizes below
 *          or equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 * @param[in] n         number of data frames to send
 * @param[in] txbuf     the pointer to the transmit buffer
 *
 * @api
 */
void dbguartSend(DBGUARTDriver *uartp, size_t n, const void *txbuf) {

  if (uartp->state != DBGUART_READY){
    dbguart_lld_start(uartp);
    uartp->state = DBGUART_READY;
  }

  dbguart_lld_send(uartp, n, txbuf);
}

#endif /* HAL_USE_DBGUART */

/** @} */
