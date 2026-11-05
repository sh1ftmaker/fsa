# game/act_wait_switch

Baseline: total 5780 / 11557 functions, 1159560 bytes (26.10 %). After: 5783, 1160288.

Matched: all 5 functions (dtor, draw, update, init, __sinit), .text/.data/.rodata/.bss/.ctors 100 %.

Remaining: .sdata2 85.71 %. The original has a fourth float (0.0 at 0x80540C0C, second half of the
8-byte lbl_80540C08) that no function here references; we emit 0.0, 1.0, 5.0 (12 bytes), the original 16.
Not forced (would need a fake use). extab/extabindex 40 % as in other units of this wave.

SHARED-EDITS:
- include/game/GameGlobals.h: GameRoot_8049EF20::fn_80134DE4(s32, s32) member (called on gGameRoot_8053AAF8, (10, count)).
- config/G4SE01/symbols.txt: fn_80134DE4 -> fn_80134DE4__17GameRoot_8049EF20Fll.
- config/G4SE01/gen_game.tsv: removed both act_wait_switch rows.
