[#ftl]
[@pp.dropOutputFile /]
[@pp.changeOutputFile name="board.c" /]
/*
 * Licensed under ST Liberty SW License Agreement V2, (the "License");
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *        http://www.st.com/software_license_agreement_liberty_v2
 *
 * Unless required by applicable law or agreed to in writing, software 
 * distributed under the License is distributed on an "AS IS" BASIS, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_PAL || defined(__DOXYGEN__)
/* Initial setup of all defined pads, the list is terminated by a {0, 0, 0}.*/
static const spc_siu_init_t spc_siu_init[] = {
[#list conf.groups.i_o_settings.pins_list.pin_settings as pin_settings]
  [#assign pcrstr = ("PCR(" + pin_settings.pin_identification.port[0] +
                     ", "   + pin_settings.pin_identification.identifier[0] +
                     "),")?right_pad(24) /]
  [#assign levelstr = ("PAL_" + pin_settings.settings.latched_state[0] + ",")?right_pad(11) /]
  [#assign modestr = "PAL_" + pin_settings.settings.pin_mode[0] /]
  {${pcrstr} ${levelstr} ${modestr}},
[/#list]
  {0, 0, 0}
};

/* Initialization array for the PSMI registers.*/
static const uint8_t spc_padsels_init[SPC5_SIUL_NUM_PADSELS] = {
[#list conf.groups.i_o_settings.alternate_inputs.psmi_register_settings as psmi]
  [#assign padsel0 = (psmi.padsel_fields.padsel0[0] + ",")?right_pad(5) /]
  [#assign padsel1 = (psmi.padsel_fields.padsel1[0] + ",")?right_pad(5) /]
  [#assign padsel2 = (psmi.padsel_fields.padsel2[0] + ",")?right_pad(5) /]
  [#assign padsel3 = psmi.padsel_fields.padsel3[0] + "," /]
  ${padsel0} ${padsel1} ${padsel2} ${padsel3}
[/#list]
};

/**
 * @brief   PAL setup.
 */
const PALConfig pal_default_config =
{
  PAL_${conf.groups.i_o_settings.unused_pins_mode[0]},
  spc_siu_init,
  spc_padsels_init
};
#endif

/*
 * Early initialization code.
 * This initialization must be performed just after stack setup and before
 * any other initialization.
 */
void __early_init(void) {

  spc_clock_init();
[#if conf.groups.initialization_code.early_initialization_code[0]?trim != ""]

${conf.groups.initialization_code.early_initialization_code[0]}
[/#if]
}

[#if conf.groups.initialization_code.late_initialization_code[0]?trim != ""]
/*
 * Late initialization code.
 */
void __late_init(void) {

${conf.groups.initialization_code.late_initialization_code[0]}
}
[/#if]

/*
 * Board-specific initialization code.
 */
void boardInit(void) {
[#if conf.groups.initialization_code.board_initialization_code[0]?trim != ""]

${conf.groups.initialization_code.board_initialization_code[0]}
[/#if]
}
