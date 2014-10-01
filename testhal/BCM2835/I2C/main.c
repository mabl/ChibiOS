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

static const uint8_t tmp102_address = 0x48;

/* Calibration for more accurate temperature voltage output */
static const float temperature_scaling = 1.07;

static float get_temperature(void) {
  uint8_t result[] = { 0, 0 };

  i2cAcquireBus(&I2C1);

  msg_t status = i2cMasterReceiveTimeout(
    &I2C1, tmp102_address, result, 2, MS2ST(1000));

  i2cReleaseBus(&I2C1);

  uint32_t ticks = ((result[0] << 8) | result[1]) >> 4;
  float celsius = ticks * 0.0625;
  float fahrenheit = (celsius * 9)/5 + 32;

  if (status != RDY_OK)
    chprintf((BaseSequentialStream *)&SD1, "Error while getting voltage: %d\r\n", status);

  return fahrenheit;
}

static const uint8_t mcp4725_address = 0x62;

static void set_voltage(float millivolts) {
  uint16_t level = millivolts * 1.241 * temperature_scaling;
  uint8_t request[2];
  request[0] = (level >> 8) & 0xff;
  request[1] = level & 0xff;

  i2cAcquireBus(&I2C1);

  msg_t status = i2cMasterTransmitTimeout(
    &I2C1, mcp4725_address, request, 2, 
    NULL, 0, MS2ST(1000));

  i2cReleaseBus(&I2C1);

  if (status != RDY_OK)
    chprintf((BaseSequentialStream *)&SD1, "Error while setting voltage: %d\r\n", status);
}

static WORKING_AREA(waThread1, 128);

static msg_t Thread1(void *p) {
  UNUSED(p);
  chRegSetThreadName("TMP102 Monitor");
  while (TRUE) {
    float temperature = get_temperature();
    chprintf((BaseSequentialStream *)&SD1, "Temperature: %f F\r\n", temperature);
    set_voltage(temperature);
    chThdSleepMilliseconds(1000);
  }
  return 0;
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
  chprintf((BaseSequentialStream *)&SD1, "BCM2835 I2C Demonstration\r\n");

  /*
   * I2C initialization.
   */
  I2CConfig i2cConfig;
  i2cStart(&I2C1, &i2cConfig);

  /*
   * Creates the temperature monitoring thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  /*
   * Events servicing loop.
   */
  chThdWait(chThdSelf());

  return 0;
}
