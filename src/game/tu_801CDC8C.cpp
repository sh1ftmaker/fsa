// EnemyBase_804A4808 (game/EnemyBase.h): the enemy base class, its
// per-frame driver and the helpers its descendants call.
//
// Not written yet: vfunc_0x80 (0x801CE694), fn_801CE800, fn_801CEA60,
// fn_801CEBD0, fn_801CECF4, fn_801CF1B0, fn_801CF798, fn_801CF888,
// fn_801CFA70, fn_801CFD68, fn_801D0020, fn_801D00B4, fn_801D01A0,
// fn_801D0338, fn_801D05F8, fn_801D08B4, fn_801D0B44.

#include "game/GameAudio.h"
#include "game/EnemyBase.h"
#include "game/GameGlobals.h"
#include "game/GameMath.h"
#include "game/GameView.h"
#include "JSystem/JMath/JMath.h"
#include "JSystem/JAudio/JAUSoundObject.h"

const bool lbl_8053EFC0 = false;

// GameView byte 0x131DF + layer: set for the layers where landing goes
// through vfunc_0x80 (the floor check). The callers load the layer before
// they call fn_802D6A9C (0x801CE1C8, 0x801D0DE4), as for an argument.
static inline u8 viewLayerFlag(s32 layer) {
    return fn_802D6A9C()->field_0x131df[layer];
}

// Stops sound `soundId` on this actor's sound object.
void EnemyBase_804A4808::fn_801CDC8C(u32 soundId) {
    // ActorSoundObj_804578F4 is a JAUSoundObject (game/ActorManager.h) but
    // opaque until game/tu_80455CA4.cpp is written.
    JAUSoundObject* sound = (JAUSoundObject*)fn_801F1184();
    if (sound != NULL) {
        sound->stopSound(soundId, 0);
    }
}

// FIXME: 99.4 %, regalloc only: the target loads pos->x into f2 and
// pos->z into f0, ours the other way round.
// Starts sound `soundId` as if this actor were at *pos on `layer`.
void EnemyBase_804A4808::fn_801CDCD0(s32 layer, const JGeometry::TVec3f* pos, u32 soundId) {
    s32 savedLayer = field_0x4;
    JGeometry::TVec3f savedPos = mPos;
    field_0x4 = layer;
    mPos.set(pos->x, pos->y, pos->z);
    fn_801F0E34(soundId, 0);
    field_0x4 = savedLayer;
    mPos.set(savedPos.x, savedPos.y, savedPos.z);
}

// FIXME: 92.3 %. The target keeps `this` in r30 and the flag in r31 (ours:
// r31 / r30), reloads mFlags before each `|=` (ours reuses the tested load),
// and tests the hit bits before vfunc_0x68 as a bool value (clrlwi; neg; or;
// srwi.) where ours uses clrlwi. A bool local and an inline isHit() gave
// the same code as the plain test.
void EnemyBase_804A4808::update() {
    if (!(mFlags & ENEMY_FLAG_STARTED)) {
        mFlags |= ENEMY_FLAG_STARTED;
        vfunc_0x54();
    }
    mFlags &= ~0x1F;
    mPlayersHit = 0;
    bool flag = true;
    if (mFlags & ENEMY_FLAG_NO_HURT) {
        flag = false;
    }
    mPrevVel.x = field_0x3c.x;
    mPrevVel.y = field_0x3c.y;
    mPrevVel.z = field_0x3c.z;
    if (mFlags & ENEMY_FLAG_KEEP_LIFE) {
        mLife = mLifeBackup;
    }
    vfunc_0x58();
    s32 prevD8 = field_0xd8;
    if (fn_801F3D94(flag)) {
        mFlags |= ENEMY_FLAG_HIT;
    } else if (vfunc_0x5C()) {
        mFlags |= ENEMY_FLAG_HIT_PLAYER;
        vfunc_0x6C();
    } else if (vfunc_0x60()) {
        mFlags |= ENEMY_FLAG_HIT_ACTOR;
        vfunc_0x70();
    }
    if (!flag && field_0xd8 > 0 && field_0xf8 == 2 && field_0x3c.z > 0.0f) {
        field_0x3c.z = 0.0f;
    }
    if (mFlags & ENEMY_FLAG_KEEP_LIFE) {
        mLife = mLifeBackup;
    }
    if (prevD8 == 1 && field_0xd8 == 0) {
        vfunc_0x74();
    }
    if (!(mFlags & ENEMY_FLAG_DEAD) && mLife <= 0) {
        mFlags |= ENEMY_FLAG_DEAD;
        vfunc_0x78();
    }
    vfunc_0x98();
    flag = vfunc_0x64();
    if (!(mFlags & 7) && !(mFlags & ENEMY_FLAG_NO_ATTACK)) {
        mPlayersHit = vfunc_0x68();
    }
    vfunc_0x9C(flag);
    if (!fn_801CD2F8(field_0x4)) {
        vfunc_0xA0();
    }
}

void EnemyBase_804A4808::vfunc_0xA0() {}

// FIXME: 88.1 %. The target picks the y with one fsel (top - y >= 0);
// ours compiles the ?: as fcmpo / cror / branch.
// The screen position of the actor, raised to the top of the view box.
void EnemyBase_804A4808::fn_801CE010(JGeometry::TVec3f* out) {
    s32 layer = field_0x4;
    GameView_804A9950* view = fn_802D6A9C();
    JGeometry::TBox2f* box = layer < 8 ? &view->mVisibleBox : &view->field_0xdc;
    out->set(mPos.x, mPos.y - mPos.z, 0.0f);
    f32 top = 1.0f + box->i.y;
    out->y = top - out->y >= 0.0f ? top : out->y;
}

inline void EnemyBase_804A4808::stateStep(u8 mapHit) {
    mStateTimer++;
    if (mStateTimer < 0) {
        mStateTimer = 0;
    }
    vfunc_0x90(mapHit);
    vfunc_0x94();
}

inline void EnemyBase_804A4808::moveStep() {
    if (mSpeed != mTargetSpeed) {
        f32 step = mSpeed > mTargetSpeed ? mDecel : mAccel;
        fn_801CD950(&mSpeed, mTargetSpeed, step);
    }
    vfunc_0x88();
    mPos += field_0x3c;
    vfunc_0x8C();
    if (mPos.z <= 0.0f) {
        if (viewLayerFlag(field_0x4)) {
            vfunc_0x80();
        } else if (mFlags & ENEMY_FLAG_BOUNCE) {
            mPos.z = 0.6f * -mPos.z;
            field_0x3c.z = 0.6f * -field_0x3c.z;
            mSpeed *= 0.6f;
            mTargetSpeed *= 0.6f;
            field_0x3c.x *= 0.6f;
            field_0x3c.y *= 0.6f;
            if (__fabsf(field_0x3c.z) < 0.1f) {
                mPos.z = 0.0f;
                field_0x3c.z = 0.0f;
            } else {
                vfunc_0x7C();
            }
        } else {
            mPos.z = 0.0f;
            field_0x3c.z = 0.0f;
        }
    }
}

void EnemyBase_804A4808::vfunc_0x9C(u8 mapHit) {
    if (!(mFlags & 7)) {
        stateStep(mapHit);
    }
}

// FIXME: 96.3 % (moveStep): the target loads mAccel (lfs 0x25C) before the
// mSpeed > mTargetSpeed branch and mDecel in it; ours branches first and
// loads the chosen one after. Literal order also off (see init).
void EnemyBase_804A4808::vfunc_0x98() {
    if (!(mFlags & 7)) {
        moveStep();
    }
}

void EnemyBase_804A4808::fn_801CE2BC(u8 mapHit) {
    stateStep(mapHit);
}

// FIXME: 96.2 %, the same moveStep difference as vfunc_0x98.
void EnemyBase_804A4808::fn_801CE32C() {
    moveStep();
}

void EnemyBase_804A4808::vfunc_0x94() {
    mAnim.fn_801D1C18();
}

void EnemyBase_804A4808::vfunc_0x90(u8 mapHit) {}

void EnemyBase_804A4808::vfunc_0x8C() {}

// The velocity along field_0x98 (0 +x, 1 -x, 2 +y, 3 -y) at mSpeed.
void EnemyBase_804A4808::vfunc_0x88() {
    switch (field_0x98) {
    case 0:
        field_0x3c.x = mSpeed;
        field_0x3c.y = 0.0f;
        break;
    case 1:
        field_0x3c.x = -mSpeed;
        field_0x3c.y = 0.0f;
        break;
    case 2:
        field_0x3c.x = 0.0f;
        field_0x3c.y = mSpeed;
        break;
    case 3:
        field_0x3c.x = 0.0f;
        field_0x3c.y = -mSpeed;
        break;
    }
    vfunc_0x84();
}

// Gravity.
void EnemyBase_804A4808::vfunc_0x84() {
    if (field_0xd8 > 0 && field_0xf8 == 2) {
        return;
    }
    field_0x3c.z += -0.2f;
    if (field_0x3c.z < -4.0f) {
        field_0x3c.z = -4.0f;
    }
}

// FIXME: 99.8 %, regalloc only (one register pair swapped).
// The knock-back step while a hit bit is set.
void EnemyBase_804A4808::fn_801CE5D4() {
    if (mFlags & 7) {
        mPos.z = mPrevPos.z;
        field_0x3c.z = mPrevVel.z;
        vfunc_0x84();
        mPos.z += field_0x3c.z;
        if (mPos.z <= 0.0f) {
            if (viewLayerFlag(field_0x4)) {
                vfunc_0x80();
            } else {
                mPos.z = 0.0f;
                field_0x3c.z = 0.0f;
            }
        }
    }
}

void EnemyBase_804A4808::vfunc_0x7C() {}

void EnemyBase_804A4808::vfunc_0x78() {}

void EnemyBase_804A4808::vfunc_0x74() {}

void EnemyBase_804A4808::vfunc_0x70() {}

void EnemyBase_804A4808::vfunc_0x6C() {}

u8 EnemyBase_804A4808::vfunc_0x68() {
    u8 hit = 0;
    if (mBox.getWidth() > 0.0f && mBox.getHeight() > 0.0f) {
        hit = fn_801F2B7C(field_0x114, -1, 1);
    }
    return hit;
}

bool EnemyBase_804A4808::vfunc_0x64() {
    bool hit = false;
    if (!(mFlags & ENEMY_FLAG_NO_MAP_HIT) && field_0x80.getWidth() > 0.0f &&
        field_0x80.getHeight() > 0.0f)
    {
        hit = fn_801F2618(&mMapHitDir, lbl_8053EFC0);
    }
    return hit;
}

bool EnemyBase_804A4808::vfunc_0x60() {
    bool hit = false;
    if (mBox.getWidth() > 0.0f && mBox.getHeight() > 0.0f) {
        hit = fn_801F2DB4('NULL', 'NULL', NULL);
    }
    return hit;
}

bool EnemyBase_804A4808::vfunc_0x5C() {
    bool hit = false;
    if (mBox.getWidth() > 0.0f && mBox.getHeight() > 0.0f) {
        hit = fn_801F2FAC();
    }
    return hit;
}

bool EnemyBase_804A4808::vfunc_0x58() {
    return false;
}

// A random direction, or another free one when that one is blocked.
s32 EnemyBase_804A4808::fn_801CF6B8() {
    s32 dir = fn_801CD664(4);
    if (fn_801CFCF0(dir)) {
        dir = fn_801CF720(dir);
    }
    return dir;
}

// A random direction other than `dir`, or another free one when blocked.
s32 EnemyBase_804A4808::fn_801CF720(s32 dir) {
    s32 newDir = fn_801CD580(dir);
    if (fn_801CFCF0(newDir)) {
        newDir = fn_801CF798(dir, newDir);
    }
    return newDir;
}

// FIXME: 99.8 %: only .sdata2 literal order (6.0f); see init.
// Whether a step of 6 units past the box edge in direction `dir` is blocked.
bool EnemyBase_804A4808::fn_801CFCF0(s32 dir) {
    f32 dist = 6.0f;
    switch (dir) {
    case 0:
        dist += field_0x80.f.x;
        break;
    case 1:
        dist -= field_0x80.i.x;
        break;
    case 2:
        dist += field_0x80.f.y;
        break;
    case 3:
        dist -= field_0x80.i.y;
        break;
    }
    return fn_801CFD68(dir, dist);
}

// FIXME: 73.7 %. The target loads mPos.x/y before the player position and
// the atanTable_ address after them; ours loads in the other order. Literal
// order also off (the angle constant, see init).
// The 16-bit angle from mPos to player `player`.
s32 EnemyBase_804A4808::fn_801D0280(s32 player) {
    JGeometry::TVec3f* playerPos = &field_0x1ac[player];
    return JMath::atanTable_.atan2_(playerPos->x - mPos.x, playerPos->y - mPos.y) * (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180());
}

// FIXME: 44.8 %. Same instructions: the target loads pos and mPos before
// materialising atanTable_; ours after. Literal order also off.
// The 16-bit angle from mPos to *pos.
s32 EnemyBase_804A4808::fn_801D02E0(const JGeometry::TVec3f* pos) {
    return JMath::atanTable_.atan2_(pos->x - mPos.x, pos->y - mPos.y) * (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180());
}

// Whether player `player` is within `dist` of *pos (x, y).
bool EnemyBase_804A4808::fn_801D0D50(s32 player, const JGeometry::TVec3f* pos, f32 dist) {
    JGeometry::TVec3f* playerPos = &field_0x1ac[player];
    f32 dy = playerPos->y - pos->y;
    f32 dx = playerPos->x - pos->x;
    bool result = false;
    if (square(dx) + square(dy) <= square(dist)) {
        result = true;
    }
    return result;
}

// The squared (x, y) distance from *pos to player `player`.
f32 EnemyBase_804A4808::fn_801D0DA0(s32 player, const JGeometry::TVec3f* pos) {
    JGeometry::TVec3f* playerPos = &field_0x1ac[player];
    f32 dy = playerPos->y - pos->y;
    f32 dx = playerPos->x - pos->x;
    return square(dx) + square(dy);
}

// FIXME: 83.9 %. The target loads field_0x4 before saving r30 and returns
// mPos.z <= 0 through branches (li 1 / li 0); ours uses mfcr / extrwi.
// Whether the actor is on the ground.
bool EnemyBase_804A4808::fn_801D0DD4() {
    if (viewLayerFlag(field_0x4)) {
        return (mFlags & (ENEMY_FLAG_LANDED | 0x10)) != 0;
    }
    if (mPos.z <= 0.0f) {
        return true;
    } else {
        return false;
    }
}

void EnemyBase_804A4808::vfunc_0x54() {}

// FIXME: 99.6 %: only .sdata2 literal order. The target's literals follow
// first use in address order (8.0f first in vfunc_0x80, 32.0f in
// fn_801CEA60, ...); they line up once the unwritten functions are in.
void EnemyBase_804A4808::init() {
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0xb0 = 0x11101;
    field_0x98 = fn_801CD664(4);
    mLifeBackup = mLife;
}

// FIXME: 99.9 %: only .sdata2 literal order (0.1f, 0.2f); see init.
EnemyBase_804A4808::EnemyBase_804A4808()
    : mFlags(0), mPrevVel(0.0f, 0.0f, 0.0f), mPlayersHit(0), mLifeBackup(8), mState(0),
      mStateTimer(0), field_0x250(0), mSpeed(0.0f), mTargetSpeed(0.0f), mAccel(0.1f),
      mDecel(0.2f), mMapHitDir(0.0f, 0.0f, 0.0f) {}
