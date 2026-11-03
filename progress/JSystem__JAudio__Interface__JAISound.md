# JSystem/JAudio/Interface/JAISound.cpp

Baseline (progress.py): total 4784 / 11561 functions, 863464 / 4442088 bytes (19.44 %).
After: total 4787 / 11561 functions, 864088 bytes (19.45 %). gate: ok.

Unit: 12/15 -> 15/15 functions; .text, .ctors, .rodata, .data, .bss, .sdata2 100 %.
`link_check --unit` SHA-1 OK; flipped to Matching.

## Matched
- mixOutAll: FSA multiplies the pitch by the property's pitch (`mProperty._8`),
  like tp's JAudio2 (mkdd drops it): target loads 0x8(r3) and 0x14(r3).
- start_JAISound_: FSA's `JAISoundParamsProperty::init()` sets `_8 = 1.0f` (tp's
  value; the header said "0.0f; // 1.0f in TP"): target stores f2 (1.0) at 0x44.
  Header JAISoundParams.h changed; full honest report diffed against the base
  build: only JAISound changes (only this unit inlines params_.init()).
- fn_80025C94 = `__sinit_JAISound_cpp` (.ctors entry; TChannelMgr/JASTrack/
  JASChannel memPool_s). .bss 0x804B2358-0x804B2380 split into this unit; the
  placeholder lbl_804B2358 (0x28) split into 0xC placeholders lbl_804B2358/64/70.
- .sdata2: placeholder lbl_8053AFF8 resized 0x8 -> 0x4 (it is the float 0.01; the
  other 4 bytes are the section's alignment pad before the next unit). The same
  fix should lift JAISeq's .sdata2 (lbl_8053AFC8, 85.71 %).

## Resistant
None.

## SHARED-EDITS
- libs/sdk/include/JSystem/JAudio/Interface/JAISoundParams.h: `_8 = 1.0f` in
  JAISoundParamsProperty::init() (no other unit changes in the honest report).
