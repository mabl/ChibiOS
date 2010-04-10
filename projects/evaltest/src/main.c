#include "ch.h"
#include "hal.h"

/*
 * Configuration
 */
#if CH_HAL_USE_CAN

#define CAN_BTR_PRESCALER(n) (n)
#define CAN_BTR1_BRP(n)      ((n) & 0x3F)
#define CAN_BTR1_SJW(n)      (((n) << 6) & 0xC0)
#define CAN_BTR2_BS1(n)      ((n) & 0x0F)
#define CAN_BTR2_BS2(n)      (((n) << 4) & 0x70)

const CANConfig cancfg = {
  CAN_MCR_ABOM | CAN_MCR_AWUM | CAN_MCR_TXFP,
  CAN_DGR_LBKM | CAN_DGR_TXM2E,
  CAN_BTR1_BRP(6) | CAN_BTR1_SJW(0),
  CAN_BTR2_BS1(8) | CAN_BTR2_BS2(1),
  0,
  NULL
};
#endif /* CH_HAL_USE_CAN */

/*
 * Debugging Output
 */
static void pulse_error(gpio_t* ioport, char iopad, char number) {
  signed char ii;
  while(1) {
    for (ii = number; ii > 0; ii--) {
      palSetPad(ioport, iopad);
      chThdSleepMilliseconds(500);
      palClearPad(ioport, iopad);
      chThdSleepMilliseconds(500);
    }
    chThdSleepMilliseconds(1250);
  }
}

/*
 * Threads
 */

static WORKING_AREA(wa_blinker1, 64);
static msg_t blinker1(void *arg) {
  int delay = 500;
  (void*)arg;

  while (TRUE) {
    palTogglePad(IOPORT8, PH_LD1);
    chThdSleepMilliseconds(delay);
  }
  return 0;
}

#if CH_HAL_USE_CAN
static Thread *canrtp;
static Thread *canttp;

static WORKING_AREA(can_rx_wa, 128);
static msg_t can_rx(void *p) {
  EventListener el;
  CANRxFrame rxmsg;

  (void)p;
  chEvtRegister(&CAND.cd_rxfull_event, &el, 0);
  while(!chThdShouldTerminate()) {
    if (chEvtWaitAnyTimeout(ALL_EVENTS, MS2ST(100)) == 0)
      continue;
    while (canReceive(&CAND, &rxmsg, TIME_IMMEDIATE) == RDY_OK) {
      if (rxmsg.cf_IDE != CAN_IDE_EXT) {
        pulse_error(IOPORT8, PH_LD2, 1);
      }
      if (rxmsg.id.cf_EID != 0x01234567) {
        pulse_error(IOPORT8, PH_LD2, 2);
      }
      if (rxmsg.cf_RTR != CAN_RTR_DATA) {
        pulse_error(IOPORT8, PH_LD2, 3);
      }
      if (rxmsg.cf_DLC != 8) {
        pulse_error(IOPORT8, PH_LD2, 4);
      }
      if (rxmsg.fd.cf_data32[0] != 0x55AA00FF) {
        pulse_error(IOPORT8, PH_LD2, 5);
      }
      if (rxmsg.fd.cf_data32[1] != 0x55AA00FF) {
        pulse_error(IOPORT8, PH_LD2, 6);
      }
  
      /* Process message.*/
      palTogglePad(IOPORT8, PH_LD2);
    }
    chThdSleepMilliseconds(10);
  }
  chEvtUnregister(&CAND.cd_rxfull_event, &el);
  return 0;
}

static WORKING_AREA(can_tx_wa, 128);
static msg_t can_tx(void * p) {
  CANTxFrame txmsg;

  (void)p;
  txmsg.cf_IDE = CAN_IDE_EXT;
  txmsg.id.cf_EID = 0x01234567;
  txmsg.cf_RTR = CAN_RTR_DATA;
  txmsg.cf_DLC = 8;
  txmsg.fd.cf_data16[0] = 0x55AA;
  txmsg.fd.cf_data16[1] = 0x00FF;
  txmsg.fd.cf_data16[2] = 0x55AA;
  txmsg.fd.cf_data16[3] = 0x00FF;

  while (!chThdShouldTerminate()) {
    canTransmit(&CAND, &txmsg, MS2ST(100));
    palTogglePad(IOPORT8, PH_LD3);
    chThdSleepMilliseconds(100);
  }
  return 0;
}
#endif /* CH_HAL_USE_CAN */

void main(void) {
  hwinit();
  chSysInit();

  chThdCreateStatic(wa_blinker1, sizeof(wa_blinker1), NORMALPRIO, blinker1, NULL);

  #if CH_HAL_USE_CAN
  canStart(&CAND, &cancfg);
  canrtp = chThdCreateStatic(can_rx_wa, sizeof(can_rx_wa),
                             NORMALPRIO + 7, can_rx, NULL);
  canttp = chThdCreateStatic(can_tx_wa, sizeof(can_tx_wa),
                             NORMALPRIO + 7, can_tx, NULL);
  #endif

  while (TRUE) {
    chThdSleepMilliseconds(1000);
  }
}
