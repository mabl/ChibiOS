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

#ifndef _CLK_LLD_H_
#define _CLK_LLD_H_

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Platform capabilities.                                                    */
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
 * @brief Type representing a system clock profile.
 */
typedef struct ClockProfile ClockProfile;

/**
 * @brief
 */
struct ClockProfile{
  uint32_t                  rcc_cfgr;
  uint32_t                  flashbits;
  bool_t                    activate_pll;
  bool_t                    hse_enabled;
  uint32_t                  sysclk;
  uint32_t                  hclk;
  uint32_t                  pclk1;
  uint32_t                  pclk2;
  uint32_t                  adcclk;
  uint32_t                  timclk1;
  uint32_t                  timclk2;
  uint32_t                  usbclk;
};

/**
 * @brief
 */
typedef struct {
  ClockProfile     const *profile;
} ClockConfig;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*
 * compatibility macros
 */
#define STM32_SW            ((CLKCFG.profile->rcc_cfgr) & STM32_SW_MASK)
#define STM32_PPRE1         ((CLKCFG.profile->rcc_cfgr) & STM32_PPRE1_MASK)
#define STM32_HPRE          ((CLKCFG.profile->rcc_cfgr) & STM32_HPRE_MASK)
#define STM32_PPRE2         ((CLKCFG.profile->rcc_cfgr) & STM32_PPRE2_MASK)
#define STM32_ADCPRE        ((CLKCFG.profile->rcc_cfgr) & STM32_ADCPRE_MASK)
#define STM32_USBPRE        ((CLKCFG.profile->rcc_cfgr) & STM32_USBPRE_MASK)
#define STM32_PLLMUL        ((CLKCFG.profile->rcc_cfgr) & STM32_PLLMUL_MASK)
#define STM32_MCOSEL        ((CLKCFG.profile->rcc_cfgr) & STM32_MCOSEL_MASK)
#define STM32_PLLXTPRE      ((CLKCFG.profile->rcc_cfgr) & STM32_PLLXTPRE_MASK)
#define STM32_PLLSRC        ((CLKCFG.profile->rcc_cfgr) & STM32_PLLSRC_MASK)

#define STM32_FLASHBITS     (CLKCFG.profile->flashbits)
#define STM32_ACTIVATE_PLL  (CLKCFG.profile->activate_pll)
#define STM32_SYSCLK        (CLKCFG.profile->sysclk)
#define STM32_HCLK          (CLKCFG.profile->hclk)
#define STM32_PCLK1         (CLKCFG.profile->pclk1)
#define STM32_PCLK2         (CLKCFG.profile->pclk2)
#define STM32_TIMCLK1       (CLKCFG.profile->timclk1)
#define STM32_TIMCLK2       (CLKCFG.profile->timclk2)
#define STM32_USBCLK        (CLKCFG.profile->usbclk)

#define STM32_HSE_ENABLED   (CLKCFG.profile->hse_enabled)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

extern ClockConfig CLKCFG;
extern const ClockProfile clk_prf_default;
extern const ClockProfile clk_prf_low;

#ifdef __cplusplus
extern "C" {
#endif
  void stm32_clock_profile_switch(ClockProfile const *prf);
  void hal_lld_systick_switch(void);
#ifdef __cplusplus
}
#endif

#endif /* _CLK_LLD_H_ */

/** @} */
