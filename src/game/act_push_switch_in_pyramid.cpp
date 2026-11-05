#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/act/act_push_switch_in_pyramid.h"

// The box (i.x, i.y, f.x, f.y) of each push step; the last one is empty.
static const f32 lbl_804751F8[5][4] = {
    {-7.5f, -7.5f, 23.5f, 7.5f}, {-7.5f, -7.5f, 19.5f, 7.5f}, {-7.5f, -7.5f, 13.5f, 7.5f},
    {-7.5f, -7.5f, 7.5f, 7.5f},  {-0.0f, -0.0f, 0.0f, 0.0f},
};

static const f32 lbl_80475248[4] = {8.0f, 32.0f, 48.0f, 64.0f};

static const char* lbl_804AE8E0[5] = {
    "kabe_switch_0.bti", "kabe_switch_1.bti", "kabe_switch_2.bti",
    "kabe_switch_3.bti", "kabe_switch_4.bti",
};

PushSwitchInPyramid_804AE8F4::~PushSwitchInPyramid_804AE8F4() {}

// FIXME: 97.85%. In the squared-length test of field_0x54 the registers f2/f3
// are swapped (the target has the psq pair in f2 and z in f3), and the target
// loads y before x when filling the scale matrix.
void PushSwitchInPyramid_804AE8F4::draw() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    if (!(field_0x54.squared() <= 32.0f * FLT_EPSILON)) {
        mtx[0][3] = 0.0f;
        mtx[1][3] = 0.0f;
        mtx[2][3] = 0.0f;
        mtx[0][0] = field_0x54.x;
        mtx[0][1] = 0.0f;
        mtx[0][2] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = field_0x54.y;
        mtx[1][2] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = field_0x54.z;
        params.mMtx = mtx;
    }
    JGeometry::TVec3f pos;
    pos.set(mPos);
    pos.x += 8.0f;
    void* res = fn_804032E8('TIMG', lbl_804AE8E0[field_0x241]);
    if (field_0x4 < 8) {
        gGameRoot2_8053AB10->field_0x20->fn_802F745C(&pos, res, &params);
    }
}

void PushSwitchInPyramid_804AE8F4::update() {
    JGeometry::TBox2f box = mBox;
    box.i.x += mPos.x;
    box.i.y += mPos.y;
    box.f.x += mPos.x;
    box.f.y += mPos.y;
    fn_80239914(&box, field_0x4, mSerial);
    fn_8020D58C()->addBox(mSerial);
    switch (field_0x23c) {
    case 0: {
        s32 player;
        if (fn_80239108(mSerial) >= 1 && field_0x240 == (player = fn_80238BC4(mSerial)) &&
            player >= 0 && player < 4) {
            field_0x242++;
            if (field_0x242 >= 20) {
                if (((field_0x90 >> 4) & 1) == 0) {
                    getGameView_802D6A9C()->fn_802D7584(field_0x16c, player + 1);
                }
                fn_80238E08(mSerial);
                field_0x243++;
                if (lbl_80475248[field_0x241] < field_0x243) {
                    field_0x241++;
                    startSound_801F0E34(0x3004B, 0);
                }
                mBox.set(-7.5f, -7.5f, 18.5f - (field_0x243 / 4), 7.5f);
                if (field_0x241 == 3) {
                    mBox.set(-7.5f, -7.5f, 0.0f, 7.5f);
                    vfunc_0x30(0xFF, true);
                    field_0x23c = 2;
                }
            }
        } else {
            field_0x242 = 0;
        }
        break;
    }
    case 1:
        field_0x241++;
        field_0x242 = 0;
        mBox.set(lbl_804751F8[field_0x241][0], lbl_804751F8[field_0x241][1],
                 lbl_804751F8[field_0x241][2], lbl_804751F8[field_0x241][3]);
        if (field_0x241 == 4) {
            vfunc_0x30(0xFF, true);
            field_0x23c = 2;
        } else {
            field_0x23c = 0;
        }
        break;
    case 2: // done; nothing to do
        break;
    }
}

void PushSwitchInPyramid_804AE8F4::init() {
    mBox.set(-7.5f, -7.5f, 18.5f, 7.5f);
    field_0x240 = 1;
    field_0x23c = 0;
    field_0x243 = 0;
    field_0x242 = 0;
    field_0x241 = 0;
    if (((field_0x90 >> 4) & 1) == 0) {
        if (getGameView_802D6A9C()->fn_802D7544(field_0x16c)) {
            field_0x241 = 3;
            mBox.set(-7.5f, -7.5f, 0.0f, 7.5f);
            vfunc_0x30(0xFF, true);
            field_0x23c = 2;
        }
    }
}
