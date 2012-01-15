/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011 Giovanni Di Sirio.

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

#include <string.h>
#include "ch.h"
#include "hal.h"
#include "storm_sdc.h"
#include "storm_i2c.h"

/*
 * Application entry point.
 */
int main(void) {
  halInit();
  chSysInit();

//  storm_sdc_init();
//  storm_i2c_init();
  storm_uart_init();

  /*
   * Normal main() thread activity.
   */
  while (TRUE) {
    palTogglePad(GPIOB, GPIOB_LED_R);
    chThdSleepMilliseconds(100);
  }
}
