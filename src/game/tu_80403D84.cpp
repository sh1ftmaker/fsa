#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/Message.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/J2D/J2DGrafContext.h"
#include "JSystem/J2D/J2DPicture.h"
#include <string.h>
#include "game/tu_80379F84.h"
#include "game/tu_803B8C74.h"
#include "JSystem/JMath/JMath.h"
#include "game/cls_804B1288.h"
#include "game/act/tu_80440BF4.h"
#include "game/StageFlag.h"
#include "game/cls_804B1B60.h"

// Read by fn_804054B8 and fn_80405D00 (lbzx), and fn_8040B5A0 (lhax).
static const u8 lbl_80543880[8] = {3, 1, 1, 0, 1, 1, 0, 1};
static const u8 lbl_80543888[8] = {1, 3, 3, 2, 3, 3, 2, 3};
static const s16 lbl_80543890[4] = {0, 1, 2, 1};

bool fn_804149C0() {
    return lbl_8050EC80.getCurrentFile().field_0x67 & 1;
}

void fn_8041499C(JUtility::TColor* dst, const JUtility::TColor* src) {
    *dst = *src;
}

void cls_804B1288::fn_80411A94(f32 alpha) {
    field_0x10->search(0x54746C763131ull /* "Ttlv11" */)->setAlpha(field_0x64b * alpha);
    field_0x10->search(0x54746C763132ull /* "Ttlv12" */)->setAlpha(field_0x64c * alpha);
    field_0x10->search(0x54746C763133ull /* "Ttlv13" */)->setAlpha(field_0x64b * (1.0f - alpha));
    field_0x10->search(0x54746C763134ull /* "Ttlv14" */)->setAlpha(field_0x64c * (1.0f - alpha));
}

void cls_804B1288::fn_804111C4() {
    if (field_0x350 == 0x15) {
        fn_8037AA7C();
    }
    gGameRoot2_8053AB10->fn_8013B490();
    field_0x18->setScissor(true);
    field_0x18->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
    lbl_8053AED8->fn_80440EE8();
}

s32 fn_8040F474() {
    return lbl_8053ADF8->field_0x668;
}

void cls_804B1288::fn_8040F3AC() {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            field_0x228[i][j]->fn_80402940(0x14, 0, -150.0f, 0.0f);
        }
    }
    field_0x238->fn_80402940(0x14, 0, -150.0f, 0.0f);
    field_0x23c->fn_80402940(0x14, 0, -150.0f, 0.0f);
    if (field_0x11c->fn_80402940(0x14, 0, 200.0f, 0.0f)) {
        field_0x350 = 0xB;
    }
}

void cls_804B1288::fn_8040F35C() {
    if (field_0x11c->fn_80402940(0x14, 0, 200.0f, 0.0f)) {
        field_0x350 = 0xB;
    }
}

// FIXME: 91.7. The original keeps -250 + 250 * t * t unfused (fmuls, fmuls,
// fadds); ours fuses it into an fmadds.
void cls_804B1288::fn_8040F288() {
    f32 t = (f32)field_0x358 / 90.0f;
    field_0x39c = -250.0f + 250.0f * t * t;
    if (field_0x39c >= 0.0f) {
        field_0x39c = 0.0f;
        field_0x358 = 0;
        if (field_0x65d == 0) {
            for (int i = 0; i < 15; i++) {
                field_0x278[i]->field_0x26 = 0;
                field_0x65d = 1;
            }
        }
        field_0x350 = 0xF;
    }
}

void cls_804B1288::fn_8040E700() {
    if (field_0x65a == 0) {
        field_0x664 = 0;
        field_0x350 = 8;
    }
}

void cls_804B1288::fn_8040E298() {
    field_0x42c++;
    if (field_0x42c > 60) {
        if (field_0x664 != 0) {
            field_0x664 = 2;
            field_0x350 = 6;
        } else if (fn_80404270(field_0x340)) {
            field_0x658 = 1;
            field_0x350 = 0x10;
            if (field_0x65d == 1) {
                for (int i = 0; i < 15; i++) {
                    field_0x278[i]->field_0x26 = 0;
                    field_0x65d = 0;
                }
            }
            field_0x4->fn_804446F0(0);
            gAudioMgr_8053AF80->fn_80458880(0x56, -1);
        } else {
            field_0x350 = 0x15;
        }
    }
}

void cls_804B1288::fn_8040E278() {
    if (++field_0x384 > 100) {
        field_0x350 = 0x15;
    }
}

void cls_804B1288::fn_80409330() {
    fn_8037AA34();
    if (fn_8037AA28() == 0x1A) {
        if (field_0x65d == 0) {
            for (int i = 0; i < 15; i++) {
                field_0x278[i]->field_0x26 = 0;
                field_0x65d = 1;
            }
        }
        field_0x350 = 0xF;
    } else if (fn_8037AA28() == 0x19) {
        field_0x668 = 1;
    }
    if (field_0x663 != 0) {
        field_0x434 += 0x200;
        field_0x436 += 0x400;
        field_0x3a0 = 15.0f * JMath::sincosTable_.sinShort(field_0x434);
        field_0x3a4 = 5.0f * JMath::sincosTable_.sinShort(field_0x436);
    }
}

BOOL cls_804B1288::fn_804045F8(s32 bit) {
    if (gGameRoot_8053AAF8->field_0x7e) {
        return (lbl_8050EC80.getCurrentFile().field_0x5a[0] & (u8)(1 << bit)) ? TRUE : FALSE;
    }
    return FALSE;
}

// Defined above fn_804045F8 although its address is lower: it inlines fn_804045F8,
// and only this order compiles to the original's .text order (tools/order_check.py).
bool cls_804B1288::fn_80404270(s32 stage) {
    if (lbl_80529DEC.field_0x23e[1] == 0 && gGameRoot_8053AAF8->field_0x7e && field_0x656 == 0 &&
        !fn_804045F8(stage))
    {
        if ((stage == 0 && isStageFlagSet(stage, 1)) || (stage == 1 && isStageFlagSet(stage, 1)) ||
            (stage == 2 && isStageFlagSet(stage, 1)) || (stage == 3 && isStageFlagSet(stage, 1)) ||
            (stage == 4 && isStageFlagSet(stage, 1)) || (stage == 5 && isStageFlagSet(stage, 1)) ||
            (stage == 6 && isStageFlagSet(stage, 1)) || (stage == 7 && isStageFlagSet(stage, 2)))
        {
            return true;
        }
    }
    return false;
}

void cls_804B1288::fn_80403F50() {
    if (field_0x65a) {
        for (int i = 0; i < 12; i++) {
            field_0x240[i]->field_0x22 = 0;
        }
        fn_803837A4(field_0x5dc, field_0x390, 0, 0, field_0x270[0]->field_0x18, 1.0f);
        for (s32 i = 0; i < 2; i++) {
            strcpy(((J2DTextBox*)field_0x270[i]->mPane)->getStringPtr(), field_0x5dc);
        }
        for (s32 i = 0; i < 12; i++) {
            field_0x240[i]->moveFromCentre_80402E08(0.0f, 100.0f);
        }
        field_0x659 = 1;
    }
}

void cls_804B1288::fn_80403EAC() {
    u8 done = field_0x240[0]->fn_80402940(0x13, 0, 100.0f, 0.0f);
    for (s32 i = 1; i < 12; i++) {
        field_0x240[i]->fn_80402940(0x13, 0, 100.0f, 0.0f);
    }
    if (done) {
        field_0x42a = 100;
        field_0x659 = 2;
    }
}

void cls_804B1288::fn_80403E24() {
    field_0x42a--;
    if (field_0x42a <= 0) {
        for (int i = 0; i < 12; i++) {
            field_0x240[i]->field_0x22 = 0;
        }
        field_0x659 = 3;
    }
}

void cls_804B1288::fn_80403D84() {
    u8 done = field_0x240[0]->fn_80402940(0xB, 0, 0.0f, 100.0f);
    for (s32 i = 1; i < 12; i++) {
        field_0x240[i]->fn_80402940(0xB, 0, 0.0f, 100.0f);
    }
    if (done) {
        field_0x65a = 0;
        field_0x659 = 0;
    }
}

// Slides field_0x2b4 from 200 above its place down to it and fades it in, eased by
// (frame / frames)^2. Dead-stripped: fn_804108B4 inlines it twice (sliding in and
// out); its own copy created the unit's first .sdata2 literals (0.0f, -200.0f,
// 200.0f and the two int-to-float constants) before fn_80403D84.
void cls_804B1288::slidePane(s16 frame, s16 frames) {
    f32 t = (f32)(frame * frame) / (f32)(frames * frames);
    field_0x2b4->moveFromCentre_80402E08(0.0f, -200.0f + 200.0f * t);
    field_0x2b4->mPane->setAlpha(t * field_0x2b4->field_0x28);
}
