#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/cls_804B16A8.h"
#include "game/tu_803E1668.h"

// Run each frame (index: the state); the table is filled by the unit's static initialiser.
static cls_804B0B00::StateFunc lbl_804B0754[15] = {0};

// Steps the cursor's blink counter (field_0x254) and the frame counter (field_0x220).
static inline void stepBlink(cls_804B0B00* self) {
    if (self->field_0x254 != 0) {
        self->field_0x254++;
        if (self->field_0x254 > 10) {
            self->field_0x254 = 0;
        }
    }
    self->field_0x220++;
    if (self->field_0x220 >= 30) {
        self->field_0x220 = 0;
    }
}

void cls_804B0B00::fn_803E29C8() {
    field_0x5c->fn_8043670C(0);
    field_0x1d8 = field_0x1d4;
    field_0x1d4 = 14;
}

void cls_804B0B00::fn_803E2A0C() {
    field_0x1d8 = field_0x1d4;
    field_0x1d4 = 13;
}

void cls_804B0B00::fn_803E3548() {
    field_0x254 = 0;
    field_0x1d8 = field_0x1d4;
    field_0x1d4 = 10;
}

void cls_804B0B00::fn_803E3C00() {
    if (field_0x1bc == 0) {
        lbl_8053AEC0->fn_80431A9C(0xA9, field_0x1bc + 1, NULL);
    } else {
        lbl_8053AEC0->fn_80431A9C(0xC4, field_0x1bc + 1, NULL);
    }
    field_0x26c = 0;
    field_0x1d8 = field_0x1d4;
    field_0x1d4 = 8;
}

void cls_804B0B00::fn_803E3C80() {
    field_0x1d8 = field_0x1d4;
    field_0x1d4 = 7;
}

void cls_804B0B00::fn_803E3C94() {
    field_0x254 = 0;
    field_0x1d8 = field_0x1d4;
    field_0x1d4 = 6;
}

void cls_804B0B00::fn_803E4308() {
    field_0x220 = 0;
    field_0x254 = 0;
    field_0x1e0 = 5;
    field_0x1d8 = field_0x1d4;
    field_0x1d4 = 4;
}

void cls_804B0B00::fn_803E4330() {
    field_0x1d8 = field_0x1d4;
    field_0x1d4 = 3;
}

void cls_804B0B00::fn_803E45FC() {
    field_0x1d8 = field_0x1d4;
    field_0x1d4 = 1;
}

void cls_804B0B00::fn_803E4610() {
    field_0x1e0 = 0;
    field_0x254 = 0;
    field_0x1d8 = field_0x1d4;
    field_0x1d4 = 0;
}

void cls_804B0B00::fn_803E462C() {
    field_0x5c->fn_80436668();
}

void cls_804B0B00::fn_803E6A28() {}

void cls_804B0B00::fn_803E6A2C() {
    if (field_0x5c->fn_804366B8()) {
        if (field_0x26e == 0) {
            field_0x1dc = 9;
            field_0x26e = 1;
        }
        if (field_0x1bc == 0) {
            field_0x1b8 = 1;
        } else {
            field_0x1b8 = 0;
        }
        lbl_8053AF80->fn_80458880(11, -1);
    }
}

// FIXME: 99.8. Registers only: the original loads field_0x1bc into r4 and field_0x1d0 into r3, ours the other way round.
void cls_804B0B00::fn_803E9034() {
    stepBlink(this);
    if (field_0x1d0 != field_0x1bc + 1 && field_0x1bc > 0) {
        field_0x26c = 255;
        (this->*lbl_804B0754[5])();
    } else if (lbl_8053AD30->fn_80309C4C(0x1100, lbl_80529DEC.field_0x23c + 1)) {
        if (field_0x1b8 < 3) {
            field_0x1dc = 15;
            fn_803E3320();
        }
        lbl_8053AF80->fn_80458880(11, -1);
    } else if (lbl_8053AD30->fn_80309C4C(0x200, lbl_80529DEC.field_0x23c + 1)) {
        field_0x1dc = 2;
        (this->*lbl_804B0754[11])();
        lbl_8053AF80->fn_80458880(12, -1);
    } else if (lbl_8053AD30->fn_80309A9C(4, lbl_80529DEC.field_0x23c + 1)) {
        field_0x1b8++;
        if (field_0x1b8 >= 3) {
            field_0x1b8 = 0;
        }
        field_0x254 = 1;
        field_0x1e0 = 30;
        lbl_8053AF80->fn_80458880(0x26, -1);
    } else if (lbl_8053AD30->fn_80309A9C(8, lbl_80529DEC.field_0x23c + 1)) {
        field_0x1b8--;
        if (field_0x1b8 < 0) {
            field_0x1b8 = 2;
        }
        field_0x254 = 1;
        field_0x1e0 = 30;
        lbl_8053AF80->fn_80458880(0x26, -1);
    }
    field_0x1e0++;
    if (field_0x1e0 >= 60) {
        field_0x1e0 = 0;
    }
}

void cls_804B0B00::fn_803ED5B4() {
    if (field_0x26e == 0) {
        field_0x1dc = 9;
        field_0x26e = 1;
    }
    field_0x1b8 = 1;
    lbl_8053AF80->fn_80458880(11, -1);
}

// FIXME: 99.8. Registers only: the original loads field_0x1bc into r4 and field_0x1d0 into r3, ours the other way round.
void cls_804B0B00::fn_803ED604() {
    stepBlink(this);
    if (field_0x1d0 != field_0x1bc + 1 && field_0x1bc > 0) {
        field_0x26c = 255;
        (this->*lbl_804B0754[5])();
    } else if (lbl_8053AD30->fn_80309C4C(0x1100, lbl_80529DEC.field_0x23c + 1)) {
        if (field_0x1b8 < 3) {
            field_0x1dc = 15;
            fn_803E3320();
        }
        lbl_8053AF80->fn_80458880(11, -1);
    } else if (lbl_8053AD30->fn_80309C4C(0x200, lbl_80529DEC.field_0x23c + 1)) {
        field_0x1dc = 2;
        (this->*lbl_804B0754[11])();
        lbl_8053AF80->fn_80458880(12, -1);
    } else if (lbl_8053AD30->fn_80309A9C(4, lbl_80529DEC.field_0x23c + 1)) {
        if (field_0x1b8 == 1) {
            field_0x1b8 = 2;
        } else {
            field_0x1b8 = 1;
        }
        field_0x254 = 1;
        field_0x1e0 = 30;
        lbl_8053AF80->fn_80458880(0x26, -1);
    } else if (lbl_8053AD30->fn_80309A9C(8, lbl_80529DEC.field_0x23c + 1)) {
        if (field_0x1b8 == 1) {
            field_0x1b8 = 2;
        } else {
            field_0x1b8 = 1;
        }
        field_0x254 = 1;
        field_0x1e0 = 30;
        lbl_8053AF80->fn_80458880(0x26, -1);
    }
    field_0x1e0++;
    if (field_0x1e0 >= 60) {
        field_0x1e0 = 0;
    }
}
