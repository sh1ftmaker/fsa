#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/Message.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/J2D/J2DGrafContext.h"
#include "JSystem/J2D/J2DPicture.h"
#include <string.h>
#include "game/tu_80379F84.h"
#include "JSystem/JMath/JMath.h"
#include "game/cls_804B1288.h"

// The pane names fn_80408F38 looks up (8 characters, right-aligned); 17 of them.
static const u64 lbl_8048C5D0[17] = {
    0x0050656E6261636Bull, // "Penback"
    0x0000000050736B79ull, // "Psky"
    0x000050636C6F3034ull, // "Pclo04"
    0x000050636C6F3033ull, // "Pclo03"
    0x000050636C6F3032ull, // "Pclo02"
    0x0000506D61706361ull, // "Pmapca"
    0x00506D6170746F75ull, // "Pmaptou"
    0x000050636C6F3031ull, // "Pclo01"
    0x50656E6261636B31ull, // "Penback1"
    0x00506C6963753031ull, // "Plicu01"
    0x00506C6963753032ull, // "Plicu02"
    0x00506C6963753037ull, // "Plicu07"
    0x00506C6963753038ull, // "Plicu08"
    0x00506C6963753033ull, // "Plicu03"
    0x00506C6963753034ull, // "Plicu04"
    0x00506C6963753035ull, // "Plicu05"
    0x00506C6963753036ull, // "Plicu06"
};

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

void cls_804B1288::fn_80403E24() {
    field_0x42a--;
    if (field_0x42a <= 0) {
        for (int i = 0; i < 12; i++) {
            field_0x240[i]->field_0x22 = 0;
        }
        field_0x659 = 3;
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
            field_0x240[i]->fn_80402E08(0.0f, 100.0f);
        }
        field_0x659 = 1;
    }
}

bool cls_804B1288::fn_804045F8(s32 bit) {
    if (lbl_8053AAF8->field_0x7e) {
        return lbl_8050EC80[lbl_8050EC80[0].field_0x10].field_0x7a[0] & (u8)(1 << bit);
    }
    return false;
}

void cls_804B1288::fn_80408F38(f32 alpha) {
    for (int i = 0; i < 9; i++) {
        field_0x14->search(lbl_8048C5D0[i])->setAlpha(field_0x642[i] * alpha);
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

void cls_804B1288::fn_8040E278() {
    if (++field_0x384 > 100) {
        field_0x350 = 0x15;
    }
}

void cls_804B1288::fn_8040E700() {
    if (field_0x65a == 0) {
        field_0x664 = 0;
        field_0x350 = 8;
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

void cls_804B1288::fn_8040F35C() {
    if (field_0x11c->fn_80402940(0x14, 0, 200.0f, 0.0f)) {
        field_0x350 = 0xB;
    }
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

s32 fn_8040F474() {
    return lbl_8053ADF8->field_0x668;
}

void fn_8041499C(JUtility::TColor* dst, const JUtility::TColor* src) {
    *dst = *src;
}

bool fn_804149C0() {
    return lbl_8050EC80[lbl_8050EC80[0].field_0x10].field_0x87 & 1;
}

void cls_804B1288::fn_80411A94(f32 alpha) {
    field_0x10->search(0x54746C763131ull /* "Ttlv11" */)->setAlpha(field_0x64b * alpha);
    field_0x10->search(0x54746C763132ull /* "Ttlv12" */)->setAlpha(field_0x64c * alpha);
    field_0x10->search(0x54746C763133ull /* "Ttlv13" */)->setAlpha(field_0x64b * (1.0f - alpha));
    field_0x10->search(0x54746C763134ull /* "Ttlv14" */)->setAlpha(field_0x64c * (1.0f - alpha));
}
