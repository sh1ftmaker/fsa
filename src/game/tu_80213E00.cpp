#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/act/tu_80213E00.h"

// Animation 0: (cel, duration) pairs, ended by a negative cel.
const s16 lbl_8046A578[] = {0, 3, 1, 3, 2, 3, 3, 3, -1, -1, 0, 0};

const s16* lbl_8053A180[2] = {lbl_8046A578, NULL};

cls_804A6400::~cls_804A6400() {}

void cls_804A6400::draw() {
    fn_803075AC(mSerial, field_0x4, &mPos, mAnim.mCel + 0x1C3, false, 1, -1, 1, 0, 0);
}

void cls_804A6400::vfunc_0x88() {
    field_0x3c.set(0.0f, 0.0f, 0.0f);
}

void cls_804A6400::vfunc_0x90(u8) {}

void cls_804A6400::init() {
    EnemyBase_804A4808::init();
    mBox.set(1.0f, 1.0f, -1.0f, -1.0f);
    mBox2.set(mBox);
    field_0x80.set(1.0f, 1.0f, -1.0f, -1.0f);
    field_0xb0 = 0;
    field_0x98 = 2;
    mFlags |= 0x380;
    mLife = 1000;
    mLifeBackup = mLife;
    field_0x194 = 1;
    mState = 0;
    mStateTimer = 0;
    field_0x250 = -1;
    mSpeed = 0.0f;
    mTargetSpeed = 0.0f;
    mAnim.fn_801D2608(lbl_8053A180, 1);
    mAnim.fn_801D22DC(0, 2, false);
}

cls_804A6400::cls_804A6400() {}
