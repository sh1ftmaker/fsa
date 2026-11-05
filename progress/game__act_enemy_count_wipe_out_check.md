# game/act_enemy_count_wipe_out_check
Baseline: total 5706 / 11557 functions, 1149900 bytes. After: 5708 / 11557, 1149964.
Matched: dtor, draw, init, sinit (4/5). Resistant: update (99.69 %, regalloc-only):
original keeps field_0x4 in r30 across getGameView_802D6A9C(), ours in r28 (see FIXME in the source).
SHARED-EDITS: include/game/GameGlobals.h: extern "C" void fn_80138B7C(GameRoot_8049EF20*) (r3 only; BgRewrite.h comment); struct Unk_8053AF80 (u32 at 0x58, size 0x90 per GameState.h comment on fn_8045A080) + extern lbl_8053AF80.
gen_game.tsv: removed this unit's 2 rows.
