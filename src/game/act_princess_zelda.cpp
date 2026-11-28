#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/EnemyBase.h"
#include "game/GameGlobals.h"
#include "game/GameMath.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Message.h"
#include "game/Player.h"
#include "game/act/act_life_meter.h"
#include "game/act/act_princess_zelda.h"
#include "JSystem/JMath/JMath.h"

static u8 lbl_8053AA80;
static u8 lbl_8053AA81;

// .rodata
static const s16 lbl_80464060[] = {
    0, 3, 1, 3, 2, 3, 3, 3, 4, 3, 5, 3, 6, 3, 7, 3, -1, -1
};
static const s16 lbl_80464084[] = {
    8, 3, 9, 3, 10, 3, 11, 3, 12, 3, 13, 3, 14, 3, 15, 3, -1, -1
};
static const s16 lbl_804640A8[] = {
    16, 3, 17, 3, 18, 3, 19, 3, 20, 3, 21, 3, 22, 3, 23, 3, -1, -1
};
static const s16 lbl_804640CC[] = {
    24, 3, 25, 3, 26, 3, 27, 3, 28, 3, 29, 3, 30, 3, 31, 3, -1, -1
};
static const s16 lbl_804640F0[] = {
    16, 6, 8, 6, 0, 6, 24, 6, 16, 6, 8, 6, 0, 6, 24, 6, 32, 3, 33, 5, 34, 6, 35, 7, 36, 30, -1, -1
};

// .sdata2 small tables:
static const s16 lbl_8053C380[] = { 0, 3, -1, -1 };
static const s16 lbl_8053C388[] = { 8, 3, -1, -1 };
static const s16 lbl_8053C390[] = { 16, 3, -1, -1 };
static const s16 lbl_8053C398[] = { 24, 3, -1, -1 };

static const s16 lbl_8053C3A0[] = { 0, 3, -1, -1 };
static const s16 lbl_8053C3A8[] = { 8, 3, -1, -1 };
static const s16 lbl_8053C3B0[] = { 16, 3, -1, -1 };
static const s16 lbl_8053C3B8[] = { 24, 3, -1, -1 };

static const s16 lbl_8053C3C0[] = { 0, 7, -1, -1 };
static const s16 lbl_8053C3C8[] = { 3, 7, -1, -1 };
static const s16 lbl_8053C3D0[] = { 6, 7, -1, -1 };
static const s16 lbl_8053C3D8[] = { 9, 7, -1, -1 };

static const s16 lbl_80464128[] = {
    0, 20, 8, 20, 0, 20, 8, 20, 16, 3, -1, -1
};
static const s16 lbl_80464140[] = {
    1, 10, 0, 7, 2, 10, 0, 7, -1, -1
};
static const s16 lbl_80464154[] = {
    4, 10, 3, 7, 5, 10, 3, 7, -1, -1
};
static const s16 lbl_80464168[] = {
    7, 10, 6, 7, 8, 10, 6, 7, -1, -1
};
static const s16 lbl_8046417C[] = {
    10, 10, 9, 7, 11, 10, 9, 7, -1, -1
};
static const s16 lbl_80464190[] = {
    20, 8, 21, 8, 22, 8, 23, 8, 20, 8, 21, 8, 22, 8, 23, 8, 24, 5, 25, 20, -1, -1
};
static const s16 lbl_804641BC[] = {
    0, 20, 3, 20, 0, 20, 3, 20, 6, 3, -1, -1
};
static const s16 lbl_804641D4[] = { 12, 10, 13, 15, -1, -1 };
static const s16 lbl_804641E0[] = { 14, 10, 15, 15, -1, -1 };
static const s16 lbl_804641EC[] = { 16, 10, 17, 15, -1, -1 };
static const s16 lbl_804641F8[] = { 18, 10, 19, 15, -1, -1 };

static const u32 lbl_80464204[] = {
    'PEAC', 'OKTA', 'AMOS', 'BZBR', 'OKTM', 'DGTL', 'HINO', 'HBMB', 'SLR1', 'TKTA',
    'SLR2', 'KETH', 'FLME', 'TKYU', 'TRAP', 'ARRW', 'Poon', 'SKUL', 'SLR3', 'POPO',
    'SBAL', 'SLR4', 'FIRE', 'ROPE', 'NZMI', 'OIWA', 'PAME', 'STAL', 'MIRA', 'WIZR',
    'ZORA', 'ZOSH', 'BMTY', 'PUKU', 'RIVR', 'GDMN', 'FAGN', 'TNDR', 'WNWN', 'FLYO',
    'LTZR', 'BEEE', 'NULL'
};

static const u16 lbl_804642B0[8] = {
    0x2C, 0x2D, 0x2E, 0x2F, 0x28, 0x29, 0x2A, 0x2B
};

// .data tables of pointers
static const s16* lbl_8049CE70[14] = {
    lbl_80464060, lbl_80464084, lbl_804640A8, lbl_804640CC, lbl_804640F0,
    lbl_8053C380, lbl_8053C388, lbl_8053C390, lbl_8053C398,
    lbl_80464128, lbl_8053C3A0, lbl_8053C3A8, lbl_8053C3B0, lbl_8053C3B8,
};

static const s16* lbl_8049CEA8[14] = {
    lbl_80464140, lbl_80464154, lbl_80464168, lbl_8046417C, lbl_80464190,
    lbl_8053C3C0, lbl_8053C3C8, lbl_8053C3D0, lbl_8053C3D8,
    lbl_804641BC, lbl_804641D4, lbl_804641E0, lbl_804641EC, lbl_804641F8,
};

PrincessZelda_8049CEE0::PrincessZelda_8049CEE0()
    : field_0x2f8(0),
      field_0x2fc(0),
      field_0x300(-1),
      field_0x304(-1),
      field_0x308(0),
      field_0x30c(0),
      field_0x310(-1),
      field_0x314(0),
      field_0x31c(-1),
      field_0x320(0),
      field_0x324(0),
      field_0x328(0),
      field_0x32c(0),
      field_0x330(1.0f),
      field_0x334(0.0f),
      field_0x338(1),
      field_0x339(1) {}

PrincessZelda_8049CEE0::~PrincessZelda_8049CEE0() {
    lbl_8053AA81 = 0;
}

void PrincessZelda_8049CEE0::init() {
    ActorBase_804A5770::init();
    field_0x320 = mSpawnParam_0x90 & 0x1F;
    field_0x324 = (mSpawnParam_0x90 >> 4) & 0x1F;
    field_0x2fc = (mSpawnParam_0x90 >> 8) & 0xFF;
    mBox.i.x = -8.0f;
    mBox.i.y = -8.0f;
    mBox.f.x = 8.0f;
    mBox.f.y = 8.0f;
    mBox2.i.x = -8.0f;
    mBox2.i.y = -8.0f;
    mBox2.f.x = 8.0f;
    mBox2.f.y = 8.0f;
    mMapBox_0x80.i.x = -8.0f;
    mMapBox_0x80.i.y = -8.0f;
    mMapBox_0x80.f.x = 8.0f;
    mMapBox_0x80.f.y = 8.0f;
    mLife = 4;
    field_0xb8 = 2;
    field_0x11e = 1;
    if (field_0x320 == 0) {
        mSpriteAnim.setKeyTables_801D2608(lbl_8049CEA8, 14);
    } else {
        mSpriteAnim.setKeyTables_801D2608(lbl_8049CE70, 14);
    }
    mSpriteAnim.startAnim_801D22DC(2, 2, 0);
    pool_8053C3E0();
    getGameView_802D6A9C()->fn_802D7440(0);
    field_0x310 = getActorManager_801F856C()->spawnActor_801F9484('LIMT', mLayer_0x4, &mPos, 0, -1, -1);
}

static inline bool checkTile(u16 code) {
    for (int i = 0; i < 8; i++) {
        if (code == lbl_804642B0[i]) {
            return true;
        }
    }
    return false;
}

inline bool PrincessZelda_8049CEE0::canBeHit() {
    if (field_0x328 >= 8 || field_0x328 <= 4) {
        return false;
    }
    if (field_0x2f8 != 0) {
        return false;
    }
    return true;
}

// The player (0-3) that takes part, stands in the box and opened message 0x170; else -1.
inline s32 PrincessZelda_8049CEE0::findTalker() {
    JGeometry::TBox2f searchBox(mBox);
    searchBox.i.x += mPos.x;
    searchBox.i.y += mPos.y;
    searchBox.f.x += mPos.x;
    searchBox.f.y += mPos.y;

    fn_8022FEB8(&searchBox, mLayer_0x4, mSerial, false, false);
    s32 player = fn_8022FD8C(mSerial);
    if (player < 0) {
        return -1;
    }
    if (!(gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << player))) {
        return -1;
    }
    if (!fn_8022C5EC(player, 0x120)) {
        return -1;
    }
    if (!openMessage_8038AD24(player, 0x170, getSerial(), getField_0x4(), mPos, 0x20)) {
        return -1;
    }
    return player;
}

// State 1 while lbl_8053AA81 is set: message 0x170 for player 0, then state 4.
inline void PrincessZelda_8049CEE0::talkToFirstPlayer() {
    if (field_0x328 != 1) {
        return;
    }
    fn_8022FE80(0, mLayer_0x4, mSerial, false, false);
    if (fn_8022FD8C(mSerial) >= 0) {
        if (openMessage_8038AD24(0, 0x170, getSerial(), getField_0x4(), mPos, 0x28)) {
            lbl_8053AA81 = 0;
            setState_801096C8(4);
        }
    }
}

inline bool PrincessZelda_8049CEE0::stepRail() {
    if (field_0x2fc <= 0) {
        return true;
    }
    s16 angle = 0;
    JGeometry::TVec3f moveVec(0.0f, 0.0f, 0.0f);
    s32 layer = mLayer_0x4;
    JGeometry::TVec3f pos = mPos;
    if (mRail.stepAlongRail_800FDEA0(&pos, 1.0f, &moveVec, &layer, (u16*)&angle, 0x200) == 3) {
        return true;
    }
    mVel_0x3c.x = moveVec.x;
    mVel_0x3c.y = moveVec.y;
    mVel_0x3c.z = moveVec.z;
    mPos += mVel_0x3c;
    s32 dir = angleToDir_801F681C(angle);
    if (dir >= 0 && dir != mSpriteAnim.mAnim) {
        mSpriteAnim.startAnim_801D22DC(dir, 2, false);
        pool_8053C3E0();
    }
    return false;
}

// The position copies for openMessage_8038AD24 are made before serial and layer are loaded (after, in the original).
void PrincessZelda_8049CEE0::update() {
    ActorBase_804A5770::update();
    field_0x330 = 1.0f;
    field_0x339 = 1;

    JGeometry::TBox2f box(mBox);
    box.i.x += mPos.x;
    box.i.y += mPos.y;
    box.f.x += mPos.x;
    box.f.y += mPos.y;

    if (canBeHit()) {
        if (findOverlappingActorOf_801F26CC(lbl_80464204) >= 0) {
            mLife -= 2;
            if (mLife > 0) {
                setState_801096C8(11);
            }
        }
    }

    if (field_0x328 >= 5 && field_0x328 <= 7) {
        s32 player = findTalker();
        if (player >= 0) {
            field_0x304 = player;
            setState_801096C8(3);
        }
    }

    if (field_0x2f8 != 0) {
        field_0x30c++;
        if (field_0x30c >= 300) {
            field_0x2f8 = 0;
            field_0x30c = 0;
        }
    }

    switch (field_0x328) {
    case 0:
        if (field_0x2fc > 0 && field_0x320 == 1) {
            mRail.fn_800FE3EC(field_0x2fc, mPos, mLayer_0x4);
        }
        setState_801096C8(1);
        break;

    case 1:
        if (lbl_8053AA81 != 0) {
            talkToFirstPlayer();
        } else {
            u16 angle;
            f32 dist;
            s32 unk;
            s32 dir;
            findTargetPlayer_801EEC98(mSerial, &angle, &dir, &dist, &unk, 0, 1);
            if (dir >= 0 && !(dist > 64.0f)) {
                switch (field_0x328) {
                case 1:
                    if (dir >= 0 && dir + 5 != mSpriteAnim.mAnim) {
                        mSpriteAnim.startAnim_801D22DC(dir + 5, 2, false);
                        pool_8053C3E0();
                    }
                    break;
                }
            }
            switch (field_0x320) {
            case 0: {
                fn_8022FEB8(&box, mLayer_0x4, mSerial, true, false);
                s32 player = fn_8022FD8C(mSerial);
                if (player >= 0 && !hasPlayerLeader_8022F514(player)) {
                    if (openMessage_8038AD24(player, 0x230, getSerial(), getField_0x4(), mPos, 0x28)) {
                        field_0x300 = player;
                        setState_801096C8(2);
                    }
                }
                break;
            }
            case 1:
                break;
            }
        }
        break;

    case 2:
        fn_8022FF8C(field_0x300, &box, mLayer_0x4, mSerial, true, false);
        if (fn_80388CAC(field_0x300, mSerial)) {
            setState_801096C8(5);
        }
        break;

    case 3:
        fn_8022FF8C(field_0x304, &box, mLayer_0x4, mSerial, true, false);
        if (fn_80388CAC(field_0x304, mSerial)) {
            setState_801096C8(field_0x32c);
        }
        break;

    case 4: {
        Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
        camera->addFocusPoint_8030C210(&mPos);
        camera = gGameRoot2_8053AB10->field_0x24;
        if (camera->mBlendBoxOn_0x840 == 0) {
            camera->mBlendBoxOn_0x840 = 1;
            camera->mBlendBox_0x844.i.x = camera->mBlendBox_0x844.f.x = mPos.x;
            camera->mBlendBox_0x844.i.y = camera->mBlendBox_0x844.f.y = mPos.y;
            camera->mBlendWeight_0x854 = 0.08f;
            camera->mMargin_0x1d4 = -1.0f;
            camera->mMinHalfWidth_0x1d8 = -1.0f;
        }
        fn_8022FE80(0, mLayer_0x4, mSerial, false, false);
        if (fn_80388CAC(0, mSerial)) {
            lbl_8053AA81 = 0;
            setState_801096C8(1);
        }
        break;
    }

    case 5: {
        if (field_0x300 < 0) {
            setState_801096C8(1);
            break;
        }
        if (isScreenFlagSet_0x34()) {
            setState_801096C8(15);
            break;
        }
        s32 player = field_0x300;
        if (player < 0) {
            break;
        }
        if (getGameView_802D6A9C()->isInTransition_802DCD0C()) {
            setState_801096C8(9);
            break;
        }
        if (fn_8022D75C(player)) {
            if (getPlayerDestLayer_8023E10C(player) == 8) {
                setState_801096C8(7);
            } else {
                mLayer_0x4 = getPlayerLayer_8023DE58(player);
                field_0x339 = 0;
                setState_801096C8(10);
            }
            break;
        }
        f32 dist;
        s32 unk;
        s32 dir;
        u16 angle = 0;
        dir = 0;
        unk = 0;
        dist = 0.0f;
        findTargetPlayer_801EEC98(mSerial, &angle, &dir, &dist, &unk, player, 2);
        stepSpeed(dist);
        mVel_0x3c.x = field_0x334 * JMath::sincosTable_.sinShort(angle);
        mVel_0x3c.y = field_0x334 * JMath::sincosTable_.cosShort(angle);
        mPos += mVel_0x3c;
        field_0x330 = magnitude(mVel_0x3c) / 1.1f;
        if (field_0x330 >= 1.0f) {
            field_0x330 = 1.0f;
        }
        if (dir >= 0 && dir != mSpriteAnim.mAnim) {
            mSpriteAnim.startAnim_801D22DC(dir, 2, false);
            pool_8053C3E0();
        }
        break;
    }

    case 6:
        break;

    case 7:
        if (field_0x300 < 0) {
            setState_801096C8(1);
        }
        if (!fn_8022D75C(field_0x300) && mLayer_0x4 == getPlayerLayer_8023DE58(field_0x300)) {
            setState_801096C8(8);
        }
        break;

    case 8:
        if (field_0x300 < 0) {
            setState_801096C8(1);
        }
        field_0x314++;
        if (field_0x314 >= 15) {
            field_0x314 = 0;
            setState_801096C8(5);
        }
        break;

    case 9:
        approachPlayerSide();
        if (!getGameView_802D6A9C()->isInTransition_802DCD0C()) {
            setState_801096C8(5);
        }
        break;

    case 10:
        mLayer_0x4 = getPlayerLayer_8023DE58(field_0x300);
        followPlayerSide();
        if (!fn_8022D75C(field_0x300)) {
            setState_801096C8(5);
        }
        break;

    case 11:
        if (mSpriteAnim.isAnimEnd_801D208C()) {
            setState_801096C8(field_0x32c);
        }
        break;

    case 12:
        if (mSpriteAnim.isAnimEnd_801D208C()) {
            field_0x308++;
            if (field_0x308 >= 90) {
                setState_801096C8(13);
            }
        }
        break;

    case 13:
        mSpriteAnim.update_801D1C18();
        if (lbl_8053AA80 != 0) {
            setState_801096C8(14);
        }
        break;

    case 14:
        if (field_0x300 < 0) {
            setState_801096C8(1);
        } else {
            setState_801096C8(5);
        }
        break;

    case 15:
        field_0x318++;
        if (field_0x318 < 15) {
            if (stepRail()) {
                setState_801096C8(16);
            }
        }
        break;

    case 16:
        fn_8022FEB8(&box, mLayer_0x4, mSerial, true, false);
        field_0x31c = fn_8022FD8C(mSerial);
        if (field_0x31c >= 0 && (gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << field_0x31c))) {
            if (openMessage_8038AD24(field_0x31c, 0x16E, getSerial(), getField_0x4(), mPos, 0x28)) {
                setState_801096C8(17);
            }
        }
        break;

    case 17:
        fn_8022FF8C(field_0x31c, &box, mLayer_0x4, mSerial, true, false);
        if (fn_80388CAC(field_0x31c, mSerial)) {
            setState_801096C8(16);
        }
        break;
    }

    if (field_0x338 != 0) {
        fn_80239914(&box, mLayer_0x4, mSerial);
    }

    if (field_0x339 != 0) {
        if (!fn_8022C52C(field_0x300, 0x400)) {
            if (!checkTile(getTileCollisionCode_8022461C(&mPos, mLayer_0x4))) {
                JGeometry::TVec3f pos;
                sweepMapBox_801F2618(&pos, false);
            }
        }
    }

    JGeometry::TVec3f push(0.0f, 0.0f, 0.0f);
    if (fn_80109AE0(&push)) {
        mPos += push;
    }

    mSpriteAnim.mFrameCtrl.setRate(field_0x330);
    mSpriteAnim.update_801D1C18();
    fn_80109534();
    gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);

    if (mLife == 0 && field_0x328 < 12 && field_0x2f8 == 0) {
        setState_801096C8(12);
    }
}

void PrincessZelda_8049CEE0::draw() {
    ActorBase_804A5770::draw();
    SpriteParams_801F5AE0 params;
    if (field_0x2f8 != 0) {
        params.field_0x10.a = 255.0f * JMath::sincosTable_.sinDegree(field_0x30c * 60);
    }
    if (field_0x320 == 0) {
        drawSpriteScaled_801F02BC("enemy_both2_sob.bin", mSpriteAnim.mCel + 0x218, &params, 0x407, 0, false, 1.0f, 1.0f);
    } else {
        drawSpriteScaled_801F02BC("enemy_both2_sob.bin", mSpriteAnim.mCel + 0x9b, &params, 0x407, 0, false, 1.0f, 1.0f);
    }
}

static inline bool overlaps(const JGeometry::TBox2f& a, const JGeometry::TBox2f& b) {
    JGeometry::TBox2f overlap(a);
    overlap.i.setMax(b.i);
    overlap.f.setMin(b.f);
    bool result = false;
    if (overlap.f.x >= overlap.i.x && overlap.f.y >= overlap.i.y) {
        result = true;
    }
    return result;
}

bool PrincessZelda_8049CEE0::fn_80109AE0(JGeometry::TVec3f* outPos) {
    outPos->zero();
    s32 serial = getActorManager_801F856C()->findSerialByActorId_801F7A08('CIRC');
    if (serial < 0) {
        return false;
    }
    ActorBase_804A5770* circle = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
    if (circle == NULL) {
        return false;
    }
    if (!circle->mAlive) {
        return false;
    }
    if (mLayer_0x4 != circle->mLayer_0x4) {
        return false;
    }
    JGeometry::TBox2f box1;
    getBoxAtPos_801F31C4(&box1);
    JGeometry::TBox2f box2;
    circle->getBoxAtPos_801F31C4(&box2);
    box2.i.x += -16.0f;
    box2.i.y += -16.0f;
    box2.f.x += 16.0f;
    box2.f.y += 16.0f;
    if (overlaps(box1, box2)) {
        JGeometry::TVec3f cpos = circle->mPos;
        outPos->set(mPos.x - cpos.x, mPos.y - cpos.y, 0.0f);
        outPos->normalize();
        outPos->scale(1.6f);
        return true;
    }
    return false;
}

// field_0x334 (the speed) towards 1.1 when `dist` is 32 or more, towards 0 below.
// No copy of its own in the original: update inlines it and the linker dropped the body.
// A plain function at this place: its literals stand in front of fn_80109AE0's in .sdata2.
// FAKEMATCH(crack-pass): pool helper failing R2 (1 call site, 7-line definition): split out only where the .sdata2 order needs it.
void PrincessZelda_8049CEE0::stepSpeed(f32 dist) {
    if (dist >= 32.0f) {
        moveToward_801CD950(&field_0x334, 1.1f, 0.02f);
    } else {
        moveToward_801CD950(&field_0x334, 0.0f, 0.04f);
    }
}

// FIXME: 99.98 %, operands; behaviour drift.
// missing/extra + reloc-name: ours has 1 more instr (263 vs 262): an extra clrlwi r4,r4,24 before a call, and some instructions are scheduled differently.
bool PrincessZelda_8049CEE0::setState_801096C8(s32 state) {
    s32 limtSerial;
    field_0x32c = field_0x328;
    field_0x328 = state;
    field_0x338 = 0;
    switch (state) {
    case 0:
        return false;
    case 1:
        field_0x338 = 1;
        mSpriteAnim.startAnim_801D22DC(7, 2, false);
        break;
    case 2:
    case 3:
    case 4:
        break;
    case 5: {
        s32 viewId = field_0x16c;
        if (viewId != -1) {
            getGameView_802D6A9C()->fn_802D7704(viewId, mParamDstr_0x1a8, mLayer_0x4);
        }
        mSpriteAnim.startAnim_801D22DC(2, 2, false);
        break;
    }
    case 6:
        mSpriteAnim.startAnim_801D22DC(2, 2, false);
        break;
    case 7: {
        pool_8053C3E4();
        mSpriteAnim.startAnim_801D22DC(9, 0, false);
        break;
    }
    case 8: {
        pool_8053C3E4();
        mSpriteAnim.startAnim_801D22DC(2, 2, false);
        break;
    }
    case 9:
        mSpriteAnim.startAnim_801D22DC(getPlayerFacing_80230CFC(field_0x300), 2, false);
        break;
    case 10:
        mSpriteAnim.startAnim_801D22DC(getPlayerFacing_80230CFC(field_0x300), 2, false);
        break;
    case 11:
        mSpriteAnim.startAnim_801D22DC(mDir_0x98 + 10, 0, false);
        field_0x2f8 = 1;
        break;
    case 12:
        mSpriteAnim.startAnim_801D22DC(4, 0, false);
        gAudioMgr_8053AF80->playSystemSound_80458880(10, (s8)field_0x300);
        for (s32 i = 0; i < 4; i++) {
            fn_80230AD4(i);
        }
        break;
    case 13: {
        for (s32 i = 0; i < 4; i++) {
            fn_80230224(i, 0);
            // The original narrows the shifted bit to a byte (clrlwi 24) before the test.
            if (gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << i)) {
                fn_8023764C(i, 0);
            }
        }
        limtSerial = field_0x310;
        if (limtSerial >= 0) {
            ActorBase_804A5770* limt = getActorManager_801F856C()->findActorBySerial_801F666C(limtSerial);
            if (limt != NULL && limt->mActorId == 'LIMT') {
                limt->mAlive = false;
            }
        }
        field_0x308 = 0;
        field_0x310 = -1;
        return true;
    }
    case 14:
        mSpriteAnim.startAnim_801D22DC(2, 2, false);
        lbl_8053AA80 = 0;
        mLife = 4;
        field_0x2f8 = 1;
        field_0x30c = 0;
        field_0x310 = getActorManager_801F856C()->spawnActor_801F9484('LIMT', mLayer_0x4, &mPos, 0, -1, -1);
        break;
    case 15: {
        if (field_0x2fc > 0 && field_0x320 == 0) {
            mRail.fn_800FE3EC(field_0x2fc, mPos, mLayer_0x4);
        }
        fn_8023077C();
        JGeometry::TVec3f pos(mPos);
        pos.y += -24.0f;
        spawnEmitter_8013CC50(mLayer_0x4, &pos, 0x33A, 0, 0, NULL, NULL, true, 1.0f);
        break;
    }
    case 16:
        fn_80230534(false);
        mSpriteAnim.startAnim_801D22DC(7, 2, false);
        field_0x338 = 1;
        field_0x318 = 0;
        break;
    case 17:
        field_0x338 = 1;
        break;
    default:
        return false;
    }
    pool_8053C3E0();
    return true;
}

void PrincessZelda_8049CEE0::fn_80109534() {
    if (field_0x310 < 0) {
        field_0x310 = getActorManager_801F856C()->spawnActor_801F9484('LIMT', mLayer_0x4, &mPos, 0, -1, -1);
        if (field_0x310 < 0) {
            return;
        }
    }
    LifeMeter_804B174C* meter =
        static_cast<LifeMeter_804B174C*>(getActorManager_801F856C()->findActorBySerial_801F666C(field_0x310));
    if (meter == NULL) {
        field_0x310 = -1;
        return;
    }
    if (field_0x2f8 != 0 || field_0x328 == 12) {
        meter->mDrawEnabled = true;
    } else {
        meter->mDrawEnabled = false;
    }

    switch (field_0x328) {
    case 12:
        if (mSpriteAnim.mKey < 8) {
            meter->fn_8019CEF8(mSerial, -28.0f, mLife + 2);
        } else if (mSpriteAnim.mKey < 12) {
            meter->fn_8019CEF8(mSerial, -28.0f, mLife);
        } else {
            meter->mDrawEnabled = false;
        }
        break;
    default: {
        s32 life = mLife;
        if (life == 0) {
            return;
        }
        if (life == 4) {
            meter->fn_8019CEF8(mSerial, -28.0f, life);
        } else if (field_0x30c < 45 && field_0x2f8 != 0) {
            meter->fn_8019CEF8(mSerial, -28.0f, life + 2);
        } else {
            meter->fn_8019CEF8(mSerial, -28.0f, life);
        }
        break;
    }
    }
}

// The four functions below have no copy of their own in the original either: update and
// setState_801096C8 inline them and the linker dropped the bodies. They were plain functions
// generated before fn_80109534, in this order from the bottom, which is the order of
// 0, -24, 1, 16, 0.5, 5, 2 in the original's .sdata2.

// State 9, with a player to follow: the player's position moved 16 to the side mDir_0x98, mPos towards it by 2.
// FAKEMATCH(crack-pass): pool helper failing R2 (1 call site, 19-line definition): split out only where the .sdata2 order needs it.
void PrincessZelda_8049CEE0::approachPlayerSide() {
    if (field_0x300 < 0) {
        return;
    }
    field_0x339 = 0;
    JGeometry::TVec3f ppos = *getPlayerPos_8023E724(field_0x300);
    mDir_0x98 = getPlayerFacing_80230CFC(field_0x300);
    switch (mDir_0x98) {
    case 0:
        ppos.x -= 16.0f;
        break;
    case 1:
        ppos.x += 16.0f;
        break;
    case 2:
        ppos.y -= 16.0f;
        break;
    case 3:
        ppos.y += 16.0f;
        break;
    }
    field_0x330 = 0.5f;
    moveToward_801CD950(&mPos.x, ppos.x, 2.0f);
    moveToward_801CD950(&mPos.y, ppos.y, 2.0f);
}

// The same side position; mPos jumps to it on the player's layer, else moves towards it by 5.
// FAKEMATCH(crack-pass): pool helper failing R2 (1 call site, 25-line definition): split out only where the .sdata2 order needs it.
void PrincessZelda_8049CEE0::followPlayerSide() {
    if (field_0x300 < 0) {
        return;
    }
    field_0x339 = 0;
    JGeometry::TVec3f ppos = *getPlayerPos_8023E724(field_0x300);
    mDir_0x98 = getPlayerFacing_80230CFC(field_0x300);
    switch (mDir_0x98) {
    case 0:
        ppos.x -= 16.0f;
        break;
    case 1:
        ppos.x += 16.0f;
        break;
    case 2:
        ppos.y -= 16.0f;
        break;
    case 3:
        ppos.y += 16.0f;
        break;
    }
    field_0x330 = 0.5f;
    if (getPlayerLayer_8023DE58(field_0x300) == getPlayerDestLayer_8023E10C(field_0x300)) {
        mPos.x = ppos.x;
        mPos.y = ppos.y;
        mPos.z = ppos.z;
    } else {
        moveToward_801CD950(&mPos.x, ppos.x, 5.0f);
        moveToward_801CD950(&mPos.y, ppos.y, 5.0f);
    }
}

// Effect 0x33A 24 above mPos (states 7 and 8).
// R2 pool helper (inlined at 2 call sites); was spawnEffect33A.
void PrincessZelda_8049CEE0::pool_8053C3E4() {
    JGeometry::TVec3f pos = mPos;
    pos.y += -24.0f;
    spawnEmitter_8013CC50(mLayer_0x4, &pos, 0x33A, 0, 0, NULL, NULL, true, 1.0f);
}

// mSpriteAnim.setFrame_801D1F14(0.0f).
// R2 pool helper (inlined at 5 call sites; one-statement body); was resetAnimFrame.
void PrincessZelda_8049CEE0::pool_8053C3E0() {
    mSpriteAnim.setFrame_801D1F14(0.0f);
}
