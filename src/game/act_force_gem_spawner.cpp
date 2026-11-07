#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/SpriteParams.h"
#include "game/act/act_force_gem_spawner.h"

// FIXME(shotgun): not written (size 0x2BC): fn_8032A4FC
// FIXME(shotgun): not written (size 0xEBC): update (0x80329640)

// FIXME(shotgun): 91.5 %, scheduling + reloc-name
void ForceGemSpawner_804AB660::init() {
    mEmitter24c = NULL;
    mEmitter250 = NULL;
    mEmitter254 = NULL;
    field_0x238 = field_0x90 & 0xFFFF;
    field_0x234 = 0;
    field_0x244 = 0.0f;
    field_0x248 = 1.0f;
    field_0x240 = 11;
    field_0x23c = (field_0x90 >> 20) & 0xF;
    switch ((field_0x90 >> 16) & 0xF) {
    case 1:
        field_0x230 = 1;
        break;
    case 2:
        field_0x238 = 0;
        mBox.i.x = -8.0f;
        mBox.i.y = -8.0f;
        mBox.f.x = 8.0f;
        mBox.f.y = 8.0f;
        field_0x230 = 3;
        break;
    case 3:
        field_0x230 = 4;
        break;
    case 4:
    case 7:
        field_0x230 = 5;
        break;
    case 0:
    case 5:
    case 6:
    default:
        field_0x230 = 0;
        break;
    }
    if (field_0x230 == 1 || field_0x230 == 3 || field_0x230 == 4) {
        if (vfunc_0x34()) {
            fn_801F3668();
        }
    }
}

void ForceGemSpawner_804AB660::draw() {
    if (field_0x230 == 3) {
        SpriteParams_801F5AE0 params;
        Mtx mtx;
        PSMTXIdentity(mtx);
        mtx[0][0] = field_0x248;
        mtx[0][1] = 0.0f;
        mtx[0][2] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = field_0x248;
        mtx[1][2] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = field_0x248;
        params.mMtx = mtx;
        JGeometry::TVec3f pos(mPos.x + 8.0f, mPos.y + 8.0f, mPos.z);
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, 0x2a, &params, 1.0f, 1.0f);
        }
    }
}

ForceGemSpawner_804AB660::~ForceGemSpawner_804AB660() {
    releaseEmitter_801EE434(mEmitter24c);
    releaseEmitter_801EE434(mEmitter250);
    releaseEmitter_801EE434(mEmitter254);
}
