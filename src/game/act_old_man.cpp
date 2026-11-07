// The old man (actor JIJI, "Old Man" in docs/names/actors.tsv). The class is
// cls_804AEED0 (its vtable, set by the constructor at 0x8037CE00); its
// declaration is in game/act/tu_801F9484.h with the spawner.

#include "JSystem/JMath/JMath.h"
#include "game/ActorBase.h"
#include "game/Effect.h"
#include "game/EmitterDelete.h"
#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/GameView.h"
#include "game/Message.h"
#include "game/Player.h"
#include "game/PlayerBit.h"
#include "game/cls_8031709C.h"
#include "game/act/tu_801F9484.h"

// Sprite bank selected by field_0x260 (lbl_804753C0[field_0x258].mKind) in draw().
enum OldManSpriteKind {
    OLD_MAN_SOB_GC = 0,    // "enemy_gc_sob.bin"
    OLD_MAN_SOB_BOTH = 1,  // "enemy_both_sob.bin" (fn_80307AC8)
    OLD_MAN_SOB_BOTH2 = 2, // "enemy_both2_sob.bin" (fn_803075AC)
    OLD_MAN_SOB_NONE = 3,  // kind 21 (drawn nowhere)
};

struct OldManSpriteInfo {
    u16 mFrame;
    u16 mKind;
};

static const OldManSpriteInfo lbl_804753C0[54] = {
    {0x0386, OLD_MAN_SOB_BOTH2}, {0x01AB, OLD_MAN_SOB_BOTH2}, {0x009B, OLD_MAN_SOB_BOTH2},
    {0x03A4, OLD_MAN_SOB_BOTH2}, {0x0388, OLD_MAN_SOB_BOTH2}, {0x0394, OLD_MAN_SOB_BOTH2},
    {0x03B4, OLD_MAN_SOB_BOTH2}, {0x00D5, OLD_MAN_SOB_GC},    {0x0232, OLD_MAN_SOB_BOTH2},
    {0x0234, OLD_MAN_SOB_BOTH2}, {0x0236, OLD_MAN_SOB_BOTH2}, {0x0259, OLD_MAN_SOB_BOTH2},
    {0x025F, OLD_MAN_SOB_BOTH2}, {0x024F, OLD_MAN_SOB_BOTH2}, {0x00C3, OLD_MAN_SOB_BOTH},
    {0x0218, OLD_MAN_SOB_BOTH2}, {0x026B, OLD_MAN_SOB_BOTH2}, {0x02B1, OLD_MAN_SOB_GC},
    {0x02B6, OLD_MAN_SOB_GC},    {0x02B4, OLD_MAN_SOB_GC},    {0x02B8, OLD_MAN_SOB_GC},
    {0x0000, OLD_MAN_SOB_NONE},  {0x007F, OLD_MAN_SOB_BOTH},  {0x004D, OLD_MAN_SOB_BOTH},
    {0x006F, OLD_MAN_SOB_BOTH},  {0x0059, OLD_MAN_SOB_BOTH},  {0x030F, OLD_MAN_SOB_BOTH2},
    {0x02BD, OLD_MAN_SOB_BOTH2}, {0x02C0, OLD_MAN_SOB_BOTH2}, {0x00EC, OLD_MAN_SOB_BOTH},
    {0x00F4, OLD_MAN_SOB_BOTH},  {0x00FC, OLD_MAN_SOB_BOTH},  {0x02D2, OLD_MAN_SOB_GC},
    {0x02E6, OLD_MAN_SOB_GC},    {0x037E, OLD_MAN_SOB_BOTH2}, {0x00C0, OLD_MAN_SOB_BOTH2},
    {0x017F, OLD_MAN_SOB_GC},    {0x0199, OLD_MAN_SOB_GC},    {0x01B3, OLD_MAN_SOB_GC},
    {0x01CD, OLD_MAN_SOB_GC},    {0x01E7, OLD_MAN_SOB_GC},    {0x0201, OLD_MAN_SOB_GC},
    {0x0315, OLD_MAN_SOB_BOTH2}, {0x0148, OLD_MAN_SOB_BOTH},  {0x014C, OLD_MAN_SOB_BOTH},
    {0x014A, OLD_MAN_SOB_BOTH},  {0x0152, OLD_MAN_SOB_BOTH},  {0x0154, OLD_MAN_SOB_BOTH},
    {0x0146, OLD_MAN_SOB_BOTH},  {0x0138, OLD_MAN_SOB_BOTH},  {0x04E2, OLD_MAN_SOB_GC},
    {0x04F0, OLD_MAN_SOB_GC},    {0x04FE, OLD_MAN_SOB_GC},    {0x050A, OLD_MAN_SOB_GC},
};

static const s32 lbl_80475498[11][4] = {
    {0, 0, 0, 0},
    {1, 1, 1, 1},
    {0, 2, 4, 6},
    {0, 3, 6, 9},
    {0, 4, 8, 12},
    {0, 6, 12, 17},
    {0, 8, 16, 24},
    {0, 2, 4, 0},
    {0, 2, 4, 7},
    {19, 20, 21, 38},
    {6, 7, 8, 9},
};

// Animation step tables (.sdata, in address order).
static u8 lbl_8053A1D0[4] = {7, 10, 7, 10};
static u8 lbl_8053A1D4[4] = {0, 1, 0, 2};
static u8 lbl_8053A1D8[2] = {20, 15};
static u8 lbl_8053A1DC[4] = {20, 13, 16, 13};
static u8 lbl_8053A1E0[4] = {0, 1, 2, 1};
static u8 lbl_8053A1E4[4] = {10, 8, 10, 8};
static u8 lbl_8053A1E8[4] = {0, 1, 0, 2};
static u8 lbl_8053A1EC[4] = {12, 10, 12, 10};
static u8 lbl_8053A1F0[4] = {0, 1, 2, 0};
static u8 lbl_8053A1F4[2] = {15, 20};
static u8 lbl_8053A1F8[4] = {10, 7, 10, 7};
static u8 lbl_8053A1FC[4] = {1, 0, 2, 0};
static u8 lbl_8053A200[4] = {12, 10, 12, 10};
static u8 lbl_8053A204[2][4] = {{0, 1, 0, 1}, {0, 1, 0, 2}};
// 3 bytes in the original (symbol size).
static u8 lbl_8053A20C[3] = {60, 10, 0};

// Not const, so its initialiser (lbl_804AE9C0) is copied into it by __sinit_act_old_man_cpp.
static void (cls_804AEED0::*lbl_804AEC48[54])() = {
    &cls_804AEED0::fn_8037B7BC, &cls_804AEED0::fn_8037B6C4, &cls_804AEED0::fn_8037B78C,
    &cls_804AEED0::fn_8037B7BC, &cls_804AEED0::fn_8037B67C, &cls_804AEED0::fn_8037B7BC,
    &cls_804AEED0::fn_8037B634, &cls_804AEED0::fn_8037B78C, &cls_804AEED0::fn_8037B5FC,
    &cls_804AEED0::fn_8037B7BC, &cls_804AEED0::fn_8037B5B4, &cls_804AEED0::fn_8037B7BC,
    &cls_804AEED0::fn_8037B744, &cls_804AEED0::fn_8037B554, &cls_804AEED0::fn_8037B7EC,
    &cls_804AEED0::fn_8037B744, &cls_804AEED0::fn_8037B7BC, &cls_804AEED0::fn_8037B7BC,
    &cls_804AEED0::fn_8037B7BC, &cls_804AEED0::fn_8037B7BC, &cls_804AEED0::fn_8037B7BC,
    &cls_804AEED0::fn_8037B7BC, &cls_804AEED0::fn_8037B7BC, &cls_804AEED0::fn_8037B67C,
    &cls_804AEED0::fn_8037B7BC, &cls_804AEED0::fn_8037B634, &cls_804AEED0::fn_8037B4D0,
    &cls_804AEED0::fn_8037B488, &cls_804AEED0::fn_8037B3E0, &cls_804AEED0::fn_8037B7BC,
    &cls_804AEED0::fn_8037B7BC, &cls_804AEED0::fn_8037B7BC, &cls_804AEED0::fn_8037B70C,
    &cls_804AEED0::fn_8037B70C, &cls_804AEED0::fn_8037B3A8, &cls_804AEED0::fn_8037B7EC,
    &cls_804AEED0::fn_8037B7EC, &cls_804AEED0::fn_8037B7EC, &cls_804AEED0::fn_8037B7EC,
    &cls_804AEED0::fn_8037B7EC, &cls_804AEED0::fn_8037B7EC, &cls_804AEED0::fn_8037B7EC,
    &cls_804AEED0::fn_8037B7BC, &cls_804AEED0::fn_8037B7BC, &cls_804AEED0::fn_8037B7BC,
    &cls_804AEED0::fn_8037B7BC, &cls_804AEED0::fn_8037B7BC, &cls_804AEED0::fn_8037B7BC,
    &cls_804AEED0::fn_8037B7BC, &cls_804AEED0::fn_8037B7BC, &cls_804AEED0::fn_8037B7BC,
    &cls_804AEED0::fn_8037B7BC, &cls_804AEED0::fn_8037B7BC, &cls_804AEED0::fn_8037B7BC,
};

cls_804AEED0::cls_804AEED0() {
    field_0x274 = new cls_8031709C;
}

cls_804AEED0::~cls_804AEED0() {
    if (field_0x274 != NULL) {
        delete field_0x274;
    }
    if (field_0x278 != NULL) {
        releaseEmitter_801EE434(field_0x278);
    }
}

void cls_804AEED0::init() {
    field_0x230 = 0;
    field_0x250 = (field_0x90 & 0xFFF) + 0x12B;
    field_0x254 = (field_0x90 >> 24) & 1;
    field_0x258 = (field_0x90 >> 16) & 0xFF;
    field_0x271 = (field_0x90 >> 25) & 1;
    field_0x268 = (field_0x90 >> 12) & 3;
    field_0x272 = 1;
    field_0x26c = 0;
    field_0x270 = 0;
    if (field_0x271 == 1) {
        field_0x230 = 2;
    }
    if (field_0x258 > 54) {
        field_0x258 = 0;
    }
    if (field_0x258 == 6 || field_0x258 == 11 || field_0x258 == 16 || field_0x258 == 25) {
        mBox.set(-16.0f, -16.0f, 16.0f, 8.0f);
    } else {
        mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    }
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    if (field_0x271 == 1) {
        field_0x3c.z = 4.0f;
    }
    mLife = 1;
    field_0x238 = 0;
    field_0x240 = 0;
    field_0x248 = 2;
    field_0x24c = 0;
    updateFacing(0);
    field_0x244 = -1;
    field_0x25c = lbl_804753C0[field_0x258].mFrame;
    field_0x260 = lbl_804753C0[field_0x258].mKind;
    field_0x273 = 0;
    switch (field_0x258) {
    case 1:
        field_0x234 = 20;
        field_0x23c = 0;
        field_0x264 = 0;
        break;
    case 2:
        field_0x23c = 3;
        field_0x234 = 3;
        field_0x264 = 6;
        break;
    case 3:
    case 22:
        field_0x23c = 15;
        field_0x234 = 15;
        field_0x264 = 4;
        break;
    case 4:
    case 23:
        field_0x234 = 10;
        field_0x23c = 0;
        field_0x264 = 3;
        break;
    case 5:
    case 24:
        field_0x23c = 20;
        field_0x234 = 20;
        field_0x264 = 4;
        break;
    case 6:
    case 25:
        field_0x234 = 12;
        field_0x23c = 0;
        field_0x264 = 5;
        break;
    case 7:
        field_0x23c = 6;
        field_0x234 = 6;
        field_0x264 = 0;
        break;
    case 8:
        field_0x234 = 15;
        field_0x23c = 0;
        field_0x264 = 0;
        break;
    case 9:
        field_0x23c = 10;
        field_0x234 = 10;
        field_0x264 = 0;
        break;
    case 10:
        field_0x234 = 10;
        field_0x23c = 0;
        field_0x264 = 3;
        break;
    case 11:
        field_0x23c = 20;
        field_0x234 = 20;
        field_0x264 = 7;
        break;
    case 12:
        field_0x234 = 7;
        field_0x23c = 0;
        field_0x264 = 3;
        break;
    case 13:
        field_0x234 = 12;
        field_0x23c = 0;
        field_0x264 = 8;
        break;
    case 14:
        field_0x234 = 0;
        field_0x23c = 0;
        field_0x264 = 6;
        break;
    case 15:
        field_0x234 = 7;
        field_0x23c = 0;
        field_0x264 = 3;
        break;
    case 16:
        field_0x23c = 15;
        field_0x234 = 15;
        field_0x264 = 0;
        break;
    case 17:
        field_0x23c = 15;
        field_0x234 = 15;
        field_0x264 = 1;
        break;
    case 18:
        field_0x23c = 15;
        field_0x234 = 15;
        field_0x264 = 0;
        break;
    case 19:
        field_0x23c = 15;
        field_0x234 = 15;
        field_0x264 = 0;
        break;
    case 20:
        field_0x23c = 15;
        field_0x234 = 15;
        field_0x264 = 0;
        break;
    case 21:
        field_0x234 = 0;
        field_0x23c = 0;
        field_0x264 = 0;
        break;
    case 26:
        field_0x234 = 55;
        field_0x23c = 0;
        field_0x264 = 0;
        break;
    case 27:
        field_0x234 = 40;
        field_0x23c = 0;
        field_0x264 = 0;
        break;
    case 28:
        field_0x234 = 90;
        field_0x23c = 0;
        field_0x264 = 2;
        break;
    case 29:
    case 30:
        field_0x23c = 20;
        field_0x234 = 20;
        field_0x264 = 2;
        break;
    case 31:
        field_0x23c = 20;
        field_0x234 = 20;
        field_0x264 = 2;
        break;
    case 32:
    case 33:
        field_0x234 = 20;
        field_0x23c = 0;
        field_0x264 = 2;
        break;
    case 34:
        field_0x264 = 2;
        field_0x234 = 60;
        field_0x23c = 0;
        break;
    case 35:
        field_0x234 = 0;
        field_0x23c = 0;
        field_0x264 = 9;
        break;
    case 36:
    case 37:
    case 38:
    case 39:
    case 40:
    case 41:
        field_0x23c = 0;
        field_0x234 = 0;
        field_0x264 = 10;
        break;
    case 42:
        field_0x23c = 40;
        field_0x234 = 40;
        field_0x264 = 0;
        break;
    case 43:
        field_0x23c = 18;
        field_0x234 = 18;
        field_0x264 = 0;
        break;
    case 44:
    case 45:
    case 46:
    case 47:
        field_0x23c = 16;
        field_0x234 = 16;
        field_0x264 = 0;
        break;
    case 48:
        field_0x23c = 10;
        field_0x234 = 10;
        field_0x264 = 0;
        break;
    case 49:
        field_0x23c = 20;
        field_0x234 = 20;
        field_0x264 = 0;
        break;
    case 50:
    case 51:
    case 52:
    case 53:
        field_0x23c = 16;
        field_0x234 = 16;
        field_0x264 = 2;
        break;
    case 0:
    default:
        field_0x23c = 20;
        field_0x234 = 20;
        field_0x264 = 0;
        break;
    }
    if (field_0x274 != NULL) {
        field_0x274->fn_80316E50(getField_0x4(), JGeometry::TVec3f(mPos));
    }
    field_0x278 = NULL;
}

void cls_804AEED0::update() {
    s32 readyCount = 0;
    s32 activeCount = 0;
    JGeometry::TBox2f box1;
    box1.set(mBox);
    box1.i.x += mPos.x;
    box1.i.y += mPos.y;
    box1.f.x += mPos.x;
    box1.f.y += mPos.y;
    if (field_0x230 != 3 && field_0x230 != 4) {
        fn_80239914(&box1, field_0x4, mSerial);
    }
    if (field_0x258 == 4 && field_0x273 == 1) {
        field_0x273 = 0;
        field_0x234--;
        (this->*lbl_804AEC48[field_0x258])();
        return;
    }
    JGeometry::TBox2f box2;
    if (field_0x272 == 1) {
        if (field_0x268 == 1) {
            box2.set(mBox.i.x - 16.0f, mBox.i.y - 16.0f, 16.0f + mBox.f.x, 16.0f + mBox.f.y);
        } else if (field_0x268 == 2) {
            box2.set(mBox);
        } else if (field_0x268 == 3) {
            box2.set(mBox.i.x - 40.0f, mBox.i.y - 40.0f, 40.0f + mBox.f.x, 40.0f + mBox.f.y);
        }
    } else {
        if (field_0x268 == 1) {
            box2.set(mBox.i.x - 28.0f, mBox.i.y - 28.0f, 28.0f + mBox.f.x, 28.0f + mBox.f.y);
        } else if (field_0x268 == 2) {
            box2.set(mBox.i.x - 12.0f, mBox.i.y - 12.0f, 12.0f + mBox.f.x, 12.0f + mBox.f.y);
        } else if (field_0x268 == 3) {
            box2.set(mBox.i.x - 48.0f, mBox.i.y - 48.0f, 48.0f + mBox.f.x, 48.0f + mBox.f.y);
        }
    }
    box2.i.x += mPos.x;
    box2.i.y += mPos.y;
    box2.f.x += mPos.x;
    box2.f.y += mPos.y;
    if (field_0x230 == 2) {
        mPos.add(field_0x3c);
        field_0x3c.z -= 0.2f;
    }
    switch (field_0x230) {
    case 0:
        if (field_0x4 < 8) {
            field_0x230 = 1;
        } else {
            field_0x230 = 3;
        }
        if (field_0x258 == 18 || field_0x258 == 19 || field_0x258 == 20) {
            field_0x278 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x1FE, 0, 0, NULL, NULL, true, 1.0f);
        }
        break;
    case 3:
        for (s32 i = 0; i < 4; i++) {
            if (isPlayerBitSet(gGameRoot_8053AAF8, i) == true) {
                activeCount++;
                if (!fn_80303750(i) || !fn_8022C8EC(i)) {
                    readyCount++;
                }
            }
        }
        if (readyCount == activeCount) {
            field_0x230 = 1;
        }
        break;
    case 4:
        if (field_0x4 < 8) {
            JGeometry::TVec3f effPos;
            effPos.x = mPos.x;
            effPos.y = mPos.y;
            effPos.z = mPos.z;
            effPos.y += 32.0f;
            effPos.z += 16.0f;
            spawnEmitter_8013CC50(field_0x4, &effPos, 0x17D, 1, 0, NULL, NULL, true, 1.0f);
            gGameRoot2_8053AB10->field_0x30->fn_8015F8FC(field_0x4, &effPos, 3);
        }
        field_0x230 = 1;
        break;
    case 1:
        updateFacing(0);
        if (!fn_8022B160()) {
            if (field_0x268 == 0) {
                fn_8022FEB8(&box1, field_0x4, mSerial, true, false);
            } else if (field_0x272 == 1) {
                fn_8022FEB8(&box2, field_0x4, mSerial, false, false);
            } else {
                if (fn_8023AF14(&box2, field_0x4, false, 500.0f) == -1) {
                    field_0x272 = 1;
                }
                break;
            }
            field_0x244 = fn_8022FD8C(mSerial);
            if (lbl_80529DEC.field_0x118 >= 8 && lbl_80529DEC.field_0x11c == 0) {
                field_0x250 = fn_8037E54C(field_0x244);
            } else if (lbl_8050DB0C.field_0x4 != 0) {
                field_0x250 = lbl_8050DB0C.field_0x6;
            }
            if (field_0x244 >= 0 && getPlayerPos_8023E724(field_0x244)->z == 0.0f &&
                fn_802300EC(field_0x244) &&
                fn_8038C678(field_0x244, field_0x250, getSerial(), getField_0x4(), mPos, 0x10)) {
                field_0x24c = 0;
                updateFacing(field_0x244 + 1);
                field_0x230 = 5;
                field_0x240 = 0;
                if (field_0x258 == 18 || field_0x258 == 19 || field_0x258 == 20) {
                    startSound_801F0E34(0x60001, 0);
                }
            }
        }
        break;
    case 2:
        if (mPos.z <= 0.0f) {
            mPos.z = 0.0f;
            field_0x3c.z = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x3c.x = 0.0f;
            field_0x230 = 1;
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x3CF, 0, -1, NULL, NULL, true, 1.5f);
            getGameView_802D6A9C()->fn_802D79E8(field_0x4, 4.0f, 0.0f, false);
        }
        break;
    case 5:
        if (field_0x268 == 0) {
            fn_8022FF8C(field_0x244, &box1, field_0x4, mSerial, true, false);
        } else {
            fn_8022FF8C(field_0x244, &box2, field_0x4, mSerial, false, false);
        }
        if (fn_80388CAC(field_0x244, mSerial)) {
            field_0x240 = 0;
            field_0x244 = -1;
            vfunc_0x30(0xFF, true);
            if (field_0x268 != 0) {
                field_0x272 = 0;
                field_0x268 = 0;
            }
            if (field_0x254 != 0) {
                field_0x230 = 6;
            } else {
                field_0x230 = 1;
            }
            if (field_0x274 != NULL) {
                field_0x274->fn_80316E50(getField_0x4(), JGeometry::TVec3f(mPos));
            }
        }
        break;
    case 6:
        field_0x240++;
        if (field_0x240 >= 150) {
            if (field_0x4 < 8) {
                JGeometry::TVec3f effPos;
                effPos.x = mPos.x;
                effPos.y = mPos.y;
                effPos.z = mPos.z;
                effPos.y += 32.0f;
                effPos.z += 16.0f;
                spawnEmitter_8013CC50(field_0x4, &effPos, 0x17D, 1, 0, NULL, NULL, true, 1.0f);
                gGameRoot2_8053AB10->field_0x30->fn_8015F8FC(field_0x4, &mPos, 3);
            }
            deleteEmitter(&field_0x278);
            startSound_801F0E34(0x40017, 0);
            mAlive = false;
        }
        break;
    }
    if (field_0x258 == 18 || field_0x258 == 19 || field_0x258 == 20) {
        f32 dist;
        fn_801EE558(NULL, &dist, 0);
        if (field_0x230 == 5) {
            field_0x270 += 2;
            if (field_0x270 >= 200) {
                field_0x270 = 200;
            }
            field_0x26c = 0;
        } else if (dist < 48.0f) {
            if ((field_0x26c & 1) == 0) {
                field_0x270 += 2;
                if (field_0x270 >= 200) {
                    field_0x26c++;
                }
            } else {
                field_0x270--;
                if (field_0x26c < 6) {
                    if (field_0x270 <= 80) {
                        field_0x26c++;
                    }
                } else {
                    if (field_0x270 <= 40) {
                        field_0x26c++;
                    }
                }
            }
            if (field_0x26c >= 8) {
                field_0x26c = 0;
            }
        } else {
            if (field_0x270 > 2) {
                field_0x270 -= 2;
            } else {
                field_0x270 = 0;
            }
            field_0x26c = 0;
        }
    }
    field_0x234--;
    (this->*lbl_804AEC48[field_0x258])();
}

// FIXME: 99.56%. regalloc-only: target->ours r8->r7, r7->r8, r7->r0, r0->r8.
void cls_804AEED0::draw() {
    if (field_0x230 == 5 && field_0x274 != NULL) {
        u32 sfx = field_0x274->fn_80316E74(field_0x244);
        if (sfx != -1U) {
            startSound_801F0E34(sfx, 0);
        }
    }
    if (field_0x230 == 4) {
        return;
    }
    if (field_0x258 == 21) {
        return;
    }
    if (field_0x258 == 17 && field_0x230 == 2) {
        drawSprite_801F06F0("enemy_gc_sob.bin", 0x2B1, NULL, 7, 1.0f, 1.0f);
        return;
    }
    if (field_0x258 == 18 || field_0x258 == 19 || field_0x258 == 20) {
        SpriteParams_801F5AE0 params;
        params.field_0x10.a = field_0x270;
        drawSprite_801F06F0("enemy_gc_sob.bin",
                            field_0x25c + lbl_80475498[field_0x264][field_0x248] + field_0x238,
                            &params, 7, 1.0f, 1.0f);
        return;
    }
    if (field_0x258 == 35 && field_0x274 != NULL && field_0x274->fn_80316E34() >= 0) {
        drawSprite_801F06F0("enemy_both2_sob.bin",
                            field_0x25c + field_0x274->fn_80316E34(),
                            NULL, 7, 1.0f, 1.0f);
        fn_803075AC(mSerial, field_0x4, &mPos,
                    field_0x25c + field_0x274->fn_80316E34(),
                    0, 2, -1, false, 0, 0);
        return;
    }
    if (field_0x260 == OLD_MAN_SOB_GC) {
        drawSprite_801F06F0("enemy_gc_sob.bin",
                            field_0x25c + lbl_80475498[field_0x264][field_0x248] + field_0x238,
                            NULL, -1, 1.0f, 1.0f);
    } else if (field_0x260 == OLD_MAN_SOB_BOTH) {
        drawSprite_801F06F0("enemy_both_sob.bin",
                            field_0x25c + lbl_80475498[field_0x264][field_0x248] + field_0x238,
                            NULL, 7, 1.0f, 1.0f);
        fn_80307AC8(mSerial, field_0x4, &mPos,
                    field_0x25c + lbl_80475498[field_0x264][field_0x248] + field_0x238,
                    0, 0, -1);
    } else {
        drawSprite_801F06F0("enemy_both2_sob.bin",
                            field_0x25c + lbl_80475498[field_0x264][field_0x248] + field_0x238,
                            NULL, 7, 1.0f, 1.0f);
        fn_803075AC(mSerial, field_0x4, &mPos,
                    field_0x25c + lbl_80475498[field_0x264][field_0x248] + field_0x238,
                    0, 2, -1, false, 0, 0);
    }
}

void cls_804AEED0::fn_8037B7EC() {
    field_0x238 = 0;
}

void cls_804AEED0::fn_8037B7BC() {
    if (field_0x234 > 0) {
        return;
    }
    field_0x234 = field_0x23c;
    field_0x238++;
    field_0x238 &= 1;
}

void cls_804AEED0::fn_8037B78C() {
    if (field_0x234 > 0) {
        return;
    }
    field_0x234 = field_0x23c;
    field_0x238++;
    field_0x238 &= 7;
}

void cls_804AEED0::fn_8037B744() {
    if (field_0x234 > 0) {
        return;
    }
    field_0x23c++;
    field_0x23c &= 3;
    field_0x234 = lbl_8053A1D0[field_0x23c];
    field_0x238 = lbl_8053A1D4[field_0x23c];
}

void cls_804AEED0::fn_8037B70C() {
    if (field_0x234 > 0) {
        return;
    }
    field_0x238++;
    field_0x238 &= 1;
    field_0x234 = lbl_8053A1D8[field_0x238];
}

void cls_804AEED0::fn_8037B6C4() {
    if (field_0x234 > 0) {
        return;
    }
    field_0x23c++;
    field_0x23c &= 3;
    field_0x234 = lbl_8053A1DC[field_0x23c];
    field_0x238 = lbl_8053A1E0[field_0x23c];
}

void cls_804AEED0::fn_8037B67C() {
    if (field_0x234 > 0) {
        return;
    }
    field_0x23c++;
    field_0x23c &= 3;
    field_0x234 = lbl_8053A1E4[field_0x23c];
    field_0x238 = lbl_8053A1E8[field_0x23c];
}

void cls_804AEED0::fn_8037B634() {
    if (field_0x234 > 0) {
        return;
    }
    field_0x23c++;
    field_0x23c &= 3;
    field_0x234 = lbl_8053A1EC[field_0x23c];
    field_0x238 = lbl_8053A1F0[field_0x23c];
}

void cls_804AEED0::fn_8037B5FC() {
    if (field_0x234 > 0) {
        return;
    }
    field_0x238++;
    field_0x238 &= 1;
    field_0x234 = lbl_8053A1F4[field_0x238];
}

void cls_804AEED0::fn_8037B5B4() {
    if (field_0x234 > 0) {
        return;
    }
    field_0x23c++;
    field_0x23c &= 3;
    field_0x234 = lbl_8053A1F8[field_0x23c];
    field_0x238 = lbl_8053A1FC[field_0x23c];
}

void cls_804AEED0::fn_8037B554() {
    if (field_0x234 > 0) {
        return;
    }
    field_0x23c++;
    field_0x23c &= 3;
    field_0x234 = lbl_8053A200[field_0x23c];
    field_0x238 = lbl_8053A204[field_0x248 / 2][field_0x23c];
}

void cls_804AEED0::fn_8037B4D0() {
    if (field_0x234 == 35) {
        field_0x238 = 1;
    } else if (field_0x234 == 20) {
        field_0x238 = 2;
    } else if (field_0x234 == 18) {
        field_0x238 = 0;
    } else if (field_0x234 == 17) {
        field_0x238 = 1;
    } else if (field_0x234 == 2) {
        field_0x238 = 2;
    } else if (field_0x234 <= 0) {
        field_0x238 = 0;
        field_0x234 = 55;
    }
}

void cls_804AEED0::fn_8037B488() {
    if (field_0x234 == 25) {
        field_0x238 = 1;
    } else if (field_0x234 == 10) {
        field_0x238 = 2;
    } else if (field_0x234 <= 0) {
        field_0x234 = 30;
        field_0x238 = 0;
    }
}

void cls_804AEED0::fn_8037B3E0() {
    if (field_0x248 < 3) {
        if (field_0x234 == 10) {
            field_0x238 = 1;
        } else if (field_0x234 <= 0) {
            field_0x234 = (s32)(50.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 40;
            field_0x238 = 0;
        }
    } else {
        field_0x234 = 0;
        field_0x238 = 0;
    }
}

void cls_804AEED0::fn_8037B3A8() {
    if (field_0x234 > 0) {
        return;
    }
    field_0x238++;
    field_0x238 &= 1;
    field_0x234 = lbl_8053A20C[field_0x238];
}

void cls_804AEED0::fn_8037B374(JGeometry::TVec3f pos, s32 arg) {
    mPos = pos;
    field_0x248 = arg / 3;
    field_0x273 = 1;
}

// updateFacing: dead-stripped in the original, init and update inline it.
// Its literals (100.0f, 10430.378f) lead the unit's .sdata2 pool ahead of
// fn_8037B3E0's, so the code using them sat in a plain function below
// fn_8037B374 (docs/RULES_CARD.md, literals before their first visible use (3)).
// The helper boundary is inferred from that pool order and from inlining into
// both init (0x8037C8F0) and update (0x8037C090, 0x8037C2B8); nothing else is
// proven.
void cls_804AEED0::updateFacing(s32 target) {
    if (field_0x24c != 0) {
        field_0x24c--;
        return;
    }
    field_0x24c = 20;
    JGeometry::TVec3f dir;
    f32 dist;
    fn_801EE558(&dir, &dist, target);
    if (dist < 100.0f) {
        u32 angle = (u16)(JMath::atanTable_.atan2_(-dir.x, -dir.y) *
                          (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180()));
        if (angle < 0x2000 || angle >= 0xE000) {
            field_0x248 = 3;
        } else if (angle >= 0x2000 && angle < 0x6000) {
            field_0x248 = 1;
        } else if (angle >= 0x6000 && angle <= 0xA000) {
            field_0x248 = 2;
        } else {
            field_0x248 = 0;
        }
    } else {
        field_0x248 = 2;
    }
}
