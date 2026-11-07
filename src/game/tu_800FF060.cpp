#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/tu_804465B8.h"
#include "game/act/tu_800FF060.h"

// FIXME(shotgun): 99.8 %, reloc-name
void HammerShopMinigame_8049C858::init() {
    mBox.i.x = -8.0f;
    mBox.i.y = -8.0f;
    mBox.f.x = 8.0f;
    mBox.f.y = 8.0f;
    field_0x235 = 2;
    field_0x23c = 0;
    field_0x234 = 0;
    field_0x240 = -1;
    field_0x236 = 0;
    field_0x239 = (field_0x90 >> 22) & 0x1F;
    field_0x230 = 0;
    field_0x248 = 0;
    field_0x260 = 600;
    if (vfunc_0x34()) {
        field_0x230 = 8;
        field_0x248 = 0;
    } else {
        field_0x230 = 0;
        field_0x248 = 0;
    }
}

// FIXME(shotgun): 91.3 %, missing/extra
void HammerShopMinigame_8049C858::fn_801003F4() {
    s32 rank = 0;
    u8 used[4] = {0, 0, 0, 0};
    s32 count = gGameRoot_8053AAF8->fn_80138A30();
    for (s32 i = 0; i < count; i++) {
        if (field_0x250[i] == 0) {
            getGameView_802D6A9C()->fn_802D7844(1, i);
        } else {
            getGameView_802D6A9C()->fn_802D7844(0, i);
        }
    }
    for (rank = 0; rank != count; rank++) {
        s32 best = -1;
        s32 bestScore = 1000000;
        for (s32 i = 0; i < count; i++) {
            if (bestScore > field_0x250[i] && used[i] == 0) {
                bestScore = field_0x250[i];
                best = i;
            }
        }
        field_0x264[rank] = best;
        used[best] = 1;
    }
    field_0x274 = 0;
    if (field_0x250[field_0x264[0]] == field_0x250[field_0x264[1]]) {
        field_0x278 = 0;
    } else {
        field_0x278 = 1;
    }
    if (count != 2) {
        if (field_0x250[field_0x264[1]] == field_0x250[field_0x264[2]]) {
            field_0x27c = field_0x278;
        } else {
            field_0x27c = 2;
        }
        if (count != 3) {
            if (field_0x250[field_0x264[2]] == field_0x250[field_0x264[3]]) {
                field_0x280 = field_0x27c;
            } else {
                field_0x280 = 3;
            }
        }
    }
}

// FIXME(shotgun): 55.4 %, missing/extra (54 more instrs; loop unroll differs)
void HammerShopMinigame_8049C858::draw() {
    if (field_0x230 == 4 || field_0x230 == 5 || field_0x230 == 6 || field_0x230 == 7) {
        s32 count = gGameRoot_8053AAF8->fn_80138A30();
        for (s32 i = 0; i < count; i++) {
            lbl_8053AF00->field_0x30 |= (u8)(1 << i);
            lbl_8053AF00->field_0x2c[i] = field_0x250[i] / 60;
            lbl_8053AF00->field_0x28[i] = (f32)(field_0x250[i] % 60) / 0.6f;
        }
        lbl_8053AF00->field_0x30 |= (u8)(1 << (field_0x240 + 4));
        if (field_0x230 == 4) {
            JGeometry::TVec3f savedPos = mPos;
            JGeometry::TVec3f* pos = getPlayerPos_8023E724(field_0x240);
            mPos.x = pos->x;
            mPos.y = pos->y;
            mPos.z = pos->z;
            mPos.y -= 20.0f;
            drawSprite_801F06F0("enemy_both2_sob.bin", 0x314, NULL, -1, 1.0f, 1.0f);
            if (getPlayerLayer_8023DE58(field_0x240) != 0) {
                fn_803075AC(mSerial, getPlayerLayer_8023DE58(field_0x240), &mPos, 0x314, 0, 2, -1, false, 0, 0);
            }
            mPos.x = savedPos.x;
            mPos.y = savedPos.y;
            mPos.z = savedPos.z;
        }
    }
    drawSprite_801F06F0("enemy_gc_sob.bin", field_0x235 * 2 + field_0x23c + 0x311, NULL, 0, 1.0f, 1.0f);
}

HammerShopMinigame_8049C858::~HammerShopMinigame_8049C858() {}
