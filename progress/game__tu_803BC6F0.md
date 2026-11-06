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
  PaneCtrl_804AF86C vtable, target has lbl_804AFF50, vtable, jump tables.
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
- GameState.h PaneCtrl_804AF86C::fn_80402628(s16, u8, u8) -> bool.
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
(`field_0x58->moveFromCentre_80402E08(0.0f, 200.0f * t + -200.0f)`) used from fn_803CE9EC
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

### fn_803BC6F0 shape (read, not written)
fn_803BC6F0(s32 file, f32 alpha): clamp alpha (`> 1.0f` first, then
`< 0.0f`); two TColors on the stack (black at sp 0xF4, white at 0xF0,
default-constructed to -1); per channel `c + (target - c) * alpha` (target - c
converted signed, c unsigned, fmadds, fctiwz). Five groups, each lerps
field_0xfc[k] (black, alpha byte 0) and field_0xe8[k] (white, alpha 0xFF)
and calls J2DPicture::setBlackWhite (vslot 0x130, by value) on
mScreenHuuin->search(table[file]):
  k=0 black->0x00 white->0x80: lbl_80476BA8 (0x5D8, Pf?t01)
  k=1 0x82 / 0xB4: 0x5C0, 0x5A8, 0x590, 0x578, 0x560 (Pf?tb01..05; the 05
      call gets black with alpha 0xFF, rebuilt bytewise from a copy at sp 0xE4)
  k=2 0x50 / 0x80: 0x548..0x4E8 (Pf?tbb01..05)
  k=3 0x82 / 0xB4: 0x6C8..0x668 (Pf?b01..05, same alpha-0xFF tail)
  k=4 0xB4 / 0xB4: 0x650..0x608 (Pf?bb01..04)
The group colours are copied to sp 0xE4 / 0xE0 and to each call's by-value
temporaries; that copy pattern is what needs working out.

## Session 2026-11-05 (Opus, boot-path functions)

Baseline (progress.py): `total: 8297 / 11557 functions, 1530728 / 4442032 bytes of code (34.46 %)`
After: same honest total (no new 100 %); structural `8630 / 11557 (+79 close)`.

Written (all try.py-checked):
- fn_803C24BC 99.93 reloc-name (instructions identical): shows the rows, title msg, row slide-in start, state 4.
- fn_803CF504 99.93 reloc-name (instructions identical): resets the screen (ctor fields again, Tf?lv/fir/date hidden, rows' start).
- fn_803CB3DC 98.40 missing/extra (1 instr): state 18, the other two rows slide out, board / buttons in (timings lbl_8050DB0C field_0x4c[0x98..0x9F]).
  FIXME: row-loop start test folded (rowDelay + delay), fsubs operand order.
- fn_803BC6F0 98.65 missing/extra (8 instrs): colour lerp of the row pictures. Group copies (sp 0xE4/0xE0 ..) are named
  locals; the original copies them through one more temporary (sp 0x20 ..) per group. Tried: explicit TColor(...) temp,
  const copy, default+assign (worse), (GXColor) cast (worse, bytewise). Group 2's 5th call is a plain one (only 1 and 3
  rebuild black with alpha 0xFF).
- fn_803CD2DC 99.83 reloc-name (instructions identical): state 5, picking a file.
Proved: file-static inlines setRowAlphas / setTabAlphas (13 setAlpha_804023D0 calls per row half; the target caches `file` in a
register across them in fn_803CB3DC, so they are a helper with a `file` parameter; used by fn_803C24BC, fn_803CF504,
fn_803CB3DC with identical code); hasSaveData(file) (`u8 bit = 1 << file`, the clrlwi before the and., 7 call sites);
fn_803BF9C4 / fn_803C1734 are FileSelect members (this in r3).
A shared 40-call inline for the rows' slide-in start is NOT inlined (MWCC refuses; ours called it), so that block is
written in both fn_803C24BC and fn_803CF504.

Near-misses: none flip. The pool still needs 0, -200, 200, dbl, dbl before 1.0, and fn_803BC6F0 (first in .text) uses
1.0 first, so the leading literals come from something compiled before it that the linker dropped (fn_803CE9EC's
kk-fade code shape: 0.0f, -200, 200, s32 and u8 conversions). Written functions' literals now pool in a different
order, so fn_803C6690 99.85 -> 99.70, fn_803CE9EC 99.86 -> 99.79, fn_803CE11C 99.50 -> 99.46, ctor 98.829 -> 98.827
(reloc-name only; instructions unchanged).

SHARED-EDITS
- symbols.txt: fn_803BF9C4 -> fn_803BF9C4__19FileSelect_804AFF60Fv, fn_803C1734 -> fn_803C1734__19FileSelect_804AFF60Fv.
- tu_803BC6F0.h: members fn_803BF9C4, fn_803C1734; inline hasSaveData.
- fn_803CD75C 95.09 missing/extra (1 instr): state 4, cursor / rows / board slide in, 'kk_' fade start (lbl_8050DB0C
  0x84..0x8A). Uses setRowAlphas(this, 2 - i) and setTabAlphas(this, i). `rowDelay + i * rowStep` keeps the add as the
  original does (a running `delay += rowStep` folds; the same holds for fn_803CB3DC's button loop).
  FIXME: end computation order (frames + rowStep * 2 first) and r4 reuse for the cursor call.

### Next
- The literal pool: the four reloc-name near-misses (and the five new reloc-name functions) need the leading
  0, -200, 200, dbl, dbl, which nothing written produces; see above.
- fn_803BDD44, fn_803CEED4, fn_803CE4CC, fn_803CABAC, fn_803C1734 (boot path, unwritten); fn_803BF9C4 next to them.

## Session 2026-11-05 (Opus, third pass: boot path continued)

Baseline (progress.py): `total: 8338 / 11557 functions, 1561464 / 4442032 bytes of code (35.15 %)`, structural 8677 (1683496 B).
After: honest unchanged (no new 100 %, no drops by address); structural 8682 (1690868 B).

Written (try.py class):
- fn_803BDD44 99.58 operands (stack slots only): resets row `file`'s pictures to the ctor colours (fn_803BC6F0 at alpha 0).
  Original numbers the by-value TColor copies upwards within a group and puts group 0's copies above the group locals.
- fn_803CEED4 99.89 regalloc-only: the draw. Seven separate fn_803B8CAC tests (a loop with break isn't unrolled; the
  field_0xb8 search loop is). `lbl_8050EC80[i]` adds base + offset in ours, offset + base in the original (also in
  fn_803CABAC, fn_803C1734; fn_803B8CAC's own row lookups look the same: probably an inlined per-file accessor).
- fn_803CABAC 98.14 different (cmpwi vs cmplwi on `gGameRoot_8053AAF8->field_0xff`; u8 / bool / s8 all fail, an int accessor?),
  plus bob scheduling, the add order above, setBlackWhite copy slots. State 19, the file menu.
- fn_803C1734 99.01 regalloc-only: opens the file menu (state 18).
- fn_803BF9C4 98.49 scheduling+regalloc: opens the copy board (state 32). Function-scope `int i; int j;` stops each
  switch case re-loading j = 0 (per-loop counters 96.49); the same change made fn_803C1734 / fn_803C2E24 worse.
- fn_803C2E24 98.27 different (same length; one instruction: original keeps &others in r29 from the fill loop). State 39.
  `i * step` delays keep the original's add (a running delay folds it, 98.16 missing/extra 3).
- fn_803C4054 96.12 scheduling+regalloc (bob only, as fn_803CABAC): delete prompt, state 37.
- fn_803C4330 99.95 reloc-name (instructions identical): back from the delete prompt, state 36.

Not written:
- fn_803CE4CC (boot path): needs `fn_803BADAC(row)` with the real row pointer (lbl_8050EC80 + 0x20 + i * 0xC8; the
  checksum over 0xC0 bytes compared with the u64 at row + 0xC0). Blocked on the lbl_8050EC80 layout decision (see the
  proposal above); the SaveRow view can't pass a row start without a cast.

SHARED-EDITS
- tu_80379F84.h SaveRow_8050EC80: field_0x14[0x14] split into u8 field_0x14[0xC] + `s64 field_0x20` (OSGetTime() of
  the last start, 0 for an empty file; fn_803CABAC stores it, fn_803C1734 / fn_803C2E24 / fn_803CABAC test it; real-row
  offset 0). No other unit uses field_0x14.
- GameGlobals.h: `bool fn_803B90E4(s32 file, s32 index)` (extern "C", owner tu_803B8C74; li 1 / li 0; index < 3 true,
  else the row's 0x28 top bit).
- symbols.txt: fn_803BDD44 -> fn_803BDD44__19FileSelect_804AFF60Fl, fn_803BE3D8, fn_803C1340, fn_803C21A8,
  fn_803BF3BC, fn_803BF698 -> ...__19FileSelect_804AFF60Fv (all take this in r3: lha 0xdc(r3), lwz 0x50(r3), mr r31,r3
  then member loads).
- tu_803BC6F0.cpp includes JSystem/J2D/J2DGrafContext.h (J2DScreen::draw's grafcontext).

### Next
- fn_803CE4CC after the lbl_8050EC80 decision; fn_803BE3D8 (0xC2C, called by the draw), fn_803C3904 (state 38),
  fn_803C4628 (35), fn_803C5190 (34), fn_803C5808 (32) ... in address order.
- The stack-slot order of by-value TColor copies (fn_803BDD44 / fn_803BC6F0 / fn_803CABAC ascend, fn_803C1734 /
  fn_803BF9C4 descend like ours) is the common open question.

## Session 2026-11-05 (Opus, leverage wave 14: save-data switch + file select)

Baseline (progress.py): `total: 8800 / 11557 functions, 1756492 / 4442032 bytes of code (39.54 %)`
After: `total: 8802 / 11557 functions, 1757340 / 4442032 bytes of code (39.56 %)`, structural 9256.

### Task 1: SaveRow_8050EC80[] retired
- Every user now reads lbl_8050EC80 as cls_804AFB30 (game/tu_803B8C74.h) through two inline
  accessors, `getFile(i)` and `getCurrentFile()` (= mFiles[mCurrentFile]). Evidence: with them
  fn_803B8CAC, fn_803B8EBC, fn_803B90E4 reach 100 % and fn_8019CE40 stays 100 %; indexing mFiles
  directly swaps the add's operands (97 %).
- SaveFile_803B950C carved from the old row fields (row offset - 0x20): 0x0C s32, 0x14/0x16 u16,
  0x58/0x59, 0x5A[2], 0x5C..0x61, 0x65, 0x67..0x69, and the checksum words 0xC0 / 0xC4
  (fn_803BADAC). mFlags is s32 (the old view's type).
- isStageFlagSet (StageFlag.h) now reads getCurrentFile().mFlags; tu_803B8C74's duplicate
  isCurrentFileFlagSet is gone (fn_803B8EBC / fn_803B9120 use isStageFlagSet). isFileFlagSet (any
  file) stays local to tu_803B8C74.
- Per-address compare against the start: gains fn_803B8CAC / fn_803B8EBC / fn_803B90E4 -> 100,
  GameView vfunc_0xC 97.5 -> 99.1, TreasureChest vfunc_0x54 94.7 -> 94.9. Drops (still open):
  tu_801DE714 __dt__12cls_804A51E4Fv 100 -> 98.60 and act_fairy init 97.86 -> 97.39 (the same two
  the earlier trial found). Both bind a reference to the current file across two blocks; the original
  keeps the pointer at lbl_8050EC80 + file * 0xC8 and folds the 0x20 into its field offsets, ours
  materialises `addi 0x20`. Tried: direct accessor expressions (lwzu, 98.36), a pointer accessor,
  a direct mFiles index for the clamp (lwzx, 91.8), a value local. Not found.

### Task 2: file select
- Constructor: the save-data loop (`fn_803B950C(&mFiles[i], &mBodies[i])`) and field_0x5c /
  field_0x60 (now SaveFile_803B950C* / SaveFileBody_803B950C*) written: 98.83 -> 99.92
  (regalloc of the font pointer, string offsets +0x58, @NNN).
- fn_803CE4CC written: 99.02 missing/extra (1 li). Inlined helper getProgressMsg (three inlined
  copies; two counters in the original's loop: cleared count and stage; out-of-line copy dropped
  by the linker, as tu_8016A2B4's getMapTexView). fn_803BADAC (tu_803BACF8) declared in
  tu_803B8C74.h. field_0x115's bit is set exactly when fn_803BADAC fails, so hasSaveData was
  renamed hasBadChecksum, with setBadChecksum / clearBadChecksum (the original clrlwi's `1 << i`
  at each use).
- fn_803CABAC: OSGetTime() before the lookup -> 98.37 (cmpwi on field_0xff, copy slots remain).
- fn_803CEED4 99.94 reloc-name only; fn_803C1734 99.33 regalloc only.

SHARED-EDITS
- tu_80379F84.h: SaveRow_8050EC80 and `extern lbl_8050EC80[]` deleted.
- tu_803B8C74.h: SaveFile fields above, getFile / getCurrentFile, fn_803BADAC decl.
- StageFlag.h: includes tu_803B8C74.h, reads getCurrentFile().mFlags.
- Mechanical switch in act_fairy, act_rank_display, act_treasure_chest, tu_80197070, tu_801DE714,
  tu_80215A94, tu_8026FC20, tu_802D39B8, tu_80379F84, tu_803944C8, tu_803E1668, tu_80403D84,
  tu_80414F6C, tu_8043A0E8 (includes + field names only).
- Comments only: GameGlobals.h, GameState.h, GameView.h.
- symbols.txt: fn_803BADAC -> fn_803BADAC__FP17SaveFile_803B950C, fn_803BD5E0 ->
  fn_803BD5E0__19FileSelect_804AFF60Fl.

### Next
- The two drops above (reference shape); fn_803BE3D8 (strings first -> string offsets);
  fn_803BD5E0; the cmpwi on GameRoot field_0xff (an int accessor?).
