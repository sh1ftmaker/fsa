#include "game/GameAudio.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/act/act_slingshot_seed.h"

// 0x80324AC4.
// FIXME(shotgun): 99.25 %, regalloc-only + reloc-name
void SlingshotSeed_804AB3A8::init() {
    field_0x98 = (field_0x90 >> 4) & 7;
    field_0x94 = (field_0x90 >> 8) & 3;
    field_0x90 = field_0x90 & 3;
    field_0x268 = 0;
    field_0x269 = 0;
    mEmitter26c = NULL;
    field_0x80.set(-2.0f, -2.0f, 2.0f, 2.0f);
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x254 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    switch (field_0x98) {
    case 0:
        field_0x3c.x = 5.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.z = 0.0f;
        break;
    case 1:
        field_0x3c.x = -5.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.z = 0.0f;
        break;
    case 2:
        field_0x3c.x = 0.0f;
        field_0x3c.y = 5.0f;
        field_0x3c.z = 0.0f;
        break;
    case 3:
        field_0x3c.x = 0.0f;
        field_0x3c.y = -5.0f;
        field_0x3c.z = 0.0f;
        break;
    case 4:
        field_0x3c.x = 3.5f;
        field_0x3c.y = 3.5f;
        field_0x3c.z = 0.0f;
        break;
    case 5:
        field_0x3c.x = -3.5f;
        field_0x3c.y = 3.5f;
        field_0x3c.z = 0.0f;
        break;
    case 6:
        field_0x3c.x = 3.5f;
        field_0x3c.y = -3.5f;
        field_0x3c.z = 0.0f;
        break;
    case 7:
        field_0x3c.x = -3.5f;
        field_0x3c.y = -3.5f;
        field_0x3c.z = 0.0f;
        break;
    }
    field_0x234 = 255.0f;
    field_0x240 = 500;
    field_0x274 = 0.0f;
    s32 layer = field_0x4;
    field_0x278 = getGameView_802D6A9C()->field_0x131df[layer];
    if (field_0x94 == 1) {
        s32 layer2 = field_0x4;
        if (getGameView_802D6A9C()->field_0x131df[layer2] == 0) {
            field_0x3c.z = 0.4f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() + 0.9f;
        } else {
            field_0x270 = 0.4f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() + 0.9f;
            field_0x3c.y = field_0x3c.y - (0.2f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() + 0.6f);
        }
    } else if (field_0x94 == 0) {
        if (mEmitter26c == NULL) {
            mEmitter26c = spawnEmitter_8013CC50(field_0x4, &mPos, 0x35D, 0, 1, NULL, NULL, true, 1.0f);
        }
    }
    field_0x25c = 0;
    field_0x260 = 0;
    field_0x264 = 0;
    mLife = 10000;
    field_0x230 = 0;
    field_0x26a = 0;
}

// Destructor at 0x80324570.
SlingshotSeed_804AB3A8::~SlingshotSeed_804AB3A8() {
    releaseEmitter_801EE434(mEmitter26c);
}

// 0x803239A0.
// FIXME(shotgun): 97.24 %, missing/extra + reloc-name
void SlingshotSeed_804AB3A8::draw() {
    SpriteParams_801F5AE0 params;
    params.field_0x10.a = (u8)(s32)field_0x234;
    if (field_0x94 == 1 && field_0x4 < 8) {
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
        gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, 0xE7, &params, 1.0f, 1.0f);
    }
    fn_803075AC(mSerial, field_0x4, &mPos, 0xE7, false, 2, -1, 0, 0, 0);
}
