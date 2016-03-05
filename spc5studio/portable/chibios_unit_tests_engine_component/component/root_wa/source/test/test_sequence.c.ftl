[#ftl]
[#import "/@ftllibs/libutils.ftl" as utils /]
[@pp.dropOutputFile /]
[#list conf.instance.sequences.sequence as sequence]
  [@pp.changeOutputFile name="test_sequence_" + (sequence_index + 1)?string("000") + ".c" /]
/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio

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

#include "hal.h"
#include "ch_test.h"
#include "test_root.h"

/**
 * @page test_sequence_${(sequence_index + 1)?string("000")} ${utils.WithoutDot(sequence.brief.value[0]?string)}
 *
 * File: @ref test_sequence_${(sequence_index + 1)?string("000")}.c
 *
 * <h2>Description</h2>
[@utils.FormatStringAsText " * "
                           " * "
                           utils.WithDot(sequence.description.value[0]?string)
                           72 /]
 *
 * <h2>Test Cases</h2>
  [#if sequence.cases.case?size > 0]
    [#list sequence.cases.case as case]
 * - @subpage test_${(sequence_index + 1)?string("000")}_${(case_index + 1)?string("000")}
    [/#list]
 * .
  [#else]
 * No test cases defined in the test sequence.
  [/#if]
 */

/****************************************************************************
 * Shared code.
 ****************************************************************************/

  [#if sequence.shared_code.value[0]?trim?length > 0]
${sequence.shared_code.value[0]?trim}
  [/#if]

/****************************************************************************
 * Test cases.
 ****************************************************************************/

  [#list sequence.cases.case as case]
    [#-- Building the sequence of the requirements covered by
         this test case.--]
    [#assign reqseq = [] /]
    [#list case.steps.step as step]
        [#assign reqseq = reqseq + step.tags.value[0]?string?trim?word_list /]
    [/#list]
    [#assign reqseq = reqseq?sort /]
    [#-- Checking if a condition should be generated.--]
    [#if case.condition.value[0]?trim?length > 0]
#if ${case.condition.value[0]?trim} || defined(__DOXYGEN__)
    [/#if]
    [#-- Header generation.--]
/**
 * @page test_${(sequence_index + 1)?string("000")}_${(case_index + 1)?string("000")} ${utils.WithoutDot(case.brief.value[0])}
 *
 * <h2>Description</h2>
[@utils.FormatStringAsText " * "
                           " * "
                           utils.WithDot(case.description.value[0]?string)
                           72 /]
 *
    [#if case.condition.value[0]?trim?length > 0]
 * <h2>Conditions</h2>
 * This test is only executed if the following preprocessor condition
 * evaluates to true:
 * - ${case.condition.value[0]}
 * .
 *
    [/#if]
 * <h2>Test Steps</h2>
    [#list case.steps.step as step]
[@utils.FormatStringAsText " * - "
                           " *   "
                           utils.WithDot(step.description.value[0]?string)
                           72 /]
    [/#list]
    [#if case.steps.step?size > 0]
 * .
    [/#if]
    [#if reqseq?size > 0]
 * <h2>Covered Requirements</h2>
    [#assign reqs = "" /]
    [#list reqseq as r]
      [#assign reqs = reqs + r /]
      [#if r_has_next]
        [#assign reqs = reqs + ", " /]
      [/#if]
    [/#list]
[@utils.FormatStringAsText " * "
                           " * "
                           utils.WithDot(reqs)
                           72 /]
    [/#if]
 */

    [#if case.various_code.setup_code.value[0]?trim?length > 0]
static void test_${(sequence_index + 1)?string("000")}_${(case_index + 1)?string("000")}_setup(void) {
[@utils.EmitIndentedCCode "  " 2 case.various_code.setup_code.value[0] /]
}

    [/#if]
    [#if case.various_code.teardown_code.value[0]?trim?length > 0]
static void test_${(sequence_index + 1)?string("000")}_${(case_index + 1)?string("000")}_teardown(void) {
[@utils.EmitIndentedCCode "  " 2 case.various_code.teardown_code.value[0] /]
}

    [/#if]
static void test_${(sequence_index + 1)?string("000")}_${(case_index + 1)?string("000")}_execute(void) {
    [#if case.various_code.local_variables.value[0]?trim?length > 0]
[@utils.EmitIndentedCCode "  " 2 case.various_code.local_variables.value[0] /]
    [/#if]
    [#list case.steps.step as step]

[@utils.FormatStringAsText "  /* "
                           "     "
                           utils.WithDot(step.description.value[0]?string) + "*/"
                           72 /]
  test_set_step(${(step_index + 1)?string});
  {
      [#if step.tags.value[0]?string?trim != ""]
        [#assign reqseq = step.tags.value[0]?string?trim?word_list?sort /]
        [#assign reqs = "" /]
        [#list reqseq as r]
          [#assign reqs = reqs + r /]
          [#if r_has_next]
            [#assign reqs = reqs + ", " /]
          [/#if]
        [/#list]
[@utils.FormatStringAsText "    /* @covers "
                           "               "
                           utils.WithDot(reqs) + "*/"
                           72 /]
      [/#if]
      [#if step.code.value[0]?trim?length > 0]
[@utils.EmitIndentedCCode "    " 2 step.code.value[0] /]
      [/#if]
  }
    [/#list]
}

static const testcase_t test_${(sequence_index + 1)?string("000")}_${(case_index + 1)?string("000")} = {
  "${utils.WithoutDot(case.brief.value[0]?string)}",
    [#if case.various_code.setup_code.value[0]?trim?length > 0]
  test_${(sequence_index + 1)?string("000")}_${(case_index + 1)?string("000")}_setup,
    [#else]
  NULL,
    [/#if]
    [#if case.various_code.teardown_code.value[0]?trim?length > 0]
  test_${(sequence_index + 1)?string("000")}_${(case_index + 1)?string("000")}_teardown,
    [#else]
  NULL,
    [/#if]
  test_${(sequence_index + 1)?string("000")}_${(case_index + 1)?string("000")}_execute
};
    [#if case.condition.value[0]?trim?length > 0]
#endif /* ${case.condition.value[0]?trim} */
    [/#if]

  [/#list]
/****************************************************************************
 * Exported data.
 ****************************************************************************/

/**
 * @brief   ${utils.WithDot(sequence.brief.value[0]?string)}
 */
const testcase_t * const test_sequence_${(sequence_index + 1)?string("000")}[] = {
  [#list sequence.cases.case as case]
   [#if case.condition.value[0]?trim?length > 0]
#if ${case.condition.value[0]?trim} || defined(__DOXYGEN__)
    [/#if]
  &test_${(sequence_index + 1)?string("000")}_${(case_index + 1)?string("000")},
    [#if case.condition.value[0]?trim?length > 0]
#endif
    [/#if]
  [/#list]
  NULL
};
[/#list]
