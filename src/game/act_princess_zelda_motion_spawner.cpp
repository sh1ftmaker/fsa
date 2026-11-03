#include "game/GameAudio.h"
#include "game/act/act_princess_zelda_motion_spawner.h"

static const f32 lbl_8053E9C0[1] = {0.0f};
static const f32 lbl_8053E9EC[1] = {8.0f};
static const f32 lbl_8053EA08[2] = {-8.0f, 0.0f};

PrincessZeldaMotionObjectSpawner_804A375C::~PrincessZeldaMotionObjectSpawner_804A375C() {}

void PrincessZeldaMotionObjectSpawner_804A375C::init() {
    mBox.i.x = lbl_8053EA08[0];
    mBox.i.y = lbl_8053EA08[0];
    mBox.f.x = lbl_8053E9EC[0];
    mBox.f.y = lbl_8053E9EC[0];
    mBox2.i.x = lbl_8053EA08[0];
    mBox2.i.y = lbl_8053EA08[0];
    mBox2.f.x = lbl_8053E9EC[0];
    mBox2.f.y = lbl_8053E9EC[0];
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
    field_0x3c.z = lbl_8053E9C0[0];
    field_0x3c.y = lbl_8053E9C0[0];
    field_0x3c.x = lbl_8053E9C0[0];
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

void PrincessZeldaMotionObjectSpawner_804A375C::draw() {}
