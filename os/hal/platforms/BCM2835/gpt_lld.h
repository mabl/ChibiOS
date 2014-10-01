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
 * @file    templates/gpt_lld.h
 * @brief   GPT Driver subsystem low level driver header template.
 *
 * @addtogroup GPT
 * @{
 */

#ifndef _GPT_LLD_H_
#define _GPT_LLD_H_

#if HAL_USE_GPT || defined(__DOXYGEN__)

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
 * @brief   GPT frequency type.
 */
typedef uint32_t gptfreq_t;

/**
 * @brief   GPT counter type.
 */
typedef uint32_t gptcnt_t;

/**
 * @brief   Type of a structure representing a GPT driver.
 */
typedef struct GPTDriver GPTDriver;

/**
 * @brief   GPT notification callback type.
 *
 * @param[in] gptp      pointer to a @p GPTDriver object
 */
typedef void (*gptcallback_t)(GPTDriver *gptp);

/**
 * @brief   Driver configuration structure.
 * @note    It could be empty on some architectures.
 */
typedef struct {
  /**
   * @brief   Timer callback pointer.
   * @note    This callback is invoked on GPT counter events.
   */
  gptcallback_t             callback;
  /* End of the mandatory fields.*/
} GPTConfig;

/**
 * @brief   Structure representing a GPT driver.
 */
struct GPTDriver {
  /**
   * @brief Driver state.
   */
  gptstate_t                state;
  /**
   * @brief Current configuration data.
   */
  const GPTConfig           *config;
#if defined(GPT_DRIVER_EXT_FIELDS)
  GPT_DRIVER_EXT_FIELDS
#endif
  /* End of the mandatory fields.*/
  /* @brief System Timer comparison register */
  volatile uint32_t         *compare;

  /* @brief System Timer match mask.*/
  uint32_t                  match_mask;

  /* @brief System Timer IRQ mask.*/
  uint32_t                  irq_mask;
  
  /* @brief Continuous timer period */
  gptcnt_t                  period;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if BCM2835_GPT_USE_TIMER1 && !defined(__DOXYGEN__)
extern GPTDriver GPTD1;
#endif

#if BCM2835_GPT_USE_TIMER2 && !defined(__DOXYGEN__)
extern GPTDriver GPTD2;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void gpt_lld_init(void);
  void gpt_lld_start(GPTDriver *gptp);
  void gpt_lld_stop(GPTDriver *gptp);
  void gpt_lld_start_timer(GPTDriver *gptp, gptcnt_t interval);
  void gpt_lld_stop_timer(GPTDriver *gptp);
  void gpt_lld_polled_delay(GPTDriver *gptp, gptcnt_t interval);

  void gpt_lld_serve_interrupt(void);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_GPT */

#endif /* _GPT_LLD_H_ */

/** @} */
