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
 * @file    icu.h
 * @brief   ICU Driver macros and structures.
 *
 * @addtogroup ICU
 * @{
 */

#ifndef _ICU_H_
#define _ICU_H_

#if HAL_USE_ICU || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

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
  ICU_UNINIT = 0,                   /**< Not initialized.                   */
  ICU_STOP = 1,                     /**< Stopped.                           */
  ICU_READY = 2,                    /**< Ready.                             */
  ICU_WAITING = 3,                  /**< Waiting for first front.           */
  ICU_ACTIVE = 4,                   /**< First front detected.              */
} icustate_t;

/**
 * @brief   Type of a structure representing an ICU driver.
 */
typedef struct ICUDriver ICUDriver;

/**
 * @brief   ICU notification callback type.
 *
 * @param[in] icup      pointer to a @p ICUDriver object
 */
typedef void (*icucallback_t)(ICUDriver *icup);

#include "icu_lld.h"

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @name    Macro Functions
 * @{
 */
/**
 * @brief   Starts the input capture.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 *
 * @iclass
 */
#define icuStartCaptureI(icup) do {                                         \
  icu_lld_start_capture(icup);                                              \
  icup->state = ICU_WAITING;                                                \
} while (0)

/**
 * @brief   Stops the input capture.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 *
 * @iclass
 */
#define icuStopCaptureI(icup) do {                                          \
  icu_lld_stop_capture(icup);                                               \
  icup->state = ICU_READY;                                                  \
} while (0)

/**
 * @brief   Enables notifications.
 * @pre     The ICU unit must have been activated using @p icuStart().
 * @note    If the notification is already enabled then the call has no effect.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 *
 * @iclass
 */
#define icuEnableNotificationsI(icup) icu_lld_enable_notifications(icup)

/**
 * @brief   Disables notifications.
 * @pre     The ICU unit must have been activated using @p icuStart().
 * @note    If the notification is already disabled then the call has no effect.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 *
 * @iclass
 */
#define icuDisableNotificationsI(icup) icu_lld_disable_notifications(icup)

/**
 * @brief   Check on notifications status.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 * @return              The notifications status.
 * @retval false        if notifications are not enabled.
 * @retval true         if notifications are enabled.
 *
 * @notapi
 */
#define icuAreNotificationsEnabledX(icup)                                   \
  icu_lld_are_notifications_enabled(icup)

/**
 * @brief   Returns the width of the latest pulse.
 * @details The pulse width is defined as number of ticks between the start
 *          edge and the stop edge.
 * @note    This function is meant to be invoked from the width capture
 *          callback.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 * @return              The number of ticks.
 *
 * @xclass
 */
#define icuGetWidthX(icup) icu_lld_get_width(icup)

/**
 * @brief   Returns the width of the latest cycle.
 * @details The cycle width is defined as number of ticks between a start
 *          edge and the next start edge.
 * @note    This function is meant to be invoked from the width capture
 *          callback.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 * @return              The number of ticks.
 *
 * @xclass
 */
#define icuGetPeriodX(icup) icu_lld_get_period(icup)
/** @} */

/**
 * @name    Low level driver helper macros
 * @{
 */
/**
 * @brief   Common ISR code, ICU width event.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 *
 * @notapi
 */
#define _icu_isr_invoke_width_cb(icup) do {                                 \
  if (((icup)->state == ICU_ACTIVE) &&                                      \
      ((icup)->config->width_cb != NULL))                                   \
    (icup)->config->width_cb(icup);                                         \
} while (0)

/**
 * @brief   Common ISR code, ICU period event.
 * @note    A period event brings the driver into the @p ICU_ACTIVE state.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 *
 * @notapi
 */
#define _icu_isr_invoke_period_cb(icup) do {                                \
  if (((icup)->state == ICU_ACTIVE) &&                                      \
      ((icup)->config->period_cb != NULL))                                  \
    (icup)->config->period_cb(icup);                                        \
  (icup)->state = ICU_ACTIVE;                                               \
} while (0)

/**
 * @brief   Common ISR code, ICU timer overflow event.
 * @note    An overflow always brings the driver back to the @p ICU_WAITING
 *          state.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 *
 * @notapi
 */
#define _icu_isr_invoke_overflow_cb(icup) do {                              \
  (icup)->config->overflow_cb(icup);                                        \
  (icup)->state = ICU_WAITING;                                              \
} while (0)
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void icuInit(void);
  void icuObjectInit(ICUDriver *icup);
  void icuStart(ICUDriver *icup, const ICUConfig *config);
  void icuStop(ICUDriver *icup);
  void icuStartCapture(ICUDriver *icup);
  bool icuWaitCapture(ICUDriver *icup);
  void icuStopCapture(ICUDriver *icup);
  void icuEnableNotifications(ICUDriver *icup);
  void icuDisableNotifications(ICUDriver *icup);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_ICU */

#endif /* _ICU_H_ */

/** @} */
