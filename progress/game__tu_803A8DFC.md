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
- fn_803AA8F8 (mode 2: the 34-state machine) 98.43 "different" = registers + state 21's
  `li; mr` zero copy only. Its alpha loops multiply by a run-time zero float (a literal 0.0f
  folds away; written as a local `rate`). fn_803AF604 got an auto_inline off/reset region
  (fn_803AA8F8 calls it out of line).
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
