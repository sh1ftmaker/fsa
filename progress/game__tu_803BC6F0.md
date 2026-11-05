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

## Session 2026-11-05 (structure-first trial: constructor)

Baseline: `total: 8234 / 11557 functions, 1516064 / 4442032 bytes of code (34.13 %)`
After: same (no new 100 % function; the constructor is structurally close).

- `__ct__19FileSelect_804AFF60Fv` written whole: 98.78 %, try.py
  `missing/extra + reloc-name`. Missing only the save-data setup (16
  instructions: `for i < 3: fn_803B950C(lbl_8050EC80 + 0x20 + i*0xC8,
  lbl_8050EC80 + 0x280 + i*0x9C4)` and field_0x5c / field_0x60), which the
  SaveRow_8050EC80[] view can't express; registers around it shift (r7/r5,
  r28/r29). String offsets are 0x58 short until fn_803BE3D8 / fn_803CE4CC
  (whose strings come first) are written.
- Proved: field_0x88[3] / field_0x94[3] (Padhu bounds x / y), 0xE0 / 0xE4
  TColor (white / black of 'Pcopb01'; default TColor ctor inlined, the
  arrays via __construct_array), field_0x118[26] / field_0x132[81] /
  field_0x183[19] / field_0x196[9] / field_0x19f[4] (pane alphas),
  field_0xb8[8] = fn_803B8CAC(0, i) (bool), field_0xa4 = GameState 0x23B
  (written as field_0x23a[1]: tu_804023D0 writes the pair as an array).
- .rodata 0x804765D0-0x80476F20 now fully split in symbols.txt and written as
  source tables (u64 tag tables, s32[8] msg ids at 0x730, s32[6] at 0x818;
  strings split as data:string). .rodata 98.12 %.
- gate --base matching: check_byte_arrays trips on the 81- and 19-entry tag
  tables (lbl_80476618, lbl_804768A0) and the file's total. They are pane tag
  tables the asm indexes (u64 'Pabb10'.. etc.); needs a blobs.txt `array`
  line from the coordinator (not added by the worker).

### lbl_8050EC80 (save data) proposal, NOT applied
Real layout (constructor, fn_803B950C, fn_803B9A94 sizes 0x258 / 0x1D4C):
{u8[8]; u64 serial @8; s32 current @0x10; pad to 0x20; SaveRow rows[3] (0xC8,
@0x20); pad; u8 blocks[3][0x9C4] @0x280}, size 0x21E0; old row fields shift
by -0x20 (0x28 -> 0x8 etc.); first method fn_803B93E8 (this = lbl_8050EC80).
Tried it with every user converted (tu_803944C8, tu_80379F84, tu_8043A0E8,
tu_801DE714, act_fairy, act_rank_display, tu_80215A94, act_treasure_chest):
all still match except tu_801DE714 __dt__12cls_804A51E4Fv (100 -> 98.6:
`SaveRow& row = lbl_8050EC80.mRows[cur]` adds `addi r3, r3, 0x20` where the
original folds 0x20 into the field offset; direct member access gives lwzx/
stwx, 89.7; a SaveData* local and a reference-returning inline didn't help)
and act_fairy init (97.86 -> 97.39). Reverted.

### Literal order (.sdata2)
Target pool: 0, -200, 200, (pad), dbl 0x4330/80000000, dbl 0x4330/0, 1, 360,
0.5, 270, 13, 10, 12, -300, 600, ... fn_803BC6F0 uses only 1.0 (first in its
code), 0.0 and the two doubles; -200 / 200 are used only by fn_803C24BC,
fn_803CD75C, fn_803CE9EC, fn_803CF504 (200 only by fn_803CE9EC). So writing
fn_803BC6F0 alone won't put -200 / 200 ahead of its doubles: something
compiled before it uses 0, -200, 200 in that order. An in-class inline
(`field_0x58->fn_80402E08(0.0f, 200.0f * t + -200.0f)`) used from fn_803CE9EC
did NOT move them (literals are pooled at use, not at parse). Open question.

### Next
- blobs.txt array line for the two tag tables (coordinator).
- fn_803BC6F0 (956 instr, TColor lerps of field_0xe8/0xfc by alpha), then
  fn_803BE3D8 / fn_803CE4CC for the string offsets.

### SHARED-EDITS
- GameGlobals.h: `bool fn_803B8CAC(s32 file, s32 index)` (extern "C", owner
  tu_803B8C74; returns li 1 / li 0, the caller clrlwi's).
- symbols.txt: lbl_804765D0 (0x368) and lbl_804769D0 (0x550) split into the
  tables / strings above (placeholders only).
