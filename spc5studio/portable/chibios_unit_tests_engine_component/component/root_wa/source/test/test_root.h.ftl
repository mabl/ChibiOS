[#ftl]
[@pp.dropOutputFile /]
[@pp.changeOutputFile name="test_root.h" /]
${conf.instance.description.copyright.value?trim}

/**
 * @file    test_root.h
 * @brief   Test Suite root structures header.
 */

#ifndef _SPC5_TEST_ROOT_H_
#define _SPC5_TEST_ROOT_H_

[#list conf.instance.sequences.sequence as sequence]
#include "test_sequence_${(sequence_index + 1)?string("000")}.h"
[/#list]

#if !defined(__DOXYGEN__)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

extern const testcase_t * const *test_suite[];

#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Shared definitions.                                                       */
/*===========================================================================*/

[#if conf.instance.global_data_and_code.global_definitions.value[0]?trim?length > 0]
${conf.instance.global_data_and_code.global_definitions.value[0]}

[/#if]
#endif /* !defined(__DOXYGEN__) */

#endif /* _SPC5_TEST_ROOT_H_ */
