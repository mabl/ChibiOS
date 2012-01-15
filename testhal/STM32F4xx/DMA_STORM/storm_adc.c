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


#define ADC_GRP_NUM_CHANNELS   6
#define ADC_GRP_BUF_DEPTH      32

static adcsample_t samples[ADC_GRP_NUM_CHANNELS * ADC_GRP_BUF_DEPTH];

/*
 * ADC streaming callback.
 */
static void adccallback(ADCDriver *adcp, adcsample_t *buffer, size_t n) {

  (void)adcp;
  (void)buffer;
  (void)n;
}

static void adcerrorcallback(ADCDriver *adcp, adcerror_t err) {

  (void)adcp;
  (void)err;

  chSysHalt();
}

/*
 * ADC conversion group.
 * Mode:        Continuous, 16 samples of 8 channels, SW triggered.
 * Channels:    IN11, IN12, IN11, IN12, IN11, IN12, Sensor, VRef.
 */
static const ADCConversionGroup adcgrpcfg = {
  TRUE,
  ADC_GRP_NUM_CHANNELS,
  adccallback,
  adcerrorcallback,
  0,                        /* CR1 */
  ADC_CR2_SWSTART,          /* CR2 */
  ADC_SMPR1_SMP_AN10(ADC_SAMPLE_3) |
  ADC_SMPR1_SMP_AN11(ADC_SAMPLE_3) |
  ADC_SMPR1_SMP_AN12(ADC_SAMPLE_3) |
  ADC_SMPR1_SMP_AN13(ADC_SAMPLE_3) |
  ADC_SMPR1_SMP_AN14(ADC_SAMPLE_3) |
  ADC_SMPR1_SMP_AN15(ADC_SAMPLE_3),
  0,                        /* SMPR2 */
  ADC_SQR1_NUM_CH(ADC_GRP_NUM_CHANNELS),
  0,
  ADC_SQR3_SQ6_N(ADC_CHANNEL_IN15)   | ADC_SQR3_SQ5_N(ADC_CHANNEL_IN14) |
  ADC_SQR3_SQ4_N(ADC_CHANNEL_IN13)   | ADC_SQR3_SQ3_N(ADC_CHANNEL_IN12) |
  ADC_SQR3_SQ2_N(ADC_CHANNEL_IN11)   | ADC_SQR3_SQ1_N(ADC_CHANNEL_IN10)
};

/*
 * Red LEDs blinker thread, times are in milliseconds.
 */
static WORKING_AREA(StormAdcThreadWA, 512);
static msg_t storm_adc(void *arg) {

  (void)arg;
  return 0;
}

void storm_adc_init(void) {

  /* Activates the ADC1 driver and the thermal sensor. */
  adcStart(&ADCD1, NULL);

  /* Starts an ADC continuous conversion. */
  adcStartConversion(&ADCD1, &adcgrpcfg, samples, ADC_GRP_BUF_DEPTH);

  chThdCreateStatic(StormAdcThreadWA,
          sizeof(StormAdcThreadWA),
          NORMALPRIO,
          storm_adc,
          NULL);
}



