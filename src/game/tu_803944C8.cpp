#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/GameView.h"
#include "game/Message.h"
#include "game/tu_803944C8.h"
#include "game/tu_803B8C74.h"
#include "game/act/act_small_key.h"
#include "JSystem/JMessage/TControl.h"

void fn_803A3228(s32 id, s32 arg1, u32 arg2) {
    lbl_8053AED0->fn_8043D874(id, arg1, arg2);
}

s32 SmallKey_804A8CD8::fn_803A3220() {
    return field_0x280;
}

s32 fn_803A3210() {
    return lbl_80529DEC.field_0x118;
}

s32 fn_803A3200() {
    return lbl_80529DEC.field_0x11c;
}

void fn_803A31AC(u8 index, s32 bit, bool set) {
    getGameView_802D6A9C()->fn_802D7EB0(index, bit, set);
}

bool fn_803A3168(u8 index, s32 bit) {
    return getGameView_802D6A9C()->fn_802D7E5C(index, bit);
}

void fn_803A3144() {
    gGameRoot_8053AAF8->fn_80138B7C();
}

u8 cls_804AF5A0::fn_803A313C() {
    return field_0x1aa;
}

void cls_804AF5A0::fn_803A3114() {
    if (field_0x1a0 == 0) {
        field_0x168++;
    }
    field_0x16c++;
}

s32 cls_804AF5A0::fn_803A310C() {
    return field_0x16c;
}

void cls_804AF5A0::fn_803A30FC(s32 index, s32 value) {
    field_0x13c[index] = value;
}

void cls_804AF5A0::fn_803A30F0() {
    field_0x1a0 = 2;
}

void cls_804AF5A0::fn_803A30E4() {
    field_0x1a1 = 0;
}

void cls_804AF5A0::fn_803A30DC(s32 value) {
    field_0x148 = value;
}

void cls_804AF5A0::fn_803A30D0() {
    field_0x1a1 = 1;
}

void cls_804AF5A0::fn_803A30C4() {
    field_0x1a0 = 3;
}

s32 cls_804AF5A0::fn_803A30BC() {
    return field_0x160;
}

void cls_804AF5A0::fn_803A30B0() {
    field_0x1a9 = 1;
}

void cls_804AF5A0::fn_803A30A4(u8 a, u32 b) {
    field_0x150 = b;
    field_0x19e = a;
}

cls_804B20F8* cls_804AF5A0::fn_803A309C() {
    return field_0x3bc;
}

s32 cls_804AF5A0::fn_803A3094() {
    return field_0x14c;
}

void cls_804AF5A0::fn_803A3084(s32 index, s32 value) {
    field_0x180[index] = value;
}

void cls_804AF5A0::fn_803A3078() {
    field_0x3b1 = 0;
}

void cls_804AF5A0::fn_803A3070(s32 value) {
    field_0x174 = value;
}

s16 cls_804AF5A0::fn_803A3068() {
    return field_0x190;
}

s16 cls_804AF5A0::fn_803A3060() {
    return field_0x192;
}

s32 cls_804AF5A0::fn_803A3058() {
    return field_0x178;
}

s32 cls_804AF5A0::fn_803A3050() {
    return field_0x17c;
}

// Dead-stripped in the original (no out-of-line copy): its body is what the
// constructor and cls_804AF774::do_reset inline, the four stores in this order
// through one load of the window (docs/RULES_CARD.md "Literals before their
// first visible use" (2), the same reconstruction).
void cls_804AF5A0::resetFields0x168() {
    field_0x16c = 0;
    field_0x1a0 = 0;
    field_0x1a1 = 0;
    field_0x168 = -1;
}

u16 fn_803A3034() {
    return lbl_8050EC80.getCurrentFile().field_0x14;
}

u16 fn_803A3018() {
    return lbl_8050EC80.getCurrentFile().field_0x16;
}

u8 fn_803A300C() {
    return lbl_8053AD78->field_0xe9;
}

void fn_803A2FFC() {
    lbl_8053AD78->field_0xe9 = 0;
}

void fn_803A2FEC() {
    lbl_8053AD78->field_0x44 = 0;
}

s32 fn_803A2FE0() {
    return lbl_8053AD78->field_0x10c;
}

void cls_804AF774::fn_803A2F74() {
    field_0x4c = 0;
    field_0x54 = 0;
    field_0x50 = 0;
    field_0x58 = 0;
    field_0x5d = 0;
    field_0x60->field_0x15c++;
    field_0x60->fn_803A30A4(0xFF, 0);
    field_0x60->fn_803A30DC(-1);
    field_0x60->fn_803A30FC(0, -1);
    field_0x60->fn_803A30FC(1, -1);
    field_0x60->fn_803A30FC(2, -1);
    field_0x60->field_0x3ae = 2;
}

// Adds the number of characters `value` prints as (a sign, then its digits
// without leading zeros, at most five) to the column count.
// FIXME: 93.87. scheduling+regalloc: in the ten-thousands step the original
// computes value % 10000 before the digit test (the later steps match).
void cls_804AF774::fn_803A2E48(s32 value) {
    bool started = false;
    if (value < 0) {
        field_0x50++;
        value *= -1;
    }
    s32 digit = value / 10000;
    value %= 10000;
    if (digit != 0) {
        field_0x50++;
        started = true;
    }
    digit = value / 1000;
    value %= 1000;
    if (digit != 0 || started) {
        field_0x50++;
        started = true;
    }
    digit = value / 100;
    value %= 100;
    if (digit != 0 || started) {
        field_0x50++;
        started = true;
    }
    if (value / 10 != 0 || started) {
        field_0x50++;
    }
    field_0x50++;
}

// Adds the length of the counted noun that follows a number (singular when
// count is 1): the unit's " Force Gem" / " Force Gems" ... " Force Fairy" /
// " Force Fairies" strings (lbl_80475DC8), plus one.
void cls_804AF774::fn_803A2CB0(u8 noun, s32 count) {
    switch (noun) {
    case 1:
        if (count == 1) {
            field_0x50 += 10;
        } else {
            field_0x50 += 11;
        }
        break;
    case 2:
        if (count == 1) {
            field_0x50 += 6;
        } else {
            field_0x50 += 7;
        }
        break;
    case 3:
        if (count == 1) {
            field_0x50 += 6;
        } else {
            field_0x50 += 8;
        }
        break;
    case 4:
        if (count == 1) {
            field_0x50 += 5;
        } else {
            field_0x50 += 6;
        }
        break;
    case 5:
        if (count == 1) {
            field_0x50 += 7;
        } else {
            field_0x50 += 8;
        }
        break;
    case 6:
        if (count == 1) {
            field_0x50 += 6;
        } else {
            field_0x50 += 8;
        }
        break;
    case 7:
        if (count == 1) {
            field_0x50 += 5;
        } else {
            field_0x50 += 6;
        }
        break;
    case 8:
        if (count == 1) {
            field_0x50 += 7;
        } else {
            field_0x50 += 8;
        }
        break;
    case 9:
        if (count == 1) {
            field_0x50 += 12;
        } else {
            field_0x50 += 14;
        }
        break;
    }
    field_0x50++;
}

void cls_804AF774::do_reset() {
    field_0x4c = 0;
    field_0x54 = 0;
    field_0x50 = 0;
    field_0x58 = 0;
    field_0x5d = 0;
    field_0x60->fn_803A30A4(0xFF, 0);
    field_0x60->field_0x15c = 0;
    field_0x60->field_0x3af = 1;
    field_0x60->resetFields0x168();
    field_0x60->fn_803A30DC(-1);
    field_0x60->field_0x1a9 = 0;
    field_0x60->fn_803A3070(-1);
    field_0x60->fn_803A30FC(0, -1);
    field_0x60->fn_803A30FC(1, -1);
    field_0x60->fn_803A30FC(2, -1);
    if (field_0x60->field_0x1aa == 0) {
        field_0x60->field_0x3ae = 7;
    } else {
        field_0x60->field_0x3ae = 2;
    }
}

void cls_804AF774::do_begin(const void*, const char*) {
    field_0x5c = 1;
}

void cls_804AF774::do_end() {
    field_0x60->field_0x3af = 0;
    if (field_0x60->field_0x1aa == 0) {
        field_0x60->field_0x3ae = 8;
    } else {
        field_0x60->field_0x3ae = 3;
    }
}

// FIXME: 98.82, kept out of the build (merge rule: the two rows differ in
// opcode, not order or registers). The original tests fn_8037E7AC's result
// with cmpwi (an int / BOOL return; its body normalises with neg/or/srwi);
// Message.h declares it bool, which gives clrlwi. here. Needs fn_8037E7AC's
// return type changed in game/tu_8037DF68.cpp (that unit's owner).
#ifdef NONMATCHING
bool cls_804AF774::do_isReady() {
    if (field_0x60->field_0x1aa == 0) {
        if (field_0x4c == 0) {
            return true;
        }
        if (field_0x4c == 99999) {
            if (field_0x60->field_0x19f != 0) {
                field_0x4c = 0;
                field_0x54 = 0;
                field_0x50 = 0;
                field_0x58 = 0;
                field_0x5d = 0;
                field_0x60->field_0x15c++;
                field_0x60->field_0x19f = 0;
            }
        } else {
            field_0x4c--;
            if (field_0x5c == 0) {
                if (field_0x60->field_0x19e != 0) {
                    if (field_0x60->field_0x3b8->field_0x28 & 0x100) {
                        if (lbl_8053AD30->fn_80309C4C(0x100, -1) != 0 || field_0x60->field_0x1a0 != 0 ||
                            fn_8037E7AC(field_0x60->field_0x160)) {
                            field_0x5d = 1;
                            field_0x4c = 0;
                        }
                    } else {
                        if (lbl_8053AD30->fn_80309D6C(getPlayerInputSlot_8030AAF4(field_0x60->field_0x160), 0x100) != 0 ||
                            field_0x60->field_0x1a0 != 0 || fn_8037E7AC(field_0x60->field_0x160)) {
                            field_0x5d = 1;
                            field_0x4c = 0;
                        }
                    }
                }
            } else {
                field_0x5c = 0;
            }
        }
    } else {
        if (field_0x4c == 0) {
            return true;
        }
        field_0x4c--;
    }
    return false;
}

#endif

void cls_804AF774::do_character(int c) {
    if (field_0x60->field_0x1aa == 0) {
        s8 lead = (c >> 8) & 0xFF;
        if (c == '\n') {
            field_0x54++;
            if (field_0x58 == 1) {
                field_0x58 = 2;
            }
        }
        if (lead != 0) {
            field_0x50++;
        }
        field_0x50++;
        if (field_0x5d == 0) {
            if (!(field_0x60->field_0x3b8->field_0x28 & 0x40)) {
                field_0x4c = 1;
            } else {
                field_0x4c = lbl_8050DB0C.field_0x4c[0];
            }
        }
        gAudioMgr_8053AF80->fn_80458880(8, -1);
        if (field_0x58 == 2 || field_0x54 >= 3 || field_0x50 >= 0xFF) {
            field_0x4c = 99999;
            field_0x60->field_0x3ae = 8;
        }
    } else {
        if (c == '\n') {
            field_0x54++;
            if (field_0x58 == 1) {
                field_0x58 = 2;
            }
        }
        field_0x50 += 2;
        if (field_0x58 == 2 || field_0x54 >= 3 || field_0x50 >= 0xFF) {
            field_0x4c = 99999;
            field_0x60->field_0x3ae = 3;
        }
    }
}

bool cls_804AF774::do_jump_isReady() {
    return true;
}

void cls_804AF774::do_jump(const void*, const char*) {}

void cls_804AF72C::do_reset() {
    *field_0x2c0 = 0;
    *field_0x2c4 = 0;
}

void cls_804AF72C::do_end() {
    if (field_0x2d8->field_0x1aa == 0) {
        return;
    }
    field_0x2c0[field_0x2d0++] = -1;
    field_0x2c0[field_0x2d0++] = -2;
}

// FIXME: 93.34. scheduling (+ the unsplit 0.0f literal): the original
// interleaves the field_0x1fe stores and field_0x1fd with the first three
// iterations of the unrolled 15-entry loop.
void cls_804AF6E4::do_begin(const void*, const char*) {
    field_0x3c = -1;
    field_0x40 = -1;
    field_0x201 = 0;
    field_0x4c[field_0x201] = 0.0f;
    field_0xf4[field_0x201] = 0.0f;
    field_0x22d[field_0x201] = 0;
    field_0x22c = 0;
    field_0x281 = 0;
    field_0x282 = 0;
    for (int i = 0; i < 3; i++) {
        field_0x1fe[i] = 0;
    }
    field_0x1fd = 0;
    for (int i = 0; i < 15; i++) {
        field_0x19c[i] = 0.0f;
        field_0x1ee[i] = 0;
    }
    field_0x202[field_0x281] = -1;
    field_0x257[field_0x281] = 0;
    field_0x1e8 = 0;
}

void cls_804AF6E4::do_end() {
    field_0x281++;
    field_0x202[field_0x281] = field_0x22c;
    field_0x257[field_0x281] = 0;
}

cls_804AF6E4::~cls_804AF6E4() {}

cls_804AF6D4::~cls_804AF6D4() {}

cls_804AF774::~cls_804AF774() {}

cls_804AF72C::~cls_804AF72C() {}

void cls_804AF5A0::fn_80396F94() {
    field_0x8->update();
    fn_80396884();
    fn_80395BE8();
    field_0x3b0 = 1;
}

void cls_804AF5A0::fn_803944C8(s32 value, u8 a, u8 b) {
    field_0x3b1 = 1;
    lbl_8053AD78->field_0x48 = value;
    lbl_8053AD78->field_0xea = a;
    lbl_8053AD78->field_0xeb = b;
}
