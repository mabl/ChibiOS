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

#include "ch.h"
#include "hal.h"

static RTCWakeup wakeupspec;

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

/**
 * Main function.
 */
int main(void){

  halInit();
  chSysInit();

  chThdCreateStatic(blinkWA, sizeof(blinkWA), NORMALPRIO, blink_thd, NULL);

  /* tune wakeup */
  wakeupspec.wakeup = ((uint32_t)4) << 16; /* select 1 Hz clock source */
  wakeupspec.wakeup |= 9; /* set counter value to 9. Period will be 9+1 seconds. */
  rtcSetPeriodicWakeup_v2(&RTCD1, &wakeupspec);

  while (TRUE){
    chThdSleepSeconds(3);
    chSysLock();

    /* going to anabiosis */
//    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
//    PWR->CR |= (PWR_CR_PDDS | PWR_CR_LPDS | PWR_CR_CSBF | PWR_CR_CWUF);
//    RTC->ISR &= ~(RTC_ISR_ALRBF | RTC_ISR_ALRAF | RTC_ISR_WUTF | RTC_ISR_TAMP1F |
//                  RTC_ISR_TSOVF | RTC_ISR_TSF);
//    __WFI();

    chSysUnlock();
  }
  return 0;
}

