#include <stdlib.h>
#include <string.h>

#include "ch.h"
#include "hal.h"
#include "storm_i2c.h"


#define addr 0b1010000
#define EEPROM_BLOCK_SIZE   128 /* bytes*/
#define EEPROM_BURST_SIZE   32  /* blocks */

/**
 * @brief   Split one uint16_t address to two uint8_t.
 *
 * @param[in] txbuf pointer to driver transmit buffer
 * @param[in] addr  uint16_t address
 */
#define eeprom_split_addr(txbuf, addr){                       \
  (txbuf)[0] = ((uint8_t)((addr >> 8) & 0xFF));               \
  (txbuf)[1] = ((uint8_t)(addr & 0xFF));                      \
}

static const I2CConfig i2cfg2 = {
    OPMODE_I2C,
    400000,
    FAST_DUTY_CYCLE_16_9,
};
static uint8_t rxbuf[EEPROM_BLOCK_SIZE * EEPROM_BURST_SIZE];
static uint8_t referencebuf[sizeof(rxbuf)];
static uint8_t txbuf[EEPROM_BLOCK_SIZE + 2];


static WORKING_AREA(EepromThreadWA, 1024);
static msg_t storm_i2c(void *arg){
  chRegSetThreadName("i2c");
  (void)arg;
  uint32_t i = 0;
  msg_t status = RDY_OK;

  /* First fill reference buffer with data begin from zero address in eeprom.*/
  txbuf[0] = 0;
  txbuf[1] = 0;
  for (i=0; i < sizeof(referencebuf); i++)
    referencebuf[i] = 0x55;
  status = i2cMasterTransmitTimeout(&I2CD2, addr, txbuf, 2, referencebuf, sizeof(referencebuf), MS2ST(500));
  if (status != RDY_OK)
    chSysHalt();

  while (TRUE) {
    /* now infinitely read data from eeprom and compare with reference */
    status = i2cMasterTransmitTimeout(&I2CD2, addr, txbuf, 2, rxbuf, sizeof(rxbuf), MS2ST(500));
    if (status != RDY_OK)
      chSysHalt();
    if (memcmp(rxbuf, referencebuf, sizeof(rxbuf)) != 0)
      chSysHalt();

    /* erase rx buffer */
    for (i=0; i < sizeof(rxbuf); i++)
      rxbuf[i] = 0x55;
  }
  return 0;
}


void storm_i2c_init(void){
  uint32_t i = 0;
  uint32_t n = 0;

  i2cStart(&I2CD2, &i2cfg2);
  chThdSleepMilliseconds(2);

  /* fill eeprom with patterns */
  while (i < EEPROM_BURST_SIZE){
    eeprom_split_addr(txbuf, (EEPROM_BLOCK_SIZE * i))
    for (n=0; n<EEPROM_BLOCK_SIZE; n++)
      txbuf[n+2] = i;
    i2cMasterTransmit(&I2CD2, addr, txbuf, (EEPROM_BLOCK_SIZE + 2), rxbuf, 0);
    /* wait until eeprom busy */
    chThdSleepMilliseconds(5);
    i++;
  }

  chThdCreateStatic(EepromThreadWA,
          sizeof(EepromThreadWA),
          NORMALPRIO,
          storm_i2c,
          NULL);
}


