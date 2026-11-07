#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Unk_8045A080.h"
#include "game/act/tu_801F9484.h"
#include "game/Effect.h"

// The 'GOL2' offset tables in rodata (per mode and sprite; only the part init reads is
// described, the rest of the 0x680 bytes belongs to update).
struct GoalTables_8046EA10 {
    f32 x1[6][4];
    f32 y1[6][4];
    f32 x2[6][4];
    f32 y2[6][4];
    u16 id[6][4];
};
extern const GoalTables_8046EA10 lbl_8046EA10;

// 0x802E3B2C.
// FIXME(shotgun): 79.6 %, missing/extra + reloc-name
void cls_804A9C20::init() {
    field_0x238 = field_0x90 & 0x1F;
    field_0x240 = (field_0x90 >> 12) & 0x1F;
    field_0x244 = (field_0x90 >> 7) & 1;
    field_0x23c = gGameRoot_8053AAF8->fn_80138A30();
    for (s32 i = 0; i < 4; i++) {
        mEmitters2c8[i] = NULL;
        mEmitters2d8[i] = NULL;
    }
    mEmitter2e8 = NULL;
    mEmitter2ec = NULL;
    field_0x2f4 = 0xFF;
    if (!getGameView_802D6A9C()->fn_802D800C(field_0x240)) {
        for (s32 i = 0; i < 4; i++) {
            field_0x258[i].x = mPos.x + lbl_8046EA10.x1[field_0x238][i];
            field_0x258[i].y = mPos.y + lbl_8046EA10.y1[field_0x238][i];
            field_0x258[i].z = 0.0f;
            field_0x298[i] = 0;
            field_0x2a8[i] = 0;
            field_0x288[i] = 1;
            mEmitters2c8[i] = spawnEmitter_8013CC50(field_0x4, &field_0x258[i], 0x10A, 0, 0, NULL, NULL,
                                                    true, 1.0f);
        }
        if (field_0x238 != 5) {
            for (s32 i = 0; i < 4; i++) {
                field_0x2b8[i] = lbl_8046EA10.id[field_0x238][i];
                if (lbl_8046EA10.id[field_0x238][i] != 0) {
                    JGeometry::TVec3f pos(field_0x258[i].x + lbl_8046EA10.x2[field_0x238][i],
                                          field_0x258[i].y + lbl_8046EA10.y2[field_0x238][i],
                                          field_0x258[i].z);
                    mEmitters2d8[i] = spawnEmitter_8013CC50(field_0x4, &pos, lbl_8046EA10.id[field_0x238][i],
                                                            1, 1, NULL, NULL, true, 1.0f);
                }
            }
        }
        field_0x230 = 0;
        field_0x234 = 0;
        field_0x24c = mPos;
    } else {
        field_0x24c = mPos;
        field_0x230 = 5;
    }
    if (field_0x238 == 5) {
        JGeometry::TVec3f pos(mPos.x + 80.0f, mPos.y + 48.0f, mPos.z);
        mEmitter2e8 = spawnEmitter_8013CC50(field_0x4, &pos, 0xC6, 0, 0, NULL, NULL, true, 1.0f);
        JGeometry::TVec3f pos2(mPos.x + 80.0f, mPos.y + 48.0f, mPos.z);
        mEmitter2ec = spawnEmitter_8013CC50(field_0x4, &pos2, 0xC7, 0, 0, NULL, NULL, true, 1.0f);
    }
    field_0x248 = 0;
    if (field_0x238 != 5) {
        gAudioMgr_8053AF80->fn_80457E54(true);
    }
}

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
