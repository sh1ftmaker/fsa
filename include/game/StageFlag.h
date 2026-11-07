#ifndef GAME_STAGEFLAG_H
#define GAME_STAGEFLAG_H

#include "game/tu_803B8C74.h"

// Bit (stage * 4 + sub) of the current save file's mFlags (stage 0-7,
// sub 0-3, else FALSE): the body of fn_8019CE40 (tu_80197070), which the
// player's reset (tu_8023FBF0), ActorBase's death (vfunc_0x24) and the save
// data's fn_803B8EBC / fn_803B9120 (tu_803B8C74) inline.
// BOOL: every caller tests the result with cmpwi (fn_8019CE40's callers,
// 0x80254744's and 0x801F5030's inlined copies), not clrlwi.
inline BOOL isStageFlagSet(s32 stage, s32 sub) {
    if (stage < 0 || stage > 7) {
        return FALSE;
    }
    if (sub < 0 || sub > 3) {
        return FALSE;
    }
    return ((1 << (stage * 4 + sub)) & lbl_8050EC80.getCurrentFile().mFlags) ? TRUE
                                                                                               : FALSE;
}

// Sets bit (stage * 4 + sub) of the current save file's mFlags (stage 0-7,
// sub 0-3, else nothing): the body of fn_8019CDE4 (tu_80197070), which
// ResultScreen_804AF9A0::fn_803AB494 (tu_803A8DFC) inlines. fn_8019CDE4 itself keeps
// its own copy of the body: as a wrapper calling this inline it compiles the early
// returns to ble/blr (21 instrs, 90.87 %) instead of the original bltlr/bgtlr.
inline void setStageFlag(s32 stage, s32 sub) {
    if (stage < 0 || stage > 7) {
        return;
    }
    if (sub < 0 || sub > 3) {
        return;
    }
    lbl_8050EC80.getCurrentFile().mFlags |= 1 << (stage * 4 + sub);
}

#endif
