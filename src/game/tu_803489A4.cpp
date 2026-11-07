#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/act/tu_803489A4.h"

// FIXME(shotgun): 99.46 %, scheduling + reloc-name
void BeeFormation_804AC7B8::init() {
    mLife = 8;
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    field_0x298.set(0.0f, 0.0f, 0.0f);
    field_0x268.set(0.0f, 0.0f, 0.0f);
    field_0x274.set(0.0f, 0.0f, 0.0f);
    field_0x280.set(0.0f, 0.0f, 0.0f);
    field_0x28c.set(0.0f, 0.0f, 0.0f);
    field_0x258 = 5.0f;
    field_0x25c = 32.0f;
    field_0x248 = 0;
    field_0x260 = 0;
    field_0x254 = 0;
    field_0x234 = 0x78;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x244 = -1;
    field_0x250 = -1;
    field_0x24c = -1;
    field_0x262 = 0;
    field_0x263 = 0;
    field_0x264 = 0;
    field_0x265 = 0;
    mBox.i.set(-8.0f, -8.0f);
    mBox.f.set(8.0f, 8.0f);
    field_0x230 = 0;
}

// FIXME(shotgun): 97.29 %, scheduling+regalloc
void BeeFormation_804AC7B8::draw() {
    SpriteParams_801F5AE0 params;
    params.field_0x1c = 1;
    params.field_0x1d = 1;
    drawSprite_801F06F0("enemy_gc_sob.bin", field_0x23c + field_0x248 + 0x2C5, &params, 1, 1.0f, 1.0f);
}

BeeFormation_804AC7B8::~BeeFormation_804AC7B8() {}

// update() (0x80348B40, 0xAE8 bytes): not written (size).
