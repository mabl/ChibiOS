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

/**
 * @file    STM8S/serial_lld.c
 * @brief   STM8S low level serial driver code.
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

/**
 * @brief   UART1 serial driver identifier.
 */
#if STM8S_SERIAL_USE_UART1 || defined(__DOXYGEN__)
SerialDriver SD1;
#endif

/**
 * @brief   UART2 serial driver identifier.
 */
#if STM8S_SERIAL_USE_UART2 || defined(__DOXYGEN__)
SerialDriver SD2;
#endif

/**
 * @brief   UART3 serial driver identifier.
 */
#if STM8S_SERIAL_USE_UART3 || defined(__DOXYGEN__)
SerialDriver SD3;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/**
 * @brief   Driver default configuration.
 */
static ROMCONST SerialConfig default_config = {
  BRR(SERIAL_DEFAULT_BITRATE),
  SD_MODE_PARITY_NONE | SD_MODE_STOP_1
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static void set_error(SerialDriver *sdp, uint8_t sr) {
  flagsmask_t sts = 0;

  /* Note, SR register bit definitions are equal for all UARTs so using
     the UART1 definitions is fine.*/
  if (sr & UART1_SR_OR)
    sts |= SD_OVERRUN_ERROR;
  if (sr & UART1_SR_NF)
    sts |= SD_NOISE_ERROR;
  if (sr & UART1_SR_FE)
    sts |= SD_FRAMING_ERROR;
  if (sr & UART1_SR_PE)
    sts |= SD_PARITY_ERROR;
  chSysLockFromIsr();
  chnAddFlagsI(sdp, sts);
  chSysUnlockFromIsr();
}

#if STM8S_SERIAL_USE_UART1 || defined(__DOXYGEN__)
static void notify1(GenericQueue *qp) {

  (void)qp;
  UART1->CR2 |= UART1_CR2_TIEN;
}

/**
 * @brief   UART1 initialization.
 *
 * @param[in] config    architecture-dependent serial driver configuration
 */
static void uart1_init(const SerialConfig *config) {

  UART1->BRR2 = (uint8_t)(((uint8_t)(config->sc_brr >> 8) & (uint8_t)0xF0) |
                          ((uint8_t)config->sc_brr & (uint8_t)0x0F));
  UART1->BRR1 = (uint8_t)(config->sc_brr >> 4);
  UART1->CR1  = (uint8_t)(config->sc_mode &
                          SD_MODE_PARITY);  /* PIEN included.               */
  UART1->CR2  = UART1_CR2_RIEN | UART1_CR2_TEN | UART1_CR2_REN;
  UART1->CR3  = (uint8_t)(config->sc_mode & SD_MODE_STOP);
  UART1->CR4  = 0;
  UART1->CR5  = 0;
  UART1->PSCR = 1;
  (void)UART1->SR;
  (void)UART1->DR;
}

/**
 * @brief   UART1 de-initialization.
 */
static void uart1_deinit(void) {

  UART1->CR1  = UART1_CR1_UARTD;
  UART1->CR2  = 0;
  UART1->CR3  = 0;
  UART1->CR4  = 0;
  UART1->CR5  = 0;
  UART1->PSCR = 0;
}
#endif /* STM8S_SERIAL_USE_UART1 */

#if STM8S_SERIAL_USE_UART2 || defined(__DOXYGEN__)
static void notify2(GenericQueue *qp) {

  (void)qp;
  UART2->CR2 |= UART2_CR2_TIEN;
}

/**
 * @brief   UART2 initialization.
 *
 * @param[in] config    architecture-dependent serial driver configuration
 */
static void uart2_init(const SerialConfig *config) {

  UART2->BRR2 = (uint8_t)(((uint8_t)(config->sc_brr >> 8) & (uint8_t)0xF0) |
                          ((uint8_t)config->sc_brr & (uint8_t)0x0F));
  UART2->BRR1 = (uint8_t)(config->sc_brr >> 4);
  UART2->CR1  = (uint8_t)(config->sc_mode &
                          SD_MODE_PARITY);  /* PIEN included.               */
  UART2->CR2  = UART2_CR2_RIEN | UART2_CR2_TEN | UART2_CR2_REN;
  UART2->CR3  = (uint8_t)(config->sc_mode & SD_MODE_STOP);
  UART2->CR4  = 0;
  UART2->CR5  = 0;
  UART2->CR6  = 0;
  UART2->PSCR = 1;
  (void)UART2->SR;
  (void)UART2->DR;
}

/**
 * @brief   UART1 de-initialization.
 */
static void uart2_deinit(void) {

  UART2->CR1  = UART2_CR1_UARTD;
  UART2->CR2  = 0;
  UART2->CR3  = 0;
  UART2->CR4  = 0;
  UART2->CR5  = 0;
  UART2->CR6  = 0;
  UART2->PSCR = 0;
}
#endif /* STM8S_SERIAL_USE_UART1 */

#if STM8S_SERIAL_USE_UART3 || defined(__DOXYGEN__)
static void notify3(GenericQueue *qp) {

  (void)qp;
  UART3->CR2 |= UART3_CR2_TIEN;
}

/**
 * @brief   UART3 initialization.
 *
 * @param[in] config    architecture-dependent serial driver configuration
 */
static void uart3_init(const SerialConfig *config) {

  UART3->BRR2 = (uint8_t)(((uint8_t)(config->sc_brr >> 8) & (uint8_t)0xF0) |
                          ((uint8_t)config->sc_brr & (uint8_t)0x0F));
  UART3->BRR1 = (uint8_t)(config->sc_brr >> 4);
  UART3->CR1  = (uint8_t)(config->sc_mode &
                          SD_MODE_PARITY);  /* PIEN included.               */
  UART3->CR2  = UART3_CR2_RIEN | UART3_CR2_TEN | UART3_CR2_REN;
  UART3->CR3  = (uint8_t)(config->sc_mode & SD_MODE_STOP);
  UART3->CR4  = 0;
  UART3->CR6  = 0;
  (void)UART3->SR;
  (void)UART3->DR;
}

/**
 * @brief   UART3 de-initialization.
 */
static void uart3_deinit(void) {

  UART3->CR1  = UART3_CR1_UARTD;
  UART3->CR2  = 0;
  UART3->CR3  = 0;
  UART3->CR4  = 0;
  UART3->CR6  = 0;
}
#endif /* STM8S_SERIAL_USE_UART3 */

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if STM8S_SERIAL_USE_UART1 || defined(__DOXYGEN__)
/**
 * @brief   IRQ 17 service routine.
 *
 * @isr
 */
CH_IRQ_HANDLER(17) {
  msg_t b;

  CH_IRQ_PROLOGUE();

  chSysLockFromIsr();
  b = sdRequestDataI(&SD1);
  chSysUnlockFromIsr();
  if (b < Q_OK)
    UART1->CR2 &= (uint8_t)~UART1_CR2_TIEN;
  else
    UART1->DR = (uint8_t)b;

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   IRQ 18 service routine.
 *
 * @isr
 */
CH_IRQ_HANDLER(18) {
  uint8_t sr = UART1->SR;

  CH_IRQ_PROLOGUE();

  if ((sr = UART1->SR) & (UART1_SR_OR | UART1_SR_NF |
                          UART1_SR_FE | UART1_SR_PE))
    set_error(&SD1, sr);
  chSysLockFromIsr();
  sdIncomingDataI(&SD1, UART1->DR);
  chSysUnlockFromIsr();

  CH_IRQ_EPILOGUE();
}
#endif /* STM8S_SERIAL_USE_UART1 */

#if STM8S_SERIAL_USE_UART2 || defined(__DOXYGEN__)
/**
 * @brief   IRQ 20 service routine.
 *
 * @isr
 */
CH_IRQ_HANDLER(20) {
  msg_t b;

  CH_IRQ_PROLOGUE();

  chSysLockFromIsr();
  b = sdRequestDataI(&SD2);
  chSysUnlockFromIsr();
  if (b < Q_OK)
    UART2->CR2 &= (uint8_t)~UART2_CR2_TIEN;
  else
    UART2->DR = (uint8_t)b;

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   IRQ 21 service routine.
 *
 * @isr
 */
CH_IRQ_HANDLER(21) {
  uint8_t sr = UART2->SR;

  CH_IRQ_PROLOGUE();

  if ((sr = UART2->SR) & (UART2_SR_OR | UART2_SR_NF |
                          UART2_SR_FE | UART2_SR_PE))
    set_error(&SD2, sr);
  chSysLockFromIsr();
  sdIncomingDataI(&SD2, UART2->DR);
  chSysUnlockFromIsr();

  CH_IRQ_EPILOGUE();
}
#endif /* STM8S_SERIAL_USE_UART2 */

#if STM8S_SERIAL_USE_UART3 || defined(__DOXYGEN__)
/**
 * @brief   IRQ 20 service routine.
 *
 * @isr
 */
CH_IRQ_HANDLER(20) {
  msg_t b;

  CH_IRQ_PROLOGUE();

  chSysLockFromIsr();
  b = sdRequestDataI(&SD3);
  chSysUnlockFromIsr();
  if (b < Q_OK)
    UART3->CR2 &= (uint8_t)~UART3_CR2_TIEN;
  else
    UART3->DR = (uint8_t)b;

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   IRQ 21 service routine.
 *
 * @isr
 */
CH_IRQ_HANDLER(21) {
  uint8_t sr = UART3->SR;

  CH_IRQ_PROLOGUE();

  if ((sr = UART3->SR) & (UART3_SR_OR | UART3_SR_NF |
                          UART3_SR_FE | UART3_SR_PE))
    set_error(&SD3, sr);
  chSysLockFromIsr();
  sdIncomingDataI(&SD3, UART3->DR);
  chSysUnlockFromIsr();

  CH_IRQ_EPILOGUE();
}
#endif /* STM8S_SERIAL_USE_UART3 */

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level serial driver initialization.
 *
 * @notapi
 */
void sd_lld_init(void) {

#if STM8S_SERIAL_USE_UART1
  sdObjectInit(&SD1, NULL, notify1);
  CLK->PCKENR1 |= CLK_PCKENR1_UART1;        /* PCKEN12, clock source.       */
  UART1->CR1 = UART1_CR1_UARTD;             /* UARTD (low power).           */
#endif

#if STM8S_SERIAL_USE_UART2
  sdObjectInit(&SD2, NULL, notify2);
  CLK->PCKENR1 |= CLK_PCKENR1_UART2;        /* PCKEN13, clock source.       */
  UART2->CR1 = UART2_CR1_UARTD;             /* UARTD (low power).           */
#endif

#if STM8S_SERIAL_USE_UART3
  sdObjectInit(&SD3, NULL, notify3);
  CLK->PCKENR1 |= CLK_PCKENR1_UART3;        /* PCKEN13, clock source.       */
  UART3->CR1 = UART3_CR1_UARTD;             /* UARTD (low power).           */
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

#if STM8S_SERIAL_USE_UART1
  if (&SD1 == sdp) {
    uart1_init(config);
    return;
  }
#endif
#if STM8S_SERIAL_USE_UART2
  if (&SD2 == sdp) {
    uart2_init(config);
    return;
  }
#endif
#if STM8S_SERIAL_USE_UART3
  if (&SD3 == sdp) {
    uart3_init(config);
    return;
  }
#endif
}

/**
 * @brief   Low level serial driver stop.
 * @details De-initializes the USART, stops the associated clock, resets the
 *          interrupt vector.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 *
 * @notapi
 */
void sd_lld_stop(SerialDriver *sdp) {

#if STM8S_SERIAL_USE_UART1
  if (&SD1 == sdp) {
    uart1_deinit();
    return;
  }
#endif
#if STM8S_SERIAL_USE_UART2
  if (&SD2 == sdp) {
    uart2_deinit();
    return;
  }
#endif
#if STM8S_SERIAL_USE_UART3
  if (&SD3 == sdp) {
    uart3_deinit();
    return;
  }
#endif
}

#endif /* HAL_USE_SERIAL */

/** @} */
