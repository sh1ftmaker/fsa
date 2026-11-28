#include "game/GameAudio.h"
#include "game/cls_8049D740.h"

#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Camera.h"
#include "game/EnemyBase.h"
#include "game/GameGlobals.h"
#include "game/GameRootSub20.h"
#include "game/Message.h"
#include "game/Player.h"
#include "game/PlayerBit.h"
#include "game/RotMatrix.h"
#include "JSystem/JMath/JMath.h"

// The Maiden's animation keys: (cel, frames) s16 pairs, ended by a cel of -1.
static const s16 lbl_8053C600[4] = {6, 20, -1, -1};
static const s16 lbl_8053C608[4] = {7, 20, -1, -1};
static const s16 lbl_8053C610[4] = {8, 20, -1, -1};
static const s16 lbl_8053C618[4] = {9, 20, -1, -1};
static const s16 lbl_8053C620[4] = {8, 9, -1, -1};
static const s16 lbl_80464678[14] = {0, 9, 1, 9, 2, 9, 3, 9, 4, 9, 5, 9, -1, -1};
static const s16 lbl_80464694[10] = {8, 5, 0x15, 6, 0x16, 6, 0x17, 60, -1, -1};
static const s16 lbl_804646A8[8] = {8, 9, 0xC, 9, 0xD, 40, -1, -1};
static const s16 lbl_804646B8[14] = {0xE, 9, 0xF, 9, 0x10, 9, 0x11, 9, 0x12, 9, 0x13, 9, -1, -1};
static const s16 lbl_804646D4[8] = {8, 6, 0x14, 9, 0xE, 9, -1, -1};
static const s16 lbl_804646E4[8] = {8, 1, 0xA, 30, 0xB, 10, -1, -1};
static const s16 lbl_804646F4[8] = {0x13, 9, 0x14, 9, 8, 40, -1, -1};

// The key tables of mAnim, in address order.
static const s16* lbl_8049D710[12] = {
    lbl_80464678, lbl_8053C600, lbl_8053C608, lbl_8053C610, lbl_8053C618, lbl_8053C620,
    lbl_80464694, lbl_804646A8, lbl_804646B8, lbl_804646D4, lbl_804646E4, lbl_804646F4,
};

// The Maiden's base sprite frames (modes 0..5), demo frame table (0x8046471C),
// then one per-mode offset table (modes 0..5) for each of three checks.
static const s32 lbl_80464704[6] = {
    0x199, 0x1E7, 0x17F, 0x201, 0x1CD, 0x1B3,
};
static const s32 lbl_8046471C[16] = {
    0, 0, 0, 0x276, 0x2B2, 0x47E, 0x76C, 0x83E, 0x848, 0xA3C,
    0xAF0, 0xC9E, 0xD5C, 0xD66, 0xE24, 0,
};
static const s32 lbl_8046475C[6] = {0xA, 0x1E, 0x32, 0, 0x14, 0x28};
static const s32 lbl_80464774[6] = {0x14, 0x37, 0x4B, 0, 0x28, 0x41};
static const s32 lbl_8046478C[6] = {5, 0xA, 0, 0, 0xF, 0x14};
static const s32 lbl_804647A4[3] = {0x0235023D, 0x02340236, 0x02370238};

MaidenCutscene_8049D740::MaidenCutscene_8049D740()
    : field_0x230(0),
      mMode(0),
      field_0x238(0),
      field_0x23c(0),
      field_0x240(-1),
      field_0x244(0),
      field_0x248(0.0f),
      field_0x24c(0.0f),
      field_0x250(0.37f),
      mEmitter298(NULL),
      mEmitter29c(NULL),
      mEmitter2a0(NULL) {
}

MaidenCutscene_8049D740::~MaidenCutscene_8049D740() {
    releaseEmitter_801EE434(mEmitter298);
    mEmitter298 = NULL;
    releaseEmitter_801EE434(mEmitter29c);
    mEmitter29c = NULL;
    releaseEmitter_801EE434(mEmitter2a0);
    mEmitter2a0 = NULL;
}

void MaidenCutscene_8049D740::init() {
    DemoActorBase_8049D140::init();
    mMode = mSpawnParam_0x90 & 0x1F;
    mAnim.setKeyTables_801D2608(lbl_8049D710, 12);
    mAnim.startAnim_801D22DC(5, 2, false);
    mAnim.setFrame_801D1F14(0.0f);
}

void MaidenCutscene_8049D740::vfunc_0x10() {
    s32 serial = getActorManager_801F856C()->findSerialByActorId_801F7A08(ACTOR_DMGL);
    ActorBase_804A5770* goal = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
    if (goal != NULL) {
        mGoalPos = goal->mPos;
    }
}

// FIXME: 97.94 %, scheduling+regalloc; behaviour drift.
void MaidenCutscene_8049D740::update() {
    DemoActorBase_8049D140::update();
    JGeometry::TBox2f box = mBox;
    box.i.x += mPos.x;
    box.i.y += mPos.y;
    box.f.x += mPos.x;
    box.f.y += mPos.y;
    if (field_0x238 == 0 || field_0x238 == 1) {
        fn_80239914(&box, mLayer_0x4, mSerial);
    }
    if (field_0x238 >= 2 && field_0x238 <= 5) {
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
    }
    switch (field_0x238) {
    case 0: {
        fn_8022FEB8(&box, mLayer_0x4, mSerial, true, false);
        s32 player = fn_8022FD8C(mSerial);
        if (player >= 0 && player < 4 && isPlayerBitSet(gGameRoot_8053AAF8, player)) {
            JGeometry::TVec3f pos = mPos;
            pos.y += -32.0f;
            if (openMessage_8038AD24(player, 0x23E, mSerial, mLayer_0x4, pos, 0x20)) {
                field_0x240 = player;
                fn_80112654(1);
            }
        }
        break;
    }
    case 1:
        fn_8022FF8C(field_0x240, &box, mLayer_0x4, mSerial, true, false);
        if (fn_80388CAC(field_0x240, mSerial)) {
            field_0x240 = -1;
            fn_80112654(0);
        }
        break;
    case 3:
        if (mAnim.isAnimEnd_801D208C() && mAnim.mAnim == 0xA) {
            mAnim.startAnim_801D22DC(0, 2, false);
            mAnim.setFrame_801D1F14(0.0f);
        }
        break;
    case 4:
        if (mAnim.isAnimEnd_801D208C() && mAnim.mAnim == 0xA) {
            mAnim.startAnim_801D22DC(0, 2, false);
            mAnim.setFrame_801D1F14(0.0f);
        }
        break;
    case 12:
        if (moveToGoal()) {
            fn_80112654(0xD);
        }
        break;
    case 13:
        pool_8053C628();
        break;
    case 15:
        mDrawEnabled = false;
        break;
    }
    mAnim.update_801D1C18();
    if (field_0x230 != 0) {
        fn_80112AE8();
    }
}

void MaidenCutscene_8049D740::draw() {
    DemoActorBase_8049D140::draw();
    if (field_0x230 == 0) {
        s32 baseFrame = lbl_80464704[mMode];
        SpriteParams_801F5AE0 params;
        drawSprite_801F06F0("enemy_gc_sob.bin", baseFrame + mAnim.mCel, &params, 7, 1.0f, 1.0f);
    }
}

// FIXME: 98.43 %, regalloc-only; behaviour clean.
void MaidenCutscene_8049D740::vfunc_0x54() {
    s32 id = fn_8011921C()->field_0x8;
    switch (field_0x23c) {
    case 0:
        if (id == lbl_8046471C[2]) {
            fn_80112654(2);
        } else if (id == lbl_8046471C[3] + lbl_8046475C[mMode]) {
            fn_80112654(3);
        } else if (id == lbl_8046471C[4]) {
            fn_80112654(4);
        } else if (id == lbl_8046471C[5]) {
            fn_80112654(5);
        } else if (id == lbl_8046471C[6]) {
            fn_80112654(6);
        } else if (id == lbl_8046471C[7]) {
            fn_80112654(7);
        } else if (id == lbl_8046471C[8]) {
            fn_80112654(8);
        } else if (id == lbl_8046471C[9] + lbl_8046478C[mMode]) {
            fn_80112654(9);
        } else if (id == lbl_8046471C[10]) {
            fn_80112654(0xA);
        } else if (id == lbl_8046471C[11]) {
            fn_80112654(0xB);
        } else if (id == lbl_8046471C[12] + lbl_80464774[mMode]) {
            fn_80112654(0xC);
        } else if (id == lbl_8046471C[14]) {
            fn_80112654(0xF);
        }
        break;
    }
}

s32 MaidenCutscene_8049D740::vfunc_0x58() {
    return field_0x238;
}

u8 MaidenCutscene_8049D740::vfunc_0x5C() {
    return 1;
}

// Not auto-inlined: update() calls fn_80112AE8 out of line in the original.
#pragma auto_inline off
// FIXME: 99.89 %, scheduling; behaviour clean.
// FIXME: lwz of field_0x244 and mMode are swapped.
void MaidenCutscene_8049D740::fn_80112AE8() {
    s32 mode;
    field_0x248 += 0.01f;

    Mtx mtx1;
    PSMTXTrans(mtx1, mPos.x, mPos.y, mPos.z);
    JGeometry::TVec3f axis1(1.0f, 0.0f, 0.0f);
    makeAxisRotation_8010F668(mtx1, &axis1, 0.08726646f);

    Mtx mtx2;
    mtx2[0][0] = mtx2[1][0] = mtx2[2][0] = mtx2[0][1] = mtx2[1][1] = mtx2[2][1] = mtx2[0][2] = mtx2[1][2] =
        mtx2[2][2] = mtx2[0][3] = mtx2[1][3] = mtx2[2][3] = 0.0f;
    JGeometry::TVec3f axis2(0.0f, 0.0f, 1.0f);
    makeAxisRotation_8010F668(mtx2, &axis2, field_0x248);

    PSMTXConcat(mtx1, mtx2, mtx1);

    field_0x244++;
    if (field_0x244 >= 200) {
        field_0x244 = 0;
    }

    s32 frame = field_0x244;
    mode = mMode;
    f32 scale = field_0x250;
    GameRootSub20_801902C0::Crystal& crystal = fn_801902C0()->mCrystals[mode];
    if (crystal.mModel != NULL && !crystal.mActive) {
        crystal.mActive = 1;
        crystal.mPos.set(scale, scale, scale);
        JMath::gekko_ps_copy12(crystal.mMtx, mtx1);
        crystal.field_0x48 = -1.0f;
        crystal.field_0x4c = frame;
        crystal.field_0x54 = 0;
        crystal.field_0x58 = 1;
    }
}
#pragma auto_inline reset

void MaidenCutscene_8049D740::fn_80112654(s32 kind) {
    switch (kind) {
    case 0:
    case 1:
    case 2:
        break;
    case 3:
        mEmitter298 = spawnEmitter_8013CC50(mLayer_0x4, &mPos, fn_801123C8(0, mMode), 0, 0, NULL, NULL, true, 1.0f);
        mEmitter29c = spawnEmitter_8013CC50(mLayer_0x4, &mPos, fn_801123C8(1, mMode), 0, 0, NULL, NULL, true, 1.0f);
        mAnim.startAnim_801D22DC(0xA, 0, false);
        mAnim.setFrame_801D1F14(0.0f);
        break;
    case 4:
        spawnEmitter_8013CC50(mLayer_0x4, &mPos, fn_801123C8(2, mMode), 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(mLayer_0x4, &mPos, fn_801123C8(3, mMode), 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(mLayer_0x4, &mPos, fn_801123C8(4, mMode), 0, 0, NULL, NULL, true, 1.0f);
        mEmitter2a0 = spawnEmitter_8013CC50(mLayer_0x4, &mPos, fn_801123C8(5, mMode), 0, 0, NULL, NULL, true, 1.0f);
        break;
    case 5:
        mAnim.startAnim_801D22DC(8, 2, false);
        mAnim.setFrame_801D1F14(0.0f);
        break;
    case 6:
        if (mEmitter298 != NULL) {
            mEmitter298->becomeInvalidEmitter();
            mEmitter298 = NULL;
        }
        if (mEmitter29c != NULL) {
            mEmitter29c->becomeInvalidEmitter();
            mEmitter29c = NULL;
        }
        if (mEmitter2a0 != NULL) {
            mEmitter2a0->becomeInvalidEmitter();
            mEmitter2a0 = NULL;
        }
        mAnim.startAnim_801D22DC(0xB, 0, false);
        mAnim.setFrame_801D1F14(0.0f);
        break;
    case 7: {
        JGeometry::TVec3f pos = mPos;
        pos.y += -16.0f;
        spawnEmitter_8013CC50(mLayer_0x4, &pos, 0x33B, 0, 0, NULL, NULL, true, 1.0f);
        break;
    }
    case 8:
        switch (mMode) {
        case 0:
            mAnim.startAnim_801D22DC(1, 2, false);
            break;
        case 1:
            mAnim.startAnim_801D22DC(1, 2, false);
            break;
        case 2:
            mAnim.startAnim_801D22DC(3, 2, false);
            break;
        case 3:
            mAnim.startAnim_801D22DC(3, 2, false);
            break;
        case 4:
            mAnim.startAnim_801D22DC(2, 2, false);
            break;
        case 5:
            mAnim.startAnim_801D22DC(2, 2, false);
            break;
        }
        mAnim.setFrame_801D1F14(0.0f);
        break;
    case 9:
        mAnim.startAnim_801D22DC(3, 2, false);
        mAnim.setFrame_801D1F14(0.0f);
        break;
    case 10:
        mAnim.startAnim_801D22DC(6, 0, false);
        mAnim.setFrame_801D1F14(0.0f);
        spawnEmitter_8013CC50(mLayer_0x4, &mPos, 0xC1, 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(mLayer_0x4, &mPos, 0xC2, 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(mLayer_0x4, &mPos, 0xC3, 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(mLayer_0x4, &mPos, 0xC4, 0, 0, NULL, NULL, true, 1.0f);
        break;
    case 11:
        field_0x230 = 1;
        break;
    case 12:
    case 13:
    case 14:
    case 15:
        break;
    }
    field_0x238 = kind;
}

// Maps (kind, mode) to a message id. Each kind has its own mode table.
u16 fn_801123C8(s32 kind, s32 mode) {
    switch (kind) {
    case 0:
        switch (mode) {
        case 0: return 0x2FD;
        case 1: return 0x307;
        case 2: return 0x301;
        case 3: return 0x2FF;
        case 4: return 0x305;
        case 5:
        default: return 0x303;
        }
    case 1:
        switch (mode) {
        case 0: return 0x2FE;
        case 1: return 0x308;
        case 2: return 0x302;
        case 3: return 0x300;
        case 4: return 0x306;
        case 5:
        default: return 0x304;
        }
    case 2:
        switch (mode) {
        case 0: return 0x2CF;
        case 1: return 0x2DE;
        case 2: return 0x2D5;
        case 3: return 0x2D2;
        case 4: return 0x2DB;
        case 5:
        default: return 0x2D8;
        }
    case 3:
        switch (mode) {
        case 0: return 0x2D0;
        case 1: return 0x2DF;
        case 2: return 0x2D6;
        case 3: return 0x2D3;
        case 4: return 0x2DC;
        case 5:
        default: return 0x2D9;
        }
    case 4:
        switch (mode) {
        case 0: return 0x2D1;
        case 1: return 0x2E0;
        case 2: return 0x2D7;
        case 3: return 0x2D4;
        case 4: return 0x2DD;
        case 5:
        default: return 0x2DA;
        }
    case 5:
    default:
        switch (mode) {
        case 0: return 0x2E4;
        case 1: return 0x2E9;
        case 2: return 0x2E6;
        case 3: return 0x2E5;
        case 4: return 0x2E8;
        case 5:
        default: return 0x2E7;
        }
    }
}

// Plain helpers inlined into update() (cases 12 and 13) whose out-of-line
// copies the linker dropped; defined at the bottom so deferred inlining pools
// their .sdata2 constants before draw().
// FAKEMATCH(crack-pass): pool helper failing R2 (1 call site, 15-line definition): split out only where the .sdata2 order needs it.
bool MaidenCutscene_8049D740::moveToGoal() {
    field_0x248 += 0.04f;
    moveToward_801CD950(&field_0x24c, 2.0f, 0.05f);
    s16 angle = (s32)(JMath::atanTable_.atan2_(mGoalPos.x - mPos.x, mGoalPos.y - mPos.y) *
                      10430.378f);
    mVel_0x3c.x = field_0x24c * JMath::sincosTable_.sinShort(angle);
    mVel_0x3c.y = field_0x24c * JMath::sincosTable_.cosShort(angle);
    s32 doneX = moveToward_801CD950(&mPos.x, mGoalPos.x, mVel_0x3c.x);
    s32 doneY = moveToward_801CD950(&mPos.y, mGoalPos.y, mVel_0x3c.y);
    moveToward_801CD950(&field_0x250, 0.05f, -0.005f);
    if (doneX == 0 && doneY == 0) {
        return true;
    }
    return false;
}

// R2 pool helper (one-statement body); was shrinkCrystal.
void MaidenCutscene_8049D740::pool_8053C628() {
    moveToward_801CD950(&field_0x250, 0.0f, -0.02f);
}
