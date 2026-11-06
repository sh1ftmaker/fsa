#ifndef GAME_STAGEFLAG_H
#define GAME_STAGEFLAG_H

#include "game/tu_80379F84.h"

// Bit (stage * 4 + sub) of the current save row's field_0x28 (stage 0-7,
// sub 0-3, else FALSE): the body of fn_8019CE40 (tu_80197070), which the
// player's reset (tu_8023FBF0) and ActorBase's death (vfunc_0x24) inline.
// BOOL: every caller tests the result with cmpwi (fn_8019CE40's callers,
// 0x80254744's and 0x801F5030's inlined copies), not clrlwi.
inline BOOL isStageFlagSet(s32 stage, s32 sub) {
    if (stage < 0 || stage > 7) {
        return FALSE;
    }
    if (sub < 0 || sub > 3) {
        return FALSE;
    }
    return ((1 << (stage * 4 + sub)) & lbl_8050EC80[lbl_8050EC80[0].field_0x10].field_0x28) ? TRUE
                                                                                             : FALSE;
}

#endif
