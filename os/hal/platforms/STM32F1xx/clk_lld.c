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
   (STM32_SW_PLL |
       (9 - 2) << 18 |
       STM32_HPRE_DIV1 |
       STM32_PPRE1_DIV2 |
       STM32_PPRE2_DIV2 |
       STM32_ADCPRE_DIV4 |
       STM32_PLLSRC_HSE |
       STM32_PLLXTPRE_DIV1 |
       0 |
       STM32_MCOSEL_NOCLOCK),
   18,
   TRUE,
   TRUE,
   72000000,
   72000000,
   36000000,
   36000000,
   9000000,
   72000000,
   72000000,
   48000000,
};

const ClockProfile clk_prf_low = {
   (STM32_SW_HSE |
       (2 - 2) << 18 |
       STM32_HPRE_DIV1 |
       STM32_PPRE1_DIV1 |
       STM32_PPRE2_DIV1 |
       STM32_ADCPRE_DIV2 |
       STM32_PLLSRC_HSE |
       STM32_PLLXTPRE_DIV1 |
       0 |
       STM32_MCOSEL_NOCLOCK),
   16,
   FALSE,
   TRUE,
   8000000,
   8000000,
   8000000,
   8000000,
   4000000,
   8000000,
   8000000,
   0,
};

/**
 * @brief Clock config structure.
 */
ClockConfig CLKCFG = {NULL};

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief Switch MCU to HSI.
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

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief Switch systick timer to new clock.
 */
void hal_lld_systick_switch(void){
  uint32_t sys_tick_load = STM32_HCLK / CH_FREQUENCY - 1;
  chDbgAssert(sys_tick_load < 0xFFFFFF,
      "hal_lld_systick_switch(), #1",
      "SysTick LOAD register overflow");
  SysTick->LOAD = sys_tick_load;
}

/**
 * @brief   Switch clocks to new profile.
 */
#if defined(STM32F10X_LD) || defined(STM32F10X_LD_VL) ||                    \
    defined(STM32F10X_MD) || defined(STM32F10X_MD_VL) ||                    \
    defined(STM32F10X_HD) || defined(STM32F10X_XL) ||                       \
    defined(__DOXYGEN__)
/*
 * Clocks initialization for all sub-families except CL.
 */
void stm32_clock_profile_switch(ClockProfile const *prf) {

  /* combined pll bits value */
  uint32_t pll = 0;

  /* combined pll mask value */
  uint32_t pll_msk = 0;

  /* use HSI during clock switching process */
  stm32_switch_to_hsi();

  /* switch clock config to new profile */
  CLKCFG.profile = prf;

  /* change HSE settings */
  if (STM32_HSE_ENABLED){
    /* HSE activation.*/
    RCC->CR |= RCC_CR_HSEON;
    while (!(RCC->CR & RCC_CR_HSERDY))
      ;                                      /* Waits until HSE is stable.   */
  }
  else
    RCC->CR &= ~RCC_CR_HSEON;

  /* change PLL settings */
  pll     = STM32_PLLMUL | STM32_PLLXTPRE | STM32_PLLSRC;
  pll_msk = STM32_PLLMUL_MASK | STM32_PLLXTPRE_MASK | STM32_PLLSRC_MASK;

  if (STM32_ACTIVATE_PLL){
    if ((RCC->CFGR & pll_msk) != pll){
      /* stop pll and clear its settings */
      RCC->CR   &= ~RCC_CR_PLLON;
      RCC->CFGR &= ~pll_msk;
      /* start pll with new settings */
      RCC->CFGR |= pll;
      RCC->CR   |= RCC_CR_PLLON;
      while (!(RCC->CR & RCC_CR_PLLRDY))
        ;                                    /* Waits until PLL is stable.   */
    }
  }
  else{
    /* stop pll and clear its settings */
    RCC->CR   &= ~RCC_CR_PLLON;
    RCC->CFGR &= ~pll_msk;
  }

  /* Clock settings.*/
  RCC->CFGR = STM32_MCOSEL | STM32_USBPRE | STM32_PLLMUL | STM32_PLLXTPRE |
              STM32_PLLSRC | STM32_ADCPRE | STM32_PPRE2  | STM32_PPRE1    |
              STM32_HPRE;

  /* Flash setup. */
  FLASH->ACR = STM32_FLASHBITS;

  /* Switching to configured clock source. */
  RCC->CFGR |= STM32_SW;
  while ((RCC->CFGR & RCC_CFGR_SWS) != (STM32_SW << 2))
    ;                                       /* Waits selection complete.    */

  /* Switch systick timer to new frequency. */
  hal_lld_systick_switch();
}


//TODO: rewrite CL family code
#elif defined(STM32F10X_CL)
/*
 * Clocks initialization for the CL sub-family.
 */
void stm32_clock_profile_switch(ClockProfile const *prf) {

#if !STM32_NO_INIT
  /* HSI setup.*/
  RCC->CR |= RCC_CR_HSION;                  /* Make sure HSI is ON.         */
  while (!(RCC->CR & RCC_CR_HSIRDY))
    ;                                       /* Wait until HSI is stable.    */
  RCC->CFGR = 0;
  RCC->CR &= RCC_CR_HSITRIM | RCC_CR_HSION; /* CR Reset value.              */
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI)
    ;                                       /* Wait until HSI is the source.*/

#if STM32_HSE_ENABLED
#if defined(STM32_HSE_BYPASS)
  /* HSE Bypass.*/
  RCC->CR |= RCC_CR_HSEBYP;
#endif
  /* HSE activation.*/
  RCC->CR |= RCC_CR_HSEON;
  while (!(RCC->CR & RCC_CR_HSERDY))
    ;                                       /* Waits until HSE is stable.   */
#endif

#if STM32_LSI_ENABLED
  /* LSI activation.*/
  RCC->CSR |= RCC_CSR_LSION;
  while ((RCC->CSR & RCC_CSR_LSIRDY) == 0)
    ;                                       /* Waits until LSI is stable.   */
#endif

  /* Settings of various dividers and multipliers in CFGR2.*/
  RCC->CFGR2 = STM32_PLL3MUL | STM32_PLL2MUL | STM32_PREDIV2 |
               STM32_PREDIV1 | STM32_PREDIV1SRC;

  /* PLL2 setup, if activated.*/
#if STM32_ACTIVATE_PLL2
  RCC->CR |= RCC_CR_PLL2ON;
  while (!(RCC->CR & RCC_CR_PLL2RDY))
    ;                                        /* Waits until PLL2 is stable. */
#endif

  /* PLL3 setup, if activated.*/
#if STM32_ACTIVATE_PLL3
  RCC->CR |= RCC_CR_PLL3ON;
  while (!(RCC->CR & RCC_CR_PLL3RDY))
    ;                                        /* Waits until PLL3 is stable. */
#endif

  /* PLL1 setup, if activated.*/
#if STM32_ACTIVATE_PLL1
  RCC->CFGR |= STM32_PLLMUL | STM32_PLLSRC;
  RCC->CR   |= RCC_CR_PLLON;
  while (!(RCC->CR & RCC_CR_PLLRDY))
    ;                           /* Waits until PLL1 is stable.              */
#endif

  /* Clock settings.*/
#if STM32_HAS_OTG1
  RCC->CFGR = STM32_MCOSEL | STM32_OTGFSPRE | STM32_PLLMUL | STM32_PLLSRC |
              STM32_ADCPRE | STM32_PPRE2    | STM32_PPRE1  | STM32_HPRE;
#else
  RCC->CFGR = STM32_MCO    |                  STM32_PLLMUL | STM32_PLLSRC |
              STM32_ADCPRE | STM32_PPRE2    | STM32_PPRE1  | STM32_HPRE;
#endif

  /* Flash setup and final clock selection.   */
  FLASH->ACR = STM32_FLASHBITS; /* Flash wait states depending on clock.    */

  /* Switching to the configured clock source if it is different from HSI.*/
#if (STM32_SW != STM32_SW_HSI)
  RCC->CFGR |= STM32_SW;        /* Switches on the selected clock source.   */
  while ((RCC->CFGR & RCC_CFGR_SWS) != (STM32_SW << 2))
    ;
#endif
#endif /* !STM32_NO_INIT */
}
#else
void stm32_clock_profile_switch(ClockProfile const *prf) {(void)prf;}
#endif

