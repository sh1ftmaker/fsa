#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Unk_8045A080.h"
#include "game/act/tu_801F9484.h"

// 0x802E3A48.
cls_804A9C20::~cls_804A9C20() {
    for (s32 i = 0; i < 4; i++) {
        if (mEmitters2c8[i] != NULL) {
            releaseEmitter_801EE434(mEmitters2c8[i]);
        }
        if (mEmitters2d8[i] != NULL) {
            releaseEmitter_801EE434(mEmitters2d8[i]);
        }
    }
    if (mEmitter2e8 != NULL) {
        releaseEmitter_801EE434(mEmitter2e8);
    }
    if (mEmitter2ec != NULL) {
        releaseEmitter_801EE434(mEmitter2ec);
    }
    if (field_0x238 != 5) {
        gAudioMgr_8053AF80->fn_80457E54(false);
    }
}

// 0x802E2998.
// FIXME(shotgun): 88.18 %, missing/extra + reloc-name
void cls_804A9C20::draw() {
    if (field_0x230 <= 3) {
        JGeometry::TVec3f saved = mPos;
        for (s32 i = 0; i < 4; i++) {
            mPos = field_0x258[i];
            if (field_0x288[i] == 1) {
                drawSprite_801F06F0("enemy_gc_sob.bin", 0x25A, NULL, 7, 1.0f, 1.0f);
            } else {
                drawSprite_801F06F0("enemy_gc_sob.bin", 0x25B, NULL, 7, 1.0f, 1.0f);
            }
        }
        mPos = saved;
    }
    if (field_0x238 == 5) {
        SpriteParams_801F5AE0 params;
        params.field_0x1d = -2;
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('TIMG', "shikimono.bti");
        JGeometry::TVec3f pos(mPos.x + 80.0f, mPos.y + 48.0f, mPos.z);
        if (field_0x4 < 8) {
            gGameRoot2_8053AB10->field_0x20->fn_802F745C(&pos, res, &params);
        }
    }
}

// 0x802E2990.
void cls_804A9C20::fn_802E2990(u8 value) {
    field_0x2f4 = value;
}
