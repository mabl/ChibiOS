/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file STM32/mac_lld.c
 * @brief STM32 low level MAC driver code.
 * @addtogroup STM32_MAC
 * @{
 */

#include <string.h>

#include "ch.h"
#include "hal.h"
#include "stm32_eth.h"

#if CH_HAL_USE_MAC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief Ethernet driver 1.
 */
MACDriver ETH1;

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

#ifndef __DOXYGEN__
static bool_t link_up;

static uint8_t default_mac[] = {0xAA, 0x55, 0x13, 0x37, 0x01, 0x10};

#define MAC_RX_DESCRIPTORS  4
#define MAC_TX_DESCRIPTORS  2
#define MAC_RECEIVE_BUFFERS_SIZE (MAC_BUFFERS_SIZE)
#define MAC_TRANSMIT_BUFFERS_SIZE (MAC_BUFFERS_SIZE)

#define PHY_ADDRESS  0x01

/* Descriptors */
static ETH_DMADESCTypeDef rd[MAC_RX_DESCRIPTORS] __attribute__((aligned(4)));
static ETH_DMADESCTypeDef td[MAC_TX_DESCRIPTORS] __attribute__((aligned(4)));

/* Buffers */
static uint8_t rb[MAC_RX_DESCRIPTORS * MAC_RECEIVE_BUFFERS_SIZE] __attribute__((aligned(4)));
static uint8_t tb[MAC_TX_DESCRIPTORS * MAC_TRANSMIT_BUFFERS_SIZE] __attribute__((aligned(4)));

/* Defined in ST ETH library: */
extern ETH_DMADESCTypeDef  *DMATxDescToSet;
extern ETH_DMADESCTypeDef  *DMARxDescToGet;

#endif

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/
static void configure_eth_struct(void) {
  static ETH_InitTypeDef ethinit;

  /* Call this to avoid having to configure all members */
  ETH_StructInit(&ethinit);

  /* MAC */
  ethinit.ETH_AutoNegotiation = ETH_AutoNegotiation_Enable;
  ethinit.ETH_LoopbackMode = ETH_LoopbackMode_Disable;
  ethinit.ETH_RetryTransmission = ETH_RetryTransmission_Disable;
  ethinit.ETH_AutomaticPadCRCStrip = ETH_AutomaticPadCRCStrip_Disable;
  ethinit.ETH_ReceiveAll = ETH_ReceiveAll_Disable;
  ethinit.ETH_BroadcastFramesReception = ETH_BroadcastFramesReception_Enable;
  ethinit.ETH_PromiscuousMode = ETH_PromiscuousMode_Disable;
  ethinit.ETH_MulticastFramesFilter = ETH_MulticastFramesFilter_Perfect;
  ethinit.ETH_UnicastFramesFilter = ETH_UnicastFramesFilter_Perfect;
  ethinit.ETH_ChecksumOffload = ETH_ChecksumOffload_Enable;

  /* DMA */
  /* Use store & forward mode because checksum offloading is enabled */
  ethinit.ETH_DropTCPIPChecksumErrorFrame = ETH_DropTCPIPChecksumErrorFrame_Enable;
  ethinit.ETH_ReceiveStoreForward = ETH_ReceiveStoreForward_Enable;
  ethinit.ETH_TransmitStoreForward = ETH_TransmitStoreForward_Enable;

  ethinit.ETH_ForwardErrorFrames = ETH_ForwardErrorFrames_Disable;
  ethinit.ETH_ForwardUndersizedGoodFrames = ETH_ForwardUndersizedGoodFrames_Disable;
  ethinit.ETH_SecondFrameOperate = ETH_SecondFrameOperate_Enable;
  ethinit.ETH_AddressAlignedBeats = ETH_AddressAlignedBeats_Enable;
  ethinit.ETH_FixedBurst = ETH_FixedBurst_Enable;
  ethinit.ETH_RxDMABurstLength = ETH_RxDMABurstLength_32Beat;
  ethinit.ETH_TxDMABurstLength = ETH_TxDMABurstLength_32Beat;
  ethinit.ETH_DMAArbitration = ETH_DMAArbitration_RoundRobin_RxTx_2_1;

  /* Configure Ethernet Peripheral */
  link_up = ETH_Init(&ethinit, PHY_ADDRESS);
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/**
 * @brief Ethernet Global Interrupt IRQ handler.
 */
CH_IRQ_HANDLER(Vector134) {
  uint32_t dmasr;

  CH_IRQ_PROLOGUE();

  dmasr = ETH->DMASR;
  ETH->DMASR = dmasr; /* Clear status bits */

  if (dmasr & ETH_DMA_IT_R) {
    /* Data Received */
    chSysLockFromIsr();
    chSemResetI(&ETH1.md_rdsem, 0);
#if CH_USE_EVENTS
    chEvtBroadcastI(&ETH1.md_rdevent);
#endif
    chSysUnlockFromIsr();
  }

  if (dmasr & ETH_DMA_IT_T) {
    /* Data Transmitted */
    chSysLockFromIsr();
    chSemResetI(&ETH1.md_tdsem, 0);
    chSysUnlockFromIsr();
  }

  CH_IRQ_EPILOGUE();
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief Low level MAC initialization.
 */
void mac_lld_init(void) {

  macObjectInit(&ETH1);

  /* Enable Ethernet Peripheral Clock */
  RCC->AHBENR |= RCC_AHBENR_ETHMACEN | RCC_AHBENR_ETHMACTXEN | RCC_AHBENR_ETHMACRXEN;

  /* Reset Ethernet on AHB Bus */
  ETH_DeInit();

  /* Use MII Mode */
  AFIO->MAPR &= ~(AFIO_MAPR_MII_RMII_SEL);

  ETH_SoftwareReset();
  while(ETH_GetSoftwareResetStatus() == SET);

  /* Set MAC Address */
  mac_lld_set_address(&ETH1, default_mac);

  /* Fills ethernet config struct and calls ETH_Init() */
  configure_eth_struct();

  /* Initialize Tx & Rx Descriptors list: Chain mode */
  ETH_DMATxDescChainInit(td, tb, MAC_TX_DESCRIPTORS);
  ETH_DMARxDescChainInit(rd, rb, MAC_RX_DESCRIPTORS);

  /* Enable Ethernet Rx Interrupt */
  ETH_DMAITConfig(ETH_DMA_IT_NIS | ETH_DMA_IT_R, ENABLE);
  int i;
  for (i = 0; i < MAC_RX_DESCRIPTORS; i++) {
    ETH_DMARxDescReceiveITConfig(&rd[i], ENABLE);
  }

  /* Enable checksum insertion for Tx frames */
  for (i = 0; i < MAC_TX_DESCRIPTORS; i++) {
    ETH_DMATxDescChecksumInsertionConfig(&td[i], ETH_DMATxDesc_ChecksumTCPUDPICMPFull);
  }

  /* Enable MAC and DMA transmission and reception */
  ETH_Start();

  /* Enable the Ethernet global interrupt */
  NVICEnableVector(ETH_IRQn,
      CORTEX_PRIORITY_MASK(STM32_ETH1_IRQ_PRIORITY));
}

/**
 * @brief Low level MAC address setup.
 *
 * @param[in] macp pointer to the @p MACDriver object
 * @param[in] p pointer to a six bytes buffer containing the MAC address. If
 *            this parameter is set to @p NULL then a system default MAC is
 *            used. The MAC address must be aligned with the most significant
 *            byte first.
 */
void mac_lld_set_address(MACDriver *macp, const uint8_t *p) {

  (void)macp;
  ETH_MACAddressConfig(ETH_MAC_Address0, (uint8_t*)p);
}

/**
 * @brief Returns a transmission descriptor.
 * @details One of the available transmission descriptors is locked and
 *          returned.
 *
 * @param[in] macp pointer to the @p MACDriver object
 * @param[out] tdp pointer to a @p MACTransmitDescriptor structure
 * @return The operation status.
 * @retval RDY_OK the descriptor was obtained.
 * @retval RDY_TIMEOUT descriptor not available.
 */
msg_t max_lld_get_transmit_descriptor(MACDriver *macp,
                                      MACTransmitDescriptor *tdp) {
  MACDescriptor *mdp;
  (void)macp;

  if (!link_up)
    return RDY_TIMEOUT;

  chSysLock();

  /* Get Current Tx buffer */
  mdp = DMATxDescToSet;

  /* Ensure that descriptor isn't owned by the Ethernet DMA */
  if ((mdp->Status & ETH_DMATxDesc_OWN) != (uint32_t)RESET) {
    chSysUnlock();
    return RDY_TIMEOUT;
  }

  //TODO: Buffer should be marked as "locked"

  /* Configure next Tx buffer (Chained mode) */
  DMATxDescToSet = (ETH_DMADESCTypeDef*)(DMATxDescToSet->Buffer2NextDescAddr);

  chSysUnlock();

  /* Set the buffer size and configuration */
  tdp->td_offset = 0;
  tdp->td_size = MAC_TRANSMIT_BUFFERS_SIZE;
  tdp->td_physdesc = mdp;
  return RDY_OK;
}

/**
 * @brief Writes to a transmit descriptor's stream.
 *
 * @param[in] tdp pointer to a @p MACTransmitDescriptor structure
 * @param[in] buf pointer to the buffer containing the data to be written
 * @param[in] size number of bytes to be written
 * @return The number of bytes written into the descriptor's stream, this
 *         value can be less than the amount specified in the parameter
 *         @p size if the maximum frame size is reached.
 */
size_t mac_lld_write_transmit_descriptor(MACTransmitDescriptor *tdp,
                                         uint8_t *buf,
                                         size_t size) {

  chDbgAssert(!(tdp->td_physdesc->Status & ETH_DMARxDesc_OWN),
      "mac_lld_release_receive_descriptor(), #1",
      "attempt to write descriptor already owned by DMA");

  if (size > tdp->td_size - tdp->td_offset)
    size = tdp->td_size - tdp->td_offset;
  if (size > 0) {
    memcpy((uint8_t *)(tdp->td_physdesc->Buffer1Addr) +
        tdp->td_offset,
        buf, size);
    tdp->td_offset += size;
  }
  return size;
}

/**
 * @brief Releases a transmit descriptor and starts the transmission of the
 *        enqueued data as a single frame.
 *
 * @param[in] tdp the pointer to the @p MACTransmitDescriptor structure
 */
void mac_lld_release_transmit_descriptor(MACTransmitDescriptor *tdp) {
  chDbgAssert(!(tdp->td_physdesc->Status & ETH_DMARxDesc_OWN),
      "mac_lld_release_receive_descriptor(), #1",
      "attempt to release descriptor already owned by DMA");

  chSysLock();

  /* Set frame length: bits[12:0] */
  tdp->td_physdesc->ControlBufferSize = (tdp->td_offset & ETH_DMATxDesc_TBS1);

  /* Set the last segment and first segment bits (frame xmit in one descriptor) */
  tdp->td_physdesc->Status |= ETH_DMATxDesc_LS | ETH_DMATxDesc_FS;

  /* Enable interrupt on completion */
  tdp->td_physdesc->Status |= ETH_DMATxDesc_IC;

  /* Give buffer back to the DMA */
  tdp->td_physdesc->Status |= ETH_DMATxDesc_OWN;

  /* Clear unavailable flag & resume transmission */
  //TODO: the following check for DMASR_TBUS doesn't work
//if ((ETH->DMASR & ETH_DMASR_TBUS) != (uint32_t)RESET) {
    ETH->DMASR = ETH_DMASR_TBUS;
    ETH->DMATPDR = 0;
//}
  chSysUnlock();
}

/**
 * @brief Returns a receive descriptor.
 *
 * @param[in] macp pointer to the @p MACDriver object
 * @param[out] rdp pointer to a @p MACReceiveDescriptor structure
 * @return The operation status.
 * @retval RDY_OK the descriptor was obtained.
 * @retval RDY_TIMEOUT descriptor not available.
 */
msg_t max_lld_get_receive_descriptor(MACDriver *macp,
                                     MACReceiveDescriptor *rdp) {
  MACDescriptor *mdp;
  (void)macp;

  chSysLock();

  /* Get current Rx Buffer */
  mdp = DMARxDescToGet;

  /* Check if owned by the Ethernet DMA */
  if ((mdp->Status & ETH_DMARxDesc_OWN) != (uint32_t)RESET) {
    //TODO: The following check for DMASR_RBUS doesn't seem to work
//  if ((ETH->DMASR & ETH_DMASR_RBUS) != (uint32_t)RESET) {
      /* Clear RBUS Ethernet DMA Flag */
      ETH->DMASR = ETH_DMASR_RBUS;
      /* Resume DMA Operation */
      ETH->DMARPDR = 0;
//  }
    /* Return error (owned by DMA) */
    chSysUnlock();
    return RDY_TIMEOUT;
  }

  if (((mdp->Status & ETH_DMARxDesc_ES) == (uint32_t)RESET) &&
      ((mdp->Status & ETH_DMARxDesc_LS) != (uint32_t)RESET) &&
      ((mdp->Status & ETH_DMARxDesc_FS) != (uint32_t)RESET))
  {
    rdp->rd_offset = 0;

    /* Frame Length of received packet - 4 bytes of CRC */
    rdp->rd_size = ((mdp->Status & ETH_DMARxDesc_FL) >> 16) - 4;

    rdp->rd_physdesc = mdp;

    /* Configure next Rx buffer (Chain mode) */
    DMARxDescToGet = (ETH_DMADESCTypeDef*)(DMARxDescToGet->Buffer2NextDescAddr);

    chSysUnlock();
    return RDY_OK;
  }

  chSysUnlock();
  return RDY_TIMEOUT;
}

/**
 * @brief Reads from a receive descriptor's stream.
 *
 * @param[in] rdp pointer to a @p MACReceiveDescriptor structure
 * @param[in] buf pointer to the buffer that will receive the read data
 * @param[in] size number of bytes to be read
 * @return The number of bytes read from the descriptor's stream, this
 *         value can be less than the amount specified in the parameter
 *         @p size if there are no more bytes to read.
 */
size_t mac_lld_read_receive_descriptor(MACReceiveDescriptor *rdp,
                                         uint8_t *buf,
                                         size_t size) {
  if (size > rdp->rd_size - rdp->rd_offset)
    size = rdp->rd_size - rdp->rd_offset;

  if (size > 0) {
    uint8_t *src = (uint8_t *)(rdp->rd_physdesc->Buffer1Addr) +
                   rdp->rd_offset;
    memcpy(buf, src, size);
    rdp->rd_offset += size;
  }

  return size;
}

/**
 * @brief Releases a receive descriptor.
 * @details The descriptor and its buffer are made available for more incoming
 *          frames.
 *
 * @param[in] rdp the pointer to the @p MACReceiveDescriptor structure
 */
void mac_lld_release_receive_descriptor(MACReceiveDescriptor *rdp) {
  MACDescriptor *mdp;

  mdp = rdp->rd_physdesc;
  chDbgAssert(!(mdp->Status & ETH_DMARxDesc_OWN),
          "mac_lld_release_receive_descriptor(), #1",
          "attempt to release descriptor already owned by DMA");

  /* Give buffer back to the Ethernet DMA */
  mdp->Status = ETH_DMARxDesc_OWN;

  //TODO: Is this even necessary?  DMA should resume when next frame is
  //      received automatically.
  if ((ETH->DMASR & ETH_DMASR_RBUS) != (uint32_t)RESET) {
    ETH->DMASR = ETH_DMASR_RBUS;
    ETH->DMARPDR = 0;
  }
}

/**
 * @brief Updates and returns the link status.
 *
 * @param[in] macp pointer to the @p MACDriver object
 * @return The link status.
 * @retval TRUE if the link is active.
 * @retval FALSE if the link is down.
 */
bool_t mac_lld_poll_link_status(MACDriver *macp) {
  (void)macp;
  //TODO: If link down, then attempt to bring link back up
  //NOTE: The ST libs attempt to bring link up within ETH_Init()
  return link_up;
}

#endif /* CH_HAL_USE_MAC */

/** @} */
