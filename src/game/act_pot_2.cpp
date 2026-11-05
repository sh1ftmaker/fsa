#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/act/act_pot_2.h"

Pot2_804AA750::~Pot2_804AA750() {}

void Pot2_804AA750::draw() {
    JGeometry::TVec3f spritePos(mPos.x, mPos.y - 2.0f, 0.0f);
    if (field_0x4 < 8) {
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
        gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&spritePos, res, 0x21, NULL, 1.0f, 1.0f);
    }
    JGeometry::TVec3f pos(mPos.x, mPos.y - 2.0f, 0.0f);
    fn_803075AC(mSerial, field_0x4, &pos, 0x21, false, 2, -1, 0, 0, 0);
}

void Pot2_804AA750::update() {
    field_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
    JGeometry::TBox2f box;
    box = field_0x80;
    box.addPos(mPos.x, mPos.y);
    fn_80239914(&box, field_0x4, mSerial);
    fn_8020D58C()->addBox(mSerial);
}

void Pot2_804AA750::init() {
    mPos.x += 8.0f;
    mPos.y += 8.0f;
}
