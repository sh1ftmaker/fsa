#include "game/GameAudio.h"
#include "game/Effect.h"
#include "game/act/act_hyrule_castle_barrier.h"

// FIXME: 99.41%. Code matches; the float literals are separate .sdata2 entries
// in ours (the original's are split as lbl_8053C91C and so on, a data split,
// not code).
void HyruleCastleBarrier_8049DEE8::init() {
    ActorBase_804A5770::init();
    if ((field_0x90 & 1) == 1) {
        field_0x240 = true;
    }
    if ((field_0x90 >> 1 & 1) == 1) {
        field_0x241 = true;
    }
    field_0x234 = field_0x90 >> 4 & 0xF;
    field_0x238 = field_0x90 >> 22 & 0x1F;
    switch (field_0x234) {
    case 1:
        mBox.i.x = -16.0f;
        mBox.i.y = -40.0f;
        mBox.f.x = 16.0f;
        mBox.f.y = 40.0f;
        mBox2.i.x = mBox.i.x;
        mBox2.i.y = mBox.i.y;
        mBox2.f.x = mBox.f.x;
        mBox2.f.y = mBox.f.y;
        field_0x80.i.x = -14.0f;
        field_0x80.i.y = -40.0f;
        field_0x80.f.x = 14.0f;
        field_0x80.f.y = 40.0f;
        break;
    // Case 0 is written out with the default: the original's dispatch sends
    // both sides of the test against 1 to this block.
    case 0:
    default:
        mBox.i.x = -40.0f;
        mBox.i.y = -16.0f;
        mBox.f.x = 40.0f;
        mBox.f.y = 16.0f;
        mBox2.i.x = mBox.i.x;
        mBox2.i.y = mBox.i.y;
        mBox2.f.x = mBox.f.x;
        mBox2.f.y = mBox.f.y;
        field_0x80.i.x = -40.0f;
        field_0x80.i.y = -14.0f;
        field_0x80.f.x = 40.0f;
        field_0x80.f.y = 14.0f;
        break;
    }
    field_0x114 = 2;
    field_0x118 = 2;
    field_0xb8 = 1;
    field_0xbc = 3;
    field_0xb0 = 0x0A040304;
    fn_8011F314();
    if (vfunc_0x34()) {
        fn_8011F5C0(1);
    }
}

HyruleCastleBarrier_8049DEE8::~HyruleCastleBarrier_8049DEE8() {
    for (s32 i = 0; i < 3; i++) {
        fn_801EE434(mEmitters[i]);
        mEmitters[i] = NULL;
    }
    fn_801EE434(field_0x250);
    field_0x250 = NULL;
    fn_801EE434(field_0x254);
    field_0x254 = NULL;
}

HyruleCastleBarrier_8049DEE8::HyruleCastleBarrier_8049DEE8()
    : field_0x230(0), field_0x234(0), field_0x238(0), field_0x23c(0), field_0x240(false),
      field_0x241(false), field_0x250(NULL), field_0x254(NULL) {
    mEmitters[0] = NULL;
    mEmitters[1] = NULL;
    mEmitters[2] = NULL;
}
