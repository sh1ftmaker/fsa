#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/act/tu_801D8400.h"

void cls_804A4C58::fn_801D8400() {
    if (mState < 8) {
        fn_801D9C58(8);
    }
}

void cls_804A4C58::vfunc_0x24() {}

// Vtable slot 0xA0 (0x801D84E4), the last slot of the vtable.
void cls_804A4C58::vfunc_0xA0() {
    if (mState == 0) {
        return;
    }
    if (mState >= 0 && mState < 4) {
        cls_804A03A8* obj = lbl_8053AB10->field_0x30;
        if (!obj->field_0x284) {
            obj->field_0x284 = 1;
            obj->field_0x28c = 0.05f;
        }
        Camera_804AA9E4* camera = lbl_8053AB10->field_0x24;
        if (!camera->field_0x7f5) {
            camera->field_0x7f4 = 1;
            camera->field_0x7f8 = 4;
            camera->field_0x7fc.set(mPos);
            camera->field_0x7f6 = 0;
        }
    } else {
        lbl_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
        if (!lbl_8053AB10->field_0x24->field_0x179) {
            lbl_8053AB10->field_0x24->field_0x178 = 1;
        }
    }
}

// Vtable slot 0x80 (0x801D8980). A position above the ceiling in
// field_0x2d0 is clamped to it, its vertical speed is zeroed, and then
// EnemyBase's floor check runs.
void cls_804A4C58::vfunc_0x80() {
    if (mPos.y > field_0x2d0) {
        mPos.y = field_0x2d0;
        field_0x3c.y = 0.0f;
    }
    EnemyBase_804A4808::vfunc_0x80();
}

// Vtable slot 0x9C (0x801D89BC). Any hit bit in mFlags skips the step
// (vfunc_0x94 only in state 0xF); otherwise EnemyBase's state step runs.
// Then the countdown at 0x336 and the effect pair at 0x2C8.
void cls_804A4C58::vfunc_0x9C(u8 mapHit) {
    if (!(mFlags & 7)) {
        fn_801CE2BC(mapHit);
    } else if (mState == 0xF) {
        vfunc_0x94();
    }
    if (field_0x336 != 0) {
        field_0x336--;
    }
    fn_801E61C4();
}

// Vtable slot 0x54 (0x801DA0FC): EnemyBase's start hook; asks
// gAudioMgr_8053AF80 for fn_80458F9C(60).
void cls_804A4C58::vfunc_0x54() {
    gAudioMgr_8053AF80->fn_80458F9C(60);
}

// 0x801DA504. The sequencer's constructor runs before the ceiling is set,
// and the twelve TVec2f are built after it (the member initialisers run in
// declaration order); the body then zeroes the fields at 0x334-0x33F and the
// four emitters.
cls_804A4C58::cls_804A4C58() : mSeq2a8(), field_0x2d0(0.0f) {
    field_0x334 = 0;
    field_0x336 = 0;
    field_0x338 = 0;
    field_0x33a = 0;
    field_0x33c = 0;
    field_0x33e = 0;
    field_0x33f = 0;
    mEmitter2c0 = NULL;
    mEmitter2c4 = NULL;
    mEmitter2c8 = NULL;
    mEmitter2cc = NULL;
}
