#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/tu_8044998C.h"
#include "game/act/act_zora_fish_herder.h"

// The big per-frame function (fn_801A68DC, 0x12BC bytes) is not written yet.

// Advances the fish animation: picks the sprite frame (field_0x254) of the
// current mode and steps to the next mode when its loop ends.
// FIXME(shotgun): 87.88 %, missing/extra (the original has a dead cmpwi 4 / beqlr after the mode 5 test)
void ZoraFishHerding_804A2E30::fn_801A7C48() {
    if (field_0x234 != 0) {
        return;
    }
    switch (field_0x250) {
    case 0:
    case 5:
        field_0x254 = field_0x240 + 2;
        field_0x234 = 8;
        field_0x240 = (field_0x240 + 1) & 7;
        if (field_0x240 >= 4 && field_0x250 == 5) {
            field_0x250 = 4;
            field_0x234 = 0;
            field_0x240 = 0;
        }
        break;
    case 1:
        field_0x254 = field_0x240 + 12;
        field_0x234 = 8;
        field_0x240 = (field_0x240 + 1) & 3;
        break;
    case 2:
        field_0x254 = 13 - field_0x240;
        field_0x234 = 8;
        field_0x240 = (field_0x240 + 1) & 3;
        if (field_0x240 == 0) {
            field_0x250 = 0;
        }
        break;
    case 3:
        field_0x254 = field_0x240 + 10;
        field_0x234 = 8;
        field_0x240 = (field_0x240 + 1) & 3;
        if (field_0x240 == 0) {
            field_0x250 = 1;
        }
        break;
    case 4:
        field_0x254 = field_0x240;
        field_0x234 = 15;
        field_0x240 = (field_0x240 + 1) & 1;
        break;
    }
}

ZoraFishHerding_804A2E30::~ZoraFishHerding_804A2E30() {
    if (mKind == 0 || mKind == 1) {
        if (mState == 4 || mState == 5) {
            fn_8022B1C8();
            if (gGameRoot_8053AAF8->field_0x4c != 9) {
                gGameRoot_8053AAF8->fn_80138B7C();
                gAudioMgr_8053AF80->fn_80458508(0);
            }
        }
    }
}

// FIXME(shotgun): 99.74 %, scheduling + reloc-name
void ZoraFishHerding_804A2E30::init() {
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    mBox2.set(-10.0f, -10.0f, 10.0f, 10.0f);
    field_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x24c = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x254 = 2;
    field_0x258 = -1;
    field_0x25c = 0;
    field_0x26c = -1;
    field_0x270 = -1;
    field_0x3c.x = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.z = 0.0f;
    field_0x264 = -1;
    field_0x268 = -1;
    field_0x250 = 0;
    if (field_0x250 != 0) {
        field_0x250 = 0;
        field_0x234 = 0;
        field_0x240 = 0;
    }
    mKind = field_0x90 & 0xF;
    if (vfunc_0x34()) {
        fn_801F3668();
        return;
    }
    switch (mKind) {
    case 1:
        if (field_0x250 != 1) {
            field_0x250 = 1;
            field_0x234 = 0;
            field_0x240 = 0;
        }
        field_0x254 = field_0x240 + 12;
        // fall through
    case 0:
        if (vfunc_0x34()) {
            mState = 10;
        } else {
            mState = 0;
        }
        break;
    case 2:
        mState = 13;
        break;
    }
}

void ZoraFishHerding_804A2E30::update() {
    switch (mKind) {
    case 0:
    case 1:
        fn_801A68DC();
        break;
    }
}

// FIXME(shotgun): 90.56 %, missing/extra
void ZoraFishHerding_804A2E30::draw() {
    switch (mKind) {
    case 0:
    case 1: {
        SpriteParams_801F5AE0 params;
        if (field_0x250 != 1) {
            drawSprite_801F06F0("enemy_gc_sob.bin", field_0x254 + 0x303, &params, -1, 1.0f, 1.0f);
        }
        if (field_0x258 >= 0) {
            lbl_8053AF20->field_0x10 = field_0x25c;
            lbl_8053AF20->field_0x13 |= 1;
            lbl_8053AF20->field_0x11 = field_0x258 / 60;
            lbl_8053AF20->field_0x12 = (f32)(field_0x258 % 60) * 100.0f / 60.0f;
            lbl_8053AF20->field_0x13 |= 2;
        }
        break;
    }
    }
}
