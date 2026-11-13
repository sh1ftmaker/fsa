#include "game/GameAudio.h"
#include "game/EmitterDelete.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/act/tu_801F9484.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/EnemyBase.h"

// lbl_8046C658 / lbl_8046C6C8: the x and y shake offsets of four steps per shake kind (s32,
// 16 bytes per kind). lbl_8046C6C8 is 0x160 bytes in the binary; only its first 0x70 are written out.
const s32 lbl_8046C658[7][4] = {
    {-24, 40, -24, 40}, {0, 64, 0, 64}, {0, -48, 48, 0}, {0, 32, 64, 96},
    {0, 0, 0, 0}, {0, 48, 112, 160}, {80, -80, 80, -80},
};
const s32 lbl_8046C6C8[7][4] = {
    {-24, -24, 40, 40}, {0, 0, 64, 64}, {0, 48, 48, 96}, {0, 0, 0, 0},
    {0, 32, 64, 96}, {0, 48, 48, 0}, {-32, -32, 80, -80},
};
// FIXME(shotgun): the drop offsets of state 1 (x, then y, by field_0x234), f32 tables inside
// the lbl_8046C6C8 block (+0xA8 and +0xC0); extern-declared under their addresses.
extern const f32 lbl_8046C770[6];
extern const f32 lbl_8046C788[6];

// The four burst effects at each shake offset; the actor then waits 10 frames in state 1.
static inline void burst(cls_804A7EB0* self) {
    JGeometry::TVec3f pos;
    pos.z = self->mPos.z;
    for (s32 i = 0; i < 4; i++) {
        pos.x = self->mPos.x + (f32)lbl_8046C658[self->field_0x234][i];
        pos.y = self->mPos.y + (f32)lbl_8046C6C8[self->field_0x234][i];
        spawnEmitter_8013CC50(self->field_0x4, &pos, 0x202, 1, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(self->field_0x4, &pos, 0x203, 1, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(self->field_0x4, &pos, 0x204, 0, -4, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(self->field_0x4, &pos, 0x205, 1, 0, NULL, NULL, true, 1.0f);
        self->field_0x30c = 60;
        self->field_0x310 = 0;
    }
    self->field_0x244 = 10;
    self->field_0x230 = 1;
}

// 0x802A6600.
// FIXME(shotgun): 71.43 %, missing/extra + reloc-name; behaviour drift.
// the player-mask test needs a bool form and the
// 1000-subtraction loop is unrolled here but not in the original
void cls_804A7EB0::init() {
    mBox.set(-3.0f, -3.0f, 3.0f, 3.0f);
    field_0x230 = 0;
    field_0x3c.z = 0.0f;
    field_0x24c = 9;
    field_0x250 = 0;
    for (s32 i = 0; i < 4; i++) {
        field_0x2a4[i] = -1;
        field_0x254[i] = -1;
        for (s32 j = 0; j < 5; j++) {
            mEmitters[j][i] = NULL;
        }
        field_0x264[i].set(0.0f, 0.0f, 0.0f);
    }
    field_0x304 = 0;
    field_0x30c = 0;
    field_0x310 = 1;
    field_0x238 = field_0x90 & 0x1F;
    field_0x23c = (field_0x90 >> 17) & 0x1F;
    field_0x240 = (field_0x90 >> 22) & 0x1F;
    field_0x234 = (field_0x90 >> 12) & 0x1F;
    u32 flags = (field_0x90 >> 7) & 0x1F;
    if (flags & 1) {
        mPos.x += 8.0f;
    }
    if (flags & 2) {
        mPos.y += 8.0f;
    }
    getGameView_802D6A9C()->fn_802D752C(true);
    getGameView_802D6A9C()->fn_802D7440(false);
    fn_8022ADE4();
    s32 total = 0;
    for (s32 p = 0; p < 4; p++) {
        if ((gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << p)) != 0) {
            total += fn_80236228(p);
        }
    }
    field_0x314 = 0;
    field_0x318 = 0;
    field_0x31c = 1;
    while (total >= 1000) {
        total -= 1000;
        field_0x314++;
    }
}

// 0x802A64E8.
cls_804A7EB0::~cls_804A7EB0() {
    for (s32 i = 0; i < 4; i++) {
        deleteEmitterKeep(&mEmitters[0][i]);
        deleteEmitterKeep(&mEmitters[1][i]);
        deleteEmitterKeep(&mEmitters[2][i]);
        deleteEmitterKeep(&mEmitters[3][i]);
        deleteEmitterKeep(&mEmitters[4][i]);
    }
}

// 0x802A4A4C.
// FIXME(shotgun): 20.02 %, missing/extra + reloc-name; behaviour drift.
// (states 0-2 only), missing/extra + reloc-name
void cls_804A7EB0::update() {
    switch (field_0x230) {
    case 0:
        switch (field_0x238) {
        case 0:
            if (getActorManager_801F856C()->field_0x305c == 1) {
                burst(this);
            }
            break;
        case 1:
            if (vfunc_0x34()) {
                burst(this);
            }
            break;
        case 2:
            burst(this);
            break;
        }
        break;
    case 1:
        vfunc_0x54();
        field_0x244--;
        if (field_0x244 == 0) {
            if (field_0x23c != 0 && gGameRoot_8053AAF8->field_0x7e == 1) {
                s32 r = fn_801CD664(100);
                u32 id = 'CRTL';
                u32 param = 0;
                if (r < 0x21) {
                    id = 'RUPY';
                    param = 0x1C0007;
                } else if (r < 0x42) {
                    param = 9;
                    id = 'HBMB';
                }
                JGeometry::TVec3f pos;
                pos.x = mPos.x + lbl_8046C770[field_0x234];
                pos.y = mPos.y + lbl_8046C788[field_0x234];
                pos.z = 300.0f + mPos.z;
                s32 serial = getActorManager_801F856C()->fn_801F8544(id, field_0x4, &pos, param);
                if (id == 'HBMB') {
                    ActorBase_804A5770* bomb = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                    if (bomb != NULL) {
                        bomb->field_0x3c = JGeometry::TVec3f(0.0f, 0.0f, -0.1f);
                    }
                }
            }
            getGameView_802D6A9C()->fn_802D8050(field_0x240, true, 0xFF);
            field_0x230 = 2;
            field_0x31c = 0xE6;
            field_0x244 = 300;
        }
        break;
    case 2:
        vfunc_0x54();
        field_0x244--;
        if (field_0x244 == 0) {
            field_0x230 = 3;
        }
        break;
    // FIXME(shotgun): states 3-10 not written (0x802A504C-0x802A61B4: the player-slot
    // assignment of state 3 and the states after it).
    }
    // FIXME(shotgun): the code after the switch is not written (0x802A61B4 on: from state 2,
    // the 'FARY' spawns counted by field_0x314 / field_0x31c, the field_0x24c / field_0x250
    // frame step, and from state 4 the part that reads the table at lbl_8046C658 + 0xE0).
}

// 0x802A4780.
// FIXME(shotgun): 92.04 %, different; behaviour drift.
void cls_804A7EB0::draw() {
    if (field_0x230 >= 2 && field_0x230 < 9) {
        SpriteParams_801F5AE0 params;
        for (s32 i = 0; i < 4; i++) {
            params.field_0x1d = -1;
            s32 frame = field_0x250 + 0x10D;
            JGeometry::TVec3f pos;
            pos.x = mPos.x + (f32)lbl_8046C658[field_0x234][i];
            pos.y = mPos.y + (f32)lbl_8046C6C8[field_0x234][i];
            pos.z = mPos.z;
            if (field_0x4 < 8) {
                void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_gc_sob.bin");
                gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, frame, &params, 1.0f, 1.0f);
            }
        }
        for (s32 i = 0; i < 4; i++) {
            if (field_0x254[i] >= 0) {
                params.field_0x1d = -1;
                s32 frame = field_0x250 + 0x10A;
                JGeometry::TVec3f pos;
                pos.x = field_0x264[i].x;
                pos.y = field_0x264[i].y;
                pos.z = field_0x264[i].z;
                if (field_0x4 < 8) {
                    void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_gc_sob.bin");
                    gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, frame, &params, 1.0f, 1.0f);
                }
            }
        }
    }
}

// 0x802A4654.
void cls_804A7EB0::vfunc_0x54() {
    f32 x = mPos.x;
    f32 y = mPos.y;
    f32 z = mPos.z;
    for (s32 i = 0; i < 4; i++) {
        mPos.x = x + (f32)lbl_8046C658[field_0x234][i];
        mPos.y = y + (f32)lbl_8046C6C8[field_0x234][i];
        startSound_801F0E34(0x30084, 0);
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
    }
    mPos.x = x;
    mPos.y = y;
    mPos.z = z;
}
