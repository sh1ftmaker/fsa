#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/DrawShadow.h"
#include "game/Effect.h"
#include "game/GameView.h"
#include "game/Unk_8045A080.h"
#include "game/act/act_spawner.h"

static const s32 lbl_8046DFD0[4] = { 1, 2, 1, 0 };
static const s32 lbl_8046DFE0[20] = {
    0, 2, 0x00010002, 4, 0x00010004, 6,
    0x00030003, 0x00030004, 0x00030005, 0x00030006,
    0x00040100, 0x00010100, 3, 0x00010102, 0x00000102,
    4, 0x14, 0x00000103, 0x00000100, 0x00071107
};
static const s32 lbl_8046E030[4] = { 12, 18, 12, 18 };

void SpawnerGenerator_804A9328::init() {
    for (int i = 0; i < 256; i++) {
        field_0x274[i] = -1;
    }
    mBox.set(-16.0f, -16.0f, 16.0f, 16.0f);
    field_0x234[0] = (field_0x90 >> 22) & 0x1F;
    field_0x234[1] = (field_0x90 >> 17) & 0x1F;
    field_0x234[2] = (field_0x90 >> 12) & 0x1F;
    field_0x24c = ((field_0x90 >> 7) & 0x1F) * 30;
    if (field_0x24c == 0) {
        field_0x24c = 10;
    }
    field_0x248 = 1;
    field_0x244 = field_0x90 & 0x7F;
    if (field_0x244 == 0) {
        field_0x244 = 256;
    }
    field_0x240 = 0;
    field_0x250 = field_0x90 >> 27;
    field_0x230 = 0;
    field_0x264 = 3;
    field_0x268 = 0;
    field_0x674 = 0;
    field_0x26c = 1;
    for (int i = 0; i < 3; i++) {
        s32 t = field_0x234[i];
        if (t == 10 || t == 17 || t == 19 || (u32)(t - 11) <= 2 || t == 15 || t == 14) {
            field_0x26c = 0;
        }
    }
    field_0x270 = 0.0f;
    field_0x254 = 0;
    field_0x258.x = mPos.x + 8.0f;
    field_0x258.y = mPos.y + 26.0f;
    field_0x258.z = mPos.z;
}

// FIXME: 99.96% regalloc-only: target->ours r28->r29 on flag
void SpawnerGenerator_804A9328::update() {
    switch (field_0x230) {
    case 0:
    case 1: {
        if (field_0x250 == 0 || vfunc_0x34()) {
            if (field_0x230 == 0) {
                field_0x230 = 1;
                field_0x268 = 12;
                field_0x264 = 0;
                if (fn_802DE764(0, 3, 10)) {
                    gAudioMgr_8053AF80->fn_80458FF0(0x1000021, false, 0);
                }
            }
            field_0x248--;
            if (field_0x248 == 0) {
                if (fn_802DE764(0, 3, 6)) {
                    if (field_0x240 == 0) {
                        field_0x254 = 1;
                    } else {
                        field_0x254 = 0;
                    }
                }
                field_0x248 = field_0x24c;
                if (field_0x240 < field_0x244) {
                    switch (field_0x234[field_0x254]) {
                    case 0:
                        field_0x274[field_0x240] = getActorManager_801F856C()->spawnActor_801F9484('HINO', field_0x4, &field_0x258, 0, -1, -1);
                        break;
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 10:
                    case 11:
                    case 13:
                    case 14:
                    case 18: {
                        s32 arg = lbl_8046DFE0[field_0x234[field_0x254]];
                        field_0x274[field_0x240] = getActorManager_801F856C()->spawnActor_801F9484('SLR1', field_0x4, &field_0x258, arg, -1, -1);
                        break;
                    }
                    case 6:
                    case 7:
                    case 8:
                    case 9: {
                        s32 arg = lbl_8046DFE0[field_0x234[field_0x254]];
                        field_0x274[field_0x240] = getActorManager_801F856C()->spawnActor_801F9484('RUPY', field_0x4, &field_0x258, arg, -1, -1);
                        break;
                    }
                    case 12:
                    case 15:
                    case 17: {
                        s32 arg = lbl_8046DFE0[field_0x234[field_0x254]];
                        field_0x274[field_0x240] = getActorManager_801F856C()->spawnActor_801F9484('BMTY', field_0x4, &field_0x258, arg, -1, -1);
                        break;
                    }
                    case 19: {
                        s32 arg = lbl_8046DFE0[field_0x234[field_0x254]];
                        field_0x274[field_0x240] = getActorManager_801F856C()->spawnActor_801F9484('HNRY', field_0x4, &field_0x258, arg, -1, -1);
                        break;
                    }
                    case 16:
                        if (getActorManager_801F856C()->fn_801F7A08('BMTY') < 0) {
                            field_0x274[field_0x240] = getActorManager_801F856C()->spawnActor_801F9484('BMTY', field_0x4, &field_0x258, 20, -1, -1);
                        }
                        break;
                    }
                    field_0x240++;
                    if (field_0x240 >= field_0x244) {
                        field_0x230 = 2;
                    }
                } else {
                    field_0x230 = 2;
                }
            } else if (field_0x248 == 29) {
                s32 flag = -1;
                s32 rand = fn_801CD664(100);
                if (rand < 10) {
                    field_0x254 = 2;
                } else if (rand < 40) {
                    field_0x254 = 1;
                } else {
                    field_0x254 = 0;
                }
                if (field_0x234[field_0x254] == 16) {
                    if (getActorManager_801F856C()->fn_801F7A08('BMTY') < 0) {
                        flag = 1;
                    }
                }
                s32 t = field_0x234[field_0x254];
                if (!((t == 10 || (u32)(t - 11) <= 4 || t == 17 || t == 19 || t == 16) && flag < 0)) {
                    JGeometry::TVec3f emitterPos1(field_0x258.x, field_0x258.y - 2.0f, field_0x258.z);
                    spawnEmitter_8013CC50(field_0x4, &emitterPos1, 0x1D3, 0, 0, NULL, NULL, true, 1.0f);
                    JGeometry::TVec3f emitterPos2(field_0x258.x, field_0x258.y - 2.0f, field_0x258.z);
                    spawnEmitter_8013CC50(field_0x4, &emitterPos2, 0x1D4, 0, -1, NULL, NULL, true, 1.0f);
                    JGeometry::TVec3f emitterPos3(field_0x258.x, field_0x258.y - 2.0f, field_0x258.z);
                    spawnEmitter_8013CC50(field_0x4, &emitterPos3, 0x1D5, 0, 0, NULL, NULL, true, 1.0f);
                }
            }
        } else {
            if (field_0x230 == 1) {
                field_0x230 = 0;
                field_0x264 = 3;
            }
        }
        if (field_0x230 == 1) {
            if (field_0x268 == 0) {
                field_0x268 = lbl_8046E030[field_0x264];
                field_0x264 = lbl_8046DFD0[field_0x264];
                field_0x270 = 0.0f;
            } else {
                field_0x268--;
            }
        }
        addCalc2_801F71A4(&field_0x270, 255.0f, 0.9f, 20.0f);
        break;
    }
    case 2: {
        s32 serial;
        for (int i = 0; i < field_0x244 + 1; i++) {
            serial = field_0x274[i];
            if (serial >= 0) {
                ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                if (actor == NULL) {
                    field_0x274[i] = -1;
                    field_0x674++;
                }
            }
        }
        if (field_0x674 >= field_0x244 - 1) {
            fn_801F35AC();
        }
        break;
    }
    }
}

void SpawnerGenerator_804A9328::draw() {
    if (field_0x26c != 1) {
        return;
    }
    SpriteParams_801F5AE0 params;
    params.field_0x1d = -1;
    params.field_0x10.a = 255.0 - field_0x270;
    s32 frame = field_0x264 + 0x175;
    JGeometry::TVec3f pos(mPos.x + 8.0f, mPos.y + 24.0f, mPos.z);
    if (field_0x4 < 8) {
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_gc_sob.bin");
        gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, frame, &params, 1.0f, 1.0f);
    }
    params.field_0x10.a = field_0x270;
    s32 frame2 = lbl_8046DFD0[field_0x264] + 0x175;
    JGeometry::TVec3f pos2(mPos.x + 8.0f, mPos.y + 24.0f, mPos.z);
    if (field_0x4 < 8) {
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_gc_sob.bin");
        gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos2, res, frame2, &params, 1.0f, 1.0f);
    }
    JGeometry::TVec3f shadowPos(mPos.x + 8.0f, mPos.y + 24.0f, mPos.z);
    JUtility::TColor color(0, 0, 0, 128);
    drawShadow(field_0x4, &shadowPos, color, 30.0f);
}

SpawnerGenerator_804A9328::~SpawnerGenerator_804A9328() {}
