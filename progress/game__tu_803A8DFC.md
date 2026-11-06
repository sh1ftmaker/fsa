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
