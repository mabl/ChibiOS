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

/**
 * @file    templates/serial_lld.c
 * @brief   Serial Driver subsystem low level driver source template.
 *
 * @addtogroup SERIAL
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_SERIAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

SerialDriver SD1;

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/**
 * @brief   Driver default configuration.
 */
static const SerialConfig default_config = {
  115200 /* default baud rate */
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static void output_notify(GenericQueue *qp) {
  UNUSED(qp);
  /* Enable tx interrupts.*/
  AUX_MU_IER_REG |= AUX_MU_IER_TX_IRQEN;
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

void sd_lld_serve_interrupt( SerialDriver *sdp ) {
  if (AUX_MU_IIR_RX_IRQ) {
    chSysLockFromIsr();
    while(!AUX_MU_LSR_RX_RDY);
    do {
      sdIncomingDataI(sdp, AUX_MU_IO_REG & 0xFF);
    } while (AUX_MU_LSR_RX_RDY);
    chSysUnlockFromIsr();
  }

  if (AUX_MU_IIR_TX_IRQ) {
    chSysLockFromIsr();
    while(!AUX_MU_LSR_TX_RDY);
    msg_t data = sdRequestDataI(sdp);
    if (data < Q_OK) {
      /* Disable tx interrupts.*/
      AUX_MU_IER_REG &= ~AUX_MU_IER_TX_IRQEN;
    }
    else {
      mini_uart_send((uint32_t)data);
    }
    chSysUnlockFromIsr();
  }
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level serial driver initialization.
 *
 * @notapi
 */
void sd_lld_init(void) {
  sdObjectInit(&SD1, NULL, output_notify);
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
  UNUSED(sdp);

  if (config == NULL)
    config = &default_config;

  IRQ_DISABLE1 = BIT(29);

  AUX_ENABLES = 1;

  AUX_MU_CNTL_REG = 0x00;    // disable tx and rx (+set other control bits to default)
  AUX_MU_IER_REG  &= ~0x03;  // disable rx tx interrupts
  AUX_MU_LCR_REG  |= 0x03;   // 8-bit mode
  AUX_MU_MCR_REG  = 0x00;    // set RTS high (default)
  AUX_MU_BAUD_REG = BAUD_RATE_COUNT(config->baud_rate);

  bcm2835_gpio_fnsel(14, GPFN_ALT5);
  bcm2835_gpio_fnsel(15, GPFN_ALT5);

  GPPUD = 0;
  bcm2835_delay(150);
  GPPUDCLK0 = (1<<14)|(1<<15);
  bcm2835_delay(150);
  GPPUDCLK0 = 0;

  AUX_MU_IER_REG |= 0x03;  // enable rx tx interrupts
  AUX_MU_IIR_REG |= 0x06;  // clear FIFOs
  AUX_MU_CNTL_REG = 0x03;  // enable tx and rx (+set other control bits to default)

  IRQ_ENABLE1 = BIT(29);
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
  UNUSED(sdp);

  IRQ_DISABLE1 |= BIT(29);
  bcm2835_gpio_fnsel(14, GPFN_IN);
  bcm2835_gpio_fnsel(15, GPFN_IN);
}

uint32_t mini_uart_recv ( void )
{
  while((AUX_MU_LSR_REG & 0x01) == 0);
  return(AUX_MU_IO_REG & 0xFF);
}

void mini_uart_send ( uint32_t c )
{
  while((AUX_MU_LSR_REG & 0x20) == 0);
  AUX_MU_IO_REG = c;
}

void mini_uart_sendstr (const char *s)
{
  char c;
  while((c = *s++)) mini_uart_send(c);
}

void mini_uart_sendhex ( uint32_t d, bool_t newline )
{
  uint32_t rb;
  uint32_t rc;

  rb=32;
  while(1)
  {
    rb-=4;
    rc=(d>>rb)&0xF;
    if(rc>9)
      rc+=0x37;
    else
      rc+=0x30;
    mini_uart_send(rc);
    if(rb==0) break;
  }

  mini_uart_send(0x20);

  if (newline) {
    mini_uart_send(0x0D);
    mini_uart_send(0x0A);
  }
}

#endif /* HAL_USE_SERIAL */

/** @} */
