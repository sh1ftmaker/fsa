# game/act_stop_npc.cpp
Baseline progress.py: 5734 / 11557 functions, 1152552 bytes (25.95 %). After: 5738 / 1152848+.
Matched: dtor, init, update, draw (+ generated __sinit).
Resistant: fn_802ED140 (StopNPC_804AA2D8::fn_802ED140) 99.30 % regalloc-only: loop registers rotated
(orig end=r31, i=r29, offset=r30; ours r30/r29/r31). Tried `for (i=0,end=..)` and separate `s32 i;` (worse).
SHARED-EDITS: GameGlobals.h (GameRoot::fn_80134FBC(u8), Unk_8045A080::fn_80458658(bool), extern "C" fn_802AC09C(ItemPedestal*)); symbols.txt renames of those.
