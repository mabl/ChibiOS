


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

static uint8_t rxbuf1[BUFFER_SIZE];
static uint8_t referencebuf1[BUFFER_SIZE];
static uint8_t txbuf1[BUFFER_SIZE];
static BinarySemaphore uart1_tx_sem;
static BinarySemaphore uart1_rx_sem;

static uint8_t rxbuf2[BUFFER_SIZE];
static uint8_t referencebuf2[BUFFER_SIZE];
static uint8_t txbuf2[BUFFER_SIZE];
static BinarySemaphore uart2_tx_sem;
static BinarySemaphore uart2_rx_sem;


/*
 * This callback is invoked when a transmission has physically completed.
 */
static void txend1(UARTDriver *uartp) {

  (void)uartp;
  chSysLockFromIsr();
  dmaStreamClearInterrupt(uartp->dmatx);
  chBSemSignalI(&uart1_tx_sem);
  chSysUnlockFromIsr();
}

/*
 * This callback is invoked when a transmission has physically completed.
 */
static void txend2(UARTDriver *uartp) {

  (void)uartp;
  chSysLockFromIsr();
  dmaStreamClearInterrupt(uartp->dmatx);
  chBSemSignalI(&uart2_tx_sem);
  chSysUnlockFromIsr();
}

/*
 * This callback is invoked on a receive error, the errors mask is passed
 * as parameter.
 */
static void rxerr1(UARTDriver *uartp, uartflags_t e) {

  (void)uartp;
  (void)e;

  chSysHalt();
}

/*
 * This callback is invoked on a receive error, the errors mask is passed
 * as parameter.
 */
static void rxerr2(UARTDriver *uartp, uartflags_t e) {

  (void)uartp;
  (void)e;

  chSysHalt();
}

/*
 * This callback is invoked when a receive buffer has been completely written.
 */
static void rxend1(UARTDriver *uartp) {

  (void)uartp;
  chSysLockFromIsr();
  dmaStreamClearInterrupt(uartp->dmarx);
  chBSemSignalI(&uart1_rx_sem);
  chSysUnlockFromIsr();
}

/*
 * This callback is invoked when a receive buffer has been completely written.
 */
static void rxend2(UARTDriver *uartp) {

  (void)uartp;
  chSysLockFromIsr();
  dmaStreamClearInterrupt(uartp->dmarx);
  chBSemSignalI(&uart2_rx_sem);
  chSysUnlockFromIsr();
}

/*
 * UART driver configuration structure.
 */
static UARTConfig uart_cfg_1 = {
  NULL,
  txend1,
  rxend1,
  NULL,
  rxerr1,
  115200,
  0,
  0,
  0
};

/*
 * UART driver configuration structure.
 */
static UARTConfig uart_cfg_2 = {
  NULL,
  txend2,
  rxend2,
  NULL,
  rxerr2,
  115200,
  0,
  0,
  0
};

static WORKING_AREA(StormUartRx1ThreadWA, 1024);
static msg_t storm_rx_uart1(void *arg) {
  chRegSetThreadName("uart2_rx");
  (void)arg;

    /* infinite read and compare with previously read data */
    while (TRUE){
      chBSemWait(&uart1_rx_sem);
      chSysLock();
      uartStartReceiveI(&UARTD2, BUFFER_SIZE, rxbuf2);
      chSysUnlock();
    }
  return 0;
}

static WORKING_AREA(StormUartRx2ThreadWA, 1024);
static msg_t storm_rx_uart2(void *arg) {
  chRegSetThreadName("uart2_rx");
  (void)arg;

    /* infinite read and compare with previously read data */
    while (TRUE){
      chBSemWait(&uart2_rx_sem);
      chSysLock();
      uartStartReceiveI(&UARTD2, BUFFER_SIZE, rxbuf2);
      chSysUnlock();
    }
  return 0;
}


static WORKING_AREA(StormUart1ThreadWA, 256);
static msg_t storm_uart1(void *arg) {
  chRegSetThreadName("uart1");
  (void)arg;
  msg_t status = RDY_OK;

    /* infinite read and compare with previously read data */
    while (TRUE){
      status = chBSemWaitTimeout(&uart1_tx_sem, MS2ST(200));
      if (status != RDY_OK)
        chSysHalt();
      uartStartSend(&UARTD1, BUFFER_SIZE, txbuf1);
    }
  return 0;
}

static WORKING_AREA(StormUart2ThreadWA, 256);
static msg_t storm_uart2(void *arg) {
  chRegSetThreadName("uart1");
  (void)arg;
  msg_t status = RDY_OK;

    /* infinite read and compare with previously read data */
    while (TRUE){
      status = chBSemWaitTimeout(&uart2_tx_sem, MS2ST(200));
      if (status != RDY_OK)
        chSysHalt();
      uartStartSend(&UARTD2, BUFFER_SIZE, txbuf2);
    }
  return 0;
}

void storm_uart_init(void){
  uint32_t i = 0;

  chBSemInit(&uart1_tx_sem,  TRUE);
  chBSemInit(&uart2_tx_sem,  TRUE);

  chBSemInit(&uart1_rx_sem,  TRUE);
  chBSemInit(&uart2_rx_sem,  TRUE);

  uartStart(&UARTD1, &uart_cfg_1);
  uartStart(&UARTD2, &uart_cfg_2);

  /* fill tx buffers with pattern */
  while (i < BUFFER_SIZE){
    txbuf1[i] = i;
    txbuf2[i] = i;
    referencebuf1[i] = 0x55;
    referencebuf2[i] = 0x55;
    i++;
  }

  chThdCreateStatic(StormUart1ThreadWA,
                    sizeof(StormUart1ThreadWA),
                    NORMALPRIO,
                    storm_uart1,
                    NULL);

  chThdCreateStatic(StormUart2ThreadWA,
                    sizeof(StormUart2ThreadWA),
                    NORMALPRIO,
                    storm_uart2,
                    NULL);

  chThdCreateStatic(StormUartRx1ThreadWA,
                    sizeof(StormUartRx1ThreadWA),
                    NORMALPRIO,
                    storm_rx_uart1,
                    NULL);

  chThdCreateStatic(StormUartRx2ThreadWA,
                    sizeof(StormUartRx2ThreadWA),
                    NORMALPRIO,
                    storm_rx_uart2,
                    NULL);

  uartStartReceive(&UARTD1, BUFFER_SIZE, rxbuf1);
  uartStartReceive(&UARTD2, BUFFER_SIZE, rxbuf2);
}


