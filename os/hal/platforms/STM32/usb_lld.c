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

/**
 * @brief   EP0 initialization structure.
 */
const USBEndpointConfig usb_lld_ep0config = {
  _usb_ep0in,
  _usb_ep0out,
  0,
  0x40,
  EPR_EP_TYPE_CONTROL | EPR_STAT_TX_STALL | EPR_STAT_RX_VALID,
  0x40
};

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

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
  USBDriver *usbp = &USBD1;

  CH_IRQ_PROLOGUE();

  /*
   * USB bus reset condition handling.
   */
  istr = STM32_USB->ISTR;
  if (istr & ISTR_RESET) {
    _usb_reset(usbp);
    if (usbp->usb_config->uc_callback)
      usbp->usb_config->uc_callback(usbp, USB_EVENT_RESET);
    STM32_USB->ISTR = ~ISTR_RESET;
  }

  if (STM32_USB->DADDR != 0x80) {
    asm ("nop");
  }

  /* Endpoint events handling.*/
  while (istr & ISTR_CTR) {
    uint32_t ep;
    uint32_t epr = STM32_USB->EPR[ep = istr & ISTR_EP_ID_MASK];

    if (epr & EPR_CTR_TX) {
      /* IN endpoint, transmission.*/
      EPR_CLEAR_CTR_TX(ep);
      if (usbp->usb_epc[ep]->uepc_in_cb)
        usbp->usb_epc[ep]->uepc_in_cb(usbp, ep);
    }
    if (epr & EPR_CTR_RX) {
      /* OUT endpoint, receive.*/
      EPR_CLEAR_CTR_RX(ep);
      if (usbp->usb_epc[ep]->uepc_out_cb)
        usbp->usb_epc[ep]->uepc_out_cb(usbp, ep);
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

      /* Reset procedure enforced on driver start.*/
      _usb_reset(&USBD1);
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
 * @brief   USB low level reset routine.
 *
 * @param[in] usbp      pointer to the @p USBDriver object triggering the
 *                      callback
 *
 * @notapi
 */
void usb_lld_reset(USBDriver *usbp) {

  /* Powers up the transceiver while holding the USB in reset state.*/
  STM32_USB->CNTR   = CNTR_FRES;

  /* Releases the USB reset, BTABLE is reset to zero.*/
  STM32_USB->CNTR   = 0;
  STM32_USB->ISTR   = 0;
  STM32_USB->DADDR  = DADDR_EF;
  STM32_USB->CNTR   = /*CNTR_ESOFM | CNTR_SOFM |*/ CNTR_RESETM  | /*CNTR_SUSPM |*/
                          /*CNTR_WKUPM | CNTR_ERRM | CNTR_PMAOVRM |*/ CNTR_CTRM;
  usb_lld_ep_open(usbp, &usb_lld_ep0config);
}

/**
 * @brief   Sets the USB address.
 *
 * @param[in] usbp      pointer to the @p USBDriver object triggering the
 *                      callback
 * @param[in] addr      the USB address
 *
 * @notapi
 */
void usb_lld_set_address(USBDriver *usbp, uint8_t addr) {

  STM32_USB->DADDR = (uint32_t)addr | DADDR_EF;
}

/**
 * @brief   Activates an endpoint.
 *
 * @param[in] usbp      pointer to the @p USBDriver object triggering the
 *                      callback
 * @param[in] epcp      the endpoint configuration
 *
 * @notapi
 */
void usb_lld_ep_open(USBDriver *usbp, const USBEndpointConfig *epcp) {
  uint16_t nblocks;
  stm32_usb_descriptor_t *dp;

  /* EPxR register setup.*/
  EPR_SET(epcp->uepc_addr, epcp->uepc_epr | epcp->uepc_addr);
  EPR_TOGGLE(epcp->uepc_addr, epcp->uepc_epr);

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

  /* Logically enabling the endpoint in the USBDriver structure.*/
  usbp->usb_epc[epcp->uepc_addr] = epcp;
}

/**
 * @brief   Endpoint read.
 * @details The buffered packet is copied into the user buffer and then
 *          the endpoint is brought to the valid state in order to allow
 *          reception of more data.
 *
 * @param[in] usbp      pointer to the @p USBDriver object triggering the
 *                      callback
 * @param[in] ep        endpoint number
 * @param[in] buf       buffer where to copy the endpoint data
 * @param[in] n         maximum number of bytes to copy
 * @return              The number of bytes that were effectively available.
 *
 * @notapi
 */
size_t usb_lld_read(USBDriver *usbp, uint32_t ep, uint8_t *buf, size_t n) {
  uint32_t *pmap;
  stm32_usb_descriptor_t *udp;
  size_t count;

  udp = USB_GET_DESCRIPTOR(ep);
  pmap = USB_ADDR2PTR(udp->RXADDR);
  count = udp->RXCOUNT & RXCOUNT_COUNT_MASK;
  if (n > count)
    n = count;
  count = (n + 1) / 2;
  while (count) {
    *(uint16_t *)buf = (uint16_t)*pmap++;
    buf += 2;
    count--;
  }
  EPR_SET_STAT_RX(ep, EPR_STAT_RX_VALID);
  return n;
}

/**
 * @brief   Endpoint write.
 * @details The user data is copied in the packer memory and then
 *          the endpoint is brought to the valid state in order to allow
 *          transmission.
 *
 * @param[in] usbp      pointer to the @p USBDriver object triggering the
 *                      callback
 * @param[in] ep        endpoint number
 * @param[in] buf       buffer where to copy the endpoint data
 * @param[in] n         maximum number of bytes to copy
 * @return              The number of bytes that were effectively available.
 *
 * @notapi
 */
void usb_lld_write(USBDriver *usbp, uint32_t ep, const uint8_t *buf, size_t n) {
  uint32_t *pmap;
  stm32_usb_descriptor_t *udp;
  size_t count;

  udp = USB_GET_DESCRIPTOR(ep);
  pmap = USB_ADDR2PTR(udp->TXADDR);
  udp->TXCOUNT = n;
  count = (n + 1) / 2;
  while (count) {
    *pmap++ = *(uint16_t *)buf;
    buf += 2;
    count--;
  }
  EPR_SET_STAT_TX(ep, EPR_STAT_TX_VALID);
}

/**
 * @brief   Brings an IN endpoint in the stalled state.
 *
 * @param[in] usbp      pointer to the @p USBDriver object triggering the
 *                      callback
 * @param[in] ep        endpoint number, always zero
 *
 * @notapi
 */
void usb_lld_stall_in(USBDriver *usbp, uint32_t ep) {

  EPR_SET_STAT_TX(ep, EPR_STAT_TX_STALL);
}

/**
 * @brief   Brings an OUT endpoint in the stalled state.
 *
 * @param[in] usbp      pointer to the @p USBDriver object triggering the
 *                      callback
 * @param[in] ep        endpoint number, always zero
 *
 * @notapi
 */
void usb_lld_stall_out(USBDriver *usbp, uint32_t ep) {

  EPR_SET_STAT_RX(ep, EPR_STAT_RX_STALL);
}

#endif /* HAL_USE_USB */

/** @} */
