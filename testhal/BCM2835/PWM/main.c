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
  chprintf((BaseSequentialStream *)&SD1, "BCM2835 PWM Demonstration\r\n");

  /*
   * Serial General Purpose Timer (GPT) #1 initialization.
   */
  PWMConfig pwmConfig;
  pwmConfig.period = 1024;

  pwmStart(&PWMD1, &pwmConfig);

  int bright;

  for (;;)
  {
    for (bright = 0 ; bright < 1024 ; ++bright)
    {
      pwmEnableChannel(&PWMD1, 0, bright);
      chThdSleepMilliseconds(1);
    }

    for (bright = 1023 ; bright >= 0 ; --bright)
    {
      pwmEnableChannel(&PWMD1, 0, bright);
      chThdSleepMilliseconds(1);
    }

    chprintf((BaseSequentialStream *)&SD1, "tick\r\n");
  }

  chThdWait(chThdSelf());

  return 0;
}
