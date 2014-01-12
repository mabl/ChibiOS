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

/**
 * @file    RL78/serial_lld.c
 * @brief   RL78 low level serial driver code.
 *
 * @addtogroup SERIAL
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_SERIAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

#if RL78_SERIAL_USE_UART0 || defined(__DOXYGEN__)
/** @brief UART0 serial driver identifier.*/
SerialDriver SD1;
#endif

#if RL78_SERIAL_USE_UART1 || defined(__DOXYGEN__)
/** @brief UART1 serial driver identifier.*/
SerialDriver SD2;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/** @brief Driver default configuration.*/
static const SerialConfig default_config = {
  .sc_speed = SERIAL_DEFAULT_BITRATE,
  .sc_scr = 0,
  .sc_smr = 0,
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   UART initialization.
 *
 * @param[in] sdp       communication channel associated to the UART
 * @param[in] config    the architecture-dependent serial driver configuration
 */
static void uart_init(SerialDriver *sdp, const SerialConfig *config) {

  SPS0 = 0x04; // 32MHz/16 = 2MHz
  SMR02 = 0x20 | 3;
  SCR02 = 0x04 | 0x8000 | 0x80 | 0x10 | 3;
  SDR02 = (uint16_t)103<<9; /* hardcoded 9600bps */
  SO0 |= 4;
  SOE0 |= 4;
  PMC0_bit.no2 = 0; // TxD as digital
/* RxD1 as digital (initial is analog) */
/*  PMC0_bit.no3 = 0; */
/* TxD1 as digital (initial is analog) */
  PM0_bit.no2 = 0;
  P0_bit.no2 = 1;
  SS0 |= 4;
}

/**
 * @brief   UART de-initialization.
 *
 * @param[in] u         pointer to an UART I/O block
 */
static void uart_deinit(SerialDriver *sdp) {

/*  ST0 = 0x04; */
}

/**
 * @brief   Error handling routine.
 *
 * @param[in] sdp       communication channel associated to the UART
 * @param[in] err       UART LSR register value
 */
static void set_error(SerialDriver *sdp, uint32_t err) {

  flagsmask_t sts = 0;

  if (err & 1)
    sts |= SD_OVERRUN_ERROR;
  if (err & 2)
    sts |= SD_PARITY_ERROR;
  if (err & 4)
    sts |= SD_FRAMING_ERROR;
  chSysLockFromIsr();
  chnAddFlagsI(sdp, sts);
  chSysUnlockFromIsr();
}

#if RL78_SERIAL_USE_UART1 || defined(__DOXYGEN__)
/**
 * @brief   Error IRQ handler.
 *
 * @param[in] sdp       communication channel associated to the UART
 */
static void serve_interrupt_ser1(SerialDriver *sdp) {

  set_error(sdp, SSR02);
}

/**
 * @brief   Receive IRQ handler.
 *
 * @param[in] sdp       communication channel associated to the UART
 */
static void serve_interrupt_sr1(SerialDriver *sdp) {

}

/**
 * @brief   Transmit IRQ handler.
 *
 * @param[in] sdp       communication channel associated to the UART
 */
__attribute__((noinline))
static void serve_interrupt_st1(SerialDriver *sdp) {

  msg_t b;

  chSysLockFromIsr();
  b = chOQGetI(&sdp->oqueue);
  chSysUnlockFromIsr();
  if (b < Q_OK) {
    STMK1 = 1;  /* Disable INTST1 interrupt */
    STIF1 = 1;  /* Set INTST1 interrupt request flag */
    chSysLockFromIsr();
    chnAddFlagsI(sdp, CHN_OUTPUT_EMPTY);
    chSysUnlockFromIsr();
    return;
  }
  TXD1 = b;
}

/**
 * @brief   Driver SD2 output notification.
 */
__attribute__((noinline))
static void notify2(GenericQueue *qp) {

  STMK1 = 0;  /* Enable INTST1 interrupt */
}
#endif

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/**
 * @brief   UART1 IRQ handler.
 *
 * @isr
 */
#if RL78_SERIAL_USE_UART1 || defined(__DOXYGEN__)
CH_IRQ_HANDLER(INT_SRE1) {

  CH_IRQ_PROLOGUE();

  serve_interrupt_ser1(&SD2);

  CH_IRQ_EPILOGUE();
}
CH_IRQ_HANDLER(INT_SR1) {

  CH_IRQ_PROLOGUE();

  serve_interrupt_sr1(&SD2);

  CH_IRQ_EPILOGUE();
}
CH_IRQ_HANDLER(INT_ST1) {

  CH_IRQ_PROLOGUE();

  serve_interrupt_st1(&SD2);

  CH_IRQ_EPILOGUE();
}
#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level serial driver initialization.
 *
 * @notapi
 */
void sd_lld_init(void) {

#if RL78_SERIAL_USE_UART1
  sdObjectInit(&SD2, NULL, notify2);
#endif
}

/**
 * @brief   Low level serial driver configuration and (re)start.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 * @param[in] config    the architecture-dependent serial driver configuration.
 *                      If this parameter is set to @p NULL then a default
 *                      configuration is used.
 *
 * @notapi
 */
void sd_lld_start(SerialDriver *sdp, const SerialConfig *config) {

  if (config == NULL)
    config = &default_config;

  if (sdp->state == SD_STOP) {
#if RL78_SERIAL_USE_UART1
    if (&SD2 == sdp) {
      SAU0EN = 1;
      STIF1 = 1;  /* Set INTST1 interrupt request flag */
      STMK1 = 1;  /* Disable INTST1 interrupt */
      STPR11 = 1;
      STPR01 = 1; /* Set INTST1 priority: 11b = 3 = lowest  */
    }
#endif
  }
  uart_init(sdp, config);
}

/**
 * @brief   Low level serial driver stop.
 * @details De-initializes the UART, stops the associated clock, resets the
 *          interrupt vector.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 *
 * @notapi
 */
void sd_lld_stop(SerialDriver *sdp) {

  if (sdp->state == SD_READY) {
    uart_deinit(sdp);
#if RL78_SERIAL_USE_UART1
    if (&SD2 == sdp) {
      return;
    }
#endif
  }
}

#endif /* HAL_USE_SERIAL */

/** @} */
