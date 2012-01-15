#include <string.h>
#include "ch.h"
#include "hal.h"
#include "storm_sdc.h"


#define SDC_BURST_SIZE              16

static uint8_t outbuf[SDC_BLOCK_SIZE * SDC_BURST_SIZE + 1];
static uint8_t  inbuf[SDC_BLOCK_SIZE * SDC_BURST_SIZE + 1];


static void clearbuffers(void){
  uint32_t i = 0;
  for (i=0; i < SDC_BLOCK_SIZE * SDC_BURST_SIZE; i++)
    outbuf[i] = 0x55;
  for (i=0; i < SDC_BLOCK_SIZE * SDC_BURST_SIZE; i++)
    inbuf[i] = 0x55;
}

/*
 * SDIO configuration.
 */
static const SDCConfig sdccfg = {
  0
};

static WORKING_AREA(StormSdcThreadWA, 1024);
static msg_t storm_sdc(void *arg) {
  chRegSetThreadName("sdc");
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

void storm_sdc_init(void){

  /* Initializes the SDIO drivers. */
  sdcStart(&SDCD1, &sdccfg);

  chThdCreateStatic(StormSdcThreadWA,
          sizeof(StormSdcThreadWA),
          NORMALPRIO,
          storm_sdc,
          NULL);
}
