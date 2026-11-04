#include "game/GameAudio.h"
#include "game/Effect.h"
#include "game/act/act_hyrule_castle_barrier.h"

HyruleCastleBarrier_8049DEE8::HyruleCastleBarrier_8049DEE8()
    : field_0x230(0), field_0x234(0), field_0x238(0), field_0x23c(0), field_0x240(false),
      field_0x241(false), field_0x250(NULL), field_0x254(NULL) {
    mEmitters[0] = NULL;
    mEmitters[1] = NULL;
    mEmitters[2] = NULL;
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

// Spawns the barrier's effects: field_0x234 == 1 selects one pair, field_0x241
// a loop of three plus two more, and otherwise two of id 0xCA.
// FIXME: 47.32%. The original keeps `this` in r31 (ours in r29), tests
// field_0x234 == 1 twice on the first path (beq, then beq again before the
// second spawn), and schedules the vector stores and literal loads
// differently; the 0xCA/0x125/0x126 ids and the pi/2 angle are right.
void HyruleCastleBarrier_8049DEE8::fn_8011EEEC() {
    switch (field_0x234) {
    case 1: {
        JGeometry::TVec3f top(mPos.x, mPos.y + 32.0f, mPos.z);
        JPABaseEmitter* first = fn_8013CC50(field_0x4, &top, 0xCA, 0, 0, NULL, NULL, true, 1.0f);
        JGeometry::TVec3f bottom(mPos.x, mPos.y - 32.0f, mPos.z);
        JPABaseEmitter* second =
            fn_8013CC50(field_0x4, &bottom, 0xCA, 0, 0, NULL, NULL, true, 1.0f);
        if (first != NULL) {
            fn_8013CAA4(first, 1.5707964f);
        }
        if (second != NULL) {
            fn_8013CAA4(second, 1.5707964f);
        }
        break;
    }
    default:
        if (field_0x241 != 0) {
            for (s32 i = 0; i < 3; i++) {
                JGeometry::TVec3f p(mPos.x, mPos.y + 16.0f * (f32)(i - 1), 0.0f);
                fn_8013CC50(field_0x4, &p, 0x125, 0, 0, NULL, NULL, true, 1.0f);
            }
            if (field_0x234 == 1) {
                JGeometry::TVec3f p(mPos.x, mPos.y + 32.0f, mPos.z);
                fn_8013CC50(field_0x4, &p, 0x126, 0, 0, NULL, NULL, true, 1.0f);
            } else {
                JGeometry::TVec3f p(mPos.x - 32.0f, mPos.y, mPos.z);
                fn_8013CC50(field_0x4, &p, 0x126, 0, 0, NULL, NULL, true, 1.0f);
            }
            if (field_0x234 == 1) {
                JGeometry::TVec3f p(mPos.x, mPos.y - 32.0f, mPos.z);
                fn_8013CC50(field_0x4, &p, 0x126, 0, 0, NULL, NULL, true, 1.0f);
            } else {
                JGeometry::TVec3f p(mPos.x + 32.0f, mPos.y, mPos.z);
                fn_8013CC50(field_0x4, &p, 0x126, 0, 0, NULL, NULL, true, 1.0f);
            }
        } else {
            if (field_0x234 == 1) {
                JGeometry::TVec3f p(mPos.x, mPos.y + 32.0f, mPos.z);
                fn_8013CC50(field_0x4, &p, 0xCA, 0, 0, NULL, NULL, true, 1.0f);
            } else {
                JGeometry::TVec3f p(mPos.x - 32.0f, mPos.y, mPos.z);
                fn_8013CC50(field_0x4, &p, 0xCA, 0, 0, NULL, NULL, true, 1.0f);
            }
            if (field_0x234 == 1) {
                JGeometry::TVec3f p(mPos.x, mPos.y - 32.0f, mPos.z);
                fn_8013CC50(field_0x4, &p, 0xCA, 0, 0, NULL, NULL, true, 1.0f);
            } else {
                JGeometry::TVec3f p(mPos.x + 32.0f, mPos.y, mPos.z);
                fn_8013CC50(field_0x4, &p, 0xCA, 0, 0, NULL, NULL, true, 1.0f);
            }
        }
        break;
    }
}
