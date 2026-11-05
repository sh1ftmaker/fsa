#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Effect.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/cls_804AE9B0.h"
#include "game/act/act_warp_cannon.h"

// The animation: (cel, duration) pairs, ended by a cel of -1.
static const s16 lbl_80465120[] = {0, 3, 1, 2, 2, 2, 3, 3, 4, 3, 5, 3, -1, -1};
static const s16* lbl_80539F48[] = {lbl_80465120, NULL};

// FIXME: 95.15 %. The loop over fn_80230360 is the same, but after it the
// original falls through into the field_0x278 store on its own `cmpwi 4; blt`,
// where ours compares i again (cmpwi; bne): 3 extra instructions. The switch's
// jump table is the compiler's (@NNN) where the original names
// jumptable_8049EB34.
void WarpCannon_8049EAE0::fn_80130FAC(s32 state) {
    field_0x268 = state;
    switch (field_0x268) {
    case 1:
        fn_80230FD0(field_0x26c, 0x20035);
        break;
    case 2:
        field_0x27c = 13.0f;
        fn_80230FD0(0, 0x20036);
        lbl_8053AAF8->fn_80134DE4(1, 0);
        lbl_8053AD70->fn_8037AE00();
        break;
    case 3: {
        field_0x290 = *fn_8023E32C(0);
        s32 i;
        for (i = 0; i < 4; i++) {
            if (!fn_80230360(i)) {
                break;
            }
        }
        if (i == 4) {
            field_0x278 = 0;
        }
        for (i = 0; i < 4; i++) {
            fn_80230498(i, &field_0x290, 0xB0, 0, -0x80, -0x80, true);
        }
        break;
    }
    case 4:
        for (s32 i = 0; i < 4; i++) {
            u16 id = fn_8023CBC0(i);
            fn_80230498(i, fn_8023E724(i), id, 0xFF, -0x80, -0x80, true);
        }
        for (s32 i = 0; i < 4; i++) {
            fn_80230FD0(i, 0x10003);
        }
        field_0x27c = 4.0f;
        gGameRoot2_8053AB10->field_0x20->fn_802F601C(&field_0x290, 1, NULL, 1.0f, -2.0f);
        spawnEmitter_8013CC50(0, &field_0x290, 0x17D, 1, 0, NULL, NULL, true, 1.0f);
        break;
    case 6:
        fn_80230534(false);
        getGameView_802D6A9C()->fn_802D9FF4(field_0x270, field_0x274);
        lbl_8053AAF8->fn_80136798(0xD, 0);
        lbl_8053AD70->fn_8037AE30();
        break;
    case 7: {
        fn_8023077C();
        field_0x278 = 0;
        for (s32 i = 0; i < 4; i++) {
            JGeometry::TVec3f pos = *fn_8023E724(i);
            pos.z = 90.0f;
            fn_80230498(i, &pos, 0xB0, 0xFF, -0x80, -0x80, true);
        }
        field_0x27c = 0.0f;
        break;
    }
    case 8:
        fn_80230534(false);
        mAlive = false;
        break;
    }
}

// FIXME: 99.90 %. Only the .sdata2 offsets of its literals differ: the
// original's pool (0x8053CCA0) also holds update()'s, which isn't written yet.
void WarpCannon_8049EAE0::draw() {
    SpriteParams_801F5AE0 params;
    params.field_0x1d = 1;
    Mtx mtx;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    mtx[0][0] = 2.0f;
    mtx[0][1] = 0.0f;
    mtx[0][2] = 0.0f;
    mtx[1][0] = 0.0f;
    mtx[1][1] = 2.0f;
    mtx[1][2] = 0.0f;
    mtx[2][0] = 0.0f;
    mtx[2][1] = 0.0f;
    mtx[2][2] = 2.0f;
    params.mMtx = mtx;
    fn_801F02BC("enemy_both2_sob.bin", mAnim.mCel + 0x14C, &params, 0x407, 1, false,
                1.0f, 1.0f);
}

void WarpCannon_8049EAE0::init() {
    field_0x274 = field_0x90 >> 8 & 0xFF;
    field_0x270 = field_0x90 >> 16 & 0xFF;
    mBox.set(-16.0f, -16.0f, 16.0f, -12.0f);
    mBox2.set(mBox);
    field_0x80.set(0.0f, 0.0f, 0.0f, 0.0f);
    mLife = 0;
    field_0xb0 = 0x50303;
    mAnim.fn_801D2608(lbl_80539F48, 1);
    mAnim.startAnim_801D22DC(0, 2, false);
    mAnim.fn_801D1F14(0.0f);
    field_0x284 = mPos.x;
    field_0x288 = mPos.y;
    field_0x28c = mPos.z;
}

WarpCannon_8049EAE0::~WarpCannon_8049EAE0() {}

WarpCannon_8049EAE0::WarpCannon_8049EAE0() {
    field_0x268 = 0;
    field_0x26c = -1;
    field_0x270 = 0;
    field_0x274 = 0;
    field_0x278 = 0;
    field_0x27c = 0.0f;
    field_0x280 = 0.0f;
    field_0x284 = 0.0f;
    field_0x288 = 0.0f;
    field_0x28c = 0.0f;
    field_0x290.set(0.0f, 0.0f, 0.0f);
}
