// EnemyBase_804A4808 (game/EnemyBase.h): the enemy base class, its
// per-frame driver and the helpers its descendants call.
//
// Every function is written. The player searches (canTargetPlayer_801D0B44, findNearestPlayer_801D05F8,
// fn_801D0D50, getPlayerDistSq_801D0DA0, fn_801D0020, calcVelocityToward_801D01A0) are inlined into the
// functions before them, so their bodies are inline helpers (canTargetPlayer,
// nearestPlayer, ...) defined ahead of the first use; likewise the landing
// shared by checkFloor_0x80 and fn_801CE800 (landStep).

#include "game/GameAudio.h"
#include "game/EnemyBase.h"
#include <float.h>
#include "game/ActorId.h"
#include "game/Player.h"
#include "game/act/tu_801D2C60.h"
#include "game/ActorManager.h"
#include "game/Map.h"
#include "game/GameGlobals.h"
#include "game/GameMath.h"
#include "game/Vec2Ops.h"
#include "game/GameView.h"
#include "JSystem/JMath/JMath.h"
#include "JSystem/JAudio/JAUSoundObject.h"
#include "game/ViewLayer.h"

// Auto-inlining is off for the whole file (docs/INLINING.md, pragma rule 2): the
// original inlines no same-unit plain function. Checked: built with inlining off,
// every written function's calls to this unit's functions equal the original's
// `bl`s. The callers that need it: fn_801CF1B0, fn_801CF6B8, pickNewDir_801CF720 and fn_801CF798 call fn_801CF6B8,
// pickNewDir_801CF720, fn_801CF798 and isStepBlocked_801CFCF0 out of line.
#pragma auto_inline off

// The gravity step and the fall-speed limit the enemy and Shadow Link units read (declared in
// GameGlobals.h); with lbl_8053EFC0 they lead this unit's .sdata2.
const f32 lbl_8053EFB8 = -0.2f;
const f32 lbl_8053EFBC = 4.0f;
const bool lbl_8053EFC0 = false;

inline void EnemyBase_804A4808::stateStep(u8 mapHit) {
    mStateTimer++;
    if (mStateTimer < 0) {
        mStateTimer = 0;
    }
    updateState_0x90(mapHit);
    updateAnim_0x94();
}

inline void EnemyBase_804A4808::moveStep() {
    if (mSpeed != mTargetSpeed) {
        f32 step = mAccel;
        if (mSpeed > mTargetSpeed) {
            step = mDecel;
        }
        moveToward_801CD950(&mSpeed, mTargetSpeed, step);
    }
    calcVelocity_0x88();
    mPos += mVel_0x3c;
    onMoved_0x8C();
    if (mPos.z <= 0.0f) {
        if (viewLayerFlag(mLayer_0x4)) {
            checkFloor_0x80();
        } else if (mFlags & ENEMY_FLAG_BOUNCE) {
            mPos.z = 0.6f * -mPos.z;
            mVel_0x3c.z = 0.6f * -mVel_0x3c.z;
            mSpeed *= 0.6f;
            mTargetSpeed *= 0.6f;
            mVel_0x3c.x *= 0.6f;
            mVel_0x3c.y *= 0.6f;
            if (__fabsf(mVel_0x3c.z) < 0.1f) {
                mPos.z = 0.0f;
                mVel_0x3c.z = 0.0f;
            } else {
                onBounce_0x7C();
            }
        } else {
            mPos.z = 0.0f;
            mVel_0x3c.z = 0.0f;
        }
    }
}

// The player tests canTargetPlayer_801D0B44 makes, inlined in every player search below
// (fn_801CECF4, findNearestPlayer_801D05F8, fn_801D08B4). These actors only need the player
// on their own layer; the others ask isPlayerOnLayer_801F320C.
inline bool EnemyBase_804A4808::isPlayerOnLayer(s32 player) {
    bool onLayer = false;
    switch (mActorId) {
    case ACTOR_FALL:
    case ACTOR_FBGE:
    case ACTOR_FLOR:
    case ACTOR_HRMN:
    case ACTOR_KEPO:
    case ACTOR_NAVI:
    case ACTOR_SLK2:
    case ACTOR_TINK:
    case ACTOR_TZOK:
        // The layer through the inlined getter: MWCC reads it before the call
        // (lwz r27, 0x4 ahead of the bl, then cmpw r3, r27), as for Player's getLayer().
        if (getPlayerLayer_8023DE58(player) == getField_0x4()) {
            onLayer = true;
        }
        break;
    default:
        onLayer = isPlayerOnLayer_801F320C(player);
        break;
    }
    return onLayer;
}

// Whether player `player` is present (fn_801CCFE4) and on this actor's layer;
// for these actors also that fn_8022CAF8 is 4 or hasPlayerLeader_8022F514 is false.
inline bool EnemyBase_804A4808::canTargetPlayer(s32 player) {
    bool ok = false;
    if (fn_801CCFE4(player)) {
        bool onLayer = false;
        switch (mActorId) {
        case ACTOR_FALL:
        case ACTOR_FBGE:
        case ACTOR_FLOR:
        case ACTOR_HRMN:
        case ACTOR_KEPO:
        case ACTOR_NAVI:
        case ACTOR_SLK2:
        case ACTOR_TINK:
        case ACTOR_TZOK:
            if (getPlayerLayer_8023DE58(player) == getField_0x4()) {
                onLayer = true;
            }
            break;
        default:
            onLayer = isPlayerOnLayer_801F320C(player);
            break;
        }
        if (onLayer) {
            switch (mActorId) {
            case ACTOR_FALL:
            case ACTOR_FLOR:
            case ACTOR_HRMN:
            case ACTOR_KEPO:
            case ACTOR_NAVI:
            case ACTOR_TINK:
            case ACTOR_TNDR:
            case ACTOR_TZOK:
                if (fn_8022CAF8(player) == 4) {
                    ok = true;
                } else if (!hasPlayerLeader_8022F514(player)) {
                    ok = true;
                }
                break;
            default:
                ok = true;
                break;
            }
        }
    }
    return ok;
}

// getPlayerDistSq_801D0DA0's body: the squared (x, y) distance from *pos to the player.
inline f32 EnemyBase_804A4808::playerDistSq(s32 player, const JGeometry::TVec3f* pos) {
    JGeometry::TVec3f* playerPos = &mPlayerPos_0x1ac[player];
    f32 dy = playerPos->y - pos->y;
    f32 dx = playerPos->x - pos->x;
    return square(dx) + square(dy);
}

// fn_801D0D50's body: whether the player is within `dist` of *pos (x, y).
inline bool EnemyBase_804A4808::isPlayerNear(s32 player, const JGeometry::TVec3f* pos, f32 dist) {
    JGeometry::TVec3f* playerPos = &mPlayerPos_0x1ac[player];
    f32 dy = playerPos->y - pos->y;
    f32 dx = playerPos->x - pos->x;
    bool result = false;
    if (square(dx) + square(dy) <= square(dist)) {
        result = true;
    }
    return result;
}

// findNearestPlayer_801D05F8's body: the nearest player that can be targeted, within
// `range` of *pos (any distance when `range` is 0), or -1.
inline s32 EnemyBase_804A4808::nearestPlayer(const JGeometry::TVec3f* pos, f32 range) {
    f32 rangeSq = range * range;
    f32 best = FLT_MAX;
    s32 nearest = -1;
    for (s32 i = 0; i < 4; i++) {
        if (canTargetPlayer(i)) {
            f32 distSq = playerDistSq(i, pos);
            if (distSq < best && (range == 0.0f || distSq <= rangeSq)) {
                best = distSq;
                nearest = i;
            }
        }
    }
    return nearest;
}

// fn_801D0020's body: the direction from *pos to player `player` on the
// dominant axis (0 +x, 1 -x, 2 +y, 3 -y), or the other axis when `axis`.
// The |dx| <= |dy| test for `axis` is what the target branches on; the
// ||/&& shape keeps both clauses as the target has them.
inline s32 EnemyBase_804A4808::directionTo(const JGeometry::TVec3f* pos, s32 player, bool axis) {
    JGeometry::TVec3f* playerPos = &mPlayerPos_0x1ac[player];
    f32 dx = playerPos->x - pos->x;
    f32 absX = __fabsf(dx);
    f32 dy = playerPos->y - pos->y;
    f32 absY = __fabsf(dy);
    if ((absX > absY && !axis) || (absX <= absY && axis == true)) {
        if (pos->x < playerPos->x) {
            return 0;
        }
        return 1;
    }
    if (pos->y < playerPos->y) {
        return 2;
    }
    return 3;
}

// calcVelocityToward_801D01A0's body: the velocity from mPos toward *target (x, y), at most
// `speed` long: the normalised offset scaled by the shorter of `speed` and
// the offset's length.
inline void EnemyBase_804A4808::velocityToward(const JGeometry::TVec3f* target, JGeometry::TVec2f* out,
                                               f32 speed) {
    JGeometry::TVec2f dir(target->x - mPos.x, target->y - mPos.y);
    dir.normalize();
    f32 dx = target->x - mPos.x;
    f32 dy = target->y - mPos.y;
    *out = dir;
    f32 len = JMAFastSqrt(square(dx) + square(dy));
    if (len < speed) {
        speed = len;
    }
    out->x *= speed;
    out->y *= speed;
}

EnemyBase_804A4808::EnemyBase_804A4808()
    : mFlags(0), mPrevVel(0.0f, 0.0f, 0.0f), mPlayersHit(0), mLifeBackup(8), mState(0),
      mStateTimer(0), field_0x250(0), mSpeed(0.0f), mTargetSpeed(0.0f), mAccel(0.1f),
      mDecel(0.2f), mMapHitDir(0.0f, 0.0f, 0.0f) {}

void EnemyBase_804A4808::init() {
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    mMapBox_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0xb0 = 0x11101;
    mDir_0x98 = randInt_801CD664(4);
    mLifeBackup = mLife;
}

void EnemyBase_804A4808::onFirstUpdate_0x54() {}

// Snaps mPos (x, y) to the centre of its tile and makes it the previous and
// the spawn position too.
void EnemyBase_804A4808::fn_801D0E58() {
    JGeometry::TVec3f snapped;
    fn_801CCD34(&mPos, &snapped);
    mPos.x = snapped.x;
    mPos.y = snapped.y;
    mPrevPos.set(mPos);
    mSpawnPos.set(mPos);
}

// Whether the actor is on the ground.
bool EnemyBase_804A4808::isOnGround_801D0DD4() const {
    if (viewLayerFlag(mLayer_0x4)) {
        return (mFlags & (ENEMY_FLAG_LANDED | 0x10)) != 0;
    }
    bool onGround;
    if (mPos.z <= 0.0f) {
        onGround = true;
    } else {
        onGround = false;
    }
    return onGround;
}

// The squared (x, y) distance from *pos to player `player`.
f32 EnemyBase_804A4808::getPlayerDistSq_801D0DA0(s32 player, const JGeometry::TVec3f* pos) {
    return playerDistSq(player, pos);
}

// Whether player `player` is within `dist` of *pos (x, y).
bool EnemyBase_804A4808::fn_801D0D50(s32 player, const JGeometry::TVec3f* pos, f32 dist) {
    return isPlayerNear(player, pos, dist);
}

// Whether player `player` can be targeted: present, on this actor's layer and,
// for some actors, not busy.
bool EnemyBase_804A4808::canTargetPlayer_801D0B44(s32 player) {
    bool ok = false;
    if (fn_801CCFE4(player)) {
        bool onLayer = false;
        switch (mActorId) {
        case ACTOR_FALL:
        case ACTOR_FBGE:
        case ACTOR_FLOR:
        case ACTOR_HRMN:
        case ACTOR_KEPO:
        case ACTOR_NAVI:
        case ACTOR_SLK2:
        case ACTOR_TINK:
        case ACTOR_TZOK:
            if (getPlayerLayer_8023DE58(player) == getField_0x4()) {
                onLayer = true;
            }
            break;
        default:
            onLayer = isPlayerOnLayer_801F320C(player);
            break;
        }
        if (onLayer) {
            switch (mActorId) {
            case ACTOR_FALL:
            case ACTOR_FLOR:
            case ACTOR_HRMN:
            case ACTOR_KEPO:
            case ACTOR_NAVI:
            case ACTOR_TINK:
            case ACTOR_TNDR:
            case ACTOR_TZOK:
                if (fn_8022CAF8(player) == 4) {
                    ok = true;
                } else if (!hasPlayerLeader_8022F514(player)) {
                    ok = true;
                }
                break;
            default:
                ok = true;
                break;
            }
        }
    }
    return ok;
}

bool EnemyBase_804A4808::fn_801D08B4(s32 player, const JGeometry::TVec3f* pos, f32 range) {
    bool result = false;
    if (player >= 0 && player < 4 && canTargetPlayer(player) && isPlayerNear(player, pos, range)) {
        result = true;
    }
    return result;
}

// FIXME: 99.87 %, scheduling; behaviour clean.
// registers and the order of the first loads in the prologue.
// The nearest player that can be targeted within `range` of *pos (any
// distance when `range` is 0), or -1.
s32 EnemyBase_804A4808::findNearestPlayer_801D05F8(const JGeometry::TVec3f* pos, f32 range) {
    f32 rangeSq = range * range;
    f32 best = FLT_MAX;
    s32 nearest = -1;
    for (s32 i = 0; i < 4; i++) {
        if (canTargetPlayer(i)) {
            f32 distSq = playerDistSq(i, pos);
            if (distSq < best && (range == 0.0f || distSq <= rangeSq)) {
                best = distSq;
                nearest = i;
            }
        }
    }
    return nearest;
}

// The player that can be targeted whose direction from *from is closest to
// `angle` (16-bit angles; the difference as a fraction of a half turn), or
// -1.
s32 EnemyBase_804A4808::fn_801D0338(const JGeometry::TVec3f* from, s16 angle) {
    f32 best = 2.0f;
    s32 nearest = -1;
    for (s32 i = 0; i < 4; i++) {
        if (canTargetPlayer(i)) {
            JGeometry::TVec3f* playerPos = &mPlayerPos_0x1ac[i];
            s32 playerAngle = JMath::atanTable_.atan2_(playerPos->x - from->x, playerPos->y - from->y) *
                              (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180());
            f32 diff = (1.0f / 32768.0f) * JMAAbs((s16)(angle - playerAngle));
            if (diff < best) {
                best = diff;
                nearest = i;
            }
        }
    }
    return nearest;
}

// The 16-bit angle from mPos to *pos.
s32 EnemyBase_804A4808::fn_801D02E0(const JGeometry::TVec3f* pos) const {
    return JMath::atanTable_.atan2_(pos->x - mPos.x, pos->y - mPos.y) * (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180());
}

// The 16-bit angle from mPos to player `player`.
s32 EnemyBase_804A4808::getAngleToPlayer_801D0280(s32 player) const {
    const JGeometry::TVec3f* playerPos = &mPlayerPos_0x1ac[player];
    return JMath::atanTable_.atan2_(playerPos->x - mPos.x, playerPos->y - mPos.y) * (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180());
}

// FIXME: 99.61 %, scheduling; behaviour clean.
// Same instructions; the target loads target->y before mPos.y for the second
// offset (ours the other way round).
// The velocity toward *target (x, y), at most `speed` long.
void EnemyBase_804A4808::calcVelocityToward_801D01A0(const JGeometry::TVec3f* target, JGeometry::TVec2f* out, f32 speed) {
    velocityToward(target, out, speed);
}

// FIXME: 92.63 %, scheduling+regalloc; behaviour clean.
// and registers as in calcVelocityToward_801D01A0 (the same body).
// The velocity (x, y) toward player `player`, at most `speed` long.
void EnemyBase_804A4808::fn_801D00B4(s32 player, JGeometry::TVec2f* out, f32 speed) {
    JGeometry::TVec3f* playerPos = &mPlayerPos_0x1ac[player];
    velocityToward(playerPos, out, speed);
}

// The direction from *pos to player `player` on the dominant axis, or the
// other axis when `axis`.
s32 EnemyBase_804A4808::fn_801D0020(const JGeometry::TVec3f* pos, s32 player, bool axis) {
    return directionTo(pos, player, axis);
}

bool EnemyBase_804A4808::fn_801CFD68(s32 dir, f32 dist) {
    f32 half = 0.5f * dist;
    JGeometry::TVec3f pos = mPos;
    switch (dir) {
    case 0:
        pos.x += half;
        break;
    case 1:
        pos.x -= half;
        break;
    case 2:
        pos.y += half;
        break;
    case 3:
        pos.y -= half;
        break;
    }
    JGeometry::TBox2f box(mMapBox_0x80);
    switch (dir) {
    case 0:
    case 1:
        box.f.x = half;
        box.i.x = -half;
        break;
    case 2:
    case 3:
        box.i.y = -half;
        box.f.y = half;
        break;
    }
    f32 screenY = pos.y - mPos.z;
    box.i.x += pos.x;
    box.f.x += pos.x;
    box.i.y += screenY;
    box.f.y += screenY;
    s32 rows;
    s32 columns;
    s32 row;
    s32 column;
    s32 layer = mLayer_0x4;
    GameView_804A9950* view = getGameView_802D6A9C();
    JGeometry::TBox2f* viewBox = layer < 8 ? &view->mVisibleBox : &view->field_0xdc;
    bool inside = false;
    if (box.i.x >= viewBox->i.x && box.i.y >= viewBox->i.y && box.f.x < viewBox->f.x &&
        box.f.y < viewBox->f.y)
    {
        inside = true;
    }
    if (!inside) {
        return true;
    }
    f32 width = box.getWidth();
    f32 height;
    if (width < 0.0f || (height = box.getHeight()) < 0.0f) {
        return true;
    }
    JGeometry::TVec3f point(box.i.x, box.i.y, 0.0f);
    columns = (s32)(0.125f * width) + 2;
    rows = (s32)(0.125f * height) + 2;
    for (row = 0; row < rows; row++) {
        point.x = box.i.x;
        if (row == rows - 1) {
            point.y = box.f.y;
        }
        for (column = 0; column < columns; column++) {
            if (column == columns - 1) {
                point.x = box.f.x;
            }
            if (isBlocked_0x4C(layer, &point)) {
                return true;
            }
            point.x += 8.0f;
        }
        point.y += 8.0f;
    }
    return false;
}

// Whether a step of 6 units past the box edge in direction `dir` is blocked.
bool EnemyBase_804A4808::isStepBlocked_801CFCF0(s32 dir) {
    f32 dist = 6.0f;
    switch (dir) {
    case 0:
        dist += mMapBox_0x80.f.x;
        break;
    case 1:
        dist -= mMapBox_0x80.i.x;
        break;
    case 2:
        dist += mMapBox_0x80.f.y;
        break;
    case 3:
        dist -= mMapBox_0x80.i.y;
        break;
    }
    return fn_801CFD68(dir, dist);
}

// FIXME: 98.53 %, missing/extra; behaviour clean.
// The target copies `layer` into a second register (mr r31,
// r28) after getGameView_802D6A9C and uses the copy for isBlocked_0x4C; the
// rest is registers.
// Whether the way from *from to *to is blocked: either point outside the
// view's box for `layer` (0xCC, or 0xDC for the upper layers), or the map
// (isBlocked_0x4C) blocking a point every 8 units along the line or the end.
bool EnemyBase_804A4808::isPathBlocked_801CFA70(const JGeometry::TVec3f* from, const JGeometry::TVec3f* to, s32 layer) {
    JGeometry::TVec3f pos;
    JGeometry::TBox2f* box = layerBox(layer);
    if (from->x < box->i.x || from->y < box->i.y || from->x >= box->f.x || from->y >= box->f.y ||
        to->x < box->i.x || to->y < box->i.y || to->x >= box->f.x || to->y >= box->f.y)
    {
        return true;
    }
    JGeometry::TVec2f dir(to->x - from->x, to->y - from->y);
    f32 len = JMAFastSqrt(dir.squared());
    if (len <= 0.0f) {
        JGeometry::TVec3f point(from->x, from->y, 0.0f);
        return isBlocked_0x4C(layer, &point);
    }
    bool blocked = false;
    dir.normalize();
    dir.scale(8.0f);
    s32 steps = (s32)(0.125f * len) + 1;
    pos.set(from->x, from->y, 0.0f);
    for (s32 i = 0; i < steps; i++) {
        if (isBlocked_0x4C(layer, &pos)) {
            blocked = true;
            break;
        }
        pos.x += dir.x;
        pos.y += dir.y;
    }
    if (!blocked) {
        JGeometry::TVec3f end(to->x, to->y, 0.0f);
        blocked = isBlocked_0x4C(layer, &end);
    }
    return blocked;
}

// Whether the way from *from to *to is blocked (isPathBlocked_801CFA70): along the line
// between them, then along the two lines 7 units to either side of it.
bool EnemyBase_804A4808::isWayBlocked_801CF888(const JGeometry::TVec2f* from, const JGeometry::TVec3f* to, s32 layer) {
    // isPathBlocked_801CFA70 reads only (x, y) of both points; the side points are
    // 8-byte TVec2f slots, as `from` is.
    bool blocked = isPathBlocked_801CFA70((const JGeometry::TVec3f*)from, to, layer);
    if (!blocked) {
        JGeometry::TVec2f dir(to->x - from->x, to->y - from->y);
        bool isZero = dir.isZero();
        if (!isZero) {
            dir.normalize();
            // dir turned by -90 degrees, 7 units long.
            JGeometry::TVec2f side(
                dir.x * JMASCos(-0x4000) - dir.y * JMASSin(-0x4000),
                dir.x * JMASSin(-0x4000) + dir.y * JMASCos(-0x4000));
            side.scale(7.0f);
            JGeometry::TVec2f a(*from);
            JGeometry::TVec2f b(xy(*to));
            a.add(side);
            b.add(side);
            blocked = isPathBlocked_801CFA70((const JGeometry::TVec3f*)&a, (const JGeometry::TVec3f*)&b, layer);
            if (!blocked) {
                side.x = -side.x;
                side.y = -side.y;
                a.set(*from);
                b.set(xy(*to));
                a.add(side);
                b.add(side);
                blocked = isPathBlocked_801CFA70((const JGeometry::TVec3f*)&a, (const JGeometry::TVec3f*)&b, layer);
            }
        }
    }
    return blocked;
}

// One of the two directions that are neither `dir` nor `except`, picked at
// random; the other one when that is blocked.
s32 EnemyBase_804A4808::fn_801CF798(s32 dir, s32 except) {
    s32 dirs[2];
    s32* next = dirs;
    for (s32 d = 0; d < 4; d++) {
        if (d != dir && d != except) {
            *next++ = d;
        }
    }
    s32 i = randInt_801CD664(2);
    return !isStepBlocked_801CFCF0(dirs[i]) ? dirs[i] : dirs[1 - i];
}

// A random direction other than `dir`, or another free one when blocked.
s32 EnemyBase_804A4808::pickNewDir_801CF720(s32 dir) {
    s32 newDir = fn_801CD580(dir);
    if (isStepBlocked_801CFCF0(newDir)) {
        newDir = fn_801CF798(dir, newDir);
    }
    return newDir;
}

// A random direction, or another free one when that one is blocked.
s32 EnemyBase_804A4808::fn_801CF6B8() {
    s32 dir = randInt_801CD664(4);
    if (isStepBlocked_801CFCF0(dir)) {
        dir = pickNewDir_801CF720(dir);
    }
    return dir;
}

// Float registers of the second directionTo differ.
// A direction toward the nearest player within `range` (fn_801CF6B8's random
// one when there is none): on the dominant axis, or the other one with
// probability `margin`. With `turn`, a direction equal to mDir_0x98 is
// replaced by another free one (pickNewDir_801CF720), and by a third one
// (fn_801CF798) when that would turn away from the player; without it, a
// blocked direction (isStepBlocked_801CFCF0) is swapped for the other axis.
s32 EnemyBase_804A4808::fn_801CF1B0(bool turn, f32 range, f32 margin) {
    s32 dir;
    s32 player = nearestPlayer(&mPos, range);
    if (player < 0) {
        dir = fn_801CF6B8();
    } else {
        bool otherAxis = false;
        if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < margin) {
            otherAxis = true;
        }
        dir = directionTo(&mPos, player, otherAxis);
        if (turn) {
            if (dir == mDir_0x98) {
                dir = pickNewDir_801CF720(mDir_0x98);
                if (dir == oppositeDir_801CD354(directionTo(&mPos, player, false))) {
                    dir = fn_801CF798(dir, mDir_0x98);
                }
            }
        } else if (isStepBlocked_801CFCF0(dir)) {
            dir = directionTo(&mPos, player, !otherAxis);
        }
    }
    return dir;
}

// Whether the map (fn_801CCB1C) blocks the point 16 units above the actor's
// screen position.
bool EnemyBase_804A4808::fn_801CF144() const {
    return isMapBlocked_801CCB1C(JGeometry::TVec3f(mPos.x, mPos.y - mPos.z - 16.0f, 0.0f), mLayer_0x4);
}

// const: the target loads mVel_0x3c.x above the prologue's stw r0.
// The direction of mVel_0x3c (its 16-bit angle through angleToDir_801F681C).
s32 EnemyBase_804A4808::fn_801CF0F4() const {
    s32 angle = JMath::atanTable_.atan2_(mVel_0x3c.x, mVel_0x3c.y) *
                (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180());
    return angleToDir_801F681C(angle);
}

// regalloc only (the two bools of canTargetPlayer swap r28 /
// r29, and the player-pointer induction register).
// Spawns `actorId` (a cls_804A48B0 shot) at mPos aimed at the nearest player
// within `range`, moving at `speed` (straight down +y when the player is on
// top of us). Returns whether it was spawned.
bool EnemyBase_804A4808::fn_801CECF4(u32 actorId, f32 speed, f32 range) {
    s32 serial;
    s32 player = nearestPlayer(&mPos, range);
    if (player < 0) {
        return false;
    }
    bool spawned = false;
    serial = getActorManager_801F856C()->spawnActor_801F9484(actorId, mLayer_0x4, &mPos, 0, -1, -1);
    if (serial >= 0) {
        cls_804A48B0* shot =
            static_cast<cls_804A48B0*>(getActorManager_801F856C()->findActorBySerial_801F666C(serial));
        if (shot != NULL) {
            JGeometry::TVec3f* playerPos = &mPlayerPos_0x1ac[player];
            JGeometry::TVec2f vel(playerPos->x - mPos.x, playerPos->y - mPos.y);
            if (vel.squared() <= JGeometry::TUtilf::epsilon()) {
                vel.set(0.0f, speed);
            } else {
                vel.normalize();
                vel.scale(speed);
            }
            shot->fn_801D2CF0(&vel);
            spawned = true;
        }
    }
    return spawned;
}

bool EnemyBase_804A4808::vfunc_0x58() {
    return false;
}

// For each player whose box overlaps mBox (findOverlappingPlayer_801F285C) and that startTimer_801EAC20
// accepts, a hit at the player's point goes to fn_801CB538 on this layer; one
// hit plays sound 0x400D1. Returns whether any player was hit.
u8 EnemyBase_804A4808::fn_801CEBD0() {
    u8 hit = 0;
    JGeometry::TVec3f hitPos;
    if (mBox.getWidth() > 0.0f && mBox.getHeight() > 0.0f) {
        for (s32 i = 0; i < 4; i++) {
            JGeometry::TVec2f xy;
            s32 flag;
            if (findOverlappingPlayer_801F285C(&xy, &flag, false, i) == i) {
                fn_8023B6CC(i);
                fn_802379E4(i, &mPos);
                if (flag == 1 && field_0x1dc.startTimer_801EAC20(i)) {
                    hitPos.set(xy.x, xy.y, 0.0f);
                    fn_801CB538(mLayer_0x4, &hitPos, 0, 0);
                    hit = 1;
                }
            }
        }
        if (hit) {
            startSound_801F0E34(0x400D1, 0);
        }
    }
    return hit;
}

u8 EnemyBase_804A4808::checkHitByPlayer_0x5C() {
    u8 hit = 0;
    if (mBox.getWidth() > 0.0f && mBox.getHeight() > 0.0f) {
        hit = checkPlayerAttacks_801F2FAC();
    }
    return hit;
}

// Hit by the actor overlapping this one (findOverlappingActor_801F82CC, within 32 in z), unless
// it is a live Bombchu (fn_801CC8E4): takeHit_801F37AC with its attack, its
// position relative to ours, its field_0x168 and field_0x110. The attacker's
// field_0x164 is set when the hit landed. Returns whether it did.
bool EnemyBase_804A4808::fn_801CEA60() {
    bool hit = false;
    if (mBox.getWidth() > 0.0f && mBox.getHeight() > 0.0f) {
        s32 serial = getActorManager_801F856C()->findOverlappingActor_801F82CC(this, field_0x195, 32.0f);
        if (serial >= 0 && serial != mSerial) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (actor != NULL) {
                if (fn_801CC8E4(actor)) {
                    hit = false;
                } else {
                    JGeometry::TVec3f from = mPos;
                    from -= actor->mVel_0x3c;
                    hit = takeHit_801F37AC(actor->mParamAtcn_0x118, &from, 0, actor->field_0x168,
                                      actor->field_0x110);
                }
            }
            if (hit) {
                actor->field_0x164 = 1;
            }
        }
    }
    return hit;
}

u8 EnemyBase_804A4808::checkHitByActor_0x60() {
    u8 hit = 0;
    if (mBox.getWidth() > 0.0f && mBox.getHeight() > 0.0f) {
        hit = checkActorHit_801F2DB4('NULL', 'NULL', NULL);
    }
    return hit;
}

u8 EnemyBase_804A4808::sweepMap_0x64() {
    u8 hit = 0;
    if (!(mFlags & ENEMY_FLAG_NO_MAP_HIT) && mMapBox_0x80.getWidth() > 0.0f &&
        mMapBox_0x80.getHeight() > 0.0f)
    {
        hit = sweepMapBox_801F2618(&mMapHitDir, lbl_8053EFC0);
    }
    return hit;
}

u8 EnemyBase_804A4808::attackPlayers_0x68() {
    u8 hit = 0;
    if (mBox.getWidth() > 0.0f && mBox.getHeight() > 0.0f) {
        hit = damagePlayersInBox_801F2B7C(mParamAtcp_0x114, -1, 1);
    }
    return hit;
}

void EnemyBase_804A4808::onHitByPlayer_0x6C() {}

void EnemyBase_804A4808::onHitByActor_0x70() {}

void EnemyBase_804A4808::onHitReactionEnd_0x74() {}

void EnemyBase_804A4808::onDeath_0x78() {}

void EnemyBase_804A4808::onBounce_0x7C() {}

#pragma auto_inline reset
void EnemyBase_804A4808::fn_801CE800() {
    if (mMapBox_0x80.getWidth() > 0.0f && mMapBox_0x80.getHeight() > 0.0f) {
        JGeometry::TVec2f pos;
        JGeometry::TVec2f snapped;
        f32 bottom = mMapBox_0x80.f.y;
        f32 screenY = mPos.y - mPos.z;
        bool onFloor = false;
        pos.x = mPos.x;
        pos.y = bottom + screenY;
        switch (getTileCollisionCodeAlt_802265FC((const JGeometry::TVec3f*)&pos, mLayer_0x4)) {
        case 0:
            if (mPos.z <= 0.0f) {
                onFloor = true;
            }
            break;
        case 2:
            break;
        case 1:
        default:
            onFloor = true;
            break;
        }
        if (onFloor) {
            fn_801CCD34((const JGeometry::TVec3f*)&pos, (JGeometry::TVec3f*)&snapped);
            mPos.y = snapped.y - 8.0f - mMapBox_0x80.f.y;
            mPos.z = 0.0f;
            mVel_0x3c.z = 0.0f;
            mFlags |= ENEMY_FLAG_LANDED;
        }
    }
}

// The floor check after a step reached z <= 0 on a floor layer: on a tile of
// kind 2 the height is folded into y and the actor lands (fn_801CE800);
// elsewhere it just stops at z = 0.
void EnemyBase_804A4808::checkFloor_0x80() {
    if (!(mFlags & ENEMY_FLAG_NO_FLOOR) && getTileCollisionCode_8022461C(&mPos, mLayer_0x4) == 2) {
        // The lift is read into a local first, as in the original (without it 99.9 -> 98 %).
        f32 lift = -mPos.z;
        mPos.y += lift;
        mPos.z = 0.0f;
        fn_801CE800();
    } else {
        mPos.z = 0.0f;
        mVel_0x3c.z = 0.0f;
        mFlags |= ENEMY_FLAG_LANDED;
    }
}
#pragma auto_inline off

// The knock-back step while a hit bit is set.
void EnemyBase_804A4808::fn_801CE5D4() {
    if (mFlags & 7) {
        mPos.z = mPrevPos.z;
        mVel_0x3c.z = mPrevVel.z;
        applyGravity_0x84();
        mPos.z += mVel_0x3c.z;
        if (mPos.z <= 0.0f) {
            if (viewLayerFlag(mLayer_0x4)) {
                checkFloor_0x80();
            } else {
                mPos.z = 0.0f;
                mVel_0x3c.z = 0.0f;
            }
        }
    }
}

// Gravity.
void EnemyBase_804A4808::applyGravity_0x84() {
    if (mHitTimer_0xd8 > 0 && mKnockbackKind_0xf8 == 2) {
        return;
    }
    mVel_0x3c.z += -0.2f;
    if (mVel_0x3c.z < -4.0f) {
        mVel_0x3c.z = -4.0f;
    }
}

// The velocity along mDir_0x98 (0 +x, 1 -x, 2 +y, 3 -y) at mSpeed.
void EnemyBase_804A4808::calcVelocity_0x88() {
    switch (mDir_0x98) {
    case 0:
        mVel_0x3c.x = mSpeed;
        mVel_0x3c.y = 0.0f;
        break;
    case 1:
        mVel_0x3c.x = -mSpeed;
        mVel_0x3c.y = 0.0f;
        break;
    case 2:
        mVel_0x3c.x = 0.0f;
        mVel_0x3c.y = mSpeed;
        break;
    case 3:
        mVel_0x3c.x = 0.0f;
        mVel_0x3c.y = -mSpeed;
        break;
    }
    applyGravity_0x84();
}

void EnemyBase_804A4808::onMoved_0x8C() {}

void EnemyBase_804A4808::updateState_0x90(u8 mapHit) {}

void EnemyBase_804A4808::updateAnim_0x94() {
    mAnim.update_801D1C18();
}

void EnemyBase_804A4808::fn_801CE32C() {
    moveStep();
}

void EnemyBase_804A4808::fn_801CE2BC(u8 mapHit) {
    stateStep(mapHit);
}

void EnemyBase_804A4808::move_0x98() {
    if (!(mFlags & 7)) {
        moveStep();
    }
}

void EnemyBase_804A4808::updateBehavior_0x9C(u8 mapHit) {
    if (!(mFlags & 7)) {
        stateStep(mapHit);
    }
}

// FIXME: 97.30 %, missing/extra; behaviour near.
// Ours adds one frsp after the fsel.
// The screen position of the actor, raised to the top of the view box.
void EnemyBase_804A4808::fn_801CE010(JGeometry::TVec3f* out) {
    s32 layer = mLayer_0x4;
    GameView_804A9950* view = getGameView_802D6A9C();
    JGeometry::TBox2f* box = layer < 8 ? &view->mVisibleBox : &view->field_0xdc;
    out->set(mPos.x, mPos.y - mPos.z, 0.0f);
    f32 top = 1.0f + box->i.y;
    out->y = __fsel(top - out->y, top, out->y);
}

void EnemyBase_804A4808::vfunc_0xA0() {}

// The target passes sweepMap_0x64's result to updateBehavior_0x9C with clrlwi 24 where
// ours has mr.
// FIXME: 99.65 %, different; behaviour near.
void EnemyBase_804A4808::update() {
    u8 mapHit;
    s32 prevD8;
    if (!checkFlag(ENEMY_FLAG_STARTED)) {
        mFlags |= ENEMY_FLAG_STARTED;
        onFirstUpdate_0x54();
    }
    mFlags &= ~0x1F;
    mPlayersHit = 0;
    bool flag = true;
    if (mFlags & ENEMY_FLAG_NO_HURT) {
        flag = false;
    }
    mPrevVel.x = mVel_0x3c.x;
    mPrevVel.y = mVel_0x3c.y;
    mPrevVel.z = mVel_0x3c.z;
    if (mFlags & ENEMY_FLAG_KEEP_LIFE) {
        mLife = mLifeBackup;
    }
    vfunc_0x58();
    prevD8 = mHitTimer_0xd8;
    if (updateHitReaction_801F3D94(flag)) {
        mFlags |= ENEMY_FLAG_HIT;
    } else if (checkHitByPlayer_0x5C()) {
        mFlags |= ENEMY_FLAG_HIT_PLAYER;
        onHitByPlayer_0x6C();
    } else if (checkHitByActor_0x60()) {
        mFlags |= ENEMY_FLAG_HIT_ACTOR;
        onHitByActor_0x70();
    }
    if (!flag && mHitTimer_0xd8 > 0 && mKnockbackKind_0xf8 == 2 && mVel_0x3c.z > 0.0f) {
        mVel_0x3c.z = 0.0f;
    }
    if (mFlags & ENEMY_FLAG_KEEP_LIFE) {
        mLife = mLifeBackup;
    }
    if (prevD8 == 1 && mHitTimer_0xd8 == 0) {
        onHitReactionEnd_0x74();
    }
    if (!checkFlag(ENEMY_FLAG_DEAD) && mLife <= 0) {
        mFlags |= ENEMY_FLAG_DEAD;
        onDeath_0x78();
    }
    move_0x98();
    mapHit = sweepMap_0x64();
    if (!isHit() && !(mFlags & ENEMY_FLAG_NO_ATTACK)) {
        mPlayersHit = attackPlayers_0x68();
    }
    updateBehavior_0x9C(mapHit);
    if (!isUpperLayer_801CD2F8(mLayer_0x4)) {
        vfunc_0xA0();
    }
}

// Starts sound `soundId` as if this actor were at *pos on `layer`.
void EnemyBase_804A4808::fn_801CDCD0(s32 layer, const JGeometry::TVec3f* pos, u32 soundId) {
    s32 savedLayer = mLayer_0x4;
    JGeometry::TVec3f savedPos = mPos;
    mLayer_0x4 = layer;
    mPos.set(*pos);
    startSound_801F0E34(soundId, 0);
    mLayer_0x4 = savedLayer;
    mPos.set(savedPos.x, savedPos.y, savedPos.z);
}

// Stops sound `soundId` on this actor's sound object.
void EnemyBase_804A4808::fn_801CDC8C(u32 soundId) {
    // ActorSoundObj_804578F4 is a JAUSoundObject (game/ActorManager.h) but
    // opaque until game/actor_sound_obj_80455CA4.cpp is written.
    JAUSoundObject* sound = (JAUSoundObject*)fn_801F1184();
    if (sound != NULL) {
        sound->stopSound(soundId, 0);
    }
}
