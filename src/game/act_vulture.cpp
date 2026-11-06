#include <float.h>
#include "game/GameAudio.h"
#include "game/act/act_vulture.h"
#include "game/GameGlobals.h"
#include "game/GameMath.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/ViewLayer.h"
#include "JSystem/JMath/JMath.h"

static inline void normalize(JGeometry::TVec2f& v) {
    f32 sq = v.x * v.x + v.y * v.y;
    if (sq <= 32.0f * FLT_EPSILON) {
        return;
    }
    f32 norm = JGeometry::TUtil<f32>::inv_sqrt(sq);
    v.x *= norm;
    v.y *= norm;
}

static inline void approach(f32& cur, f32 target, f32 remaining) {
    if (remaining < FLT_EPSILON) {
        cur = target;
    } else {
        cur += (1.0f / remaining) * (target - cur);
    }
}

static const s16 lbl_8053ED28[] = {3, 20, -1, -1};
static const s16 lbl_804693F0[] = {0, 8, 1, 8, 2, 8, -1, -1};
static const s16* lbl_8053A128[] = {lbl_8053ED28, lbl_804693F0};

Vulture_804A4230::Vulture_804A4230() {
    field_0x2a8 = -1;
    field_0x2ac = 0.0f;
    field_0x2b0 = 0;
    field_0x2b2 = 0;
    field_0x2b4 = 0;
    field_0x2b8.x = 0.0f;
    field_0x2b8.y = 0.0f;
    field_0x2c0.x = 0.0f;
    field_0x2c0.y = 0.0f;
    field_0x2c8.x = 0.0f;
    field_0x2c8.y = 0.0f;
}

void Vulture_804A4230::init() {
    EnemyBase_804A4808::init();
    u32 val = 4;
    u32 param = (u8)field_0x90;
    if (param > 4) {
        val = param;
    }
    field_0x2b4 = val;
    if (field_0x90 >> 31) {
        mFlags |= 0x4000;
    }
    field_0x80.set(1.0f, 1.0f, -1.0f, -1.0f);
    mBox.set(-10.0f, -8.0f, 10.0f, 8.0f);
    if (mFlags & 0x4000) {
        mBox2.set(1.0f, 1.0f, -1.0f, -1.0f);
        field_0xb0 = 0;
    } else {
        mBox2.set(mBox.i.x, mBox.i.y, mBox.f.x, mBox.f.y);
        field_0xb0 = 0x50202;
    }
    field_0x98 = 2;
    field_0x194 = 1;
    mPos.z = 1.0f;
    mFlags |= 0x20;
    mState = 0;
    mStateTimer = 0;
    field_0x250 = fn_801CD664(55) + 6;
    mSpeed = 0.0f;
    mTargetSpeed = 0.0f;
    mAnim.fn_801D2608(lbl_8053A128, 2);
    mAnim.startAnim_801D22DC(0, 0, false);
}

// FIXME: 85.17 % (missing/extra + reloc-name). Compiler promotes local TVec2s to registers
// instead of keeping stack temporaries at 0x8, 0x10, 0x18, 0x20.
void Vulture_804A4230::fn_801C0634() {
    JGeometry::TVec2f diff = field_0x2b8;
    diff.y -= field_0x2c0.y;
    diff.x -= field_0x2c0.x;
    normalize(diff);
    diff.x *= field_0x2ac;
    diff.y *= field_0x2ac;

    f32 targetX;
    f32 targetY;
    if (field_0x2a8 >= 0) {
        JGeometry::TVec3f* pPos = getPlayerPos_8023E724(field_0x2a8);
        targetX = pPos->x;
        targetY = pPos->y;
    } else {
        targetX = mSpawnPos.x;
        targetY = mSpawnPos.y;
    }

    fn_801CD8E4(&field_0x2c0.x, targetX, 1.0f, 0.0f);
    fn_801CD8E4(&field_0x2c0.y, targetY, 1.0f, 0.0f);

    JGeometry::TVec2f target = field_0x2c0;
    target.x += diff.x;
    target.y += diff.y;
    fn_801CD8E4(&field_0x2b8.x, target.x, 0.05f, 0.0f);
    fn_801CD8E4(&field_0x2b8.y, target.y, 0.05f, 0.0f);

    JGeometry::TVec2f diff2 = field_0x2b8;
    diff2.x -= field_0x2c0.x;
    diff2.y -= field_0x2c0.y;

    f32 dist;
    if (field_0x2a8 >= 0) {
        dist = JMAFastSqrt(fn_801D0DA0(field_0x2a8, &mPos));
    } else {
        dist = JMAFastSqrt(square(mSpawnPos.x - mPos.x) + square(mSpawnPos.y - mPos.y));
    }

    s16 angle = field_0x2b0;
    if (dist > 0.001f) {
        s32 a = (s32)((1.0f / dist) * (field_0x2ac * (f32)field_0x2b0));
        if ((s16)a > field_0x2b0) {
            angle = field_0x2b0;
        } else if ((s16)a < 0x100) {
            angle = 0x100;
        } else {
            angle = a;
        }
    }

    if (mState == 5) {
        angle = (s32)((1.0f - 0.05f * (20 - mStateTimer)) * (f32)(s16)angle);
    }

    if (mFlags & 0x2000) {
        angle = -angle;
    }

    s16 rot = -angle;
    f32 s = JMath::sincosTable_.sinShort(rot);
    f32 c = JMath::sincosTable_.cosShort(rot);

    JGeometry::TVec2f res = field_0x2c0;
    res.x += diff2.x * c - diff2.y * s;
    res.y += diff2.x * s + diff2.y * c;
    field_0x2b8 = res;
}

void Vulture_804A4230::fn_801C0438(s32 state) {
    switch (state) {
    case 0:
        field_0x250 = fn_801CD664(55) + 6;
        field_0x2a8 = -1;
        mAnim.startAnim_801D22DC(0, 0, false);
        break;
    case 1:
        mAnim.fn_801D22A0(1, 2);
        if (mState != 6) {
            startSound_801F0E34(0x400AA, 0);
        }
        break;
    case 2:
        if (mFlags & 0x4000) {
            field_0x250 = fn_801CD664(421) + 180;
        } else {
            field_0x250 = fn_801CD664(181) + 120;
        }
        mAnim.fn_801D22A0(1, 2);
        field_0x2b8.x = mPos.x;
        field_0x2b8.y = mPos.y;
        if (field_0x2a8 >= 0) {
            const JGeometry::TVec3f* pPos = getPlayerPos_8023E724(field_0x2a8);
            field_0x2c0.x = pPos->x;
            field_0x2c0.y = pPos->y;
        } else {
            field_0x2c0.x = mSpawnPos.x;
            field_0x2c0.y = mSpawnPos.y;
        }
        break;
    case 3:
        mAnim.fn_801D22A0(1, 2);
        mTargetSpeed = 0.0f;
        mSpeed = 0.0f;
        break;
    case 4:
        mAnim.fn_801D22A0(1, 2);
        mTargetSpeed = 4.5f;
        field_0x2b2 = fn_801D0280(field_0x2a8);
        break;
    case 5:
        mAnim.fn_801D22A0(1, 2);
        field_0x2b8.x = mPos.x;
        field_0x2b8.y = mPos.y;
        {
            const JGeometry::TVec3f* pPos = getPlayerPos_8023E724(field_0x2a8);
            field_0x2c0.x = pPos->x;
            field_0x2c0.y = pPos->y;
        }
        field_0x2c8.x = field_0x3c.x;
        field_0x2c8.y = field_0x3c.y;
        break;
    case 6:
        field_0x2a8 = -1;
        mAnim.fn_801D22A0(1, 2);
        break;
    }
    mStateTimer = 0;
    mState = state;
}

// FIXME: 97.44 % (scheduling+regalloc + reloc-name). Same instructions modulo registers
// and literal order.
void Vulture_804A4230::vfunc_0x90(u8 mapHit) {
    switch (mState) {
    case 0:
        if (mStateTimer >= field_0x250) {
            field_0x2a8 = fn_801D05F8(&mPos, 16.0f * (f32)field_0x2b4);
            if (field_0x2a8 >= 0) {
                fn_801C0438(1);
            }
        }
        break;

    case 1:
        if (mPos.z >= 18.0f) {
            mPos.z = 18.0f;
            field_0x2ac = fn_801CD664(33) + 48;
            field_0x2b0 = fn_801CD664(257) + 512;
            if (fn_801CD664(2) == 0) {
                mFlags &= ~0x2000;
            } else {
                mFlags |= 0x2000;
            }
            fn_801C0438(2);
        }
        break;

    case 2:
        if (field_0x2a8 >= 0) {
            if (!fn_80230188(field_0x2a8) && !fn_801D0B44(field_0x2a8)) {
                s32 player = fn_801D05F8(&mPos, 16.0f * (f32)field_0x2b4);
                if (player >= 0 && player != field_0x2a8 && fn_8022E438(player, field_0x2a8)) {
                    field_0x2a8 = player;
                    fn_801C0438(2);
                } else {
                    fn_801C0438(6);
                }
                break;
            }

            if (mStateTimer >= field_0x250) {
                if (!fn_80230188(field_0x2a8)) {
                    f32 dist = fn_801D0DA0(field_0x2a8, &mPos);
                    if (dist <= square(16.0f + field_0x2ac)) {
                        fn_801C0438(3);
                        break;
                    }
                }
            }
            fn_801C0634();
        } else {
            field_0x2a8 = fn_801D05F8(&mPos, 16.0f * (f32)field_0x2b4);
            if (field_0x2a8 >= 0) {
                fn_801C0438(2);
            } else {
                fn_801C0634();
            }
        }
        break;

    case 3:
        if (mStateTimer >= 15) {
            if (!fn_801D0B44(field_0x2a8)) {
                s32 player = fn_801D05F8(&mPos, 16.0f * (f32)field_0x2b4);
                if (player >= 0 && player != field_0x2a8 && fn_8022E438(player, field_0x2a8)) {
                    field_0x2a8 = player;
                    fn_801C0438(2);
                } else {
                    fn_801C0438(6);
                }
            } else {
                fn_801C0438(4);
            }
        }
        break;

    case 4:
        if (mStateTimer >= 50) {
            if (!fn_801D0B44(field_0x2a8)) {
                s32 player = fn_801D05F8(&mPos, 16.0f * (f32)field_0x2b4);
                if (player >= 0 && player != field_0x2a8 && fn_8022E438(player, field_0x2a8)) {
                    field_0x2a8 = player;
                    fn_801C0438(2);
                } else {
                    fn_801C0438(6);
                }
            } else {
                fn_801C0438(5);
            }
        }
        break;

    case 5:
        if (!fn_801D0B44(field_0x2a8)) {
            s32 player = fn_801D05F8(&mPos, 16.0f * (f32)field_0x2b4);
            if (player >= 0 && player != field_0x2a8 && fn_8022E438(player, field_0x2a8)) {
                field_0x2a8 = player;
                fn_801C0438(2);
            } else {
                fn_801C0438(6);
            }
        } else {
            fn_801C0634();
            if (mStateTimer >= 20) {
                fn_801C0438(2);
            }
        }
        break;

    case 6:
        if (mPos.z <= 1.0f) {
            mPos.z = 1.0f;
            bool hit = false;
            if (viewLayerFlag(field_0x4)) {
                JGeometry::TVec2f point;
                point.x = mPos.x;
                point.y = 8.0f + (mPos.y - mPos.z);
                if (fn_802265FC((const JGeometry::TVec3f*)&point, field_0x4) == 2) {
                    hit = true;
                }
            }
            if (hit) {
                field_0x2a8 = -1;
                fn_801C0438(1);
            } else {
                fn_801C0438(0);
            }
        }
        break;
    }
}

// FIXME: 99.53 % (reloc-name). 100 % instruction match; literal names not yet split.
void Vulture_804A4230::vfunc_0x88() {
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    JGeometry::TVec2f vel(0.0f, 0.0f);

    switch (mState) {
    case 2:
    case 5:
        fn_801D01A0((const JGeometry::TVec3f*)&field_0x2b8, &vel, FLT_MAX);
        field_0x3c.x = vel.x;
        field_0x3c.y = vel.y;
        if (mState == 5) {
            f32 remaining = 20 - mStateTimer;
            approach(field_0x2c8.x, 0.0f, remaining);
            approach(field_0x2c8.y, 0.0f, remaining);
            field_0x3c.x += field_0x2c8.x;
            field_0x3c.y += field_0x2c8.y;
        }
        break;

    case 4:
        vel.x = JMath::sincosTable_.sinShort(field_0x2b2);
        vel.y = JMath::sincosTable_.cosShort(field_0x2b2);
        field_0x3c.x = mSpeed * vel.x;
        field_0x3c.y = mSpeed * vel.y;
        break;
    }

    switch (mState) {
    case 1:
        mPos.z += 0.4f;
        if (mPos.z > 18.0f) {
            mPos.z = 18.0f;
        }
        break;
    case 6:
        mPos.z -= 0.2f;
        if (mPos.z < 1.0f) {
            mPos.z = 1.0f;
        }
        break;
    }
}

// FIXME: 99.75 % (reloc-name). 100 % instruction match; literal names not yet split.
void Vulture_804A4230::vfunc_0x94() {
    f32 rate = 1.0f;
    switch (mState) {
    case 1:
    case 3:
    case 4:
        rate = 1.5f;
        break;
    }
    mAnim.mFrameCtrl.setRate(rate);
    EnemyBase_804A4808::vfunc_0x94();
}

void Vulture_804A4230::vfunc_0x74() {
    field_0x2b8.x = mPos.x;
    field_0x2b8.y = mPos.y;
    if (field_0x2a8 >= 0) {
        const JGeometry::TVec3f* pPos = getPlayerPos_8023E724(field_0x2a8);
        field_0x2c0.x = pPos->x;
        field_0x2c0.y = pPos->y;
    } else {
        field_0x2c0.x = mSpawnPos.x;
        field_0x2c0.y = mSpawnPos.y;
    }
}

Vulture_804A4230::~Vulture_804A4230() {}
