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
 * @file    templates/i2c_lld.h
 * @brief   I2C Driver subsystem low level driver header template.
 *
 * @addtogroup I2C
 * @{
 */

#ifndef _I2C_LLD_H_
#define _I2C_LLD_H_

#if HAL_USE_I2C || defined(__DOXYGEN__)

#include "bcm2835.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/


/**
 * @brief   Type of a structure representing an I2C driver.
 */
typedef struct I2CDriver I2CDriver;

/**
 * @brief   I2C status type
 */
typedef uint32_t i2cstatus_t;

/**
 * @brief   I2C flags type
 */
typedef uint32_t i2cflags_t;

/**
 * @brief   I2C address type
 * @details I2C address type. May support 10 bit addressing in the future.
 */
typedef uint16_t i2caddr_t;

/**
 * @brief   I2C completion callback type.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 * @param[in] sts       operation status
 */
typedef void (*i2ccallback_t)(I2CDriver *i2cp, i2cstatus_t sts);

/**
 * @brief   Driver configuration structure.
 * @note    Implementations may extend this structure to contain more,
 *          architecture dependent, fields.
 */
typedef struct {
  /** @brief I2C bus bit rate.*/
  uint32_t                  ic_speed;
  /* End of the mandatory fields.*/
} I2CConfig;

/**
 * @brief   Structure representing an I2C driver.
 * @note    Implementations may extend this structure to contain more,
 *          architecture dependent, fields.
 */
struct I2CDriver {
  /** @brief Driver state.*/
  i2cstate_t                state;
  /** @brief Current configuration data.*/
  const I2CConfig           *config;
  /** @brief Error flags.*/
  i2cflags_t                errors;
  /** @brief BSC device registers.*/
  bscdevice_t               *device;
#if I2C_USE_MUTUAL_EXCLUSION
#if CH_USE_MUTEXES
  Mutex                     mutex;
#endif /* CH_USE_MUTEXES */
#endif /* I2C_USE_MUTUAL_EXCLUSION */
#if defined(I2C_DRIVER_EXT_FIELDS)
  I2C_DRIVER_EXT_FIELDS
#endif
  /* End of the mandatory fields.*/
  /**
   * @brief   Thread waiting for I/O completion.
   */
  Thread                    *thread;
  /**
   * @brief   Address of slave device.
   */
  i2caddr_t                 addr;
  /**
   * @brief   Pointer to the buffer with data to send.
   */
  const uint8_t             *txbuf;
  /**
   * @brief   Number of bytes of data to send.
   */
  size_t                    txbytes;
  /**
   * @brief   Current index in buffer when sending data.
   */
  size_t                    txidx;
  /**
   * @brief   Pointer to the buffer to put received data.
   */
  uint8_t                   *rxbuf;
  /**
   * @brief   Number of bytes of data to receive.
   */
  size_t                    rxbytes;
  /**
   * @brief   Current index in buffer when receiving data.
   */
  size_t                    rxidx;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

#define i2c_lld_master_start(i2cp, header)

#define i2c_lld_master_stop(i2cp)

#define i2c_lld_master_restart(i2cp)

#define i2c_lld_get_errors(i2cp) ((i2cp)->errors)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

extern I2CDriver I2C0;
extern I2CDriver I2C1;

#ifdef __cplusplus
extern "C" {
#endif
  void i2c_lld_init(void);
  void i2c_lld_start(I2CDriver *i2cp);
  void i2c_lld_stop(I2CDriver *i2cp);

  msg_t i2c_lld_master_transmit_timeout(I2CDriver *i2cp, i2caddr_t addr, 
                                       const uint8_t *txbuf, size_t txbytes, 
                                       uint8_t *rxbuf, const uint8_t rxbytes, 
                                       systime_t timeout);

  msg_t i2c_lld_master_receive_timeout(I2CDriver *i2cp, i2caddr_t addr, 
                                       uint8_t *rxbuf, size_t rxbytes, 
                                       systime_t timeout);

  void i2c_lld_serve_interrupt(I2CDriver *i2cp);

#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_I2C */

#endif /* _I2C_LLD_H_ */

/** @} */
