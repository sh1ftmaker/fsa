#include "game/GameAudio.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/act/act_vaati_boss.h"

static const s16 lbl_80469C00[] = {
    0xDB, 1, 0xDC, 1, 0xDD, 1, 0xDE, 1, 0xDF, 1, 0xE0, 1, 0xE1, 1,
    0xE2, 1, 0xE3, 1, 0xE4, 1, 0xE5, 1, 0xE6, 8, 0xE7, 1, 0xE8, 2,
    -1, -1,
};
static const s16* lbl_8053A158[2] = {lbl_80469C00, NULL};
// Read by act_horse_race_man and tu_80441318 (unwritten); owner here by its .sdata split.
s32 lbl_8053A160 = -1;

bool cls_801E6AB8::fn_801E667C() {
    s32 i; // i, cel declared in this order to match (97 % otherwise)
    s32 cel;
    bool done = false;
    mTimer++;
    if (mTimer < 0) {
        mTimer = 0;
    }
    switch (mState) {
    case 0:
        break;
    case 1:
        if (mTimer >= 400) {
            fn_801E67C4(2);
        }
        break;
    case 2:
        mAnim.fn_801D1C18();
        cel = mAnim.mCel;
        for (i = 0; i < 4; i++) {
            if (mActive[i]) {
                fn_80230498(i, fn_8023E724(i), cel, 0xFF, -0x80, -0x80, true);
            }
        }
        if (mAnim.fn_801D21F0()) {
            fn_801E67C4(3);
        }
        break;
    case 3:
        if (mTimer >= 130) {
            fn_801E67C4(4);
        }
        break;
    case 4:
        done = true;
        break;
    }
    return done;
}

void cls_801E6AB8::fn_801E67C4(s32 state) {
    switch (state) {
    case 1: {
        mActive[0] = 0;
        mActive[1] = 0;
        mActive[2] = 0;
        mActive[3] = 0;
        s32 count = lbl_8053AAF8->fn_80138A30();
        s32 j; // j before i to match (99.49 % otherwise)
        s32 i;
        for (i = 0; i < count; i++) {
            if (fn_8023F264(i) && !fn_8022F514(i)) {
                mActive[i] = 1;
            }
            for (j = i + 1; j < 4; j++) {
                if (fn_8023F264(j) && fn_8022F514(j) && fn_8022F3DC(j) == i && !fn_802349EC(j)) {
                    mActive[j] = 1;
                }
            }
        }
        for (s32 i = 0; i < 4; i++) {
            if (mActive[i]) {
                fn_80230498(i, fn_8023E724(i), 0x3E, 0xFF, -0x80, -0x80, true);
            }
        }
        lbl_8053AF80->fn_80458F9C(0);
        lbl_8053AF80->fn_80458F04(0x1000001);
        break;
    }
    case 2:
        mAnim.startAnim_801D22DC(0, 0, false);
        for (s32 i = 0; i < 4; i++) {
            if (mActive[i]) {
                fn_80230FD0(i, 0x10001);
            }
        }
        break;
    case 3:
        for (s32 i = 0; i < 4; i++) {
            if (mActive[i]) {
                JGeometry::TVec3f pos;
                pos.set(*fn_8023E724(i));
                pos.x += 7.0f;
                pos.y += -27.0f;
                spawnEmitter_8013CC50(fn_8023DE58(i), &pos, 0xCB, 1, 0, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(fn_8023DE58(i), &pos, 0xCC, 1, 0, NULL, NULL, true, 1.0f);
                fn_80230FD0(i, 0x2000E);
            }
        }
        break;
    case 4:
        if (field_0x8 == 0) {
            getGameView_802D6A9C()->fn_802D9FF4(9, 0);
            lbl_8053AAF8->fn_80136798(13, 0);
        }
        break;
    }
    mTimer = 0;
    mState = state;
}

void cls_801E6AB8::fn_801E6A84(s32 arg) {
    if (mState == 0) {
        field_0x8 = arg;
        fn_801E67C4(1);
    }
}

cls_801E6AB8::cls_801E6AB8() : mState(0), mTimer(0), field_0x8(1) {
    mActive[0] = 0;
    mActive[1] = 0;
    mActive[2] = 0;
    mActive[3] = 0;
    mAnim.fn_801D2608(lbl_8053A158, 1);
}
