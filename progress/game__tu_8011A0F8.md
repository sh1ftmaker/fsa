# game/tu_8011A0F8
Baseline: total 5708 / 11557 functions, 1150032 bytes (25.89 %). After: 5710, 1150168.
Matched: init, dtor, ctor (fn_8011A248 renamed __ct__12cls_8049DAD8Fv), sinit.
Resistant: update (97.6 %, missing/extra): original ends the vfunc_0x34 test with `beq end`; ours `bne body; b end` (op_mismatch at 0x68 plus an inserted `b`). Tried && vs nested, early-return vs else-return.
SHARED-EDITS: GameGlobals.h: cls_801192AC, fn_8011921C, fn_801186B0 (owner tu_80118384). gen_game.tsv: rows of this unit deleted.
