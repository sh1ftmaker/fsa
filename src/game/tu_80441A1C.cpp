#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/Message.h"
#include "game/cls_804B19F0.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/J2D/J2DPane.h"
#include "JSystem/J2D/J2DGrafContext.h"

static cls_804B19F0* lbl_8053AEE0;

void cls_804B19F0::fn_80442200() {
    gGameRoot2_8053AB10->fn_8013B490();
    if (field_0x18 != 0) {
        field_0x4->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
    }
}

cls_804B19F0::~cls_804B19F0() {
    delete field_0x4;
    field_0x4 = NULL;
    delete field_0x8;
    field_0x8 = NULL;
    delete field_0xc;
    field_0xc = NULL;
    lbl_8053AEE0 = NULL;
}

cls_804B19F0::cls_804B19F0() {
    lbl_8053AEE0 = this;
    field_0x4 = new J2DScreen();
    field_0x4->setPriority("gba_connect_error_window.blo", 0x1100000, lbl_80529DEC.field_0x20);
    JUTResFont* font = lbl_8053AD78->mFont;
    ((J2DTextBox*)field_0x4->search(0x0000005477743031ull))->setFont(font);
    field_0x8 = new PaneCtrl_804AF86C();
    field_0x8->setPane_80402F04(field_0x4->search(0x005077626c6b3031ull));
    field_0xc = new PaneCtrl_804AF86C();
    field_0xc->setPane_80402F04(field_0x4->search(0x4e64617461673031ull));
    field_0x8->mPane->setAlpha(0);
    field_0x8->field_0x26 = 0;
    field_0xc->moveFromCentre_80402E08(0.0f, -480.0f);
    field_0xc->field_0x22 = 0;
    field_0x10 = 0x16;
    field_0x14 = 0;
    field_0x18 = 0;
    field_0x1c = 0;
}

void cls_804B19F0::fn_80442254(s32 arg, s32 index) {
    field_0x8->mPane->setAlpha(0);
    field_0x8->field_0x26 = 0;
    field_0xc->moveFromCentre_80402E08(0.0f, -480.0f);
    field_0xc->field_0x22 = 0;
    field_0x10 = arg;
    field_0x14 = index - 1;
    field_0x18 = 1;
    field_0x1c = 0;
    char* str = ((J2DTextBox*)field_0x4->search(0x0000005477743031ull))->getStringPtr();
    s32 player = field_0x14;
    u32 mode = (lbl_8053AAF8->field_0x83 >> (player * 2)) & 3;
    if (mode == 1) {
        getMessageText_803832BC(str, 0x3B, player + 1, NULL, NULL);
    } else if (mode == 2) {
        getMessageText_803832BC(str, 0x3A, player + 1, NULL, NULL);
    }
}

// FIXME: 85.4 %. The first differing rows are in state 1 (0x21C-0x23C): our
// `this` lives in r27 where the original keeps it in r29, so every later
// access to a field in this function uses another base register. From state 2
// on (0x2C0 onward) the float ratio is scheduled in a different order (the
// original loads the magic double and 480.0f before the ratio's first lfs).
s32 cls_804B19F0::fn_80441A1C() {
    s32 player = field_0x14;
    u32 mode = (lbl_8053AAF8->field_0x83 >> (player * 2)) & 3;
    if (mode == 1) {
        if (lbl_8053AD30->fn_8030A258(getPlayerInputSlot_8030AAF4(player))) {
            if (field_0x8->field_0x26 > lbl_8050DB0C.field_0x4c[36]) {
                field_0x8->field_0x26 = lbl_8050DB0C.field_0x4c[36];
            }
            if (field_0xc->field_0x26 > lbl_8050DB0C.field_0x4c[35] + lbl_8050DB0C.field_0x4c[36]) {
                field_0xc->field_0x26 = lbl_8050DB0C.field_0x4c[35] + lbl_8050DB0C.field_0x4c[36];
            }
            field_0x18 = 11;
        }
    } else if (mode == 2) {
        if (lbl_8053AD30->fn_8030A388(getPlayerInputSlot_8030AAF4(player))) {
            if (field_0x8->field_0x26 > lbl_8050DB0C.field_0x4c[36]) {
                field_0x8->field_0x26 = lbl_8050DB0C.field_0x4c[36];
            }
            if (field_0xc->field_0x26 > lbl_8050DB0C.field_0x4c[35] + lbl_8050DB0C.field_0x4c[36]) {
                field_0xc->field_0x26 = lbl_8050DB0C.field_0x4c[35] + lbl_8050DB0C.field_0x4c[36];
            }
            field_0x18 = 11;
        }
    }

    switch (field_0x18) {
    case 1: {
        u32 bits = (lbl_8053AAF8->field_0x83 >> (field_0x14 * 2)) & 3;
        if (bits == 2) {
            if (!lbl_8053AD30->fn_8030A2E4(getPlayerInputSlot_8030AAF4(field_0x14))) {
                break;
            }
        }
        field_0x1c++;
        if (field_0x8->field_0x26 < lbl_8050DB0C.field_0x4c[32] + 2) {
            field_0x8->field_0x26++;
            s32 lim = lbl_8050DB0C.field_0x4c[32] + 2;
            f32 ratio = (f32)(field_0x8->field_0x26 * field_0x8->field_0x26) / (f32)(lim * lim);
            field_0x8->mPane->setAlpha(ratio * (f32)field_0x8->field_0x28);
        }
        if (field_0x1c >= 0x78) {
            field_0x1c = 0;
            field_0x18 = 2;
        }
        break;
    }
    case 2: {
        field_0x1c++;
        s32 lim = lbl_8050DB0C.field_0x4c[32] + 2;
        if (field_0xc->field_0x26 < lim) {
            field_0xc->field_0x26++;
            f32 ratio = (f32)(field_0xc->field_0x26 * field_0xc->field_0x26) / (f32)(lim * lim);
            field_0xc->moveFromCentre_80402E08(0.0f, (480.0f + lbl_8050DB0C.field_0x4c[34]) * ratio - 480.0f);
        }
        if (field_0xc->field_0x26 >= lim) {
            field_0x18 = 3;
        }
        break;
    }
    case 3: {
        field_0x1c++;
        PaneCtrl_804AF86C* q = field_0xc;
        s32 lim = lbl_8050DB0C.field_0x4c[32] + lbl_8050DB0C.field_0x4c[33] + 2;
        if (q->field_0x26 < lim) {
            q->field_0x26++;
            f32 ratio = (f32)((lim - q->field_0x26) * (lim - q->field_0x26))
                / (f32)(lbl_8050DB0C.field_0x4c[33] * lbl_8050DB0C.field_0x4c[33]);
            q->moveFromCentre_80402E08(0.0f, (f32)lbl_8050DB0C.field_0x4c[34] * ratio);
        }
        if (q->field_0x26 >= lim) {
            field_0x18 = 4;
        }
        break;
    }
    case 4:
        field_0x1c++;
        if (field_0x1c >= 0xB4) {
            field_0x1c = 0;
            field_0x18 = 5;
        }
        break;
    case 5:
        field_0x1c++;
        if (field_0x1c >= 0xB4) {
            PaneCtrl_804AF86C* q = field_0xc;
            if (q->field_0x26 > 10) {
                q->field_0x26 = 10;
            }
            field_0x18 = 13;
        }
        break;
    case 6: {
        if (lbl_8053AD30->fn_80309C4C(0x1300, -1)) {
            PaneCtrl_804AF86C* q = field_0xc;
            if (q->field_0x26 > 10) {
                q->field_0x26 = 10;
            }
            field_0x18 = 13;
            break;
        }
        s32 count1 = 0;
        s32 count2 = 0;
        for (s32 i = 0; i < 4; i++) {
            u32 bits = (lbl_8053AAF8->field_0x83 >> (i * 2)) & 3;
            if (bits != 0) {
                count1++;
                if (bits == 1) {
                    if (!lbl_8053AD30->fn_8030A2E4(getPlayerInputSlot_8030AAF4(i))) {
                        count2++;
                    }
                }
                u32 bits2 = (lbl_8053AAF8->field_0x83 >> (i * 2)) & 3;
                if (bits2 == 2) {
                    if (!lbl_8053AD30->fn_8030A364(getPlayerInputSlot_8030AAF4(i))) {
                        count2++;
                    }
                }
            }
        }
        if (count1 == count2) {
            field_0x1c = 0;
            field_0x18 = 5;
        }
        break;
    }
    case 11: {
        PaneCtrl_804AF86C* p = field_0x8;
        PaneCtrl_804AF86C* q = field_0xc;
        s32 sum = lbl_8050DB0C.field_0x4c[35] + lbl_8050DB0C.field_0x4c[36];
        if (p->field_0x26 > 0 && p->field_0x26 >= q->field_0x26) {
            p->field_0x26--;
            f32 ratio = (f32)(p->field_0x26 * p->field_0x26)
                / (f32)(lbl_8050DB0C.field_0x4c[36] * lbl_8050DB0C.field_0x4c[36]);
            p->mPane->setAlpha((s32)(ratio * (f32)p->field_0x28));
        }
        if (q->field_0x26 > 0) {
            q->field_0x26--;
        }
        if (q->field_0x26 > lbl_8050DB0C.field_0x4c[36]) {
            f32 ratio = (f32)((sum - q->field_0x26) * (sum - q->field_0x26))
                / (f32)(lbl_8050DB0C.field_0x4c[35] * lbl_8050DB0C.field_0x4c[35]);
            q->moveFromCentre_80402E08(0.0f, (f32)lbl_8050DB0C.field_0x4c[34] * ratio);
        } else if (q->field_0x26 > 0) {
            f32 ratio = (f32)(q->field_0x26 * q->field_0x26)
                / (f32)(lbl_8050DB0C.field_0x4c[36] * lbl_8050DB0C.field_0x4c[36]);
            q->moveFromCentre_80402E08(0.0f, (480.0f + lbl_8050DB0C.field_0x4c[34]) * ratio - 480.0f);
        } else {
            q->moveFromCentre_80402E08(0.0f, -480.0f);
        }
        if (p->field_0x26 > 0 || q->field_0x26 > 0) {
            break;
        }
        return field_0x10;
    }
    case 13: {
        PaneCtrl_804AF86C* q = field_0xc;
        if (q->field_0x26 > 0) {
            q->field_0x26--;
            f32 ratio = (f32)((10 - q->field_0x26) * (10 - q->field_0x26)) / 100.0f;
            q->moveFromCentre_80402E08(0.0f, 480.0f * ratio);
        }
        if (field_0xc->field_0x26 > 0) {
            break;
        }
        lbl_8053AF80->fn_80458F9C(0);
        lbl_8053AF80->fn_80459274();
        return 1;
    }
    default:
        break;
    }
    return 0x16;
}
