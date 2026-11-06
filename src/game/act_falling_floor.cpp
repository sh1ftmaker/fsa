#include "game/GameAudio.h"
#include "game/act/act_falling_floor.h"

void FallingFloor_804A9078::init() {
    mBox.i.x = -5.0f;
    mBox.i.y = -5.0f;
    mBox.f.x = 5.0f;
    mBox.f.y = 5.0f;
    field_0x23c = 0;
    field_0x238 = field_0x90 & 0x3F;
    if (field_0x238 != 2) {
        mPos.x += 8.0f;
        mPos.y += 8.0f;
    } else {
        mPos.x += 8.0f;
    }
    if (field_0x238 == 0 || field_0x238 == 3) {
        field_0x230 = 0;
    } else if (field_0x238 == 1) {
        field_0x230 = 5;
    } else {
        field_0x230 = 8;
    }
}

void FallingFloor_804A9078::draw() {}

FallingFloor_804A9078::~FallingFloor_804A9078() {}
