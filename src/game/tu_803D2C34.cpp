#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/tu_803B8C74.h"
#include "game/tu_80379F84.h"
#include <string.h>
#include <dolphin/os.h>

void cls_804B0288::fn_803D3928() {
    s32 result = lbl_8051C1A0.fn_803B9C5C();
    if (result == 1) {
        field_0x54 = 0x74;
        field_0x2177 = 0;
        field_0x216d = 10;
        field_0x216e = 0;
        lbl_80529DEC.field_0x24c = 0;
        field_0x7c = 90;
    } else if (result == 2) {
        field_0x54 = 0x75;
        field_0x2177 = 0;
        field_0x216d = 0x17;
        field_0x216e = 0;
        lbl_80529DEC.field_0x24c = 0;
    }
}

void cls_804B0288::fn_803D3898() {
    if (field_0x7c > 0) {
        field_0x7c--;
    }
    if (lbl_8053AD30->fn_80309C4C(0x1300, -1) || field_0x7c == 0) {
        field_0x7c = -1;
        field_0x54 = 0;
        field_0x216c = 5;
        field_0x2175 = 1;
        gAudioMgr_8053AF80->fn_80458880(0xB, -1);
    }
}

void cls_804B0288::fn_803D35C4() {
    OSResetSystem(1, 1, 1);
}

void cls_804B0288::fn_803D3460() {
    if (lbl_8053AD30->fn_80309C4C(1, -1)) {
        if (field_0x2170 != 0) {
            field_0x2170--;
            field_0x2171 = field_0x2170;
            field_0x82 = 1;
            gAudioMgr_8053AF80->fn_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(2, -1)) {
        if (field_0x2170 < 1) {
            field_0x2170++;
            field_0x2171 = field_0x2170;
            field_0x82 = 1;
            gAudioMgr_8053AF80->fn_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(0x100, -1)) {
        field_0x2171 = field_0x2170;
        if (field_0x2170 == 0) {
            field_0x54 = 0x78;
            field_0x2177 = 0;
            field_0x216d = 0xF;
            field_0x216e = 1;
            lbl_80529DEC.field_0x24c = 1;
            gAudioMgr_8053AF80->fn_80458880(0xB, -1);
        } else {
            field_0x54 = 0x77;
            field_0x2177 = 0;
            field_0x216d = 6;
            gAudioMgr_8053AF80->fn_80458880(0xC, -1);
        }
        field_0x82 = 1;
    }
}

// FIXME(shotgun): 93.54 %, missing/extra (the three save-file index setups)
void cls_804B0288::fn_803D335C() {
    lbl_8050EC80.fn_803B93E8((SaveFile_803B950C*)field_0x1c0, (SaveFileBody_803B950C*)field_0x420, 0);
    fn_803BACF8((SaveFile_803B950C*)field_0x1c0, 0);
    lbl_8050EC80.fn_803B93E8((SaveFile_803B950C*)field_0x1c0, (SaveFileBody_803B950C*)field_0x420, 1);
    fn_803BACF8((SaveFile_803B950C*)field_0x1c0, 1);
    lbl_8050EC80.fn_803B93E8((SaveFile_803B950C*)field_0x1c0, (SaveFileBody_803B950C*)field_0x420, 2);
    fn_803BACF8((SaveFile_803B950C*)field_0x1c0, 2);
    lbl_8051C1A0.fn_803B9D54(field_0x1c0, 0x258, field_0x420, 0x1D4C, 0);
    for (s32 i = 0; i < 3; i++) {
        if (i != lbl_8050EC80.mCurrentFile) {
            memcpy(&lbl_8050EC80.mFiles[i], field_0x1c0 + i * 0xC8, 0xC8);
        }
    }
    field_0x216c = 0x10;
}

void cls_804B0288::fn_803D3180() {
    if (field_0x7c > 0) {
        field_0x7c--;
    }
    if (lbl_8053AD30->fn_80309C4C(0x1300, -1) || field_0x7c == 0) {
        field_0x7c = -1;
        field_0x2170 = 0;
        field_0x54 = 0x7B;
        field_0x2177 = 0;
        field_0x216d = 2;
        gAudioMgr_8053AF80->fn_80458880(0xB, -1);
    }
}

void cls_804B0288::fn_803D3108() {
    memcpy(field_0x1c0, (u8*)&lbl_8050EC80 + 0x20, 0x258);
    fn_803BACF8((SaveFile_803B950C*)field_0x1c0, lbl_8050EC80.mCurrentFile);
    lbl_8051C1A0.fn_803B9CCC(field_0x1c0, 0x258, 0);
    field_0x216c = 0x13;
}

void cls_804B0288::fn_803D3060() {
    s32 result = lbl_8051C1A0.fn_803B9BDC();
    if (result == 1) {
        field_0x54 = 0x80;
        field_0x2177 = 0;
        field_0x216d = 0x14;
        field_0x216e = 0;
        lbl_80529DEC.field_0x24c = 0;
        field_0x1b4 = 1;
        field_0x7c = 90;
    } else if (result == 2) {
        field_0x54 = 0x7F;
        field_0x2177 = 0;
        field_0x216d = 0x17;
        field_0x216e = 0;
        lbl_80529DEC.field_0x24c = 0;
    }
}

// FIXME(shotgun): 99.80 %, scheduling
void cls_804B0288::fn_803D2FFC() {
    if (field_0x7c > 0) {
        field_0x7c--;
        if (field_0x7c != 0) {
            return;
        }
    }
    if (lbl_80529DEC.field_0x24d != 0) {
        field_0x216d = 0x19;
        field_0x216c = 4;
    } else {
        field_0x54 = 0x82;
        field_0x2177 = 0;
        field_0x216d = 3;
    }
}

void cls_804B0288::fn_803D2C34() {
    if (lbl_8053AD30->fn_80309C4C(0x1300, -1)) {
        field_0x54 = 0;
        field_0x216c = 5;
        field_0x2175 = 1;
        gAudioMgr_8053AF80->fn_80458880(0xB, -1);
    }
}
