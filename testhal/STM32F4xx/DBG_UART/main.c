/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

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

/*
 *
 */
void togglePanicLed(void){
  palTogglePad(GPIOB, GPIOB_LED_R);
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
   * Route PA2(TX) and PA3(RX) pins to USART2 (our dbg uart).
   */
  palSetPadMode(GPIOA, 2, PAL_MODE_ALTERNATE(7));
  palSetPadMode(GPIOA, 3, PAL_MODE_ALTERNATE(7));

  /*
  const char* msg = "test\r\n";
  port_disable();
  while (1) {
    dbguartSend(&DBGUARTD, 8, msg);
  }
  */

  /*
   * main goal of this hal test
   */
  uint32_t* test = NULL;
  chDbgCheck(NULL != test, "Null pointer forbidden");

  /*
   * Normal main() thread activity, in this demo it never reached.
   */
  while (TRUE) {
    chThdSleepMilliseconds(500);
  }
}
