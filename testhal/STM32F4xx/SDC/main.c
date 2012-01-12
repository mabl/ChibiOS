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


#define SDC_READONLY_TEST   TRUE

#define SDC_BURST_SIZE      4
static uint8_t blkbuf1[SDC_BLOCK_SIZE * SDC_BURST_SIZE + 1];
static uint8_t blkbuf2[SDC_BLOCK_SIZE * SDC_BURST_SIZE + 1];

/*
 * SDIO configuration.
 */
static const SDCConfig sdccfg = {
  0
};

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

  uint32_t i = 0;
  for (i=0; i < SDC_BLOCK_SIZE * SDC_BURST_SIZE; i++)
    blkbuf1[i] = 0x55;
  for (i=0; i < SDC_BLOCK_SIZE * SDC_BURST_SIZE; i++)
    blkbuf2[i] = 0x55;


  /*
   * Initializes the SDIO drivers.
   */
  sdcStart(&SDCD1, &sdccfg);
  if (!sdcConnect(&SDCD1)) {

    /* Single aligned read.*/

    /* Single unaligned read.*/

    /* Multiple aligned read.*/
    if (sdcRead(&SDCD1, 0, blkbuf1, SDC_BURST_SIZE))
      chSysHalt();

    /* Multiple unaligned read.*/

    /* Repeated multiple aligned reads.*/

    /* Repeated multiple unaligned reads.*/

    #if !SDC_READONLY_TEST

    for (i=0; i < SDC_BLOCK_SIZE * SDC_BURST_SIZE; i++)
      blkbuf2[i] = 0x55;

    for (i=0; i < 1000; i++){
      if (sdcWrite(&SDCD1, i, blkbuf2, SDC_BURST_SIZE))
        chSysHalt();
    }
    for (i=0; i < 1000; i++){
      if (sdcRead(&SDCD1, i, blkbuf1, SDC_BURST_SIZE))
        chSysHalt();
      if (memcmp(blkbuf1, blkbuf2, SDC_BURST_SIZE) != 0)
        chSysHalt();
    }

    /* Repeated multiple aligned writes.*/

    /* Repeated multiple unaligned writes.*/
    #endif /* !SDC_READONLY_TEST */

    if (sdcDisconnect(&SDCD1))
      chSysHalt();
  }

  /*
   * Normal main() thread activity.
   */
  while (TRUE) {
    palTogglePad(GPIOB, GPIOB_LED_R);
    chThdSleepMilliseconds(500);
  }
}
