#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/tu_80379F84.h"

// Defined in address order, as in the original: MWCC inlines a call to a
// function it has already seen, which the original does not.

// FIXME: 96.43 %. The original truncates the mode byte again (clrlwi r0, r0, 24)
// after its first compare with 5; ours has no such instruction. Tried: an s32
// mode (cmpwi throughout, 87.86 %) and a u32 mode with (u8) casts on the later
// compares (same 96.43 %: the byte load already fits, so no clrlwi is emitted).
bool cls_804AE9A0::fn_80379F84() {
    GameRoot_8049EF20* root = gGameRoot_8053AAF8;
    if (root->field_0x7f == 1 && root->field_0x7e == 1 && lbl_8053AD68 != NULL && field_0x4 != NULL) {
        u8 mode = lbl_8053ADA0->field_0x4c0;
        if (mode != 5 && mode != 8 && mode != 7 && mode != 9) {
            return true;
        }
    }
    return false;
}

// FIXME: 94.29 %. The original's first block tests stage < 8, sub < 4, stage >= 0,
// stage <= 7, sub >= 0, sub <= 3, in that order (six separate compares); ours
// tests stage >= 0, stage < 8, sub >= 0, sub < 4 and has no stage <= 7 / sub <= 3.
void fn_80379FF4(void) {
    s32 stage = lbl_80529DEC.field_0x118;
    s32 sub = lbl_80529DEC.field_0x11c;
    if (stage >= 0 && stage < 8 && sub >= 0 && sub < 4) {
        lbl_8050EC80[lbl_8050EC80[0].field_0x10].field_0x28 |= 1 << (stage * 4 + sub);
    }
    if (stage >= 8) {
        return;
    }
    s32 next = sub + 1;
    if (next > 3) {
        s32 nextStage = stage + 1;
        next = 0;
        if (nextStage >= 8) {
            nextStage = 0;
        }
        lbl_80529DEC.field_0x118 = nextStage;
    } else if ((stage == 0 && next == 2) || (stage == 1 && next == 2) || (stage == 2 && next == 2) ||
               (stage == 3 && next == 2) || (stage == 4 && next == 2) || (stage == 5 && next == 2) ||
               (stage == 6 && next == 2) || (stage == 7 && next == 1)) {
        next++;
    }
    lbl_80529DEC.field_0x11c = next;
}

u8 fn_8037A120(void) {
    if (lbl_8053AD68->field_0x4 != NULL) {
        return lbl_8053ADA0->field_0x4c0;
    }
    if (lbl_8053AD68->field_0x8 != NULL) {
        return lbl_8053ADA8->field_0x294;
    }
    if (lbl_8053AD68->field_0xc != NULL) {
        return lbl_8053ADC0->field_0x76;
    }
    return 0;
}

// FIXME: 94.74 %. Same mode-byte clrlwi as fn_80379F84 and fn_8037A664.
void cls_804AE9A0::fn_8037A580() {
    if (field_0x4 != NULL) {
        u8 mode = lbl_8053ADA0->field_0x4c0;
        if (mode != 5 && mode != 8 && mode != 7) {
            lbl_8053ADA0->fn_803A8F5C();
        } else {
            fn_8037AA7C();
            lbl_8053ADA0->fn_803B31AC();
        }
    }
    if (field_0x8 != NULL) {
        u8 mode = lbl_8053ADA8->field_0x294;
        if (mode == 5 || mode == 8 || mode == 7) {
            fn_8037AA7C();
            lbl_8053ADA8->fn_803B7DCC();
        }
    }
    if (field_0xc != NULL) {
        u8 mode = lbl_8053ADC0->field_0x76;
        if (mode == 2 || mode == 8 || mode == 7) {
            lbl_8053ADC0->fn_803D8364();
            lbl_8053ADC0->fn_803D82EC();
        }
    }
}

// FIXME: 95.12 %. Same as fn_80379F84 and fn_8037A580: the original truncates
// the mode byte again (clrlwi r0, r0, 24) after its first compare with 5, in
// both blocks. Tried: nested ifs and a function-scope mode (same code); a u32
// mode (same code); a negated || chain (MWCC turns it into a range check, 84.59 %).
void cls_804AE9A0::fn_8037A664() {
    if (field_0x4 != NULL) {
        u8 mode = lbl_8053ADA0->field_0x4c0;
        if (mode != 5 && mode != 8 && mode != 7) {
            lbl_8053ADA0->fn_803A97A0();
        }
    }
    if (field_0x8 != NULL) {
        u8 mode = lbl_8053ADA8->field_0x294;
        if (mode != 5 && mode != 8 && mode != 7) {
            lbl_8053ADA8->fn_803B7E64();
        }
    }
    if (field_0xc != NULL) {
        lbl_8053ADC0->fn_803D8670();
    }
}

void cls_804AE9A0::fn_8037A708() {
    if (field_0x4 != NULL) {
        field_0x4->fn_803B6768();
    }
    if (field_0x8 != NULL) {
        field_0x8->fn_803B8188();
    }
    if (field_0xc != NULL) {
        field_0xc->fn_803D87C0();
    }
    field_0x1c = 0;
}

u8 fn_8037AA28(void) {
    return lbl_8053ADB8->field_0x216c;
}

void fn_8037AA34(void) {
    lbl_8053ADB8->fn_803D5090();
}

void fn_8037AA58(void) {
    lbl_8053ADB8->fn_803D5554();
}

void fn_8037AA7C(void) {
    lbl_8053ADB8->fn_803D521C();
}
