
#include <ch.h>
#include <test.h>
#include "board.h"
#include <coldfire_serial.h>

static WORKING_AREA(waThread1, 64);
static msg_t Thread1(void *arg) {

  while (TRUE) {
    LEDOn();
    chThdSleepMilliseconds(100);
    LEDOff();
    chThdSleepMilliseconds(900);
  }
  return 0;
}

/*
 * Entry point, note, the main() function is already a thread in the system
 * on entry.
 */
int main(int argc, char **argv) {

  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  /*
   * Normal main() thread activity.
   */
  while (TRUE) {
    chThdSleepMilliseconds(500);
    chFDDWrite(&COM1, (uint8_t *)"Hello World!\r\n", 14);
    TestThread(&COM1);

/*
    switch (chFDDGetTimeout(&COM1, 500))
    {
      case '1':
        chFDDWrite(&COM1, (uint8_t *)"Hello World!\r\n", 14);
        break;
      case '2':
        TestThread(&COM1);
        break;
    }
*/
  }
  return 0;
}
