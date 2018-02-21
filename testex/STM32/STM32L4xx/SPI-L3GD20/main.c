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

#include "string.h"
#include "shell.h"
#include "chprintf.h"

#include "l3gd20.h"

/*===========================================================================*/
/* L3GD20 related.                                                           */
/*===========================================================================*/

/* L3GD20 Driver: This object represent an L3GD20 instance.*/
static L3GD20Driver L3GD20D1;

static int32_t rawdata[L3GD20_NUMBER_OF_AXES];
static float cookeddata[L3GD20_NUMBER_OF_AXES];

static char axisID[L3GD20_NUMBER_OF_AXES] = {'X', 'Y', 'Z'};
static uint32_t i;

static const SPIConfig spicfg = {
  FALSE,
  NULL,
  GPIOD,
  GPIOD_GYRO_CS,
  SPI_CR1_BR | SPI_CR1_CPOL | SPI_CR1_CPHA,
  0
};

static L3GD20Config l3gd20cfg = {
  &SPID2,
  &spicfg,
  NULL,
  NULL,
  L3GD20_FS_250DPS,
  L3GD20_ODR_760HZ,
#if L3GD20_USE_ADVANCED
  L3GD20_BDU_CONTINUOUS,
  L3GD20_END_LITTLE,
  L3GD20_BW3,
  L3GD20_HPM_REFERENCE,
  L3GD20_HPCF_8,
  L3GD20_LP2M_ON,
#endif
};

/*===========================================================================*/
/* Command line related.                                                     */
/*===========================================================================*/

/*
 * DP resistor control is not possible on the STM32F3-Discovery, using stubs
 * for the connection macros.
 */
#define usb_lld_connect_bus(usbp)
#define usb_lld_disconnect_bus(usbp)

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
      gyroscopeReadRaw(&L3GD20D1, rawdata);
      chprintf(chp, "L3GD20 Gyroscope raw data...\r\n");
      for(i = 0; i < L3GD20_NUMBER_OF_AXES; i++) {
        chprintf(chp, "%c-axis: %d\r\n", axisID[i], rawdata[i]);
      }
    }
    else if (!strcmp (argv[0], "cooked")) {
#if CHPRINTF_USE_ANSI_CODE
      chprintf(chp, "\033[2J\033[1;1H");
#endif
      gyroscopeReadCooked(&L3GD20D1, cookeddata);
      chprintf(chp, "L3GD20 Gyroscope cooked data...\r\n");
      for(i = 0; i < L3GD20_NUMBER_OF_AXES; i++) {
        chprintf(chp, "%c-axis: %.4f DPS\r\n", axisID[i], cookeddata[i]);
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
    chprintf(chp, "Usage: fullscale [250|500|2000]\r\n");
    return;
  }
#if CHPRINTF_USE_ANSI_CODE
    chprintf(chp, "\033[2J\033[1;1H");
#endif
  if(!strcmp (argv[0], "250")) {
    gyroscopeSetFullScale(&L3GD20D1, L3GD20_FS_250DPS);
    chprintf(chp, "L3GD20 Gyroscope full scale set to 250 dps...\r\n");
  }
  else if(!strcmp (argv[0], "500")) {
    gyroscopeSetFullScale(&L3GD20D1, L3GD20_FS_500DPS);
    chprintf(chp, "L3GD20 Gyroscope full scale set to 500 dps...\r\n");
  }
  else if(!strcmp (argv[0], "2000")) {
    gyroscopeSetFullScale(&L3GD20D1, L3GD20_FS_2000DPS);
    chprintf(chp, "L3GD20 Gyroscope full scale set to 2000 dps...\r\n");
  }
  else {
    chprintf(chp, "Usage: fullscale [250|500|2000]\r\n");
    return;
  }
}

static void cmd_bias(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)argv;
  if (argc != 1) {
    chprintf(chp, "Usage: bias [sample|reset]\r\n");
    return;
  }
  if(!strcmp (argv[0], "sample")) {
#if CHPRINTF_USE_ANSI_CODE
    chprintf(chp, "\033[2J\033[1;1H");
#endif
    chprintf(chp, "Please don't move the device while Green LED is on!\r\n");
    chprintf(chp, "Press a key to start...\r\n");
    while (chnGetTimeout((BaseChannel *)chp, 500) == Q_TIMEOUT)
      ;
    palSetLine(LINE_LED_GREEN);
    chThdSleepMilliseconds(1000);
    gyroscopeSampleBias(&L3GD20D1);
    palClearLine(LINE_LED_GREEN);
    chprintf(chp, "Procedure completed!\r\n");
  }
  else if(!strcmp (argv[0], "reset")) {
#if CHPRINTF_USE_ANSI_CODE
    chprintf(chp, "\033[2J\033[1;1H");
#endif
    gyroscopeResetBias(&L3GD20D1);
    chprintf(chp, "Bias correction removed!\r\n");
  }
  else {
    chprintf(chp, "Usage: bias [sample|reset]\r\n");
    return;
  }
}

static const ShellCommand commands[] = {
  {"read", cmd_read},
  {"fullscale", cmd_fullscale},
  {"bias", cmd_bias},
  {NULL, NULL}
};

static const ShellConfig shell_cfg1 = {
  (BaseSequentialStream *)&SD2,
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
    palClearLine(LINE_LED_RED);
    chThdSleepMilliseconds(500);
    palSetLine(LINE_LED_RED);
    chThdSleepMilliseconds(500);
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

  /* Activates the serial driver 2 using the driver default configuration.*/
  sdStart(&SD2, NULL);

  /* Creates the blinker thread.*/
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO + 1, Thread1, NULL);

  /* L3GD20 Object Initialization.*/
  l3gd20ObjectInit(&L3GD20D1);

  /* Activates the L3GD20 driver.*/
  l3gd20Start(&L3GD20D1, &l3gd20cfg);

  /* Shell manager initialization.*/
  shellInit();

  while(TRUE) {
    thread_t *shelltp = chThdCreateFromHeap(NULL, SHELL_WA_SIZE,
                                              "shell", NORMALPRIO + 1,
                                              shellThread, (void *)&shell_cfg1);
    chThdWait(shelltp);               /* Waiting termination.             */
  }
  l3gd20Stop(&L3GD20D1);
  return 0;
}
