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

#define STM32_SW            ((CLK_CFG->rcc_cfgr) & STM32_SW_MASK)
#define STM32_PPRE1         ((CLK_CFG->rcc_cfgr) & STM32_PPRE1_MASK)
#define STM32_HPRE          ((CLK_CFG->rcc_cfgr) & STM32_HPRE_MASK)
#define STM32_PPRE2         ((CLK_CFG->rcc_cfgr) & STM32_PPRE2_MASK)
#define STM32_ADCPRE        ((CLK_CFG->rcc_cfgr) & STM32_ADCPRE_MASK)
#define STM32_USBPRE        ((CLK_CFG->rcc_cfgr) & STM32_USBPRE_MASK)
#define STM32_PLLMUL        ((CLK_CFG->rcc_cfgr) & STM32_PLLMUL_MASK)
#define STM32_MCOSEL        ((CLK_CFG->rcc_cfgr) & STM32_MCOSEL_MASK)
#define STM32_PLLXTPRE      ((CLK_CFG->rcc_cfgr) & STM32_PLLXTPRE_MASK)
#define STM32_PLLSRC        ((CLK_CFG->rcc_cfgr) & STM32_PLLSRC_MASK)

#define STM32_FLASHBITS     (CLK_CFG->flashbits)
#define STM32_ACTIVATE_PLL  (CLK_CFG->pll)
#define STM32_SYSCLK        (CLK_CFG->sysclk)
#define STM32_HCLK          (CLK_CFG->hclk)
#define STM32_PCLK1         (CLK_CFG->pclk1)
#define STM32_PCLK2         (CLK_CFG->pclk2)

typedef struct ClockConfig ClockConfig;

/**
 * @brief   Configuration structure.
 */
struct ClockConfig{
  uint32_t                  rcc_cfgr;
  uint32_t                  flashbits;
  bool_t                    pll;
  uint32_t                  sysclk;
  uint32_t                  hclk;
  uint32_t                  pclk1;
  uint32_t                  pclk2;
  uint32_t                  adcclk;
};



extern ClockConfig const * CLK_CFG;


#ifdef __cplusplus
extern "C" {
#endif
  bool_t stm32_hse_enabled(void);
  ClockConfig const* clkcfgObjectInit(void);
#ifdef __cplusplus
}
#endif

#endif /* _DYN_CLK_LLD_H_ */

/** @} */
