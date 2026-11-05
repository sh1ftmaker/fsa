# game/tu_803F0A54.cpp

Baseline (progress.py before): `total: 7123 / 11557 functions, 1392832 / 4442032 bytes of code (31.36 %)`

What it is: cls_804B0F10 (vtable 0x804B0F10, ctor 0x803FCBC4, 0x354 bytes), the
multiplayer GBA-connect screen that cls_804AF878 (tu_803A85E4) builds as its
field_0x8 when lbl_8053AAF8->field_0x7f is clear. Seven J2DScreens from the
fn_804031C4 archive, a 13-state machine through three static PMF tables
(enter lbl_804B0BDC, update lbl_804B0D14, draw lbl_804B0E4C; state 13 runs
field_0x1c8's entries), a full-screen fade J2DPicture (field_0x5c), four
cls_804B20F8 sound objects. The tables are copied at boot by __sinit from
their @NNN initializers (MWCC does this for PMF tables of a class with
virtuals) — that copy is the boot-path code.

## Matched (100 %)
__sinit_tu_803F0A54_cpp (boot path), fn_803F1C18, fn_803F1C5C, fn_803F2660,
fn_803F2AEC, fn_803F2B48, fn_803F2B5C, fn_803F32E8, fn_803F3310,
fn_803F358C, fn_803F59A0, fn_803F8050, fn_803FC304, __dt__12cls_804B0F10Fv.
.bss 100 % (9 @NNN dtor chains + 9 weak memPool copies split per COORDINATOR).

## Resistant
- fn_803F3324 99.74, fn_803F715C 99.56, fn_803FC46C 99.79, fn_803FC5E4 99.80:
  reloc-name only. The .sdata2 literal pool follows first use; fn_803F0A54
  (0x11C4, first in the file, 49 literal loads) and the other unwritten
  functions come first, so our @NNN literals sit at other addresses.
  They match once every earlier function exists.
- fn_803F2580 98.04, fn_803FB6F4 99.41 (`different`, regalloc): setEntries'
  loop gets i / 2*i in swapped registers (orig: zero already live -> 2*i,
  fresh li -> i). fn_803F8050 inlines fn_803F2580 and matches.

## Not written (18)
fn_803F0A54 (sprite draw, SpriteParams on stack, fn_802F86CC), fn_803F1C70,
fn_803F267C, fn_803F2B78, fn_803F35B0, fn_803F38A4, fn_803F4120,
fn_803F5410, fn_803F5A0C, fn_803F726C, fn_803F8354, fn_803F8AE4,
fn_803F8FBC, fn_803F943C, fn_803F9EA8, fn_803FAA88, fn_803FB9E4, ctor.
.rodata lbl_80478A60 (0x7D8) is still one blob: float tables, then u64 pane
tag tables (ctor loops at +0xB8, +0x158, +0x1E8, +0x2A8, +0x918), split it
when writing the ctor. lbl_804B0EE8 (kk_rupyNum_*.bti names) is used by
fn_803F1C70 / fn_803F5A0C. Ctor note: its second 0x273 loop runs 4 rows over
a [3][6] array (original overrun into field_0x285).

## SHARED-EDITS
- include/game/GameGlobals.h: cls_804AA8C8::fn_8030A308(s32) -> u8,
  cls_804AA8C8::fn_80309A9C(u32, s32) -> u32; extern "C" fn_804310CC(void*),
  fn_80431A9C(void*, s32, u8, void*) (tu_8042E964).
- include/game/GameState.h: GameState_804B0FE0 0x23D u8 field_0x23d (split
  from field_0x23a[]; ctor copies it, fn_803F8050 polls slot 2 + it);
  cls_804AF86C::fn_80402940 returns u8 (li 1 / li 0 in its body).
- include/game/cls_804B16A8.h (new): vtable 0x804B16A8, ctor fn_804372C4
  (tu_80434370), fn_80436668 / fn_804366B8 / fn_8043670C(u8).
- include/game/tu_803F0A54.h: full class layout (used by tu_803A85E4 too).
- symbols.txt: placeholder renames (members of cls_804B0F10 / cls_804B16A8 /
  cls_804AA8C8, __vt__12cls_804B0F10, __sinit, __dt__), .data PMF-table
  split (@685-@723 initializers), .bss split.
