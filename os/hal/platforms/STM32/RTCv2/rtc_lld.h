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
/*
   Concepts and parts of this file have been contributed by Uladzimir Pylinsky
   aka barthess.
 */

/**
 * @file    STM32/RTCv2/rtc_lld.h
 * @brief   STM32L1xx/STM32F2xx/STM32F4xx RTC low level driver header.
 *
 * @addtogroup RTC
 * @{
 */

#ifndef _RTC_LLD_H_
#define _RTC_LLD_H_

#if HAL_USE_RTC || defined(__DOXYGEN__)

#include <time.h>

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Two alarm comparators available on STM32F4x.
 */
#define RTC_ALARMS                  2

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if HAL_USE_RTC && !STM32_HAS_RTC
#error "RTC not present in the selected device"
#endif

#if !(STM32_RTCSEL == STM32_RTCSEL_LSE) &&                                  \
    !(STM32_RTCSEL == STM32_RTCSEL_LSI) &&                                  \
    !(STM32_RTCSEL == STM32_RTCSEL_HSEDIV)
#error "invalid source selected for RTC clock"
#endif

#if !defined(RTC_USE_INTERRUPTS) || defined(__DOXYGEN__)
#define RTC_USE_INTERRUPTS                FALSE
#endif

#if RTC_USE_INTERRUPTS && !(HAL_USE_EXT)
#error "interrupts from RTC works only through EXTI on this platform"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a structure representing an RTC alarm time stamp.
 */
typedef struct RTCAlarm RTCAlarm;

/**
 * @brief   Type of a structure representing an RTC wakeup period.
 */
typedef struct RTCWakeup RTCWakeup;

/**
 * @brief   Type of a structure representing an RTC callbacks config.
 */
typedef struct RTCCallbackConfig RTCCallbackConfig;

/**
 * @brief   Type of an RTC alarm.
 * @details Meaningful on platforms with more than 1 alarm comparator.
 */
typedef uint32_t rtcalarm_t;

/**
 * @brief   Structure representing an RTC time stamp.
 */
struct RTCTime {
  /**
   * @brief RTC date register in STM32 BCD format.
   */
  uint32_t tv_date;
  /**
   * @brief RTC time register in STM32 BCD format.
   */
  uint32_t tv_time;
  /**
   * @brief Fractional part of time.
   */
#if STM32_RTC_HAS_SUBSECONDS
  uint32_t tv_msec;
#endif
};

/**
 * @brief   Structure representing an RTC alarm time stamp.
 */
struct RTCAlarm {
  /**
   * @brief Date and time of alarm in STM32 BCD.
   */
  uint32_t tv_datetime;
};

/**
 * @brief   Structure representing an RTC periodic wakeup period.
 */
struct RTCWakeup {
  /**
   * @brief   RTC WUTR register.
   * @details Bits [15:0] contain value of WUTR register
   *          Bits [18:16] contain value of WUCKSEL bits in CR register
   *
   * @notes   ((WUTR == 0) || (WUCKSEL == 3)) is forbidden combination.
   */
  uint32_t wakeup;
};

/**
 * @brief   Structure representing an RTC driver.
 */
struct RTCDriver{
  /**
   * @brief Pointer to the RTC registers block.
   */
  RTC_TypeDef               *id_rtc;
  /**
   * @brief Current callback confuguration.
   */
  const RTCCallbackConfig   *cb_cfg;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(__DOXYGEN__)
extern RTCDriver RTCD1;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void rtc_lld_init(void);
  void rtc_lld_set_time(RTCDriver *rtcp, const RTCTime *timespec);
  void rtc_lld_get_time(RTCDriver *rtcp, RTCTime *timespec);
  void rtc_lld_set_alarm(RTCDriver *rtcp,
                         rtcalarm_t alarm,
                         const RTCAlarm *alarmspec);
  void rtc_lld_get_alarm(RTCDriver *rtcp,
                         rtcalarm_t alarm,
                         RTCAlarm *alarmspec);
  void rtcSetPeriodicWakeup_v2(RTCDriver *rtcp, RTCWakeup *wakeupspec);
  void rtcGetPeriodicWakeup_v2(RTCDriver *rtcp, RTCWakeup *wakeupspec);
  void stm32_rtc_bcd2tm(struct tm *timp, RTCTime *timespec);
  void stm32_rtc_tm2bcd(struct tm *timp, RTCTime *timespec);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_RTC */

#endif /* _RTC_LLD_H_ */

/** @} */
