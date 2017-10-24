[#ftl]
[#import "/@ftllibs/libutils.ftl" as utils /]
[#assign prefix_lower = conf.instance.global_data_and_code.code_prefix.value[0]?trim?lower_case /]
[#assign prefix_upper = conf.instance.global_data_and_code.code_prefix.value[0]?trim?upper_case /]
[@pp.dropOutputFile /]
[#list conf.instance.sequences.sequence as sequence]
  [@pp.changeOutputFile name=prefix_lower+"test_sequence_" + (sequence_index + 1)?string("000") + ".h" /]
[@utils.EmitIndentedCCode "" 2 conf.instance.description.copyright.value[0] /]

/**
 * @file    ${prefix_lower}test_sequence_${(sequence_index + 1)?string("000")}.h
 * @brief   Test Sequence ${(sequence_index + 1)?string("000")} header.
 */

#ifndef ${prefix_upper}TEST_SEQUENCE_${(sequence_index + 1)?string("000")}_H
#define ${prefix_upper}TEST_SEQUENCE_${(sequence_index + 1)?string("000")}_H

extern const testsequence_t ${prefix_lower}test_sequence_${(sequence_index + 1)?string("000")};

#endif /* ${prefix_upper}TEST_SEQUENCE_${(sequence_index + 1)?string("000")}_H */
[/#list]
