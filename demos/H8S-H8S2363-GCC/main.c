
#include <ch.h>
#include <hal.h>
#include <test.h>
#include "board.h"

/*
static const SerialConfig sd1_config =
{
  115200,
};
*/
static WORKING_AREA(waBlinker, 64);
static msg_t blinker_thread(void *arg) {

  while (TRUE) {
    palClearPad(IOPORT2, GPIO_LED);
    chThdSleepMilliseconds(100);
    palSetPad(IOPORT2, GPIO_LED);
    chThdSleepMilliseconds(900);
  }
  return 0;
}


/*
 * Entry point, note, the main() function is already a thread in the system
 * on entry.
 */
int main(int argc, char **argv) {

//  sdStart(&SD1, &sd1_config);

  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waBlinker, sizeof(waBlinker), NORMALPRIO, blinker_thread, NULL);

  /*
   * Normal main() thread activity.
   */
  while (TRUE) {
    chThdSleepMilliseconds(1000);
//    sdWrite(&SD1, (uint8_t *)"Hello World!\r\n", 14);
//    TestThread(&SD1);
  }

  return 0;
}
