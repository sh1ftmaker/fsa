#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/EnemyBase.h"
#include "game/GameGlobals.h"
#include "game/GameMath.h"
#include "game/Message.h"
#include "game/Player.h"
#include "game/Camera.h"
#include "game/GameView.h"
#include "game/tu_80131D98.h"
#include <string.h>
#include "std/math.h"
#include "JSystem/JMath/JMath.h"
#include "game/tu_801213EC.h"
#include "game/act/act_life_meter.h"

// Passes the position on by value (the original loads the other arguments before its copy).
static inline bool talk(s32 player, u32 msg, s32 serial, s32 layer, const JGeometry::TVec3f& pos, u32 flags) {
    return fn_8038AD24(player, msg, serial, layer, pos, flags);
}

// The ids vfunc_0x80 asks fn_801F26CC about (0xAC bytes of 'PEAC', 'OKTA', 'AMOS', ... ids).
extern const u32 lbl_80464CD8[43];

static const u16 lbl_80464D84[8] = {0x2C, 0x2D, 0x2E, 0x2F, 0x28, 0x29, 0x2A, 0x2B};

// FIXME(shotgun): 99.8 %, reloc-name
EscortBase_8049E0C8::EscortBase_8049E0C8(const char* sobName, s32 arg2, s32 arg3) {
    field_0x330 = 1;
    field_0x331 = 1;
    field_0x332 = arg3;
    field_0x333 = 0;
    field_0x334 = 0;
    field_0x335 = 0;
    field_0x336 = 0;
    field_0x338 = arg2;
    field_0x33c = 0;
    field_0x340 = 0;
    field_0x344 = -1;
    field_0x348 = -1;
    field_0x34c = -1;
    field_0x350 = 0;
    field_0x354 = 0;
    field_0x358 = 0;
    field_0x360 = 1;
    field_0x364[4] = -1;
    field_0x378 = 0;
    field_0x37c = 1.0f;
    field_0x380 = 0.0f;
    field_0x364[0] = -1;
    field_0x364[1] = -1;
    field_0x364[2] = -1;
    field_0x364[3] = -1;
    strcpy(mSobName, sobName);
}

// FIXME(shotgun): 100 %, matches
EscortBase_8049E0C8::~EscortBase_8049E0C8() {
    lbl_80539F30 = -1;
}

// The escort's answer to a screen change (see the header); 0x5C is called without the direction
// argument the original passes in r4 (the slot's signature is not known).
// FIXME(shotgun): 86.1 %, missing/extra (the slot-0x5C call lacks its r4 argument)
s32 fn_80124600(const JGeometry::TVec2<s32>* dir) {
    s32 count = fn_80131F0C()->mCount;
    if (count == 0) {
        s32 serial = lbl_80539F30;
        if (serial < 0) {
            return 3;
        }
        EscortBase_8049E0C8* escort = static_cast<EscortBase_8049E0C8*>(getActorManager_801F856C()->findActorBySerial_801F666C(serial));
        if (escort == NULL) {
            return 3;
        }
        escort->field_0x334 = 1;
        return escort->vfunc_0x5C();
    }
    bool flag = false;
    for (s32 i = 0; i < count; i++) {
        EscortBase_8049E0C8* escort = static_cast<EscortBase_8049E0C8*>(fn_80131F0C()->fn_80131E30(i));
        if (escort != NULL && escort->mActorId == ACTOR_FAZR) {
            escort->field_0x334 = 1;
            switch (escort->vfunc_0x5C()) {
            case 1:
                return 1;
            case 2:
                flag = false;
                break;
            case 3:
                break;
            default:
                flag = false;
                break;
            }
        }
    }
    if (flag) {
        return 3;
    }
    return 2;
}

// FIXME(shotgun): 99.4 %, regalloc-only + reloc-name
void EscortBase_8049E0C8::fn_80124448() {
    ActorBase_804A5770::init();
    lbl_80539F30 = mSerial;
    vfunc_0x64();
    mBox.i.x = -7.0f;
    mBox.i.y = -7.0f;
    mBox.f.x = 7.0f;
    mBox.f.y = 7.0f;
    mBox2.i.x = mBox.i.x;
    mBox2.i.y = mBox.i.y;
    mBox2.f.x = mBox.f.x;
    mBox2.f.y = mBox.f.y;
    field_0x80.i.x = mBox.i.x;
    field_0x80.i.y = mBox.i.y;
    field_0x80.f.x = mBox.f.x;
    field_0x80.f.y = mBox.f.y;
    field_0xb8 = 2;
    vfunc_0x70();
    vfunc_0x74();
    getGameView_802D6A9C()->fn_802D7440(0);
    if (field_0x332) {
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
}

// FIXME(shotgun): 94.1 %, scheduling+regalloc + reloc-name
void EscortBase_8049E0C8::update() {
    ActorBase_804A5770::update();
    field_0x37c = 1.0f;
    field_0x331 = 1;
    JGeometry::TBox2f box(mBox);
    box.addPos(mPos.x, mPos.y);
    vfunc_0x80();
    if (field_0x333) {
        field_0x354++;
        if (field_0x354 >= 300) {
            field_0x333 = 0;
            field_0x354 = 0;
        }
    }
    if (field_0x335) {
        field_0x378++;
        if (field_0x378 >= 90) {
            field_0x335 = 0;
            field_0x378 = 0;
        }
    }
    vfunc_0x68();
    if (field_0x330) {
        fn_80239914(&box, field_0x4, mSerial);
    }
    JGeometry::TVec3f out;
    if (field_0x331) {
        fn_801F2618(&out, false);
    }
    JGeometry::TVec3f dir;
    dir.x = dir.y = dir.z = 0.0f;
    if (fn_80121438(&dir)) {
        mPos.add(dir);
    }
    mAnim.mFrameCtrl.setRate(field_0x37c);
    mAnim.fn_801D1C18();
    fn_801217EC(mLife == 0);
    gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
    vfunc_0x90();
}

// FIXME(shotgun): 97.2 %, scheduling+regalloc + reloc-name
void EscortBase_8049E0C8::draw() {
    ActorBase_804A5770::draw();
    if (field_0x332) {
        JGeometry::TVec3f pos(mPos);
        pos.x -= 6.0f * (field_0x360 - 1);
        pos.y += -28.0f;
        for (s32 i = 0; i < field_0x360; i++) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x364[i]);
            if (actor == NULL) {
                field_0x364[i] = -1;
            } else {
                if (i > 0) {
                    pos.x += 12.0f;
                }
                actor->mPos = pos;
            }
        }
    }
    SpriteParams_801F5AE0 params;
    if (field_0x333) {
        params.field_0x10.a = (s32)(255.0f * JMath::sincosTable_.sinDegree((f32)(field_0x354 * 60)));
    }
    fn_801F02BC(mSobName, field_0x338 + mAnim.mCel, &params, 0x407, 0, false, 1.0f, 1.0f);
}

bool EscortBase_8049E0C8::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    u16 tile = fn_8022461C(pos, layer);
    for (s32 i = 0; i < 8; i++) {
        if (tile == lbl_80464D84[i]) {
            return false;
        }
    }
    return ActorBase_804A5770::vfunc_0x4C(layer, pos);
}

s32 EscortBase_8049E0C8::vfunc_0x5C() {
    return 3;
}

s32 EscortBase_8049E0C8::vfunc_0x6C(s32 state) {
    field_0x340 = field_0x33c;
    field_0x33c = state;
    return 1;
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

void EscortBase_8049E0C8::vfunc_0x7C() {
    if (gGameRoot_8053AAF8->fn_80138A30() == 1) {
        field_0x360 = 1;
    } else {
        field_0x360 = 4;
    }
    mLife = field_0x360 * 4;
}

// FIXME(shotgun): 97 %, missing/extra + reloc-name
s32 EscortBase_8049E0C8::vfunc_0x80() {
    if (!(u8)vfunc_0x78()) {
        return 0;
    }
    s32 serial = fn_801F26CC(lbl_80464CD8);
    if (serial < 0) {
        return 0;
    }
    ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
    if (actor != NULL) {
        switch (actor->mActorId) {
        case ACTOR_FIRE: {
            u32 kind = (actor->field_0x90 >> 6) & 0xF;
            if (kind <= 1 || kind == 5 || kind == 6) {
                return 0;
            }
        }
        // fall through
        case ACTOR_OKTM:
            // The state word the two actors share (EnemyBase::mFlags is the closest known name).
            if (static_cast<EnemyBase_804A4808*>(actor)->mFlags == 2) {
                return 0;
            }
            break;
        }
    }
    mLife -= 2;
    if (mLife > 0) {
        if (serial >= 0) {
            ActorBase_804A5770* target = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (target != NULL) {
                JGeometry::TVec3f pos = target->mPos;
                field_0x98 = fn_801F681C((u16)(s32)(JMath::atanTable_.atan2_(pos.x - mPos.x, (pos.y - pos.z) - (mPos.y - mPos.z)) * 10430.378f));
            }
        }
        vfunc_0x94();
    }
    return 1;
}

// FIXME(shotgun): 99.9 %, stack layout (dist/who slots swapped)
void EscortBase_8049E0C8::vfunc_0x98(s32 a, s32 b, f32 limit) {
    u16 angle;
    s32 dir;
    f32 dist;
    s32 who;
    fn_801EEC98(mSerial, &angle, &dir, &dist, &who, a, b);
    if (dir >= 0 && !(dist > limit)) {
        fn_80122730(dir, true, true);
    }
}

// FIXME(shotgun): 98.6 %, different + reloc-name
void EscortBase_8049E0C8::fn_8012309C(u16* outAngle, s32* outDir, f32* outDist, s32* outPlayer, s32 player) {
    if (field_0x334) {
        fn_801EEC98(mSerial, outAngle, outDir, outDist, outPlayer, player, 2);
        field_0x334 = 0;
    } else {
        f32 y = 0.0f;
        f32 x = 0.0f;
        s32 who = player;
        switch (fn_8022CAF8(player)) {
        case 0:
            vfunc_0xA4(&x, &y, &who);
            break;
        case 1:
            vfunc_0xA8(&x, &y, &who);
            break;
        case 2:
            vfunc_0xAC(&x, &y, &who);
            break;
        case 3:
            vfunc_0xB0(&x, &y, &who);
            break;
        case 5:
            vfunc_0xB4(&x, &y, &who);
            break;
        default:
            vfunc_0xB8(&x, &y, &who);
            break;
        }
        f32 dx = mPos.x - (x + getPlayerPos_8023E724(who)->x);
        f32 dy = mPos.y - (y + getPlayerPos_8023E724(who)->y);
        *outDist = std::sqrtf(dx * dx + dy * dy);
        *outAngle = (s32)(JMath::atanTable_.atan2_(-dx, -dy) * 10430.378f);
        *outDir = fn_801F681C(*outAngle);
        *outPlayer = player;
    }
}

// FIXME(shotgun): 99.5 %, reloc-name
void EscortBase_8049E0C8::fn_80122F6C(f32* x, f32* y, s32* player) {
    f32 v = 0.0f;
    if (vfunc_0xC4()) {
        v = 16.0f;
    }
    f32 off = 0.5f * (fn_8022E970(*player) - 1) - 0.5f;
    switch (fn_80230CFC(*player)) {
    case 0:
        *x = 16.0f * off + v;
        *y = 16.0f;
        break;
    case 1:
        *x = 16.0f * off + v;
        *y = 16.0f;
        break;
    case 2:
        *x = 16.0f * off + v;
        *y = -16.0f;
        break;
    case 3:
        *x = 16.0f * off + v;
        *y = 16.0f;
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

void EscortBase_8049E0C8::vfunc_0xB0(f32* x, f32* y, s32* player) {
    f32 v = 0.0f;
    if (vfunc_0xC4()) {
        v = 16.0f;
    }
    *player = fn_8022EA0C(*player);
    *x = v;
    *y = 16.0f + v;
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

s32 EscortBase_8049E0C8::fn_8012285C() const {
    if (field_0x344 < 0 || field_0x344 >= 4) {
        return -1;
    }
    return fn_8022EA0C(field_0x344);
}

bool EscortBase_8049E0C8::fn_80122834() {
    if (mLife == 0 && field_0x333 == 0) {
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

bool EscortBase_8049E0C8::fn_801227D4() {
    field_0x350++;
    if (field_0x350 >= 90) {
        field_0x350 = 0;
        return true;
    }
    return false;
}

void EscortBase_8049E0C8::fn_80122730(s32 anim, bool shifted, bool throttled) {
    if (anim >= 0) {
        if (throttled) {
            field_0x364[4]++;
            if (field_0x364[4] >= 10) {
                field_0x364[4] = 10;
            }
            if (field_0x364[4] < 10) {
                return;
            }
            field_0x364[4] = 0;
        }
        field_0x98 = anim;
        if (shifted) {
            anim += 5;
        }
        if (anim != mAnim.mAnim) {
            mAnim.startAnim_801D22DC(anim, 2, false);
            mAnim.fn_801D1F14(0.0f);
        }
    }
}

void EscortBase_8049E0C8::fn_801220C4() {
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

void EscortBase_8049E0C8::fn_80121FA8() {
    JGeometry::TVec3f pos(mPos);
    pos.y += -24.0f;
    spawnEmitter_8013CC50(field_0x4, &pos, 0x33A, 0, 0, NULL, NULL, true, 1.0f);
    startSound_801F0E34(0x400ED, 0);
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

f32 EscortBase_8049E0C8::fn_80121DC8(s32 player) {
    JGeometry::TVec3f playerPos = *getPlayerPos_8023E724(player);
    return JMAFastSqrt(square(playerPos.x - mPos.x) + square(playerPos.y - mPos.y));
}

s32 EscortBase_8049E0C8::fn_80121DB0() {
    mRail.clear();
    return 1;
}

// FIXME: 92.8. The original passes field_0x35c to fn_800FE3EC's u8 parameter without the clrlwi (and keeps this in r6).
s32 EscortBase_8049E0C8::fn_80121D68() {
    mRail.fn_800FE3EC(field_0x35c, mPos, field_0x4);
    return 1;
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

// The heart index (0-4) a life meter shows for `life` (clamped).
static inline s32 heartIndex(s32 life) {
    if (life < 0) {
        life = 0;
    }
    if (life >= 4) {
        life = 4;
    }
    return life;
}

// LifeMeter_804B174C::fn_8019CEF8's body, which the original inlines into fn_801217EC: it was a
// header inline (its out-of-line copy sits with tu_80197070's other inline copies at that unit's
// end). It stays a plain function in tu_80197070 until its out-of-line caller there (fn_80198828)
// is written; then this copy goes and both call the header inline.
static inline void followActor(LifeMeter_804B174C* meter, s32 serial, f32 yOffset, s32 index) {
    meter->field_0x250 = serial;
    meter->field_0x254 = yOffset;
    meter->field_0x24c = index;
}

// update's life-meter refresh: respawns the missing meters, then shows each meter (or hides it)
// and points it at the escort with its heart index. `dead` is update's mLife == 0.
// FIXME: 99.69. Regalloc only (+ reloc-name: the pool's first literal, 8.0f, belongs to a function
// not in the binary): in the respawn loop the original keeps the array pointer in r30 and the
// spawned serial in r31; here they are swapped.
void EscortBase_8049E0C8::fn_801217EC(bool dead) {
    if (!field_0x332) {
        return;
    }
    respawnLifeMeters();
    for (s32 i = 0; i < field_0x360; i++) {
        LifeMeter_804B174C* meter =
            (LifeMeter_804B174C*)getActorManager_801F856C()->findActorBySerial_801F666C(field_0x364[i]);
        if (!meter) {
            field_0x364[i] = -1;
            continue;
        }
        if (field_0x333 || dead || field_0x335) {
            meter->mDrawEnabled = true;
        } else {
            meter->mDrawEnabled = false;
        }
        if (dead) {
            if (mAnim.mKey < 8) {
                followActor(meter, mSerial, -28.0f, heartIndex(mLife + 2 - i * 4));
            } else if (mAnim.mKey < 12) {
                followActor(meter, mSerial, -28.0f, mLife);
            } else {
                meter->mDrawEnabled = false;
            }
        } else if (field_0x335) {
            followActor(meter, mSerial, -28.0f, heartIndex(mLife - i * 4));
        } else if (field_0x333) {
            if (field_0x354 < 45) {
                followActor(meter, mSerial, -28.0f, heartIndex(mLife + 2 - i * 4));
            } else {
                followActor(meter, mSerial, -28.0f, heartIndex(mLife - i * 4));
            }
        }
    }
}

// Inlined into fn_801217EC only (its out-of-line copy was dead-stripped): spawns the life
// meters whose serials are gone.
void EscortBase_8049E0C8::respawnLifeMeters() {
    if (field_0x332) {
        for (s32 i = 0; i < field_0x360; i++) {
            if (field_0x364[i] < 0) {
                spawnLifeMeter(i);
            }
        }
    }
}

// Inlined into respawnLifeMeters only (dead-stripped): spawns meter `index`, hidden, 8 units
// apart to the left of the escort.
void EscortBase_8049E0C8::spawnLifeMeter(s32 index) {
    if (field_0x332 && index < field_0x360 && index >= 0) {
        JGeometry::TVec3f pos(mPos);
        pos.x -= 8.0f * index + 16.0f;
        field_0x364[index] = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_LIMT, field_0x4, &pos, 0, -1, -1);
        s32 serial = field_0x364[index];
        if (serial >= 0) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (actor) {
                actor->mDrawEnabled = false;
            }
        }
    }
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

// FIXME: 99.76. Reloc-name only: the unit's first .sdata2 literal (8.0f, lbl_8053C9B0) comes from
// a dead-stripped function emitted before this one, so every pool entry up to 0x1C is one slot off.
// When a live magic circle ('CIRC') on the escort's layer overlaps the escort's box grown by 16,
// dir is the xy direction from the circle to the escort, length 1.6; else dir stays zero.
bool EscortBase_8049E0C8::fn_80121438(JGeometry::TVec3f* dir) {
    dir->zero();
    s32 serial = getActorManager_801F856C()->fn_801F7A08(ACTOR_CIRC);
    if (serial < 0) {
        return false;
    }
    ActorBase_804A5770* circle = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
    if (!circle) {
        return false;
    }
    if (!circle->mAlive) {
        return false;
    }
    if (field_0x4 != circle->field_0x4) {
        return false;
    }
    JGeometry::TBox2f box;
    JGeometry::TBox2f circleBox;
    fn_801F31C4(&box);
    circle->fn_801F31C4(&circleBox);
    circleBox.i.x += -16.0f;
    circleBox.i.y += -16.0f;
    circleBox.f.x += 16.0f;
    circleBox.f.y += 16.0f;
    JGeometry::TBox2f overlap(box);
    if (overlap.intersect(circleBox)) {
        JGeometry::TVec3f circlePos(circle->mPos);
        dir->set(mPos.x - circlePos.x, mPos.y - circlePos.y, 0.0f);
        dir->normalize();
        dir->scale(1.6f);
        return true;
    }
    return false;
}

bool EscortBase_8049E0C8::vfunc_0xC4() {
    return false;
}

s32 EscortBase_8049E0C8::vfunc_0xC0() {
    return 1;
}

void EscortBase_8049E0C8::vfunc_0x94() {}

void EscortBase_8049E0C8::vfunc_0x90() {}

void EscortBase_8049E0C8::vfunc_0x8C() {}

s32 EscortBase_8049E0C8::vfunc_0x88() {}

// 0x84/0x88: s32 slots whose result the caller (fn_80123394) passes on, but the base body is a bare blr.
s32 EscortBase_8049E0C8::vfunc_0x84() {}

void EscortBase_8049E0C8::vfunc_0x74() {}

void EscortBase_8049E0C8::vfunc_0x70() {}

void EscortBase_8049E0C8::vfunc_0x68() {}

void EscortBase_8049E0C8::vfunc_0x64() {}

u8 EscortBase_8049E0C8::vfunc_0x60() {
    return 0;
}

bool EscortBase_8049E0C8::vfunc_0x58() {
    return false;
}

bool EscortBase_8049E0C8::vfunc_0x54() {
    return false;
}
