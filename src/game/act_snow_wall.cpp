#include "game/GameAudio.h"
#include "game/act/tu_801F9484.h"
#include "game/Effect.h"
#include "game/GameState.h"
#include "game/Player.h"
#include "game/EnemyBase.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/Map.h"

static const char* lbl_80539F40[] = {"yuki_l.bti", NULL};

cls_8049E8A0::cls_8049E8A0()
    : field_0x230(0), field_0x234(0), field_0x238(1.0f), field_0x23c(NULL), field_0x240(NULL), field_0x244(NULL) {}

cls_8049E8A0::~cls_8049E8A0() {
    if (field_0x240 != NULL) {
        releaseEmitter_801EE434(field_0x240);
        field_0x240 = NULL;
    }
    if (field_0x244 != NULL) {
        releaseEmitter_801EE434(field_0x244);
        field_0x244 = NULL;
    }
}

void cls_8049E8A0::init() {
    ActorBase_804A5770::init();
    mBox.i.x = -24.0f;
    mBox.i.y = -32.0f;
    mBox.f.x = 24.0f;
    mBox.f.y = 32.0f;
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
    field_0xb0 = 0x0a040304;
    if (field_0x23c == NULL) {
        field_0x23c = fn_804032E8('TIMG', lbl_80539F40[0]);
    }
    if (field_0x240 == NULL) {
        field_0x240 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x452, 0, 0, NULL, NULL, true, 1.0f);
    }
    if (field_0x244 == NULL) {
        field_0x244 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x453, 0, 0, NULL, NULL, true, 1.0f);
    }
}

void cls_8049E8A0::update() {
    ActorBase_804A5770::update();
    JGeometry::TBox2f box(mBox);
    box.i.x += mPos.x;
    box.i.y += mPos.y;
    box.f.x += mPos.x;
    box.f.y += mPos.y;
    fn_80239914(&box, field_0x4, mSerial);
    switch (field_0x230) {
    case 0: {
        startSound_801F0E34(0x50069, 0);
        fn_801F2DB4('NULL', 'NULL', NULL);
        s32 player = fn_801F285C(NULL, NULL, true, -1);
        if (player >= 0) {
            startSound_801F0E34(0x400d1, 0);
            JGeometry::TVec2f pos(mPos.x, mPos.y);
            fn_802379E4(player, (const JGeometry::TVec3f*)&pos);
        }
        s32 fireSerial = fn_801F2718('FIRE');
        bool hasFire;
        if (fireSerial <= 0) {
            hasFire = false;
        } else {
            ActorBase_804A5770* fire = getActorManager_801F856C()->findActorBySerial_801F666C(fireSerial);
            if (fire == NULL) {
                hasFire = false;
            } else if (static_cast<FireRodFire_804A89F0*>(fire)->fn_802BA9F8() == 1) {
                hasFire = true;
            } else {
                hasFire = false;
            }
        }
        if (!hasFire) {
            field_0x234 = 0;
            break;
        }
        if (field_0x234 == 0) {
            JGeometry::TVec3f pos = mPos;
            pos.y += 16.0f;
            spawnEmitter_8013CC50(field_0x4, &pos, 0x28d, 0, 0, NULL, NULL, true, field_0x238);
            spawnEmitter_8013CC50(field_0x4, &pos, 0x28e, 0, 0, NULL, NULL, true, field_0x238);
            startSound_801F0E34(0x30061, 0);
        }
        field_0x234++;
        if (field_0x234 >= 60) {
            field_0x230 = 1;
            releaseEmitter_801EE434(field_0x240);
            releaseEmitter_801EE434(field_0x244);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x44f, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x450, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x451, 0, 0, NULL, NULL, true, 1.0f);
            startSound_801F0E34(0x5006a, 0);
        }
        break;
    }
    case 1: {
        mDrawEnabled = false;
        bool done;
        if (field_0x238 >= 0.5f && fn_801CD950(&field_0x238, 0.5f, -0.005f) != 0) {
            done = false;
        } else if (fn_801CD950(&field_0x238, 0.1f, -0.05f) == 0) {
            done = true;
        } else {
            done = false;
        }
        if (done) {
            field_0x230 = 2;
            fn_801F0D20(3);
            fn_801F3668();
        }
        break;
    }
    case 2:
        break;
    }
    field_0x54.z = field_0x54.y = field_0x54.x = field_0x238;
}

// FIXME: 93.59%. scheduling+regalloc + reloc-name. Same instructions modulo registers,
// different order; target loads y before x when filling scale matrix, and .sdata2
// float pool order differs.
void cls_8049E8A0::draw() {
    ActorBase_804A5770::draw();
    SpriteParams_801F5AE0 params;
    SpriteParams_801F5AE0 sp;
    Mtx mtx;
    if (!field_0x54.isZero()) {
        mtx[0][3] = 0.0f;
        mtx[1][3] = 0.0f;
        mtx[2][3] = 0.0f;
        mtx[0][0] = field_0x54.x;
        mtx[0][1] = 0.0f;
        mtx[0][2] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = field_0x54.y;
        mtx[1][2] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = field_0x54.z;
        mtx[0][3] = 0.0f;
        mtx[2][3] = 0.0f;
        mtx[1][3] = (1.0f - field_0x238) * 32.0f;
        params.mMtx = mtx;
    }
    if (field_0x4 < 8) {
        gGameRoot2_8053AB10->field_0x20->fn_802F745C(&mPos, field_0x23c, &params);
    }
}
