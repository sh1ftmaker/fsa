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
    return (1.0f / 32768.0f) * __fabsf(diff);
}

// 0x801D11FC: turns field_0x33c by 0x5B0 whichever way is nearer the angle
// of mMapHitDir.
// FIXME: 96.2 %, regalloc only: the original keeps field_0x33c in r5 and
// the left angle in r5 (lis r3 0x4330 for the int-to-float constant, f0 for
// the 1/32768 scale); ours uses r3/r4/r5 and r0 for the constant.
void cls_804A46F4::fn_801D11FC() {
    s16 target = JMath::atanTable_.atan2_(mMapHitDir.x, mMapHitDir.y) * (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180());
    s16 left = field_0x33c + 0x5B0;
    s16 right = field_0x33c - 0x5B0;
    f32 distLeft = angleDist(target, left);
    f32 distRight = angleDist(target, right);
    if (distLeft < distRight) {
        field_0x33c = left;
    } else {
        field_0x33c = right;
    }
}

// The waypoint `i` of mPath, or the first when it is past the end.
static inline void waypoint(const cls_804A46F4& self, s32 i, JGeometry::TVec2f& out) {
    if (self.mPathCount <= i) {
        i = 0;
    }
    out = self.mPath[i];
}

// Appends (x, y) to the waypoints unless the list is full.
static inline void pushWaypoint(cls_804A46F4& self, f32 x, f32 y) {
    if (self.mPathCount < 16) {
        s32 n = self.mPathCount;
        self.mPath[n].x = x;
        self.mPath[n].y = y;
        self.mPathCount++;
    }
}

// 0x801D12CC.
// FIXME: 86 %, 30 instructions fewer than the original. (1) The front-pop
// loop mPath[i] = mPath[i + 1] unrolls with the source index recomputed per
// element (addi/slwi/add) and a walking store pointer (stfsu); ours strength
// reduces both to one pointer. (2) The final field_0x32c.set(mPath[0]...) in
// the original clamps the index twice (cmpwi remnants, y loaded first).
// (3) The pos push in the else branch loads pos.x after the address.
bool cls_804A46F4::fn_801D12CC() {
    bool result = false;
    if (field_0x338 >= 0 && fn_801D0B44(field_0x338)) {
        s32 player = field_0x338;
        JGeometry::TVec3f pos;
        JGeometry::TVec3f probe;
        JGeometry::TVec3f other;
        JGeometry::TVec3f playerPos;
        fn_8022E114(player, &playerPos);
        if (fn_801F329C(player)) {
            fn_801CD150(&playerPos, &other);
            pos.set(other.x, other.y, other.z);
        } else {
            pos.set(playerPos.x, playerPos.y, playerPos.z);
        }
        probe.set(pos.x, pos.y, 0.0f);
        if (vfunc_0x4C(field_0x4, &probe)) {
            s32 code = fn_802265FC(&pos, field_0x4);
            if (code >= 0x2C || code < 0x27) {
                mPathCount = 0;
                return false;
            }
        }
        result = true;
        // fn_801CF888 reads only (x, y) of its first position (8-byte slots).
        if (!fn_801CF888((const JGeometry::TVec2f*)&mPos, &pos, field_0x4)) {
            mPathCount = 0;
            field_0x32c.set(pos.x, pos.y, 0.0f);
        } else {
            if (mPathCount >= 1) {
                f32 dx = field_0x32c.x - mPos.x;
                f32 dy = field_0x32c.y - mPos.y;
                if (square(dx) + square(dy) <= square(0.001f + mTargetSpeed)) {
                    if (mPathCount >= 1) {
                        s32 last = mPathCount - 1;
                        for (s32 i = 0; i < last; i++) {
                            mPath[i] = mPath[i + 1];
                        }
                        mPathCount = last;
                    }
                }
            }
            bool far = false;
            bool clear = false;
            if (mPathCount == 0) {
                clear = true;
            } else {
                JGeometry::TVec2f from;
                waypoint(*this, mPathCount - 1, from);
                if (fn_801CF888(&from, &pos, field_0x4)) {
                    clear = true;
                } else {
                    f32 ey = pos.y - from.y;
                    f32 ex = pos.x - from.x;
                    if (ex * ex + ey * ey >= 64.0f) {
                        far = true;
                        if (mPathCount >= 2) {
                            JGeometry::TVec2f prev;
                            waypoint(*this, mPathCount - 2, prev);
                            if (!fn_801CF888(&prev, &pos, field_0x4)) {
                                if (mPathCount > 0) {
                                    mPathCount--;
                                }
                            }
                        }
                    }
                }
            }
            if (far || clear) {
                if (mPathCount >= 16) {
                    mPathCount = 0;
                    return false;
                }
                if (clear && mStateTimer > 2) {
                    f32 offsetY = fn_8023E724(field_0x338)->y - fn_8023E578(field_0x338)->y;
                    f32 y = pos.y - offsetY;
                    f32 offsetX = fn_8023E724(field_0x338)->x - fn_8023E578(field_0x338)->x;
                    f32 x = pos.x - offsetX;
                    pushWaypoint(*this, x, y);
                } else {
                    pushWaypoint(*this, pos.x, pos.y);
                }
            }
            JGeometry::TVec2f first;
            waypoint(*this, 0, first);
            field_0x32c.set(first.x, first.y, 0.0f);
        }
    }
    if (!result) {
        mPathCount = 0;
    }
    return result;
}

// 0x801D183C.
// FIXME: 99.2 %, scheduling+regalloc: the original keeps the result in r30
// and the player index in r31; ours has them the other way round.
bool cls_804A46F4::fn_801D183C(f32 range) {
    bool result = false;
    if (field_0x338 >= 0 && field_0x338 < 4 && fn_801F320C(field_0x338)) {
        s32 player = field_0x338;
        JGeometry::TVec3f pos;
        JGeometry::TVec3f other;
        JGeometry::TVec3f playerPos;
        fn_8022E114(player, &playerPos);
        if (fn_801F329C(player)) {
            fn_801CD150(&playerPos, &other);
            pos.set(other.x, other.y, other.z);
        } else {
            pos.set(playerPos.x, playerPos.y, playerPos.z);
        }
        if (range == 0.0f || square(pos.x - mPos.x) + square(pos.y - mPos.y) <= square(range)) {
            // fn_801CF888 reads only (x, y) of its first position (8-byte slots).
        if (!fn_801CF888((const JGeometry::TVec2f*)&mPos, &pos, field_0x4)) {
                result = true;
            }
        }
    }
    return result;
}

// 0x801D1974.
cls_804A46F4::cls_804A46F4() : mPathCount(0) {
    field_0x32c.set(0.0f, 0.0f, 0.0f);
    field_0x338 = -1;
    field_0x33c = 0;
}
