#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/act/act_force_gem_winged.h"
#include "JSystem/JMath/JMath.h"
#include "game/Player.h"
#include "dolphin/mtx.h"

// Sprite frames by kind (field_0x270, 0-7); 8-15 are the frames for states 5 and 6.
s32 lbl_804AB038[16] = {
    0x11, 0x15, 0x19, 0x1D, 0x11, 0x09, 0x0D, 0x05,
    0x11, 0x15, 0x19, 0x1D, 0x11, 0x09, 0x0D, 0x05,
};

// FIXME(shotgun): 95.66 %, missing/extra + reloc-name (95.73 % before draw; literal order waits for update)
void WingedForceGem_804AB098::init() {
    field_0x94 = (field_0x90 >> 16) & 0xFF;
    field_0x270 = field_0x90 & 7;
    field_0x274 = (field_0x90 >> 12) & 1;
    field_0x284 = (field_0x90 >> 14) & 1;
    field_0x286 = (field_0x90 >> 10) & 1;
    field_0x282 = (field_0x90 >> 25) & 1;
    field_0x283 = (field_0x90 >> 26) & 1;
    field_0x25c = 0;
    field_0x260 = 0;
    field_0x264 = 0;
    field_0x268 = 0;
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x254 = 0;
    field_0x258 = 0;
    field_0x288 = 0;
    mLife = 10000;
    field_0x26c = -1;
    field_0x98 = 0;
    field_0x298.x = mPos.x;
    field_0x298.y = mPos.y;
    field_0x298.z = mPos.z;
    field_0x2a4.zero();
    field_0x268 = 100;
    field_0x278 = 0;
    field_0x23c = 1.0f;
    field_0x240 = 0.0f;
    field_0x244 = 0.0f;
    field_0x248 = 0.0f;
    field_0x280 = 0;
    field_0x281 = (field_0x90 >> 24) & 1;
    field_0x285 = (field_0x90 >> 8) & 1;
    field_0x238 = 30.0f;
    field_0x2b0.zero();
    field_0x27c = (s32)(65536.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
    field_0x230 = 0;
    field_0x234 = field_0x230;
    field_0x264 = (s32)(65535.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
    mPos.z = 24.0f;
    if (field_0x274 != 0) {
        mPos.z = 8.0f;
        field_0xb0 = 0x20000;
    }
    if (getGameView_802D6A9C()->field_0x131df[field_0x4]) {
        mPos.y -= mPos.z;
        mPos.z = 0.0f;
        field_0xb0 = 0x20000;
    }
    switch (field_0x94) {
    case 0:
        field_0x17c = 0;
        field_0x174 = 50;
        break;
    case 1:
    case 6:
        field_0x98 = 2;
        field_0x98 |= (s32)(1.99f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
        break;
    case 2:
    case 7:
        field_0x98 |= (s32)(1.99f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
        break;
    case 3:
        field_0x268 = -100;
    case 4:
        mPos.x = field_0x298.x + field_0x238 * JMath::sincosTable_.sinShort(field_0x264);
        mPos.y = field_0x298.y + field_0x238 * JMath::sincosTable_.cosShort(field_0x264);
        break;
    case 5:
        field_0x180 = 60;
        break;
    case 8:
        field_0x230 = 3;
        mPos.z = 0.0f;
        break;
    }
    field_0x28c.x = 0.0f;
    field_0x28c.y = 0.0f;
    field_0x28c.z = 0.0f;
    field_0x3c.x = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.z = 0.0f;
    field_0x80.set(-7.0f, -4.0f, 7.0f, 4.0f);
    if (field_0x94 == 5 || field_0x274 != 0) {
        mBox.set(-7.0f, -23.0f, 7.0f, 7.0f);
    } else {
        mBox.set(-8.0f, -32.0f, 8.0f, 16.0f);
    }
    if (gGameRoot_8053AAF8->field_0x7f == 0) {
        fn_801F3668();
    } else if (field_0x283 == 1 && gGameRoot_8053AAF8->field_0x7e == 0) {
        fn_801F3668();
    } else if (field_0x285 == 1) {
        field_0x234 = field_0x230;
        field_0x230 = 1;
    }
}

// FIXME(shotgun): 92.78 %, different + reloc-name
void WingedForceGem_804AB098::draw() {
    if (field_0x281 == 1 || field_0x230 == 1 || field_0x230 == 2) {
        return;
    }
    SpriteParams_801F5AE0 params;
    s32 kind;
    if (field_0x270 == 4) {
        kind = (field_0x278 < 3) ? 0 : 3;
    } else {
        kind = field_0x270;
    }
    params.field_0x20 = 0;
    params.field_0x2d = 1;
    if (field_0x284 == 0) {
        params.field_0x1c = 1;
        params.field_0x1d = 1;
    } else {
        params.field_0x1c = 0;
        params.field_0x1d = 3;
    }
    if (field_0x230 == 5 || field_0x230 == 6) {
        params.field_0x2d = 1;
        params.field_0x1f = 0;
        if (field_0x26c >= 0 && field_0x26c <= 3) {
            params.field_0x1d = fn_8023CB24(field_0x26c) + 1;
            params.field_0x1c = fn_8023CA88(field_0x26c);
        }
        drawSprite_801F06F0("enemy_both2_sob.bin", field_0x25c + lbl_804AB038[kind + 8], &params, -1, 1.0f,
                            1.0f);
        fn_803075AC(mSerial, field_0x4, &mPos, field_0x25c + lbl_804AB038[kind + 8], 0, 2, -1, true, 0, 0);
        return;
    }
    JGeometry::TVec3f saved(mPos);
    mPos.add(field_0x2b0);
    mPos.z += field_0x248;
    f32 scale = 1.0f + (0.3f * field_0x248) / 20.0f;
    Mtx mtx;
    PSMTXIdentity(mtx);
    mtx[0][0] = scale;
    mtx[0][1] = 0.0f;
    mtx[0][2] = 0.0f;
    mtx[1][0] = 0.0f;
    mtx[1][1] = scale;
    mtx[1][2] = 0.0f;
    mtx[2][0] = 0.0f;
    mtx[2][1] = 0.0f;
    mtx[2][2] = scale;
    params.mMtx = mtx;
    if (field_0x274 == 1) {
        drawSprite_801F06F0("enemy_both2_sob.bin", field_0x25c + lbl_804AB038[kind], &params, -1, 1.0f, 1.0f);
        params.field_0x1d = -1;
        params.field_0x1c = 0;
        if (mSpriteParams.mMtx == NULL) {
            fn_801EF830(field_0x4, &mPos, &params, 7, "enemy_both2_sob.bin", field_0x25c + lbl_804AB038[kind],
                        NULL);
        }
    } else {
        drawSprite_801F06F0("enemy_both2_sob.bin", field_0x25c + lbl_804AB038[kind], &params, 7, 1.0f, 1.0f);
    }
    fn_803075AC(mSerial, field_0x4, &mPos, field_0x25c + lbl_804AB038[kind], 0, 2, -1, false, 0, 0);
    mPos = saved;
}

WingedForceGem_804AB098::~WingedForceGem_804AB098() {}
