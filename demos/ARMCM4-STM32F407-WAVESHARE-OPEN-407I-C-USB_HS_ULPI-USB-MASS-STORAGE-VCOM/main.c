/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013 Giovanni Di Sirio.

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

#include <stdio.h>
#include <string.h>

#include "ch.h"
#include "hal.h"
#include "test.h"

#include "usb_cdc.h"
#include "chprintf.h"
#include "shell.h"
#include "usb_msd.h"
#include "serial_usb.h"


/*Serial over USB Driver structure.*/
SerialUSBDriver SDU1;
SerialUSBConfig serusbcfg = { NULL };

USBMassStorageDriver UMSD1;


/*
 * CDC ACM Echo Thread
 */
static WORKING_AREA(waThread1, 1024);
static msg_t Thread1(void *arg) {
  (void)arg;
  chRegSetThreadName("blinker");

  BaseSequentialStream *usb_cdc = (BaseSequentialStream *) &SDU1;
  char c;

  while (TRUE) {
   if (chSequentialStreamRead(usb_cdc, (uint8_t *)&c, 1) != 0) {
        chprintf(usb_cdc, "You entered hex char 0x%X\r\n", c);
    }
  }
  return(0);
}

int init_sd(void) {
  BaseSequentialStream *chp = (BaseSequentialStream*)&SD2;

    // power cycle sd card
    palSetPad(GPIOC, GPIOC_SDIO_POWER);
    chThdSleepMilliseconds(1000);
    // this is probably longer than needed
    palClearPad(GPIOC, GPIOC_SDIO_POWER);
    chThdSleepMilliseconds(100);
    // let power stabilize

    // startup sdc driver
    sdcStart(&SDCD1, NULL);

    if (sdcConnect(&SDCD1) == CH_FAILED) {
        chprintf(chp, "sdcConnect FAILED\r\n");
        uint32_t errors = sdcGetAndClearErrors(&SDCD1);
        chprintf(chp, "error flags %d\r\n", errors);
        //FIXME: handle error
        return(1);
    } else {
        chprintf(chp, "sdcConnect SUCCEEDED\r\n");
    }

    return(0);
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
   * Activates the serial driver 2 and SDC driver 1 using default
   * configuration.
   */
  sdStart(&SD2, NULL);

  BaseSequentialStream *chp = (BaseSequentialStream *)&SD2;
  chprintf(chp, "running main()\r\n");
  chThdSleepMilliseconds(50);


#if STM32_USB_USE_OTG2
  USBDriver *usb_driver = &USBD2;
#else
  USBDriver *usb_driver = &USBD1;
#endif


  /*
   * Activates the card insertion monitor.
   */
  init_sd();
  chprintf(chp, "done starting SDC\r\n");
  sdcConnect(&SDCD1);

  chprintf(chp, "setting up MSD\r\n");
  msdInit(usb_driver, (BaseBlockDevice*) &SDCD1, &UMSD1);

  chprintf(chp, "Initializing SDU1...\r\n");
  serusbcfg.usbp = usb_driver;
  sduObjectInit(&SDU1);




  /*Disconnect the USB Bus*/
  usbDisconnectBus(usb_driver);
  chThdSleepMilliseconds(200);

  /*Start the useful functions*/
  sduStart(&SDU1, &serusbcfg);
  msdStart(usb_driver, &UMSD1);

  /*Connect the USB Bus*/
  usbConnectBus(usb_driver);

  /*
   * Creates the blinker thread.
   */
  chprintf(chp, "starting blinker thread\r\n");
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);


  while (TRUE) {
       palTogglePad(GPIOH, GPIOH_LED1);
       chThdSleepMilliseconds(500);
   }
}
