#include "game/GameAudio.h"
#include "game/DrawShadow.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/act/act_arrow.h"

// The arrows each player has shot, the unit's .bss object at 0x804C6330 (declared extern: defining it here changes the static initialiser).
extern s32 lbl_804C6330[4];

// 0x80166814.
// FIXME(shotgun): 99.46 %, reloc-name
void ArrowPlayer_804A05E0::init() {
    field_0x98 = (field_0x90 >> 4) & 3;
    field_0x94 = (field_0x90 >> 8) & 0xF;
    field_0x90 = field_0x90 & 3;
    if (fn_8022B160()) {
        s32 player = fn_8022F340(field_0x90);
        lbl_804C6330[player]++;
    }
    mEmitters[0] = NULL;
    mEmitters[1] = NULL;
    mBox.set(-2.0f, -2.0f, 2.0f, 2.0f);
    field_0x80.set(-2.0f, -2.0f, 2.0f, 2.0f);
    field_0x258 = 0;
    field_0x25c = 0;
    field_0x260 = 0;
    field_0x264 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x254 = 0;
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
    }
    field_0x234 = 255.0f;
    field_0x24c = 500;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x26c = 0;
    field_0x270 = 0;
    mLife = 10000;
    field_0x268 = field_0x98;
    field_0x230 = 0;
    field_0x274 = 0;
}

// 0x801662D8.
ArrowPlayer_804A05E0::~ArrowPlayer_804A05E0() {
    for (int i = 0; i < 2; i++) {
        releaseEmitter_801EE434(mEmitters[i]);
    }
}

// 0x8016545C.
// FIXME(shotgun): 99.39 %, missing/extra + reloc-name
void ArrowPlayer_804A05E0::draw() {
    if (field_0x94 != 3) {
        if (field_0x24c >= 50 || (field_0x24c & 2) == 0) {
            SpriteParams_801F5AE0 params;
            params.field_0x10.a = (u8)(s32)field_0x234;
            if (fn_8022B160() == true) {
                switch (field_0x90) {
                case 0:
                    params.field_0x30 = 0;
                    break;
                case 1:
                    params.field_0x30 = 1;
                    break;
                case 2:
                    params.field_0x30 = 2;
                    break;
                case 3:
                    params.field_0x30 = 3;
                    break;
                }
            }
            s32 frame = field_0x268 + 0x4C;
            if (field_0x4 < 8) {
                void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
                gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, &params, 1.0f, 1.0f);
            }
            s32 base = 0x4C;
            if ((field_0x94 == 0 || field_0x94 == 4) && field_0x230 == 0) {
                base = 0x2DF;
            }
            fn_803075AC(mSerial, field_0x4, &mPos, base + field_0x268, false, 2, -1, 0, 0, 0);
        }
        if ((field_0x94 == 2 || field_0x94 == 4) && field_0x230 != 4) {
            drawShadow(field_0x4, &mPos, JUtility::TColor(0, 0, 0, 0x80), 32.0f);
        }
    }
}
