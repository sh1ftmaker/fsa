#include "game/GameAudio.h"
#include <math.h>
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/act/act_sand_crab.h"

SandCrab_8049F3B0::~SandCrab_8049F3B0() {}

void SandCrab_8049F3B0::draw() {
    fn_801F06F0("enemy_gc_sob.bin", field_0x240 + 0x1A, NULL, 0, 1.0f, 1.0f);
}

// FIXME: 99.7%. Only the field_0x23c countdown differs: the original adds 8 to
// this in r4 and stores through 0x234(r4), plus its one extra instruction.
void SandCrab_8049F3B0::update() {
    if (fn_801F3D94(true) || fn_801F305C()) {
        fn_801F2618(NULL, false);
        field_0x23c = 0;
        if (field_0x230 != 0) {
            field_0x238 = 10;
            field_0x230 = 0;
        }
        return;
    }
    if (field_0x234 > 0) {
        field_0x234--;
    }
    if (field_0x238 > 0) {
        field_0x238--;
    }
    // The original counts this one down through a base register of this+8
    // (addi r4, r31, 8; stw r0, 0x234(r4)); we store through r31 directly.
    if (field_0x23c > 0) {
        field_0x23c--;
    }
    if (field_0x23c != 0) {
        return;
    }
    mPos += field_0x3c;
    fn_801F2B7C(-100, -1, 1);
    bool hit = fn_801F2618(NULL, false);
    switch (field_0x230) {
    case 0:
        field_0x234 = 2;
        field_0x3c.scale(0.0f);
        if (field_0x238 == 0) {
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
            field_0x238 = (s32)(100.0f * lbl_8053AAF8->mRandom.get_ufloat_1()) + 20;
            field_0x230 = 1;
        }
        break;
    case 1:
        if (hit || field_0x238 == 0) {
            field_0x238 = (s32)(50.0f * lbl_8053AAF8->mRandom.get_ufloat_1()) + 20;
            field_0x230 = 0;
        }
        for (s32 i = 0; i < 4; i++) {
            if (fn_8023F264(i) && getField_0x4() == fn_8023DE58(i) && !fn_80230188(i) && !fn_8022DCD8(i) &&
                FABS(fn_8023E724(i)->x - mPos.x) < 200.0f &&
                FABS(fn_8023E724(i)->y - mPos.y) < 40.0f) {
                JGeometry::TVec3f outPos;
                JGeometry::TVec3f out2;
                if (!fn_801F23BC(field_0x4, &mPos, fn_8023E724(i), &field_0x80, &outPos, &out2, false)) {
                    field_0x234 = 0;
                    field_0x3c.x = 2.0f;
                    if (fn_8023E724(i)->x < mPos.x) {
                        field_0x3c.x = -2.0f;
                    }
                    field_0x238 = (s32)(100.0f * lbl_8053AAF8->mRandom.get_ufloat_1()) + 50;
                    field_0x230 = 2;
                    break;
                }
            }
        }
        break;
    case 2:
        if (hit || field_0x238 == 0) {
            field_0x238 = (s32)(50.0f * lbl_8053AAF8->mRandom.get_ufloat_1());
            field_0x230 = 0;
        }
        break;
    }
    if (field_0x234 <= 0) {
        field_0x234 = 8;
        if (field_0x230 == 2) {
            field_0x234 = 2;
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
    field_0x234 = 0;
    field_0x238 = (s32)(50.0f * lbl_8053AAF8->mRandom.get_ufloat_1());
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0xb0 = 0x11101;
    field_0x230 = 0;
}
