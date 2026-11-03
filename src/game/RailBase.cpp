#include "game/GameAudio.h"
#include "game/RailBase.h"

void cls_8049C770::init() {
    mPos.x += 8.0f;
    mPos.y += 8.0f;
}

void cls_8049C770::update() {}
void cls_8049C770::draw() {}

s32 cls_800FE3EC::fn_800FE25C(u8 index) {
    if (index == 0xFF) {
        return field_0x4[field_0x3]->layer;
    }
    return field_0x4[index]->layer;
}
