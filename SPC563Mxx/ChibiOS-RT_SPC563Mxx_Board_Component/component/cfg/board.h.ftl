[#ftl]
[@pp.dropOutputFile /]
[@pp.changeOutputFile name="board.h" /]
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

#ifndef _BOARD_H_
#define _BOARD_H_

/*
 * Setup for a generic SPC563Mxx proto board.
 */

/*
 * Board identifiers.
 */
#define BOARD_${conf.groups.identification.board_identifier[0]}
#define BOARD_NAME                  "${conf.groups.identification.board_name[0]}"

/*
 * Board frequencies.
 */
#if !defined(SPC5_XOSC_CLK)
#define SPC5_XOSC_CLK               ${conf.groups.clock_settings.xosc_clock[0]}
#endif

/*
 * I/O definitions.
 */
[#list conf.groups.i_o_settings.pins_list.pin_settings as pin_settings]
  [#assign macro_name = (pin_settings.pin_identification.identifier[0])?right_pad(27) /]
#define ${macro_name} ${pin_settings.pin_identification.bit[0]}
[/#list]

/*
 * Support macros.
 */
#define PCR(port, pin)  (((port) * 16) + (pin))

#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* _BOARD_H_ */
