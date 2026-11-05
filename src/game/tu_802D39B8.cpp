// The stage/screen state, GameView_804A9950 (include/game/GameView.h).
// Written so far: the stage predicates that also read GameState_804B0FE0.

#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"

void GameView_804A9950::fn_802D39B8(s32 index, s32 value) {
    field_0x131a8[index] = value;
}

bool GameView_804A9950::fn_802D39CC() {
    if (field_0x18 == field_0x10 && field_0x14 == field_0xc) {
        return true;
    }
    return false;
}

s32 GameView_804A9950::fn_802D39FC() {
    return field_0x60[field_0x18][field_0x14];
}

// FIXME: fn_802D3A18 and fn_802D3D48 are not written yet.

GameView_804A9950* fn_802D6A9C(void) {
    return lbl_8053AAF8->field_0x4->mView;
}

void GameView_804A9950::fn_802D6AAC(u8 value) {
    lbl_8053AAF8->field_0x4->mView->field_0x131f9 = value;
}

// FIXME: 91.0 %. Only scheduling: the target computes bit / 32 (srawi/addze) and
// the row offset earlier; ours places one add later (word & mask order is as
// in the target; a mask local and swapped & operands are worse).
bool GameView_804A9950::fn_802D6AC4(s32 bit) {
    return (field_0xfd68[field_0x18][field_0x14][bit / 32] & (1 << (bit % 32))) != 0;
}

void GameView_804A9950::fn_802D6B24(s32 bit, bool set) {
    // word before shift: the declaration order decides the registers (as fn_802D7310).
    s32 word = bit / 32;
    s32 shift = bit % 32;
    if (set) {
        field_0xfd68[field_0x18][field_0x14][word] |= 1 << shift;
    } else {
        field_0xfd68[field_0x18][field_0x14][word] &= (1 << shift) ^ -1;
    }
}

void GameView_804A9950::fn_802D6BC8(u8 value) {
    lbl_8053AAF8->field_0x4->mView->field_0x131f4 = value;
}

void GameView_804A9950::fn_802D6BE0(u8 value) {
    lbl_8053AAF8->field_0x4->mView->field_0x131f3 = value;
}

void GameView_804A9950::fn_802D6BF8(u8 value) {
    lbl_8053AAF8->field_0x4->mView->field_0x131f2 = value;
}

bool GameView_804A9950::fn_802D72BC(s32 bit) {
    return ((1 << (bit % 32)) & lbl_8053AAF8->field_0x4->mView->field_0x12fe8[bit / 32]) != 0;
}

void GameView_804A9950::fn_802D7310(s32 bit, bool set) {
    // word before shift: the declaration order decides the registers; `^ -1` because the target
    // clears with xor+and, not andc.
    s32 word = bit / 32;
    s32 shift = bit % 32;
    if (set) {
        field_0x12fe8[word] |= 1 << shift;
    } else {
        field_0x12fe8[word] &= (1 << shift) ^ -1;
    }
}

// FIXME: 88.1 %. Scheduling: one add (the word address) is emitted two slots
// earlier than in the target; a mask local and swapped & operands are worse.
bool GameView_804A9950::fn_802D7384(s32 bit) {
    return (field_0x12f68[bit / 32] & (1 << (bit % 32))) != 0;
}

void GameView_804A9950::fn_802D73CC(s32 bit, bool set) {
    // word before shift: the declaration order decides the registers; `^ -1` because the target
    // clears with xor+and, not andc.
    s32 word = bit / 32;
    s32 shift = bit % 32;
    if (set) {
        field_0x12f68[word] |= 1 << shift;
    } else {
        field_0x12f68[word] &= (1 << shift) ^ -1;
    }
}

void GameView_804A9950::fn_802D752C(u8 value) {
    lbl_8053AAF8->field_0x4->mView->field_0x131ef = value;
}

u8 GameView_804A9950::fn_802D7544(s32 index) {
    if (index >= 0 && index < 0x100) {
        return field_0x9968[field_0x18][field_0x14][index];
    }
    return 0;
}

void GameView_804A9950::fn_802D7584(s32 index, u8 value) {
    if (index >= 0 && index < 0x100) {
        field_0x9968[field_0x18][field_0x14][index] = value;
    }
}

// FIXME: 91.3. Only scheduling: the word address add is emitted two slots earlier than in the target
// (same as fn_802D6AC4; swapped & operands are worse).
bool GameView_804A9950::fn_802D75BC(s32 bit) {
    return (field_0x6768[field_0x18][field_0x14][bit / 32] & (1 << (bit % 32))) != 0;
}

void GameView_804A9950::fn_802D7618(s32 bit) {
    field_0x6768[field_0x18][field_0x14][bit / 32] |= 1 << (bit % 32);
}

void GameView_804A9950::fn_802D766C(bool flag, s32 layer) {
    if (flag) {
        field_0x6748[layer < 8 ? 0 : layer - 7]++;
    }
}

// FIXME: 91.3. Only scheduling: the word address add is emitted two slots earlier than in the target
// (same as fn_802D6AC4; swapped & operands are worse).
bool GameView_804A9950::fn_802D76A8(s32 bit) {
    return (field_0x3548[field_0x18][field_0x14][bit / 32] & (1 << (bit % 32))) != 0;
}

void GameView_804A9950::fn_802D7704(s32 bit, bool count, s32 layer) {
    field_0x3548[field_0x18][field_0x14][bit / 32] |= 1 << (bit % 32);
    if (count) {
        field_0x6748[layer < 8 ? 0 : layer - 7]++;
    }
}

// `bits ^ 0xFF` (not ~bits): the target clears with xori+and, not andc.
void GameView_804A9950::fn_802D7790(s32 index, u16 bits) {
    lbl_8053AAF8->field_0x4->mView->field_0x131b8[index] &= bits ^ 0xFF;
}

void GameView_804A9950::fn_802D77BC(s32 index, u16 bits) {
    lbl_8053AAF8->field_0x4->mView->field_0x131b8[index] |= bits;
}

void GameView_804A9950::fn_802D77E4(s32 index, u16 value) {
    lbl_8053AAF8->field_0x4->mView->field_0x131b8[index] = value;
}

// FIXME: 80.0 %. The clrlwi of bits is emitted before the lhz load in ours,
// after it in the target; swapping the & operands is worse.
bool GameView_804A9950::fn_802D7804(s32 index, u16 bits) {
    return (field_0x131b8[index] & bits) != 0;
}

void GameView_804A9950::fn_802D782C(u8 value) {
    lbl_8053AAF8->field_0x4->mView->field_0x131de = value;
}

void GameView_804A9950::fn_802D7844(u8 value, s32 index) {
    if (index >= 0 && index < 4) {
        lbl_8053AAF8->field_0x4->mView->field_0x131d8[index] = value;
    }
}

void GameView_804A9950::fn_802D7870(u16 value) {
    lbl_8053AAF8->field_0x4->mView->field_0x131dc = value;
}

bool GameView_804A9950::fn_802D7888(s32 bit) {
    return ((1 << bit) & lbl_8053AAF8->field_0x4->mView->field_0x174) != 0;
}

// `^ -1` (not ~): the target clears with xor+and, not andc.
void GameView_804A9950::fn_802D78B4(s32 bit, bool set) {
    if (set) {
        lbl_8053AAF8->field_0x4->mView->field_0x174 |= 1 << bit;
    } else {
        lbl_8053AAF8->field_0x4->mView->field_0x174 &= (1 << bit) ^ -1;
    }
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

s32 GameView_804A9950::fn_802D7948() {
    GameView_804A9950* view = lbl_8053AAF8->field_0x4->mView;
    if (view->field_0x5c == 0) {
        return view->field_0x58;
    }
    return view->field_0x5c - 1;
}

void GameView_804A9950::fn_802D7970(s32 value) {
    lbl_8053AAF8->field_0x4->mView->field_0x34 = value;
    if (value < 0x11) {
        lbl_8053AAF8->field_0x4->mView->field_0x1328c = 0;
    }
}

u64 GameView_804A9950::fn_802D7AC4(s32 layer) {
    if (layer < 8) {
        return lbl_8053AAF8->field_0x4->mView->field_0x1010[field_0x18][field_0x14][7];
    }
    return lbl_8053AAF8->field_0x4->mView->field_0x1010[field_0x18][field_0x14][layer];
}

// FIXME: 99.0. Regalloc only: the target keeps the shift count in r31 and the slot index in r30; ours swaps them.
s32 GameView_804A9950::fn_802D7B34(s32 layer, s32 slot) {
    s32 idx = layer < 8 ? 0 : layer - 7;
    s32 shift = 60 - slot * 4;
    return (lbl_8053AAF8->field_0x4->mView->field_0x1010[field_0x18][field_0x14][7 + idx] & (0xFULL << shift)) >> shift;
}

s32 GameView_804A9950::fn_802D7E04(s32 index) {
    return lbl_8053AAF8->field_0x4->mView->field_0x308[index];
}

s32 GameView_804A9950::fn_802D7E1C(s32 index) {
    return lbl_8053AAF8->field_0x4->mView->field_0x328[index];
}

void GameView_804A9950::fn_802D7E34(s32 index) {
    GameView_804A9950* view = lbl_8053AAF8->field_0x4->mView;
    if (view->field_0x308[index] < 0xFF) {
        view->field_0x308[index]++;
    }
}

// FIXME: 85.2 %. Scheduling: the target loads field_0x8 before the stage
// (field_0x4) and the view; ours loads the stage first.
bool GameView_804A9950::fn_802D7F40(s32 bit) {
    return ((1 << bit) & lbl_8053AAF8->field_0x4->mView->field_0x348[field_0x4][field_0x8]) != 0;
}

void GameView_804A9950::fn_802D7F84(s32 bit, bool set) {
    if (set) {
        lbl_8053AAF8->field_0x4->mView->field_0x348[field_0x4][field_0x8] |= 1 << bit;
    } else {
        lbl_8053AAF8->field_0x4->mView->field_0x348[field_0x4][field_0x8] &= (1 << bit) ^ -1;
    }
}

bool GameView_804A9950::fn_802D800C(s32 bit) {
    return ((1 << bit) & lbl_8053AAF8->field_0x4->mView->field_0x178[field_0x18][field_0x14]) != 0;
}

void GameView_804A9950::fn_802D8050(s32 bit, bool set, u8 value) {
    if (set) {
        lbl_8053AAF8->field_0x4->mView->field_0x178[field_0x18][field_0x14] |= 1 << bit;
        if (lbl_8053AAF8->field_0x4->mView->field_0x308[bit] < 0xFF) {
            lbl_8053AAF8->field_0x4->mView->field_0x308[bit]++;
        }
        lbl_8053AAF8->field_0x4->mView->field_0x328[bit] = value;
    } else {
        lbl_8053AAF8->field_0x4->mView->field_0x178[field_0x18][field_0x14] &= (1 << bit) ^ -1;
        lbl_8053AAF8->field_0x4->mView->field_0x328[bit] = 0xFF;
    }
}

void GameView_804A9950::fn_802D9CB0() {
    lbl_8053AB10->field_0x24->fn_8030C85C();
}

void GameView_804A9950::fn_802D9FBC() {
    if (lbl_8053AAF8->field_0x7f == 1) {
        field_0xc4 = 0x8CA0;
    } else {
        field_0xc4 = 0x2A30;
    }
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

void GameView_804A9950::fn_802DCDF4() {
    fn_801F856C()->fn_801F7CAC();
    fn_801F856C()->fn_801F6204(0);
    fn_80308D0C();
    lbl_8053AAF8->field_0x4->mView->field_0x131de = 0;
    fn_8013C824()->fn_8013C8CC();
    field_0xc9 = 0;
}

void GameView_804A9950::vfunc_0x14() {}

// FIXME: the other GameView_804A9950 functions before 0x802DE814 are not written yet.

GameView_804A9950::~GameView_804A9950() {}

bool fn_802DE764(s32 stage, s32 level, s32 value) {
    if (lbl_8053AAF8->field_0x7f == 0) {
        return false;
    }
    GameView_804A9950* view = lbl_8053AAF8->field_0x4->mView;
    if (stage == view->field_0x4 && level == view->field_0x8 && value == view->field_0x60[view->field_0x18][view->field_0x14]) {
        return true;
    }
    return false;
}

bool fn_802DE7CC(void) {
    if (lbl_8053AAF8->field_0x7f == 0) {
        return false;
    }
    if (lbl_8053AAF8->field_0x4->mView->field_0x4 == 0 && lbl_8053AAF8->field_0x4->mView->field_0x8 == 0) {
        return true;
    }
    return false;
}

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

// FIXME: 91.0 %. Same as fn_802DE9A0: the target keeps lbl_8053AAF8->0x4 in r4
// and reloads the view (lwz 0xc) for the second pair; ours loads it once.
bool fn_802DE93C(void) {
    if (lbl_8053AAF8->field_0x7f != 0) {
        if ((lbl_8053AAF8->field_0x4->mView->field_0x4 == 4 && lbl_8053AAF8->field_0x4->mView->field_0x8 == 0) ||
            (lbl_8053AAF8->field_0x4->mView->field_0x4 == 7 && lbl_8053AAF8->field_0x4->mView->field_0x8 == 2))
        {
            return true;
        }
        return false;
    }
    return false;
}

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

// FIXME: 93.5. Same family as fn_802DEABC: the target reloads the view (lwz 0xc) before the second
// pair and reuses it for the third; ours loads it once. An || of the last two blocks is 87 %.
bool fn_802DEA34(void) {
    if (lbl_8053AAF8->field_0x7f != 0) {
        if (lbl_8053AAF8->field_0x4->mView->field_0x4 == 6 && lbl_8053AAF8->field_0x4->mView->field_0x8 == 0) {
            return true;
        }
        if (lbl_8053AAF8->field_0x4->mView->field_0x4 == 6 && lbl_8053AAF8->field_0x4->mView->field_0x8 == 1) {
            return true;
        }
        if (lbl_8053AAF8->field_0x4->mView->field_0x4 == 6 && lbl_8053AAF8->field_0x4->mView->field_0x8 == 3) {
            return true;
        }
        return false;
    }
    return false;
}

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

bool fn_802DEB58(void) {
    if (lbl_8053AAF8->field_0x7f == 0) {
        return false;
    }
    if (lbl_8053AAF8->field_0x4->mView->field_0x4 == 3 && lbl_8053AAF8->field_0x4->mView->field_0x8 == 1) {
        return true;
    }
    return false;
}

// FIXME: 89.8 %. Same as fn_802DE9A0: the target reloads the view (lwz 0xc off
// the kept lbl_8053AAF8->0x4) for the stage >= 13 test; ours loads it once.
bool fn_802DEBA0(void) {
    if (lbl_8053AAF8->field_0x7f == 0) {
        return false;
    }
    if ((lbl_8053AAF8->field_0x4->mView->field_0x4 == 9 && lbl_8053AAF8->field_0x4->mView->field_0x8 == 3) || lbl_8053AAF8->field_0x4->mView->field_0x4 >= 13) {
        return true;
    }
    return false;
}

bool fn_802DEBF8(void) {
    if (lbl_8053AAF8->field_0x7f == 0) {
        return false;
    }
    if (lbl_8053AAF8->field_0x4->mView->field_0x4 == 8 && lbl_8053AAF8->field_0x4->mView->field_0x8 == 3) {
        return true;
    }
    return false;
}

bool fn_802DEDFC(const s32* dir) {
    return lbl_8053AAF8->field_0x4->mView->fn_802DCE5C(dir);
}
