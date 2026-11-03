# game/act_dummy_conversation

Baseline: total: 5839 / 11557 functions, 1169096 / 4442032 bytes of code (26.32 %).
After: 5840 / 11557, 1169180 bytes. Unit 2/4 -> 3/4 functions.

Matched: __dt, init, __sinit (generated lines kept; gen_game.tsv rows removed).

Resistant: update (94.27 %, scheduling+regalloc). `this` is r31 and player r30 in the
original; ours swaps them (u32 player gives cmplw, original is cmpw, so s32; `player ==
call` vs `call == player` made no difference to the swap). Also the by-value mPos copy
for fn_8038C678 is ordered after the r5/r6 loads in the original (same as act_talking_npc).
Replacing mPos with a dummy kept the r30/r31 swap, so it is not the copy.

SHARED-EDITS: include/game/GameGlobals.h (cls_804AA8C8 / lbl_8053AD30 + fn_8030AAF4,
fn_80309D24, fn_80309D6C; cls_804B1920 / lbl_8053AED0 byte array at 0x1F0),
include/game/Player.h (fn_8022FE80, fn_8022FD8C, fn_8023061C), config/G4SE01/gen_game.tsv
(unit rows removed), include/game/act/act_dummy_conversation.h (generator-owned: mState,
mMessage).
Next: extab/extabindex at 47 % should be rechecked once update matches.
