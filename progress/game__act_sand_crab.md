# game/act_sand_crab.cpp

Baseline: total: 6007 / 11557 functions, 1208940 / 4442032 bytes of code (27.22 %)
After: 6008 / 11557, 1209004 bytes.

Matched: ~dtor, draw, __sinit. Resistant:
- init 99.29% regalloc-only: original zero in r6, gGameRoot_8053AAF8 in r7 (swapped in ours), 13 insns.
- update 99.65% missing/extra: original decrements field_0x23c via addi r4,r31,8; stw 0x234(r4).
- .sdata2 95.65%: original section is 0x30, trailing 4 bytes (0.0 pad after 8.0) not emitted by us.

SHARED-EDITS: include/game/Player.h (fn_8022DCD8 declaration, bool(s32), clrlwi 24 result, used by update);
config/G4SE01/gen_game.tsv (removed both act_sand_crab rows). Header act_sand_crab.h: members 0x230-0x248.
Fields (no name evidence, kept as field_0xNNN). fabs via FABS() macro of libs/sdk/msl_include/math.h (fabs;frsp).
