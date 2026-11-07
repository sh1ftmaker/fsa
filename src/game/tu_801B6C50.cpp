#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/tu_802EF76C.h"
#include "game/act/tu_801B6C50.h"

// 0x801B7550.
u8 fn_801B7550(void) {
    return gGameRoot2_8053AB10->field_0x2c->field_0xc;
}

// 0x801B73B8.
// FIXME(shotgun): 70.31 %, missing/extra + reloc-name
void cls_804A39DC::init() {
    mBox.set(-128.0f, -128.0f, 128.0f, 128.0f);
    field_0x25c = fn_801B7550();
    field_0x260 = field_0x25c;
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    for (s32 i = 0; i < 4; i++) {
        field_0x264[i] = *getPlayerPos_8023E724(i);
    }
    field_0x250 = field_0x90 & 0xF;
    field_0x254 = (field_0x90 >> 4) & 0xF;
    field_0x24c = (field_0x90 >> 8) & 0xF;
    field_0x258 = (field_0x90 >> 12) & 0xF;
    if (field_0x258 > 10) {
        field_0x258 = 0;
    }
    lbl_8053AC00 = 0;
    mEmitters[0] = NULL;
    mEmitters[1] = NULL;
    mEmitters[2] = NULL;
    mEmitters[3] = NULL;
    if (field_0x24c == 0) {
        field_0x230 = 0;
    } else {
        fn_80119DD0()->fn_80119468(0, 0);
        for (s32 i = 0; i < 4; i++) {
            fn_8023EE28(i);
        }
        field_0x230 = 6;
    }
}

// 0x801B7324.
cls_804A39DC::~cls_804A39DC() {
    for (int i = 0; i < 4; i++) {
        releaseEmitter_801EE434(mEmitters[i]);
    }
}

// 0x801B6C50.
void cls_804A39DC::draw() {}
