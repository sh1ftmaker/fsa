#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/act/tu_80343A70.h"

// 0x803450AC.
// FIXME(shotgun): 95.10 %, missing/extra + reloc-name
void cls_804AC600::init() {
    field_0x270 = field_0x90 & 1;
    mLife = 1;
    for (s32 i = 0; i < 7; i++) {
        field_0x2a4[i].set(0.0f, 0.0f, 0.0f);
        field_0x2f8[i].set(0.0f, 0.0f, 0.0f);
        field_0x34c[i].set(0.0f, 0.0f, 0.0f);
        field_0x278[i] = 2.0f + gGameRoot_8053AAF8->mRandom.get_ufloat_1();
        field_0x254[i] = 0;
        field_0x294 = 0;
    }
    field_0x278[0] = 1.5f + 0.5f * gGameRoot_8053AAF8->mRandom.get_ufloat_1();
    field_0x2a4[0] = mPos;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x244 = -1;
    field_0x24c = -1;
    field_0x250 = 0;
    field_0x2a2 = 0;
    field_0x248 = -1;
    field_0x274 = 0;
    field_0x234 = 0;
    mBox.set(-4.0f, -4.0f, 4.0f, 4.0f);
    field_0x230 = 0;
    field_0x194 = 1;
}

// 0x80343B48.
// FIXME(shotgun): 93.18 %, missing/extra
void cls_804AC600::draw() {
    SpriteParams_801F5AE0 params;
    if (field_0x274 == 2) {
        params.field_0x20 = 0;
    }
    if (field_0x230 != 0) {
        for (s32 i = 0; i < 7; i++) {
            if (i > 0) {
                mPos = field_0x2a4[0] + field_0x2a4[i];
            }
            if (mPos.z <= 0.0f) {
                mPos.z = 0.1f;
            }
            drawSprite_801F06F0("enemy_both2_sob.bin", field_0x240 + field_0x254[0] + 0x1CC, &params, 1, 1.0f, 1.0f);
        }
        mPos = field_0x2a4[0];
        if (field_0x274 == 2 && mPos.z <= 0.0f) {
            mPos.z = 0.1f;
        }
    }
}

// 0x80343AD0.
// FIXME(shotgun): 96.67 %, missing/extra
void cls_804AC600::vfunc_0x24() {
    s32 kind = 0;
    if (field_0x274 == 1) {
        kind = 3;
    } else if (field_0x274 == 2) {
        kind = 1;
    }
    getActorManager_801F856C()->fn_801F8544(ACTOR_RUPY, field_0x4, &mPos, kind | 0x261E00);
    ActorBase_804A5770::vfunc_0x24();
}

// Destructor at 0x80343A70.
cls_804AC600::~cls_804AC600() {}
