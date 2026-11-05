#include "game/GameAudio.h"
#include "game/act/act_gba_window.h"

GBAWindowControl_804AA440::~GBAWindowControl_804AA440() {}

void GBAWindowControl_804AA440::draw() {}

void GBAWindowControl_804AA440::update() {
    switch (field_0x230) {
    case 0:
        if (field_0x234 == 0x1F && vfunc_0x34()) {
            field_0x230 = 1;
            field_0x23c = field_0x238;
        }
        break;
    case 1:
        if (field_0x23c != 0) {
            gGameRoot2_8053AB10->field_0x20->field_0x216 = 1;
            field_0x23c--;
        } else {
            field_0x230 = 0;
        }
        break;
    }
}

void GBAWindowControl_804AA440::init() {
    field_0x234 = (field_0x90 >> 22) & 0x1F;
    field_0x238 = field_0x90 & 0xFFF;
    getGameView_802D6A9C()->fn_802D7870(field_0x234);
    field_0x230 = 0;
    field_0x23c = 0;
    update();
}
