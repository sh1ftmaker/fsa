// FIXME(shotgun): not written (size 0x410): fn_8028DDD8
// FIXME(shotgun): not written (size 0x16A4): update (0x8028E1EC)

#include "game/GameAudio.h"
#include "game/act/tu_8028DD78.h"

// FIXME(shotgun): not written (size 0x410): fn_8028DDD8
// FIXME(shotgun): not written (size 0x16A4): update (0x8028E1EC)

// FIXME(shotgun): 98.6 %, reloc-name
void cls_804A7A68::init() {
    switch (field_0x90 & 0xF) {
    case 0:
    case 1:
    case 4:
        mBox.i.x = -40.0f;
        mBox.i.y = -10.0f;
        mBox.f.x = 40.0f;
        mBox.f.y = 8.0f;
        break;
    case 5:
        mBox.i.x = -40.0f;
        mBox.i.y = -8.0f;
        mBox.f.x = 40.0f;
        mBox.f.y = 10.0f;
        break;
    case 6:
        mBox.i.x = -8.0f;
        mBox.i.y = -40.0f;
        mBox.f.x = 10.0f;
        mBox.f.y = 40.0f;
        break;
    case 7:
        mBox.i.x = -10.0f;
        mBox.i.y = -40.0f;
        mBox.f.x = 8.0f;
        mBox.f.y = 40.0f;
        break;
    }
    mLife = 0;
    field_0x230 = 0;
    field_0x234 = 0;
    field_0x268 = 0xFF;
}

void cls_804A7A68::draw() {}

cls_804A7A68::~cls_804A7A68() {}
