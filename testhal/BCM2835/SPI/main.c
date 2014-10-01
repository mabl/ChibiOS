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

SPIConfig spiConfig;

static void vt100_clearscreen(void) {
    chprintf((BaseSequentialStream *)&SD1, "%c[2J", 0x1b);
}

static void vt100_cursorhome(void) {
    chprintf((BaseSequentialStream *)&SD1, "%c[H", 0x1b);
}

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
  chprintf((BaseSequentialStream *)&SD1, "BCM2835 SPI Demonstration\r\n");

  /*
   * SPI startup.
   */
  spiConfig.chip_select = 0;;
  spiStart(&SPI0, &spiConfig);

  spiSelect(&SPI0);
  
  uint8_t txbuf[] = { 0x80, 0x00 };
  uint8_t rxbuf[] = { 0x00, 0x00 };

  for (;;) {
    spiExchange(&SPI0, 3, txbuf, rxbuf);
    vt100_clearscreen();
    vt100_cursorhome();
    chprintf((BaseSequentialStream *)&SD1, "%-5d %.2x %.2x ", 
	     ((rxbuf[0] & 0x03) << 8) | rxbuf[1], rxbuf[0] & 0x03, rxbuf[1]);
    chThdSleepMilliseconds(100);
  }

  /*
   * Events servicing loop.
   */
  chThdWait(chThdSelf());

  return 0;
}
