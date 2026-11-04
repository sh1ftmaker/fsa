#include "game/GameAudio.h"
#include "game/act/act_princess_zelda_motion_spawner.h"

PrincessZeldaMotionObjectSpawner_804A375C::~PrincessZeldaMotionObjectSpawner_804A375C() {}

void PrincessZeldaMotionObjectSpawner_804A375C::draw() {}

void PrincessZeldaMotionObjectSpawner_804A375C::init() {
    mBox.i.x = -8.0f;
    mBox.i.y = -8.0f;
    mBox.f.x = 8.0f;
    mBox.f.y = 8.0f;
    mBox2.i.x = -8.0f;
    mBox2.i.y = -8.0f;
    mBox2.f.x = 8.0f;
    mBox2.f.y = 8.0f;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    mKind = field_0x90 & 0xFF;
    field_0x254 = (field_0x90 >> 22) & 0x1F;
    field_0x258 = (field_0x90 >> 8) & 0xF;
    field_0x24c = 0;
    mHomePos.x = mPos.x;
    mHomePos.y = mPos.y;
    mHomePos.z = mPos.z;
    field_0x3c.z = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.x = 0.0f;
    field_0x25c = 0;
    field_0x94 = 0;
    field_0x1a8 = false;
    switch (mKind) {
    case 0:
        mState = 0;
        break;
    case 2:
        mState = 1;
        break;
    case 1:
        mState = 2;
        break;
    case 3:
    case 7:
        mState = 4;
        break;
    default:
        mState = 10;
        break;
    }
}
