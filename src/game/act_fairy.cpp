#include "game/GameAudio.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/act/act_fairy.h"
#include "game/tu_803B8C74.h"

Fairy_804A03B8::~Fairy_804A03B8() {
    releaseEmitter_801EE434(mEmitter290);
    releaseEmitter_801EE434(mEmitter294);
}

// FIXME: 97.39 %. Apart from the float literals (`@NNN` against lbl_8053D6xx, a data split), the
// save-file block differs: the original keeps the file pointer at lbl_8050EC80 + file * 0xC8 in r6
// (field 0x67 read and written at 0x87) and loads lbl_80529DEC's count before the 0x339 argument;
// ours materialises the reference with `addi r5, r3, 0x20` for the store and loads the count after
// li r4, 0x339 (tu_801DE714's destructor has the same reference shape).
void Fairy_804A03B8::init() {
    mBox.i.x = -4.0f;
    mBox.i.y = -4.0f;
    mBox.f.x = 4.0f;
    mBox.f.y = 4.0f;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x24c = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x250 = 0;
    field_0x26c = 0.0f;
    field_0x278 = 0.0f;
    field_0x27c = 0.0f;
    field_0x280 = 0.0f;
    field_0x234 = 0x400;
    field_0x280 = 10.0f;
    field_0x23c = 10;
    field_0x240 = (s32)(30.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 30;
    field_0x260 = (field_0x90 >> 22) & 0x1F;
    field_0x258 = (field_0x90 >> 4) & 0xF;
    field_0x25c = field_0x90 & 0xF;
    field_0x264 = 0;
    field_0x268 = 0;
    field_0x270 = 255.0f;
    if (field_0x25c > 0) {
        field_0x25c--;
    } else {
        field_0x25c = -1;
    }
    field_0x248 = (s32)(32768.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
    field_0x230 = 0;
    field_0xb0 = 0x20000;
    field_0x254 = -1;
    if (field_0x258 == 2) {
        field_0x11f = 1;
    }
    mEmitter290 = NULL;
    mEmitter294 = NULL;
    field_0x284.set(mPos);
    if (field_0x258 == 2) {
        field_0x234 = 0;
        field_0x238 = 0;
        field_0x248 = 0;
        field_0x230 = 4;
    }
    if (field_0x258 == 3) {
        field_0x234 = 10000;
        field_0x280 = 100.0f;
        if (lbl_80529DEC.field_0x24f < 0x63) {
            lbl_80529DEC.field_0x24f++;
        } else {
            lbl_80529DEC.field_0x24f = 0x63;
        }
        field_0x23c = 30;
        startSound_801F0E34(0x400E5, 0);
        field_0x230 = 5;
    }
    if ((field_0x90 >> 27) != 0 && !vfunc_0x34()) {
        field_0xb0 = 0;
        return;
    }
    mEmitter290 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x183, 0, 0, NULL, NULL, true, 1.0f);
    mEmitter294 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x184, 0, 0, NULL, NULL, true, 1.0f);
    if (field_0x230 == 0 || field_0x230 == 5) {
        if (gGameRoot_8053AAF8->field_0x7f == 1) {
            SaveFile_803B950C& file = lbl_8050EC80.getCurrentFile();
            if ((file.field_0x67 & 1) == 0) {
                file.field_0x67 |= 1;
                lbl_8053AED0->fn_8043D67C(0x339, 0, lbl_80529DEC.field_0x24f);
            }
        }
    }
}

void Fairy_804A03B8::draw() {
    SpriteParams_801F5AE0 params;
    if ((field_0x90 >> 27) != 0 && !vfunc_0x34()) {
        return;
    }
    if (field_0x258 != 2) {
        if (field_0x234 < 0x100 && (field_0x234 & 2)) {
            return;
        }
        params.field_0x2d = 1;
        if (field_0x258 == 3) {
            params.field_0x10.a = (u8)field_0x270;
            params.field_0x30 = 0x11;
        }
        drawSprite_801F06F0("enemy_both2_sob.bin", field_0x250 + 0x5E, &params, 1, 1.0f, 1.0f);
        fn_803075AC(mSerial, field_0x4, &mPos, field_0x250 + 0x5E, false, 2, -1, 0, 0, 1);
    } else {
        s32 frame = 0x5E;
        if (field_0x248 != 0) {
            frame = 0x7D;
        }
        fn_803075AC(mSerial, field_0x4, &mPos, frame + field_0x250, false, 2, -1, 1, 0, 1);
    }
}
