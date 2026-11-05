# game/tu_803BC6F0.cpp

Baseline (progress.py before): `total: 7529 / 11557 functions, 1452944 / 4442032 bytes of code (32.71 %)`
After: `total: 7546 / 11557 functions, 1458164 / 4442032 bytes of code (32.83 %)`

What it is: FileSelect_804AFF60 (vtable 0x804AFF60, ctor fn_803D0264, dtor
fn_803CFFBC, 0x1AC bytes), the file select screen: loads "file_select.blo" /
"file_select_huuin.blo", stores itself in lbl_8053ADB0, runs a 40-entry
pointer-to-member state table lbl_804AFD70 (copied by __sinit from @663..@702)
from fn_803CE9EC each frame, and talks to the memory card manager
lbl_8051C1A0 (cls_803B9940) and the save rows lbl_8050EC80. Sister unit:
tu_803F0A54 (GBA connect screen), same shape.

## Matched (100 %)
__sinit_tu_803BC6F0_cpp (boot path, 1444 B), __dt__, fn_803C579C, fn_803C92B8,
fn_803C9F14, fn_803CBE38, fn_803CBEE0, fn_803CBF0C, fn_803CC048, fn_803CC0D8,
fn_803CC17C, fn_803CC2BC, fn_803CC3DC, fn_803CC490, fn_803CC62C, fn_803CE05C,
fn_803CE4B0 (boot path). .bss 100 % (@814..@822 + 9 weak memPool copies).

## Resistant
- fn_803CE9EC 99.86 (boot path), fn_803C6690 99.85: reloc-name only (.sdata2
  literal order; fn_803BC6F0, first in the file, uses 0, -200, 200, the int
  conversion doubles first). Will match once the earlier functions exist.
- fn_803CE370 99.69, fn_803CE11C 99.50 (boot path): `getBounds()->i.x -
  field_0x20->field_0x8 < 600.0f` loads into f1/f2 swapped (orig fsubs f0,f0,f1
  / f1,f1,f2). A local and a file-local inline (worse, 97.6) didn't fix it.
  fn_803CE11C's loop also swaps r29/r30.

## Data
- .data: @663..@702 split; lbl_804AFD70 is 0x1E0 (scope local), lbl_804AFF50
  (0x10: 00010003 00010003 00010000 00020000) unreferenced, owner unknown.
  Our vtable and jump tables come out after the jump tables / weak
  cls_804AF86C vtable, target has lbl_804AFF50, vtable, jump tables.
- Jump tables renamed @979 (fn_803CE11C) / @1109 (fn_803CE9EC): the numbers
  move whenever a function is added before them; re-check with nm.
- .rodata 0x804765D0-0x80476F20 not split yet except lbl_80476938 (u64[9]
  'Pdb' tags). It is u64 pane tag tables (offsets 0x0, 0x18, 0x30, 0x48,
  0x2D0, 0x4B8.., 0x5C0, 0x5D8, 0x5F0.., 0x6E0..), an s32[8] msg table at
  0x730, more tags 0x750-0x810, s32 pairs at 0x818, then strings
  (enemy_both2_sob.bin, player_sob.bin, enemy_gc_sob.bin, the date format
  "%02d.%02d.%04d %02d:%02d:%02d", gb_ch_obj_base_sch.szs, ..., file_select.blo,
  file_select_huuin.blo, kk_black.bti, kk_kettei_01.bti, kk_modoru_01.bti).
  Most functions address it off one base (lbl_804765D0 + off).

## Not written (46)
Boot path first: ctor fn_803D0264 (8.6 KB), fn_803CF504, fn_803CB3DC,
fn_803C24BC, fn_803C1734, fn_803CD75C, fn_803BDD44, fn_803CEED4, fn_803CE4CC
(needs the 0x0/0x18/0x30 tag tables, the 0x730 table and the date string),
fn_803CD2DC, fn_803CABAC. fn_803CC53C and fn_803CDEE8 / the ctor need
lbl_8050EC80 as {0x20 header; rows[3] of 0xC8 at +0x20; blocks[3] of 0x9C4 at
+0x280}, which conflicts with tu_80379F84.h's SaveRow_8050EC80[] view;
fn_803B93E8 (tu_803B8C74) takes lbl_8050EC80 in r3 unused (a member of that
object's class?).

## SHARED-EDITS
- GameGlobals.h GameRoot field_0xff: u8 (was u8[9]); 0x100 padding.
- GameGlobals.h cls_803B9940: fn_803B9940(s32)->u32, fn_803B9B8C(), fn_803B9A94(void*,u32,void*,u32,s32)->s32,
  fn_803B9BDC()->s32, fn_803B9C5C()->s32, fn_803B9DDC(); fields 0x1FF4 u8
  (card channel), 0x1FF5 u8, 0x1FF8 u64 (CARDGetSerialNo serial).
- GameGlobals.h cls_804B1634: fn_80431174(u8), fn_804317AC()/fn_80431740() -> BOOL.
- GameState.h cls_804AF86C::fn_80402628(s16, u8, u8) -> bool.
- tu_80379F84.h SaveRow_8050EC80: u64 field_0x8 (row 0: card serial).
- symbols.txt: renamed the placeholders above to their mangled names.
