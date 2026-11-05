#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/EnemyBase.h"
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

bool EscortBase_8049E0C8::vfunc_0xC4() {
    return false;
}

// Never reads this: only the player argument.
s32 EscortBase_8049E0C8::fn_801216D0(s32 player) {
    if (!fn_8023E98C(player)) {
        return -1;
    }
    for (s32 i = 0; i < 4; i++) {
        if (fn_8023ED8C(i)) {
            s32 owner = fn_8023ECF0(i);
            if (player == owner && fn_8022D75C(i)) {
                return i;
            }
        }
    }
    return -1;
}

void EscortBase_8049E0C8::fn_80121764() {
    if (field_0x332) {
        for (s32 i = 0; i < field_0x360; i++) {
            ActorBase_804A5770* actor = fn_801F856C()->fn_801F666C(field_0x364[i]);
            if (actor && actor->mActorId == ACTOR_LIMT) {
                actor->mAlive = false;
                field_0x364[i] = -1;
            }
        }
    }
}

// FIXME: 99.1. Regalloc only: the original keeps the array pointer in r29 and the counter in r30; here they are swapped.
void EscortBase_8049E0C8::fn_80121ABC() {
    lbl_8053AAA0 = 0;
    vfunc_0x7C();
    if (field_0x332) {
        JGeometry::TVec3f pos(mPos);
        pos.x -= 16.0f;
        for (s32 i = 0; i < field_0x360; i++) {
            pos.x += 8.0f;
            field_0x364[i] = fn_801F856C()->fn_801F9484(ACTOR_LIMT, field_0x4, &pos, 0, -1, -1);
            s32 serial = field_0x364[i];
            if (serial < 0) {
                break;
            }
            ActorBase_804A5770* actor = fn_801F856C()->fn_801F666C(serial);
            if (actor) {
                actor->mDrawEnabled = false;
            }
        }
    }
}

// FIXME: 92.8. The original passes field_0x35c to fn_800FE3EC's u8 parameter without the clrlwi (and keeps this in r6).
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

bool EscortBase_8049E0C8::fn_80121E40() {
    switch (field_0x98) {
    case 0:
        fn_801CD950(&field_0x3c.x, 0.0f, 0.1f);
        break;
    case 1:
        fn_801CD950(&field_0x3c.x, 0.0f, -0.1f);
        break;
    case 2:
        fn_801CD950(&field_0x3c.y, 0.0f, 0.1f);
        break;
    case 3:
        fn_801CD950(&field_0x3c.y, 0.0f, -0.1f);
        break;
    }
    field_0x3c.z -= 0.2f;
    mPos += field_0x3c;
    if (mPos.z <= 0.0f) {
        mPos.z = 0.0f;
        return true;
    }
    return false;
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

void EscortBase_8049E0C8::fn_80121FA8() {
    JGeometry::TVec3f pos(mPos);
    pos.y += -24.0f;
    fn_8013CC50(field_0x4, &pos, 0x33A, 0, 0, NULL, NULL, true, 1.0f);
    fn_801F0E34(0x400ED, 0);
}

void EscortBase_8049E0C8::fn_8012202C() {
    JGeometry::TVec3f pos(mPos);
    pos.y += -24.0f;
    fn_8013CC50(field_0x4, &pos, 0x33A, 0, 0, NULL, NULL, true, 1.0f);
    fn_801F0E34(0x400ED, 0);
    mAnim.fn_801D22DC(9, 0, false);
}

void EscortBase_8049E0C8::fn_801220C4() {
    JGeometry::TVec3f pos(mPos);
    pos.y += -24.0f;
    fn_8013CC50(field_0x4, &pos, 0x33A, 0, 0, NULL, NULL, true, 1.0f);
    fn_801F0E34(0x400ED, 0);
}

// FIXME: 91.8. The box corners are added in a different order and the stores of the box's y values and the pt zero are interleaved differently (target: x pair, then y pair, stores after pt's).
bool EscortBase_8049E0C8::vfunc_0xBC() {
    JGeometry::TBox2f box(mBox);
    box.i.x += mPos.x;
    box.f.x += mPos.x;
    box.i.y += mPos.y;
    box.f.y += mPos.y;
    JGeometry::TVec2f pt(0.0f, 0.0f);
    if (fn_8023AE34(&box, field_0x4, &pt) >= 0) {
        return true;
    }
    bool inBox = fn_8023AB94(&box, field_0x4, &pt) >= 0;
    return inBox;
}

void EscortBase_8049E0C8::vfunc_0xB0(f32* x, f32* y, s32* player) {
    f32 v = 0.0f;
    if (vfunc_0xC4()) {
        v = 16.0f;
    }
    *player = fn_8022EA0C(*player);
    *x = v;
    *y = 16.0f + v;
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
