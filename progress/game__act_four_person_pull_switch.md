# game/act_four_person_pull_switch.cpp

## Status
- `.text`: 99.49%
- `.ctors`: 100.00%
- `.rodata`: 100.00%
- `.data`: 100.00%
- `.bss`: 100.00%
- `.sdata2`: 92.86% (pool order)

## Functions
- `FourPersonPullSwitch_804AE230::~FourPersonPullSwitch_804AE230` (`0x8036CB5C`, 96B): 100.00%
- `FourPersonPullSwitch_804AE230::fn_8036CBBC` (`0x8036CBBC`, 668B): 99.54% (`operands: same opcodes, an immediate or offset differs` — stack offsets for `pos`/`playerPos` and loop register allocation `r28..r30`)
- `FourPersonPullSwitch_804AE230::draw` (`0x8036CE58`, 196B): 99.90% (reloc-name only, .sdata2 pool order)
- `FourPersonPullSwitch_804AE230::update` (`0x8036CF1C`, 1212B): 99.44% (operands + reloc-name) (`operands: same opcodes, an immediate or offset differs` — stack frame `0xa0` vs `0xb0` and register allocation `r29`/`r30`)
- `FourPersonPullSwitch_804AE230::init` (`0x8036D3D8`, 508B): left unwritten (98.77%, mr vs lwz opcode difference; attempt saved by the coordinator outside the repo).
- `__sinit_act_four_person_pull_switch_cpp` (`0x8036D5D4`, 548B): 100.00%

## Proven Facts
- `FourPersonPullSwitch_804AE230` size is `0x264` (`tu_801F9484.s` at `0x801FB640`: `li r3, 0x264` for `'PSW2'`).
- `fn_8036CBBC` is a member function `FourPersonPullSwitch_804AE230::fn_8036CBBC()` called from `update()` at `0x8036CF84` with `mr r3, r31`.
- `checkAllPulled()` is an `inline` helper inlined into `update()`. (Review: an earlier non-inline version emitted a bogus out-of-line copy only to reorder .sdata2 pooling; removed.)
