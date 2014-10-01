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
 * @file    rtc_lld.c
 * @brief   RTC low level driver for DS1307
 *
 * @addtogroup RTC
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_RTC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief RTC driver identifier.
 */
RTCDriver RTCD1;

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Initialize DS1307.
 *
 * @api
 */
void rtc_lld_init(void) {
  I2CConfig i2cConfig;
  i2cStart(&I2C0, &i2cConfig);
  RTCD1.i2c_bus = &I2C0;
}

/**
 * @brief   Set current time.
 * @note    Fractional part will be silently ignored. There is no possibility
 *          to set it on STM32 platform.
 *
 * @param[in] rtcp      pointer to RTC driver structure
 * @param[in] timespec  pointer to a @p RTCTime structure
 *
 * @api
 */
void rtc_lld_set_time(RTCDriver *rtcp, const RTCTime *timespec) {
  I2CDriver *i2cp = rtcp->i2c_bus;
  i2cAcquireBus(i2cp);
  i2cMasterTransmitTimeout(&I2C0, 0x68, timespec->registers, 7, NULL, 0, MS2ST(1000));
  i2cReleaseBus(i2cp);
}

/**
 * @brief   Get current time.
 *
 * @param[in] rtcp      pointer to RTC driver structure
 * @param[out] timespec pointer to a @p RTCTime structure
 *
 * @api
 */
void rtc_lld_get_time(RTCDriver *rtcp, RTCTime *timespec) {
  I2CDriver *i2cp = rtcp->i2c_bus;
  i2cAcquireBus(i2cp);
  uint8_t command = 0;
  i2cMasterTransmitTimeout(i2cp, 0x68, &command, 1, timespec->registers, 7, MS2ST(1000));
  i2cReleaseBus(i2cp);
}

/**
 * @brief     Set alarm time.
 *
 * @note      Default value after BKP domain reset for both comparators is 0.
 * @note      Function does not performs any checks of alarm time validity.
 *
 * @param[in] rtcp      Pointer to RTC driver structure.
 * @param[in] alarm     Alarm identifier. Can be 1 or 2.
 * @param[in] alarmspec Pointer to a @p RTCAlarm structure.
 *
 * @api
 */
void rtc_lld_set_alarm(RTCDriver *rtcp,
                       rtcalarm_t alarm,
                       const RTCAlarm *alarmspec) {
  UNUSED(rtcp);
  UNUSED(alarm);
  UNUSED(alarmspec);
  /* Not supported. No return status.*/
}

/**
 * @brief   Get alarm time.
 *
 * @param[in] rtcp       pointer to RTC driver structure
 * @param[in] alarm      alarm identifier
 * @param[out] alarmspec pointer to a @p RTCAlarm structure
 *
 * @api
 */
void rtc_lld_get_alarm(RTCDriver *rtcp,
                       rtcalarm_t alarm,
                       RTCAlarm *alarmspec) {
  UNUSED(rtcp);
  UNUSED(alarm);
  UNUSED(alarmspec);
  /* Not supported. */
}

/**
 * @brief   Get current time in format suitable for usage in FatFS.
 *
 * @param[in] rtcp      pointer to RTC driver structure
 * @return              FAT time value.
 *
 * @api
 */
uint32_t rtc_lld_get_time_fat(RTCDriver *rtcp) {
  uint32_t fattime;
  RTCTime timespec;

  chSysLock();
  rtcGetTimeI(rtcp, &timespec);
  chSysUnlock();

  fattime  = bcd2int(timespec.registers[0] & 0x7F);
  fattime |= bcd2int(timespec.registers[1]) << 5;
  fattime |= bcd2int(timespec.registers[2]) << 11;
  fattime |= bcd2int(timespec.registers[4]) << 16;
  fattime |= bcd2int(timespec.registers[5]) << 21;
  fattime |= (2000 + bcd2int(timespec.registers[6])) << 25;

  return fattime;
}

#endif /* HAL_USE_RTC */

/** @} */
