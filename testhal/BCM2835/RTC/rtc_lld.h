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
 * @file    rtc_lld.h
 * @brief   DS1307 RTC low level driver header.
 *
 * @addtogroup RTC
 * @{
 */

#ifndef _RTC_LLD_H_
#define _RTC_LLD_H_

#if HAL_USE_RTC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Two alarm comparators available on STM32F4x.
 */
#define RTC_ALARMS                  2

/**
 * @brief   Data offsets in RTC date and time registers.
 */
#define RTC_TR_PM_OFFSET    22
#define RTC_TR_HT_OFFSET    20
#define RTC_TR_HU_OFFSET    16
#define RTC_TR_MNT_OFFSET   12
#define RTC_TR_MNU_OFFSET   8
#define RTC_TR_ST_OFFSET    4
#define RTC_TR_SU_OFFSET    0

#define RTC_DR_YT_OFFSET    20
#define RTC_DR_YU_OFFSET    16
#define RTC_DR_WDU_OFFSET   13
#define RTC_DR_MT_OFFSET    12
#define RTC_DR_MU_OFFSET    8
#define RTC_DR_DT_OFFSET    4
#define RTC_DR_DU_OFFSET    0

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
  /* @brief DS1307 registers */
  uint8_t registers[7];
};

/**
 * @brief   Structure representing an RTC alarm time stamp.
 */
struct RTCAlarm {
};

/**
 * @brief   Structure representing an RTC periodic wakeup period.
 */
struct RTCWakeup {
#if 0
  /**
   * @brief   RTC WUTR register.
   * @details Bits [15:0] contain value of WUTR register
   *          Bits [18:16] contain value of WUCKSEL bits in CR register
   *
   * @notes   ((WUTR == 0) || (WUCKSEL == 3)) is forbidden combination.
   */
  uint32_t wakeup;
#endif
};

/**
 * @brief   Structure representing an RTC driver.
 */
struct RTCDriver{
  I2CDriver                   *i2c_bus;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

#define bcd2int(x) ((((x) >> 4) * 10) + ((x) & 0x0F))

#define rtc_second(t)   bcd2int((t).registers[0])
#define rtc_minute(t)   bcd2int((t).registers[1])
#define rtc_hour(t)     bcd2int((t).registers[2])
#define rtc_dayofweek(t) ((t).registers[3])
#define rtc_day(t)       bcd2int((t).registers[4])
#define rtc_month(t)     bcd2int((t).registers[5])
#define rtc_year(t)      (2000 + bcd2int((t).registers[6]))

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
  uint32_t rtc_lld_get_time_fat(RTCDriver *rtcp);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_RTC */

#endif /* _RTC_LLD_H_ */

/** @} */
