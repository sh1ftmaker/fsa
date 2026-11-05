#include "game/GameAudio.h"
#include "game/act/tu_8034A6B0.h"

cls_804AC868::~cls_804AC868() {}

void cls_804AC868::draw() {
    drawSprite_801F06F0("enemy_both2_sob.bin", field_0x234 + 0x2DD, NULL, -1, 1.0f, 1.0f);
    fn_803075AC(mSerial, field_0x4, &mPos, field_0x234 + 0x2DD, false, 2, -1, 0, 0, 0);
}

void cls_804AC868::update() {
    if (field_0x230 <= 0 && field_0x234 == 0) {
        field_0x234++;
        field_0x230 = 2;
    } else if (field_0x230 <= 0 && field_0x234 == 1) {
        fn_801F3668();
        return;
    }
    field_0x230--;
}

void cls_804AC868::init() {
    mLife = 10000;
    field_0x234 = 0;
    field_0x230 = 2;
}
