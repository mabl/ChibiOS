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
 * @file    i2s.h
 * @brief   I2S Driver macros and structures.
 *
 * @addtogroup I2S
 * @{
 */

#ifndef _I2S_H_
#define _I2S_H_

#if HAL_USE_I2S || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    I2S modes
 * @{
 */
#define I2S_MODE_SLAVE          0
#define I2S_MODE_MASTER         1
/** @} */

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
  I2S_UNINIT = 0,                   /**< Not initialized.                   */
  I2S_STOP = 1,                     /**< Stopped.                           */
  I2S_READY = 2,                    /**< Ready.                             */
  I2S_ACTIVE = 3,                   /**< Active.                            */
  I2S_COMPLETE = 4                  /**< Transmission complete.             */
} i2sstate_t;

#include "i2s_lld.h"

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @name    Macro Functions
 * @{
 */
/**
 * @brief   Starts a I2S data exchange.
 *
 * @param[in] i2sp      pointer to the @p I2SDriver object
 *
 * @iclass
 */
#define i2sStartExchangeI(i2sp) {                                           \
  i2s_lld_start_exchange(i2sp);                                             \
  (i2sp)->state = I2S_ACTIVE;                                               \
}

/**
 * @brief   Stops the ongoing data exchange.
 * @details The ongoing data exchange, if any, is stopped, if the driver
 *          was not active the function does nothing.
 *
 * @param[in] i2sp      pointer to the @p I2SDriver object
 *
 * @iclass
 */
#define i2sStopExchangeI(i2sp) {                                            \
  i2s_lld_stop_exchange(i2sp);                                              \
  (i2sp)->state = I2S_READY;                                                \
}

/**
 * @brief   Common ISR code, half buffer event.
 * @details This code handles the portable part of the ISR code:
 *          - Callback invocation.
 *          .
 * @note    This macro is meant to be used in the low level drivers
 *          implementation only.
 *
 * @param[in] i2sp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
#define _i2s_isr_half_code(i2sp) {                                          \
  if ((i2sp)->config->end_cb != NULL) {                                     \
    (i2sp)->config->end_cb(i2sp, 0, (i2sp)->config->size / 2);              \
  }                                                                         \
}

/**
 * @brief   Common ISR code.
 * @details This code handles the portable part of the ISR code:
 *          - Callback invocation.
 *          - Driver state transitions.
 *          .
 * @note    This macro is meant to be used in the low level drivers
 *          implementation only.
 *
 * @param[in] i2sp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
#define _i2s_isr_full_code(i2sp) {                                               \
  if ((i2sp)->config->end_cb) {                                             \
    (i2sp)->state = I2S_COMPLETE;                                           \
    (i2sp)->config->end_cb(i2sp,                                            \
                           (i2sp)->config->size / 2,                        \
                           (i2sp)->config->size / 2);                       \
    if ((i2sp)->state == I2S_COMPLETE)                                      \
      (i2sp)->state = I2S_READY;                                            \
  }                                                                         \
  else                                                                      \
    (i2sp)->state = I2S_READY;                                              \
}
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void i2sInit(void);
  void i2sObjectInit(I2SDriver *i2sp);
  void i2sStart(I2SDriver *i2sp, const I2SConfig *config);
  void i2sStop(I2SDriver *i2sp);
  void i2sStartExchange(I2SDriver *i2sp);
  void i2sStopExchange(I2SDriver *i2sp);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_I2S */

#endif /* _I2S_H_ */

/** @} */
