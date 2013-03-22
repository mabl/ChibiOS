[#ftl]
[@pp.dropOutputFile /]
[@pp.changeOutputFile name="adc_lld_cfg.c" /]
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

/**
 * @file    adc_lld_cfg.c
 * @brief   ADC Driver configuration code.
 *
 * @addtogroup ADC
 * @{
 */

#include "ch.h"
#include "hal.h"
#include "adc_lld_cfg.h"

#if HAL_USE_ADC || defined(__DOXYGEN__)

/* Forward declarations.*/
[#list conf.instance.conversion_groups.groups.conversion_group_settings as group]
static const adccommand_t adc_${group.symbolic_name[0]}_commands[];
[/#list]

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

[#list conf.instance.conversion_groups.groups.conversion_group_settings as group]
  [#assign name = group.symbolic_name[0] /]
  [#assign mode = group.triggers.mode[0].@index[0]?number /]
  [#assign conv_cb = group.notifications.conversion_callback[0]?string?trim /]
  [#if conv_cb == ""]
    [#assign conv_cb = "NULL" /]
  [/#if]
  [#assign err_cb = group.notifications.error_callback[0]?string?trim /]
  [#if err_cb == ""]
    [#assign err_cb = "NULL" /]
  [/#if]
/**
 * @brief   Structure defining the conversion group "${group.symbolic_name[0]}".
 */
const ADCConversionGroup adc_group_${group.symbolic_name[0]?trim} = {
  ${(group.conversion_mode[0].@index[0]?number != 0)?string?upper_case},
  ADC_GROUP_${group.symbolic_name[0]?upper_case}_NUM_CHANNELS,
  ${conv_cb},
  ${err_cb},
  [#if mode == 0]
  EQADC_CFCR_MODE_SWCS,
  [#elseif mode == 1]
  EQADC_CFCR_MODE_HWCS_LL,
  [#elseif mode == 2]
  EQADC_CFCR_MODE_HWCS_HL,
  [#elseif mode == 3]
  EQADC_CFCR_MODE_HWCS_FE,
  [#elseif mode == 4]
  EQADC_CFCR_MODE_HWCS_RE,
  [#else]
  EQADC_CFCR_MODE_HWCS_BE,
  [/#if]
  ${group.triggers.tsel[0].@index[0]}, ${group.triggers.etsel[0]},
  ADC_GROUP_${name?upper_case}_BUF_DEPTH,
  adc_${name}_commands
};

[/#list]
/*===========================================================================*/
/* Driver local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

[#list conf.instance.conversion_groups.groups.conversion_group_settings as group]
  [#assign FIFO = group.adc_driver[0].@index[0]?number /]
  [#if FIFO < 3]
    [#assign ADC = "ADC0" /]
  [#else]
    [#assign ADC = "ADC1" /]
  [/#if]
  [#assign mode = group.triggers.mode[0].@index[0]?number /]
  [#if mode == 0]
    [#assign TRIGGER_MODE = "" /]
  [#else]
    [#assign TRIGGER_MODE = " | EQADC_CONV_PAUSE" /]
  [/#if]
  [#assign num_channels = group.maximum_sequential[0]?number /]
/*
 * eQADC commands for conversion group "${group.symbolic_name[0]}".
 */
static const adccommand_t adc_${group.symbolic_name[0]}_commands[ADC_GROUP_${group.symbolic_name[0]?upper_case}_NUM_COMMANDS] = {
  [#list 1..num_channels as n]
    [#list group.channels.sequence.sequence as channel_settings]
      [#assign CHANNEL = channel_settings.channel[0]?string /]
      [#assign CYCLES = channel_settings.sampling_time[0]?string /]
      [#if channel_settings.calibrated[0]?lower_case == "true"]
        [#assign CAL = "EQADC_CONV_CAL" /]
      [#else]
        [#assign CAL = "0" /]
      [/#if]
      [#if channel_settings.signed_result[0]?lower_case == "true"]
        [#assign SIGNED = "EQADC_CONV_FMT_RJS" /]
      [#else]
        [#assign SIGNED = "EQADC_CONV_FMT_RJU" /]
      [/#if]
      [#assign size = channel_settings.sample_size[0]?number /]
      [#if size == 12]
        [#assign SAMPLE = "EQADC_CONV_CONFIG_STD" /]
      [#elseif size == 10]
        [#assign SAMPLE = "EQADC_CONV_CONFIG_SEL1" /]
      [#else]
        [#assign SAMPLE = "EQADC_CONV_CONFIG_SEL2" /]
      [/#if]

  EQADC_CONV_BN_${ADC} | EQADC_CONV_LST_${CYCLES} | ${CAL} |
  ${SIGNED} | ${SAMPLE} | EQADC_CONV_MSG_RFIFO(${FIFO}) |
  EQADC_CONV_CHANNEL(ADC_CHN_${CHANNEL})[#rt]
      [#if channel_settings_has_next]
,
      [#else]
${TRIGGER_MODE},
      [/#if]
    [/#list]
  [/#list]
};

[/#list]
/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

#endif /* HAL_USE_ADC */

/** @} */
