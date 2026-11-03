# JSystem/JSupport/JSUOutputStream.cpp

Status: Matching (1/1, .text and .data 100 %, link_check --unit SHA-1 OK).

- Identified by the review of tu_80086A54: pikmin2 (GPVE01 splits.txt)
  links JSUOutputStream.cpp directly before JSUInputStream.cpp, and its
  .text ends in the weak __dt__10JSUIosBaseFv with .data __vt__10JSUIosBase.
- Source: mkdd's file as is (already in libs/jsystem). Its own
  ~JSUOutputStream and both write() are dead-stripped by FSA's link; only the
  weak JSUIosBase dtor (0x80086A54) and vtable (0x80499018) remain.
- symbols.txt: __dt__10JSUIosBaseFv marked scope:weak.
- lib_map.tsv: the 0x80085968-0x80086AF8 "JKRTask" row is now two rows,
  JKRUnitHeap.cpp and JSUOutputStream.cpp.
