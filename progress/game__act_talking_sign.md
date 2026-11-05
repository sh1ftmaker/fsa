# game/act_talking_sign

Baseline: total: 5839 / 11557 functions, 1169096 / 4442032 bytes of code (26.32 %)
After: 5841 / 11557, 1169372 bytes.

Matched: dtor, draw, init, __sinit (4/5).
Resistant: update (90.26 %, missing/extra): the two fn_8038C678 calls load mSerial/field_0x4 into
r29/r28 before getPlayerPos_8023E724 in the original; ours loads them into r5/r6 after. Tried a local TVec3f
and an inline openMessage helper (worse). Area box f.x store scheduled earlier than original.
SHARED-EDITS: Player.h (fn_8022FEB8, fn_8022FD8C), GameGlobals.h (fn_80138AB0), gen_game.tsv rows removed.
