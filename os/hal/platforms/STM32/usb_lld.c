/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

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
 * @file    templates/usb_lld.c
 * @brief   USB Driver subsystem low level driver source template.
 *
 * @addtogroup USB
 * @{
 */

#include "ch.h"
#include "hal.h"
#include "usb.h"

#if HAL_USE_USB || defined(__DOXYGEN__)

#define BTABLE_ADDR     0x0000
#define EP0IN_ADDR      0x0040
#define EP0OUT_ADDR     0x00C0

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief USB1 driver identifier.*/
#if STM32_USB_USE_USB1 || defined(__DOXYGEN__)
USBDriver USBD1;
#endif

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

static USBEndpoint ep0;

static const USBEndpointConfig ep0config = {
  0,
  0x40,
  EPR_EP_TYPE_CONTROL,
  EPR_STAT_TX_STALL,
  EPR_STAT_RX_VALID,
  0x40
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static void usb_reset(USBDriver *usbp) {
  int32_t i;

  /* Invalidates all endpoints into the USBDriver structure.*/
  i = USB_ENDOPOINTS_NUMBER + 1;
  do {
    usbp->usb_ep[--i] = NULL;
  } while (i > 0);

  /* Powers up the transceiver while holding the USB in reset state.*/
  STM32_USB->CNTR   = CNTR_FRES;

  /* Releases the USB reset, BTABLE is reset to zero.*/
  STM32_USB->CNTR   = 0;
  STM32_USB->ISTR   = 0;
  STM32_USB->DADDR  = DADDR_EF;
  STM32_USB->CNTR   = /*CNTR_ESOFM | CNTR_SOFM |*/ CNTR_RESETM  | /*CNTR_SUSPM |*/
                          /*CNTR_WKUPM | CNTR_ERRM | CNTR_PMAOVRM |*/ CNTR_CTRM;
  usb_lld_ep_open(usbp, &ep0config, &ep0, NULL);
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if STM32_USB_USE_USB1 || defined(__DOXYGEN__)
/**
 * @brief   USB high priority interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(USB_HP_IRQHandler) {

  CH_IRQ_PROLOGUE();

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   USB low priority interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(USB_LP_IRQHandler) {
  uint32_t istr;
  uint32_t id;
  USBDriver *usbp = &USBD1;

  CH_IRQ_PROLOGUE();

  /*
   * USB bus reset condition handling.
   */
  istr = STM32_USB->ISTR;
  if (istr & ISTR_RESET) {
    usb_reset(usbp);
    if (usbp->usb_config->uc_callback)
      usbp->usb_config->uc_callback(usbp, USB_EVENT_RESET);
    STM32_USB->ISTR = ~ISTR_RESET;
  }

  /* Endpoint events handling.*/
  while (istr & ISTR_CTR) {
    uint32_t ep;
    uint32_t epr = STM32_USB->EPR[ep = istr & ISTR_EP_ID_MASK]; 

    if (epr & EPR_CTR_TX) {
      /* IN endpoint, transmission.*/

      STM32_USB->EPR[ep] = ~EPR_CTR_TX;
    }
    if (epr & EPR_CTR_RX) {
      /* OUT endpoint, receive.*/

      STM32_USB->EPR[ep] = ~EPR_CTR_RX;
    }
    istr = STM32_USB->ISTR;
  }

  CH_IRQ_EPILOGUE();
}
#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level USB driver initialization.
 *
 * @notapi
 */
void usb_lld_init(void) {
  
  /* USB reset, ensures reset state in order to avoid trouble with JTAGs.*/
  RCC->APB1RSTR = RCC_APB1RSTR_USBRST;
  RCC->APB1RSTR = 0;

  /* Driver initialization.*/
  usbObjectInit(&USBD1);
}

/**
 * @brief   Configures and activates the USB peripheral.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 *
 * @notapi
 */
void usb_lld_start(USBDriver *usbp) {

  if (usbp->usb_state == USB_STOP) {
    /* Clock activation.*/
#if STM32_USB_USE_USB1
    if (&USBD1 == usbp) {
      /* USB clock enabled.*/
      RCC->APB1ENR |= RCC_APB1ENR_USBEN;
      /* Powers up the transceiver while holding the USB in reset state.*/
      STM32_USB->CNTR = CNTR_FRES;
      /* Enabling the USB IRQ vectors, this also gives enough time to allow
         the transceiver power up (1uS).*/
      NVICEnableVector(USB_HP_CAN1_TX_IRQn,
                       CORTEX_PRIORITY_MASK(STM32_USB_USB1_HP_IRQ_PRIORITY));
      NVICEnableVector(USB_LP_CAN1_RX0_IRQn,
                       CORTEX_PRIORITY_MASK(STM32_USB_USB1_LP_IRQ_PRIORITY));
      /* Releases the reset, then enables the required IRQ sources.*/
      usb_reset(&USBD1);
      STM32_USB->ISTR = 0;
      STM32_USB->CNTR = /*CNTR_ESOFM | CNTR_SOFM |*/ CNTR_RESETM  | /*CNTR_SUSPM |*/
                        /*CNTR_WKUPM | CNTR_ERRM | CNTR_PMAOVRM |*/ CNTR_CTRM;
    }
#endif
  }
  /* Configuration.*/
}

/**
 * @brief   Deactivates the USB peripheral.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 *
 * @notapi
 */
void usb_lld_stop(USBDriver *usbp) {

  /* If in ready state then disables the USB clock.*/
  if (usbp->usb_state == USB_STOP) {
#if STM32_ADC_USE_ADC1
    if (&USBD1 == usbp) {
      NVICDisableVector(USB_HP_CAN1_TX_IRQn);
      NVICDisableVector(USB_LP_CAN1_RX0_IRQn);
      RCC->APB1ENR &= ~RCC_APB1ENR_USBEN;
    }
#endif
  }
}

/**
 * @brief   Activates an endpoint.
 */
void usb_lld_ep_open(USBDriver *usbp, const USBEndpointConfig *epcp,
                     USBEndpoint *epp, usbepcallback_t epcb) {
  uint16_t nblocks;
  stm32_usb_descriptor_t *dp;
 
  /* EPxR register setup.*/
  SET_EPR(epcp->uepc_addr, epcp->uepc_epr | epcp->uepc_addr);
  SET_EPR_TOGGLE(epcp->uepc_addr, epcp->uepc_stattx);
  SET_EPR_TOGGLE(epcp->uepc_addr, epcp->uepc_statrx);
  
  /* Endpoint size and address initialization.*/
  if (epcp->uepc_size > 62) 
    nblocks = (((((epcp->uepc_size - 1) | 0x1f) + 1) / 32) << 10) | 0x8000;
  else 
    nblocks = ((((epcp->uepc_size - 1) | 1) + 1) / 2) << 10;
  dp = USB_GET_DESCRIPTOR(epcp->uepc_addr);
  dp->TXCOUNT = 0;
  dp->RXCOUNT = nblocks;
  dp->RXADDR  = epcp->uepc_offset;
  dp->TXADDR  = epcp->uepc_offset + epcp->uepc_size; 

  /* Endpoint callback setup.*/
  epp->uep_callback = epcb;

  /* Logically enabling the endpoint in the USBDriver structure.*/
  usbp->usb_ep[epcp->uepc_addr] = epp;
}

#endif /* HAL_USE_USB */

/** @} */
