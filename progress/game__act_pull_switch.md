# game/act_pull_switch

Baseline: total 5980 / 11557 functions, 1201588 bytes (27.05 %). After: 5982 functions, 1201980 bytes.

Matched: dtor, draw, init, __sinit (generated rows removed from gen_game.tsv).
Resistant: update (99.94 %, class `operands`): the push vector passed to
fn_8023DFF4 sits at sp+0x20 in the original, ours shares the by-value player
position copy's slot at sp+0x2c. Tried: named/outer-scope push, helper returning
by value, out-param helper (current: out-param, in-place `a.sub(b); result = a`).
sdata2 95.65 %: the original's 8-byte entry at 0x805404B8 is -8.0f followed by 0.
SHARED-EDITS: include/game/Player.h (fn_80239470, fn_8023DFF4, fn_80230AD4 declarations,
owner tu_8022AD64); config/G4SE01/gen_game.tsv rows removed; act_pull_switch.h fields.
