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
 * @file H8S/mac_lld.c
 * @brief H8S and CS8900A MAC driver code.
 * @addtogroup H8S_CS8900A_MAC
 * @{
 */

#include <string.h>

#include "ch.h"
#include "hal.h"

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

static uint8_t default_mac[] = {0xAA, 0x55, 0x13, 0x37, 0x10, 0x10};

static uint8_t *rxfirst;                ///< Pointer to first allocated rx buffer
static uint8_t *rxfree;                 ///< Pointer to first free rx buffer
static uint8_t *rxcurrent;              ///< Pointer to selected rx buffer
static volatile uint16_t rxcount;       ///< Allocated rx buffers count
static uint8_t rb[MAC_RECEIVE_BUFFERS * MAC_RECEIVE_BUFFERS_SIZE]
    __attribute__((aligned(4)));

static uint16_t tb[(MAC_TRANSMIT_BUFFERS_SIZE+1)/2];
#endif

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief Gets CS8900A register value.
 *
 * @note Interrupts must be disabled, because this function is called from irq.
 * @param[in] reg register offset
 * @return Register value.
 */
uint16_t cs8900a_readreg(uint16_t reg) {

  CS8900A_WRITE(ADD_PORT, reg);
  return CS8900A_READ(DATA_PORT);
}

/**
 * @brief Sets CS8900A register value.
 *
 * @note Interrupts must be disabled, because this function is called from irq.
 * @param[in] reg register offset
 * @param[in] value register value
 */
void cs8900a_writereg(uint16_t reg, uint16_t value) {

  CS8900A_WRITE(ADD_PORT, reg);
  CS8900A_WRITE(DATA_PORT, value);
}

/**
 * @brief Configures CS8900A internal registers.
 */
void cs8900a_init(void) {

  cs8900a_writereg(PP_SelfCTL, POWER_ON_RESET);
  // TODO: maybe add some delay here
  while (cs8900a_readreg(PP_SelfST) == 0); // TODO: protect against endless loop
  cs8900a_writereg(PP_RxCFG, RX_OK_ENBL);
//  cs8900a_writereg(PP_RxCFG, RX_OK_ENBL | RX_CRC_ERROR_ENBL | RX_RUNT_ENBL | RX_EXTRA_DATA_ENBL); // TEST

  cs8900a_writereg(PP_TxCFG, TX_OK_ENBL | TX_SQE_ERROR_ENBL | TX_LATE_COL_ENBL | TX_JBR_ENBL | TX_16_COL_ENBL);
  cs8900a_writereg(PP_LineCTL, SERIAL_RX_ON | SERIAL_TX_ON);
  cs8900a_writereg(PP_RxCTL, RX_OK_ACCEPT | RX_IA_ACCEPT | RX_BROADCAST_ACCEPT);
//  cs8900a_writereg(PP_RxCTL, RX_OK_ACCEPT | RX_IA_ACCEPT | RX_BROADCAST_ACCEPT | RX_PROM_ACCEPT); // TEST
  cs8900a_writereg(PP_CS8900_ISAINT, CS8900A_IRQ_NUMBER);
  cs8900a_writereg(PP_BusCTL, ENABLE_IRQ);

}

/**
 * @brief Checks if any buffer is available, but don't gets it.
 *
 * @return The pointer to the buffer or NULL, if no one available.
 */
static uint8_t *rb_peekfirst(void) {

  if (rxcount)
    return rxfirst;
  else
    return NULL;
}

/**
 * @brief Gets a pointer to allocated buffer.
 *
 * @return The pointer to the buffer or NULL, if no one available.
 */
static uint8_t *rb_getfirst(void) {

  uint8_t *ptr = NULL;

  if (rxcount) {
    ptr = rxfirst;
    rxcount--;
    rxfirst += MAC_RECEIVE_BUFFERS_SIZE;
    if (rxfirst >= &rb[MAC_RECEIVE_BUFFERS * MAC_RECEIVE_BUFFERS_SIZE])
      rxfirst = rb;
  }
  return ptr;
}

/**
 * @brief Gets pointer to free element in the ring buffer.
 *
 * @return The pointer to free buffer or NULL, if no buffers available.
 */
static uint8_t *rb_getfree(void) {

  uint8_t *ptr = NULL;

  if (rxcount < MAC_RECEIVE_BUFFERS) {
    ptr = rxfree;
    rxcount++;
    rxfree += MAC_RECEIVE_BUFFERS_SIZE;
    if (rxfree >= &rb[MAC_RECEIVE_BUFFERS * MAC_RECEIVE_BUFFERS_SIZE])
      rxfree = rb;
  }
  return ptr;
}

/**
 * @brief Processes receive event.
 *
 * @return The operation status.
 * @retval TRUE Frame received with no errors.
 * @retval FALSE Frame not received or received with errors.
 */
static bool_t process_rx_event(void)
{
  uint16_t status;
  int16_t length;
  uint16_t tmp;
  int i;
  uint16_t *ptr;

  status = CS8900A_READ(RX_FRAME_PORT);
  if ( (status & RX_OK ) == 0)
    return FALSE;
  length = CS8900A_READ(RX_FRAME_PORT);
  if (length == 0)
    return FALSE;
  if (length > MAC_BUFFERS_SIZE) {
    cs8900a_writereg(PP_RxCFG, cs8900a_readreg(PP_RxCFG) | SKIP_1);
    return FALSE;
  }
  ptr = (uint16_t *)rb_getfree();
  if (ptr == NULL) {
    cs8900a_writereg(PP_RxCFG, cs8900a_readreg(PP_RxCFG) | SKIP_1);
    return FALSE;
  }
  i = 0;
  ptr[0] = length; // first 2 bytes are length of the frame
  ptr++;
  while (length > 0) {

    tmp = CS8900A_READ(RX_FRAME_PORT);
    ptr[i++] = (tmp<<8)|(tmp>>8);
    length -= 2;
  }
  return TRUE;
}

//void process_tx_event(void)
//{
//}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/**
 * @brief CS8900A IRQ handler.
 */
CH_IRQ_HANDLER(IRQ7Handler) {

  uint16_t isq;
//  uint16_t status;

  CH_IRQ_PROLOGUE();

  while ( (isq = CS8900A_READ(ISQ_PORT)) ) {

    isq &= ISQ_EVENT_MASK;
    switch (isq) {

      case ISQ_RX_EVENT:
        if (process_rx_event() == TRUE) {
          chSysLockFromIsr();
          chSemResetI(&ETH1.md_rdsem, 0);
#if CH_USE_EVENTS
          chEvtBroadcastI(&ETH1.md_rdevent);
#endif
          chSysUnlockFromIsr();
        }
        break;
      case ISQ_TX_EVENT:
//        process_tx_event();
        chSysLockFromIsr();
        chSemResetI(&ETH1.md_tdsem, 0);
        chSysUnlockFromIsr();
        break;
      case ISQ_BUFFER_EVENT:
      case ISQ_RX_MISS_EVENT:
        // TODO: add statistics handling
//        status = cs8900a_readreg(PP_RxMiss);
        break;
      case ISQ_TX_COL_EVENT:
        break;
    }
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

  /*
   * Bus initialization.
   */
  ABWCR &= ~_BV(5);                    // 16-bit access space for CS5
  ASTCR |= _BV(5);                     // 3-state access space for CS5
  WTCRAL = (WTCRAL & 0x0F) | 0x20;     // there must be at least 5 wait states
                                       // access for 32MHz clock
                                       // (3-state bus + 2 wait states)
  PFDDR = 0x02;                        // enable CS5 for output
  PFCR0 |= _BV(5);                     // assign CS5 to right gpio pin

  /*
   * rx buffer initialization
   */
  rxfirst = rb;
  rxfree = rb;
  rxcount = 0;

  /*
   * Configure CS8900A registers.
   */
  cs8900a_init();

  mac_lld_set_address(&ETH1, default_mac);

  /*
   * Configure external h8s interrupt pin connected to CS8900A.
   */
  *(volatile uint32_t *)(IRQ_7) = (uint32_t)IRQ7Handler;
  ISCRL = (ISCRL & 0x3FFF) | 0x0000;   // irq7 on level
  ITSR |= _BV(7);                      // irq7 on pin P47
  ISR = ISR & (~_BV(7));               // clear pending irq7 (just in case)
  IER |= _BV(7);                       // enable irq7

}

/**
 * @brief Low level MAC address setup.
 *
 * @param[in] macp pointer to the @p MACDriver object
 * @param[in] p pointer to a six bytes buffer containing the MAC address. If
 *            this parameter is set to @p NULL then a system default MAC is
 *            used.
 */
void mac_lld_set_address(MACDriver *macp, const uint8_t *p) {

  cs8900a_writereg(PP_IA+0, (p[1]<<8)|p[0]);
  cs8900a_writereg(PP_IA+2, (p[3]<<8)|p[2]);
  cs8900a_writereg(PP_IA+4, (p[5]<<8)|p[4]);
/*
  // if data bus is not switched
  cs8900a_writereg(PP_IA, (p[0]<<8)|p[1]);
  cs8900a_writereg(PP_IA+2, (p[2]<<8)|p[3]);
  cs8900a_writereg(PP_IA+4, (p[4]<<8)|p[5]);
*/
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
  // check link status
  chSysLock();
  if ( (cs8900a_readreg(PP_LineST) & LINK_OK) == 0 ) {
    chSysUnlock();
    return RDY_TIMEOUT;
  }
  chSysUnlock();
  // send transmit command
  CS8900A_WRITE(TX_CMD_PORT, TX_START_ALL_BYTES);
  CS8900A_WRITE(TX_LEN_PORT, tdp->td_req_size);
  // check if buffer space is available in CS8900A internal memory and if so reserve one
  chSysLock();
  if ( (cs8900a_readreg(PP_BusST) & READY_FOR_TX_NOW) == 0 ) {
    // skip received frames to make space for transmit frame
    cs8900a_writereg(PP_RxCFG, cs8900a_readreg(PP_RxCFG) | SKIP_1);
    chSysUnlock();
    return RDY_TIMEOUT;
  }
  chSysUnlock();
  tdp->td_offset = 0;
  tdp->td_size = MAC_BUFFERS_SIZE;
  return RDY_OK;
}

/**
 * @brief Writes to a transmit descriptor's stream.
 *
 * @param[in] tdp pointer to a @p MACTransmitDescriptor structure
 * @param[in] buf pointer to the buffer cointaining the data to be written
 * @param[in] size number of bytes to be written
 * @return The number of bytes written into the descriptor's stream, this
 *         value can be less than the amount specified in the parameter
 *         @p size if the maximum frame size is reached.
 */
size_t mac_lld_write_transmit_descriptor(MACTransmitDescriptor *tdp,
                                         uint8_t *buf,
                                         size_t size) {

  if (size > tdp->td_size - tdp->td_offset)
    size = tdp->td_size - tdp->td_offset;
  if (size > 0) {
    memcpy((uint8_t *)(tb) + tdp->td_offset, buf, size);
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

  int i;

  for (i=0; i<(tdp->td_offset+1)/2; i++) {
    CS8900A_WRITE(TX_FRAME_PORT, (tb[i]<<8)|(tb[i]>>8));
  }

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

  uint16_t *ptr;

  chSysLock();
  ptr=(uint16_t *)rb_peekfirst();
  chSysUnlock();
  if (ptr == NULL) {
    return RDY_TIMEOUT;
  }
  rxcurrent = (uint8_t *)(&ptr[1]);
  rdp->rd_offset = 0;
  rdp->rd_size = ptr[0];
  return RDY_OK;
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
    memcpy(buf, rxcurrent+rdp->rd_offset, size);
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

  uint8_t *ptr;

  chSysLock();
  ptr = rb_getfirst();
  chSysUnlock();
  chDbgAssert(ptr!=rxcurrent,
              "mac_lld_release_receive_descriptor(), #1",
              "different pointers");
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

  chSysLock();
  link_up = cs8900a_readreg(PP_LineST) & LINK_OK ? TRUE : FALSE;
  chSysUnlock();
  return link_up;
}

#endif /* CH_HAL_USE_MAC */

/** @} */
