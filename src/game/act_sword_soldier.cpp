// The SLR1 actor (sword soldier, docs/names/actors.tsv; vtable 0x8049FCA0,
// constructor 0x80203CA4 in the spawner). Only the two helpers below are
// written so far.

#include "game/GameGlobals.h"
#include "game/act/act_sword_soldier.h"
#include "std/math.h"

s32 SwordSoldier_8049FCA0::fn_801544AC() const {
    return field_0x28c;
}

// FIXME: the instructions match; its four constants (0.0f, .5, 3.0, 0.0)
// only pair with lbl_8053D258 / lbl_8053D2F8-lbl_8053D308 once the unit's
// .sdata2 is laid out in the original order. std::sqrtf out of line; 13
// calls from 7 units.
s32 SwordSoldier_8049FCA0::fn_801544C0() {
    return field_0x288;
}

u32 SwordSoldier_8049FCA0::fn_801544C8() const {
    return field_0x90;
}

f32 fn_80154378(f32 x) {
    return std::sqrtf(x);
}

// Keeps pos in the camera's view (3 units call it; they ignore r3).
void fn_80154718(const JGeometry::TVec3f* pos) {
    lbl_8053AB10->field_0x24->fn_8030C210(pos);
}

// A random float in [0, 1) from the game's RNG. Called from 10 units.
// The instructions match; its 1.0f only pairs with lbl_8053D25C once the
// unit's earlier functions put 0.0f (lbl_8053D258) first in .sdata2.
f32 fn_80154744() {
    return lbl_8053AAF8->mRandom.get_ufloat_1();
}
