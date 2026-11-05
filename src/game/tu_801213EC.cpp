#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/EnemyBase.h"
#include "game/GameGlobals.h"
#include "game/GameMath.h"
#include "game/Message.h"
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

void EscortBase_8049E0C8::vfunc_0x74() {}

// 0x84/0x88: s32 slots whose result the caller (fn_80123394) passes on, but the base body is a bare blr.
s32 EscortBase_8049E0C8::vfunc_0x84() {}

s32 EscortBase_8049E0C8::vfunc_0x88() {}

void EscortBase_8049E0C8::vfunc_0x8C() {}

void EscortBase_8049E0C8::vfunc_0x90() {}

void EscortBase_8049E0C8::vfunc_0x94() {}

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
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x364[i]);
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
            field_0x364[i] = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_LIMT, field_0x4, &pos, 0, -1, -1);
            s32 serial = field_0x364[i];
            if (serial < 0) {
                break;
            }
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
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
    JGeometry::TVec3f playerPos = *getPlayerPos_8023E724(player);
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
    spawnEmitter_8013CC50(field_0x4, &pos, 0x33A, 0, 0, NULL, NULL, true, 1.0f);
    startSound_801F0E34(0x400ED, 0);
}

void EscortBase_8049E0C8::fn_8012202C() {
    JGeometry::TVec3f pos(mPos);
    pos.y += -24.0f;
    spawnEmitter_8013CC50(field_0x4, &pos, 0x33A, 0, 0, NULL, NULL, true, 1.0f);
    startSound_801F0E34(0x400ED, 0);
    mAnim.startAnim_801D22DC(9, 0, false);
}

void EscortBase_8049E0C8::fn_801220C4() {
    JGeometry::TVec3f pos(mPos);
    pos.y += -24.0f;
    spawnEmitter_8013CC50(field_0x4, &pos, 0x33A, 0, 0, NULL, NULL, true, 1.0f);
    startSound_801F0E34(0x400ED, 0);
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

// Passes the position on by value (the original loads the other arguments before its copy).
static inline bool talk(s32 player, u32 msg, s32 serial, s32 layer, const JGeometry::TVec3f& pos, u32 flags) {
    return fn_8038AD24(player, msg, serial, layer, pos, flags);
}

// FIXME: 96.8. The box corners' adds are scheduled differently (see vfunc_0xBC) and mSerial/field_0x4 are loaded in the other order before the talk call.
s32 EscortBase_8049E0C8::fn_80122898(u16 msg) {
    JGeometry::TBox2f box(mBox);
    box.i.x += mPos.x;
    box.f.x += mPos.x;
    box.i.y += mPos.y;
    box.f.y += mPos.y;
    JGeometry::TVec3f pos(mPos);
    pos.y -= 32.0f;
    fn_8022FEB8(&box, field_0x4, mSerial, true, false);
    s32 player = fn_8022FD8C(mSerial);
    if (player < 0 || player >= 4) {
        field_0x34c = -1;
        return -1;
    }
    if (!fn_802300EC(player)) {
        field_0x34c = -1;
        return -1;
    }
    if (talk(player, msg, mSerial, field_0x4, pos, 0x28)) {
        field_0x34c = player;
        return player;
    }
    field_0x34c = -1;
    return -1;
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

void EscortBase_8049E0C8::vfunc_0xB8(f32* x, f32* y, s32* player) {
    f32 v = 0.0f;
    if (vfunc_0xC4()) {
        v = 16.0f;
    }
    switch (fn_80230CFC(*player)) {
    case 0:
        *x = -16.0f - v;
        *y = 0.0f;
        break;
    case 1:
        *x = 16.0f + v;
        *y = 0.0f;
        break;
    case 2:
        *x = 0.0f;
        *y = -16.0f - v;
        break;
    case 3:
        *x = 0.0f;
        *y = 16.0f + v;
        break;
    }
}

// FIXME: 92.9. Branch layout differs: the original tests target < 0 / >= 4 with a direct jump past the loop (result = target
// on that path) and keeps target in r29, result in r31; ours copies target into result before the test (extra mr) and the
// loop/result registers are shifted by one.
void EscortBase_8049E0C8::vfunc_0xB4(f32* x, f32* y, s32* player) {
    *x = 0.0f;
    *y = 0.0f;
    s32 target = *player;
    s32 result = target;
    if (target >= 0 && target < 4) {
        for (s32 i = 0; i < 4; i++) {
            if (fn_8022E438(i, target)) {
                result = i;
            }
        }
    }
    *player = result;
    f32 v = 0.0f;
    if (vfunc_0xC4()) {
        v = 16.0f;
    }
    switch (fn_80230CFC(*player)) {
    case 0:
        *x = -v;
        break;
    case 1:
        *x = v;
        break;
    case 2:
        *y = -v;
        break;
    case 3:
        *y = v;
        break;
    }
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

void EscortBase_8049E0C8::vfunc_0xAC(f32* x, f32* y, s32* player) {
    f32 v = 0.0f;
    if (vfunc_0xC4()) {
        v = 16.0f;
    }
    switch (fn_80230CFC(*player)) {
    case 0:
        *x = -16.0f - v;
        *y = 8.0f;
        break;
    case 1:
        *x = 32.0f + v;
        *y = 8.0f;
        break;
    case 2:
        *x = 8.0f;
        *y = -16.0f - v;
        break;
    case 3:
        *x = 8.0f;
        *y = 32.0f + v;
        break;
    }
}

void EscortBase_8049E0C8::vfunc_0xA8(f32* x, f32* y, s32* player) {
    f32 v = 0.0f;
    if (vfunc_0xC4()) {
        v = 16.0f;
    }
    f32 offset = 0.5f * (fn_8022E970(*player) - 1) - 0.5f;
    switch (fn_80230CFC(*player)) {
    case 0:
        *x = -16.0f;
        *y = 16.0f * offset + v;
        break;
    case 1:
        *x = 16.0f;
        *y = 16.0f * offset + v;
        break;
    case 2:
        *x = -16.0f;
        *y = 16.0f * offset + v;
        break;
    case 3:
        *x = -16.0f;
        *y = 16.0f * offset + v;
        break;
    }
}

void EscortBase_8049E0C8::vfunc_0x7C() {
    if (lbl_8053AAF8->fn_80138A30() == 1) {
        field_0x360 = 1;
    } else {
        field_0x360 = 4;
    }
    mLife = field_0x360 * 4;
}

s32 EscortBase_8049E0C8::vfunc_0x78() const {
    if (field_0x333) {
        return 0;
    }
    if (fn_8012285C() >= 0 && fn_8012285C() < 4) {
        if (fn_80230188(fn_8012285C())) {
            return 0;
        }
        if (!fn_8023F264(fn_8012285C())) {
            return 0;
        }
    }
    return 1;
}

s32 EscortBase_8049E0C8::vfunc_0x6C(s32 state) {
    field_0x340 = field_0x33c;
    field_0x33c = state;
    return 1;
}

s32 EscortBase_8049E0C8::vfunc_0x5C() {
    return 3;
}
