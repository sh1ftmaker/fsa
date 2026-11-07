#include "game/GameAudio.h"
#include "game/act/tu_8028DD78.h"
#include "game/Player.h"
#include "game/GameGlobals.h"

// FIXME(shotgun): update (0x8028E1EC) is a skeleton under NONMATCHING below

// FIXME(shotgun): 98.6 %, reloc-name
void cls_804A7A68::init() {
    switch (field_0x90 & 0xF) {
    case 0:
    case 1:
    case 4:
        mBox.i.x = -40.0f;
        mBox.i.y = -10.0f;
        mBox.f.x = 40.0f;
        mBox.f.y = 8.0f;
        break;
    case 5:
        mBox.i.x = -40.0f;
        mBox.i.y = -8.0f;
        mBox.f.x = 40.0f;
        mBox.f.y = 10.0f;
        break;
    case 6:
        mBox.i.x = -8.0f;
        mBox.i.y = -40.0f;
        mBox.f.x = 10.0f;
        mBox.f.y = 40.0f;
        break;
    case 7:
        mBox.i.x = -10.0f;
        mBox.i.y = -40.0f;
        mBox.f.x = 8.0f;
        mBox.f.y = 40.0f;
        break;
    }
    mLife = 0;
    field_0x230 = 0;
    field_0x234 = 0;
    field_0x268 = 0xFF;
}

#ifdef NONMATCHING
// 0x8028E1EC.
// FIXME(shotgun): not compiled, nothing written. The 0x16A4-byte sequencer switches on
// field_0x230 (jump table); its per-state conditions were not read in full.
void cls_804A7A68::update() {
    // FIXME(shotgun): states 0-4 not written
}
#endif

void cls_804A7A68::draw() {}

// 0x8028DDD8. Orders the players for the four marker slots: out[] gets the player indices
// sorted by their x (y for the vertical variants 6 and 7), grouped by team for two players.
// FIXME(shotgun): 92.25 %, missing/extra
void cls_804A7A68::fn_8028DDD8(s32* out) {
    f32 key[4];
    for (s32 i = 0; i < 4; i++) {
        if ((field_0x90 & 0xF) == 6 || (field_0x90 & 0xF) == 7) {
            key[i] = getPlayerPos_8023E724(i)->y;
        } else {
            key[i] = getPlayerPos_8023E724(i)->x;
        }
    }
    switch (gGameRoot_8053AAF8->fn_80138A30()) {
    case 2: {
        s32 n = 1;
        if (key[0] <= key[1]) {
            out[0] = 0;
            if (fn_8022E970(0) > 1) {
                for (s32 i = 0; i < 4; i++) {
                    if (fn_8022F514(i) && fn_8022F340(i) == 0) {
                        out[n] = i;
                        n++;
                    }
                }
            }
            out[n] = 1;
            n++;
            if (fn_8022E970(1) > 1) {
                for (s32 i = 0; i < 4; i++) {
                    if (fn_8022F514(i) && fn_8022F340(i) == 1) {
                        out[n] = i;
                        n++;
                    }
                }
            }
        } else {
            out[0] = 1;
            if (fn_8022E970(1) > 1) {
                for (s32 i = 0; i < 4; i++) {
                    if (fn_8022F514(i) && fn_8022F340(i) == 1) {
                        out[n] = i;
                        n++;
                    }
                }
            }
            out[n] = 0;
            n++;
            if (fn_8022E970(0) > 1) {
                for (s32 i = 0; i < 4; i++) {
                    if (fn_8022F514(i) && fn_8022F340(i) == 0) {
                        out[n] = i;
                        n++;
                    }
                }
            }
        }
        break;
    }
    case 3: {
        out[0] = 0;
        out[1] = 1;
        out[2] = 2;
        out[3] = 3;
        for (s32 i = 0; i < 2; i++) {
            s32 a = out[i];
            s32 b = out[i + 1];
            if (key[a] > key[b]) {
                out[i] = b;
                out[i + 1] = a;
                i = -1;
            }
        }
        s32 leader = fn_8022F3DC(3);
        for (s32 i = 0; i < 3; i++) {
            if (leader == out[i]) {
                if (i == 2) {
                    out[3] = 3;
                } else {
                    for (s32 m = 3; m > i + 1; m--) {
                        out[m] = out[m - 1];
                    }
                    out[i + 1] = 3;
                }
            }
        }
        break;
    }
    case 4:
        out[0] = 0;
        out[1] = 1;
        out[2] = 2;
        out[3] = 3;
        for (s32 i = 0; i < 3; i++) {
            s32 a = out[i];
            s32 b = out[i + 1];
            if (key[a] > key[b]) {
                out[i] = b;
                out[i + 1] = a;
                i = -1;
            }
        }
        break;
    default:
        out[0] = 0;
        out[1] = 1;
        out[2] = 2;
        out[3] = 3;
        break;
    }
}

cls_804A7A68::~cls_804A7A68() {}
