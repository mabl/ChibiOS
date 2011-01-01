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
 * @file    serial_usb.h
 * @brief   Serial over USB Driver macros and structures.
 *
 * @addtogroup SERIAL_USB
 * @{
 */

#ifndef _SERIAL_USB_H_
#define _SERIAL_USB_H_

#if HAL_USE_SERIAL_USB || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/** @brief No pending conditions.*/
#define SD_NO_ERROR             0
/** @brief Connection happened.*/
#define SD_CONNECTED            1
/** @brief Disconnection happened.*/
#define SD_DISCONNECTED         2
/** @brief Parity error happened.*/
#define SD_PARITY_ERROR         4
/** @brief Framing error happened.*/
#define SD_FRAMING_ERROR        8
/** @brief Overflow happened.*/
#define SD_OVERRUN_ERROR        16
/** @brief Noise on the line.*/
#define SD_NOISE_ERROR          32
/** @brief Break detected.*/
#define SD_BREAK_DETECTED       64

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   Serial over USB buffers size.
 * @details Configuration parameter, the buffer size must be a multiple of
 *          the USB data endpoint maximum packet size.
 * @note    The default is 64 bytes for both the transmission and receive
 *          buffers.
 */
#if !defined(SERIAL_USB_BUFFERS_SIZE) || defined(__DOXYGEN__)
#define SERIAL_USB_BUFFERS_SIZE     64
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !HAL_USE_USB && !CH_USE_EVENTS
#error "Serial over USB Driver requires HAL_USE_USB and CH_USE_EVENTS"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief Driver state machine possible states.
 */
typedef enum {
  SDU_UNINIT = 0,                   /**< Not initialized.                   */
  SDU_STOP = 1,                     /**< Stopped.                           */
  SDU_READY = 2                     /**< Ready.                             */
} sdustate_t;

/**
 * @brief   Serial over USB Driver condition flags type.
 */
typedef uint8_t sduflags_t;

/**
 * @brief   Type of an I/O buffer.
 */
typedef struct {
  /**
   * @brief   Buffer guard semaphore.
   */
  Semaphore                     sem;
  /**
   * @brief   Next position to read or write.
   */
  uint8_t                       *next;
  /**
   * @brief   Buffer area.
   */
  uint8_t                       *buffer[SERIAL_USB_BUFFERS_SIZE];
} sdubuffer_t;

/**
 * @brief   Structure representing a serial over USB driver.
 */
typedef struct SerialUSBDriver SerialUSBDriver;

/**
 * @brief   Serial over USB Driver configuration structure.
 * @details An instance of this structure must be passed to @p sduStart()
 *          in order to configure and start the driver operations.
 */
typedef struct {
  /**
   * @brief   USB driver to use.
   */
  USBDriver                     *usbp;
  /**
   * @brief   USB driver configuration structure.
   */
  USBConfig                     usb_config;
} SerialUSBConfig;

/**
 * @brief   @p SerialDriver specific data.
 */
#define _serial_usb_driver_data                                             \
  _base_asynchronous_channel_data                                           \
  /* Driver state.*/                                                        \
  sdustate_t                    state;                                      \
  /** Current configuration data.*/                                         \
  const SerialUSBConfig         *config;                                    \
  /* I/O driver status flags.*/                                             \
  sduflags_t                    flags;                                      \
  /* Receive buffer.*/                                                      \
  sdubuffer_t                   rxbuf;                                      \
  /* Transmit buffer.*/                                                     \
  sdubuffer_t                   txbuf;

/**
 * @brief   @p SerialUSBDriver specific methods.
 */
#define _serial_usb_driver_methods                                          \
  _base_asynchronous_channel_methods

/**
 * @brief   @p SerialDriver virtual methods table.
 */
struct SerialUSBDriverVMT {
  _serial_usb_driver_methods
};

/**
 * @extends BaseAsynchronousChannel
 *
 * @brief   Full duplex serial driver class.
 * @details This class extends @p BaseAsynchronousChannel by adding physical
 *          I/O queues.
 */
struct SerialUSBDriver {
  /** @brief Virtual Methods Table.*/
  const struct SerialUSBDriverVMT *vmt;
  _serial_usb_driver_data
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void sduInit(void);
  void sduObjectInit(SerialUSBDriver *sdp);
  void sduStart(SerialUSBDriver *sdup, const SerialUSBConfig *config);
  void sduStop(SerialUSBDriver *sdup);
  void sduAddFlagsI(SerialUSBDriver *sdup, sduflags_t mask);
  sduflags_t sduGetAndClearFlags(SerialUSBDriver *sdup);
  bool_t sduRequestsHook(USBDriver *usbp);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_SERIAL_USB */

#endif /* _SERIAL_USB_H_ */

/** @} */
