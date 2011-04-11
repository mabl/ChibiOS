/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

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

extern const void *HardwareVectors[];
/*
 * Red LED blinker thread, times are in milliseconds.
 */
WORKING_AREA(waThread1, 128);
WORKING_AREA(waThread2, 128);
WORKING_AREA(waThread3, 128);

long cntThread1;
long cntThread2;
long cntThread3;

msg_t Thread1(void *arg) {

  (void)arg;
  while (TRUE) {
    
    chThdSleepMilliseconds(500);

	cntThread1++;
    
    chThdSleepMilliseconds(500);
  }
}


static msg_t Thread2(void *arg) {

  (void)arg;
  while (TRUE) {
    
    chThdSleepMilliseconds(100);

	cntThread2++;
    
    chThdSleepMilliseconds(100);
  }
}


static msg_t Thread3(void *arg) {

  (void)arg;
  while (TRUE) {
    
    chThdSleepMilliseconds(200);

	cntThread3++;
    
    chThdSleepMilliseconds(200);
  }
}


/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  chSysInit();

  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO+1, Thread1, NULL);
  chThdCreateStatic(waThread2, sizeof(waThread2), NORMALPRIO+2, Thread2, NULL);
  chThdCreateStatic(waThread3, sizeof(waThread3), NORMALPRIO+3, Thread3, NULL);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and check the button state.
   */
   
   char *p = (char *) HardwareVectors;
   char a;
   
  while (TRUE) {
    chThdSleepMilliseconds(500);
	
	a = *p++;	
  }
}
