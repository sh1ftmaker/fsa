#include "game/ActorBase.h"
#include "game/GameAudio.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/cls_804A1630.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "std/math.h"

// The marker frames of the SHA actor (.data 0x804A1440, not written as source here), by row.
extern const s32 lbl_804A1440[];
extern s32 lbl_80539F98;
extern s32 lbl_80539F9C;
extern s32 lbl_80539FA0;
extern s32 lbl_80539FA4;
extern s32 lbl_8053AB70;
extern u8 lbl_8053AB74;
extern u8 lbl_8053AB75;

// Out-of-line copy of TVec2<f32>::set(x, y).
void fn_801826E4(JGeometry::TVec2f* v, f32 x, f32 y) {
    v->set(x, y);
}

// The view's box for an actor layer: mVisibleBox for layers 0-7, field_0xdc
// for the rest.
JGeometry::TBox2f* fn_801826A4(s32 layer) {
    GameView_804A9950* view = getGameView_802D6A9C();
    if (layer < 8) {
        return &view->mVisibleBox;
    }
    return &view->field_0xdc;
}

// Out-of-line copy of TVec2<f32>::set(const TVec2&).
void fn_80182690(JGeometry::TVec2f* dst, const JGeometry::TVec2f* src) {
    dst->set(*src);
}

// Out-of-line copy of TVec3<f32>::set(const TVec3&).
void fn_80182674(JGeometry::TVec3f* dst, const JGeometry::TVec3f* src) {
    dst->set(*src);
}

void ActorBase_804A5770::fn_80182660(const JGeometry::TVec3f& pos) {
    mPos = pos;
}

// Out-of-line copy of TVec3<f32>::set(x, y, z).
void fn_80182650(JGeometry::TVec3f* v, f32 x, f32 y, f32 z) {
    v->set(x, y, z);
}

// A second out-of-line copy of TVec3<f32>::set(x, y, z).
void fn_80182640(JGeometry::TVec3f* v, f32 x, f32 y, f32 z) {
    v->set(x, y, z);
}

// FIXME(shotgun): 98.77 %, missing/extra + reloc-name.
void cls_804A1630::init() {
    field_0x90 &= 0xF;
    field_0x94 = 0;
    field_0x80.i.x = -1.0f;
    field_0x80.i.y = -1.0f;
    field_0x80.f.x = 1.0f;
    field_0x80.f.y = 1.0f;
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x254 = 0;
    field_0x258 = 0;
    field_0x25c = 0;
    field_0x260 = 0;
    field_0x264 = 0;
    field_0x268 = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x196 = 1;
    field_0x278 = 0;
    field_0x274 = -1;
    for (s32 i = 0; i < 4; i++) {
        field_0x294[i].set(mPos);
        field_0x2c4[i].set(mPos);
        field_0x2f4[i] = 0;
        field_0x304[i] = 0;
    }
    field_0x98 = 0;
    field_0x26c = 0;
    field_0x314 = 0.0f;
    mEmitter318[0] = NULL;
    mEmitter318[1] = NULL;
    field_0x270 = -1;
    field_0x288.set(mPos);
    field_0x30c = 1.0f;
    field_0x310 = 255.0f;
    field_0x284 = 0.0f;
    field_0x280 = 0.0f;
    field_0x27c = 0.0f;
    field_0x308 = 0.0f;
    switch (field_0x90) {
    case 0:
        mBox.i.x = -16.0f;
        mBox.i.y = -16.0f;
        mBox.f.x = 16.0f;
        mBox.f.y = 16.0f;
        mBox2.i.x = -16.0f;
        mBox2.i.y = -16.0f;
        mBox2.f.x = 16.0f;
        mBox2.f.y = 16.0f;
        getGameView_802D6A9C()->fn_802D7440(false);
        gAudioMgr_8053AF80->fn_80458F9C(60);
        lbl_8053AB70 = 0;
        lbl_80539F98 = 8;
        lbl_80539F9C = 180;
        if (gGameRoot_8053AAF8->fn_80138A30() > 1) {
            lbl_80539F9C = 110;
        }
        lbl_8053AB74 = 0;
        lbl_8053AB75 = 0;
        field_0x268 = -1;
        for (s32 i = 0; i < 4; i++) {
            fn_80304C10(i, "gb_ch_obj_tr_sch0.szs", 0x6017000, 1, 0);
        }
        gGameRoot2_8053AB10->field_0x20->fn_802F9714(
            gGameRoot_8053AAF8->field_0xa4->getResource('SCH ', "gb_ch_obj_tr_sch0.szs"), 0, 0, 0x7000, 0);
        field_0xb0 = 0x9040004;
        field_0x98 = 1;
        mPos.z = 500.0f;
        field_0x308 = 500.0f;
        mState = 0;
        break;
    case 1:
        field_0xb0 = 0x9040004;
        field_0x1a8 = false;
        mBox.i.x = -16.0f;
        mBox.i.y = -16.0f;
        mBox.f.x = 16.0f;
        mBox.f.y = 16.0f;
        mBox2.i.x = -16.0f;
        mBox2.i.y = -16.0f;
        mBox2.f.x = 16.0f;
        mBox2.f.y = 16.0f;
        mState = 0x10;
        break;
    case 4:
        field_0x98 = 1;
        // fall through
    case 5:
        field_0xb0 = 0x5000000;
        field_0x1a8 = false;
        mBox.i.x = -32.0f;
        mBox.i.y = -32.0f;
        mBox.f.x = 32.0f;
        mBox.f.y = 32.0f;
        mBox2.i.x = -48.0f;
        mBox2.i.y = -48.0f;
        mBox2.f.x = 48.0f;
        mBox2.f.y = 48.0f;
        mState = 0x11;
        break;
    case 6:
        field_0x1a8 = false;
        field_0x30c = 0.2f * (gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f) + 0.1f;
        mState = 0x15;
        break;
    case 7:
        field_0x308 = 500.0f;
        field_0x1a8 = false;
        field_0xb0 = 0x4000000;
        mState = 0x17;
        break;
    case 8:
        field_0x26c = (s32)(2.99f * (gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f)) + 0x13;
        field_0x1a8 = false;
        mState = 0x19;
        break;
    case 9:
        field_0x1a8 = false;
        field_0x234 = 20;
        field_0x24c = 1;
        if (field_0x98 == 0) {
            field_0x26c = 0x16;
        } else {
            field_0x26c = 0x17;
        }
        mPos.y -= 22.0f;
        mState = 0x19;
        break;
    case 10:
        field_0x26c = (s32)(2.99f * (gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f)) + 0x10;
        field_0x1a8 = false;
        mState = 0x19;
        break;
    default:
        if (field_0x90 == 2) {
            lbl_80539FA0 = mSerial;
        } else if (field_0x90 == 3) {
            lbl_80539FA4 = mSerial;
        }
        field_0x1a8 = false;
        field_0xb0 = 0x4000000;
        mBox.i.x = -16.0f;
        mBox.i.y = -16.0f;
        mBox.f.x = 16.0f;
        mBox.f.y = 16.0f;
        mBox2.i.x = -16.0f;
        mBox2.i.y = -16.0f;
        mBox2.f.x = 16.0f;
        mBox2.f.y = 16.0f;
        mState = 0x16;
        break;
    }
}

// FIXME(shotgun): 97.75 %, missing/extra + reloc-name.
void cls_804A1630::fn_80181BC8(s32 row, s32 mode) {
    s32 frame = 0;
    for (s32 i = 0; i < 4; i++) {
        if (!fn_8023F264(i)) {
            continue;
        }
        if (field_0x4 != getPlayerLayer_8023DE58(i)) {
            continue;
        }
        switch (mode) {
        case 0:
            if (getPlayerPos_8023E724(i)->x < mPos.x) {
                row = 0;
            } else {
                row = 1;
            }
            field_0x2f4[i] = 0;
            field_0x294[i].set(*getPlayerPos_8023E724(i));
            field_0x2c4[i].set(*getPlayerPos_8023E724(i));
            field_0x294[i].z = 0.0f;
            field_0x2c4[i].z = 0.0f;
            field_0x304[i] = 0;
            break;
        case 1:
            if (field_0x304[i] == 1) {
                row += 4;
            }
            if (field_0x2f4[i] < 4) {
                frame = 0;
            } else if (field_0x2f4[i] < 8) {
                frame = 1;
            } else if (field_0x2f4[i] < 11) {
                frame = 2;
            } else if (field_0x2f4[i] < 14) {
                frame = 3;
            } else if (field_0x2f4[i] < 17) {
                frame = 4;
            } else if (field_0x2f4[i] < 20) {
                frame = 5;
            } else {
                field_0x2f4[i] = 20;
                frame = 0;
                row = 3;
            }
            addCalc2_801F71A4(&field_0x294[i].y, field_0x2c4[i].y, 0.3f, 1.0f);
            if (field_0x304[i] == 0) {
                f32 dx = getPlayerPos_8023E724(i)->x - mPos.x;
                if (std::sqrtf(dx * dx) < 128.0f) {
                    f32 dy = getPlayerPos_8023E724(i)->y - field_0x288.y;
                    if (std::sqrtf(dy * dy) < 80.0f) {
                        field_0x2c4[i].y = 30.0f + getPlayerPos_8023E724(i)->y;
                        field_0x304[i] = 1;
                    }
                }
            } else {
                field_0x2f4[i]++;
            }
            break;
        }
        if (mode == 0 || field_0x304[i] == 1) {
            fn_80230498(i, &field_0x294[i], frame + lbl_804A1440[row], 0xFF, -0x80, -0x80, true);
        }
    }
}

// The actor's destructor: unless field_0x90 is set, it turns the view's flag
// off and calls fn_8022ADCC; then it releases its two emitters.
cls_804A1630::~cls_804A1630() {
    if (field_0x90 == 0) {
        getGameView_802D6A9C()->fn_802D7440(true);
        fn_8022ADCC();
    }
    for (s32 i = 0; i < 2; i++) {
        if (mEmitter318[i] != NULL) {
            releaseEmitter_801EE434(mEmitter318[i]);
        }
    }
}

void cls_804A1630::vfunc_0x24() {
    startSound_801F0E34(0x40038, 0);
    vfunc_0x28(2, 0.0f, -24.0f);
}

// FIXME(shotgun): draw (fn_8017EF98, 0xB78 bytes) and update (fn_8017FB10, 0x20B8 bytes) are not written.
