# game/act_thwomp_switch
Baseline: total: 5667 / 11557 functions, 1147428 / 4442032 bytes (25.83 %). After: 5669 / 11557, 1147436 bytes.
Matched: dtor, init, draw, __sinit (4/5 in try.py).
Resistant: update (90.25%, missing/extra): fmadds fused where the original has fmuls/fmuls/fadds; pair loads scheduled differently (target loads player x first, then mPos.x, then y pair; ours y first). A local `pos = &mPos` raised it to 93% but is a register-steering temp, dropped.
SHARED-EDITS: include/game/Player.h: added fn_8023A478 (TVec3f* fn_8023A478(s32), lfs 0/4 of result, fallback lbl_804CF460 like getPlayerPos_8023E724) and bool fn_8023A51C(s32) (clrlwi 24 + cmplwi 1 in callers, e.g. act_warp 0x80228098). Both defined in tu_8022AD64 asm, not yet written.
Removed act_thwomp_switch rows from gen_game.tsv.
extab/extabindex/.sdata2 below 100% only follow update (sdata2 literal pooled differently).
