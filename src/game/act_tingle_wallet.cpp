#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/act/act_tingle_wallet.h"

void TinglesWallet_804A3948::init() {
    mBox.i.x = -8.0f;
    mBox.i.y = -8.0f;
    mBox.f.x = 8.0f;
    mBox.f.y = 8.0f;
    field_0x80.i.x = -8.0f;
    field_0x80.i.y = -8.0f;
    field_0x80.f.x = 8.0f;
    field_0x80.f.y = 8.0f;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    mTimers[0] = 0;
    mTimers[1] = 0;
    mTimers[2] = 0;
    field_0x258 = 1.0f;
    field_0x25c = 1.0f;
    field_0x260 = 1.0f;
    field_0x264 = 1.0f;
    field_0x268 = 1.0f;
    field_0x26c = 1.0f;
    field_0x254 = 0.0f;
    field_0x250 = 0.0f;
    field_0x24c = 0.0f;
    field_0x230 = 0;
    field_0x11f = 1;
}

void TinglesWallet_804A3948::update() {
    addCalc2_801F71A4(&field_0x3c.x, field_0x24c, 0.3f, 1.0f);
    addCalc2_801F71A4(&field_0x3c.y, field_0x250, 0.3f, 1.0f);
    addCalc2_801F71A4(&field_0x258, field_0x264, 0.2f, 0.1f);
    addCalc2_801F71A4(&field_0x25c, field_0x268, 0.2f, 0.1f);
    mPos += field_0x3c;
    switch (field_0x230) {
    case 0:
        if (field_0x94 != 0) {
            field_0x94 = 0;
            field_0x3c.z = -3.0f;
            field_0x230 = 1;
        }
        break;
    case 1:
        if (mPos.z < 0.0f) {
            field_0x258 = 1.2f;
            field_0x25c = 0.8f;
            field_0x260 = 1.0f;
            mPos.z = 0.0f;
            field_0x3c.z = 0.0f;
            field_0x230 = 2;
        }
        break;
    case 2:
        break;
    }
    for (s32 i = 0; i < 3; i++) {
        if (mTimers[i] > 0) {
            mTimers[i]--;
        }
    }
}

// FIXME: 92%. Only scheduling differs: the target loads the 1.0f argument (lfs f1) and
// stores params.field_0x10 earlier, before the string address and the mMtx pointer.
// (The string is also a literal here, lbl_80469120 in the target: a data fix.)
void TinglesWallet_804A3948::draw() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    mtx[0][0] = field_0x258;
    mtx[0][1] = 0.0f;
    mtx[0][2] = 0.0f;
    mtx[1][0] = 0.0f;
    mtx[1][1] = field_0x25c;
    mtx[1][2] = 0.0f;
    mtx[2][0] = 0.0f;
    mtx[2][1] = 0.0f;
    mtx[2][2] = field_0x25c;
    params.mMtx = mtx;
    params.field_0x1c = 1;
    params.field_0x1d = 1;
    params.field_0x20 = 0;
    params.field_0x2d = 1;
    drawSprite_801F06F0("enemy_both2_sob.bin", 0xE6, &params, 0, 1.0f, 1.0f);
    fn_803075AC(mSerial, field_0x4, &mPos, 0xE6, false, 2, -1, 0, 0, 1);
}

TinglesWallet_804A3948::~TinglesWallet_804A3948() {}
