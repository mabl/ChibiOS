[#ftl]
[#import "/@ftllibs/libutils.ftl" as utils /]
[@pp.dropOutputFile /]
[@pp.changeOutputFile name="test_root.c" /]
/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

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
 * @mainpage Test Suite Specification
[#if conf.instance.introduction.value[0]?trim != ""]
[@utils.FormatStringAsText " * "
                           " * "
                           utils.WithDot(conf.instance.introduction.value[0]?trim?cap_first)
                           72 /]
[#else]
 * No introduction.
[/#if]
 *
 * <h2>Test Sequences</h2>
[#if conf.instance.sequences.sequence?size > 0]
  [#list conf.instance.sequences.sequence as sequence]
 * - @subpage test_sequence_${(sequence_index + 1)?string("000")}
  [/#list]
 * .
[#else]
 * No test sequences defined in the test suite.
[/#if]
 */

/**
 * @file    test_root.c
 * @brief   Test Suite root structures code.
 *
 * @addtogroup SPC5_TEST_ROOT
 * @{
 */

#include "hal.h"
#include "ch_test.h"
#include "test_root.h"

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   Array of all the test sequences.
 */
const testcase_t * const *test_suite[] = {
[#list conf.instance.sequences.sequence as sequence]
  test_sequence_${(sequence_index + 1)?string("000")},
[/#list]
  NULL
};

/*===========================================================================*/
/* Shared code.                                                              */
/*===========================================================================*/

[#if conf.instance.global_data_and_code.global_code.value[0]?trim?length > 0]
${conf.instance.global_data_and_code.global_code.value[0]}

[/#if]
/** @} */
