# game/act_enemy_generator_3
Baseline: total: 5853 / 11557 functions, 1171280 / 4442032 bytes of code (26.37 %)
After: 5855 / 11557, 1171396 bytes.

Matched: dtor, draw, init. sinit already.
Resistant: update (97.79 %, missing/extra): the 30-iteration search loop; target
compares r3 (cmpwi) before `mr r29,r3` and has no post-loop count test (falls
into the mRemaining/state=4 block), ours emits `mr.` and a second cmpwi 30/blt.
Registers for field_0x4 then differ (r29 vs r30). Tried for/do-while/temp.
.sdata2 66.67 %: original pair is 1.0f, 0.0f; only 1.0f is used by update.

SHARED-EDITS: GameGlobals.h AudioMgr_8045A080::fn_80458FF0(s32,s32,bool); symbols.txt
fn_80458FF0 -> fn_80458FF0__17AudioMgr_8045A080Fllb; gen_game.tsv rows removed.
