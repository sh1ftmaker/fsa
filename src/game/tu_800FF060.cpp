#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
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

HammerShopMinigame_8049C858::~HammerShopMinigame_8049C858() {}
