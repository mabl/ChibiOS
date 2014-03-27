/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

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

#include "adc_lld_cfg.h"

/*
 * Buffers used for the ADC conversions, note, the constants are defined
 * in adc_lld_cfg.h and are generated automatically.
 */
static adcsample_t samples1[ADC_GROUP_CGRP1_NUM_CHANNELS *
                            ADC_GROUP_CGRP1_BUF_DEPTH];
static adcsample_t samples2[ADC_GROUP_CGRP2_NUM_CHANNELS *
                            ADC_GROUP_CGRP2_BUF_DEPTH];

/*
 * ADC streaming callback, the name is defined in the ADC graphic
 * configuration.
 */
size_t nx = 0, ny = 0;
void adc_conversion_callback(ADCDriver *adcp, adcsample_t *buffer, size_t n) {

  (void)adcp;
  if (samples2 == buffer) {
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
void adc_error_callback(ADCDriver *adcp, adcerror_t err) {

  (void)adcp;
  (void)err;

  palSetPad(PORT11, P11_LED4);
  chSysHalt();
}

/*
 * LED blinker thread, times are in milliseconds.
 */
static WORKING_AREA(waThread1, 128);
static msg_t Thread1(void *arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (TRUE) {
    palSetPad(PORT11, P11_LED1);
    chThdSleepMilliseconds(500);
    palClearPad(PORT11, P11_LED1);
    chThdSleepMilliseconds(500);
  }
  return 0;
}

/*
 * Application entry point.
 */
int main(void) {

  /* Initialization of all the imported components in the order specified in
     the application wizard. The function is generated automatically.*/
  componentsInit();

  /* Creates the blinker thread.*/
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  /* Starts the ADCD1, ADCD4 drivers.*/
  adcStart(&ADCD1, NULL);
  adcStart(&ADCD4, NULL);

  /* Linear conversion.*/
  adcConvert(&ADCD1, &adc_group_cgrp1,
             samples1, ADC_GROUP_CGRP1_BUF_DEPTH);
  chThdSleepMilliseconds(1000);

  /* Starts an ADC continuous conversion.*/
  adcStartConversion(&ADCD4, &adc_group_cgrp2,
                     samples2, ADC_GROUP_CGRP2_BUF_DEPTH);

  /* Normal main() thread activity, once the button is pressed the background
     ADC conversion is stopped.*/
  while (TRUE) {
    if (palReadPad(PORT11, P11_BUTTON1)) {
      adcStopConversion(&ADCD4);
    }
    chThdSleepMilliseconds(500);
  }
}
