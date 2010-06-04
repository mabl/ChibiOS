
#include <string.h>
#include <ch.h>
#include <hal.h>
#include <test.h>
#include "board.h"

#include "web/webthread.h"

static WORKING_AREA(waWebThread, 1024);

static const SerialConfig sd1_config =
{
  .sc_speed = 115200,
  .sc_smr = 0,
};
/*
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
*/

/*
 * Entry point, note, the main() function is already a thread in the system
 * on entry.
 */
int main(int argc, char **argv) {

  int c;

  sdStart(&SD1, &sd1_config);

  /*
   * Creates the blinker thread.
   */
//  chThdCreateStatic(waBlinker, sizeof(waBlinker), NORMALPRIO, blinker_thread, NULL);

//  chThdCreateStatic(waWebThread, sizeof(waWebThread), LOWPRIO, WebThread, NULL);
  chThdCreateStatic(waWebThread, sizeof(waWebThread), NORMALPRIO, WebThread, NULL);

  /*
   * Normal main() thread activity.
   */
//  sdWrite(&SD1, (uint8_t *)"\r\nHello World!\r\n", 16);
  while (TRUE) {
    chThdSleepMilliseconds(1000);
    sdWrite(&SD1, (uint8_t *)"Hello World!\r\n", 14);
    TestThread(&SD1);
    while ( (c = sdGetTimeout(&SD1, MS2ST(1000))) != Q_TIMEOUT)
    {
      if (c == 0x1B) {
        chSysHalt();
        while (1);
      }
    }
  }

  return 0;
}
