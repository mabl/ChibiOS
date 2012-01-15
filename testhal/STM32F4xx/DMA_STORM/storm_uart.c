


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

#include "ch.h"
#include "hal.h"

#define BUFFER_SIZE   256 /* bytes*/

static uint8_t rxbuf[BUFFER_SIZE];
static uint8_t referencebuf[BUFFER_SIZE];
static uint8_t txbuf[BUFFER_SIZE];

/*
 * This callback is invoked when a transmission buffer has been completely
 * read by the driver.
 */
static void txend1(UARTDriver *uartp) {

  (void)uartp;

}

/*
 * This callback is invoked when a transmission has physically completed.
 */
static void txend2(UARTDriver *uartp) {

  (void)uartp;

}

/*
 * This callback is invoked on a receive error, the errors mask is passed
 * as parameter.
 */
static void rxerr(UARTDriver *uartp, uartflags_t e) {

  (void)uartp;
  (void)e;

  chSysHalt();
}

/*
 * This callback is invoked when a receive buffer has been completely written.
 */
static void rxend(UARTDriver *uartp) {

  (void)uartp;
}

/*
 * UART driver configuration structure.
 */
static UARTConfig uart_cfg_1 = {
  txend1,
  txend2,
  rxend,
  NULL,
  rxerr,
  115200,
  0,
  USART_CR2_LINEN,
  0
};




static WORKING_AREA(StormUartThreadWA, 1024);
static msg_t storm_uart1(void *arg) {
  chRegSetThreadName("uart1");
  (void)arg;

  if (!sdcConnect(&SDCD1)) {
    clearbuffers();
    if (sdcRead(&SDCD1, 0, inbuf, SDC_BURST_SIZE))
      chSysHalt();

    /* infinite read and compare with previously read data */
    while (TRUE){
      if (sdcRead(&SDCD1, 0, outbuf, SDC_BURST_SIZE))
        chSysHalt();
      if (memcmp(inbuf, outbuf, SDC_BURST_SIZE * SDC_BLOCK_SIZE) != 0)
        chSysHalt();
    }
  }
  return 0;
}


void storm_uart_init(void){
  uint32_t i = 0;
  uint32_t n = 0;

  uartStart(&UARTD2, &uart_cfg_1);

  while (i < EEPROM_BURST_SIZE){
    txbuf[0] = 0;
    txbuf[1] = i;
    for (n=0; n<EEPROM_BLOCK_SIZE; n++)
      txbuf[n+2] = i;
    i2cMasterTransmit(&I2CD2, addr, txbuf, (EEPROM_BLOCK_SIZE + 2), rxbuf, 0);
    chThdSleepMilliseconds(5);
    i++;
  }

  chThdCreateStatic(StormUartThreadWA,
          sizeof(StormUartThreadWA),
          NORMALPRIO,
          storm_uart,
          NULL);
}


