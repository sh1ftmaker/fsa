# JSystem/JMessage/processor.cpp

Baseline (start): `total: 3711 / 11561 functions, 710408 / 4442088 bytes of code (15.99 %)`
Now:              `total: 3744 / 11561 functions, 713292 / 4442088 bytes of code (16.06 %)`

Unit: 24/56 -> 56/56 functions, .text and .data 100 %, `Matching`, gate ok (link SHA-1 ok).

## What made it match
- FSA's TProcessor vtable is tp's layout (plus do_systemTagCode), not pikmin2's. Slots read
  from `__vt__Q28JMessage10TProcessor` / TSequenceProcessor / TRenderingProcessor and the stub
  bodies: ~TProcessor, do_reset, do_begin, do_end, do_character, do_tag (li r3,0),
  do_systemTagCode (li r3,0), do_select_begin, do_select_end, do_select_separate,
  do_reset_ (non-pure, blr), do_setBegin_isReady_ (li r3,1), then 4 pure (do_begin_, do_end_,
  do_tag_, do_systemTagCode_). TSequenceProcessor/TRenderingProcessor don't override
  do_begin/do_end; TRenderingProcessor doesn't override do_reset_.
  (dtk sized `__vt__Q28JMessage10TProcessor` 0x40 + an 8-byte gap; the real vtable is 0x48.
  objdiff and the link are fine with it, so the size is left alone.)
- Header (libs/sdk/include/JSystem/JMessage/TProcessor.h, used only by the JMessage units):
  reset_(text) became an inline (tp's on_resetStatus_), plus on_begin/on_end (do_X_ then do_X),
  pushCurrent_/popCurrent_ (tp's stack_pushCurrent_/stack_popCurrent_).
- `reset()` (0x800778DC) = reset_(NULL) + do_reset() (vtable calls 0x30, 0x0C).
- on_select_begin/end/separate and process_onCharacterEnd_select_ follow tp (unchecked push).
- process_onCharacterEnd_normal_ calls on_end(); on_jump/on_branch call reset_ then do_jump/do_branch.
- TSequenceProcessor::process compares the branch result against 0x7FFFFFFF (tp).
- TSequenceProcessor::do_begin_ is empty, do_end_ only sets STATUS_End; TRendering's are empty.
- 31 placeholder renames in symbols.txt (vtable slots + call sites), incl. lbl_80498B00 ->
  `__vt__Q28JMessage10TReference` (holds ~TReference and do_word).

## SHARED-EDITS
- None outside the JMessage units. TProcessor.h is shared by processor.cpp and control.cpp
  (both mine); control.cpp still compiles and did not regress.
