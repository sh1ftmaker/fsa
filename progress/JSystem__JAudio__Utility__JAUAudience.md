# JSystem/JAudio/Utility/JAUAudience.cpp

Baseline (progress.py): 3711 / 11561 functions, 710408 bytes (15.99 %).
After: 3717 / 11561, 711320 bytes (16.01 %). gate: ok. Per-unit honest comparison
against the baseline report: only JAUAudience (3 -> 8) and JAIAudible (0 -> 1) changed.

Unit: 3/18 -> 8/18 functions. .data 82 %, .sdata2 73.5 %.

## Matched
- `__ct__23JAUAudience_withSettingFv` (fn_8003C984): FSA has the ctor out of line right
  after updateSetting (with updateSetting inlined into it), and game code calls it
  (auto_fn_80459AE4). Moved the body from the header into the .cpp.
- Vtables from the dtor chains (most derived first): `__vt__21JAUDopplerAudience<4>`
  (804954D8), `__vt__98JAUAudience_<...>` (804954F8), `__vt__31JAUAudience_withSetting_doppler`
  (80495480), `__vt__23JAUAudience_withSetting` (80495450), `__vt__20JAUDopplerAudible<4>`
  (8049552C), `__vt__74JAUGenericAudible_<...>` (80495518); bases `__dt__11JAIAudienceFv`
  (fn_80021D60, stores `__vt__11JAIAudience` = lbl_804916A0), `__dt__10JAIAudibleFv`
  (fn_80027654 = JAIAudible.cpp's only defined function, stores `__vt__10JAIAudible` =
  lbl_80491880). `fn_8003D200` = `__dt__74JAUGenericAudible_<...>Fv` (generic vtable,
  then ~JAIAudible; same bytes as ours).
- `lbl_804B21DC` = `JASPoolAllocObject<JAUDopplerAudible<4>>::memPool_` (newAudible alloc,
  ~JAUDopplerAudible free), guard lbl_8053A2AF, dtor fn_80021C10. `__sinit_JAUAudience_cpp`.
- Splits: .data 0x804954D8-0x80495540, .bss 0x804B4C28-0x804B4C98 (__sinit chain), .sdata2 0x8053B2B0-0x8053B2F0.

## Resistant (layout, not names)
- FSA's `JAUDopplerAudible<4>` is 0x40 bytes (dtor frees 0x40; ours 0x10C), and
  `getOuterParams` reads the channel pointer at +0x18 (ours +0x2C). The
  JAUGenericAudible_/JAUDopplerAudible templates in JAUAudience.h need FSA's layout
  before mixChannelOut (fn_8003CBC8, 0x23C), calcPriority (fn_8003CE20, 0x164),
  newAudible (fn_8003D03C, 0x1C4) can match; ours are 0x330 / 0x2EC / 0x394.
- `__sinit_JAUAudience_cpp`: 9 pools (3 MT, then 6 per-TU at 8052E78C.. / 804B21EC /
  804B21DC; dtors fn_8003D488..D584 are local copies). Waits for the memPool_ work,
  and the six non-MT pools' classes are not identified with evidence yet (tp's dummies
  suggest JAUAudible<1,2,4> then JAUDopplerAudible<1,2,4>; the last is confirmed).

## SHARED-EDITS
- libs/sdk/include/JSystem/JAudio/JAUAudience.h: ctor of JAUAudience_withSetting
  declared only (body in JAUAudience.cpp). Separate commit; no other unit changed.
- symbols.txt names in other units: JAIAudience/JAIAudible vtables and dtors.
- FIXMEs added (review): JAUAudience.h at JAUGenericAudible_, mixChannelOut, calcPriority, newAudible; JAUAudience.cpp at the end for __sinit.
