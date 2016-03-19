[#ftl]
[@pp.dropOutputFile /]
[#list conf.instance.sequences.sequence as sequence]
  [@pp.changeOutputFile name="test_sequence_" + (sequence_index + 1)?string("000") + ".h" /]
${conf.instance.description.copyright.value?trim}

extern const testcase_t * const test_sequence_${(sequence_index + 1)?string("000")}[];
[/#list]
