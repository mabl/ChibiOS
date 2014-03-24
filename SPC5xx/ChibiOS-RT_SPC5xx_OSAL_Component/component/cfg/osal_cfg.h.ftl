[#ftl]
[@pp.dropOutputFile /]
[@pp.changeOutputFile name="osal_cfg.h" /]
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
 * @file    osal_cfg.h
 * @brief   OSAL configuration macros and structures.
 *
 * @addtogroup OSAL
 * @{
 */

#ifndef _OSAL_CFG_H_
#define _OSAL_CFG_H_

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    ISR vectors identifiers
 * @{
 */
[#assign ids = [] /]
[#assign numbers = [] /]
[#list conf.instance.isrs.enabled_vectors.vector as vector]
  [#assign id = vector.identifier.value[0]?trim /]
  [#if ids?seq_contains(id)]
    [#stop "Multiple definitions of ISR Vector Identifier: " + id /]
  [/#if]
  [#assign ids = ids + [id] /]
  [#assign number = vector.number.value[0]?trim /]
  [#if numbers?seq_contains(number)]
    [#stop "Multiple definitions of ISR Vector Number: " + number /]
  [/#if]
  [#assign numbers = numbers + [number] /]
  [#assign def = ("#define OSAL_VECTOR_" + id)?right_pad(43) /]
${def} ${number}
[/#list]
/** @} */

/**
 * @name    ISR vectors names overrides
 * @{
 */
[#assign names = [] /]
[#list conf.instance.isrs.enabled_vectors.vector as vector]
  [#assign name = vector.name_override.value[0]?trim /]
  [#if name != ""]
    [#if names?seq_contains(name)]
      [#stop "Multiple definitions of ISR Vector Name Override: " + name /]
    [/#if]
    [#assign names = names + [name] /]
    [#assign def = ("#define vector" + vector.number.value[0]?trim)?right_pad(43) /]
${def} ${name}
  [/#if]
[/#list]
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void osal_cfg_init(void);
#ifdef __cplusplus
}
#endif
#endif /* !defined(_FROM_ASM_) */

#endif /* _OSAL_CFG_H_ */

/** @} */
