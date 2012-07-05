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

#include "ch.h"
#include "hal.h"


/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

const ClockProfile clk_prf_default = {
    (STM32_SW_PLL |    (9 - 2) << 18 |    STM32_HPRE_DIV1 |    STM32_PPRE1_DIV2 |    STM32_PPRE2_DIV2 |    STM32_ADCPRE_DIV4 |    STM32_PLLSRC_HSE |    STM32_PLLXTPRE_DIV1 |    0 |    STM32_MCOSEL_NOCLOCK),
    18,
    TRUE,
    72000000,
    72000000,
    36000000,
    36000000,
    9000000,
    72000000,
    72000000,
};

const ClockProfile clk_prf_low = {
    (STM32_SW_HSE |    (2 - 2) << 18 |    STM32_HPRE_DIV1 |    STM32_PPRE1_DIV1 |    STM32_PPRE2_DIV1 |    STM32_ADCPRE_DIV2 |    STM32_PLLSRC_HSE |    STM32_PLLXTPRE_DIV1 |    0 |    STM32_MCOSEL_NOCLOCK),
    16,
    FALSE,
    8000000,
    8000000,
    8000000,
    8000000,
    4000000,
    8000000,
    8000000,
};

/**
 * @brief Clock config structure.
 * @note  Must be initially set to @p NULL in order to proper clock init during
 *        startup.
 */
ClockConfig CLKCFG = {NULL};

bool_t stm32_hse_enabled(void) {
  if ((STM32_SW == STM32_SW_HSE) || (STM32_PLLSRC == STM32_PLLSRC_HSE))
    return TRUE;
  else
    return FALSE;
}

/**
 * @brief   Switch to HSI.
 */
void stm32_switch_to_hsi(void){
  RCC->CR |= RCC_CR_HSION;                  /* Make sure HSI is ON.         */
  while (!(RCC->CR & RCC_CR_HSIRDY))
    ;                                       /* Wait until HSI is stable.    */

  RCC->CFGR &= ~STM32_SW_MASK;
  RCC->CFGR |= STM32_SW_HSI;
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI)
    ;                                       /* Waits until HSI is selected. */
}


/**
 * @brief   Switching clocks to new profile.
 * @details Detect what must be stopped and what must be started.
 */
void stm32_clock_profile_switch(ClockProfile const *prf) {

  stm32_switch_to_hsi();

  CLKCFG.clk_profile = prf;

  /* change HSE settings */
  if (STM32_HSE_ENABLED){
    /* HSE activation.*/
    RCC->CR |= RCC_CR_HSEON;
    while (!(RCC->CR & RCC_CR_HSERDY))
      ;                                     /* Waits until HSE is stable.   */
  }
  else
    RCC->CR &= ~RCC_CR_HSEON;

  /* change PLL settings */
  if (STM32_ACTIVATE_PLL){
    uint32_t new_pll = STM32_PLLMUL | STM32_PLLXTPRE | STM32_PLLSRC;
    uint32_t pll_msk = STM32_PLLMUL_MASK | STM32_PLLXTPRE_MASK | STM32_PLLSRC_MASK;

    if ((RCC->CFGR & pll_msk) != new_pll){
      /* stop pll and clear its settings */
      RCC->CR   &= ~RCC_CR_PLLON;
      RCC->CFGR &= ~(STM32_PLLMUL_MASK | STM32_PLLXTPRE_MASK | STM32_PLLSRC_MASK);
      /* start pll with new settings */
      RCC->CFGR |= STM32_PLLMUL | STM32_PLLXTPRE | STM32_PLLSRC;
      RCC->CR   |= RCC_CR_PLLON;
      while (!(RCC->CR & RCC_CR_PLLRDY))
        ;                                     /* Waits until PLL is stable.   */
    }
  }
  else{
    /* stop pll and clear its settings */
    RCC->CR   &= ~RCC_CR_PLLON;
    RCC->CFGR &= ~(STM32_PLLMUL_MASK | STM32_PLLXTPRE_MASK | STM32_PLLSRC_MASK);
  }

  /* Clock settings.*/
#if STM32_HAS_USB
  RCC->CFGR = STM32_MCOSEL | STM32_USBPRE | STM32_PLLMUL | STM32_PLLXTPRE |
              STM32_PLLSRC | STM32_ADCPRE | STM32_PPRE2  | STM32_PPRE1    |
              STM32_HPRE;
#else
  RCC->CFGR = STM32_MCOSEL |                STM32_PLLMUL | STM32_PLLXTPRE |
              STM32_PLLSRC | STM32_ADCPRE | STM32_PPRE2  | STM32_PPRE1    |
              STM32_HPRE;
#endif

  /* Flash setup. */
  FLASH->ACR = STM32_FLASHBITS;

  /* Switching to configured clock source. */
  RCC->CFGR |= STM32_SW;
  while ((RCC->CFGR & RCC_CFGR_SWS) != (STM32_SW << 2))
    ;                                       /* Waits selection complete.    */

  /* Switch systick timer to new frequency. */
  hal_lld_systick_init();
}

/**
 *
 */
void hal_lld_systick_init(void){
  /* SysTick initialization using the system clock.*/
  SysTick->LOAD = STM32_HCLK / CH_FREQUENCY - 1;
  SysTick->VAL = 0;
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                  SysTick_CTRL_ENABLE_Msk |
                  SysTick_CTRL_TICKINT_Msk;
}

