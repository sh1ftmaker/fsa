# game/act_rock

Baseline (progress.py): total: 5901 / 11557 functions, 1181784 / 4442032 bytes of code (26.60 %)
After: total: 5906 / 11557 functions, 1182900 / 4442032 bytes of code (26.63 %)

Matched: all 7 functions (dtor, vfunc_0x4C, vfunc_0x24, draw, update, init, __sinit).
Resistant: none in code. .sdata2 reports 93.33 % although the object's bytes
equal the target's (0x20, trailing pad float 0): dtk's lbl_80540F30 is 8 bytes
(12.0f + pad) and ours is a 4-byte literal. Not fixable from source.

SHARED-EDITS
- include/game/Player.h: fn_8023E7B0(s32 player) -> TVec3f* (extern "C", owner tu_8022AD64;
  same shape as fn_8023E724, falls back to &lbl_804CF460; act_rock update passes field_0xc8).
- config/G4SE01/gen_game.tsv: removed act_rock rows (dtor, sinit).
- include/game/act/act_rock.h (generator-owned): field_0x230/0x234 now s32.
