#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/EmitterDelete.h"
#include "game/Effect.h"
#include "game/EnemyBase.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Player.h"
#include "JSystem/JMath/JMath.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/act/act_snowball.h"

// Animations: (cel, duration) pairs, ended by a cel of -1.
static const s16 lbl_80464308[] = {0, 10, 1, 10, -1, -1};
static const s16 lbl_80464314[] = {2, 3, 3, 3, 4, 3, 5, 3, -1, -1};
static const u32 lbl_80464328[] = {0x00260001, 0x00260004, 0x00260007};
static const s16* lbl_80539EE8[] = {lbl_80464308, lbl_80464314};

// Diagonal scale matrix with the translation (0, ty, 0).
static inline void setScaleMtx(Mtx m, const JGeometry::TVec3f& scale, f32 ty) {
    m[0][3] = 0.0f;
    m[1][3] = ty;
    m[2][3] = 0.0f;
    m[0][0] = scale.x;
    m[0][1] = 0.0f;
    m[0][2] = 0.0f;
    m[1][0] = 0.0f;
    m[1][1] = scale.y;
    m[1][2] = 0.0f;
    m[2][0] = 0.0f;
    m[2][1] = 0.0f;
    m[2][2] = scale.z;
}

// Emits and updates the trail emitter (inlined three times in update()).
inline void cls_8049D020::updateTrailEmitter() {
    if (field_0x4 == 0) {
        if (field_0x374 == NULL) {
            field_0x374 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x3D3, 0, 0, NULL, NULL, true, 1.5f);
        }
        JGeometry::TVec3f emitterPos = mPos;
        emitterPos.y += -2.0f * field_0x260;
        setEmitterPos_8013CB44(field_0x374, &emitterPos, 0, 0, true);
        field_0x374->setGlobalAlpha(0x80);
        if (field_0x264 < 0.2f) {
            field_0x374->stopCreateParticle();
        } else {
            field_0x374->playCreateParticle();
        }
    }
}

// Applies gravity to the velocity and moves; true once landed (inlined three times in update()).
inline bool cls_8049D020::fallStep() {
    fn_801CD950(&field_0x3c.z, -10.0f, -0.15f);
    mPos += field_0x3c;
    bool atGround;
    if (mPos.z <= 0.0f) {
        mPos.z = 0.0f;
        field_0x3c.z = 0.0f;
        atGround = true;
    } else {
        atGround = false;
    }
    return atGround;
}

// FIXME: 99.88%. reloc-name: exact instructions match target.
cls_8049D020::cls_8049D020()
    : field_0x230(0), field_0x234(0), field_0x238(0), field_0x23c(-1), field_0x240(0),
      field_0x244(0), field_0x248(0), field_0x24c(0), field_0x250(0), field_0x254(0),
      field_0x258(0), field_0x25c(0), field_0x260(1.0f), field_0x264(0.0f),
      field_0x268(0.0f), field_0x26c(1.0f), field_0x270(1.0f), mSprite(),
      mRail(), field_0x374(NULL), field_0x378(NULL), field_0x37c(NULL) {}

cls_8049D020::~cls_8049D020() {
    releaseEmitter_801EE434(field_0x374);
    field_0x374 = NULL;
    releaseEmitter_801EE434(field_0x378);
    field_0x378 = NULL;
    releaseEmitter_801EE434(field_0x37c);
    field_0x37c = NULL;
}

// FIXME: 99.80%. reloc-name: exact instructions match target.
void cls_8049D020::init() {
    ActorBase_804A5770::init();
    field_0x234 = field_0x90 & 0xF;
    switch ((s32)((field_0x90 >> 4) & 0xF)) {
    case 1:
        field_0x260 = 2.0f;
        field_0x54.set(field_0x260, field_0x260, field_0x260);
        break;
    }
    field_0x258 = (field_0x90 >> 8) & 0xFF;
    field_0x25c = (field_0x90 >> 16) & 0xFF;
    if (field_0x258 > 0 && field_0x260 == 2.0f) {
        mBox.i.x = -16.0f;
        mBox.i.y = -16.0f;
        mBox.f.x = 16.0f;
        mBox.f.y = 16.0f;
    } else {
        mBox.i.x = -8.0f;
        mBox.i.y = -8.0f;
        mBox.f.x = 8.0f;
        mBox.f.y = 8.0f;
    }
    mBox2.i.x = mBox.i.x;
    mBox2.i.y = mBox.i.y;
    mBox2.f.x = mBox.f.x;
    mBox2.f.y = mBox.f.y;
    field_0x80.i.x = mBox.i.x;
    field_0x80.i.y = mBox.i.y;
    field_0x80.f.x = mBox.f.x;
    field_0x80.f.y = mBox.f.y;
    mLife = 1000;
    field_0xb8 = 1;
    field_0xf8 = 2;
    field_0xbc = 3;
    field_0x1a4 = 1;
    field_0x118 = 2;
    field_0x114 = 2;
    field_0x240 = field_0x98;
    field_0xb0 = 0x0A040304;
    mSprite.fn_801D2608(lbl_80539EE8, 2);
    mSprite.startAnim_801D22DC(0, 2, false);
    mSprite.fn_801D1F14(0.0f);
    if (field_0x258 > 0) {
        fn_8010C294(3);
    }
}

// FIXME: 98.85%. missing/extra + reloc-name: ours has 3 more instrs (713 vs 710): a mov/clrlwi pair and a lwz in cases 2 and 4, plus swapped stack slots and registers.
void cls_8049D020::update() {
    ActorBase_804A5770::update();
    field_0x26c = 1.0f;
    JGeometry::TBox2<f32> box = mBox;
    box.i.x += mPos.x;
    box.f.x += mPos.x;
    box.i.y += mPos.y;
    box.f.y += mPos.y;
    if (field_0x238 != 12 && field_0x238 != 13) {
        JGeometry::TVec2<f32> playerPos;
        if (fn_8023AB94(&box, field_0x4, &playerPos) >= 0) {
            fn_8010C294(12);
        }
    }
    switch (field_0x238) {
    case 0: {
        fn_801F2DB4('NULL', 'NULL', NULL);
        field_0x26c = 0.0f;
        u16 angle;
        s32 dir;
        f32 dist;
        s32 player;
        fn_801EEC98(mSerial, &angle, &dir, &dist, &player, 0, 0);
        if (player >= 0 && dist < 64.0f) {
            field_0x23c = player;
            fn_8010C294(1);
        }
        break;
    }
    case 1:
        field_0x26c = 0.8f;
        field_0x24c++;
        if (field_0x24c >= 50) {
            fn_8010C294(2);
        }
        break;
    case 2:
        if (field_0x23c >= 0) {
            u16 angle = 0;
            s32 dir = 0;
            s32 player = 0;
            f32 dist = 0.0f;
            fn_801EEC98(mSerial, &angle, &dir, &dist, &player, field_0x23c, 2);
            s32 step;
            if (JMath::sincosTable_.cosShort((s16)(field_0x230 - angle)) >= 0.0f) {
                step = (s16)((s32)fabs(300.0f * JMath::sincosTable_.sinShort((s16)(field_0x230 - angle))) + 100);
            } else {
                step = 700;
            }
            fn_801CD930(&field_0x230, (s16)angle, step);
            if (dist >= 64.0f) {
                fn_801CD950(&field_0x264, 1.6f, 0.08f);
            } else {
                fn_801CD950(&field_0x264, 1.2f, 0.03f);
            }
            field_0x3c.x = field_0x264 * JMath::sincosTable_.sinShort(field_0x230);
            field_0x3c.y = field_0x264 * JMath::sincosTable_.cosShort(field_0x230);
            field_0x3c.z = 0.0f;
            mPos += field_0x3c;
            field_0x26c = PSVECMag(&field_0x3c) / 1.6f;
            if (field_0x26c >= 1.0f) {
                field_0x26c = 1.0f;
            }
            updateTrailEmitter();
            startSound_801F0E34(0x3007B, 0);
        }
        field_0x260 += (0.002f * field_0x264) / 1.6f;
        if (field_0x260 >= 2.0f) {
            field_0x260 = 2.0f;
        }
        f32 boxHalf = 8.0f * field_0x260;
        mBox.i.x = -boxHalf;
        mBox.i.y = -boxHalf;
        mBox.f.x = boxHalf;
        mBox.f.y = boxHalf;
        mBox2.i.x = mBox.i.x;
        mBox2.i.y = mBox.i.y;
        mBox2.f.x = mBox.f.x;
        mBox2.f.y = mBox.f.y;
        field_0x80.i.x = mBox.i.x;
        field_0x80.i.y = mBox.i.y;
        field_0x80.f.x = mBox.f.x;
        field_0x80.f.y = mBox.f.y;
        fn_801F2DB4('NULL', 'NULL', NULL);
        if (fn_801F2B7C(-100, -1, 1)) {
            fn_8010C294(7);
        }
        {
            JGeometry::TVec3f unkVec;
            if (fn_801F2618(&unkVec, false)) {
                fn_8010C294(5);
            }
        }
        break;
    case 3:
        mRail.fn_800FE3EC(field_0x258, mPos, field_0x4);
        fn_8010C294(4);
        break;
    case 4:
        if (field_0x258 > 0) {
            u16 angle = 0;
            JGeometry::TVec3f outStep(0.0f, 0.0f, 0.0f);
            s32 layer = field_0x4;
            JGeometry::TVec3f pos = mPos;
            mRail.fn_800FDEA0(&pos, 1.5f, &outStep, &layer, &angle, 0x200);
            field_0x3c.x = outStep.x;
            field_0x3c.y = outStep.y;
            field_0x3c.z = outStep.z;
            mPos += field_0x3c;
            updateTrailEmitter();
            startSound_801F0E34(0x3007B, 0);
        }
        fn_801F2DB4('NULL', 'NULL', NULL);
        fn_801F2B7C(-100, -1, 1);
        break;
    case 5: {
        if (fallStep()) {
            fn_8010C294(6);
        }
        break;
    }
    case 6:
        field_0x26c = 0.0f;
        fn_801F2DB4('NULL', 'NULL', NULL);
        fn_80239914(&box, field_0x4, mSerial);
        field_0x244++;
        if (field_0x244 >= 180) {
            fn_8010C294(0);
        }
        break;
    case 7: {
        field_0x248++;
        if (field_0x248 > 60) {
            fn_801F2B7C(-100, -1, 1);
            field_0x248 = 60;
        }
        JGeometry::TVec3f unkVec;
        if (fn_801F2618(&unkVec, false)) {
            fn_8010C294(5);
            break;
        }
        if (fn_801CD950(&field_0x264, 0.1f, 0.015f) == 0) {
            fn_8010C294(2);
        }
        field_0x3c.x = field_0x264 * JMath::sincosTable_.sinShort(field_0x230);
        field_0x3c.y = field_0x264 * JMath::sincosTable_.cosShort(field_0x230);
        field_0x3c.z = 0.0f;
        mPos += field_0x3c;
        updateTrailEmitter();
        break;
    }
    case 8:
        field_0x26c = 0.0f;
        if (field_0xc4 != 0) {
            JGeometry::TVec3f* pPos = fn_8023E7B0(field_0xc8);
            mPos.x = pPos->x;
            mPos.y = pPos->y;
            mPos.z = pPos->z;
            field_0x4 = getPlayerLayer_8023DE58(field_0xc8);
        } else {
            fn_8010C294(9);
        }
        break;
    case 9: {
        JGeometry::TVec3f unkVec;
        if (fn_801F2B7C(field_0x114, field_0x23c, 1) || fn_801F2B20(0) || fn_801F2618(&unkVec, false)) {
            fn_8010C294(10);
            break;
        }
        if (fallStep()) {
            fn_8010C294(10);
        }
        break;
    }
    case 10: {
        fn_80239914(&box, field_0x4, mSerial);
        if (mSprite.fn_801D21F0()) {
            if (fallStep()) {
                fn_8010C294(11);
            }
        }
        break;
    }
    case 11:
        fn_80239914(&box, field_0x4, mSerial);
        break;
    case 12:
        mDrawEnabled = false;
        fn_80239914(&box, field_0x4, mSerial);
        field_0x250++;
        if (field_0x250 >= 90) {
            fn_8010C294(13);
        }
        break;
    case 13:
        break;
    }
    field_0x54.set(field_0x260, field_0x260, field_0x260);
    mSprite.mFrameCtrl.setRate(field_0x26c);
    mSprite.fn_801D1C18();
}

// FIXME: 95.71%. missing/extra + reloc-name: ours has 1 fewer instr (215 vs 216); the scale matrices are built with different float register assignment and the frame/pos/res locals are scheduled differently.
void cls_8049D020::draw() {
    ActorBase_804A5770::draw();
    SpriteParams_801F5AE0 params;
    SpriteParams_801F5AE0 params2;
    Mtx mtx1;
    Mtx mtx2;
    if (!field_0x54.isZero()) {
        setScaleMtx(mtx1, field_0x54, 0.0f);
        params2.mMtx = mtx1;
        setScaleMtx(mtx2, field_0x54, 4.0f * field_0x260);
        params.mMtx = mtx2;
    }
    params.field_0x10.a = 0x80;
    s32 frame = mSprite.mCel + 0x165;
    fn_801EF830(field_0x4, &mPos, &params, 0x407, "enemy_gc_sob.bin", frame, NULL);
    if (field_0x4 < 8) {
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_gc_sob.bin");
        gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, &params2, 1.0f, 1.0f);
    }
}

// FIXME: 90.19%. missing/extra + reloc-name: ours has 2 fewer instrs (274 vs 276): the original keeps the table, the actor manager tag and the serial in r27-r29 (stmw/lmw r27) and negates case 5 through two stack TVec3f copies; the single-case switch in case 13 has an extra bge in the original.
bool cls_8049D020::fn_8010C294(s32 state) {
    switch (state) {
    case 1:
        field_0x24c = 0;
        field_0x254 = 5;
        break;
    case 2: {
        field_0x248 = 0;
        field_0x24c = 0;
        u16 outAngle = 0;
        s32 outDir = 0;
        s32 outPlayer = 0;
        f32 outDist = 0.0f;
        fn_801EEC98(mSerial, &outAngle, &outDir, &outDist, &outPlayer, field_0x23c, 2);
        field_0x230 = outAngle;
        break;
    }
    case 3:
        mRail.clear();
        break;
    case 4:
        field_0x264 = 1.5f;
        if (field_0x258 > 0 && field_0x25c > 0) {
            u8 v = field_0x25c;
            if (v < mRail.field_0x2) {
                mRail.field_0x3 = v;
            }
        }
        break;
    case 5:
        field_0x3c.negate();
        field_0x3c.z = 1.0f;
        getGameView_802D6A9C()->fn_802D79E8(field_0x4, 4.0f, 0.0f, false);
        if (field_0x374 != NULL) {
            field_0x374->stopCreateParticle();
        }
        break;
    case 6:
        field_0x264 = 0.0f;
        field_0x244 = 0;
        break;
    case 7:
        field_0x248 = 0;
        break;
    case 8:
        field_0x23c = field_0xc8;
        field_0x264 = 0.0f;
        field_0x3c.zero();
        break;
    case 10:
        field_0x3c.x *= -0.1f;
        field_0x3c.y *= -0.1f;
        field_0x3c.z = 0.0f;
        field_0xd8 = 0;
        mSprite.startAnim_801D22DC(1, 0, false);
        if (field_0x4 == 0) {
            field_0x378 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x93, 0, -1, NULL, NULL, true, field_0x260);
            field_0x37c = spawnEmitter_8013CC50(field_0x4, &mPos, 0x94, 1, 0, NULL, NULL, true, field_0x260);
        }
        break;
    case 12: {
        deleteEmitter(&mEmitter100);
        deleteEmitter(&mEmitter104);
        if (field_0x374 != NULL) {
            field_0x374->stopCreateParticle();
        }
        JGeometry::TVec3f pos = mPos;
        pos.y -= 16.0f * field_0x260;
        spawnEmitter_8013CC50(field_0x4, &pos, 0x44A, 0, 1, NULL, NULL, true, field_0x260);
        spawnEmitter_8013CC50(field_0x4, &pos, 0x44B, 0, 1, NULL, NULL, true, field_0x260);
        spawnEmitter_8013CC50(field_0x4, &pos, 0x44C, 0, 1, NULL, NULL, true, field_0x260);
        startSound_801F0E34(0x50077, 0);
        field_0x270 = field_0x260;
        break;
    }
    case 13:
        field_0x3c.zero();
        mPos.z = 0.0f;
        switch (field_0x234) {
        case 1:
            fn_801F0D20(3);
            getActorManager_801F856C()->spawnActor_801F9484('KEY0', field_0x4, &mPos, 0x6000, -1, -1);
            break;
        default: {
            s32 kind = 0;
            if (field_0x270 >= 2.0f) {
                kind = 2;
            } else if (field_0x270 >= 1.4f) {
                kind = 1;
            }
            if (field_0x258 >= 1) {
                kind = 1;
            }
            ActorManager_804A5800* mgr = getActorManager_801F856C();
            s32 serial = mgr->fn_801F8544('RUPY', field_0x4, &mPos, lbl_80464328[kind]);
            if (serial >= 0) {
                mgr->findActorBySerial_801F666C(serial);
            }
            break;
        }
        }
        fn_801F3668();
        break;
    }
    field_0x238 = state;
    mSprite.fn_801D1F14(0.0f);
    return true;
}
