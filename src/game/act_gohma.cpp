#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/ActorManager.h"
#include "game/ViewLayer.h"
#include "game/act/act_gohma.h"
#include "game/Camera.h"
#include "game/Effect.h"
#include "game/Player.h"
#include "JSystem/JMath/JMath.h"

static const char* lbl_804A3020[] = {
    "e_goma_yoko_0.bti",
    "e_goma_yoko_1.bti",
    "e_goma_yoko_2.bti",
    "e_goma_yoko_3.bti",
    "e_goma_yoko_0.bti",
    "e_goma_yoko_1.bti",
    "e_goma_yoko_2.bti",
    "e_goma_yoko_3.bti",
    "e_goma_mae_0.bti",
    "e_goma_mae_1.bti",
    "e_goma_mae_2.bti",
    "e_goma_mae_3.bti",
    "e_goma_ushiro_0.bti",
    "e_goma_ushiro_1.bti",
    "e_goma_ushiro_2.bti",
    "e_goma_ushiro_3.bti",
    "e_goma_ake_0.bti",
    "e_goma_ake_1.bti",
    "e_goma_ake_2.bti",
    "e_goma_toji_0.bti",
    "e_goma_toji_1.bti",
    "e_goma_toji_2.bti",
    "e_goma_toji_3.bti",
};

static s32 lbl_8053ABD8;

void Gohma_804A307C::init() {
    mBox.i.x = -10.0f;
    mBox.i.y = -8.0f;
    mBox.f.x = 10.0f;
    mBox.f.y = 8.0f;
    field_0x80.i.x = -12.0f;
    field_0x80.i.y = -8.0f;
    field_0x80.f.x = 12.0f;
    field_0x80.f.y = 8.0f;
    mBox2.i.x = -16.0f;
    mBox2.i.y = -16.0f;
    mBox2.f.x = 16.0f;
    mBox2.f.y = 16.0f;
    field_0x254 = field_0x90 & 0xF;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x24c = 0;
    mTimers[0] = 0;
    mTimers[1] = 0;
    mTimers[2] = 0;
    mTimers[3] = 0;
    mEmitters[0] = NULL;
    mEmitters[1] = NULL;
    field_0xb8 = 1;
    field_0x98 = (s32)(4.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
    field_0x98 &= 3;
    s32 layer = field_0x4;
    if (viewLayerFlag(layer)) {
        field_0x98 &= 1;
    }
    field_0x250 = field_0x98 * 4 + field_0x244;
    field_0x26c = 0.0f;
    field_0x268 = 0.0f;
    field_0x264 = 0.0f;
    mLife = 0x3EF;
    field_0x258 = 3;
    field_0x260 = field_0x98;
    field_0xb0 = 0x0A040010;
    if (field_0x254 == 0 || field_0x254 == 2) {
        field_0x258 = 2;
        field_0xbc = 2;
        field_0x270 = 1.0f;
        field_0x274 = 1.0f;
        field_0x278 = 1.0f;
        s32 layer = field_0x4;
        if (viewLayerFlag(layer)) {
            field_0x80.i.x = -4.0f;
            field_0x80.i.y = -8.0f;
            field_0x80.f.x = 4.0f;
            field_0x80.f.y = 8.0f;
        }
        field_0x230 = 1;
        if (field_0x254 == 2) {
            if (lbl_8053ABD8 == 0) {
                gAudioMgr_8053AF80->fn_80458F9C(0x3C);
            }
            lbl_8053ABD8++;
            field_0x230 = 0;
        }
    } else {
        field_0xbc = 3;
        mLife = 0x3F2;
        field_0x98 = 2;
        field_0xb0 = 0x0A040004;
        field_0x270 = 1.5f;
        field_0x274 = 1.2f;
        field_0x278 = 1.2f;
        field_0xc0 = 3;
        field_0x98 &= 1;
        field_0x250 = 0x13;
        field_0x230 = 8;
        if (field_0x254 == 3) {
            if (lbl_8053ABD8 == 0) {
                gAudioMgr_8053AF80->fn_80458F9C(0x3C);
            }
            lbl_8053ABD8++;
            field_0x230 = 7;
        }
    }
    field_0x25c = mLife;
}

Gohma_804A307C::~Gohma_804A307C() {
    for (int i = 0; i < 2; i++) {
        releaseEmitter_801EE434(mEmitters[i]);
    }
    if (field_0x254 == 2 || field_0x254 == 3) {
        if (--lbl_8053ABD8 == 0) {
            if (gGameRoot_8053AAF8->field_0x4c != 9) {
                gGameRoot_8053AAF8->fn_80138B7C();
            }
        }
    }
}

void Gohma_804A307C::vfunc_0x44() {
    if (field_0x25c == mLife) {
        return;
    }
    if (field_0x258 > 0) {
        field_0x258--;
    }
    if (field_0x258 <= 0) {
        field_0xb0 = 0;
    }
    field_0x25c = mLife;
    if (field_0x254 == 1 || field_0x254 == 3) {
        field_0x26c = 0.0f;
        field_0x268 = 0.0f;
        field_0x264 = 0.0f;
        field_0x3c.z = 0.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.x = 0.0f;
        field_0xb0 = 0x0A040004;
        field_0x248 = 1;
        field_0x230 = 12;
        return;
    }
    u16 angle = (u16)(field_0x98 + 0x8000);
    f32 speed = -10.0f;
    field_0x3c.x = speed * JMASinShort(fn_801F6874(angle));
    field_0x3c.y = speed * JMACosShort(fn_801F6874(angle));
    field_0x230 = 5;
}

void Gohma_804A307C::fn_801AB6A8() {
}

void Gohma_804A307C::fn_801AAF44() {
}

void Gohma_804A307C::update() {
    switch (field_0x254) {
    case 0:
    case 2:
        fn_801AB6A8();
        break;
    case 1:
    case 3:
        fn_801AAF44();
        break;
    }

    for (int i = 0; i < 4; i++) {
        if (mTimers[i] > 0) {
            mTimers[i]--;
        }
    }
}

void Gohma_804A307C::draw() {
    SpriteParams_801F5AE0 params;
    f32 scaleX = field_0x270;
    if (field_0x98 == 0) {
        scaleX *= -1.0f;
    }
    f32 scaleY = field_0x274;
    Mtx mtx;
    mtx[0][0] = scaleX;
    mtx[0][1] = 0.0f;
    mtx[0][2] = 0.0f;
    mtx[1][0] = 0.0f;
    mtx[1][1] = scaleY;
    mtx[1][2] = 0.0f;
    mtx[2][0] = 0.0f;
    mtx[2][1] = 0.0f;
    mtx[2][2] = scaleY;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    params.mMtx = mtx;

    s32 r30 = 4;
    f32 f31 = -5.0f;
    if (field_0x254 == 0 || field_0x254 == 2) {
        r30 = 3;
        f31 = -3.0f;
    }

    void* res = gGameRoot_8053AAF8->field_0xa4->getResource('TIMG', lbl_804A3020[field_0x250]);
    fn_801EFD34(res, &params, r30, 0.0f, f31, 0.0f);
}
