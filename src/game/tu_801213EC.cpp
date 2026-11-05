#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameMath.h"
#include "game/Player.h"
#include "game/tu_801213EC.h"

bool EscortBase_8049E0C8::vfunc_0x54() {
    return false;
}

bool EscortBase_8049E0C8::vfunc_0x58() {
    return false;
}

u8 EscortBase_8049E0C8::vfunc_0x60() {
    return 0;
}

void EscortBase_8049E0C8::vfunc_0x64() {}

void EscortBase_8049E0C8::vfunc_0x68() {}

void EscortBase_8049E0C8::vfunc_0x70() {}

// The empty slots below return nothing in the original (a bare blr), though the
// children's overrides return s32.
s32 EscortBase_8049E0C8::vfunc_0x74() {}

s32 EscortBase_8049E0C8::vfunc_0x84() {}

s32 EscortBase_8049E0C8::vfunc_0x88() {}

s32 EscortBase_8049E0C8::vfunc_0x8C() {}

void EscortBase_8049E0C8::vfunc_0x90() {}

s32 EscortBase_8049E0C8::vfunc_0x94() {}

s32 EscortBase_8049E0C8::vfunc_0xC0() {
    return 1;
}

s32 EscortBase_8049E0C8::vfunc_0xC4() {
    return 0;
}

s32 EscortBase_8049E0C8::fn_80121D68() {
    mRail.fn_800FE3EC(field_0x35c, mPos, field_0x4);
    return 1;
}

s32 EscortBase_8049E0C8::fn_80121DB0() {
    mRail.clear();
    return 1;
}

f32 EscortBase_8049E0C8::fn_80121DC8(s32 player) {
    JGeometry::TVec3f playerPos = *fn_8023E724(player);
    return JMAFastSqrt(square(playerPos.x - mPos.x) + square(playerPos.y - mPos.y));
}

void EscortBase_8049E0C8::fn_80121F34() {
    field_0x3c.x = field_0x3c.y = field_0x3c.z = 0.0f;
    switch (field_0x98) {
    case 0:
        field_0x3c.x = -2.5f;
        break;
    case 1:
        field_0x3c.x = 2.5f;
        break;
    case 2:
        field_0x3c.y = -2.5f;
        break;
    case 3:
        field_0x3c.y = 2.5f;
        break;
    }
    field_0x3c.z = 3.0f;
}

bool EscortBase_8049E0C8::fn_801227D4() {
    field_0x350++;
    if (field_0x350 >= 90) {
        field_0x350 = 0;
        return true;
    }
    return false;
}

bool EscortBase_8049E0C8::fn_80122804() {
    field_0x358++;
    if (field_0x358 >= 15) {
        field_0x358 = 0;
        return true;
    }
    return false;
}

bool EscortBase_8049E0C8::fn_80122834() {
    if (mLife == 0 && field_0x333 == 0) {
        return true;
    }
    return false;
}

s32 EscortBase_8049E0C8::fn_8012285C() const {
    if (field_0x344 < 0 || field_0x344 >= 4) {
        return -1;
    }
    return fn_8022EA0C(field_0x344);
}

void EscortBase_8049E0C8::vfunc_0x7C() {
    if (lbl_8053AAF8->fn_80138A30() == 1) {
        field_0x360 = 1;
    } else {
        field_0x360 = 4;
    }
    mLife = field_0x360 * 4;
}

s32 EscortBase_8049E0C8::vfunc_0x6C(s32 state) {
    field_0x340 = field_0x33c;
    field_0x33c = state;
    return 1;
}

s32 EscortBase_8049E0C8::vfunc_0x5C() {
    return 3;
}
