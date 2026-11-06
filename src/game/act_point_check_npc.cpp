#include "game/GameAudio.h"
#include "game/act/act_point_check_npc.h"
#include "game/ActorManager.h"
#include "game/BgRewrite.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Player.h"

void PointCheckNPC_804A81C8::init() {
    field_0x234 = field_0x90 & 0x1F;
    mBox.set(-1.0f, -1.0f, 1.0f, 1.0f);
    field_0x230 = 0;
    if (vfunc_0x34()) {
        field_0x230 = 4;
    }
}

// FIXME: 98.3%. Regalloc / scheduling: same length, 8 opcodes differ (tile compare regalloc, PRNG drops regalloc, and fn_802F9974 call site independent load scheduling).
void PointCheckNPC_804A81C8::update() {
    JGeometry::TVec2<f32> p(mPos.x + 4.0f, mPos.y + 4.0f);
    JGeometry::TBox2f box(mBox);
    box.addPos(p);
    s32 player = fn_8023AF14(&box, field_0x4, false, 0.0f);

    switch (field_0x230) {
    case 0: {
        s32 layer = field_0x4;
        GameView_804A9950* view = getGameView_802D6A9C();
        const JGeometry::TBox2f* src = layer < 8 ? &view->mVisibleBox : &view->field_0xdc;
        s32 x = (s32)(mPos.x - src->i.x);
        s32 y = (s32)(mPos.y - src->i.y);
        if (x < 0) {
            x = 0;
        } else if (x >= 0x200) {
            x = 0x1FF;
        }
        if (y < 0) {
            y = 0;
        } else if (y >= 0x200) {
            y = 0x1FF;
        }
        field_0x23c = ((y & 0x100) << 1) | (x & 0x100) | (y & 0xF0) | ((x >> 4) & 0xF);
        field_0x240 = gGameRoot2_8053AB10->field_0x20->fn_802F9908(field_0x4, field_0x23c) & 0x3FF;
        field_0x230 = 1;
        break;
    }
    case 1: {
        u16 tile = gGameRoot2_8053AB10->field_0x20->fn_802F9908(field_0x4, field_0x23c) & 0x3FF;
        if (tile == field_0x240) {
            break;
        }
        switch (field_0x234) {
        case 5:
            vfunc_0x30(0xFF, true);
            // fallthrough
        case 0:
        case 10:
            getActorManager_801F856C()->spawnActor_801F9484('GOLD', field_0x4, &mPos, 0x30000, -1, -1);
            if (field_0x234 == 10) {
                mAlive = false;
            } else {
                fn_801F35AC();
            }
            break;
        case 1:
            field_0x238 = 100;
            field_0x230 = 2;
            break;
        case 2:
            field_0x230 = 2;
            break;
        case 3:
            vfunc_0x30(0xFF, true);
            mAlive = false;
            break;
        case 4:
        case 11:
            field_0x238 = 300;
            field_0x230 = 2;
            break;
        case 6:
            if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.2f) {
                bool spawned = false;
                if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.8f) {
                    s32 serial = getActorManager_801F856C()->spawnActor_801F9484('RUPY', field_0x4, &mPos, 0, -1, -1);
                    ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                    if (actor != NULL) {
                        JGeometry::TVec3f vel(0.0f, 0.0f, 3.0f);
                        actor->field_0x3c = vel;
                    }
                } else {
                    if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.5f) {
                        getActorManager_801F856C()->spawnActor_801F9484('MTBO', field_0x4, &mPos, 0, -1, -1);
                        spawned = true;
                    }
                    if (!spawned) {
                        if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.2f) {
                            getActorManager_801F856C()->spawnActor_801F9484('FARY', field_0x4, &mPos, 0, -1, -1);
                        } else {
                            getActorManager_801F856C()->spawnActor_801F9484('HART', field_0x4, &mPos, 0, -1, -1);
                        }
                    }
                }
            }
            fn_801F35AC();
            break;
        case 7:
            getActorManager_801F856C()->spawnActor_801F9484('HART', field_0x4, &mPos, 0, -1, -1);
            mAlive = false;
            break;
        case 8:
            getActorManager_801F856C()->spawnActor_801F9484('MTBO', field_0x4, &mPos, 0, -1, -1);
            fn_801F35AC();
            break;
        case 9:
            getActorManager_801F856C()->spawnActor_801F9484('FARY', field_0x4, &mPos, 0, -1, -1);
            fn_801F35AC();
            break;
        default:
            fn_801F35AC();
            break;
        }
        break;
    }
    case 2:
        switch (field_0x234) {
        case 1:
            field_0x238--;
            if (field_0x238 == 0) {
                gGameRoot2_8053AB10->field_0x20->fn_802F9974(field_0x4, field_0x23c, field_0x240);
                field_0x230 = 1;
            }
            break;
        case 2:
            if (player >= 0) {
                if (fn_80234F68(player) == true) {
                    field_0x238 = 60;
                    field_0x230 = 3;
                }
            }
            break;
        case 4:
        case 11:
            field_0x238--;
            if (field_0x238 == 0) {
                if (player < 0) {
                    s32 layer = field_0x4;
                    if (layer >= 8) {
                        JGeometry::TVec3f pos(mPos.x + 8.0f, mPos.y + 8.0f, 0.0f);
                        getActorManager_801F856C()->spawnActor_801F9484('KMRI', layer, &pos, 3, -1, -1);
                    } else {
                        JGeometry::TVec3f pos(mPos.x + 8.0f, mPos.y + 8.0f, 0.0f);
                        spawnEmitter_8013CC50(layer, &pos, 0x503, 1, 0, NULL, NULL, true, 1.0f);
                    }
                    if (field_0x234 == 4) {
                        gBgRewrite_8053AC98->fn_802A3884(&mPos, 0, field_0x4, 0x4F);
                    } else {
                        gBgRewrite_8053AC98->fn_802A3884(&mPos, 0, field_0x4, 0x4E);
                    }
                    field_0x230 = 1;
                } else {
                    field_0x238 = 1;
                }
            }
            break;
        default:
            fn_801F35AC();
            break;
        }
        break;
    case 3:
        switch (field_0x234) {
        case 2:
            field_0x238--;
            if (field_0x238 == 0) {
                gGameRoot2_8053AB10->field_0x20->fn_802F9974(field_0x4, field_0x23c, field_0x240);
                field_0x230 = 1;
            }
            break;
        default:
            fn_801F35AC();
            break;
        }
        break;
    case 4:
        switch (field_0x234) {
        case 5:
            vfunc_0x30(0xFF, true);
            break;
        case 3:
            vfunc_0x30(0xFF, true);
            break;
        }
        mAlive = false;
        break;
    }
}

void PointCheckNPC_804A81C8::draw() {}

PointCheckNPC_804A81C8::~PointCheckNPC_804A81C8() {}
