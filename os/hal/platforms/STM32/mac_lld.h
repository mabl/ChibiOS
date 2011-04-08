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
 * @file STM32/mac_lld.h
 * @brief STM32 low level MAC driver header.
 * @addtogroup STM32_MAC
 * @{
 */

#ifndef _MAC_LLD_H_
#define _MAC_LLD_H_

#if HAL_USE_MAC || defined(__DOXYGEN__)

/*
 * Note: Currently this MAC driver is dependent on the STM32 Ethernet code
 * library.
 */
#include "stm32_eth.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief Number of available transmit buffers.
 */
#if !defined(MAC_TRANSMIT_BUFFERS) || defined(__DOXYGEN__)
#define MAC_TRANSMIT_BUFFERS        2
#endif

/**
 * @brief Number of available receive buffers.
 */
#if !defined(MAC_RECEIVE_BUFFERS) || defined(__DOXYGEN__)
#define MAC_RECEIVE_BUFFERS         4
#endif

/**
 * @brief Maximum supported frame size.
 */
#if !defined(MAC_BUFFERS_SIZE) || defined(__DOXYGEN__)
#define MAC_BUFFERS_SIZE            1520
#endif

/**
 * @brief ETH1 interrupt priority level setting.
 */
#if !defined(STM32_ETH1_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_ETH1_IRQ_PRIORITY     13
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief Structure representing a Frame.
 */
typedef ETH_DMADESCTypeDef MACDescriptor;

/**
 * @brief Structure representing a MAC driver.
 */
typedef struct {
  Semaphore             tdsem;          /**< Transmit semaphore.        */
  Semaphore             rdsem;          /**< Receive semaphore.         */
#if CH_USE_EVENTS
  EventSource           rdevent;        /**< Receive event source.      */
#endif
  /* End of the mandatory fields.*/
} MACDriver;

/**
 * @brief Structure representing a transmit descriptor.
 */
typedef struct {
  size_t                offset;         /**< Current write offset.      */
  size_t                size;           /**< Available space size.      */
  /* End of the mandatory fields.*/
  MACDescriptor        *physdesc;      /**< Pointer to the physical
                                             descriptor.                */
} MACTransmitDescriptor;

/**
 * @brief Structure representing a receive descriptor.
 */
typedef struct {
  size_t                offset;         /**< Current read offset.       */
  size_t                size;           /**< Available data size.       */
  /* End of the mandatory fields.*/
  MACDescriptor        *physdesc;       /**< Pointer to the first descriptor
                                             of the buffers chain.      */
} MACReceiveDescriptor;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(__DOXYGEN__)
extern MACDriver ETH1;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void mac_lld_init(void);
  void mac_lld_set_address(MACDriver *macp, const uint8_t *p);
  msg_t max_lld_get_transmit_descriptor(MACDriver *macp,
                                        MACTransmitDescriptor *tdp);
  size_t mac_lld_write_transmit_descriptor(MACTransmitDescriptor *tdp,
                                           uint8_t *buf,
                                           size_t size);
  void mac_lld_release_transmit_descriptor(MACTransmitDescriptor *tdp);
  msg_t max_lld_get_receive_descriptor(MACDriver *macp,
                                       MACReceiveDescriptor *rdp);
  size_t mac_lld_read_receive_descriptor(MACReceiveDescriptor *rdp,
                                         uint8_t *buf,
                                         size_t size);
  void mac_lld_release_receive_descriptor(MACReceiveDescriptor *rdp);
  bool_t mac_lld_poll_link_status(MACDriver *macp);
#ifdef __cplusplus
}
#endif

#endif /* CH_HAL_USE_MAC */

#endif /* _MAC_LLD_H_ */

/** @} */
