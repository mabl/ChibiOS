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
  chprintf((BaseSequentialStream *)&SD1, "BCM2835 RTC Demonstration\r\n");

  //uint8_t request[] = { 0x00, 0x30, 0x20, 0x08, 0x06, 0x06, 0x10, 0x12 };
  //i2cMasterTransmit(&I2C0, 0x68, request, 7, NULL, 0);

  for (;;) {

    RTCTime time;
    rtcGetTime(&RTCD1, &time);
    chprintf((BaseSequentialStream *)&SD1, "%.2d/%.2d/%.2d %.2d:%.2d:%.2d\r\n",
	     rtc_month(time), rtc_day(time), rtc_year(time),
	     rtc_hour(time), rtc_minute(time), rtc_second(time));

    chThdSleepMilliseconds(1000);
  }

  /*
   * Events servicing loop.
   */
  chThdWait(chThdSelf());

  return 0;
}
