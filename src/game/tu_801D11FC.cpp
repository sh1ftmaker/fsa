#include "game/GameAudio.h"
#include "game/GameMath.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/cls_804A46F4.h"
#include "JSystem/JMath/JMath.h"
#include <math.h>

// The distance between two angles as a fraction of a half turn.
static inline f32 angleDist(s16 a, s16 b) {
    s16 diff = a - b;
    return (1.0f / 32768.0f) * JMAAbs(diff);
}

// The waypoint `i` of mPath, or the first when it is past the end.
static inline void waypoint(const cls_804A46F4& self, s32 i, JGeometry::TVec2f& out) {
    if (self.mPath.mCount <= i) {
        i = 0;
    }
    out = self.mPath.mPoints[i];
}

// The x of waypoint `i`, or of the first when it is past the end.
static inline f32 waypointX(const cls_804A46F4& self, s32 i) {
    if (self.mPath.mCount <= i) {
        i = 0;
    }
    return self.mPath.mPoints[i].x;
}

// The y of waypoint `i`, or of the first when it is past the end.
static inline f32 waypointY(const cls_804A46F4& self, s32 i) {
    if (self.mPath.mCount <= i) {
        i = 0;
    }
    return self.mPath.mPoints[i].y;
}

// 0x801D1974.
cls_804A46F4::cls_804A46F4() {
    field_0x32c.set(0.0f, 0.0f, 0.0f);
    field_0x338 = -1;
    field_0x33c = 0;
}

// 0x801D183C.
// FIXME: 99.24 %, scheduling+regalloc; behaviour clean.
// the original keeps the result in r30
// and the player index in r31; ours has them the other way round.
bool cls_804A46F4::fn_801D183C(f32 range) {
    bool result = false;
    if (field_0x338 >= 0 && field_0x338 < 4 && isPlayerOnLayer_801F320C(field_0x338)) {
        s32 player = field_0x338;
        JGeometry::TVec3f pos;
        JGeometry::TVec3f other;
        JGeometry::TVec3f playerPos;
        fn_8022E114(player, &playerPos);
        if (isPlayerOnLowerLayer_801F329C(player)) {
            fn_801CD150(&playerPos, &other);
            pos.set(other.x, other.y, other.z);
        } else {
            pos.set(playerPos.x, playerPos.y, playerPos.z);
        }
        if (range == 0.0f || square(pos.x - mPos.x) + square(pos.y - mPos.y) <= square(range)) {
            // isWayBlocked_801CF888 reads only (x, y) of its first position (8-byte slots).
        if (!isWayBlocked_801CF888((const JGeometry::TVec2f*)&mPos, &pos, mLayer_0x4)) {
                result = true;
            }
        }
    }
    return result;
}

// 0x801D12CC.
// dx/dy of the arrival check sit in swapped float registers (f3/f4).
bool cls_804A46F4::fn_801D12CC() {
    f32 dy;
    bool result = false;
    if (field_0x338 >= 0 && canTargetPlayer_801D0B44(field_0x338)) {
        s32 player = field_0x338;
        JGeometry::TVec3f pos;
        JGeometry::TVec3f probe;
        JGeometry::TVec3f other;
        JGeometry::TVec3f playerPos;
        fn_8022E114(player, &playerPos);
        if (isPlayerOnLowerLayer_801F329C(player)) {
            fn_801CD150(&playerPos, &other);
            pos.set(other.x, other.y, other.z);
        } else {
            pos.set(playerPos.x, playerPos.y, playerPos.z);
        }
        probe.set(pos.x, pos.y, 0.0f);
        if (isBlocked_0x4C(mLayer_0x4, &probe)) {
            s32 code = getTileCollisionCodeAlt_802265FC(&pos, mLayer_0x4);
            if (code >= 0x2C || code < 0x27) {
                mPath.mCount = 0;
                return false;
            }
        }
        result = true;
        // isWayBlocked_801CF888 reads only (x, y) of its first position (8-byte slots).
        if (!isWayBlocked_801CF888((const JGeometry::TVec2f*)&mPos, &pos, mLayer_0x4)) {
            mPath.mCount = 0;
            field_0x32c.set(pos.x, pos.y, 0.0f);
        } else {
            if (mPath.mCount >= 1) {
                f32 dx = field_0x32c.x - mPos.x;
                dy = field_0x32c.y - mPos.y;
                if (square(dx) + square(dy) <= square(0.001f + mTargetSpeed)) {
                    mPath.popFront();
                }
            }
            bool far = false;
            bool clear = false;
            if (mPath.mCount == 0) {
                clear = true;
            } else {
                JGeometry::TVec2f from;
                waypoint(*this, mPath.mCount - 1, from);
                if (isWayBlocked_801CF888(&from, &pos, mLayer_0x4)) {
                    clear = true;
                } else {
                    f32 ey = pos.y - from.y;
                    f32 ex = pos.x - from.x;
                    if (ex * ex + ey * ey >= 64.0f) {
                        far = true;
                        if (mPath.mCount >= 2) {
                            JGeometry::TVec2f prev;
                            waypoint(*this, mPath.mCount - 2, prev);
                            if (!isWayBlocked_801CF888(&prev, &pos, mLayer_0x4)) {
                                if (mPath.mCount > 0) {
                                    mPath.mCount--;
                                }
                            }
                        }
                    }
                }
            }
            if (far || clear) {
                s32 count = mPath.mCount;
                if (count >= 16) {
                    mPath.mCount = 0;
                    return false;
                }
                if (clear && mStateTimer > 2) {
                    f32 offsetY = getPlayerPos_8023E724(field_0x338)->y - fn_8023E578(field_0x338)->y;
                    f32 y = pos.y - offsetY;
                    f32 offsetX = getPlayerPos_8023E724(field_0x338)->x - fn_8023E578(field_0x338)->x;
                    f32 x = pos.x - offsetX;
                    mPath.push(x, y);
                } else {
                    mPath.push(pos.x, pos.y);
                }
            }
            field_0x32c.set(waypointX(*this, 0), waypointY(*this, 0), 0.0f);
        }
    }
    if (!result) {
        mPath.mCount = 0;
    }
    return result;
}

// 0x801D11FC: turns field_0x33c by 0x5B0 whichever way is nearer the angle
// of mMapHitDir.
// regalloc only: the original keeps field_0x33c in r5 and
// the left angle in r5 (lis r3 0x4330 for the int-to-float constant, f0 for
// the 1/32768 scale); ours uses r3/r4/r5 and r0 for the constant.
void cls_804A46F4::fn_801D11FC() {
    s16 target = JMath::atanTable_.atan2_(mMapHitDir.x, mMapHitDir.y) * (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180());
    s16 left = field_0x33c + 0x5B0;
    const s16 right = field_0x33c - 0x5B0;
    f32 distLeft = angleDist(target, left);
    f32 distRight = angleDist(target, right);
    if (distLeft < distRight) {
        field_0x33c = left;
    } else {
        field_0x33c = right;
    }
}
