#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Message.h"
#include "game/Player.h"
#include "JSystem/JMath/JMath.h"
#include "game/act/act_princess_zelda_motion_spawner.h"

// FIXME: 99.8 %. Code identical; only the float literal relocations differ
// (this unit's .sdata2 literals are not split to match yet).
void PrincessZeldaMotionObjectSpawner_804A375C::init() {
    mBox.i.x = -8.0f;
    mBox.i.y = -8.0f;
    mBox.f.x = 8.0f;
    mBox.f.y = 8.0f;
    mBox2.i.x = -8.0f;
    mBox2.i.y = -8.0f;
    mBox2.f.x = 8.0f;
    mBox2.f.y = 8.0f;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    mKind = field_0x90 & 0xFF;
    field_0x254 = (field_0x90 >> 22) & 0x1F;
    field_0x258 = (field_0x90 >> 8) & 0xF;
    field_0x24c = 0;
    mHomePos.x = mPos.x;
    mHomePos.y = mPos.y;
    mHomePos.z = mPos.z;
    field_0x3c.z = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.x = 0.0f;
    field_0x25c = 0;
    field_0x94 = 0;
    field_0x1a8 = false;
    switch (mKind) {
    case 0:
        mState = 0;
        break;
    case 2:
        mState = 1;
        break;
    case 1:
        mState = 2;
        break;
    case 3:
    case 7:
        mState = 4;
        break;
    default:
        mState = 10;
        break;
    }
}

// Message ids by the spawner's bits 8-11 (field_0x258).
static const u32 lbl_804A3740[] = {0x2D6, 0x2F0, 0x2F1, 0x2F3, 0x2F7, 0x2F8, 0x2F9};

// FIXME(shotgun): 46.46 %, missing/extra. States 6-8 and the camera setup at the start of state 9 are not written
void PrincessZeldaMotionObjectSpawner_804A375C::update() {
    f32 dist = 0.0f;
    s32 dir = -1;
    switch (mState) {
    case 0:
        if (field_0x25c != 0) {
            if (vfunc_0x34()) {
                if (field_0x254 != 0) {
                    getGameView_802D6A9C()->fn_802D8050(field_0x254, true, 0xFF);
                }
                field_0x94 = 1;
            } else {
                field_0x94 = 0;
                JGeometry::TBox2f box = mBox2;
                box.addPos(mHomePos.x, mHomePos.y);
                fn_8022FEB8(&box, field_0x4, mSerial, false, false);
                field_0x248 = fn_8022FD8C(mSerial);
                if (field_0x248 >= 0 && fn_802300EC(field_0x248)) {
                    JGeometry::TVec3f pos = mPos;
                    pos.x -= 50.0f;
                    if (fn_8038AD24(field_0x248, lbl_804A3740[field_0x258], mSerial, field_0x4, pos, 0)) {
                        mState = 3;
                    }
                }
            }
        }
        break;
    case 1:
        if (field_0x25c != 0) {
            vfunc_0x30(0xFF, true);
            mState = 10;
        }
        break;
    case 2:
        if (field_0x25c != 0) {
            if (!vfunc_0x34()) {
                if (field_0x254 != 0) {
                    getGameView_802D6A9C()->fn_802D8050(field_0x254, true, 0xFF);
                }
                field_0x94 = 1;
            } else {
                field_0x94 = 0;
            }
        }
        break;
    case 3:
        fn_8022FE80(field_0x248, field_0x4, mSerial, true, false);
        if (fn_803865B8(field_0x248)) {
            if (fn_80384C58(field_0x248, mSerial)) {
                mState = 0;
            }
        }
        break;
    case 4: {
        if (field_0x25c == 0) {
            break;
        }
        fn_801EEC98(mSerial, NULL, &dir, &dist, NULL, 0, 0);
        if (mKind == 3) {
            if (dir < 0 || dist > 32.0f) {
                break;
            }
        } else {
            s32 count = 0;
            for (s32 i = 0; i < 4; i++) {
                if (fn_8023F264(i) && i == fn_8022EA0C(i) && field_0x4 == getPlayerLayer_8023DE58(i) &&
                    !fn_80230188(i) && !fn_8022DCD8(i) && !fn_80236B50(i) && !fn_8022D534(i)) {
                    f32 dx = getPlayerPos_8023E724(i)->x - mPos.x;
                    f32 dy = getPlayerPos_8023E724(i)->y - mPos.y;
                    JGeometry::TVec2f d(dx, dy);
                    if (d.length() < 64.0f) {
                        count++;
                    }
                }
            }
            if (count < gGameRoot_8053AAF8->fn_80138A30() || dir < 0 || dist > 32.0f) {
                break;
            }
        }
        if (field_0x258 == 0) {
            if (field_0x254 != 0) {
                getGameView_802D6A9C()->fn_802D8050(field_0x254, true, 0xFF);
            }
            mAlive = false;
            break;
        }
        mState = 5;
    }
        // fall through
    case 5:
        if (getActorManager_801F856C()->fn_801F68E0() != 0) {
            for (s32 i = 0; i < 4; i++) {
                fn_80230C6C(i);
            }
            mState = 6;
        }
        // FIXME(shotgun): states 6-8 not written (state 5 falls through into state 6 in the asm)
        break;
    case 9:
        // FIXME(shotgun): camera/focus setup before the timer check not written (asm 0x801B486C-0x801B491C)
        if (field_0x234 == 0) {
            for (s32 i = 0; i < 4; i++) {
                if (fn_8023F264(i) && field_0x4 == getPlayerLayer_8023DE58(i)) {
                    fn_80236554(i, 0x20, false);
                }
            }
            getGameView_802D6A9C()->fn_802D9FF4(0, 9);
            getGameView_802D6A9C()->fn_802DBD60();
        }
        break;
    }
    if (field_0x234 > 0) {
        field_0x234--;
    }
    if (field_0x238 > 0) {
        field_0x238--;
    }
    if (field_0x23c > 0) {
        field_0x23c--;
    }
}

void PrincessZeldaMotionObjectSpawner_804A375C::draw() {}

PrincessZeldaMotionObjectSpawner_804A375C::~PrincessZeldaMotionObjectSpawner_804A375C() {}
