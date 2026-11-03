// The stage/screen state, GameView_804A9950 (include/game/GameView.h).
// Written so far: the stage predicates that also read GameState_804B0FE0.

#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"

void GameView_804A9950::vfunc_0x14() {}

void GameView_804A9950::fn_802D39B8(s32 index, s32 value) {
    field_0x131a8[index] = value;
}

void GameView_804A9950::fn_802D9FE8(s32 a, s32 b) {
    field_0x1c = a;
    field_0x20 = b;
}

void GameView_804A9950::fn_802D9FF4(s32 a, s32 b) {
    field_0x24 = a;
    field_0x28 = b;
}

u8 GameView_804A9950::fn_802DA000() {
    return lbl_8053AB10->field_0x24->field_0x1e8;
}

u8 GameView_804A9950::fn_802DCD0C() {
    return lbl_8053AB10->field_0x24->field_0x1e8;
}

void GameView_804A9950::fn_802D790C(u8 value) {
    lbl_8053AAF8->field_0x4->mView->field_0x171 = value;
}

void GameView_804A9950::fn_802D7920(u8 value) {
    lbl_8053AAF8->field_0x4->mView->field_0x170 = value;
}

void GameView_804A9950::fn_802D7934(s32 value) {
    lbl_8053AAF8->field_0x4->mView->field_0x5c = value;
}

void GameView_804A9950::fn_802D6AAC(u8 value) {
    lbl_8053AAF8->field_0x4->mView->field_0x131f9 = value;
}

void GameView_804A9950::fn_802D6BC8(u8 value) {
    lbl_8053AAF8->field_0x4->mView->field_0x131f4 = value;
}

GameView_804A9950* fn_802D6A9C(void) {
    return lbl_8053AAF8->field_0x4->mView;
}

// FIXME: everything between 0x802D6AAC and 0x802DE814 is not written yet.

bool fn_802DE814(void) {
    if (lbl_8053AAF8->field_0x7f == 0) {
        return false;
    }
    s32 stage = lbl_80529DEC.field_0x118;
    if ((stage == 0 && lbl_80529DEC.field_0x11c == 2) ||
        (stage == 1 && lbl_80529DEC.field_0x11c == 2) ||
        (stage == 2 && lbl_80529DEC.field_0x11c == 2) ||
        (stage == 3 && lbl_80529DEC.field_0x11c == 2) ||
        (stage == 4 && lbl_80529DEC.field_0x11c == 2) ||
        (stage == 5 && lbl_80529DEC.field_0x11c == 2) ||
        (stage == 6 && lbl_80529DEC.field_0x11c == 2) ||
        (stage == 7 && lbl_80529DEC.field_0x11c == 1) ||
        (stage == 10 && lbl_80529DEC.field_0x11c == 2))
    {
        return true;
    }
    return false;
}

// FIXME: fn_802DE93C is not written yet.

// FIXME: 93.9 %. The target keeps lbl_8053AAF8->0x4 in r4 and reloads its
// mView (lwz 0xc) and the view's field_0x4 for the second stage pair; ours
// loads both once. No-ops: a local for lbl_8053AAF8->0x4, an inline view
// getter defined in this file (with or without a local), a free inline on
// GameRootSub04_801F856C; an inline isStage(stage, level) helper drops to 78 %.
bool fn_802DE9A0(void) {
    if (lbl_8053AAF8->field_0x7f != 0) {
        if ((lbl_8053AAF8->field_0x4->mView->field_0x4 == 2 && lbl_8053AAF8->field_0x4->mView->field_0x8 == 3) ||
            (lbl_8053AAF8->field_0x4->mView->field_0x4 == 7 && lbl_8053AAF8->field_0x4->mView->field_0x8 == 1))
        {
            return true;
        }
        return false;
    }
    s32 n = lbl_80529DEC.field_0x12c + lbl_80529DEC.field_0x128 * 4;
    if (n == 1 || n == 6) {
        return true;
    }
    return false;
}

// FIXME: fn_802DEA34 is not written yet.

// FIXME: 94.2 %. The target keeps lbl_8053AAF8->0x4 in r4 and reloads its
// mView (lwz 0xc) and the view's field_0x4 for the second stage pair; ours
// loads both once. An inline view getter in this file is a no-op. Re-reading
// the view into a local before each pair gives 96.9 % (field_0x4 reloaded,
// mView still not) but has no precedent, so it is not used.
bool fn_802DEABC(void) {
    if (lbl_8053AAF8->field_0x7f != 0) {
        if (lbl_8053AAF8->field_0x4->mView->field_0x4 == 7 && lbl_8053AAF8->field_0x4->mView->field_0x8 == 0) {
            return true;
        }
        if (lbl_8053AAF8->field_0x4->mView->field_0x4 == 7 && lbl_8053AAF8->field_0x4->mView->field_0x8 == 2) {
            return true;
        }
        return false;
    }
    s32 n = lbl_80529DEC.field_0x12c + lbl_80529DEC.field_0x128 * 4;
    if (n == 4 || n == 9) {
        return true;
    }
    return false;
}
