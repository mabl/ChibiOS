[#ftl]
[#import "/@ftllibs/libutils.ftl" as utils /]
[@pp.dropOutputFile /]
[@pp.changeOutputFile name="test_root.c" /]
${conf.instance.description.copyright.value?trim}

/**
 * @mainpage Test Suite Specification
[#if conf.instance.description.introduction.value[0]?trim != ""]
[@utils.FormatStringAsText " * "
                           " * "
                           utils.WithDot(conf.instance.description.introduction.value[0]?trim?cap_first)
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
 */

#include "hal.h"
#include "ch_test.h"
#include "test_root.h"

#if !defined(__DOXYGEN__)

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
#endif /* !defined(__DOXYGEN__) */
