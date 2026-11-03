# JSystem/JAudio/Interface/JAIAudible.cpp

Baseline (progress.py): total 4784 / 11561 functions, 863464 / 4442088 bytes (19.44 %).
After: total 4785 / 11561 functions, 863676 bytes (19.44 %). gate: ok.

Unit: 1/2 -> 2/2 functions; .text, .ctors, .bss 100 %, .data 92.21 % (see SHARED-EDITS).
`link_check --unit` SHA-1 OK. Still NonMatching only because of the vtable size below.

## Matched
- fn_8002769C = `__sinit_JAIAudible_cpp` (the unit's .ctors entry; TChannelMgr/
  JASTrack/JASChannel memPool_s).
- .bss 0x804B2470-0x804B2498 split into this unit (the __register_global_object
  chain); placeholder lbl_804B2470 (0x28) split into three 0xC placeholders
  lbl_804B2470/7C/88 (+4 bytes pad).
- .data 0x80491880-0x80491898 split into this unit: `__vt__10JAIAudible` is emitted
  by this TU (key function ~JAIAudible). Without it the Matching link failed with
  "dup JAIAudible::__vt".

## Resistant
- .data 92.21 %: `__vt__10JAIAudible` is 0x18 in symbols.txt, but the vtable is
  2 header words + ~JAIAudible + getOuterParams + calc = 0x14 (tp GZ2E01/GZ2P01/
  GZ2J01/ShieldD and mkdd symbols.txt all 0x14); the DOL word at +0x14 is 0, the
  alignment pad before `__vt__13JASTaskThread` at 0x80491898 (.balign 8). Verified
  locally: with size 0x14, .data is 100 % (change reverted; it's a real symbol).

## SHARED-EDITS
- approved_symbol_changes.txt / symbols.txt (needs review):
  `__vt__10JAIAudible  0x18  0x14` (evidence above; same pattern as
  `__vt__11J3DShapeMtx` and `__vt__7J2DPane`). After it, flip JAIAudible.cpp to
  Matching (link_check --unit already SHA-1 OK).
- 0x80491870 `__vt__16JAIStreamDataMgr` (0x10) is in no unit's .data; it belongs to a
  JAIStreamDataMgr.cpp TU (source exists in libs/jsystem), like JAISeqDataMgr.
