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

#ifndef _DYN_CLK_LLD_H_
#define _DYN_CLK_LLD_H_

/* compatibility macros */
#define STM32_SW            ((CLKCFG.clk_profile->rcc_cfgr) & STM32_SW_MASK)
#define STM32_PPRE1         ((CLKCFG.clk_profile->rcc_cfgr) & STM32_PPRE1_MASK)
#define STM32_HPRE          ((CLKCFG.clk_profile->rcc_cfgr) & STM32_HPRE_MASK)
#define STM32_PPRE2         ((CLKCFG.clk_profile->rcc_cfgr) & STM32_PPRE2_MASK)
#define STM32_ADCPRE        ((CLKCFG.clk_profile->rcc_cfgr) & STM32_ADCPRE_MASK)
#define STM32_USBPRE        ((CLKCFG.clk_profile->rcc_cfgr) & STM32_USBPRE_MASK)
#define STM32_PLLMUL        ((CLKCFG.clk_profile->rcc_cfgr) & STM32_PLLMUL_MASK)
#define STM32_MCOSEL        ((CLKCFG.clk_profile->rcc_cfgr) & STM32_MCOSEL_MASK)
#define STM32_PLLXTPRE      ((CLKCFG.clk_profile->rcc_cfgr) & STM32_PLLXTPRE_MASK)
#define STM32_PLLSRC        ((CLKCFG.clk_profile->rcc_cfgr) & STM32_PLLSRC_MASK)

#define STM32_FLASHBITS     (CLKCFG.clk_profile->flashbits)
#define STM32_ACTIVATE_PLL  (CLKCFG.clk_profile->pll)
#define STM32_SYSCLK        (CLKCFG.clk_profile->sysclk)
#define STM32_HCLK          (CLKCFG.clk_profile->hclk)
#define STM32_PCLK1         (CLKCFG.clk_profile->pclk1)
#define STM32_PCLK2         (CLKCFG.clk_profile->pclk2)
#define STM32_TIMCLK1       (CLKCFG.clk_profile->timclk1)
#define STM32_TIMCLK2       (CLKCFG.clk_profile->timclk2)

#define STM32_HSE_ENABLED   (stm32_hse_enabled())


/**
 * @brief
 */
typedef struct ClockProfile ClockProfile;


/**
 * @brief
 */
//TODO: add TIM clocking
struct ClockProfile{
  uint32_t                  rcc_cfgr;
  uint32_t                  flashbits;
  bool_t                    pll;
  uint32_t                  sysclk;
  uint32_t                  hclk;
  uint32_t                  pclk1;
  uint32_t                  pclk2;
  uint32_t                  adcclk;
  uint32_t                  timclk1;
  uint32_t                  timclk2;
};

/**
 * @brief
 */
typedef struct {
  ClockProfile     const * clk_profile;
} ClockConfig;

/**
 * @brief
 */
extern ClockConfig CLKCFG;

extern const ClockProfile clk_prf_default;
extern const ClockProfile clk_prf_low;



#ifdef __cplusplus
extern "C" {
#endif
  bool_t stm32_hse_enabled(void);
  void clkcfgObjectInit(ClockConfig *cfg, ClockProfile const *prf);
  void stm32_clock_profile_switch(ClockProfile const *prf);
  void hal_lld_systick_init(void);
#ifdef __cplusplus
}
#endif

#endif /* _DYN_CLK_LLD_H_ */

/** @} */
