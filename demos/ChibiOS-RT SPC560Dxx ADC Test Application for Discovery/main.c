/*
    ChibiOS/RT - Copyright (C) 2006-2014 Giovanni Di Sirio

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

/* Inclusion of the main header files of all the imported components in the
   order specified in the application wizard. The file is generated
   automatically.*/
#include "components.h"
#include "shellcmd.h"
#include "adc_lld_cfg.h"

/*
 * Shell configuration structure, the first field is the serial port used by
 * the shell, the second is the array of commands accepted by the shell and
 * defined in shellcmd.c.
 */
static const ShellConfig shell_cfg1 = {
  (BaseSequentialStream *)&SD1,
  shell_commands
};

/*
 * LEDs blinker thread, times are in milliseconds.
 */
static WORKING_AREA(waThread1, 128);
static msg_t Thread1(void *arg) {

  (void)arg;
  chRegSetThreadName("blinker");

  while (TRUE) {
    unsigned i;

    for (i = 0; i < 4; i++) {
      palClearPad(PORT_C, PC_LED7);
      osalThreadSleepMilliseconds(100);
      palClearPad(PORT_C, PC_LED8);
      osalThreadSleepMilliseconds(100);
      palSetPad(PORT_C, PC_LED7);
      osalThreadSleepMilliseconds(100);
      palSetPad(PORT_C, PC_LED8);
      osalThreadSleepMilliseconds(100);
    }

    for (i = 0; i < 4; i++) {
      palTogglePad(PORT_C, PC_LED7);
      osalThreadSleepMilliseconds(250);
      palTogglePad(PORT_C, PC_LED7);
      palTogglePad(PORT_C, PC_LED8);
      osalThreadSleepMilliseconds(250);
      palTogglePad(PORT_C, PC_LED8);
    }

    palSetPort(PORT_C, PAL_PORT_BIT(PC_LED7) | PAL_PORT_BIT(PC_LED8));
  }

  return 0;
}

/*
 * Buffers used for the ADC conversions, note, the constants are defined
 * in adc_lld_cfg.h and are generated automatically.
 */
static adcsample_t samples[ADC1_GROUP_CFG0_NUM_CHANNELS *
                           ADC1_GROUP_CFG0_BUF_DEPTH];

/*
 * ADC streaming callback, the name is defined in the ADC graphic
 * configuration.
 */
size_t nx = 0, ny = 0;
void adc_conversion_callback(ADCDriver *adcp, adcsample_t *buffer, size_t n) {

  (void)adcp;
  if (samples == buffer) {
    nx += n;
  }
  else {
    ny += n;
  }
}

/*
 * ADC error callback, the name is defined in the ADC graphic
 * configuration.
 */
uint32_t errore = 30;
void adc_error_callback(ADCDriver *adcp, adcerror_t err) {

  (void)adcp;
  errore = err;

  palClearPad(PORT_C, PC_LED8);
  osalSysHalt("ADC failure");
}

/*
 * Display the buffer in the shell
 */
void adc_print_buffer(BaseSequentialStream *chp) {
  int i;

  chprintf(chp, "Result ADC CONVERSION \r\n");

  for (i=0; i<ADC1_GROUP_CFG0_NUM_CHANNELS * ADC1_GROUP_CFG0_BUF_DEPTH; i++) {
    chprintf(chp, "Item: %d ==> %d\r\n",i,samples[i]);
  }
}


/*
 * Application entry point.
 */
int main(void) {
  Thread *shelltp = NULL;

  /* Initialization of all the imported components in the order specified in
     the application wizard. The function is generated automatically.*/
  componentsInit();

  /*
   * Activates the serial driver 1 using the driver default configuration.
   */
  sdStart(&SD1, NULL);

  /* Starts the ADCD2 driver.*/
  adcStart(&ADCD2, NULL);

  /* Starts an ADC continuous conversion.*/
  adcStartConversion(&ADCD2, &adc1_group_cfg0,
                     samples, ADC1_GROUP_CFG0_BUF_DEPTH);

  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  /* Application main loop.*/
  while (1) {
    if (!shelltp)
      shelltp = shellCreate(&shell_cfg1, SHELL_WA_SIZE, NORMALPRIO);
    else if (chThdTerminated(shelltp)) {
      chThdRelease(shelltp);    /* Recovers memory of the previous shell.   */
      shelltp = NULL;           /* Triggers spawning of a new shell.        */
    }

    /* Normal main() thread activity, once the button is pressed the background
        ADC conversion is stopped and displayed the buffer.*/
    if (palReadPad(PORT_A, PA_BUTTON) == 0) {
      adcStopConversion(&ADCD2);
      adc_print_buffer((BaseSequentialStream *)&SD1);
      palClearPad(PORT_C, PC_LED7);
    }

    chThdSleepMilliseconds(1000);
  }
}
