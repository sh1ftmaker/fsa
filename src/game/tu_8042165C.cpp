#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/Player.h"
#include "game/cls_804B146C.h"
#include "game/Message.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/J2D/J2DGrafContext.h"

// FIXME(shotgun): 99.95 %, reloc-name
cls_804B146C::~cls_804B146C() {
    delete field_0x4c;
    field_0x4c = NULL;
    delete field_0xf0;
    field_0xf0 = NULL;
    delete field_0x4;
    field_0x4 = NULL;
    for (s32 i = 0; i < 4; i++) {
        delete field_0x8[i];
        field_0x8[i] = NULL;
        delete field_0x2c[i];
        field_0x2c[i] = NULL;
    }
    delete mScreen;
    mScreen = NULL;
    for (s32 i = 0; i < 4; i++) {
        delete field_0x50[i];
        field_0x50[i] = NULL;
        delete field_0x60[i];
        field_0x60[i] = NULL;
        delete field_0x70[i];
        field_0x70[i] = NULL;
        delete field_0x80[i];
        field_0x80[i] = NULL;
        delete field_0x90[i];
        field_0x90[i] = NULL;
        delete field_0xa0[i];
        field_0xa0[i] = NULL;
        delete field_0xb0[i];
        field_0xb0[i] = NULL;
        delete field_0xc0[i];
        field_0xc0[i] = NULL;
        delete field_0xd0[i];
        field_0xd0[i] = NULL;
        delete field_0xe0[i];
        field_0xe0[i] = NULL;
    }
}

void cls_804B146C::fn_80425F24() {
    gGameRoot2_8053AB10->fn_8013B490();
    mScreen->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
}

// FIXME(shotgun): 86.63 %, missing/extra (counter pointer and register order)
void cls_804B146C::fn_804253E0(s32 player) {
    s32* counter = &lbl_80529DEC.field_0xd8[player];
    s32 shown = *counter;
    if (shown < fn_80237774(player)) {
        field_0x168[player]++;
        if (field_0x168[player] >= 10) {
            *counter += 1;
            for (s32 j = 0; j < 4; j++) {
                if (j != player && fn_8022F514(j) && player == fn_8022F3DC(j)) {
                    lbl_80529DEC.field_0xd8[j] = *counter;
                }
            }
            field_0x256 |= (u8)(1 << player);
            field_0x168[player] = 0;
            fn_80230EB8(player, 9);
            field_0x1e8[player] = lbl_8050DB0C.field_0x4c[(0x74 - 0x4C) / 2] * lbl_8050DB0C.field_0x4c[(0x76 - 0x4C) / 2] + 30;
            field_0x1b8[player] = field_0x1a8[player];
            field_0x1a8[player] = *counter;
            field_0x1c8[player] = fn_80237810(player);
        }
    } else if (*counter > fn_80237774(player)) {
        lbl_80529DEC.field_0x24a |= (u8)(1 << player);
        field_0x168[player]++;
        if (field_0x168[player] >= 10) {
            *counter -= 1;
            for (s32 j = 0; j < 4; j++) {
                if (j != player && fn_8022F514(j) && player == fn_8022F3DC(j)) {
                    lbl_80529DEC.field_0xd8[j] = *counter;
                }
            }
            field_0x256 |= (u8)(1 << player);
            field_0x168[player] = 0;
        }
        field_0x1e8[player] = lbl_8050DB0C.field_0x4c[(0x74 - 0x4C) / 2] * lbl_8050DB0C.field_0x4c[(0x76 - 0x4C) / 2] + 30;
        field_0x1b8[player] = field_0x1a8[player];
        field_0x1a8[player] = *counter;
        field_0x1c8[player] = fn_80237810(player);
    } else {
        field_0x168[player] = 0;
    }
    s32* life = &lbl_80529DEC.field_0xe8[player];
    if (*life < fn_80236100(player)) {
        field_0x178[player]++;
        if (field_0x178[player] >= 2) {
            *life += 2;
            if (*life >= fn_80236100(player)) {
                *life = fn_80236100(player);
                fn_80230EB8(player, 0x51);
            } else {
                fn_80230EB8(player, 4);
            }
            field_0x257 |= (u8)(1 << player);
            field_0x178[player] = 0;
        }
    } else if (*life > fn_80236100(player)) {
        field_0x178[player]++;
        if (field_0x178[player] >= 2) {
            *life -= 2;
            if (*life < fn_80236100(player)) {
                *life = fn_80236100(player);
                fn_80230EB8(player, 0x51);
            } else {
                fn_80230EB8(player, 0x4E);
            }
            field_0x257 |= (u8)(1 << player);
            field_0x178[player] = 0;
        }
    } else {
        field_0x178[player] = 0;
    }
    if (field_0x1f8[player] > 0) {
        if (field_0x200[player] == 0) {
            field_0x1f8[player]--;
            fn_80230FD0(player, 0x20034);
            if (field_0x1f8[player] != 0) {
                field_0x200[player] = 12;
            }
        } else {
            field_0x200[player]--;
        }
    }
}

// FIXME(shotgun): 99.58 %, regalloc-only
void cls_804B146C::fn_80425234(s32 id, s32 arg1, u32 arg2) {
    s32 target;
    if (fn_8022F514(id)) {
        target = fn_8022F3DC(id);
    } else {
        target = id;
    }
    if ((u8)arg2) {
        s32 sum = arg1 + fn_80236100(target);
        if (sum > 9999) {
            arg1 = 9999 - fn_80236228(target);
        } else if (sum < -5000) {
            arg1 = -5000 - fn_80236228(target);
        }
    } else {
        s32 sum = arg1 + fn_80236228(target);
        if (sum > 9999) {
            arg1 = 9999 - fn_80236228(target);
        } else if (sum < 0) {
            if (fn_80236228(target) <= 0) {
                arg1 = 0;
            } else {
                arg1 = -fn_80236228(target);
            }
        }
    }
    if (arg1 >= 1000 || arg1 <= -1000) {
        field_0x1f8[id] = __abs(arg1) / 100;
        field_0x200[id] = 0;
    } else {
        fn_80230FD0(id, 0x20034);
    }
    if ((u8)arg2) {
        fn_8023619C(target, arg1);
    } else {
        fn_802362C4(target, arg1);
    }
    lbl_80529DEC.field_0xe8[target] = fn_80236100(target);
    field_0x178[target] = 0;
    field_0x257 |= (u8)(1 << target);
}

// FIXME(shotgun): 98.36 %, missing/extra (the takesPart bool test is one cmplwi longer)
void cls_804B146C::fn_80425128() {
    for (s32 i = 0; i < 4; i++) {
        bool takesPart = gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << i);
        if (takesPart == true) {
            lbl_80529DEC.field_0xd8[i] = fn_80237774(i);
            for (s32 j = 0; j < 4; j++) {
                if (j != i && fn_8022F514(j) && i == fn_8022F3DC(j)) {
                    lbl_80529DEC.field_0xd8[j] = lbl_80529DEC.field_0xd8[i];
                }
            }
            field_0x256 |= (u8)(1 << i);
            field_0x168[i] = 0;
            lbl_80529DEC.field_0xe8[i] = fn_80236100(i);
            field_0x257 |= (u8)(1 << i);
            field_0x178[i] = 0;
        }
    }
}
