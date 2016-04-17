[#ftl]
[#import "/@ftllibs/libutils.ftl" as utils /]
[@pp.dropOutputFile /]
[#list conf.instance.sequences.sequence as sequence]
  [@pp.changeOutputFile name="test_sequence_" + (sequence_index + 1)?string("000") + ".h" /]
[@utils.EmitIndentedCCode "" 2 conf.instance.description.copyright.value[0] /]

/**
 * @file    test_sequence_${(sequence_index + 1)?string("000")}.h
 * @brief   Test Sequence ${(sequence_index + 1)?string("000")} header.
 */

#ifndef TEST_SEQUENCE_${(sequence_index + 1)?string("000")}_H
#define TEST_SEQUENCE_${(sequence_index + 1)?string("000")}_H

extern const testcase_t * const test_sequence_${(sequence_index + 1)?string("000")}[];

#endif /* TEST_SEQUENCE_${(sequence_index + 1)?string("000")}_H */
[/#list]
