#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/Camera.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/tu_801E9AFC.h"
#include "JSystem/JMath/JMath.h"
#include "game/act/tu_801E6D48.h"
#include "game/act/tu_801E801C.h"

f32 fn_801CD6D8(f32 p0, f32 p1, f32 p2, f32 p3, f32 t);


// FIXME(shotgun): 67.00 %, missing/extra + reloc-name; behaviour clean.
cls_804A5568::cls_804A5568() {
    mMask = 0;
    field_0x2ac = 0;
    mCount = 0;
    field_0x3b0 = 0;
    field_0x3b4 = 0.0f;
    field_0x3b8 = -1;
    field_0x3bc = 0;
    mEmitter3c0 = NULL;
}

cls_804A5568::~cls_804A5568() {
    releaseEmitter_801EE434(mEmitter3c0);
}

// FIXME(shotgun): 86.04 %, different + reloc-name; behaviour drift.
void cls_804A5568::init() {
    EnemyBase_804A4808::init();
    field_0x2ac = field_0x90;
    f32 half = 16.0f;
    mBox.set(-half, -half, half, half);
    mBox2.set(1.0f, 1.0f, -1.0f, -1.0f);
    field_0x80.set(1.0f, 1.0f, -1.0f, -1.0f);
    field_0xb0 = 0;
    field_0x98 = 2;
    mFlags |= 0x280;
    mLife = 1000;
    mLifeBackup = mLife;
    mState = 0;
    mStateTimer = 0;
    field_0x250 = -1;
    mSpeed = 0.0f;
    mTargetSpeed = 0.0f;
}

// The point on the chain's curve at control point `idx`, sixteenth `seg` (0 and 16 are the control
// points themselves): inlined twice by fn_801E9224.
static inline void curvePoint(const cls_804A5568* self, u8 idx, u8 seg, f32* outX, f32* outY) {
    if (seg == 0) {
        *outX = self->field_0x2b0[idx].x;
        *outY = self->field_0x2b0[idx].y;
    } else if (seg == 16) {
        *outX = self->field_0x2b0[idx + 1].x;
        *outY = self->field_0x2b0[idx + 1].y;
    } else {
        f32 prevX;
        f32 prevY;
        if (idx == 0) {
            prevX = self->field_0x2b0[0].x;
            prevY = self->field_0x2b0[0].y;
        } else {
            prevX = self->field_0x2b0[idx - 1].x;
            prevY = self->field_0x2b0[idx - 1].y;
        }
        f32 p2x = self->field_0x2b0[idx + 1].x;
        f32 p2y = self->field_0x2b0[idx + 1].y;
        f32 p1x = self->field_0x2b0[idx].x;
        f32 p1y = self->field_0x2b0[idx].y;
        f32 p3x;
        f32 p3y;
        if (idx == self->mCount - 2) {
            p3x = p2x;
            p3y = p2y;
        } else {
            p3x = self->field_0x2b0[idx + 2].x;
            p3y = self->field_0x2b0[idx + 2].y;
        }
        f32 t = 0.0625f * (f32)seg;
        *outY = fn_801CD6D8(prevY, p1y, p2y, p3y, t);
        *outX = fn_801CD6D8(prevX, p1x, p2x, p3x, t);
    }
}

// Moves the chain's head along its curve by the speed (0.5 per frame).
// FIXME(shotgun): 81.23 %, missing/extra; behaviour drift.
void cls_804A5568::fn_801E9224() {
    if (!(mFlags & 0x4000)) {
        f32 budget = 0.5f;
        bool finished = false;
        while (budget > 0.0f) {
            u8 seg = (s32)field_0x3b4;
            u8 idx = field_0x3b0;
            f32 curX;
            f32 curY;
            curvePoint(this, idx, seg, &curX, &curY);
            u8 idx2 = field_0x3b0;
            u8 seg2 = (s32)field_0x3b4 + 1;
            if (seg2 >= 16) {
                if (idx2 >= mCount - 2) {
                    finished = true;
                } else {
                    idx2++;
                    seg2 -= 16;
                }
            }
            f32 nextX;
            f32 nextY;
            curvePoint(this, idx2, seg2, &nextX, &nextY);
            f32 dx = nextX - mPos.x;
            f32 dy = nextY - mPos.y;
            f32 sq = dx * dx + dy * dy;
            f32 len;
            if (sq <= 0.0f) {
                len = sq;
            } else {
                len = sq * JGeometry::TUtil<f32>::inv_sqrt(sq);
            }
            if (budget >= len) {
                mPos.x = nextX;
                mPos.y = nextY;
                mPos.z = 0.0f;
                if (finished) {
                    mFlags |= 0x4000;
                    field_0x3b0 = mCount - 2;
                    field_0x3b4 = 16.0f;
                    return;
                }
                budget -= len;
                field_0x3b4 = (f32)seg2;
                field_0x3b0 = idx2;
            } else {
                if (sq > 32.0f * FLT_EPSILON) {
                    f32 norm = JGeometry::TUtil<f32>::inv_sqrt(sq);
                    dx *= norm;
                    dy *= norm;
                }
                mPos.x += dx * budget;
                mPos.y += dy * budget;
                f32 sx = nextX - curX;
                f32 sy = nextY - curY;
                f32 sq2 = sx * sx + sy * sy;
                if (sq2 > 0.0f) {
                    sq2 = sq2 * JGeometry::TUtil<f32>::inv_sqrt(sq2);
                }
                field_0x3b4 += budget * (1.0f / sq2);
                return;
            }
        }
    }
}

// FIXME(shotgun): 98.07 %, missing/extra; behaviour near.
void cls_804A5568::vfunc_0x90(u8 mapHit) {
    switch (mState) {
    case 0: {
        u16 trigger = field_0x94;
        if (trigger != 0) {
            if (getGameView_802D6A9C()->fn_802D800C(trigger)) {
                mFlags |= 0x2000;
                if (mEmitter3c0 != NULL) {
                    getEffectSystem_8013C824()->mEmitterManager->forceDeleteEmitter(mEmitter3c0);
                }
                mEmitter3c0 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x260, 0, -2, NULL, NULL, true, 1.0f);
                mStateTimer = 0;
                mState = 1;
            }
        }
        break;
    }
    case 1:
        fn_801E9224();
        setEmitterPos_8013CB44(mEmitter3c0, &mPos, 0, -2, true);
        startSound_801F0E34(0x3003E, 0);
        if (mFlags & 0x4000) {
            s32 serial = field_0x3b8;
            if (serial >= 0) {
                ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                if (actor != NULL) {
                    ((cls_804A5418*)actor)->fn_801E6EAC();
                }
            }
            mFlags &= ~0x2000;
            releaseEmitter_801EE434(mEmitter3c0);
            mEmitter3c0 = NULL;
            mStateTimer = 0;
            mState = 2;
        }
        break;
    }
}

u8 cls_804A5568::vfunc_0x5C() {
    return 0;
}

// FIXME(shotgun): 93.68 %, missing/extra; behaviour near.
u8 cls_804A5568::vfunc_0x60() {
    if (mState == 0 && mBox.f.x - mBox.i.x > 0.0f && mBox.f.y - mBox.i.y > 0.0f) {
        if (fn_801E9B58() >= 0) {
            mFlags |= 0x2000;
            if (mEmitter3c0 != NULL) {
                getEffectSystem_8013C824()->mEmitterManager->forceDeleteEmitter(mEmitter3c0);
            }
            mEmitter3c0 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x260, 0, -2, NULL, NULL, true, 1.0f);
            mStateTimer = 0;
            mState = 1;
        }
    }
    return 0;
}

u8 cls_804A5568::vfunc_0x68() {
    u8 result = 0;
    if (mFlags & 0x2000) {
        f32 savedIX = mBox.i.x;
        f32 savedIY = mBox.i.y;
        f32 savedFX = mBox.f.x;
        f32 savedFY = mBox.f.y;
        mBox.i.x = -8.0f;
        mBox.i.y = -8.0f;
        mBox.f.x = 8.0f;
        mBox.f.y = 8.0f;
        result = fn_801F2B7C(0, -1, 0x16);
        mBox.i.x = savedIX;
        mBox.i.y = savedIY;
        mBox.f.x = savedFX;
        mBox.f.y = savedFY;
    }
    return result;
}

void cls_804A5568::vfunc_0xA0() {
    if (mFlags & 0x2000) {
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
    }
}

void cls_804A5568::fn_801E8EB4(u8 index, s32 arg, const JGeometry::TVec2f* pos) {
    mMask |= 1 << index;
    field_0x2b0[index].x = pos->x;
    field_0x2b0[index].y = pos->y;
}

// FIXME(shotgun): 95.88 %, missing/extra; behaviour drift.
void cls_804A5568::fn_801E8C74(s32 arg) {
    field_0x3b8 = arg;
    mCount = 0;
    bool run = true;
    for (s32 i = 0; i < 8; i++) {
        if (mMask & (1 << i)) {
            if (run) {
                mCount++;
            }
        } else {
            run = false;
        }
    }
    mPos.x = field_0x2b0[0].x;
    mPos.y = field_0x2b0[0].y;
    mPos.z = 0.0f;
    f32 y2 = field_0x2b0[0].y;
    f32 x0 = field_0x2b0[0].x;
    f32 x1 = field_0x2b0[1].x;
    f32 y1 = field_0x2b0[1].y;
    f32 x2;
    f32 y3;
    if (mCount == 2) {
        x2 = x1;
        y3 = y1;
    } else {
        x2 = field_0x2b0[2].x;
        y3 = field_0x2b0[2].y;
    }
    f32 ry = fn_801CD6D8(y2, y2, y1, y3, 0.0625f);
    f32 rx = fn_801CD6D8(x0, x0, x1, x2, 0.0625f);
    field_0x3bc = (s32)(JMath::atanTable_.atan2_(mPos.x - rx, mPos.y - ry) * 10430.378f);
    JGeometry::TVec2f off;
    fn_801E9C00(field_0x3bc, &off);
    mBox.i.x += off.x;
    mBox.i.y += off.y;
    mBox.f.x += off.x;
    mBox.f.y += off.y;
}

// Draws one strip of the chain (two vertices) at `center`, from angle `ang` to `ang + span`, `a` and
// `b` scaling the texture coordinate.
// Not auto-inlined: fn_801E8080 calls fn_801E8B1C out of line in the original (docs/INLINING.md, pragma rule 1).
#pragma auto_inline off
// FIXME(shotgun): 69.58 %, missing/extra + reloc-name; behaviour drift.
void fn_801E8B1C(f32 a, f32 b, const JGeometry::TVec2f* center, u16 ang, u16 span) {
    static f32 sZ = -256.0f;
    f32 h = 0.5f * (f32)(s32)(ang + span);
    s16 half = (s16)(h + (h > 0.0f ? 0.5f : -0.5f));
    f32 v = a * (1.0f / (4.0f * b));
    s16 delta = half - ang;
    f32 s0 = JMath::sincosTable_.sinShort(ang);
    f32 c0 = JMath::sincosTable_.cosShort(ang);
    f32 scale = 2.0f * (1.0f / JMath::sincosTable_.cosShort(delta));
    s16 perp = -(s16)(delta - 0x4000);
    f32 s2 = JMath::sincosTable_.sinShort(perp);
    f32 c2 = JMath::sincosTable_.cosShort(perp);
    f32 dx = (s0 * c2 - c0 * s2) * scale;
    f32 dy = (s0 * s2 + c0 * c2) * scale;
    GXPosition3f32(center->x + dx, sZ + (center->y + dy), sZ);
    GXTexCoord2f32(1.0f, v);
    GXPosition3f32(center->x + -dx, sZ + (center->y + -dy), sZ);
    GXTexCoord2f32(0.0f, v);
}
#pragma auto_inline reset

// FIXME(shotgun): 75.52 %, missing/extra + reloc-name; behaviour drift.
// cls_804A5568
void cls_804A5568::fn_801E8080() {
    f32 len = 0.0f;
    u8 split = 0;
    s32 totalVerts = ((mCount - 1) << 5) + 2;
    f32 b0 = fn_801E9AFC()->field_0x178[0];
    f32 b1 = fn_801E9AFC()->field_0x178[1];
    f32 b = b0;
    if (field_0x3b0 == 0 && field_0x3b4 == 0.0f) {
        fn_801E9AFC()->fn_801EA344();
    } else {
        fn_801E9AFC()->fn_801EA304();
        b = b1;
        if (!(mFlags & 0x4000)) {
            split = 1;
        }
    }
    f32 dx, dy;
    if (split != 0) {
        u8 segCur = (s32)(1.0f + field_0x3b4);
        s32 verts1 = (field_0x3b0 << 5) + (segCur << 1) + 2;
        GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, (u16)verts1);
        JGeometry::TVec2f prev;
        prev.x = field_0x2b0[0].x;
        prev.y = field_0x2b0[0].y;
        s32 drawn = 0;
        for (u8 i = 0; i <= field_0x3b0; i++) {
            for (u8 s = 1; s <= 16; s++) {
                f32 cx, cy;
                curvePoint(this, i, s, &cx, &cy);
                f32 sx = cx - prev.x;
                f32 sy = cy - prev.y;
                if (i == 0 && s == 1) {
                    dx = sx;
                    dy = sy;
                }
                u16 ang1 = (s32)(JMath::atanTable_.atan2_(sx, sy) * 10430.378f);
                u16 ang0 = (s32)(JMath::atanTable_.atan2_(dx, dy) * 10430.378f);
                fn_801E8B1C(len, b1, &prev, ang0, ang1);
                prev.x = cx;
                prev.y = cy;
                dx = sx;
                dy = sy;
                f32 sq = sx * sx + sy * sy;
                f32 d = (sq <= 0.0f) ? sq : sq * JGeometry::TUtil<f32>::inv_sqrt(sq);
                drawn += 2;
                len += d;
                if (drawn >= verts1 - 2) {
                    break;
                }
            }
            if (drawn >= verts1 - 2) {
                break;
            }
        }
        f32 sq = dx * dx + dy * dy;
        f32 d = (sq <= 0.0f) ? sq : sq * JGeometry::TUtil<f32>::inv_sqrt(sq);
        u8 segFloor = (s32)field_0x3b4;
        f32 midLen = (len - d) + d * (field_0x3b4 - (f32)segFloor);
        u16 angEnd1 = (s32)(JMath::atanTable_.atan2_(dx, dy) * 10430.378f);
        u16 angEnd0 = (s32)(JMath::atanTable_.atan2_(dx, dy) * 10430.378f);
        fn_801E8B1C(midLen, b1, (const JGeometry::TVec2f*)&mPos, angEnd0, angEnd1);
        fn_801E9AFC()->fn_801EA344();
        GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, (u16)(totalVerts - verts1 + 4));
        u16 angStart1 = (s32)(JMath::atanTable_.atan2_(dx, dy) * 10430.378f);
        u16 angStart0 = (s32)(JMath::atanTable_.atan2_(dx, dy) * 10430.378f);
        fn_801E8B1C(midLen, b0, (const JGeometry::TVec2f*)&mPos, angStart0, angStart1);
        s32 minSeg = segCur + 1;
        u8 minIdx = field_0x3b0;
        for (u8 i = minIdx; i < mCount - 1; i++) {
            for (u8 s = 1; s <= 16; s++) {
                if (i > minIdx || s >= minSeg) {
                    f32 cx, cy;
                    curvePoint(this, i, s, &cx, &cy);
                    f32 sx = cx - prev.x;
                    f32 sy = cy - prev.y;
                    u16 ang1 = (s32)(JMath::atanTable_.atan2_(sx, sy) * 10430.378f);
                    u16 ang0 = (s32)(JMath::atanTable_.atan2_(dx, dy) * 10430.378f);
                    fn_801E8B1C(len, b0, &prev, ang0, ang1);
                    prev.x = cx;
                    prev.y = cy;
                    dx = sx;
                    dy = sy;
                    f32 sq2 = sx * sx + sy * sy;
                    f32 d2 = (sq2 <= 0.0f) ? sq2 : sq2 * JGeometry::TUtil<f32>::inv_sqrt(sq2);
                    len += d2;
                }
            }
        }
        u16 angLast1 = (s32)(JMath::atanTable_.atan2_(dx, dy) * 10430.378f);
        u16 angLast0 = (s32)(JMath::atanTable_.atan2_(dx, dy) * 10430.378f);
        fn_801E8B1C(len, b0, &prev, angLast0, angLast1);
    } else {
        GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, (u16)totalVerts);
        JGeometry::TVec2f prev;
        prev.x = field_0x2b0[0].x;
        prev.y = field_0x2b0[0].y;
        for (u8 i = 0; i < mCount - 1; i++) {
            for (u8 s = 1; s <= 16; s++) {
                f32 cx, cy;
                curvePoint(this, i, s, &cx, &cy);
                f32 sx = cx - prev.x;
                f32 sy = cy - prev.y;
                if (i == 0 && s == 1) {
                    dx = sx;
                    dy = sy;
                }
                u16 ang1 = (s32)(JMath::atanTable_.atan2_(sx, sy) * 10430.378f);
                u16 ang0 = (s32)(JMath::atanTable_.atan2_(dx, dy) * 10430.378f);
                fn_801E8B1C(len, b, &prev, ang0, ang1);
                prev.x = cx;
                prev.y = cy;
                dx = sx;
                dy = sy;
                f32 sq = sx * sx + sy * sy;
                f32 d = (sq <= 0.0f) ? sq : sq * JGeometry::TUtil<f32>::inv_sqrt(sq);
                len += d;
            }
        }
        u16 angLast1 = (s32)(JMath::atanTable_.atan2_(dx, dy) * 10430.378f);
        u16 angLast0 = (s32)(JMath::atanTable_.atan2_(dx, dy) * 10430.378f);
        fn_801E8B1C(len, b, &prev, angLast0, angLast1);
    }
}

void cls_804A5568::fn_801E801C() {
    static f32 sZ = -256.0f;
    if (mState == 0) {
        fn_801E9AFC()->fn_801EA108((const JGeometry::TVec2f*)&mPos, field_0x3bc, sZ);
    }
}

