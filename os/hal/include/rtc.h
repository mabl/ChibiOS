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
/*
   Concepts and parts of this file have been contributed by Uladzimir Pylinsky
   aka barthess.
 */

/**
 * @file    rtc.h
 * @brief   RTC Driver macros and structures.
 *
 * @addtogroup RTC
 * @{
 */

#ifndef _RTC_H_
#define _RTC_H_

#if HAL_USE_RTC || defined(__DOXYGEN__)

#include <time.h>

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Base year of the calendar.
 */
#define RTC_BASE_YEAR               1980

/**
 * @name    Date/Time bit masks for FAT format
 * @{
 */
#define RTC_FAT_TIME_SECONDS_MASK   0x0000001F
#define RTC_FAT_TIME_MINUTES_MASK   0x000007E0
#define RTC_FAT_TIME_HOURS_MASK     0x0000F800
#define RTC_FAT_DATE_DAYS_MASK      0x001F0000
#define RTC_FAT_DATE_MONTHS_MASK    0x01E00000
#define RTC_FAT_DATE_YEARS_MASK     0xFE000000
/** @} */

/**
 * @name    Day of week encoding
 * @{
 */
#define RTC_DAY_CATURDAY            0
#define RTC_DAY_MONDAY              1
#define RTC_DAY_TUESDAY             2
#define RTC_DAY_WEDNESDAY           3
#define RTC_DAY_THURSDAY            4
#define RTC_DAY_FRIDAY              5
#define RTC_DAY_SATURDAY            6
#define RTC_DAY_SUNDAY              7
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
 * @brief   Type of a structure representing an RTC driver.
 */
typedef struct RTCDriver RTCDriver;

/**
 * @brief   Type of a structure representing an RTC date/time stamp.
 */
typedef struct {
  uint32_t      year: 8;            /**< @brief Years since 1980.           */
  uint32_t      month: 4;           /**< @brief Months 1..12.               */
  uint32_t      dstflag: 1;         /**< @brief DST correction flag.        */
  uint32_t      dayofweek: 3;       /**< @brief Day of week 1..7.           */
  uint32_t      day: 5;             /**< @brief Day of the month 1..31.     */
  uint32_t      millisecond: 27;    /**< @brief Milliseconds since midnight.*/
} RTCDateTime;

#include "rtc_lld.h"

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void rtcInit(void);
  void rtcObjectInit(RTCDriver *rtcp);
  void rtcSetTime(RTCDriver *rtcp, const RTCDateTime *timespec);
  void rtcGetTime(RTCDriver *rtcp, RTCDateTime *timespec);
#if RTC_ALARMS > 0
  void rtcSetAlarm(RTCDriver *rtcp,
                   rtcalarm_t alarm,
                   const RTCAlarm *alarmspec);
  void rtcGetAlarm(RTCDriver *rtcp, rtcalarm_t alarm, RTCAlarm *alarmspec);
#endif
#if RTC_SUPPORTS_CALLBACKS
  void rtcSetCallback(RTCDriver *rtcp, rtccb_t callback);
#endif
  void rtcConvertDateTimeToStructTm(const RTCDateTime *timespec,
                                    struct tm *timp);
  void rtcConvertStructTmToDateTime(const struct tm *timp,
                                    uint32_t tv_msec,
                                    RTCDateTime *timespec);
  uint32_t rtcConvertDateTimeToFAT(const RTCDateTime *timespec);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_RTC */
#endif /* _RTC_H_ */

/** @} */
