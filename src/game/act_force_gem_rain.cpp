#include "game/GameAudio.h"
#include "game/act/act_force_gem_rain.h"
#include "game/ActorManager.h"
#include "game/Camera.h"
#include "game/GameGlobals.h"
#include "game/ViewLayer.h"
#include "JSystem/JMath/JMath.h"

ForceGemRain_804A8010::~ForceGemRain_804A8010() {}

void ForceGemRain_804A8010::draw() {}

// FIXME: 75.31 %. Codegen only: the layer, spawn id and angle sit in r25, r30
// and r28 where the original has r27, r28 and r30; the RNG loads and the float
// ops of each position are scheduled in another order (same arithmetic); the
// four `range == 0.0f` tests are a bool materialised by mfcr/cntlzw in the
// original. Data: the jump table reloc (jumptable_804A8064 vs @NNN) and the
// sdata2 literals (@NNN vs lbl_) are the data split.
void ForceGemRain_804A8010::update() {
    s32 layer = field_0x4;
    GameView_804A9950* view = getGameView_802D6A9C();
    JGeometry::TBox2f* box = (layer < 8) ? &view->mVisibleBox : &view->field_0xdc;
    JGeometry::TVec3f pos;

    switch (field_0x230) {
    case 0: {
        if (field_0x244 == 3 || field_0x244 == 0xC) {
            fn_801F0D20(3);
            field_0x230 = 1;
            break;
        }
        if (!vfunc_0x34()) {
            break;
        }
        u32 spawnId;
        u32 spawnArg = 0;
        if (field_0x248 > 0) {
            field_0x248--;
        }
        if (field_0x24c > 0) {
            field_0x24c--;
        }
        if (field_0x24c != 0) {
            if (field_0x244 == 0 || field_0x244 == 1 || field_0x244 == 0xB || field_0x244 == 0xD) {
                Camera_804AA9E4* cam = gGameRoot2_8053AB10->field_0x24;
                if (cam->field_0x7f4 == 0) {
                    cam->field_0x7f5 = 1;
                }
            }
            return;
        }
        if (field_0x244 == 0 || field_0x244 == 1 || field_0x244 == 0xB || field_0x244 == 0xD) {
            Camera_804AA9E4* cam = gGameRoot2_8053AB10->field_0x24;
            if (cam->field_0x7f4 == 0) {
                cam->field_0x7f5 = 1;
            }
        }
        {
            switch (field_0x244) {
            case 0:
            case 6:
            case 7:
                fn_801F0D20(3);
                field_0x230 = 1;
                break;
            case 5:
                field_0x240 = 0x80;
                lbl_8053AF80->fn_80458880(6, -1);
                field_0x230 = 3;
                break;
            case 1:
                field_0x240 = 8;
                lbl_8053AF80->fn_80458880(6, -1);
                field_0x230 = 3;
                break;
            case 2:
                field_0x240 = 8;
                lbl_8053AF80->fn_80458880(6, -1);
                spawnId = 'HBMB';
                spawnArg = 3;
                break;
            case 3:
            case 12:
                break;
            case 4:
                field_0x240 = 8;
                lbl_8053AF80->fn_80458880(6, -1);
                spawnId = 'SLR1';
                spawnArg = 5 << 16;
                break;
            case 10:
                field_0x240 = 1;
                spawnId = 'SLR1';
                // Built with an OR, as the original does (one literal folds to a single addi).
                spawnArg = 3 << 16;
                spawnArg |= 0x102;
                break;
            case 8:
                field_0x240 = 2;
                lbl_8053AF80->fn_80458880(6, -1);
                spawnId = 'HINO';
                spawnArg = 1;
                break;
            case 9:
                field_0x240 = 8;
                lbl_8053AF80->fn_80458880(6, -1);
                spawnId = 'STAL';
                spawnArg = 4;
                break;
            case 11:
                lbl_8053AF80->fn_80458880(6, -1);
            // fall through
            case 13:
                spawnId = 'ROPE';
                spawnArg = 0;
                break;
            }
            if (field_0x244 == 2 || field_0x244 == 4 || (u32)(field_0x244 - 8) <= 3 ||
                field_0x244 == 0xD) {
                s32 angle = 0;
                for (s32 i = 0; i < field_0x240; i++) {
                    if (field_0x244 == 8) {
                        if (i == 0) {
                            pos.y = mPos.y + 48.0f * JMath::sincosTable_.mTable[0x200].second;
                            pos.x = mPos.x + 48.0f * JMath::sincosTable_.mTable[0x200].first;
                        } else {
                            pos.y = mPos.y + 48.0f * JMath::sincosTable_.mTable[0x600].second;
                            pos.x = mPos.x + 48.0f * JMath::sincosTable_.mTable[0x600].first;
                        }
                        pos.z = 1000.0f;
                    } else if (field_0x244 == 0xA) {
                        pos.x = mPos.x;
                        pos.y = mPos.y;
                        pos.z = 400.0f;
                    } else if (field_0x244 == 0xB || field_0x244 == 0xD) {
                        u32 n = (field_0x90 >> 8) & 0xF;
                        f32 range = 8.0f * (f32)n;
                        if (range == 0.0f) {
                            range = 64.0f;
                        }
                        f32 twoRange = 2.0f * range;
                        pos.z = 50.0f * lbl_8053AAF8->mRandom.get_ufloat_1() + 300.0f;
                        pos.y = mPos.y + (range - twoRange * lbl_8053AAF8->mRandom.get_ufloat_1());
                        pos.x = mPos.x + (range - twoRange * lbl_8053AAF8->mRandom.get_ufloat_1());
                    } else {
                        f32 s = JMath::sincosTable_.mTable[(u16)angle >> 5].first;
                        f32 c = JMath::sincosTable_.mTable[(u16)angle >> 5].second;
                        pos.y = mPos.y + 32.0f * c;
                        pos.x = mPos.x + 32.0f * s;
                        pos.z = 1000.0f;
                    }
                    if (viewLayerFlag(field_0x4)) {
                        pos.y = mPos.y;
                        pos.z = 0.0f;
                    }
                    s32 serial = fn_801F856C()->fn_801F9484(spawnId, field_0x4, &pos, spawnArg, -1, -1);
                    if (serial >= 0) {
                        ActorBase_804A5770* actor = fn_801F856C()->findActorBySerial_801F666C(serial);
                        if (actor != NULL) {
                            actor->field_0x3c = JGeometry::TVec3f(0.0f, 0.0f, -1.0f);
                        }
                    }
                    angle += 0x2000;
                }
                field_0x248 = 0x28;
                field_0x230 = 4;
                break;
            }
        }
        break;
    }
    case 1: {
        if (field_0x238 == 1) {
            if (lbl_8053AAF8->fn_80138A30() == 2) {
                field_0x240 = 0x50;
            }
            if (lbl_8053AAF8->fn_80138A30() > 2) {
                field_0x240 = 0x50;
                field_0x234 = 2;
            }
        } else if (field_0x238 == 2) {
            if (lbl_8053AAF8->fn_80138A30() == 2) {
                field_0x240 = 0xF0;
            }
            if (lbl_8053AAF8->fn_80138A30() > 2) {
                field_0x240 = 0xF0;
                field_0x234 = 2;
            }
        } else if (field_0x238 == 3) {
            if (lbl_8053AAF8->fn_80138A30() > 1) {
                field_0x240 = 0xFF;
                if (lbl_8053AAF8->fn_80138A30() > 2) {
                    field_0x234 = lbl_8053AAF8->fn_80138A30() - 1;
                }
            }
        }
        field_0x230 = 2;
    }
    // fall through
    case 2: {
        if (field_0x244 == 0 || field_0x244 == 1 || field_0x244 == 0xB || field_0x244 == 0xD) {
            Camera_804AA9E4* cam = gGameRoot2_8053AB10->field_0x24;
            if (cam->field_0x7f4 == 0) {
                cam->field_0x7f5 = 1;
            }
        }
        pos.x = mPos.x;
        pos.y = mPos.y;
        pos.z = mPos.z;
        s32 k = 0;
        do {
            if (viewLayerFlag(field_0x4)) {
                if (field_0x244 == 6 || field_0x244 == 7 || field_0x244 == 0xC) {
                    u32 n = (field_0x90 >> 8) & 0xF;
                    f32 range = 8.0f * (f32)n;
                    if (range == 0.0f) {
                        range = 64.0f;
                    }
                    f32 twoRange = 2.0f * range;
                    pos.x = mPos.x + (range - twoRange * lbl_8053AAF8->mRandom.get_ufloat_1());
                } else {
                    f32 w = box->f.x - box->i.x;
                    pos.x = w * lbl_8053AAF8->mRandom.get_ufloat_1() + box->i.x;
                }
                pos.z = 0.0f;
            } else {
                if (field_0x244 == 6 || field_0x244 == 7 || field_0x244 == 0xC) {
                    u32 n = (field_0x90 >> 8) & 0xF;
                    f32 range = 8.0f * (f32)n;
                    if (range == 0.0f) {
                        range = 64.0f;
                    }
                    f32 twoRange = 2.0f * range;
                    pos.z = 50.0f * lbl_8053AAF8->mRandom.get_ufloat_1() + 300.0f;
                    pos.y = mPos.y + (range - twoRange * lbl_8053AAF8->mRandom.get_ufloat_1());
                    pos.x = mPos.x + (range - twoRange * lbl_8053AAF8->mRandom.get_ufloat_1());
                } else {
                    pos.z = 50.0f * lbl_8053AAF8->mRandom.get_ufloat_1() + 300.0f;
                    f32 h = box->f.y - box->i.y;
                    pos.y = h * lbl_8053AAF8->mRandom.get_ufloat_1() + box->i.y;
                    f32 w = box->f.x - box->i.x;
                    pos.x = w * lbl_8053AAF8->mRandom.get_ufloat_1() + box->i.x;
                }
            }
            s32 t = (s32)(100.0f * lbl_8053AAF8->mRandom.get_ufloat_1());
            s32 pick;
            if (t < 5) {
                pick = 2;
            } else if (t < 20) {
                pick = 1;
            } else {
                pick = 0;
            }
            fn_801F856C()->fn_801F8544('RUPY', field_0x4, &pos, pick);
            field_0x23c++;
        } while (++k < 2 && field_0x23c < field_0x240);
        if (field_0x23c < field_0x240) {
            break;
        }
        for (s32 i = 0; i < field_0x234; i++) {
            pos.x = mPos.x;
            pos.y = mPos.y;
            pos.z = mPos.z;
            if (viewLayerFlag(field_0x4)) {
                pos.y = mPos.y;
                pos.z = 0.0f;
            } else if (field_0x244 == 6 || field_0x244 == 7 || field_0x244 == 0xC) {
                u32 n = (field_0x90 >> 8) & 0xF;
                f32 range = 8.0f * (f32)n;
                if (range == 0.0f) {
                    range = 64.0f;
                }
                f32 twoRange = 2.0f * range;
                pos.z = 50.0f * lbl_8053AAF8->mRandom.get_ufloat_1() + 300.0f;
                pos.y = mPos.y + (range - twoRange * lbl_8053AAF8->mRandom.get_ufloat_1());
                pos.x = mPos.x + (range - twoRange * lbl_8053AAF8->mRandom.get_ufloat_1());
            } else {
                pos.z = 50.0f * lbl_8053AAF8->mRandom.get_ufloat_1() + 300.0f;
                f32 h = box->f.y - box->i.y;
                pos.y = h * lbl_8053AAF8->mRandom.get_ufloat_1() + box->i.y;
                f32 w = box->f.x - box->i.x;
                pos.x = w * lbl_8053AAF8->mRandom.get_ufloat_1() + box->i.x;
            }
            if (field_0x244 == 7) {
                fn_801F856C()->fn_801F9484('LPLK', field_0x4, &mPos, 0x400, -1, -1);
                break;
            }
            fn_801F856C()->fn_801F8544('RUPY', field_0x4, &pos, 4);
        }
        break;
    }
    case 3: {
        if (field_0x244 == 1) {
            Camera_804AA9E4* cam = gGameRoot2_8053AB10->field_0x24;
            if (cam->field_0x7f4 == 0) {
                cam->field_0x7f5 = 1;
            }
        }
        pos.z = 50.0f * lbl_8053AAF8->mRandom.get_ufloat_1() + 300.0f;
        f32 h = box->f.y - box->i.y;
        pos.y = h * lbl_8053AAF8->mRandom.get_ufloat_1() + box->i.y;
        f32 w = box->f.x - box->i.x;
        pos.x = w * lbl_8053AAF8->mRandom.get_ufloat_1() + box->i.x;
        if (viewLayerFlag(field_0x4)) {
            pos.y = mPos.y;
            pos.z = 0.0f;
        }
        if (field_0x248 == 0) {
            if (field_0x244 == 1) {
                s32 serial = fn_801F856C()->fn_801F9484('HBMB', field_0x4, &pos, 3, -1, -1);
                if (serial >= 0) {
                    ActorBase_804A5770* actor = fn_801F856C()->findActorBySerial_801F666C(serial);
                    if (actor != NULL) {
                        actor->field_0x3c = JGeometry::TVec3f(0.0f, 0.0f, 1.0f);
                    }
                }
                field_0x248 = 0x1E;
            } else {
                fn_801F856C()->fn_801F9484('POPO', field_0x4, &pos, 1, -1, -1);
            }
            field_0x23c++;
        }
        break;
    }
    case 4:
        if (field_0x244 == 0xB || field_0x244 == 0xD) {
            Camera_804AA9E4* cam = gGameRoot2_8053AB10->field_0x24;
            if (cam->field_0x7f4 == 0) {
                cam->field_0x7f5 = 1;
            }
        }
        if (field_0x248 == 0) {
            fn_801F3668();
        }
        break;
    }

    if (field_0x230 == 0) {
        return;
    }
    if (field_0x248 > 0) {
        field_0x248--;
    }
    if (field_0x24c > 0) {
        field_0x24c--;
    }
    if (field_0x230 == 4) {
        return;
    }
    if (field_0x23c < field_0x240) {
        return;
    }
    GameView_804A9950* v = getGameView_802D6A9C();
    v->fn_802D8050((field_0x90 >> 22) & 0x1F, true, 0xFF);
    fn_801F3668();
}

void ForceGemRain_804A8010::init() {
    field_0x248 = 0;
    field_0x24c = 0;
    field_0x230 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x244 = field_0x90 & 0xFF;
    if (!(field_0x244 == 3 || field_0x244 == 0xC)) {
        if (vfunc_0x34()) {
            mAlive = false;
        }
    }
    field_0x240 = (field_0x90 >> 12) & 0xFF;
    if (field_0x240 == 0) {
        field_0x240 = 0x100;
    }
    field_0x234 = 1;
    field_0x238 = 0;
    switch (field_0x240) {
    case 0x28:
        field_0x238 = 1;
        break;
    case 0x78:
        field_0x238 = 2;
        break;
    case 0xC8:
        field_0x238 = 3;
        break;
    }
    if (field_0x244 == 2 || field_0x244 == 4 || (u32)field_0x244 <= 1 ||
        (u32)(field_0x244 - 8) <= 3 || field_0x244 == 0xD) {
        field_0x24c = 0x28;
    }
}
