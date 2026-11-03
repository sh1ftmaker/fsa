// The SLR1 actor (sword soldier, docs/names/actors.tsv; vtable 0x8049FCA0,
// constructor 0x80203CA4 in the spawner). Only the random helper below is
// written so far.

#include "game/GameGlobals.h"

// A random float in [0, 1) from the game's RNG. Called from 10 units.
// The instructions match; its 1.0f only pairs with lbl_8053D25C once the
// unit's earlier functions put 0.0f (lbl_8053D258) first in .sdata2.
f32 fn_80154744() {
    return lbl_8053AAF8->mRandom.get_ufloat_1();
}
