/*
    ChibiOS/RT - Copyright (C) 2006-2014 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "ch.h"
#include "hal.h"
#include "test.h"

/*
To summarize, user can use following set of options (command line) to achieve,
a. maximum code size optimization
--------------------------------------------------------------------------------------------------------
        -Os                        -fno-function-cse             -funit-at-a-time       -falign-jumps
        -fdata-sections        -ffunction-sections           –Wl,--gc-sections
        AND    < target specific options >
--------------------------------------------------------------------------------------------------------
b. maximum code speed optimization
--------------------------------------------------------------------------------------------------------
        -O3                         -fno-function-cse             -funit-at-a-time       -funroll-loops
        -fno-gcse                  AND    < target specific options >
--------------------------------------------------------------------------------------------------------
*/

/*
 * Blinker thread.
 */
static WORKING_AREA(waThread1, 128);
static msg_t Thread1(void *arg) {

  while (TRUE) {
    P7_bit.no7 = 0;
    chThdSleepMilliseconds(50);
    P7_bit.no7 = 1;
    chThdSleepMilliseconds(950);
  }
  return 0;
}

/*===========================================================================*/
/* Main and generic code.                                                    */
/*===========================================================================*/

void led_init(void) {

/*  uint32_t i;*/

  // led port init
  PM7_bit.no7 = 0;
  P7_bit.no7 = 0;
  chThdSleepMilliseconds(1000);
/*
  for (i=0; i<2000000; i++)
  {
    asm volatile("nop\n\t");
  }
*/
  P7_bit.no7 = 1;
  chThdSleepMilliseconds(1000);
/*
  for (i=0; i<2000000; i++)
  {
    asm volatile("nop\n\t");
  }
*/
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

  sdStart(&SD2, NULL);

  /*
   * Leds initialization.
   */
  led_init();

  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop.
   */
  TestThread(&SD2);
  while (TRUE) {
    chThdSleepMilliseconds(100);
  }
  return 0;
}
