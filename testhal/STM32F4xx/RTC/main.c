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
#include <time.h>
#include <stdlib.h>

#include "ch.h"
#include "hal.h"

/*===========================================================================*/
/* Notes.                                                                    */
/*===========================================================================*/
/*
This structure is used to hold the values representing a calendar time.
It contains the following members, with the meanings as shown.

int tm_sec       seconds after minute [0-61] (61 allows for 2 leap-seconds)
int tm_min       minutes after hour [0-59] 
int tm_hour      hours after midnight [0-23] 
int tm_mday      day of the month [1-31] 
int tm_mon       month of year [0-11] 
int tm_year      current year-1900 
int tm_wday      days since Sunday [0-6] 
int tm_yday      days since January 1st [0-365] 
int tm_isdst     daylight savings indicator (1 = yes, 0 = no, -1 = unknown)
*/

RTCTime  timespec;
RTCAlarm alarmspec;
RTCWakeup wakeupspec;
time_t unix_time;

/**
 * Alarms callback
 */
static void exti_rtcalarm_cb(EXTDriver *extp, expchannel_t channel){
  (void)extp;
  (void)channel;
  if (RTCD1.id_rtc->ISR | RTC_ISR_ALRBF){
    RTCD1.id_rtc->ISR &= ~RTC_ISR_ALRBF;
  }
  if (RTCD1.id_rtc->ISR | RTC_ISR_ALRAF){
    RTCD1.id_rtc->ISR &= ~RTC_ISR_ALRAF;
  }
  palTogglePad(GPIOB, GPIOB_LED_R);
}

/**
 * Periodic wakeup callback
 */
static void exti_rtcwakeup_cb(EXTDriver *extp, expchannel_t channel){
  (void)extp;
  (void)channel;
  /* manually clear flags because exti driver does not do that */
  if (RTCD1.id_rtc->ISR | RTC_ISR_WUTF){
    RTCD1.id_rtc->ISR &= ~RTC_ISR_WUTF;
  }
  palTogglePad(GPIOB, GPIOB_LED_B);
  palTogglePad(GPIOB, GPIOB_LED_R);
}


static const EXTConfig extcfg = {
  {
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_RISING_EDGE | EXT_CH_MODE_AUTOSTART, exti_rtcalarm_cb},/* RTC alarms */
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},/* timestamp */
    {EXT_CH_MODE_RISING_EDGE| EXT_CH_MODE_AUTOSTART, exti_rtcwakeup_cb},/* wakeup */
  },
  EXT_MODE_EXTI(
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0)/* 15 */
};



/**
 * Main function.
 */
int main(void){
  struct tm timp;

  halInit();
  chSysInit();

  extStart(&EXTD1, &extcfg);

  /* tune wakeup callback */
  wakeupspec.wakeup = ((uint32_t)4) << 16; /* select 1 Hz clock source */
  wakeupspec.wakeup |= 3; /* set counter value to 3. Period will be 3+1 seconds. */
  rtcSetPeriodicWakeup_v2(&RTCD1, &wakeupspec);

  /* get current time in unix format */
  rtcGetTime(&RTCD1, &timespec);
  stm32_rtc_bcd2tm(&timp, &timespec);
  unix_time = mktime(&timp);

  if (unix_time == -1){/* incorrect time in RTC cell */
    unix_time = 1000000000;
  }
  /* set correct time */
  stm32_rtc_tm2bcd((localtime(&unix_time)), &timespec);
  rtcSetTime(&RTCD1, &timespec);

  while (TRUE){
    rtcGetTime(&RTCD1, &timespec);
    stm32_rtc_bcd2tm(&timp, &timespec);
    unix_time = mktime(&timp);
    chThdSleepMilliseconds(1500);
  }
  return 0;
}





