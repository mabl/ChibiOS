/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,2011 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

#include "ch.h"
#include "hal.h"
#include "test.h"

/*
 * Green LED blinker thread, times are in milliseconds.
 * GPIOA,5 is the green LED on the Olimexino STM32.
 * GPIOA_GREEN_LED is defined as 5 in the board header.
 */
static WORKING_AREA(waGreenLED, 128);
static msg_t GreenLED(void *arg) {
  (void)arg;
  while (TRUE) {
    palClearPad(GPIOA, GPIOA_GREEN_LED);
    chThdSleepMilliseconds(500);
    palSetPad(GPIOA, GPIOA_GREEN_LED);
    chThdSleepMilliseconds(500);
  }
}

/*
 * Yellow LED blinker thread, times are in milliseconds.
 * GPIOA,1 is the yellow LED on the Olimexino STM32.
 * GPIOA_YELLOW_LED is defined as 1 in the board header.
 */
static WORKING_AREA(waYellowLED, 128);
static msg_t YellowLED(void *arg) {
  (void)arg;
  while (TRUE) {
    palClearPad(GPIOA, GPIOA_YELLOW_LED);
    chThdSleepMilliseconds(100);
    palSetPad(GPIOA, GPIOA_YELLOW_LED);
    chThdSleepMilliseconds(100);
  }
}

/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();
  
  /*
   * Creates the blinker threads.
   */
  chThdCreateStatic(waGreenLED, sizeof(waGreenLED), NORMALPRIO, GreenLED, NULL);
  chThdCreateStatic(waYellowLED, sizeof(waYellowLED), NORMALPRIO, YellowLED, NULL);

  /*
   * Normal main() thread activity, in this demo it does nothing.
   */
  while (TRUE) {
    chThdSleepMilliseconds(500);
  }
}