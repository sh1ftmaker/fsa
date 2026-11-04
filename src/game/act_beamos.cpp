#include "game/GameAudio.h"
#include "game/Effect.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/Player.h"
#include "JSystem/JMath/JMath.h"
#include "game/act/act_beamos.h"
#include <float.h>

// The draw's once-only state (.sbss): a flag, then four bytes of 0xFF.
static s8 lbl_8053AC08;
static u8 lbl_8053AC0C[4];

Beamos_804A40E0::Beamos_804A40E0() {
    field_0x2a8 = 0;
    field_0x2aa = 0;
    field_0x2ac = 0;
    field_0x2b0 = 0;
    field_0x2b4 = 0;
}

Beamos_804A40E0::~Beamos_804A40E0() {}

void Beamos_804A40E0::init() {
    EnemyBase_804A4808::init();
    u32 count = field_0x90 & 0xFF;
    if (count > 0xF) {
        count = 0xF;
    } else if (count < 1) {
        count = 1;
    }
    field_0x2ac = count;
    field_0x2b0 = (field_0x90 >> 22) & 0x1F;
    field_0x2b4 = field_0x90 >> 27;
    if (field_0x2b0 != 0) {
        mFlags |= 0x2000;
    }
    mBox.set(-8.0f, -16.0f, 8.0f, 8.0f);
    mBox2.set(-8.0f, -20.0f, 8.0f, 8.0f);
    field_0x80.set(1.0f, 1.0f, -1.0f, -1.0f);
    field_0x98 = 2;
    mFlags |= ENEMY_FLAG_NO_HURT | ENEMY_FLAG_NO_ATTACK;
    field_0x2a8 = fn_801CD664(0x10000);
    mState = 0;
    mStateTimer = 0;
    field_0x250 = fn_801CD664(1) + 0x5A;
    mSpeed = 0.0f;
    mTargetSpeed = 0.0f;
    if (field_0x2aa != 0) {
        field_0xb0 = 0x40002;
    } else {
        field_0xb0 = 0x40004;
    }
}

void Beamos_804A40E0::vfunc_0x24() {
    fn_8013CC50(field_0x4, &mPos, 0x70, 0, 0, NULL, NULL, true, 1.0f);
    fn_8013CC50(field_0x4, &mPos, 0x71, 0, 0, NULL, NULL, true, 1.0f);
    fn_8013CC50(field_0x4, &mPos, 0x72, 0, 0, NULL, NULL, true, 1.0f);
    ActorBase_804A5770::vfunc_0x24();
}

bool Beamos_804A40E0::vfunc_0x5C() {
    bool hit = false;
    if (mState == 2) {
        for (s32 i = 0; i < 4; i++) {
            if (i == fn_801F285C(NULL, NULL, true, i)) {
                fn_8023B6CC(i);
                if (!fn_8023E854(i)) {
                    JGeometry::TVec2f center;
                    fn_801F3128(&center);
                    if (fn_80238548(i, field_0x114, &center, 10)) {
                        hit = true;
                    }
                }
            }
        }
        if (hit) {
            fn_801F0E34(0x4000C, 0);
        }
    } else {
        hit = fn_801CEBD0();
    }
    return hit;
}

// FIXME: 80.85 %. The original keeps `this` in r30 and loads the layer and
// serial into r30 and r31 before the fn_8020D58C call (ours loads them after
// it, so only r31 is saved); its box corners also load mPos.y before the 8.0f
// constant and add it as (y, c) where ours adds (c, y).
void Beamos_804A40E0::vfunc_0x9C(u8 mapHit) {
    JGeometry::TBox2f listBox;
    fn_801CE2BC(mapHit);
    if (field_0x2aa != 0) {
        field_0x2aa--;
    }
    JGeometry::TBox2f box(mPos.x - 8.0f, mPos.y - 8.0f, mPos.x + 8.0f, mPos.y + 8.0f);
    fn_80239914(&box, field_0x4, mSerial);
    if (field_0x2aa == 0) {
        listBox.set(mPos.x - 8.0f, mPos.y - 8.0f, mPos.x + 8.0f, mPos.y + 8.0f);
        CollisionBoxes_8020D58C* boxes = fn_8020D58C();
        boxes->mLists[boxes->field_0x0].fn_8020D8C0(&listBox, field_0x4, mSerial);
    }
}

// FIXME: 85.63 %. The original computes the depth test's -15 + y in its own
// register before the sin/cos loads and the x term; ours reuses the y sum.
void Beamos_804A40E0::fn_801BE6C4(s32 state) {
    switch (state) {
    case 0:
        field_0x250 = fn_801CD664(1) + 0x5A;
        break;
    case 2: {
        const JMath::TSinCosTable<11, f32>& sincos = JMath::sincosTable_;
        JGeometry::TVec3f pos;
        pos.x = mPos.x + 10.0f * sincos.sinShort(field_0x2a8);
        pos.y = -15.0f + (6.0f * sincos.cosShort(field_0x2a8) + mPos.y);
        pos.z = mPos.z;
        s32 depth = 0;
        if (pos.y < -15.0f + mPos.y) {
            depth = 1;
        }
        fn_8013CC50(field_0x4, &pos, 0x1C, 0, depth, NULL, NULL, true, 1.0f);
        fn_8013CC50(field_0x4, &pos, 0x1D, 0, depth, NULL, NULL, true, 1.0f);
        fn_801F0E34(0x30062, 0);
        field_0x2aa = 60;
        break;
    }
    case 3:
        field_0x250 = fn_801CD664(1) + 0x5A;
        fn_801BE86C();
        fn_801F0E34(0x30063, 0);
        break;
    }
    mStateTimer = 0;
    mState = state;
    if (field_0x2aa != 0) {
        field_0xb0 = 0x40002;
    } else {
        field_0xb0 = 0x40004;
    }
}

// FIXME: 75.41 %. Same position and depth sums as fn_801BE6C4, but the original
// keeps the table pointer in r30 and the actor in r31 and schedules the
// sin/cos loads and the (x, y) temporaries differently; the normalise is the
// JGeometry one (TUtil::inv_sqrt, 32 * FLT_EPSILON) and matches.
static inline void normalize(JGeometry::TVec2f& v) {
    f32 sq = v.x * v.x + v.y * v.y;
    if (sq <= 32.0f * FLT_EPSILON) {
        return;
    }
    f32 norm = JGeometry::TUtil<f32>::inv_sqrt(sq);
    v.x *= norm;
    v.y *= norm;
}

void Beamos_804A40E0::fn_801BE86C() {
    const JMath::TSinCosTable<11, f32>& sincos = JMath::sincosTable_;
    JGeometry::TVec3f pos;
    pos.x = mPos.x + 10.0f * sincos.sinShort(field_0x2a8);
    pos.y = -15.0f + (6.0f * sincos.cosShort(field_0x2a8) + mPos.y);
    pos.z = mPos.z;
    s32 serial = fn_801F856C()->fn_801F9484('LNBM', field_0x4, &pos, mSerial, -1, -1);
    if (serial < 0) {
        return;
    }
    ActorBase_804A5770* actor = fn_801F856C()->fn_801F666C(serial);
    if (actor == NULL) {
        return;
    }
    s32 depth = 0;
    if (pos.y < -15.0f + mPos.y) {
        depth = 1;
    }
    actor->field_0x94 = depth;
    JGeometry::TVec2f dir(10.0f * sincos.sinShort(field_0x2a8), 6.0f * sincos.cosShort(field_0x2a8));
    normalize(dir);
    dir.x *= 10.5f;
    dir.y *= 10.5f;
    actor->field_0x3c = JGeometry::TVec3f(dir.x, dir.y, 0.0f);
}

// FIXME: 94.45 %. The sprite call and the once-only table match. The position
// block is one instruction shorter than the original's: the table pointer and
// the x term are loaded in another order (the same sum as fn_801BE86C).
void Beamos_804A40E0::draw() {
    SpriteParams_801F5AE0 params;
    if (mState == 2) {
        params.field_0x14.mColor.r = 0x80;
        params.field_0x14.mColor.g = 0x80;
        params.field_0x14.mColor.b = 0x80;
    }
    fn_801F06F0("enemy_gc_sob.bin", 0x13C, &params, -1, 1.0f, 1.0f);
    if (mFlags & 0x2000) {
        return;
    }
    if (!lbl_8053AC08) {
        for (s32 i = 0; i < 4; i++) {
            lbl_8053AC0C[i] = 0xFF;
        }
        lbl_8053AC08 = 1;
    }
    const JMath::TSinCosTable<11, f32>& sincos = JMath::sincosTable_;
    f32 floorY = -15.0f + mPos.y;
    JGeometry::TVec3f pos;
    pos.x = mPos.x + 10.0f * sincos.sinShort(field_0x2a8);
    pos.y = -15.0f + (6.0f * sincos.cosShort(field_0x2a8) + mPos.y);
    pos.z = mPos.z;
    s32 depth = 0;
    if (floorY > pos.y) {
        depth = 1;
    }
    fn_8013AC90(&pos, lbl_8053AC0C, 0, depth, 0, 4.0f);
}

void Beamos_804A40E0::vfunc_0x98() {}

bool Beamos_804A40E0::vfunc_0x60() {
    u32 includeId = field_0x2aa != 0 ? 'NULL' : mActorId;
    return fn_801F2DB4(includeId, 'SWBM', NULL);
}
