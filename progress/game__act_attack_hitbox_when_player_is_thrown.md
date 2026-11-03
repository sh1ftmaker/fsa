# game/act_attack_hitbox_when_player_is_thrown

Baseline: total 5734 / 11557 functions, 1152552 bytes. After: 5738, 1152640.
State: 6/7 functions match (was 2/7), .text 99.13 %, .sdata2 90.91 %, extab/extabindex 40 %.

Resistant: update() 97.53 % scheduling+regalloc. The box copy + offset adds: target keeps maxx in f2 and
maxy in f3, ours swaps them, and stfs of box.f.x lands one lfs earlier. Tried: TBox2f::addPos(x, y)
(93 %), explicit i.x/f.x/i.y/f.y += (97.5 %), copy-construct vs assign (same).
.sdata2 90.91 %: original section is 0x18 (4.0f then 4 bytes pad at 0x8054077C), ours 0x14; trailing pad only.
extab/extabindex follow update's size mismatch only if at all; not investigated.

SHARED-EDITS: include/game/Player.h: extern "C" fn_80236BEC(s32) bool, fn_8022E438(s32,s32) bool,
fn_8023B0F4(s32, const TBox2f*, s32, bool, f32) bool, fn_80238548(s32, s32, const TVec3f*, s32) bool
(owner tu_8022AD64; types from this unit's call sites and ActorBase.h's fn_801F2B7C/fn_801F2A10 comments).
config/G4SE01/gen_game.tsv: this unit's two rows removed.
