[#ftl]
[@pp.dropOutputFile /]
[@pp.changeOutputFile name="osal_cfg.c" /]
/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013 Giovanni Di Sirio.

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    osal_cfg.c
 * @brief   OSAL configuration code.
 *
 * @addtogroup OSAL
 * @{
 */

#include "osal.h"

[#if conf.instance.isrs.headers.value[0]??]
  [#assign s = conf.instance.isrs.headers.value[0]?trim /]
  [#if s?length != 0]
    [#list s?split(",") as header]
#include "${header}"
    [/#list]

  [/#if]
[/#if]
/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module ISRs.                                                              */
/*===========================================================================*/

[#list conf.instance.isrs.enabled_vectors.vector as vector]
  [#if vector.isr_code.value[0]?trim?length != 0]
${vector.isr_code.value[0]?trim}
  [/#if]

[/#list]
/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Generated initialization code.
 *
 * @special
 */
void osal_cfg_init(void) {

[#list conf.instance.isrs.enabled_vectors.vector as vector]
  [#assign id = vector.identifier.value[0]?trim /]
  [#if vector.cores.value[0]??]
    [#assign cores = vector.cores.value[0]?replace(" ","")?replace("-","")?upper_case /]
    [#assign psr = "INTC_PSR_ENABLE(INTC_PSR_" + cores + ", " + vector.priority.value[0] + ")" /]
  [#else]
    [#assign psr = "INTC_PSR_ENABLE(INTC_PSR_CORE0, " + vector.priority.value[0] + ")"/]
  [/#if]
  osalIsrEnableVector(OSAL_VECTOR_${id},
                      ${psr});
[/#list]
}

/** @} */
