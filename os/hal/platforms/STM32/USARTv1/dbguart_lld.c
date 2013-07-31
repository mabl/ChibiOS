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
 * @file    templates/uart_lld.c
 * @brief   UART Driver subsystem low level driver source template.
 *
 * @file    STM32/dbguart_lld.c
 * @brief   STM32 DBGUART subsystem low level driver source.
 *
 * @addtogroup DBGUART
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_DBGUART || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   DBGUART driver identifier.
 */
DBGUARTDriver DBGUARTD;

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level UART driver initialization.
 *
 * @notapi
 */
void dbguart_lld_init(void) {

#if STM32_DBGUART_USE_USART1
  dbguartObjectInit(&DBGUARTD);
  DBGUARTD.usart = USART1;
#endif

#if STM32_DBGUART_USE_USART2
  dbguartObjectInit(&DBGUARTD);
  DBGUARTD.usart = USART2;
#endif

#if STM32_DBGUART_USE_USART3
  dbguartObjectInit(&DBGUARTD);
  DBGUARTD.usart = USART3;
#endif

#if STM32_DBGUART_USE_UART4
  dbguartObjectInit(&DBGUARTD);
  DBGUARTD.usart = UART4;
#endif

#if STM32_DBGUART_USE_UART5
  dbguartObjectInit(&DBGUARTD);
  DBGUARTD.usart = UART5;
#endif

#if STM32_DBGUART_USE_USART6
  dbguartObjectInit(&DBGUARTD);
  DBGUARTD.usart = USART1;
#endif
}

/**
 * @brief   Configures and activates the UART peripheral.
 *
 * @param[in] uartp      pointer to the @p UARTDriver object
 *
 * @notapi
 */
void dbguart_lld_start(DBGUARTDriver *uartp) {
  USART_TypeDef *u = uartp->usart;

#if STM32_DBGUART_USE_USART1
  rccEnableUSART1(false);
  rccResetUSART1();
#endif
#if STM32_DBGUART_USE_USART2
  rccEnableUSART2(false);
  rccResetUSART2();
#endif
#if STM32_DBGUART_USE_USART3
  rccEnableUSART3(false);
  rccResetUSART3();
#endif
#if STM32_DBGUART_USE_UART4
  rccEnableUART4(false);
  rccResetUART4();
#endif
#if STM32_DBGUART_USE_UART5
  rccEnableUART5(false);
  rccResetUART5();
#endif
#if STM32_DBGUART_USE_USART6
  rccEnableUSART5(false);
  rccResetUSART6();
#endif

  /* Stop all operation */
  u->CR1 = 0;
  u->CR2 = 0;
  u->CR3 = 0;

  /* Baud rate setting.*/
#if STM32_HAS_USART6
  if ((uartp->usart == USART1) || (uartp->usart == USART6))
#else
  if (uartp->usart == USART1)
#endif
    u->BRR = STM32_PCLK2 / DBGUART_BITRATE;
  else
    u->BRR = STM32_PCLK1 / DBGUART_BITRATE;

  /* Resetting eventual pending status flags.*/
  (void)u->SR;  /* SR reset step 1.*/
  (void)u->DR;  /* SR reset step 2.*/
  u->SR = 0;

  u->CR1 = USART_CR1_UE | USART_CR1_TE;
}

/**
 * @brief   Starts a transmission on the UART peripheral.
 * @note    The buffers are organized as uint8_t arrays for data sizes below
 *          or equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 * @param[in] n         number of data frames to send
 * @param[in] txbuf     the pointer to the transmit buffer
 *
 * @notapi
 */
void dbguart_lld_send(DBGUARTDriver *uartp, size_t n, const void *txbuf) {

  USART_TypeDef *u = uartp->usart;
  const uint8_t *buf = txbuf;

  while (n--) {
    u->DR = *buf++;
    while (!(u->SR & USART_SR_TXE)) {;}
  }

  /* wait until last byte writing to wire */
  while (!(u->SR & USART_SR_TC)) {;}
}

#endif /* HAL_USE_DBGUART */

/** @} */
