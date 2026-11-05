# game/tu_803F0A54.cpp

Baseline (progress.py before): `total: 7123 / 11557 functions, 1392832 / 4442032 bytes of code (31.36 %)`

What it is: ConnectScreen_804B0F10 (vtable 0x804B0F10, ctor 0x803FCBC4, 0x354 bytes), the
multiplayer GBA-connect screen that cls_804AF878 (tu_803A85E4) builds as its
field_0x8 when gGameRoot_8053AAF8->field_0x7f is clear. Seven J2DScreens from the
fn_804031C4 archive, a 13-state machine through three static PMF tables
(enter lbl_804B0BDC, update lbl_804B0D14, draw lbl_804B0E4C; state 13 runs
field_0x1c8's entries), a full-screen fade J2DPicture (field_0x5c), four
cls_804B20F8 sound objects. The tables are copied at boot by __sinit from
their @NNN initializers (MWCC does this for PMF tables of a class with
virtuals) — that copy is the boot-path code.

## Matched (100 %)
__sinit_tu_803F0A54_cpp (boot path), fn_803F1C18, fn_803F1C5C, fn_803F2660,
fn_803F2AEC, fn_803F2B48, fn_803F2B5C, fn_803F32E8, fn_803F3310,
fn_803F358C, fn_803F59A0, fn_803F8050, fn_803FC304, __dt__22ConnectScreen_804B0F10Fv.
.bss 100 % (9 @NNN dtor chains + 9 weak memPool copies split per COORDINATOR).

## Resistant
- fn_803F0A54 98.08 (`different`): written whole; its literals now sit at the
  original .sdata2 addresses (with the file-scope s16 lbl_80543710[4] that
  fn_803F1C70 / fn_803F5A0C index, emitted first). Left: rodata base offsets
  (.rodata unsplit) and the i / 2 + (i % 2) * 2 scheduling in two phases.
- fn_803F3324 99.77, fn_803F715C 99.78, fn_803FC46C 99.89, fn_803FC5E4 99.83:
  reloc-name only (.sdata2 literals of the still unwritten fn_803F1C70 and
  later functions come first). They match once every earlier function exists.
- fn_803F2580 98.04, fn_803FB6F4 99.41 (`different`, regalloc): setEntries'
  loop gets i / 2*i in swapped registers (orig: zero already live -> 2*i,
  fresh li -> i). fn_803F8050 inlines fn_803F2580 and matches.

## Not written (17)
fn_803F1C70,
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
  cls_804AA8C8::fn_80309A9C(u32, s32) -> u32; review fix: lbl_8053AEC0 is a
  cls_804B1634* (vtable 0x804B1634, ctor fn_80431D84, tu_8042E964) with
  members fn_804310CC(), fn_804316EC(), fn_80431A9C(s32, u8, void*) (were
  extern "C" void* functions); tu_8044C7F0's caller updated.
- include/game/GameState.h: GameState_804B0FE0 0x23D u8 field_0x23d (split
  from field_0x23a[]; ctor copies it, fn_803F8050 polls slot 2 + it);
  PaneCtrl_804AF86C::fn_80402940 returns u8 (li 1 / li 0 in its body).
- include/game/cls_804B16A8.h (new): vtable 0x804B16A8, ctor fn_804372C4
  (tu_80434370), fn_80436668 / fn_804366B8 / fn_8043670C(u8).
- include/game/tu_803F0A54.h: full class layout (+ field_0x335) (used by tu_803A85E4 too).
- symbols.txt: placeholder renames (members of ConnectScreen_804B0F10 / cls_804B16A8 /
  cls_804AA8C8, __vt__22ConnectScreen_804B0F10, __sinit, __dt__), .data PMF-table
  split (@685-@723 initializers), .bss split.

## Checkpoint (Opus 5.5 pass 2, twin units, 2026-11-05)
Baseline total 8416. Twin of game/tu_803E1668.cpp (cls_804B0B00, the 1-4 player
version; see its checkpoint for the screen map). Ported both ways:
fn_803F38A4 99.6 (stack slots), fn_803F35B0 99.2, fn_803F5410 98.7 (operands: the
original shares one int->float slot), fn_803F8AE4 / fn_803F8FBC 99.9 (reloc-name
only); fn_803F0A54 now uses `size / 2` (fmadds operand order, proven at 98.3 in the
twin's fn_803E1668; try.py 98.08 -> 98.24).
Header: field_0xf0 ('Pscuro02' y), field_0xf8 / field_0xfc[3] (game-select panes),
field_0x118 / 0x124 / 0x130 as [3] arrays (Pnopb08-16 x / y / offset; fn_803F3324
updated), field_0x1a0 ('Paccur02' y).
Partial .rodata tables (25 tags, under the tripwire): lbl_80479238[4] 'Pplli',
lbl_804793D8 / F8 / 80479418 [4] cursor panes, lbl_804798C0 / D8 / F0 [3]; the
.rodata objects lbl_80478A60 / lbl_80479238 / lbl_804795AC are still unsplit blobs.
BLOCKER for the rest (fn_803F1C70, fn_803F9EA8, fn_803F943C, fn_803FAA88, fn_803FB9E4,
fn_803F726C, fn_803F8354, the constructor): they index the big tag tables (4x6, 4x9,
34-entry ...), i.e. this unit's whole .rodata as source data, which needs an `array`
line in blobs.txt like tu_803E1668's (same table set, same layout). The twin's
tables are the template.
SHARED-EDITS: none (own header only).
