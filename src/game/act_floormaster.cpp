#include "game/GameAudio.h"
#include "game/act/act_floormaster.h"
#include "game/Camera.h"
#include "game/GameGlobals.h"

static const s16 lbl_8046A3D8[] = { 0, 5, 1, 5, 2, 5, -1, -1 };
static const s16 lbl_8046A3E8[] = { 3, 5, 4, 10, -1, -1 };
static const s16 lbl_8046A3F4[] = { 2, 10, 5, 5, -1, -1 };
static const s16 lbl_8053FA20[] = { 3, 40, -1, -1 };
static const s16 lbl_8046A400[] = { 6, 5, 7, 5, 8, 5, -1, -1 };
static const s16 lbl_8046A410[] = { 2, 10, 5, 5, -1, -1 };
static const s16 lbl_8046A41C[] = { 3, 5, 6, 5, 7, 5, 8, 5, -1, -1 };

static const s16* lbl_804A5FE8[] = {
    lbl_8046A3D8,
    lbl_8046A3E8,
    lbl_8046A3F4,
    lbl_8053FA20,
    lbl_8046A400,
    lbl_8046A410,
    lbl_8046A41C,
};

Floormaster_804A6004::Floormaster_804A6004() {
    field_0x2a8 = 0.0f;
    field_0x2ac = 0.0f;
    mEmitter2b0 = NULL;
    field_0x2b4 = 0.0f;
    field_0x2b8 = -1;
    field_0x2bc = 0;
    field_0x2c0 = -1;
}

Floormaster_804A6004::~Floormaster_804A6004() {
    releaseEmitter_801EE434(mEmitter2b0);
}

// FIXME: 98.61%. scheduling+regalloc + reloc-name (literal pool and float conversion stack order)
void Floormaster_804A6004::init() {
    EnemyBase_804A4808::init();
    if (field_0x90 & 1) {
        mFlags |= 0x2000;
        u32 arg = field_0x90;
        u32 y = ((arg >> 8) & 0x3F) * 8;
        u32 x = ((arg >> 16) & 0x3F) * 8;
        field_0x2a8 = 8.0f + (f32)x;
        field_0x2ac = 8.0f + (f32)y;
    }
    mBox.set(1.0f, 1.0f, -1.0f, -1.0f);
    mBox2.i.x = mBox.i.x;
    mBox2.i.y = mBox.i.y;
    mBox2.f.x = mBox.f.x;
    mBox2.f.y = mBox.f.y;
    field_0x80.set(1.0f, 1.0f, -1.0f, -1.0f);
    field_0xb0 = 0x50303;
    field_0x98 = 2;
    mFlags |= ENEMY_FLAG_NO_HURT | ENEMY_FLAG_NO_ATTACK | ENEMY_FLAG_KEEP_LIFE;
    mLife = 1000;
    mLifeBackup = mLife;
    field_0x194 = 1;
    mState = 0;
    mStateTimer = 0;
    field_0x250 = fn_801CD664(61) + 60;
    mSpeed = 0.0f;
    mTargetSpeed = 0.0f;
    mAnim.fn_801D2608(lbl_804A5FE8, 7);
    mAnim.startAnim_801D22DC(0, 0, false);
}

void Floormaster_804A6004::vfunc_0xA0() {
    if (mState != 0) {
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
    }
}
