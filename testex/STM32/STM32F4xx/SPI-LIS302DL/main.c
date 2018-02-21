/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "ch.h"
#include "hal.h"

#include "usbcfg.h"
#include "string.h"
#include "shell.h"
#include "chprintf.h"

#include "lis302dl.h"

/*===========================================================================*/
/* LIS302DL related.                                                         */
/*===========================================================================*/

/* LIS302DL Driver: This object represent an LIS302DL instance */
static LIS302DLDriver LIS302DLD1;

static int32_t rawdata[LIS302DL_NUMBER_OF_AXES];
static float cookeddata[LIS302DL_NUMBER_OF_AXES];

static char axisID[LIS302DL_NUMBER_OF_AXES] = {'X', 'Y', 'Z'};
static uint32_t i;

static const SPIConfig spicfg = {
  FALSE,
  NULL,
  GPIOE,
  GPIOE_CS_SPI,
  SPI_CR1_BR_0 | SPI_CR1_CPOL | SPI_CR1_CPHA,
  0
};

static LIS302DLConfig l3gd20cfg = {
  &SPID1,
  &spicfg,
  NULL,
  NULL,
  LIS302DL_FS_2G,
  LIS302DL_ODR_100HZ,
#if LIS302DL_USE_ADVANCED || defined(__DOXYGEN__)
  LIS302DL_HP_DISABLED,
#endif
};

/*===========================================================================*/
/* Command line related.                                                     */
/*===========================================================================*/

/* Enable use of special ANSI escape sequences.*/
#define CHPRINTF_USE_ANSI_CODE      TRUE
#define SHELL_WA_SIZE               THD_WORKING_AREA_SIZE(2048)

static void cmd_read(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)argv;
  if (argc != 1) {
    chprintf(chp, "Usage: read [raw|cooked]\r\n");
    return;
  }

  while (chnGetTimeout((BaseChannel *)chp, 150) == Q_TIMEOUT) {
    if (!strcmp (argv[0], "raw")) {
#if CHPRINTF_USE_ANSI_CODE
      chprintf(chp, "\033[2J\033[1;1H");
#endif
      accelerometerReadRaw(&LIS302DLD1, rawdata);
      chprintf(chp, "LIS302DL Accelerometer raw data...\r\n");
      for(i = 0; i < LIS302DL_NUMBER_OF_AXES; i++) {
        chprintf(chp, "%c-axis: %d\r\n", axisID[i], rawdata[i]);
      }
    }
    else if (!strcmp (argv[0], "cooked")) {
#if CHPRINTF_USE_ANSI_CODE
      chprintf(chp, "\033[2J\033[1;1H");
#endif
      accelerometerReadCooked(&LIS302DLD1, cookeddata);
      chprintf(chp, "LIS302DL Accelerometer cooked data...\r\n");
      for(i = 0; i < LIS302DL_NUMBER_OF_AXES; i++) {
        chprintf(chp, "%c-axis: %.4f mg\r\n", axisID[i], cookeddata[i]);
      }
    }
    else {
      chprintf(chp, "Usage: read [raw|cooked]\r\n");
      return;
    }
  }
  chprintf(chp, "Stopped\r\n");
}

static void cmd_fullscale(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)argv;
  if (argc != 1) {
    chprintf(chp, "Usage: fullscale [2G|8G]\r\n");
    return;
  }
#if CHPRINTF_USE_ANSI_CODE
    chprintf(chp, "\033[2J\033[1;1H");
#endif
  if(!strcmp (argv[0], "2G")) {
    accelerometerSetFullScale(&LIS302DLD1, LIS302DL_FS_2G);
    chprintf(chp, "LIS302DL Accelerometer full scale set to 2G...\r\n");
  }
  else if(!strcmp (argv[0], "8G")) {
    accelerometerSetFullScale(&LIS302DLD1, LIS302DL_FS_8G);
    chprintf(chp, "LIS302DL Accelerometer full scale set to 8G...\r\n");
  }
  else {
    chprintf(chp, "Usage: fullscale [2G|8G]\r\n");
    return;
  }
}

static const ShellCommand commands[] = {
  {"read", cmd_read},
  {"fullscale", cmd_fullscale},
  {NULL, NULL}
};

static const ShellConfig shell_cfg1 = {
  (BaseSequentialStream *)&SDU1,
  commands
};

/*===========================================================================*/
/* Main code.                                                                */
/*===========================================================================*/

/*
 * LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (true) {
    systime_t time;

    time = serusbcfg.usbp->state == USB_ACTIVE ? 250 : 500;
    palClearLine(LINE_LED6);
    chThdSleepMilliseconds(time);
    palSetLine(LINE_LED6);
    chThdSleepMilliseconds(time);
  }
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

  /* Initializes a serial-over-USB CDC driver.*/
  sduObjectInit(&SDU1);
  sduStart(&SDU1, &serusbcfg);

  /*
   * Activates the USB driver and then the USB bus pull-up on D+.
   * Note, a delay is inserted in order to not have to disconnect the cable
   * after a reset.
   */
  usbDisconnectBus(serusbcfg.usbp);
  chThdSleepMilliseconds(1500);
  usbStart(serusbcfg.usbp, &usbcfg);
  usbConnectBus(serusbcfg.usbp);

  /* Creates the blinker thread.*/
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO + 1, Thread1, NULL);

  /* LIS302DL Object Initialization.*/
  lis302dlObjectInit(&LIS302DLD1);

  /* Activates the LIS302DL driver.*/
  lis302dlStart(&LIS302DLD1, &l3gd20cfg);

  /* Shell manager initialization.*/
  shellInit();

  while(TRUE) {
    if (SDU1.config->usbp->state == USB_ACTIVE) {
      thread_t *shelltp = chThdCreateFromHeap(NULL, SHELL_WA_SIZE,
                                              "shell", NORMALPRIO + 1,
                                              shellThread, (void *)&shell_cfg1);
      chThdWait(shelltp);               /* Waiting termination.             */
    }
    chThdSleepMilliseconds(1000);
  }
  lis302dlStop(&LIS302DLD1);
  return 0;
}

