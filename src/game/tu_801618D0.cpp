#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/act/tu_801F9484.h"
#include "game/ActorManager.h"
#include "game/Camera.h"
#include "game/Effect.h"
#include "game/Map.h"
#include "game/RotMatrix.h"
#include "JSystem/JMath/JMath.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "JSystem/JParticle/JPAMath.h"
#include "JSystem/JUtility/JUTTexture.h"
#include "dolphin/gx.h"
#include "std/math.h"

// Offsets of the four ghost positions from mPos (x, y pairs), .data at 0x804A0410.
static const f32 lbl_804A0410[8] = {-80.0f, -40.0f, 80.0f, -40.0f, -80.0f, 40.0f, 80.0f, 40.0f};
static const s32 lbl_804A0430[4] = {0, 1, 2, 1};

// FIXME(shotgun): table not written out (gate.py literal tripwire); 8 colour sets of three
// RGBA colours (96 bytes), then the three texture names; extern-declared, size from symbols.txt.
extern const JUtility::TColor lbl_804661E0[24];

// A flag of the ghost's attacks, .sbss at 0x8053AB58.
s32 lbl_8053AB58;

// 0x80164118.
cls_804A0440::~cls_804A0440() {
    releaseEmitter_801EE434(field_0x2e8);
    if (field_0x268 == 2) {
        getGameView_802D6A9C()->fn_802D7440(true);
        fn_8022ADCC();
    }
}

// 0x80163E80.
// FIXME(shotgun): 74.67 %, missing/extra + reloc-name
void cls_804A0440::init() {
    field_0x94 = 0;
    mBox.set(-60.0f, -60.0f, 60.0f, 60.0f);
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x258 = 0;
    field_0x25c = 5;
    field_0x27c = 1.0f;
    field_0x280 = 1.0f;
    field_0x284 = 0.4f;
    field_0x274 = 0.0f;
    field_0x278 = 0.05f;
    field_0x254 = 0;
    field_0x294[8] = 0.0f;
    field_0x294[7] = 0.0f;
    field_0x294[6] = 0.0f;
    field_0x294[0] = mPos.x;
    field_0x294[1] = mPos.y;
    field_0x294[2] = mPos.z;
    field_0x294[3] = mPos.x;
    field_0x294[4] = mPos.y;
    field_0x294[5] = mPos.z;
    field_0x260 = 0;
    field_0x288 = 0.0f;
    field_0x28c = 0.0f;
    lbl_8053AB58 = 0;
    field_0x2e8 = NULL;
    field_0x2b8[0].x = mPos.x;
    field_0x2b8[0].y = mPos.y;
    field_0x2b8[0].z = mPos.z;
    field_0x2b8[0].x += lbl_804A0410[0];
    field_0x2b8[0].y += lbl_804A0410[1];
    field_0x2b8[1].x = mPos.x;
    field_0x2b8[1].y = mPos.y;
    field_0x2b8[1].z = mPos.z;
    field_0x2b8[1].x += lbl_804A0410[2];
    field_0x2b8[1].y += lbl_804A0410[3];
    field_0x2b8[2].x = mPos.x;
    field_0x2b8[2].y = mPos.y;
    field_0x2b8[2].z = mPos.z;
    field_0x2b8[2].x += lbl_804A0410[4];
    field_0x2b8[2].y += lbl_804A0410[5];
    field_0x2b8[3].x = mPos.x;
    field_0x2b8[3].y = mPos.y;
    field_0x2b8[3].z = mPos.z;
    field_0x2b8[3].x += lbl_804A0410[6];
    field_0x2b8[3].y += lbl_804A0410[7];
    field_0x2ec = -1;
    field_0x2f0 = -1;
    field_0x2f4[0].set(0);
    field_0x2f4[1].set(0);
    field_0x2f4[2].set(0);
    field_0x26c = -1;
    field_0x290 = 0;
    field_0x270 = (field_0x90 >> 2) & 0x1F;
    field_0x196 = 1;
    field_0x268 = field_0x90 & 3;
    mLife = 10000;
    switch (field_0x268) {
    case 0:
        getGameView_802D6A9C()->fn_802D7440(false);
        field_0x230 = 0;
        break;
    case 1:
        field_0x290 = 0x8000;
        mBox.set(-12.0f, -12.0f, 12.0f, 12.0f);
        field_0x288 = 1.0f;
        field_0x230 = 12;
        break;
    case 2:
        field_0x230 = 14;
        break;
    }
}

// 0x80162628.
// FIXME(shotgun): 85.87 %, missing/extra + reloc-name
void cls_804A0440::update() {
    mLife = 10000;
    f32 x0 = -30.0f;
    f32 y0 = -30.0f;
    f32 x1 = 30.0f;
    f32 y1 = 30.0f;
    f32 px = mPos.x;
    f32 py = mPos.y;
    if (field_0x268 == 0) {
        Camera_804AA9E4* cam = gGameRoot2_8053AB10->field_0x24;
        if (cam->field_0x179 == 0) {
            cam->field_0x178 = 1;
        }
    }
    if (field_0x268 == 2) {
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
    }
    x0 += px;
    y0 += py;
    x1 += px;
    y1 += py;
    if (vfunc_0x2C(0.0f, 0.0f, 50.0f)) {
        Camera_804AA9E4* cam = gGameRoot2_8053AB10->field_0x24;
        if (cam->field_0x840 == 0) {
            cam->field_0x840 = 1;
            cam->field_0x844.f.x = mPos.x;
            cam->field_0x844.i.x = mPos.x;
            cam->field_0x844.f.y = mPos.y;
            cam->field_0x844.i.y = mPos.y;
            cam->field_0x854 = 1.0f;
            cam->field_0x1d4 = -1.0f;
            cam->field_0x1d8 = -1.0f;
        }
        cls_804A03A8* spot = gGameRoot2_8053AB10->field_0x30;
        if (spot->field_0x284 == 0) {
            spot->field_0x284 = 1;
            spot->field_0x28c = 0.05f;
        }
        field_0x248++;
        field_0x248 &= 0xF;
        if (field_0x248 == 0) {
            JGeometry::TVec3f pos(mPos);
            pos.x += -(64.0f * (gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f) - 32.0f);
            pos.y += -(64.0f * (gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f) - 32.0f);
            spawnEmitter_8013CC50(field_0x4, &pos, 0x1FF, 1, 4, NULL, NULL, true, 1.0f);
        }
        if (field_0x188 == 1) {
            getActorManager_801F856C()->spawnActor_801F9484('BPOH', field_0x4, &mPos, 2, -1, -1);
        }
        return;
    }
    if (field_0x234 > 0) {
        field_0x234--;
    }
    if (field_0x238 > 0) {
        field_0x238--;
    }
    if (field_0x23c > 0) {
        field_0x23c--;
    }
    if (field_0x240 > 0) {
        field_0x240--;
    }
    if (field_0x268 == 0) {
        fn_80161F98(field_0x258, field_0x234);
        if (field_0x230 != 0 && field_0x230 != 1) {
            gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
        }
        if (field_0x25c > 0 && field_0x230 != 6 && field_0x230 != 7) {
            addCalc2_801F71A4(&field_0x27c, field_0x280, 0.2f, field_0x284);
        }
        if (fn_801F3D94(0) != 0) {
            return;
        }
        if (field_0x230 == 2 || field_0x230 == 4) {
            f32 dx = field_0x294[0] - mPos.x;
            f32 dy = field_0x294[1] - mPos.y;
            if (getGameView_802D6A9C()->fn_802D800C((field_0x90 >> (27 - field_0x254 * 5)) & 0x1F)) {
                if (std::sqrtf(dx * dx + dy * dy) < 40.0f && field_0x258 != 3) {
                    field_0x258 = 3;
                    field_0x234 = 10;
                    startSound_801F0E34(0x4001F, 0);
                }
            }
        }
        if (field_0x230 == 4) {
            f32 half = 10.0f * (f32)field_0x25c;
            mBox.set(-half, -half, half, half);
            if (fn_801F2FAC()) {
                field_0x3c.set(0.0f, 0.0f, 0.0f);
                field_0x25c--;
                getGameView_802D6A9C()->fn_802D7E34((field_0x90 >> 7) & 0x1F);
                field_0x258 = 6;
                field_0x234 = 10;
                if (field_0x25c > 0) {
                    field_0x27c = 2.5f * (0.2f * (f32)field_0x25c);
                    field_0x280 = 0.2f * (f32)field_0x25c;
                    startSound_801F0E34(0x40014, 0);
                    field_0x23c = 50;
                    field_0x258 = 6;
                    field_0x234 = 10;
                    field_0x230 = 5;
                } else {
                    field_0x230 = 9;
                }
                field_0x284 = 0.5f * field_0x280;
                return;
            }
        }
        if (field_0x230 != 3) {
            addCalc2_801F71A4(&field_0x274, field_0x278, 0.1f, 0.1f);
            addCalc2_801F71A4(&mPos.x, field_0x294[0], 0.3f, field_0x274 * std::fabsf(mPos.x - field_0x294[0]));
            addCalc2_801F71A4(&mPos.y, field_0x294[1], 0.3f, field_0x274 * std::fabsf(mPos.y - field_0x294[1]));
        }
    }
    switch (field_0x230) {
    case 0:
        if (getGameView_802D6A9C()->fn_802D800C((field_0x90 >> 7) & 0x1F)) {
            Camera_804AA9E4* cam = gGameRoot2_8053AB10->field_0x24;
            bool visible = false;
            JGeometry::TVec3f p1(x0, y0, 0.0f);
            if (cam->fn_8030C2A8(&p1)) {
                JGeometry::TVec3f p2(x1, y1, 0.0f);
                if (cam->fn_8030C2A8(&p2)) {
                    visible = true;
                }
            }
            if (visible) {
                fn_8022ADE4();
                field_0x230 = 1;
            }
        }
        break;
    case 1: {
        Camera_804AA9E4* cam = gGameRoot2_8053AB10->field_0x24;
        if (cam->field_0x7f5 == 0) {
            cam->field_0x7f4 = 1;
            cam->field_0x7f8 = 4;
            cam->field_0x7fc = mPos;
            cam->field_0x7f6 = 0;
        }
        if (field_0x258 == 1 && field_0x234 == 1) {
            getGameView_802D6A9C()->fn_802D8050((field_0x90 >> 7) & 0x1F, false, 0xFF);
        }
        if (field_0x238 == 0) {
            field_0x258++;
            switch (field_0x258) {
            case 1:
                field_0x234 = 10;
                field_0x238 = 180;
                break;
            case 3:
                field_0x1a8 = true;
                getActorManager_801F856C()->fn_801F6204(0);
                // fall through
            case 2:
                field_0x234 = 30;
                field_0x238 = 30;
                break;
            }
            if (field_0x258 > 3) {
                field_0x258 = 4;
                field_0x234 = 10;
                field_0x254 = (s32)(4.0f * (gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f));
                field_0x254 &= 3;
                field_0x274 = 0.0f;
                field_0x294[0] = field_0x2b8[field_0x254].x;
                field_0x294[1] = field_0x2b8[field_0x254].y;
                field_0x230 = 2;
            }
        }
        break;
    }
    case 2: {
        f32 dx = field_0x294[0] - mPos.x;
        f32 dy = field_0x294[1] - mPos.y;
        if (std::sqrtf(dx * dx + dy * dy) < 2.0f) {
            if (field_0x258 == 3) {
                field_0x23c = 180;
                field_0x248 = 0;
                field_0x230 = 4;
            } else {
                field_0x24c++;
                if (field_0x24c > 3 && (gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f) < 0.3f) {
                    field_0x274 = 0.0f;
                    field_0x248 = 0;
                    field_0x24c = 0;
                    field_0x254 += (s32)(2.99f * (gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f)) + 1;
                    field_0x254 &= 3;
                    field_0x294[0] = field_0x2b8[field_0x254].x;
                    field_0x294[1] = field_0x2b8[field_0x254].y;
                    JGeometry::TVec3f pos(mPos);
                    field_0x26c = getActorManager_801F856C()->spawnActor_801F9484('BPOH', field_0x4, &pos, 1, -1, -1);
                    if (field_0x26c >= 0) {
                        field_0x258 = 5;
                        field_0x234 = 10;
                        field_0x238 = 3;
                        field_0x280 = 0.2f * (f32)field_0x25c;
                        field_0x27c = 1.5f * (0.2f * (f32)field_0x25c);
                        field_0x28c = 0.0f;
                        field_0x23c = 300;
                        field_0x230 = 3;
                    }
                } else {
                    field_0x284 = 0.4f;
                    field_0x274 = 0.0f;
                    field_0x254 += (s32)(2.99f * (gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f)) + 1;
                    field_0x254 &= 3;
                    field_0x294[0] = field_0x2b8[field_0x254].x;
                    field_0x294[1] = field_0x2b8[field_0x254].y;
                    field_0x230 = 2;
                }
            }
        }
        break;
    }
    case 3: {
        s32 serial = field_0x26c;
        if (serial >= 0) {
            cls_804A0440* child = static_cast<cls_804A0440*>(getActorManager_801F856C()->findActorBySerial_801F666C(serial));
            if (child != NULL) {
                child->field_0x294[6] = mPos.x;
                child->field_0x294[7] = mPos.y;
                child->field_0x294[8] = mPos.z;
                child->field_0x290 = (u16)field_0x290 + 0x8000;
                if (field_0x23c != 0) {
                    field_0x290 = (u16)field_0x290 + (s32)field_0x28c;
                    addCalc2_801F71A4(&field_0x28c, 2000.0f, 0.3f, 10.0f);
                }
                child->field_0x294[6] += (10.0f * (f32)field_0x25c) * JMath::sincosTable_.sinShort((u16)field_0x290);
                child->field_0x294[7] += (10.0f * (f32)field_0x25c) * JMath::sincosTable_.cosShort((u16)field_0x290);
            }
            fn_801F2B7C(field_0x114, -1, 9);
            if (field_0x23c == 0) {
                field_0x28c = (u16)field_0x290;
                addCalc2_801F71A4(&field_0x28c, 0.0f, 0.3f, 100.0f);
                field_0x290 = (s32)field_0x28c;
                if ((u16)field_0x290 < 0x10) {
                    field_0x290 = 0;
                    child->field_0x94 = 1;
                    field_0x258 = 4;
                    field_0x234 = 10;
                    field_0x230 = 2;
                }
            }
        }
        break;
    }
    case 4:
        if (field_0x258 == 3) {
            startSound_801F0E34(0x40020, 0);
        }
        if (gGameRoot_8053AAF8->fn_80138A30() > 1 && field_0x258 == 3) {
            if (!getGameView_802D6A9C()->fn_802D800C((field_0x90 >> (27 - field_0x254 * 5)) & 0x1F)) {
                field_0x23c = 0;
                field_0x248 = 60;
            }
        }
        if (field_0x23c == 0) {
            field_0x284 = 0.01f;
            field_0x280 = 0.2f * (f32)field_0x25c;
            if (std::fabsf(field_0x27c - field_0x280) <= 0.1f) {
                field_0x248++;
                if (field_0x25c == 1) {
                    field_0x248 += 9;
                }
                if (field_0x248 > 60) {
                    field_0x258 = 4;
                    field_0x234 = 10;
                    startSound_801F0E34(0x40022, 0);
                    field_0x284 = 0.4f;
                    field_0x274 = 0.0f;
                    field_0x254 += (s32)(2.99f * (gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f)) + 1;
                    field_0x254 &= 3;
                    field_0x294[0] = field_0x2b8[field_0x254].x;
                    field_0x294[1] = field_0x2b8[field_0x254].y;
                    field_0x230 = 2;
                }
            }
        }
        break;
    case 5:
        field_0x258 = 5;
        field_0x234 = 10;
        field_0x280 = 10.0f;
        field_0x278 = 0.2f;
        field_0x274 = 0.0f;
        field_0x294[0] = field_0x294[3];
        field_0x294[1] = field_0x294[4];
        field_0x294[2] = field_0x294[5];
        field_0x230 = 6;
        // fall through
    case 6: {
        f32 dx = field_0x294[0] - mPos.x;
        f32 dy = field_0x294[1] - mPos.y;
        addCalc2_801F71A4(&field_0x27c, field_0x280, 0.5f, 1.0f);
        if (std::sqrtf(dx * dx + dy * dy) < 2.0f && std::fabsf(field_0x280 - field_0x27c) < 0.1f) {
            field_0x280 = 0.2f * (f32)field_0x25c;
            field_0x230 = 7;
        }
        break;
    }
    case 7:
        addCalc2_801F71A4(&field_0x27c, field_0x280, 0.5f, 1.0f);
        if (field_0x27c < 0.2f * (f32)field_0x25c + 0.1f) {
            if (field_0x4 < 8) {
                gGameRoot2_8053AB10->field_0x20->fn_802F601C(&mPos, 2, NULL, 1.0f, 1.0f);
            }
            startSound_801F0E34(0x40021, 0);
            for (s32 i = 0, param = 0; i < 6; i++, param += 0x2AAA00) {
                JGeometry::TVec3f pos(mPos);
                getActorManager_801F856C()->spawnActor_801F9484('POOH', field_0x4, &pos, param | 10, -1, -1);
            }
            field_0x27c = 1.5f * (0.2f * (f32)field_0x25c);
            field_0x23c = 20;
            field_0x230 = 8;
        }
        break;
    case 8:
        if (field_0x23c == 0) {
            field_0x258 = 4;
            field_0x234 = 10;
            field_0x254 += (s32)(2.99f * (gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f)) + 1;
            field_0x254 &= 3;
            field_0x274 = 0.0f;
            field_0x278 = 0.05f;
            if (field_0x25c == 1) {
                field_0x278 = 0.2f;
            }
            field_0x294[0] = field_0x2b8[field_0x254].x;
            field_0x294[1] = field_0x2b8[field_0x254].y;
            field_0x27c = 0.2f * (f32)field_0x25c;
            field_0x230 = 2;
        }
        break;
    case 9:
        field_0x294[0] = field_0x294[3];
        field_0x294[1] = field_0x294[4];
        field_0x294[2] = field_0x294[5];
        lbl_8053AB58 = 1;
        field_0x27c = 0.2f;
        field_0x280 = 1.0f;
        field_0x284 = 0.4f;
        field_0x258 = 6;
        field_0x234 = 10;
        field_0x238 = 100;
        field_0x244 = 0;
        field_0x230 = 10;
        // fall through
    case 10:
        addCalc2_801F71A4(&field_0x27c, field_0x280, 0.1f, field_0x284);
        if (std::fabsf(field_0x27c - field_0x280) < 0.1f) {
            ActorBase_804A5770* list[0x400];
            s32 count = getActorManager_801F856C()->fn_801F7818('POOH', field_0x4, list, -1);
            for (s32 i = 0; i < count; i++) {
                cls_804A0440* pooh = static_cast<cls_804A0440*>(list[i]);
                if (pooh != NULL) {
                    pooh->field_0x234 = 1;
                }
            }
            vfunc_0x24();
        }
        break;
    case 11:
        break;
    case 12:
        if (field_0x2e8 == NULL) {
            JGeometry::TVec3f emitPos(field_0x294[6], field_0x294[7], field_0x294[8]);
            field_0x2e8 = spawnEmitter_8013CC50(field_0x4, &emitPos, 0x53, 1, 1, NULL, NULL, true, field_0x288);
        }
        field_0x230 = 13;
        // fall through
    case 13:
        if (field_0x234 == 0) {
            mPos.x = field_0x294[6];
            mPos.y = field_0x294[7];
            mPos.z = field_0x294[8];
            field_0x3c.x = -20.0f * JMath::sincosTable_.sinShort((u16)field_0x290);
            field_0x3c.y = -40.0f * JMath::sincosTable_.cosShort((u16)field_0x290);
            field_0x234 = 3;
        }
        mPos.add(field_0x3c);
        fn_801F2B7C(field_0x114, -1, 9);
        if (field_0x2e8 != NULL) {
            JGeometry::TVec3f emitPos(field_0x294[6], field_0x294[7], field_0x294[8]);
            setEmitterPos_8013CB44(field_0x2e8, &emitPos, 1, 1, true);
            JPAGetXYZRotateMtx(0, field_0x290, -0x8000, field_0x2e8->mGlobalRot);
        }
        startSound_801F0E34(0x30011, 0);
        if (field_0x94 == 1) {
            if (field_0x2e8 != NULL) {
                field_0x2e8->becomeInvalidEmitter();
            }
            mAlive = false;
        }
        break;
    case 14:
        field_0x238 = 100;
        field_0x23c = 500;
        field_0x230 = 15;
        // fall through
    case 15:
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
        if (field_0x238 == 0) {
            field_0x294[4] = -0.5f;
            if (mPos.z > 0.0f) {
                field_0x3c.z -= 1.0f;
            }
            if (mPos.z <= 0.0f) {
                mPos.z = 0.0f;
                field_0x3c.z = 3.0f;
                startSound_801F0E34(0x50086, 0);
                if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f < 0.5f) {
                    field_0x294[3] = 0.5f;
                } else {
                    field_0x294[3] = -0.5f;
                }
            }
            if (field_0x234 == 0) {
                field_0x244++;
                field_0x244 &= 3;
                field_0x234 = 8;
                field_0x258 = lbl_804A0430[field_0x244];
            }
            if (field_0x23c == 0 || fn_801F6758(&mPos, field_0x4, 0.0f)) {
                mAlive = false;
            }
        }
        addCalc2_801F71A4(&field_0x3c.x, field_0x294[3], 0.3f, 1.0f);
        addCalc2_801F71A4(&field_0x3c.y, field_0x294[4], 0.3f, 1.0f);
        mPos.add(field_0x3c);
        break;
    }
}

// 0x80162208.
// FIXME(shotgun): 96.91 %, missing/extra + reloc-name
void cls_804A0440::draw() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    if (field_0x268 == 1) {
        return;
    }
    if (field_0x268 == 2) {
        params.field_0x1d = 3;
        params.field_0x1c = 1;
        fn_801F02BC("enemy_both2_sob.bin", field_0x258 + 0x1B1, &params, 0, 0, false, 1.0f, 1.0f);
        return;
    }
    params.field_0x1c = 1;
    if (field_0x258 >= 3) {
        params.field_0x1d = 2;
    } else {
        params.field_0x1d = -1;
    }
    if (field_0x188 == 0 && field_0xd8 > 0) {
        s32 start = field_0xdc - 25;
        if (field_0xd8 > start || field_0xdc < 25) {
            s32 v;
            if (field_0xdc < 25) {
                v = field_0xd8 * 255 / field_0xdc;
            } else {
                v = (field_0xd8 - start) * 255 / 25;
            }
            if (field_0xf8 == 2) {
                v = (s32)(v - 6.0f * mPos.z);
                if (v < 0) {
                    v = 0;
                }
            }
            if (field_0xfc == 5) {
                params.field_0x14.r += v;
                params.field_0x14.g += v >> 1;
            } else {
                params.field_0x14.r += v;
                params.field_0x14.g += v;
                params.field_0x14.b += v;
            }
        }
    }
    if (field_0x188 > 0) {
        params.field_0x14.r = 200;
        params.field_0x14.g = 0x41;
    }
    f32 angle = (u16)field_0x290 / -10430.228f;
    JGeometry::TVec3f rot(0.0f, angle, 0.0f);
    setRotScaleMtx(mtx, rot, field_0x27c);
    params.field_0x2d = 1;
    params.mMtx = mtx;
    fn_801618D0(&mPos, &params);
}

// Not auto-inlined: update calls fn_80161F98 out of line in the original (docs/INLINING.md, pragma rule 1).
#pragma auto_inline off
// 0x80161F98.
// FIXME(shotgun): 67.72 %, missing/extra
void cls_804A0440::fn_80161F98(s32 set, s32 frames) {
    if (field_0x2ec != set) {
        if (field_0x2ec < 0) {
            for (s32 i = 0; i < 3; i++) {
                field_0x2f4[i] = lbl_804661E0[set * 3 + i];
            }
        }
        field_0x2ec = set;
        if (field_0x2f0 < 0) {
            field_0x2f0 = 0;
        } else {
            field_0x2f0 = frames;
        }
    } else if (field_0x2f0 > 0) {
        f32 weight = 1.0f / field_0x2f0;
        for (s32 i = 0; i < 3; i++) {
            JUtility::TColor& c = field_0x2f4[i];
            c.r = (u8)((f32)c.r * (1.0f - weight) + weight * (f32)lbl_804661E0[field_0x2ec * 3 + i].r);
            c.g = (u8)((f32)c.g * (1.0f - weight) + weight * (f32)lbl_804661E0[field_0x2ec * 3 + i].g);
            c.b = (u8)((f32)c.b * (1.0f - weight) + weight * (f32)lbl_804661E0[field_0x2ec * 3 + i].b);
            c.a = (u8)((f32)c.a * (1.0f - weight) + weight * (f32)lbl_804661E0[field_0x2ec * 3 + i].a);
        }
        field_0x2f0--;
    }
}
#pragma auto_inline reset

// 0x80161F20.
void cls_804A0440::vfunc_0x24() {
    startSound_801F0E34(0x40015, 0);
    vfunc_0x28(7, 0.0f, 16.0f);
    s32 bit = field_0x270;
    if (bit != 0) {
        getGameView_802D6A9C()->fn_802D8050(bit, true, 0xFF);
    }
}

// 0x801618D0.
// FIXME(shotgun): 99.99 %, operands (the texture names sit after the extern colour table: +0x20 here, +0x60 in the original)
void cls_804A0440::fn_801618D0(const JGeometry::TVec3f* pos, SpriteParams_801F5AE0* params) {
    JUTTexture tex0(static_cast<const ResTIMG*>(fn_804032E8('BPOH', "obake_sirokuro.bti")), 0);
    JUTTexture tex1(static_cast<const ResTIMG*>(fn_804032E8('BPOH', "obake_ci.bti")), 0);
    JUTTexture tex2(static_cast<const ResTIMG*>(fn_804032E8('BPOH', "obake_eye.bti")), 0);
    f32 w = tex0.getWidth();
    f32 h = tex0.getHeight();
    JGeometry::TVec3f out;
    fn_802F5040(pos, params, &out);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumChans(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTexGens(1);
    tex0.load(GX_TEXMAP0);
    tex1.load(GX_TEXMAP1);
    tex2.load(GX_TEXMAP2);

    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_KONST, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_TEXA, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevKColorSel(GX_TEVSTAGE0, GX_TEV_KCSEL_K0);
    GXSetTevKAlphaSel(GX_TEVSTAGE0, GX_TEV_KASEL_K0_A);

    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_CPREV, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_KONST, GX_CA_APREV, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevKColorSel(GX_TEVSTAGE1, GX_TEV_KCSEL_K0);
    GXSetTevKAlphaSel(GX_TEVSTAGE1, GX_TEV_KASEL_K0_A);

    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD0, GX_TEXMAP2, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_C1, GX_CC_TEXC, GX_CC_CPREV);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_A1, GX_CA_TEXA, GX_CA_APREV);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevSwapMode(GX_TEVSTAGE2, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevKColorSel(GX_TEVSTAGE2, GX_TEV_KCSEL_K0);
    GXSetTevKAlphaSel(GX_TEVSTAGE2, GX_TEV_KASEL_K0_A);

    GXSetTevOrder(GX_TEVSTAGE3, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE3, GX_CC_ZERO, GX_CC_CPREV, GX_CC_C2, GX_CC_KONST);
    GXSetTevAlphaIn(GX_TEVSTAGE3, GX_CA_ZERO, GX_CA_APREV, GX_CA_A2, GX_CA_KONST);
    GXSetTevColorOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevSwapMode(GX_TEVSTAGE3, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevKColorSel(GX_TEVSTAGE3, GX_TEV_KCSEL_K1);
    GXSetTevKAlphaSel(GX_TEVSTAGE3, GX_TEV_KASEL_K1_A);
    GXSetTevDirect(GX_TEVSTAGE3);
    GXSetNumTevStages(4);

    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetColorUpdate(GX_TRUE);
    GXSetCullMode(GX_CULL_NONE);
    GXSetTevColor(GX_TEVREG0, field_0x2f4[0]);
    GXSetTevKColor(GX_KCOLOR0, field_0x2f4[1]);
    GXSetTevColor(GX_TEVREG1, field_0x2f4[2]);
    if (params != NULL) {
        GXSetTevColor(GX_TEVREG2, params->field_0x10);
        GXSetTevKColor(GX_KCOLOR1, params->field_0x14);
    } else {
        static const GXColor white = {0xFF, 0xFF, 0xFF, 0xFF};
        GXSetTevColor(GX_TEVREG2, white);
        GXSetTevKColor(GX_KCOLOR1, lbl_80543F30);
    }
    out.x -= 0.5f * w;
    out.z -= 0.5f * h;
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(out.x, out.y, out.z + h);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(out.x + w, out.y, out.z + h);
    GXTexCoord2f32(1.0f, 0.0f);
    GXPosition3f32(out.x + w, out.y, out.z);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(out.x, out.y, out.z);
    GXTexCoord2f32(0.0f, 1.0f);
}
