# game/act_dummy_conversation

Baseline: total: 5839 / 11557 functions, 1169096 / 4442032 bytes of code (26.32 %).
After: 5840 / 11557, 1169180 bytes. Unit 2/4 -> 3/4 functions.

Matched: __dt, init, __sinit (generated lines kept; gen_game.tsv rows removed).

G4 revisit baseline: total 5922 / 11557 functions, 1186620 bytes (26.71 %); unit 3/4.
Applied the by-value rule: member `open(player, layer, serial, const TVec3f&, flags)` in the
act header reads mMessage inside (after the psq copy); layer before serial as parameters.
update 94.27 -> 99.96 % (the r30/r31 swap went away too).

Resistant: update (99.96 %, regalloc-only). Only `cmpw r3, r30` (fn_8022FD8C result, player)
in the original vs `cmpw r30, r3` ours; `player == call` compiles identically. 3 tries used.

SHARED-EDITS: include/game/GameGlobals.h (cls_804AA8C8 / lbl_8053AD30 + fn_8030AAF4,
fn_80309D24, fn_80309D6C; cls_804B1920 / lbl_8053AED0 byte array at 0x1F0),
include/game/Player.h (fn_8022FE80, fn_8022FD8C, fn_8023061C), config/G4SE01/gen_game.tsv
(unit rows removed), include/game/act/act_dummy_conversation.h (generator-owned: mState,
mMessage).
Next: extab/extabindex at 47 % should be rechecked once update matches.
