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
#include "chprintf.h"

static MUTEX_DECL(mutex1);
static CONDVAR_DECL(cond1);

static uint32_t callback_count_1;

static void gptcallback1(GPTDriver *gptp) {
  UNUSED(gptp);
  chSysLockFromIsr();
  callback_count_1++;
  if (gptp->state == GPT_ONESHOT) {
    chCondSignalI(&cond1);
  }
  chSysUnlockFromIsr();
}

static uint32_t callback_count_2;

static void gptcallback2(GPTDriver *gptp) {
  UNUSED(gptp);
  chSysLockFromIsr();
  callback_count_2++;
  chSysUnlockFromIsr();
}

/*
 * Application entry point.
 */
int main(void) {
  halInit();
  chSysInit();

  /*
   * Serial port initialization.
   */
  sdStart(&SD1, NULL); 
  chprintf((BaseSequentialStream *)&SD1, "BCM2835 GPT Demonstration\r\n");

  /*
   * Serial General Purpose Timer (GPT) #1 initialization.
   */
  GPTConfig gptConfig1;
  gptConfig1.callback = gptcallback1;
  gptStart(&GPTD1, &gptConfig1);

  /*
   * Serial General Purpose Timer (GPT) #2 initialization.
   */
  GPTConfig gptConfig2;
  gptConfig2.callback = gptcallback2;
  gptStart(&GPTD2, &gptConfig2);

  chprintf((BaseSequentialStream *)&SD1, "Polled timer started: %d\r\n", SYSTIMER_CLO);
  gptPolledDelay(&GPTD1,  500000);
  chprintf((BaseSequentialStream *)&SD1, "    Fired: %d\r\n", SYSTIMER_CLO);

  chprintf((BaseSequentialStream *)&SD1, "Starting one shot timer (3 seconds)\r\n");

  chCondInit(&cond1);
  chMtxInit(&mutex1);

  gptStartOneShot(&GPTD1, 3000000);

  chMtxLock(&mutex1);
  chCondWait(&cond1);
  chMtxUnlock();

  chprintf((BaseSequentialStream *)&SD1, 
	   "Starting continuous timer 1 (2 second period)\r\n");

  gptStartContinuous(&GPTD1, 2000000);

  chprintf((BaseSequentialStream *)&SD1, 
	   "Starting continuous timer 2 (1 second period)\r\n");

  gptStartContinuous(&GPTD2, 1000000);

  for (;;) {
    chprintf((BaseSequentialStream *)&SD1, "c1=%d, c2=%d\r\n", callback_count_1 ,callback_count_2);
    chThdSleepMilliseconds(1000);
  }

  return 0;
}
