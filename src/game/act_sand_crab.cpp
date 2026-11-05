#include "game/GameAudio.h"
#include <math.h>
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/act/act_sand_crab.h"

SandCrab_8049F3B0::~SandCrab_8049F3B0() {}

void SandCrab_8049F3B0::draw() {
    drawSprite_801F06F0("enemy_gc_sob.bin", field_0x240 + 0x1A, NULL, 0, 1.0f, 1.0f);
}

void SandCrab_8049F3B0::update() {
    if (fn_801F3D94(true) || fn_801F305C()) {
        fn_801F2618(NULL, false);
        mTimers[2] = 0;
        if (field_0x230 != 0) {
            mTimers[1] = 10;
            field_0x230 = 0;
        }
        return;
    }
    for (s32 i = 0; i < 3; i++) {
        if (mTimers[i] > 0) {
            mTimers[i]--;
        }
    }
    if (mTimers[2] != 0) {
        return;
    }
    mPos += field_0x3c;
    fn_801F2B7C(-100, -1, 1);
    bool hit = fn_801F2618(NULL, false);
    switch (field_0x230) {
    case 0:
        mTimers[0] = 2;
        field_0x3c.scale(0.0f);
        if (mTimers[1] == 0) {
            field_0x98++;
            field_0x98 &= 1;
            switch (field_0x98) {
            case 0:
                field_0x3c.x = 1.0f;
                break;
            case 1:
                field_0x3c.x = -1.0f;
                break;
            }
            mTimers[1] = (s32)(100.0f * lbl_8053AAF8->mRandom.get_ufloat_1()) + 20;
            field_0x230 = 1;
        }
        break;
    case 1:
        if (hit || mTimers[1] == 0) {
            mTimers[1] = (s32)(50.0f * lbl_8053AAF8->mRandom.get_ufloat_1()) + 20;
            field_0x230 = 0;
        }
        for (s32 i = 0; i < 4; i++) {
            if (fn_8023F264(i) && getField_0x4() == fn_8023DE58(i) && !fn_80230188(i) && !fn_8022DCD8(i) &&
                FABS(fn_8023E724(i)->x - mPos.x) < 200.0f &&
                FABS(fn_8023E724(i)->y - mPos.y) < 40.0f) {
                JGeometry::TVec3f outPos;
                JGeometry::TVec3f out2;
                if (!fn_801F23BC(field_0x4, &mPos, fn_8023E724(i), &field_0x80, &outPos, &out2, false)) {
                    mTimers[0] = 0;
                    field_0x3c.x = 2.0f;
                    if (fn_8023E724(i)->x < mPos.x) {
                        field_0x3c.x = -2.0f;
                    }
                    mTimers[1] = (s32)(100.0f * lbl_8053AAF8->mRandom.get_ufloat_1()) + 50;
                    field_0x230 = 2;
                    break;
                }
            }
        }
        break;
    case 2:
        if (hit || mTimers[1] == 0) {
            mTimers[1] = (s32)(50.0f * lbl_8053AAF8->mRandom.get_ufloat_1());
            field_0x230 = 0;
        }
        break;
    }
    if (mTimers[0] <= 0) {
        mTimers[0] = 8;
        if (field_0x230 == 2) {
            mTimers[0] = 2;
        }
        field_0x240++;
        field_0x240 &= 1;
    }
}

// FIXME: 99.3%. Regalloc only: the original keeps the zero in r6 and the
// lbl_8053AAF8 pointer in r7 (we swap them), so r6/r7 differ in 13 instructions.
void SandCrab_8049F3B0::init() {
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x98 = 0;
    switch ((s32)(2.0f * lbl_8053AAF8->mRandom.get_ufloat_1())) {
    case 0:
        field_0x3c.x = 1.0f;
        break;
    case 1:
        field_0x3c.x = -1.0f;
        field_0x98 = 1;
        break;
    }
    mTimers[0] = 0;
    mTimers[1] = (s32)(50.0f * lbl_8053AAF8->mRandom.get_ufloat_1());
    mTimers[2] = 0;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0xb0 = 0x11101;
    field_0x230 = 0;
}
