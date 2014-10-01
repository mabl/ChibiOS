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
  chprintf((BaseSequentialStream *)&SD1, "BCM2835 GPIO Demonstration\r\n");

  ioportid_t ledPort = ONBOARD_LED_PORT;
  uint32_t ledPad = ONBOARD_LED_PAD;

  palSetPadMode(ledPort, ledPad, PAL_MODE_OUTPUT);
  palSetPad(ledPort, ledPad);

  palSetPadMode(GPIO4_PORT, GPIO4_PAD, PAL_MODE_INPUT_PULLUP);

  for (;;) {
    uint32_t button_state = palReadPad(GPIO4_PORT, GPIO4_PAD);
    if (button_state) {
      palSetPad(ledPort, ledPad);
    }
    else {
      palClearPad(ledPort, ledPad);
    }
  }

  /*
   * Events servicing loop.
   */
  chThdWait(chThdSelf());

  return 0;
}
