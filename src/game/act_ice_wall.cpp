#include "game/GameAudio.h"
#include "game/act/act_ice_wall.h"

// One animation: (cel, duration) pairs, ended by a cel of -1.
static const s16 lbl_8053C8B0[] = {0, 2, -1, -1};
static const s16* lbl_80539F28[] = {lbl_8053C8B0, NULL};

// FIXME: 99.81%. Code matches; the 1.0f literal is loaded from its own .sdata2
// entry in ours, the original's is lbl_8053C8BC (a data split, not code).
IceWall_8049DE18::IceWall_8049DE18()
    : field_0x230(0), field_0x234(0), field_0x238(0), field_0x23c(0), field_0x240(1.0f), mSprite(),
      field_0x27c(NULL) {}

IceWall_8049DE18::~IceWall_8049DE18() {
    fn_801EE434(field_0x27c);
    field_0x27c = NULL;
}

// FIXME: 99.39%. Code matches; the float literals are separate .sdata2 entries
// in ours (the original's are split as lbl_8053C8xx, a data split, not code).
void IceWall_8049DE18::init() {
    ActorBase_804A5770::init();
    field_0x230 = field_0x90 & 0xF;
    field_0x238 = field_0x90 >> 4 & 0xF;
    field_0x23c = field_0x90 >> 8 & 0xFF;
    switch (field_0x230) {
    case 1:
        field_0x240 = 0.5f;
        field_0x54.x = field_0x54.y = field_0x54.z = field_0x240;
        mBox.i.x = -8.0f;
        mBox.i.y = -8.0f;
        mBox.f.x = 8.0f;
        mBox.f.y = 8.0f;
        field_0x80.i.x = -4.0f;
        field_0x80.i.y = -4.0f;
        field_0x80.f.x = 4.0f;
        field_0x80.f.y = 4.0f;
        break;
    // Case 0 is written out with the default: the original's dispatch sends
    // both sides of the test against 1 to this block.
    case 0:
    default:
        mBox.i.x = -16.0f;
        mBox.i.y = -16.0f;
        mBox.f.x = 16.0f;
        mBox.f.y = 16.0f;
        field_0x80.i.x = -8.0f;
        field_0x80.i.y = -8.0f;
        field_0x80.f.x = 8.0f;
        field_0x80.f.y = 8.0f;
        break;
    }
    mBox2.i.x = mBox.i.x;
    mBox2.i.y = mBox.i.y;
    mBox2.f.x = mBox.f.x;
    mBox2.f.y = mBox.f.y;
    field_0xb8 = 1;
    field_0xb0 = 0x06040304;
    field_0x118 = 0;
    field_0xbc = 3;
    mSprite.fn_801D2608(lbl_80539F28, 1);
    mSprite.fn_801D22DC(0, 2, false);
    mSprite.fn_801D1F14(0.0f);
}
