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

#define STM32_PPRE1_TEST (CLK_CFG->ppre1_div)

/**
 * @brief   Configuration structure.
 */
typedef struct {
  uint32_t                  sw;
  uint32_t                  hpre_div;
  uint32_t                  ppre1_div;
  uint32_t                  ppre2_div;
  uint32_t                  adcpre_div;
  uint32_t                  pll_src;
  uint32_t                  pllxtpre;
  uint32_t                  usbpre;
  uint32_t                  mcosel;
} ClockConfig;

extern ClockConfig *CLK_CFG;

#endif /* _DYN_CLK_LLD_H_ */

/** @} */
