#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/act/tu_80333958.h"

IronBall_804AB968::~IronBall_804AB968() {}

bool IronBall_804AB968::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    JGeometry::TVec3f p(*pos);
    switch (field_0x240) {
    case 0:
        if (field_0x23c) {
            p.y -= 32.0f;
        } else {
            p.y -= 32.0f;
        }
        break;
    case 1:
        if (field_0x23c) {
            p.y += 32.0f;
        } else {
            p.y += 32.0f;
        }
        break;
    case 2:
        if (field_0x23c) {
            p.x -= 32.0f;
        } else {
            p.x -= 32.0f;
        }
        break;
    case 3:
        if (field_0x23c) {
            p.x += 32.0f;
        } else {
            p.x += 32.0f;
        }
        break;
    }
    s32 code = fn_8022461C(&p, layer);
    switch (field_0x240) {
    case 0:
        if (code == 0x41) {
            return true;
        }
        break;
    case 1:
        if (code == 0x40) {
            return true;
        }
        break;
    case 2:
        if (code == 0x43) {
            return true;
        }
        break;
    case 3:
        if (code == 0x42) {
            return true;
        }
        break;
    }
    return false;
}

void IronBall_804AB968::draw() {
    SpriteParams_801F5AE0 params;
    params.field_0x1d = -1;
    if (field_0x23c == 0) {
        JGeometry::TVec3f p(mPos.x, 3.0f + mPos.y, 0.0f);
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "kage_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&p, res, 0, &params, 1.0f, 1.0f);
        }
        s32 frame = field_0x238 + 0x25;
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, NULL, 1.0f, 1.0f);
        }
        fn_80307AC8(mSerial, field_0x4, &mPos, field_0x238 + 0x25, 0, 0, -1);
    } else {
        JGeometry::TVec3f p(mPos.x, 12.0f + mPos.y, 0.0f);
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "kage_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&p, res, 3, &params, 2.0f, 1.0f);
        }
        s32 frame = field_0x238 + 0x27;
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, NULL, 1.0f, 1.0f);
        }
        fn_80307AC8(mSerial, field_0x4, &mPos, field_0x238 + 0x27, 0, 0, -1);
    }
}

// FIXME: 99.74%. Regalloc only: the |x| + |y| speed sum uses f1, f0 / f0, f1 here;
// the original has f1 (|y|), f3 (|x|) then frsp into f0, f1.
void IronBall_804AB968::update() {
    fn_801F2B7C(-100, -1, 1);
    if (!vfunc_0x34()) {
        mPos += field_0x3c;
        if (vfunc_0x4C(field_0x4, &mPos) || field_0x244 > 600) {
            mAlive = false;
        }
        field_0x234 -= (s32)(0.5f + (FABS(field_0x3c.y) + FABS(field_0x3c.x)));
        if (field_0x234 <= 15) {
            field_0x238 = 1;
            if (field_0x234 <= 0) {
                field_0x234 = 30;
                field_0x238 = 0;
                startSound_801F0E34(0x30010, 0);
            }
        }
        field_0x244++;
    }
}

void IronBall_804AB968::init() {
    field_0x240 = (field_0x90 >> 4) & 0xF;
    mLife = 0x80;
    if ((field_0x90 & 0xF) != 0) {
        field_0x23c = 1;
        mBox.set(-5.6f, -5.6f, 5.6f, 5.6f);
    } else {
        field_0x23c = 0;
        mBox.set(-2.8f, -2.8f, 2.8f, 2.8f);
    }
    switch (field_0x240) {
    case 0:
        field_0x3c.x = 0.0f;
        field_0x3c.y = 1.35f;
        field_0x3c.z = 0.0f;
        break;
    case 1:
        field_0x3c.x = 0.0f;
        field_0x3c.y = -1.35f;
        field_0x3c.z = 0.0f;
        break;
    case 2:
        field_0x3c.x = 1.35f;
        field_0x3c.y = 0.0f;
        field_0x3c.z = 0.0f;
        break;
    case 3:
        field_0x3c.x = -1.35f;
        field_0x3c.y = 0.0f;
        field_0x3c.z = 0.0f;
        break;
    }
    field_0x230 = 0;
    field_0x238 = 0;
    field_0x234 = 30;
    field_0x244 = 0;
    startSound_801F0E34(0x30019, 0);
}
