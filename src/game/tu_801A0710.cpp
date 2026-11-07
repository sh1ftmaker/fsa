#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Effect.h"
#include "game/EmitterDelete.h"
#include "game/GameState.h"
#include "game/SpriteParams.h"
#include "game/Unk_8045A080.h"
#include "game/cls_804B1D00.h"
#include "game/ActorManager.h"
#include "game/Camera.h"
#include "game/Player.h"
#include "game/act/tu_801A0710.h"
#include "JSystem/JMath/JMath.h"
#include "std/math.h"

void fn_8022AF2C();

f32 lbl_8053A0F8[2] = {2.0f, 0.0f};
s32 lbl_8053A100 = -1;
s32 lbl_8053A104 = 0; // second word of the 8-byte object at 0x8053A100
s32 lbl_8053ABC0;
u8 lbl_8053ABC4;
u8 lbl_8053ABC5;

// The four TIMG pictures of the shop keeper.
const char* lbl_804A2BE4[4] = {"obachan_1.bti", "obachan_2.bti", "obachan_3.bti", "obachan_4.bti"};
const s32 lbl_804A2BD0[5] = {1, 5, 20, 50, 100};
const s32 lbl_804A2B90[16] = {0, 1, 2, 0, 0, 1, 2, 0, 0, 1, 2, 0, 0, 1, 2, 0};

// FIXME(shotgun): 93.8 %, missing/extra + reloc-name
void TennisShop_804A2BF4::init() {
    field_0x248 = 0;
    for (s32 i = 0; i < 6; i++) {
        field_0x24c[i] = 0;
    }
    for (s32 i = 0; i < 4; i++) {
        field_0x238[i] = 0;
    }
    for (s32 i = 0; i < 20; i++) {
        field_0x2c8[i] = 0;
    }
    for (s32 i = 0; i < 4; i++) {
        field_0x28c[i] = 0;
        field_0x27c[i] = -1;
        field_0x29c[i] = 0;
        field_0x2ac[i] = 0;
        field_0x2bc[i] = 0;
        mEmitters[i] = NULL;
    }
    field_0x31c = 0;
    field_0x2c4 = -1;
    field_0x318 = 0;
    field_0x264 = 0;
    field_0x268 = 0;
    field_0x33c = 0.0f;
    for (s32 i = 0; i < 6; i++) {
        field_0x324[i] = 1.0f;
    }
    field_0x340 = 255.0f;
    field_0x278 = -1;
    field_0x2c0 = 0;
    field_0x320 = -1;
    field_0x26c = 0;
    field_0x270 = 0;
    field_0x274 = field_0x90 & 0xF;
    switch (field_0x274) {
    case 0:
        if (field_0x238[0] == 0) {
            field_0x264 = lbl_804A2B90[field_0x98 * 4 + field_0x248];
            if (field_0x248 & 1) {
                field_0x238[0] = 10;
            } else {
                field_0x238[0] = 12;
            }
            field_0x248++;
            field_0x248 &= 3;
        }
        field_0xb0 = 0xA040004;
        mBox.i.x = -24.0f;
        mBox.i.y = -12.0f;
        mBox.f.x = 24.0f;
        mBox.f.y = 8.0f;
        mBox2.i.x = -28.0f;
        mBox2.i.y = -10.0f;
        mBox2.f.x = 28.0f;
        mBox2.f.y = 10.0f;
        field_0x98 = 2;
        lbl_8053ABC0 = 0;
        lbl_8053ABC5 = 0;
        lbl_8053ABC4 = 0;
        lbl_8053A0F8[0] = 2.0f;
        if (vfunc_0x34()) {
            field_0x230 = 0xB;
        } else {
            field_0x230 = 0;
        }
        break;
    case 1:
        mLife = 10000;
        mBox.i.x = -10.0f;
        mBox.i.y = -10.0f;
        mBox.f.x = 10.0f;
        mBox.f.y = 10.0f;
        field_0x80.i.x = -2.0f;
        field_0x80.i.y = -2.0f;
        field_0x80.f.x = 2.0f;
        field_0x80.f.y = 2.0f;
        field_0x230 = 0xE;
        break;
    case 2:
        mLife = 10000;
        mBox.i.x = -10.0f;
        mBox.i.y = -10.0f;
        mBox.f.x = 10.0f;
        mBox.f.y = 10.0f;
        field_0x80.i.x = -2.0f;
        field_0x80.i.y = -2.0f;
        field_0x80.f.x = 2.0f;
        field_0x80.f.y = 2.0f;
        field_0x230 = 0x12;
        break;
    }
}

TennisShop_804A2BF4::~TennisShop_804A2BF4() {
    if (field_0x274 == 0 && (field_0x230 == 5 || field_0x230 == 6)) {
        fn_8022AF2C();
        if (gGameRoot_8053AAF8->field_0x4c != 9) {
            gGameRoot_8053AAF8->fn_80138B7C();
            gAudioMgr_8053AF80->fn_80458508(0);
        }
    }
    for (s32 i = 0; i < 4; i++) {
        releaseEmitter_801EE434(mEmitters[i]);
    }
}

void TennisShop_804A2BF4::update() {
    switch (field_0x274) {
    case 0:
        fn_801A17B8();
        break;
    case 1:
        fn_801A0F90();
        break;
    case 2:
        fn_801A0BE4();
        break;
    }
}

// FIXME(shotgun): 85.9 %, missing/extra
void TennisShop_804A2BF4::fn_801A0BE4() {
    mPos.x += field_0x3c.x;
    mPos.y += field_0x3c.y;
    mPos.z += field_0x3c.z;
    gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
    switch (field_0x230) {
    case 0x12: {
        s32 serial = field_0x320;
        if (serial < 0 || getActorManager_801F856C()->findActorBySerial_801F666C(serial) == NULL) {
            field_0x238[1] = 50;
            field_0x3c.x = 0.0f;
            field_0x3c.y = 5.0f;
            field_0x3c.z = 3.0f;
            startSound_801F0E34(0x3004D, 0);
            field_0x230 = 0x13;
            break;
        }
        for (s32 i = 0; i < 4; i++) {
            if (field_0x27c[i] >= 0) {
                f32 dx = mPos.x - getPlayerPos_8023E724(i)->x;
                f32 dy = mPos.y - getPlayerPos_8023E724(i)->y;
                f32 dist = std::sqrtf(dx * dx + dy * dy);
                if (dist > 16.0f) {
                    s32 angle = (s32)(JMath::atanTable_.atan2_(-dx, -dy) * 10430.378f);
                    field_0x3c.x = 10.0f * JMath::sincosTable_.sinShort(angle);
                    field_0x3c.y = 10.0f * JMath::sincosTable_.cosShort(angle);
                } else {
                    field_0x2ac[i] = 1;
                    field_0x238[1] = 50;
                    field_0x3c.x *= -0.5f;
                    field_0x3c.y *= -0.5f;
                    field_0x3c.z = 3.0f;
                    startSound_801F0E34(0x3004D, 0);
                    field_0x230 = 0x13;
                }
            }
        }
        break;
    }
    case 0x13:
        addCalc2_801F71A4(&field_0x340, 0.0f, 0.5f, 5.0f);
        if (mPos.z <= 0.0f) {
            startSound_801F0E34(0x3004C, 0);
            mPos.z = 0.0f;
            field_0x3c.z = 3.0f;
        } else {
            field_0x3c.z -= 0.5f;
        }
        if (field_0x238[1] != 0 && field_0x340 < 1.0f) {
            mAlive = false;
        }
        break;
    }
}

// Not auto-inlined: draw calls fn_801A0910 / fn_801A0754 out of line in the original (docs/INLINING.md, pragma rule 1).
#pragma auto_inline off
// FIXME(shotgun): 87.3 %, missing/extra + reloc-name
void TennisShop_804A2BF4::fn_801A0910() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    f32 savedY = mPos.y;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    mtx[0][0] = field_0x324[0];
    mtx[0][1] = 0.0f;
    mtx[0][2] = 0.0f;
    mtx[1][0] = 0.0f;
    mtx[1][1] = field_0x324[1];
    mtx[1][2] = 0.0f;
    mtx[2][0] = 0.0f;
    mtx[2][1] = 0.0f;
    mtx[2][2] = field_0x324[1];
    params.mMtx = mtx;
    mPos.y = savedY + (field_0x33c - 16.0f);
    fn_801EFD34(fn_804032E8('TIMG', lbl_804A2BE4[field_0x264]), &params, 7, 0.0f, -2.0f, 0.0f);
    mPos.y = savedY;
    if (field_0x2c4 >= 0) {
        lbl_8053AF28->field_0x10 = (u8)field_0x26c;
        lbl_8053AF28->field_0x14 |= 1;
        lbl_8053AF28->field_0x12 = field_0x2c4 / 60;
        lbl_8053AF28->field_0x13 = (s32)(100.0f * (f32)(field_0x2c4 % 60) / 60.0f);
        lbl_8053AF28->field_0x14 |= 2;
    }
}

// FIXME(shotgun): 99.8 %, reloc-name
void TennisShop_804A2BF4::fn_801A0754() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    mtx[0][0] = 1.2f;
    mtx[0][1] = 0.0f;
    mtx[0][2] = 0.0f;
    mtx[1][0] = 0.0f;
    mtx[1][1] = 1.2f;
    mtx[1][2] = 0.0f;
    mtx[2][0] = 0.0f;
    mtx[2][1] = 0.0f;
    mtx[2][2] = 1.2f;
    if (field_0x274 == 2) {
        mtx[0][1] = 0.0f;
        mtx[0][0] = 2.0f;
        mtx[0][2] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = 2.0f;
        mtx[1][2] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = 2.0f;
    }
    params.mMtx = mtx;
    params.field_0x30 = 0x11;
    params.field_0x10.a = (s32)field_0x340;
    drawSprite_801F06F0("enemy_gc_sob.bin", field_0x264 + 0x2BA, &params, 0, 1.0f, 1.0f);
}

#pragma auto_inline reset

void TennisShop_804A2BF4::draw() {
    switch (field_0x274) {
    case 0:
        fn_801A0910();
        break;
    case 1:
    case 2:
        fn_801A0754();
        break;
    }
}
