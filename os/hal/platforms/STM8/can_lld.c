/*
    ChibiOS/RT - Copyright (C) 2006-2007 Giovanni Di Sirio.

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

/**
 * @file STM8/can_lld.c
 * @brief STM8 CAN subsystem low level driver source.
 * @addtogroup STM8_CAN
 * @{
 */

#include "ch.h"
#include "hal.h"

#if CH_HAL_USE_CAN || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief CAN driver identifier.*/
#if USE_STM8_CAN || defined(__DOXYGEN__)
CANDriver CAND;
#endif

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*
 * CAN TX/ER/SC interrupt handler.
 */
CH_IRQ_HANDLER(9) {
  uint8_t reg;
  CH_IRQ_PROLOGUE();

  reg = CAN->TSR;
  CAN->TSR = CAN_TSR_RQCP0 | CAN_TSR_RQCP1 | CAN_TSR_RQCP2;

  /* Transmit Event */
  if (reg & (CAN_TSR_RQCP0 | CAN_TSR_RQCP1 | CAN_TSR_RQCP2)) {
    /* No more events until a message is transmitted.*/
    chSysLockFromIsr();
    while (chSemGetCounterI(&CAND.cd_txsem) < 0)
      chSemSignalI(&CAND.cd_txsem);
    chEvtBroadcastI(&CAND.cd_txempty_event);
    chSysUnlockFromIsr();
  }

  reg = CAN->MSR;
  CAN->MSR = CAN_MSR_ERRI | CAN_MSR_WKUI;

  /* Wakeup event.*/
  if (reg & CAN_MSR_WKUI) {
    chSysLockFromIsr();
    chEvtBroadcastI(&CAND.cd_wakeup_event);
    chSysUnlockFromIsr();
  }

  /* Error event.*/
  if (reg & CAN_MSR_ERRI) {
    canstatus_t flags;
    STM8CAN_Page_t page;
    uint8_t esr;

    page = CAN->PSR;
    CAN->PSR = STM8CAN_Page_Config;
    esr  = CAN->Page.Config.ESR;
    CAN->Page.Config.ESR &= ~CAN_ESR_LEC;
    CAN->PSR = page; /* Restore previous Page Select */
  
    flags = (canstatus_t)(esr & 7);
    if ((esr & CAN_ESR_LEC) > 0)
      flags |= CAN_FRAMING_ERROR;
    chSysLockFromIsr();
    canAddFlagsI(&CAND, flags);
    chEvtBroadcastI(&CAND.cd_error_event);
    chSysUnlockFromIsr();
  }

  CH_IRQ_EPILOGUE();
}

/*
 * CAN RX interrupt handler.
 */
CH_IRQ_HANDLER(8) {
  uint8_t rfr;

  CH_IRQ_PROLOGUE();

  rfr = CAN->RFR;
  if ((rfr & CAN_RFR_FMP01) > 0) {
    /* No more receive events until the queue has been emptied.*/
    CAN->IER &= ~CAN_IER_FMPIE;
    chSysLockFromIsr();
    while (chSemGetCounterI(&CAND.cd_rxsem) < 0)
      chSemSignalI(&CAND.cd_rxsem);
    chEvtBroadcastI(&CAND.cd_rxfull_event);
    chSysUnlockFromIsr();
  }
  if ((rfr & CAN_RFR_FOVR) > 0) {
    /* Overflow events handling.*/
    CAN->RFR = CAN_RFR_FOVR;
    canAddFlagsI(&CAND, CAN_OVERFLOW_ERROR);
    chSysLockFromIsr();
    chEvtBroadcastI(&CAND.cd_error_event);
    chSysUnlockFromIsr();
  }

  CH_IRQ_EPILOGUE();
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief Low level CAN driver initialization.
 */
void can_lld_init(void) {
#if USE_STM8_CAN
  /* Driver initialization.*/
  canObjectInit(&CAND);
  CAND.cd_can = CAN;
#endif
}

/**
 * @brief Configures and activates the CAN peripheral.
 *
 * @param[in] canp pointer to the @p CANDriver object
 */
void can_lld_start(CANDriver *canp) {
  /* Clock activation.*/
#if USE_STM8_CAN
  if (&CAND == canp) {
    CLK_PCKENR2 |= (1<<7);   /* PCKEN27 bit for CAN */
  }
#endif

  /* Entering initialization mode. */
  canp->cd_state = CAN_STARTING;
  canp->cd_can->MCR = CAN_MCR_INRQ;
  while ((canp->cd_can->MSR & CAN_MSR_INAK) == 0)
    chThdSleepS(1);

  /* BTR initialization.*/
  canp->cd_can->DGR = canp->cd_config->cc_dgr;
  canp->cd_can->PSR = STM8CAN_Page_Config;
  canp->cd_can->Page.Config.BTR1 = canp->cd_config->cc_btr1;
  canp->cd_can->Page.Config.BTR2 = canp->cd_config->cc_btr2;

  /* MCR initialization.*/
  canp->cd_can->MCR = canp->cd_config->cc_mcr;

  /* Filters initialization.*/
  if (canp->cd_config->cc_num > 0) {
    uint8_t cfn; /* Current Filter Number */

    canp->cd_can->Page.Config.FCR1 = 0;
    canp->cd_can->Page.Config.FCR2 = 0;
    canp->cd_can->Page.Config.FCR3 = 0;
    canp->cd_can->Page.Config.FMR1 = 0;
    canp->cd_can->Page.Config.FMR2 = 0;

    for (cfn = 0; cfn < CAN_MAX_FILTERS; cfn++) {
      uint8_t fact; /* FACT bit offset  in FCRx*/
      uint8_t fsc;  /* FSC bit offset  in FCRx */
      uint8_t fmhl; /* FML bit offset in FMRx */
      STM8CAN_Page_t can_page_filter; /* Page Register for current filter */

      switch(cfn) {
        case 0:
          fact = 0;
          fsc  = 1;
          fmhl = 0;
          can_page_filter = STM8CAN_Page_Filter01;
          break;
        case 1:
          fact = 4;
          fsc  = 5;
          fmhl = 2;
          can_page_filter = STM8CAN_Page_Filter01;
          break;
        case 2:
          fact = 0;
          fsc  = 1;
          fmhl = 4;
          can_page_filter = STM8CAN_Page_Filter23;
          break;
        case 3:
          fact = 4;
          fsc  = 5;
          fmhl = 6;
          can_page_filter = STM8CAN_Page_Filter23;
          break;
        case 4:
          fact = 0;
          fsc  = 1;
          fmhl = 0;
          can_page_filter = STM8CAN_Page_Filter45;
          break;
        case 5:
        default:
          fact = 4;
          fsc  = 5;
          fmhl = 2;
          can_page_filter = STM8CAN_Page_Filter45;
          break;
      }

      canp->cd_can->PSR = STM8CAN_Page_Config;

      if (cfn < canp->cd_config->cc_num) {
        /* Configure Filter Scale */
        if (can_page_filter == STM8CAN_Page_Filter01) {
          canp->cd_can->Page.Config.FCR1 |= ((uint8_t)canp->cd_config->cc_filters[cfn].cf_scale << fsc);
        }
        else if (can_page_filter == STM8CAN_Page_Filter23) {
          canp->cd_can->Page.Config.FCR2 |= ((uint8_t)canp->cd_config->cc_filters[cfn].cf_scale << fsc);
        }
        else /* if (can_page_filter == STM8CAN_Page_Filter45) */ {
          canp->cd_can->Page.Config.FCR3 |= ((uint8_t)canp->cd_config->cc_filters[cfn].cf_scale << fsc);
        }

        /* Configure Filter Mode */
        if (can_page_filter != STM8CAN_Page_Filter45) { /* Use FMR1 */
          canp->cd_can->Page.Config.FMR1 |= ((uint8_t)canp->cd_config->cc_filters[cfn].cf_mode << fmhl);
        }
        else { /* Use FMR2 */
          canp->cd_can->Page.Config.FMR2 |= ((uint8_t)canp->cd_config->cc_filters[cfn].cf_mode << fmhl);
        }

        /* Configure Filter IDs */
        canp->cd_can->PSR = can_page_filter;
        if (fsc != 1) { /* Use FR09 thru FR16 */
          if (canp->cd_config->cc_filters[cfn].cf_scale == 0) { /* 8-Bit */
            canp->cd_can->Page.Filter.FR09 = canp->cd_config->cc_filters[cfn].cf_filterid1;
            canp->cd_can->Page.Filter.FR10 = canp->cd_config->cc_filters[cfn].cf_filteridmask1;
            canp->cd_can->Page.Filter.FR11 = canp->cd_config->cc_filters[cfn].cf_filterid2;
            canp->cd_can->Page.Filter.FR12 = canp->cd_config->cc_filters[cfn].cf_filteridmask2;
            canp->cd_can->Page.Filter.FR13 = canp->cd_config->cc_filters[cfn].cf_filterid3;
            canp->cd_can->Page.Filter.FR14 = canp->cd_config->cc_filters[cfn].cf_filteridmask3;
            canp->cd_can->Page.Filter.FR15 = canp->cd_config->cc_filters[cfn].cf_filterid4;
            canp->cd_can->Page.Filter.FR16 = canp->cd_config->cc_filters[cfn].cf_filteridmask4;
          }
          else if (canp->cd_config->cc_filters[cfn].cf_scale == 1) { /* 16/8-Bit */
            canp->cd_can->Page.Filter.FR09 = canp->cd_config->cc_filters[cfn].cf_filterid1;
            canp->cd_can->Page.Filter.FR10 = canp->cd_config->cc_filters[cfn].cf_filterid2;
            canp->cd_can->Page.Filter.FR11 = canp->cd_config->cc_filters[cfn].cf_filteridmask1;
            canp->cd_can->Page.Filter.FR12 = canp->cd_config->cc_filters[cfn].cf_filteridmask2;
            canp->cd_can->Page.Filter.FR13 = canp->cd_config->cc_filters[cfn].cf_filterid3;
            canp->cd_can->Page.Filter.FR14 = canp->cd_config->cc_filters[cfn].cf_filteridmask3;
            canp->cd_can->Page.Filter.FR15 = canp->cd_config->cc_filters[cfn].cf_filterid4;
            canp->cd_can->Page.Filter.FR16 = canp->cd_config->cc_filters[cfn].cf_filteridmask4;
          }
          else if (canp->cd_config->cc_filters[cfn].cf_scale == 2) { /* 16-Bit */
            canp->cd_can->Page.Filter.FR09 = canp->cd_config->cc_filters[cfn].cf_filterid1;
            canp->cd_can->Page.Filter.FR10 = canp->cd_config->cc_filters[cfn].cf_filterid2;
            canp->cd_can->Page.Filter.FR11 = canp->cd_config->cc_filters[cfn].cf_filteridmask1;
            canp->cd_can->Page.Filter.FR12 = canp->cd_config->cc_filters[cfn].cf_filteridmask2;
            canp->cd_can->Page.Filter.FR13 = canp->cd_config->cc_filters[cfn].cf_filterid3;
            canp->cd_can->Page.Filter.FR14 = canp->cd_config->cc_filters[cfn].cf_filterid4;
            canp->cd_can->Page.Filter.FR15 = canp->cd_config->cc_filters[cfn].cf_filteridmask3;
            canp->cd_can->Page.Filter.FR16 = canp->cd_config->cc_filters[cfn].cf_filteridmask4;
          }
          else /*(canp->cd_config->cc_filters[cfn].cf_scale == 3)*/ { /* 32-Bit */
            canp->cd_can->Page.Filter.FR09 = canp->cd_config->cc_filters[cfn].cf_filterid1;
            canp->cd_can->Page.Filter.FR10 = canp->cd_config->cc_filters[cfn].cf_filterid2;
            canp->cd_can->Page.Filter.FR11 = canp->cd_config->cc_filters[cfn].cf_filterid3;
            canp->cd_can->Page.Filter.FR12 = canp->cd_config->cc_filters[cfn].cf_filterid4;
            canp->cd_can->Page.Filter.FR13 = canp->cd_config->cc_filters[cfn].cf_filteridmask1;
            canp->cd_can->Page.Filter.FR14 = canp->cd_config->cc_filters[cfn].cf_filteridmask2;
            canp->cd_can->Page.Filter.FR15 = canp->cd_config->cc_filters[cfn].cf_filteridmask3;
            canp->cd_can->Page.Filter.FR16 = canp->cd_config->cc_filters[cfn].cf_filteridmask4;
          }
        }
        else {
          /* Use FR01 thru FR08 */
          if (canp->cd_config->cc_filters[cfn].cf_scale == 0) { /* 8-Bit */
            canp->cd_can->Page.Filter.FR01 = canp->cd_config->cc_filters[cfn].cf_filterid1;
            canp->cd_can->Page.Filter.FR02 = canp->cd_config->cc_filters[cfn].cf_filteridmask1;
            canp->cd_can->Page.Filter.FR03 = canp->cd_config->cc_filters[cfn].cf_filterid2;
            canp->cd_can->Page.Filter.FR04 = canp->cd_config->cc_filters[cfn].cf_filteridmask2;
            canp->cd_can->Page.Filter.FR05 = canp->cd_config->cc_filters[cfn].cf_filterid3;
            canp->cd_can->Page.Filter.FR06 = canp->cd_config->cc_filters[cfn].cf_filteridmask3;
            canp->cd_can->Page.Filter.FR07 = canp->cd_config->cc_filters[cfn].cf_filterid4;
            canp->cd_can->Page.Filter.FR08 = canp->cd_config->cc_filters[cfn].cf_filteridmask4;
          }
          else if (canp->cd_config->cc_filters[cfn].cf_scale == 1) { /* 16/8-Bit */
            canp->cd_can->Page.Filter.FR01 = canp->cd_config->cc_filters[cfn].cf_filterid1;
            canp->cd_can->Page.Filter.FR02 = canp->cd_config->cc_filters[cfn].cf_filterid2;
            canp->cd_can->Page.Filter.FR03 = canp->cd_config->cc_filters[cfn].cf_filteridmask1;
            canp->cd_can->Page.Filter.FR04 = canp->cd_config->cc_filters[cfn].cf_filteridmask2;
            canp->cd_can->Page.Filter.FR05 = canp->cd_config->cc_filters[cfn].cf_filterid3;
            canp->cd_can->Page.Filter.FR06 = canp->cd_config->cc_filters[cfn].cf_filteridmask3;
            canp->cd_can->Page.Filter.FR07 = canp->cd_config->cc_filters[cfn].cf_filterid4;
            canp->cd_can->Page.Filter.FR08 = canp->cd_config->cc_filters[cfn].cf_filteridmask4;
          }
          else if (canp->cd_config->cc_filters[cfn].cf_scale == 2) { /* 16-Bit */
            canp->cd_can->Page.Filter.FR01 = canp->cd_config->cc_filters[cfn].cf_filterid1;
            canp->cd_can->Page.Filter.FR02 = canp->cd_config->cc_filters[cfn].cf_filterid2;
            canp->cd_can->Page.Filter.FR03 = canp->cd_config->cc_filters[cfn].cf_filteridmask1;
            canp->cd_can->Page.Filter.FR04 = canp->cd_config->cc_filters[cfn].cf_filteridmask2;
            canp->cd_can->Page.Filter.FR05 = canp->cd_config->cc_filters[cfn].cf_filterid3;
            canp->cd_can->Page.Filter.FR06 = canp->cd_config->cc_filters[cfn].cf_filterid4;
            canp->cd_can->Page.Filter.FR07 = canp->cd_config->cc_filters[cfn].cf_filteridmask3;
            canp->cd_can->Page.Filter.FR08 = canp->cd_config->cc_filters[cfn].cf_filteridmask4;
          }
          else /*(canp->cd_config->cc_filters[cfn].cf_scale == 3)*/ { /* 32-Bit */
            canp->cd_can->Page.Filter.FR01 = canp->cd_config->cc_filters[cfn].cf_filterid1;
            canp->cd_can->Page.Filter.FR02 = canp->cd_config->cc_filters[cfn].cf_filterid2;
            canp->cd_can->Page.Filter.FR03 = canp->cd_config->cc_filters[cfn].cf_filterid3;
            canp->cd_can->Page.Filter.FR04 = canp->cd_config->cc_filters[cfn].cf_filterid4;
            canp->cd_can->Page.Filter.FR05 = canp->cd_config->cc_filters[cfn].cf_filteridmask1;
            canp->cd_can->Page.Filter.FR06 = canp->cd_config->cc_filters[cfn].cf_filteridmask2;
            canp->cd_can->Page.Filter.FR07 = canp->cd_config->cc_filters[cfn].cf_filteridmask3;
            canp->cd_can->Page.Filter.FR08 = canp->cd_config->cc_filters[cfn].cf_filteridmask4;
          }
        }

        /* Configure Filter Activation */
        canp->cd_can->PSR = STM8CAN_Page_Config;
        if (cfn <= 1)
          canp->cd_can->Page.Config.FCR1 |= 1 << fact;
        else if (cfn <= 3)
          canp->cd_can->Page.Config.FCR2 |= 1 << fact;
        else /* if (cfn <= 5) */
          canp->cd_can->Page.Config.FCR3 |= 1 << fact;
      }
      else {
        /* Leave Filter Disabled */
      }

      /* Gives a chance for preemption since this is a rather long loop.*/
      //chSysUnlock();
      //chThdYield();
      //chSysLock();
    }
  }
  else {
    /* Disable all filters (default).*/
    canp->cd_can->Page.Config.FCR1 = 0;
    canp->cd_can->Page.Config.FCR2 = 0;
    canp->cd_can->Page.Config.FCR3 = 0;
    canp->cd_can->Page.Config.FMR1 = 0;
    canp->cd_can->Page.Config.FMR2 = 0;
  }

  /* Leave Initialization */
  canp->cd_can->MCR &= (uint8_t)(~CAN_MCR_INRQ);
  while ((canp->cd_can->MSR & CAN_MSR_INAK))
    chThdSleepS(1);

  /* Interrupt sources initialization.*/
  canp->cd_can->PSR = STM8CAN_Page_Config;
  canp->cd_can->IER = CAN_IER_WKUIE | CAN_IER_FOVIE | CAN_IER_FFIE |
                      CAN_IER_FMPIE | CAN_IER_TMEIE;
  canp->cd_can->Page.Config.EIER = CAN_EIER_ERRIE | CAN_EIER_LECIE |
                      CAN_EIER_BOFIE | CAN_EIER_EPVIE | CAN_EIER_EWGIE;
}

/**
 * @brief Deactivates the CAN peripheral.
 *
 * @param[in] canp pointer to the @p CANDriver object
 */
void can_lld_stop(CANDriver *canp) {
  /* If in ready state then disables the CAN peripheral.*/
  if (canp->cd_state == CAN_READY) {
#if USE_STM8_CAN
    canp->cd_can->MCR = 0x02;                 /* Register reset value.    */
    canp->cd_can->PSR = STM8CAN_Page_Config;
    canp->cd_can->IER = 0x00;                 /* All interrupts disabled. */
    canp->cd_can->Page.Config.EIER = 0x00;
#endif
  }
}

/**
 * @brief Determines whether a frame can be transmitted.
 *
 * @param[in] canp pointer to the @p CANDriver object
 *
 * @return The queue space availability.
 * @retval FALSE no space in the transmit queue.
 * @retval TRUE transmit slot available.
 */
bool_t can_lld_can_transmit(CANDriver *canp) {
  return (canp->cd_can->TPR & CAN_TPR_TME012) != 0;
}

/**
 * @brief Inserts a frame into the transmit queue.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 * @param[in] ctfp      pointer to the CAN frame to be transmitted
 */
void can_lld_transmit(CANDriver *canp, const CANTxFrame *ctfp) {
  uint8_t mbxcode;

  /* Get a free transmission mailbox.*/
  mbxcode = canp->cd_can->TPR & CAN_TPR_CODE01;
  if (mbxcode == 0)
    canp->cd_can->PSR = STM8CAN_Page_TxMailBox0;
  else if (mbxcode == 1)
    canp->cd_can->PSR = STM8CAN_Page_TxMailBox1;
  else /* if (mbxcode == 2) */
    canp->cd_can->PSR = STM8CAN_Page_TxMailBox2;

  /* Preparing the message.*/
  if (ctfp->cf_IDE) { /* Extended Identifier Type */
    uint32_t canid = ctfp->id.cf_EID & CAN_EXTID_MASK;
    canp->cd_can->Page.TxMailbox.MIDR4 = (uint8_t)(canid);
    canid >>= 8;
    canp->cd_can->Page.TxMailbox.MIDR3 = (uint8_t)(canid);
    canid >>= 8;
    canp->cd_can->Page.TxMailbox.MIDR2 = (uint8_t)(canid);
    canid >>= 8;
    canp->cd_can->Page.TxMailbox.MIDR1 = (uint8_t)(canid) | CAN_MIDR1_IDE |
          (uint8_t)(ctfp->cf_RTR << 5);
  }
  else { /* Standard Identifier Type */
    uint16_t canid = ctfp->id.cf_SID & CAN_STDID_MASK;
    canp->cd_can->Page.TxMailbox.MIDR2 = (uint8_t)(canid << 2);
    canp->cd_can->Page.TxMailbox.MIDR1 = (uint8_t)(canid >> 6) |
          (uint8_t)(ctfp->cf_RTR << 5);
  }

  canp->cd_can->Page.TxMailbox.MDLCR &= 0xF0; /* Clear old DLC Value */
  canp->cd_can->Page.TxMailbox.MDLCR |= ctfp->cf_DLC; /* Set new DLC */

  canp->cd_can->Page.TxMailbox.MDAR1 = ctfp->fd.cf_data8[0];
  canp->cd_can->Page.TxMailbox.MDAR2 = ctfp->fd.cf_data8[1];
  canp->cd_can->Page.TxMailbox.MDAR3 = ctfp->fd.cf_data8[2];
  canp->cd_can->Page.TxMailbox.MDAR4 = ctfp->fd.cf_data8[3];
  canp->cd_can->Page.TxMailbox.MDAR5 = ctfp->fd.cf_data8[4];
  canp->cd_can->Page.TxMailbox.MDAR6 = ctfp->fd.cf_data8[5];
  canp->cd_can->Page.TxMailbox.MDAR7 = ctfp->fd.cf_data8[6];
  canp->cd_can->Page.TxMailbox.MDAR8 = ctfp->fd.cf_data8[7];

  /* Request Transmission */
  canp->cd_can->Page.TxMailbox.MCSR |= CAN_MCSR_TXRQ;
}

/**
 * @brief Determines whether a frame has been received.
 *
 * @param[in] canp pointer to the @p CANDriver object
 *
 * @return The queue space availability.
 * @retval FALSE no messages in the receive queue.
 * @retval TRUE message(s) available.
 */
bool_t can_lld_can_receive(CANDriver *canp) {
  return (canp->cd_can->RFR & CAN_RFR_FMP01) > 0;
}

/**
 * @brief Receives a frame from the input queue.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 * @param[out] crfp     pointer to the buffer where the CAN frame is copied
 */
void can_lld_receive(CANDriver *canp, CANRxFrame *crfp) {
//TODO: port to stm8
  uint8_t r;
  /* Select FIFO Page */
  canp->cd_can->PSR = STM8CAN_Page_RxFifo;

  /* Get the Identifier */
  r = canp->cd_can->Page.RxFIFO.MIDR1;
  crfp->cf_RTR = (r & CAN_MIDR1_RTR) >> 5;
  crfp->cf_IDE = (r & CAN_MIDR1_IDE) >> 6;

  if (crfp->cf_IDE) {
    /* Extended Identifier */
    crfp->id.cf_EID  = (uint32_t)(canp->cd_can->Page.RxFIFO.MIDR4);
    crfp->id.cf_EID |= (uint32_t)((uint32_t)canp->cd_can->Page.RxFIFO.MIDR3 << 8);
    crfp->id.cf_EID |= (uint32_t)((uint32_t)canp->cd_can->Page.RxFIFO.MIDR2 << 16);
    crfp->id.cf_EID |= (uint32_t)(((uint32_t)canp->cd_can->Page.RxFIFO.MIDR1 & 0x1F) << 24);
  }
  else {
    /* Standard Identifier */
    crfp->id.cf_SID  = ((uint16_t)((uint16_t)(canp->cd_can->Page.RxFIFO.MIDR1 & 0x1F) << 6) |
                    ((canp->cd_can->Page.RxFIFO.MIDR2 >> 2) & 0x3F));
  }

  crfp->cf_DLC = (uint8_t)(canp->cd_can->Page.RxFIFO.MDLCR & 0x0F);
  crfp->cf_FMI = canp->cd_can->Page.RxFIFO.MFMI;
  crfp->cf_TIME = canp->cd_can->Page.RxFIFO.MTSRL;
  crfp->cf_TIME |= (uint16_t)(((uint16_t)canp->cd_can->Page.RxFIFO.MTSRH) << 8);
  crfp->fd.cf_data8[0] = canp->cd_can->Page.RxFIFO.MDAR1;
  crfp->fd.cf_data8[1] = canp->cd_can->Page.RxFIFO.MDAR2;
  crfp->fd.cf_data8[2] = canp->cd_can->Page.RxFIFO.MDAR3;
  crfp->fd.cf_data8[3] = canp->cd_can->Page.RxFIFO.MDAR4;
  crfp->fd.cf_data8[4] = canp->cd_can->Page.RxFIFO.MDAR5;
  crfp->fd.cf_data8[5] = canp->cd_can->Page.RxFIFO.MDAR6;
  crfp->fd.cf_data8[6] = canp->cd_can->Page.RxFIFO.MDAR7;
  crfp->fd.cf_data8[7] = canp->cd_can->Page.RxFIFO.MDAR8;

  /* Releases the mailbox FIFO.*/
  canp->cd_can->RFR = CAN_RFR_RFOM;

  /* If the queue is empty re-enables the interrupt in order to generate
     events again.*/
  if ((canp->cd_can->RFR & CAN_RFR_FMP01) == 0)
    canp->cd_can->IER |= CAN_IER_FMPIE;
}

#if CAN_USE_SLEEP_MODE || defined(__DOXYGEN__)
/**
 * @brief Enters the sleep mode.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 */
void can_lld_sleep(CANDriver *canp) {
  canp->cd_can->MCR |= CAN_MCR_SLEEP;
}

/**
 * @brief Enforces leaving the sleep mode.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 */
void can_lld_wakeup(CANDriver *canp) {
  canp->cd_can->MCR &= ~CAN_MCR_SLEEP;
}
#endif /* CAN_USE_SLEEP_MODE */

#endif /* CH_HAL_USE_CAN */

/** @} */
