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

#include <string.h>
#include <stdlib.h>

#include "ch.h"
#include "hal.h"

#include "shell.h"
#include "chprintf.h"

static RTCWakeup wakeupspec;
static RTCTime timespec;
static time_t unix_time;


/* sleep indicator thread */
static WORKING_AREA(blinkWA, 128);
static msg_t blink_thd(void *arg){
  (void)arg;
  while (TRUE) {
    chThdSleepMilliseconds(100);
    palTogglePad(GPIOB, GPIOB_LED_R);
  }
  return 0;
}

static void cmd_sleep(BaseChannel *chp, int argc, char *argv[]){
  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: sleep\r\n");
    return;
  }
  chprintf(chp, "Going to sleep.\r\n");

  chThdSleepMilliseconds(200);

  /* going to anabiosis */
  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
  PWR->CR |= (PWR_CR_PDDS | PWR_CR_LPDS | PWR_CR_CSBF | PWR_CR_CWUF);
  RTC->ISR &= ~(RTC_ISR_ALRBF | RTC_ISR_ALRAF | RTC_ISR_WUTF | RTC_ISR_TAMP1F |
                RTC_ISR_TSOVF | RTC_ISR_TSF);
  __WFI();
}

/*
 *
 */
static void cmd_alarm(BaseChannel *chp, int argc, char *argv[]){
  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: alarm A get\r\n");
    chprintf(chp, "       alarm B set 10\r\n");
    return;
  }
  chprintf(chp, "rebooting...\r\n");
}

/*
 *
 */
static void cmd_date(BaseChannel *chp, int argc, char *argv[]){
  (void)argv;
  struct tm timp;

  if (argc == 0) {
    goto ERROR;
  }

  if ((argc == 1) && (strcmp(argv[0], "get"))){
    rtcGetTime(&RTCD1, &timespec);
    stm32_rtc_bcd2tm(&timp, &timespec);
    unix_time = mktime(&timp);

    if (unix_time == -1){
      chprintf(chp, "incorrect time in RTC cell\r\n");
    }
    else{
      chprintf(chp, "%D%s","unix_time","\r\n");
    }
    return;
  }
  else{
    goto ERROR;
  }

  if ((argc == 2) && (strcmp(argv[0], "set") == 0)){
    unix_time = atol(argv[1]);
    if (unix_time > 0){
      stm32_rtc_tm2bcd((localtime(&unix_time)), &timespec);
      rtcSetTime(&RTCD1, &timespec);
      return;
    }
    else{
      goto ERROR;
    }
  }
  else{
    goto ERROR;
  }

ERROR:
  chprintf(chp, "Usage: date get\r\n");
  chprintf(chp, "       date set N\r\n");
  chprintf(chp, "       where N is time in seconds sins Unix epoch\r\n");
  return;
}


static const ShellCommand commands[] = {
  {"alarm", cmd_alarm},
  {"date",  cmd_date},
  {"sleep", cmd_sleep},
  {NULL, NULL}
};

static const ShellConfig shell_cfg1 = {
  (BaseChannel *)&SD2,
  commands
};


/**
 * Main function.
 */
int main(void){

  halInit();
  chSysInit();

  chThdCreateStatic(blinkWA, sizeof(blinkWA), NORMALPRIO, blink_thd, NULL);

  /* Shell manager initialization.*/
  shellInit();
  static WORKING_AREA(waShell, 1024);
  shellCreateStatic(&shell_cfg1, waShell, sizeof(waShell), NORMALPRIO);

  /* tune wakeup */
  wakeupspec.wakeup = ((uint32_t)4) << 16; /* select 1 Hz clock source */
  wakeupspec.wakeup |= 9; /* set counter value to 9. Period will be 9+1 seconds. */
  rtcSetPeriodicWakeup_v2(&RTCD1, &wakeupspec);

  while (TRUE){
    chThdSleepMilliseconds(666);
  }
  return 0;
}

