#include "game/GameAudio.h"
#include "game/Effect.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/Player.h"
#include "JSystem/JMath/JMath.h"
#include "game/act/act_beamos.h"

Beamos_804A40E0::Beamos_804A40E0() {
    field_0x2a8 = 0;
    field_0x2aa = 0;
    field_0x2ac = 0;
    field_0x2b0 = 0;
    field_0x2b4 = 0;
}

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

// Not auto-inlined: fn_801BE6C4 calls fn_801BE86C out of line in the original (docs/INLINING.md,
// pragma rule 1).
#pragma auto_inline off
// FIXME: 81.63 %. One instruction short: the original keeps the sincos table
// and its cos half (table + 4) in r30 and loads mPos.y before mPos.x; ours keeps
// only the table base and loads the literals earlier.
void Beamos_804A40E0::fn_801BE86C() {
    const JMath::TSinCosTable<11, f32>& sincos = JMath::sincosTable_;
    JGeometry::TVec3f pos(mPos.x + 10.0f * sincos.sinShort(field_0x2a8),
                          -15.0f + (6.0f * sincos.cosShort(field_0x2a8) + mPos.y), mPos.z);
    s32 serial = getActorManager_801F856C()->spawnActor_801F9484('LNBM', field_0x4, &pos, mSerial, -1, -1);
    if (serial < 0) {
        return;
    }
    ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
    if (actor == NULL) {
        return;
    }
    s32 depth = 0;
    if (pos.y >= -15.0f + mPos.y) {
        depth = 1;
    }
    actor->field_0x94 = depth;
    JGeometry::TVec2f dir(10.0f * sincos.sinShort(field_0x2a8), 6.0f * sincos.cosShort(field_0x2a8));
    dir.normalize();
    dir.scale(10.5f);
    actor->field_0x3c = JGeometry::TVec3f(dir.x, dir.y, 0.0f);
}
#pragma auto_inline reset

// FIXME: 99.76 %. Instructions identical; only the .sdata2 literal names differ
// (the unit's literals are not split in the original order yet).
void Beamos_804A40E0::fn_801BE6C4(s32 state) {
    switch (state) {
    case 0:
        field_0x250 = fn_801CD664(1) + 0x5A;
        break;
    case 2: {
        const JMath::TSinCosTable<11, f32>& sincos = JMath::sincosTable_;
        JGeometry::TVec3f pos(mPos.x + 10.0f * sincos.sinShort(field_0x2a8),
                              -15.0f + (6.0f * sincos.cosShort(field_0x2a8) + mPos.y), mPos.z);
        s32 depth = 0;
        if (pos.y >= -15.0f + mPos.y) {
            depth = 1;
        }
        spawnEmitter_8013CC50(field_0x4, &pos, 0x1C, 0, depth, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &pos, 0x1D, 0, depth, NULL, NULL, true, 1.0f);
        startSound_801F0E34(0x30062, 0);
        field_0x2aa = 60;
        break;
    }
    case 3:
        field_0x250 = fn_801CD664(1) + 0x5A;
        fn_801BE86C();
        startSound_801F0E34(0x30063, 0);
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

void Beamos_804A40E0::vfunc_0x98() {}

bool Beamos_804A40E0::vfunc_0x60() {
    u32 includeId = field_0x2aa != 0 ? 'NULL' : mActorId;
    return fn_801F2DB4(includeId, 'SWBM', NULL);
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
            startSound_801F0E34(0x4000C, 0);
        }
    } else {
        hit = fn_801CEBD0();
    }
    return hit;
}

void Beamos_804A40E0::vfunc_0x24() {
    spawnEmitter_8013CC50(field_0x4, &mPos, 0x70, 0, 0, NULL, NULL, true, 1.0f);
    spawnEmitter_8013CC50(field_0x4, &mPos, 0x71, 0, 0, NULL, NULL, true, 1.0f);
    spawnEmitter_8013CC50(field_0x4, &mPos, 0x72, 0, 0, NULL, NULL, true, 1.0f);
    ActorBase_804A5770::vfunc_0x24();
}

// FIXME: 95.22 %. Same instructions modulo registers: the position block's
// table pointer and x term are scheduled in another order (as in fn_801BE86C),
// plus the .sdata2 literal names.
void Beamos_804A40E0::draw() {
    SpriteParams_801F5AE0 params;
    if (mState == 2) {
        params.field_0x14.r = 0x80;
        params.field_0x14.g = 0x80;
        params.field_0x14.b = 0x80;
    }
    drawSprite_801F06F0("enemy_gc_sob.bin", 0x13C, &params, -1, 1.0f, 1.0f);
    if (mFlags & 0x2000) {
        return;
    }
    static JUtility::TColor lbl_8053AC0C(0xFF, 0xFF, 0xFF, 0xFF);
    const JMath::TSinCosTable<11, f32>& sincos = JMath::sincosTable_;
    f32 floorY = -15.0f + mPos.y;
    JGeometry::TVec3f pos(mPos.x + 10.0f * sincos.sinShort(field_0x2a8),
                          -15.0f + (6.0f * sincos.cosShort(field_0x2a8) + mPos.y), mPos.z);
    s32 depth = 0;
    if (pos.y >= floorY) {
        depth = 1;
    }
    fn_8013AC90(4.0f, &pos, &lbl_8053AC0C, 0, depth, NULL);
}

Beamos_804A40E0::~Beamos_804A40E0() {}
