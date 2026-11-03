# game/act_zoom

Baseline (progress.py): 5783 / 11557 functions, 1160172 bytes. After: 5785, 1160388.

Matched: dtor, draw, init (100%), sinit.
Resistant: update (91%, scheduling+regalloc). FIXME in source: original loads mPos.y, mPos.x
before copying mBox to the stack, then adds into f4/f3/f2/f0; same shape as
act_attack_hitbox_when_player_is_thrown::update. extab/extabindex below 100% follow from it.

SHARED-EDITS: include/game/Player.h: added fn_8023AF14 (any-player box test, s32) and
fn_8023077C (bool) in the extern "C" block (owner tu_8022AD64). config/G4SE01/gen_game.tsv:
removed both act_zoom rows.
