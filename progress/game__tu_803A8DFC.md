# game/tu_803A8DFC.cpp (ResultScreen_804AF9A0), Opus 5.5, 2026-11-06

Baseline (progress.py): 9057 / 11557. After: 9062 / 11557 (+5, unit 1 -> 6 of 47); no drops by address.

Identified: the results screen (result_{4,3,2}P.blo, result_score_*, result_back, result_title,
result_*.btk, kk_1st..4th rank textures). cls_804AF9A0 -> ResultScreen_804AF9A0 (names.tsv, med).
Class moved to include/game/tu_803A8DFC.h (layout from the ctor/dtor, size 0x4E0).

100 %: __dt__, fn_803B2FB0, fn_803AF604, fn_803A8DFC, cls_803B7B90 ctor (fn_803B7B90).
Structurally done (reloc-name only: .sdata2 / jump-table names): fn_803A97A0, fn_803B31AC,
fn_803AD220, fn_803ADC0C, fn_803AE308, fn_803AEC78, fn_803B0158, fn_803B3358, fn_803B35F8.
Structurally close: ctor 99.00 (lhz vs lha of J2DTextBox::mStringLength, 4 sites),
fn_803A9830 98.53 (field_0x1b8 loop unrolled in two halves in the original).

Data: the four u64 tag tables are in source (41 literals, under the tripwire). Not written:
the .data int tables lbl_804AF888 / 8CC / 910 / 948 (62 s32) and lbl_804AF980 (char*[8]),
the 6 floats after 'Ndatar24' in lbl_80476310 (object boundary unknown), the function-local
tables 0x80476368-0x804763D0 and sdata2 lbl_80543378-90 (locals of unwritten functions).
Adding the .data tables would push the file past check_byte_arrays' 512-byte per-file gain.

SHARED-EDITS: include/game/tu_80379F84.h (ResultScreen declaration moved out; cls_804AE9A0
gains /*0x18*/ JKRArchive* field_0x18 (ctor's setPriority archive) and /*0x1D*/ u8 field_0x1d
(set by fn_803A9830)); gen_game.tsv row tu_803A8DFC removed (file hand-written);
symbols.txt placeholder renames; names.tsv row; categories.tsv row.

Next: fn_803B6768 (reset, 0x59C), fn_803A8F5C (draw, 0x844), the 2/3/4P result updates
fn_803A9AB8 / fn_803AA1F8 / fn_803AA8F8; then the .data tables (array line needed).

## Round 2 (Opus 5.5, 2026-11-06)

Baseline (rebased on matching caea6e25): 9105 / 11557, structural 9679. After: 9105 / 11557
(+0 honest), structural 9681 (+2 done); no drops by address against fsa-decomp's report.

Written (structure first, at their descending-order places):
- fn_803A8F5C (draw: score digits with minus sign, player sprites, fade) 99.66 regalloc-only.
- fn_803B6768 (reset: layout from the player count, per-player state, music) 99.89 reloc-name.
- fn_803A9AB8 / fn_803AA1F8 (score-bar pages, modes 4 / 3) 98.71 / 98.67: registers, plus one
  `addi 0x30` per clamp of lbl_8050EC80.getCurrentFile().field_0x10 (the save-data row shape,
  docs/CRACK_PASS.md "Save-data layout"; not applied here).
- fn_803AA8F8 (mode 2: the 34-state machine) 98.43 "different" (state 21's `li; mr` zero copy
  is an opcode difference): reviewer moved it under #ifdef NONMATCHING. Its alpha loops multiply by a run-time zero float (a literal 0.0f
  folds away; written as a local `rate`). The fn_803AF604 auto_inline region was removed with it (no
  built caller calls it out of line).
- New fields: J2DTextBox* digit arrays 0xA8-0x138, panes 0x158[4] / 0x168, s32 arrays
  0x320-0x3E0, floats 0x41C-0x430, 0x434-0x448, s16 0x458-0x466, u8 0x4C8, 0x4C9[13],
  0x4D6[3], 0x4D9[2], 0x4DB-0x4DD. 25 placeholder functions renamed to ResultScreen members.

Pool: our .sdata2 now equals the original from "%1d" (0x805433C0) to 12.0f, except the
unsigned-conversion double 0x4330000000000000 (lbl_805433B8): the original has it before
"%1d" (after the P1..P4.bti strings of lbl_804AF980), ours at the end. Still missing in
front: the {150, 0} locals of fn_803AFCF0 / fn_803AF1B0 / fn_803AE810 and the P*.bti
strings, so every function stays reloc-name until those are written.

Next:
- fn_803AE598 / fn_803AEF08 / fn_803AFA78 (and fn_803AC154) inline one bool helper (step
  field_0x4c4 through lbl_804AF8CC, set frames from lbl_804AF888, rank effects 0xCB / 0xCC,
  sound 0x2000E): write it once as a plain member with the inlined body, then the four
  callers (states 17 / 14 / 10 / 30, next state 18 / 15 / 15). That writes lbl_804AF888[17]
  / lbl_804AF8CC[17] (s32, .data, original sizes).
- fn_803ADE9C (lbl_804AF910 / 948), fn_803ABCEC (lbl_804AF980, char*[8]), fn_803AB494.
- Then the locals of fn_803B236C / fn_803AFCF0 / fn_803AF1B0 / fn_803AE810 to finish the pool.

SHARED-EDITS: include/game/tu_803B8C74.h (SaveFile_803B950C 0x10: u8[4] -> s32 field_0x10);
config/G4SE01/symbols.txt (25 placeholder member renames, unit-local).

## Round 3 (Opus 5.5, 2026-11-06)

Baseline (matching bd727044): 9112 / 11557, structural 9684. After: 9113 / 11557 (+1 honest,
fn_803ADE9C), structural 9690 (+6); unit 6 -> 7 of 47; no drops by address against
fsa-decomp's report_honest.json.

Written (structure first, descending order):
- animateWinners() (dead-stripped BOOL helper: steps field_0x4c4 through lbl_804AF8CC, frames
  from lbl_804AF888, rank effects 0xCB / 0xCC and sound 0x2000E at the end). Declared `inline`:
  as a plain member it exceeds auto-inlining and all four callers call it out of line (13-20 %).
  Callers fn_803AFA78 / fn_803AEF08 / fn_803AE598 / fn_803AC154: reloc-name only (99.87-99.94).
- fn_803ADE9C (bonus count, lbl_804AF910 / 948) 100 %.
- fn_803ABCEC (title -> winner pane slide, win counting) 99.96 reloc-name.
- fn_803AB494 (final ranking, rank textures / corner colours, confetti, stage flag) 99.52:
  registers and stack slots only + .sdata2 names. The stage flag's `x < 8 ? x : 0` on locals
  compiles to the original's subfc/subfe/and mask (MWCC if-converts a ternary with a 0 arm when
  the operand is a register, as tu_802D39B8's `layer < 8 ? 0 : layer - 7`).
- fn_803AFCF0 / fn_803AF1B0 / fn_803AE810 (bonus pages: points by place from function-local
  static tables {300,150,50,0} / {300,150,0} / {150,0}) 99.93 reloc-name (local table names).
- Data as source: lbl_804AF888[17], lbl_804AF8CC[17], lbl_804AF910[14], lbl_804AF948[14] (s32),
  lbl_804AF980[4] / lbl_804AF990[4] (char*). gate's tripwire did not trip (no array line).

Evidence-driven layout:
- lbl_804AF980 (0x20) is two objects: fn_803AB494 addresses the rank textures off a base at
  0x804AF990 (section base + 0x108, folded); a [2][4] or [8] table keeps a separate +0x10.
  symbols.txt: lbl_804AF980 size 0x10 + new lbl_804AF990 size 0x10.
- The two string tables are defined below fn_803AA8F8 (extern declarations on top): the
  original's .sdata2 / .rodata have their strings after the function-local tables of fn_803AFCF0,
  fn_803AF1B0, fn_803AE810 and fn_803AA8F8. With that, .sdata2 0x0-0x18 now equals the original.
- field_0x46c is J2DPicture::TCornerColor[4] (copied into field_0x40[i]'s mCornerColors by
  setCornerColor; TColor's default constructor is white, hence the out-of-line ctor storing -1 x4).
  cls_803B7B90 removed; symbols.txt __ct__12cls_803B7B90Fv -> __ct__Q210J2DPicture12TCornerColorFv.
- GameState field_0x1fc is u8[8][3] (stage x level record): fn_803ABCEC indexes it as
  base + stage*3 then + level; tu_804023D0's two reset loops became nested 8 x 3 loops (both
  functions unchanged at 100 %).

Pool: still missing in front: fn_803AA8F8's s16 ids (lbl_80543390, NONMATCHING) and the
unsigned-conversion double lbl_805433B8 (its first user is not known: it precedes every
codegen literal, so it is not from fn_803AA8F8's codegen order alone).
fn_803AA8F8's li/mr zero-copy row: not retried (no natural form found in round 2); stays under
NONMATCHING.

SHARED-EDITS: include/game/GameState.h (field_0x1fc u8[0x18] -> u8[8][3]); src/game/tu_804023D0.cpp
(the two field_0x1fc reset loops); include/game/StageFlag.h (new inline setStageFlag, fn_8019CDE4's
body); include/game/tu_803A8DFC.h (field_0x30/40/50/60 pane arrays, TCornerColor field_0x46c,
J2DPicture include); config/G4SE01/symbols.txt (TCornerColor ctor name, lbl_804AF990 split).

Next: fn_803B236C (its .rodata locals lbl_80476368 / 78 lead the pool), fn_803B03E8, fn_803B0978,
fn_803B0FD4, fn_803B14F8, fn_803B1B60, fn_803AF79C, fn_803AD4B0 / AD894, fn_803AC6FC / AC998 / ACDA4;
fn_803B38E8 / B42C0 / B5378 (layout setup: should set field_0x30..0x60).
