# game/act_fairy_revival.cpp (RVFR, RevivalFairy_804AC250)

Baseline: 5980 / 11557 functions, 1201588 bytes (27.05 %). After: 5983 / 11557, 1202344 bytes (27.07 %).

Matched: draw, init, __dt__, __sinit (4/5; was 1/5).

Resistant: update (99.93 %, regalloc-only). The target multiplies the atan2 result (f1) by
10430.378f (f0) as `fmuls f0, f1, f0`; ours emits `fmuls f0, f0, f1`. Tried const*atan, a
local for the atan result, JMAAtan2Radian; same known quirk as tu_8021C2B4.

SHARED-EDITS: config/G4SE01/gen_game.tsv (row removed); symbols.txt: 4 placeholders renamed,
vtable 0x58 -> 0x54, lbl_80542198 resized 0x8 -> 0x4 (trailing 0.0f is padding).

Notes: .sdata2 trailing 4-byte alignment pad (ours 0x24, original 0x28); extab/extabindex follow update.
