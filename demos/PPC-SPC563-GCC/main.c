/*
    ChibiOS/RT - Copyright (C) 2006-2007 Giovanni Di Sirio.

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
/*#include "test.h"*/

int a = 1234;

/*
 * LEDs blinker thread, times are in milliseconds.
 */
static WORKING_AREA(waThread1, 128);
static msg_t Thread1(void *arg) {

  (void)arg;
  
  SIU.PCR[GPIO_LED1].R = 0x0300;
  SIU.PCR[GPIO_LED2].R = 0x0300;
  SIU.PCR[GPIO_LED3].R = 0x0300;
  SIU.PCR[GPIO_LED4].R = 0x0300;
  SIU.GPDO[GPIO_LED1].R = 1;
  SIU.GPDO[GPIO_LED2].R = 1;
  SIU.GPDO[GPIO_LED3].R = 1;
  SIU.GPDO[GPIO_LED4].R = 1;

  while (TRUE) {
    SIU.GPDO[GPIO_LED1].R = 0;
    chThdSleepMilliseconds(100);
    SIU.GPDO[GPIO_LED2].R = 0;
    chThdSleepMilliseconds(100);
    SIU.GPDO[GPIO_LED3].R = 0;
    chThdSleepMilliseconds(100);
    SIU.GPDO[GPIO_LED4].R = 0;
    chThdSleepMilliseconds(100);
    SIU.GPDO[GPIO_LED1].R = 1;
    chThdSleepMilliseconds(100);
    SIU.GPDO[GPIO_LED2].R = 1;
    chThdSleepMilliseconds(100);
    SIU.GPDO[GPIO_LED3].R = 1;
    chThdSleepMilliseconds(100);
    SIU.GPDO[GPIO_LED4].R = 1;
    chThdSleepMilliseconds(300);
  }
  return 0;
}

/*
 * Entry point, note, the main() function is already a thread in the system
 * on entry.
 */
int main(int argc, char **argv) {

  (void)argc;
  (void)argv;

  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  /*
   * Normal main() thread activity.
   */
  while (!chThdShouldTerminate())
    chThdSleepMilliseconds(1000);
  return 0;
}
