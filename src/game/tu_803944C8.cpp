#include <stdio.h>
#include <string.h>
#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/GameView.h"
#include "game/Message.h"
#include "game/tu_803944C8.h"
#include "game/tu_803B8C74.h"
#include "game/act/act_small_key.h"
#include "JSystem/JMessage/TControl.h"
#include "JSystem/J2D/J2DPane.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/J2D/J2DPicture.h"
#include "JSystem/J2D/J2DGrafContext.h"
#include "JSystem/JMessage/TResource.h"
#include "game/cls_804B20F8.h"
#include "JSystem/JMessage/TParse.h"
#include "JSystem/JMessage/TControl.h"

// The window frames' pane tags: 'PNar' per player (fn_80395BE8, fn_80398700).
static const u64 lbl_80475768[5] = {'P1ar', 'P2ar', 'P3ar', 'P4ar', 'P5ar'};
// Per player the 'PNb02', 'PNb01', 'PNn02', 'PNn01' panes (fn_80395BE8); player 5 has no 'P5n02'.
static const u64 lbl_80475790[5][4] = {
    {'P1b02', 'P1b01', 'P1n02', 'P1n01'},
    {'P2b02', 'P2b01', 'P2n02', 'P2n01'},
    {'P3b02', 'P3b01', 'P3n02', 'P3n01'},
    {'P4b02', 'P4b01', 'P4n02', 'P4n01'},
    {'P5b02', 'P5b01', 0, 'P5n01'},
};
// Shift-JIS character code -> glyph offset in the text-image font (32-pixel
// cells: column * 0x20, row * 0x400); ends with 0xFFFF / 0xFFFF.
struct SjisGlyph_80475830 {
    u16 code;
    u16 glyph;
};
static const SjisGlyph_80475830 lbl_80475830[317] = {
    {0x82A0, 0x0000}, {0x82A2, 0x0020}, {0x82A4, 0x0040}, {0x82A6, 0x0060}, {0x82A8, 0x0080}, {0x82A9, 0x00A0},
    {0x82AB, 0x00C0}, {0x82AD, 0x00E0}, {0x82AF, 0x0100}, {0x82B1, 0x0120}, {0x82B3, 0x0140}, {0x82B5, 0x0160},
    {0x82B7, 0x0180}, {0x82B9, 0x01A0}, {0x82BB, 0x01C0}, {0x82BD, 0x01E0}, {0x82BF, 0x0400}, {0x82C2, 0x0420},
    {0x82C4, 0x0440}, {0x82C6, 0x0460}, {0x82C8, 0x0480}, {0x82C9, 0x04A0}, {0x82CA, 0x04C0}, {0x82CB, 0x04E0},
    {0x82CC, 0x0500}, {0x82CD, 0x0520}, {0x82D0, 0x0540}, {0x82D3, 0x0560}, {0x82D6, 0x0580}, {0x82D9, 0x05A0},
    {0x82DC, 0x05C0}, {0x82DD, 0x05E0}, {0x82DE, 0x0800}, {0x82DF, 0x0820}, {0x82E0, 0x0840}, {0x82E7, 0x0860},
    {0x82E8, 0x0880}, {0x82E9, 0x08A0}, {0x82EA, 0x08C0}, {0x82EB, 0x08E0}, {0x82E2, 0x0900}, {0x82E4, 0x0920},
    {0x82E6, 0x0940}, {0x82ED, 0x0960}, {0x82F0, 0x0980}, {0x82F1, 0x09A0}, {0x82AA, 0x09C0}, {0x82AC, 0x09E0},
    {0x82AE, 0x0C00}, {0x82B0, 0x0C20}, {0x82B2, 0x0C40}, {0x82B4, 0x0C60}, {0x82B6, 0x0C80}, {0x82B8, 0x0CA0},
    {0x82BA, 0x0CC0}, {0x82BC, 0x0CE0}, {0x82BE, 0x0D00}, {0x82C0, 0x0D20}, {0x82C3, 0x0D40}, {0x82C5, 0x0D60},
    {0x82C7, 0x0D80}, {0x82CE, 0x0DA0}, {0x82D1, 0x0DC0}, {0x82D4, 0x0DE0}, {0x82D7, 0x1000}, {0x82DA, 0x1020},
    {0x82CF, 0x1040}, {0x82D2, 0x1060}, {0x82D5, 0x1080}, {0x82D8, 0x10A0}, {0x82DB, 0x10C0}, {0x82E1, 0x10E0},
    {0x82E3, 0x1100}, {0x82E5, 0x1120}, {0x82C1, 0x1140}, {0x829F, 0x1160}, {0x82A1, 0x1180}, {0x82A3, 0x11A0},
    {0x82A5, 0x11C0}, {0x82A7, 0x11E0}, {0x8394, 0x1400}, {0x8140, 0x1420}, {0x8141, 0x1440}, {0x8142, 0x1460},
    {0x8163, 0x1480}, {0x8160, 0x14A0}, {0x817C, 0x14C0}, {0x81AA, 0x14E0}, {0x81AB, 0x1500}, {0x81A8, 0x1520},
    {0x81A9, 0x1540}, {0x8184, 0x1560}, {0x8175, 0x1580}, {0x8176, 0x15A0}, {0x8148, 0x15C0}, {0x8149, 0x15E0},
    {0x8341, 0x1800}, {0x8343, 0x1820}, {0x8345, 0x1840}, {0x8347, 0x1860}, {0x8349, 0x1880}, {0x834A, 0x18A0},
    {0x834C, 0x18C0}, {0x834E, 0x18E0}, {0x8350, 0x1900}, {0x8352, 0x1920}, {0x8354, 0x1940}, {0x8356, 0x1960},
    {0x8358, 0x1980}, {0x835A, 0x19A0}, {0x835C, 0x19C0}, {0x835E, 0x19E0}, {0x8360, 0x1C00}, {0x8363, 0x1C20},
    {0x8365, 0x1C40}, {0x8367, 0x1C60}, {0x8369, 0x1C80}, {0x836A, 0x1CA0}, {0x836B, 0x1CC0}, {0x836C, 0x1CE0},
    {0x836D, 0x1D00}, {0x836E, 0x1D20}, {0x8371, 0x1D40}, {0x8374, 0x1D60}, {0x8377, 0x1D80}, {0x837A, 0x1DA0},
    {0x837D, 0x1DC0}, {0x837E, 0x1DE0}, {0x8380, 0x2000}, {0x8381, 0x2020}, {0x8382, 0x2040}, {0x8389, 0x2060},
    {0x838A, 0x2080}, {0x838B, 0x20A0}, {0x838C, 0x20C0}, {0x838D, 0x20E0}, {0x8384, 0x2100}, {0x8386, 0x2120},
    {0x8388, 0x2140}, {0x838F, 0x2160}, {0x8392, 0x2180}, {0x8393, 0x21A0}, {0x834B, 0x21C0}, {0x834D, 0x21E0},
    {0x834F, 0x2400}, {0x8351, 0x2420}, {0x8353, 0x2440}, {0x8355, 0x2460}, {0x8357, 0x2480}, {0x8359, 0x24A0},
    {0x835B, 0x24C0}, {0x835D, 0x24E0}, {0x835F, 0x2500}, {0x8361, 0x2520}, {0x8364, 0x2540}, {0x8366, 0x2560},
    {0x8368, 0x2580}, {0x836F, 0x25A0}, {0x8372, 0x25C0}, {0x8375, 0x25E0}, {0x8378, 0x2800}, {0x837B, 0x2820},
    {0x8370, 0x2840}, {0x8373, 0x2860}, {0x8376, 0x2880}, {0x8379, 0x28A0}, {0x837C, 0x28C0}, {0x8383, 0x28E0},
    {0x8385, 0x2900}, {0x8387, 0x2920}, {0x8362, 0x2940}, {0x8340, 0x2960}, {0x8342, 0x2980}, {0x8344, 0x29A0},
    {0x8346, 0x29C0}, {0x8348, 0x29E0}, {0x824F, 0x2C00}, {0x8250, 0x2C20}, {0x8251, 0x2C40}, {0x8252, 0x2C60},
    {0x8253, 0x2C80}, {0x8254, 0x2CA0}, {0x8255, 0x2CC0}, {0x8256, 0x2CE0}, {0x8257, 0x2D00}, {0x8258, 0x2D20},
    {0x8260, 0x2D40}, {0x8261, 0x2D60}, {0x8262, 0x2D80}, {0x8263, 0x2DA0}, {0x8264, 0x2DC0}, {0x8265, 0x2DE0},
    {0x8266, 0x3000}, {0x8267, 0x3020}, {0x8268, 0x3040}, {0x8269, 0x3060}, {0x826A, 0x3080}, {0x826B, 0x30A0},
    {0x826C, 0x30C0}, {0x826D, 0x30E0}, {0x826E, 0x3100}, {0x826F, 0x3120}, {0x8270, 0x3140}, {0x8271, 0x3160},
    {0x8272, 0x3180}, {0x8273, 0x31A0}, {0x8274, 0x31C0}, {0x8275, 0x31E0}, {0x8276, 0x3400}, {0x8277, 0x3420},
    {0x8278, 0x3440}, {0x8279, 0x3460}, {0x8281, 0x3480}, {0x8282, 0x34A0}, {0x8283, 0x34C0}, {0x8284, 0x34E0},
    {0x8285, 0x3500}, {0x8286, 0x3520}, {0x8287, 0x3540}, {0x8288, 0x3560}, {0x8289, 0x3580}, {0x828A, 0x35A0},
    {0x828B, 0x35C0}, {0x828C, 0x35E0}, {0x828D, 0x3800}, {0x828E, 0x3820}, {0x828F, 0x3840}, {0x8290, 0x3860},
    {0x8291, 0x3880}, {0x8292, 0x38A0}, {0x8293, 0x38C0}, {0x8294, 0x38E0}, {0x8295, 0x3900}, {0x8296, 0x3920},
    {0x8297, 0x3940}, {0x8298, 0x3960}, {0x8299, 0x3980}, {0x829A, 0x39A0}, {0x8144, 0x39C0}, {0x8166, 0x39E0},
    {0x815B, 0x14C0}, {0x0020, 0x1420}, {0x002C, 0x1440}, {0x002D, 0x14C0}, {0x003F, 0x15C0}, {0x0021, 0x15E0},
    {0x0030, 0x2C00}, {0x0031, 0x2C20}, {0x0032, 0x2C40}, {0x0033, 0x2C60}, {0x0034, 0x2C80}, {0x0035, 0x2CA0},
    {0x0036, 0x2CC0}, {0x0037, 0x2CE0}, {0x0038, 0x2D00}, {0x0039, 0x2D20}, {0x0041, 0x2D40}, {0x0042, 0x2D60},
    {0x0043, 0x2D80}, {0x0044, 0x2DA0}, {0x0045, 0x2DC0}, {0x0046, 0x2DE0}, {0x0047, 0x3000}, {0x0048, 0x3020},
    {0x0049, 0x3040}, {0x004A, 0x3060}, {0x004B, 0x3080}, {0x004C, 0x30A0}, {0x004D, 0x30C0}, {0x004E, 0x30E0},
    {0x004F, 0x3100}, {0x0050, 0x3120}, {0x0051, 0x3140}, {0x0052, 0x3160}, {0x0053, 0x3180}, {0x0054, 0x31A0},
    {0x0055, 0x31C0}, {0x0056, 0x31E0}, {0x0057, 0x3400}, {0x0058, 0x3420}, {0x0059, 0x3440}, {0x005A, 0x3460},
    {0x0061, 0x3480}, {0x0062, 0x34A0}, {0x0063, 0x34C0}, {0x0064, 0x34E0}, {0x0065, 0x3500}, {0x0066, 0x3520},
    {0x0067, 0x3540}, {0x0068, 0x3560}, {0x0069, 0x3580}, {0x006A, 0x35A0}, {0x006B, 0x35C0}, {0x006C, 0x35E0},
    {0x006D, 0x3800}, {0x006E, 0x3820}, {0x006F, 0x3840}, {0x0070, 0x3860}, {0x0071, 0x3880}, {0x0072, 0x38A0},
    {0x0073, 0x38C0}, {0x0074, 0x38E0}, {0x0075, 0x3900}, {0x0076, 0x3920}, {0x0077, 0x3940}, {0x0078, 0x3960},
    {0x0079, 0x3980}, {0x007A, 0x39A0}, {0x002E, 0x39C0}, {0x0027, 0x39E0}, {0x002B, 0x0000}, {0x0028, 0x0020},
    {0x0029, 0x0040}, {0x003A, 0x0060}, {0x0025, 0x0080}, {0x0022, 0x3CA0}, {0xFFFF, 0xFFFF},
};
// The buffers fn_8039F540 / fn_8039C9A0 format the colour code "\033CR[%d]" into.
static char lbl_8050E518[16];
static char lbl_8050E528[16];

// The glyph of a character in the text-image font (the space's when it has none).
static inline u16 findGlyph(u16 code) {
    for (int i = 0; lbl_80475830[i].code != 0xFFFF; i++) {
        if (code == lbl_80475830[i].code) {
            return lbl_80475830[i].glyph;
        }
    }
    return 0x1420;
}

static const u8 lbl_80475D24[32] = {
    0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x13, 0x11, 0x10, 0x12, 0x14, 0x16,
    0x19, 0x25, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x00, 0x00,
};
static const u8 lbl_80475D44[32] = {
    0x49, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x13, 0x11, 0x10, 0x12, 0x14, 0x16,
    0x19, 0x25, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x00, 0x00,
};
static const GXColor lbl_80475D64[5] = {{0xFF, 0xFF, 0xFF, 0xFF}, {0x64, 0xFF, 0x00, 0xFF}, {0xFA, 0x50, 0x00, 0xFF}, {0x32, 0xD2, 0xFF, 0xFF}, {0xF0, 0x64, 0xFF, 0xFF}};
static const f32 lbl_80475D78[5] = {1.0f, 1.3f, 0.8f, 1.2f, 1.0f};
static const f32 lbl_80475D8C[5] = {1.0f, 0.3f, 1.1f, 0.8f, 1.0f};
static const f32 lbl_80475DA0[5] = {1.0f, 1.3f, 0.8f, 1.2f, 1.0f};
static const f32 lbl_80475DB4[5] = {1.0f, 0.3f, 1.1f, 0.8f, 1.0f};

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

// FIXME: 99.34, kept out of the build (merge rule: the rows differ in opcode,
// `mr r8,r7` vs `li r8,1`). The original keeps -1 in r0 and 1 in r7 (ours the
// other way round), so its minutes fallback copies r7 where ours loads li r8, 1.
// Lays the message out once with field_0x38 (counting lines and pages), then
// starts the window's page state.
#ifdef NONMATCHING
void cls_804AF72C::do_begin(const void* entry, const char* text) {
    field_0x2d8->resetFields0x3ab();
    *field_0x2c0 = 0;
    *field_0x2c4 = 0;
    field_0x2cc = 0;
    field_0x2d0 = 0;
    field_0x2d4 = 0;
    field_0x2d5 = 0;
    for (int i = 0; i < 3; i++) {
        field_0x2d8->field_0x3b2[i] = 0;
    }
    field_0x2d8->field_0x154 = -1;
    field_0x2d6 = 1;
    field_0x38.field_0x38 = field_0x2bc;
    field_0x38.setFields0x1e0(field_0x2d8->fn_803A3068(), field_0x2d8->fn_803A3060(), field_0x2d8->fn_803A3058(),
                              field_0x2d8->fn_803A3050());
    s32 count = lbl_8053AD78->field_0x44;
    s32 minutes = count / 60 % 60;
    if (minutes == 0) {
        minutes = 1;
    }
    field_0x38.field_0x1d8 = count / 3600;
    field_0x38.field_0x1dc = minutes;
    field_0x38.setBegin_messageEntryText(getResourceCache(), entry, text);
    field_0x38.process(NULL);
    field_0x2d8->field_0x164 =
        field_0x38.field_0x202[field_0x2d8->field_0x15c + 1] - field_0x38.field_0x202[field_0x2d8->field_0x15c];
    field_0x2d8->field_0x170 = 0;
    if (field_0x38.field_0x257[field_0x2d8->field_0x15c] != 0) {
        field_0x2d8->field_0x19e = 0;
    }
    cls_804AF5A0* window = field_0x2d8;
    if (window->field_0x1aa == 0) {
        for (int i = 0; i < 12; i++) {
            window->field_0x68[i]->field_0x8 = 0xFF;
        }
    } else {
        field_0x2c0[field_0x2d0++] = -1;
        field_0x2c0[field_0x2d0++] = -1;
        if (field_0x2d8->field_0x164 < 3) {
            field_0x2c0[field_0x2d0++] = -1;
            field_0x2c0[field_0x2d0++] = field_0x2d8->field_0x164 + 0xC0;
        }
    }
}
#endif

void cls_804AF72C::do_end() {
    if (field_0x2d8->field_0x1aa == 0) {
        return;
    }
    field_0x2c0[field_0x2d0++] = -1;
    field_0x2c0[field_0x2d0++] = -2;
}

// FIXME(shotgun): 99.03 %, different (the text pointer is formed before the clearing store)
void cls_804AF72C::fn_8039F540(int c) {
    char* text = field_0x2d8->field_0x3ab;
    field_0x2d8->field_0x3ab[0] = 0;
    if (field_0x2d8->field_0x1aa == 0) {
        char newline[2];
        newline[1] = 0;
        if (c == '\n') {
            field_0x2cc++;
            if (field_0x2cc > (s8)field_0x38.field_0x202[field_0x2d8->field_0x15c] + 1) {
                field_0x2d6 = 1;
                newline[0] = c;
                strcat(field_0x2c0, newline);
                strcat(field_0x2c4, newline);
                field_0x2d0++;
            }
        }
        if (field_0x2cc > (s8)field_0x38.field_0x202[field_0x2d8->field_0x15c]) {
            if (field_0x2d6 != 0) {
                s32 index = field_0x2cc;
                cls_804AF5A0* window = field_0x2d8;
                if (!field_0x38.isMasked(index) && !(window->field_0x16c != 0 && field_0x38.isMasked(index))) {
                    f32 base;
                    f32 pos;
                    if (!(window->field_0x3b8->field_0x28 & 0x40)) {
                        base = window->field_0xe8;
                        pos = field_0x38.field_0x4c[index];
                    } else {
                        base = window->field_0xf0;
                        pos = field_0x38.field_0x4c[index] + (field_0x38.field_0xf4[index] - 3.0f);
                    }
                    s32 offset = (s32)((base - pos) * 0.5f + 0.5f);
                    if (offset > 0) {
                        snprintf(lbl_8050E518, 15, "\033CR[%d]", offset);
                        strcat(field_0x2c0, lbl_8050E518);
                        strcat(field_0x2c4, lbl_8050E518);
                        field_0x2d0 += strlen(lbl_8050E518);
                    }
                }
                field_0x2d6 = 0;
            }
            if (c == '\n') {
                return;
            }
            s32 length = 0;
            s8 high = c >> 8;
            if (high != 0) {
                field_0x2d0++;
                c &= 0xFF;
                length = 1;
                text[0] = high;
            }
            field_0x2d0++;
            text[length] = c;
            s32 shown = field_0x2d0 - (length + 1);
            if (shown != field_0x2d8->field_0x170) {
                field_0x2d8->field_0x170 = shown;
            }
            strcat(field_0x2c0, text);
            strcat(field_0x2c4, text);
        }
    } else if (c == '\n') {
        field_0x2cc++;
        if (field_0x2cc > (s8)field_0x38.field_0x202[field_0x2d8->field_0x15c] + 1) {
            field_0x2c0[field_0x2d0++] = -1;
            field_0x2c0[field_0x2d0++] = -0x10;
            field_0x2d6 = 1;
        }
    } else if (field_0x2cc > (s8)field_0x38.field_0x202[field_0x2d8->field_0x15c]) {
        if (field_0x2d6 != 0) {
            s32 index = field_0x2cc;
            if (!field_0x38.isMasked(index) && !(field_0x2d8->field_0x16c != 0 && field_0x38.isMasked(index))) {
                s32 offset = 0x1C - (s8)field_0x38.field_0x22d[index];
                if (offset > 0 && offset < 0x1C) {
                    field_0x2c0[field_0x2d0++] = -1;
                    field_0x2c0[field_0x2d0++] = offset + 0xD0;
                }
            }
            field_0x2d6 = 0;
        }
        appendGlyph(findGlyph(c));
    }
}

// FIXME(shotgun): 96.37 %, missing/extra (the u8 line count is subtracted as an int in the original)
void cls_804AF72C::fn_8039D30C(u8 value) {
    s32 index = field_0x2cc;
    s32 offset = 0;
    if (!field_0x38.isMasked(index) && !(field_0x2d8->field_0x16c != 0 && field_0x38.isMasked(index))) {
        offset = (0x1C - (s8)field_0x38.field_0x22d[index]) / 2;
    }
    switch (field_0x2d4) {
    case 0: {
        field_0x2c0[field_0x2d0++] = -2;
        field_0x2c0[field_0x2d0++] = offset + (s8)field_0x38.field_0x1fe[field_0x2d4];
        field_0x2c0[field_0x2d0++] = -2;
        cls_804AF5A0* window = field_0x2d8;
        if (window->field_0x1a1 != 0) {
            u8 lines = field_0x38.field_0x1ee[field_0x2d4];
            s8 first = field_0x38.field_0x202[window->field_0x15c];
            if (lines - first == 1) {
                field_0x2c0[field_0x2d0++] = lines + 0x20 - first;
                break;
            }
        }
        field_0x2c0[field_0x2d0++] = field_0x38.field_0x1ee[field_0x2d4] + 0x1f - (s8)field_0x38.field_0x202[window->field_0x15c];
        break;
    }
    case 1: {
        field_0x2c0[field_0x2d0++] = -2;
        field_0x2c0[field_0x2d0++] = offset + (s8)field_0x38.field_0x1fe[field_0x2d4] + 0x40;
        field_0x2c0[field_0x2d0++] = -2;
        cls_804AF5A0* window = field_0x2d8;
        if (window->field_0x1a1 != 0) {
            u8 lines = field_0x38.field_0x1ee[field_0x2d4];
            s8 first = field_0x38.field_0x202[window->field_0x15c];
            if (lines - first == 1) {
                field_0x2c0[field_0x2d0++] = lines + 0x60 - first;
                break;
            }
        }
        field_0x2c0[field_0x2d0++] = field_0x38.field_0x1ee[field_0x2d4] + 0x5f - (s8)field_0x38.field_0x202[window->field_0x15c];
        break;
    }
    case 2: {
        field_0x2c0[field_0x2d0++] = -2;
        field_0x2c0[field_0x2d0++] = offset + (s8)field_0x38.field_0x1fe[field_0x2d4] + 0x80;
        field_0x2c0[field_0x2d0++] = -2;
        cls_804AF5A0* window = field_0x2d8;
        if (window->field_0x1a1 != 0) {
            u8 lines = field_0x38.field_0x1ee[field_0x2d4];
            s8 first = field_0x38.field_0x202[window->field_0x15c];
            if (lines - first == 1) {
                field_0x2c0[field_0x2d0++] = lines + 0xa0 - first;
                break;
            }
        }
        field_0x2c0[field_0x2d0++] = field_0x38.field_0x1ee[field_0x2d4] + 0x9f - (s8)field_0x38.field_0x202[window->field_0x15c];
        break;
    }
    }
    field_0x2d8->field_0x3b2[field_0x2d4] = value;
    field_0x2d4++;
}

// FIXME(shotgun): 82.41 %, different (schedule of the window load / int conversion)
void cls_804AF72C::fn_8039D0DC(u8 value) {
    s32 offset = 0;
    s32 index = field_0x2cc;
    if (!field_0x38.isMasked(index) && !(field_0x2d8->field_0x16c != 0 && field_0x38.isMasked(index))) {
        f32 base;
        f32 pos;
        if (!(field_0x2d8->field_0x3b8->field_0x28 & 0x40)) {
            base = field_0x2d8->field_0xe8;
            pos = field_0x38.field_0x4c[index];
        } else {
            base = field_0x2d8->field_0xf0;
            pos = field_0x38.field_0x4c[index] + (field_0x38.field_0xf4[index] - 3.0f);
        }
        offset = (s32)((base - pos) * 0.5f + 0.5f);
    }
    cls_804AF5A0* window = field_0x2d8;
    u8 slot = field_0x2d4;
    window->field_0x124[slot] = (f32)offset + field_0x38.field_0x19c[slot];
    window->field_0x130[slot] = (f32)(field_0x38.field_0x1ee[slot] - field_0x38.field_0x202[window->field_0x15c] - 1);
    field_0x2d8->field_0x3b2[field_0x2d4] = value;
    field_0x2d4++;
}

// FIXME(shotgun): 98.29 %, scheduling+regalloc
void cls_804AF72C::fn_8039CDF8(u8 value) {
    if (field_0x2cc > (s8)field_0x38.field_0x202[field_0x2d8->field_0x15c]) {
        strcat(field_0x2c0, "  ");
        strcat(field_0x2c4, "  ");
        field_0x2d0 += 4;
    } else {
        field_0x2d5++;
        return;
    }
    s32 index = field_0x2cc;
    s32 offset = 0;
    if (!field_0x38.isMasked(index) && !(field_0x2d8->field_0x16c != 0 && field_0x38.isMasked(index))) {
        f32 base;
        f32 pos;
        if (!(field_0x2d8->field_0x3b8->field_0x28 & 0x40)) {
            base = field_0x2d8->field_0xe8;
            pos = field_0x38.field_0x4c[index];
        } else {
            base = field_0x2d8->field_0xf0;
            pos = field_0x38.field_0x4c[index] + (field_0x38.field_0xf4[index] - 3.0f);
        }
        offset = (s32)((base - pos) * 0.5f + 0.5f);
    }
    cls_804AF5A0* window = field_0x2d8;
    u8 slot = field_0x2d5;
    f32 x = (f32)offset + field_0x38.field_0x19c[slot + 3];
    f32 y = (f32)(field_0x38.field_0x1ee[slot + 3] - (s8)field_0x38.field_0x202[window->field_0x15c] - 1);
    for (int i = 0; i < 12; i++) {
        if (window->field_0x68[i]->field_0x8 == 0xFF) {
            window->field_0x68[i]->field_0x0 = x;
            window->field_0x68[i]->field_0x4 = y;
            window->field_0x68[i]->field_0x8 = value;
            break;
        }
    }
    field_0x2d5++;
}

// FIXME(shotgun): 99.55 %, regalloc-only
void cls_804AF72C::fn_8039C9A0(s32 number) {
    bool started = false;
    bool negative = false;
    if (number < 0) {
        number *= -1;
        negative = true;
    }
    if (field_0x2d6 != 0) {
        s32 index = field_0x2cc;
        if (!field_0x38.isMasked(index)) {
            if (!(field_0x2d8->field_0x16c != 0 && field_0x38.isMasked(index))) {
                f32 base;
                f32 pos;
                if (!(field_0x2d8->field_0x3b8->field_0x28 & 0x40)) {
                    base = field_0x2d8->field_0xe8;
                    pos = field_0x38.field_0x4c[index];
                } else {
                    base = field_0x2d8->field_0xf0;
                    pos = field_0x38.field_0x4c[index] + (field_0x38.field_0xf4[index] - 3.0f);
                }
                s32 offset = (s32)((base - pos) * 0.5f + 0.5f);
                if (offset > 0) {
                    snprintf(lbl_8050E528, 15, "\033CR[%d]", offset);
                    strcat(field_0x2c0, lbl_8050E528);
                    strcat(field_0x2c4, lbl_8050E528);
                    field_0x2d0 += strlen(lbl_8050E528);
                }
            }
            field_0x2d6 = 0;
        }
    }
    char ch[3];
    ch[2] = 0;
    ch[1] = 0;
    ch[0] = 0;
    if (negative) {
        ch[0] = '-';
        appendText(ch);
    }
    s32 digit = number / 10000;
    number %= 10000;
    if (digit != 0) {
        ch[0] = digit + '0';
        started = true;
        appendText(ch);
    }
    digit = number / 1000;
    number %= 1000;
    if (digit != 0 || started) {
        ch[0] = digit + '0';
        started = true;
        appendText(ch);
    }
    digit = number / 100;
    number %= 100;
    if (digit != 0 || started) {
        ch[0] = digit + '0';
        started = true;
        appendText(ch);
    }
    digit = number / 10;
    number %= 10;
    if (digit != 0 || started) {
        ch[0] = digit + '0';
        appendText(ch);
    }
    ch[0] = number + '0';
    appendText(ch);
}

// FIXME(shotgun): 99.11 %, reloc-name
void cls_804AF72C::fn_8039C630(u8 noun, s32 count) {
    switch (noun) {
    case 1:
        if (count == 1) {
            appendText(" Force Gem", 10);
        } else {
            appendText(" Force Gems", 11);
        }
        break;
    case 2:
        if (count == 1) {
            appendText(" Cucco", 6);
        } else {
            appendText(" Cuccos", 6);
        }
        break;
    case 3:
        if (count == 1) {
            appendText(" baddy", 6);
        } else {
            appendText(" baddies", 8);
        }
        break;
    case 4:
        if (count == 1) {
            appendText(" time", 5);
        } else {
            appendText(" times", 6);
        }
        break;
    case 5:
        if (count == 1) {
            appendText(" system", 7);
        } else {
            appendText(" systems", 8);
        }
        break;
    case 6:
        if (count == 1) {
            appendText(" thief", 6);
        } else {
            appendText(" thieves", 8);
        }
        break;
    case 7:
        if (count == 1) {
            appendText(" mole", 5);
        } else {
            appendText(" moles", 6);
        }
        break;
    case 8:
        if (count == 1) {
            appendText(" return", 7);
        } else {
            appendText(" returns", 8);
        }
        break;
    case 9:
        if (count == 1) {
            appendText(" Force Fairy", 7);
        } else {
            appendText(" Force Fairies", 8);
        }
        break;
    }
}

// FIXME(shotgun): 97.05 %, scheduling+regalloc
void cls_804AF72C::fn_8039C258(s32 number) {
    bool started = false;
    if (number < 0) {
        number *= -1;
        appendGlyph(findGlyph('-'));
    }
    s32 digit = number / 10000;
    number %= 10000;
    if (digit != 0) {
        started = true;
        appendGlyph(findGlyph(digit + '0'));
    }
    digit = number / 1000;
    number %= 1000;
    if (digit != 0 || started) {
        started = true;
        appendGlyph(findGlyph(digit + '0'));
    }
    digit = number / 100;
    number %= 100;
    if (digit != 0 || started) {
        started = true;
        appendGlyph(findGlyph(digit + '0'));
    }
    digit = number / 10;
    number %= 10;
    if (digit != 0 || started) {
        appendGlyph(findGlyph(digit + '0'));
    }
    appendGlyph(findGlyph(number + '0'));
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

void cls_804AF6E4::do_character(int c) {
    if (c == '\n') {
        field_0x201++;
        field_0x22c++;
        field_0x4c[field_0x201] = 0.0f;
        field_0x22d[field_0x201] = 0;
        field_0xf4[field_0x201] = 0.0f;
        field_0x1e8 = 0;
        if (field_0x202[field_0x281] != field_0x22c && field_0x201 - field_0x202[field_0x281] >= 3) {
            field_0x281++;
            field_0x202[field_0x281] = field_0x22c;
            field_0x257[field_0x281] = 0;
            field_0x1e8 = 1;
        }
    } else {
        f32 ratio = field_0x48 / field_0x38->getCellWidth();
        JUTFont::TWidth width;
        field_0x38->getWidthEntry(c, &width);
        field_0x4c[field_0x201] += ratio * (field_0x44 * (s32)width.w1);
        field_0xf4[field_0x201] += 3.0f;
        field_0x22d[field_0x201]++;
    }
}

// FIXME(shotgun): 99.75 %, reloc-name
void cls_804AF6E4::fn_8039934C(s32 minutes, s32 seconds) {
    f32 ratio = field_0x48 / field_0x38->getCellWidth();
    if (minutes > 0) {
        if (minutes > 99) {
            minutes = 99;
            seconds = 59;
        }
        s32 digit = minutes / 10;
        s32 ones = minutes % 10;
        if (digit > 0) {
            addCharWidth(ratio, digit + '0');
        }
        addCharWidth(ratio, ones + '0');
        addCharWidth(ratio, '\'');
    }
    if (seconds > 59) {
        seconds = 59;
    }
    s32 digit = seconds / 10;
    s32 ones = seconds % 10;
    if (digit > 0 || minutes > 0) {
        addCharWidth(ratio, digit + '0');
    }
    addCharWidth(ratio, ones + '0');
    addSecondsMarkWidth(ratio);
}

// FIXME(shotgun): 98.68 %, scheduling+regalloc
void cls_804AF6E4::fn_80398EF4(s32 minutes, s32 seconds) {
    f32 ratio = field_0x48 / field_0x38->getCellWidth();
    if (minutes > 99) {
        minutes = 99;
    }
    s32 digit = minutes / 10;
    minutes %= 10;
    if (digit > 0) {
        addCharWidth(ratio, digit + '0');
    }
    addCharWidth(ratio, minutes + '0');
    addCharWidth(ratio, '\'');
    if (seconds > 99) {
        seconds = 99;
    }
    addCharWidth(ratio, seconds / 10 + '0');
    addCharWidth(ratio, seconds % 10 + '0');
    addSecondsMarkWidth(ratio);
}

// FIXME(shotgun): 99.79 %, reloc-name
void cls_804AF6E4::fn_80398A34(s32 number) {
    bool started = false;
    f32 ratio = field_0x48 / field_0x38->getCellWidth();
    if (number < 0) {
        number *= -1;
        addCharWidth(ratio, '-');
    }
    s32 digit = number / 10000;
    number %= 10000;
    if (digit != 0) {
        started = true;
        addCharWidth(ratio, digit + '0');
    }
    digit = number / 1000;
    number %= 1000;
    if (digit != 0 || started) {
        started = true;
        addCharWidth(ratio, digit + '0');
    }
    digit = number / 100;
    number %= 100;
    if (digit != 0 || started) {
        started = true;
        addCharWidth(ratio, digit + '0');
    }
    digit = number / 10;
    number %= 10;
    if (digit != 0 || started) {
        addCharWidth(ratio, digit + '0');
    }
    addCharWidth(ratio, number + '0');
}

// FIXME: 98.15. Scheduling only: the original increments and compares the
// loop index right after getWidthEntry, ours at the end of the body.
void cls_804AF6E4::fn_80398700(u8 noun, s32 count) {
    f32 ratio = field_0x48 / field_0x38->getCellWidth();
    char text[15];
    for (int i = 0; i < 15; i++) {
        text[i] = 0;
    }
    switch (noun) {
    case 1:
        if (count == 1) {
            strcat(text, " Force Gem");
        } else {
            strcat(text, " Force Gems");
        }
        break;
    case 2:
        if (count == 1) {
            strcat(text, " cocco");
        } else {
            strcat(text, " coccos");
        }
        break;
    case 3:
        if (count == 1) {
            strcat(text, " baddy");
        } else {
            strcat(text, " baddies");
        }
        break;
    case 4:
        if (count == 1) {
            strcat(text, " time");
        } else {
            strcat(text, " times");
        }
        break;
    case 5:
        if (count == 1) {
            strcat(text, " system");
        } else {
            strcat(text, " systems");
        }
        break;
    case 6:
        if (count == 1) {
            strcat(text, " thief");
        } else {
            strcat(text, " thieves");
        }
        break;
    case 7:
        if (count == 1) {
            strcat(text, " mole");
        } else {
            strcat(text, " moles");
        }
        break;
    case 8:
        if (count == 1) {
            strcat(text, " return");
        } else {
            strcat(text, " returns");
        }
        break;
    case 9:
        if (count == 1) {
            strcat(text, " Force Fairy");
        } else {
            strcat(text, " Force Fairies");
        }
        break;
    }
    for (int i = 0; i < 15 && text[i] != 0; i++) {
        JUTFont::TWidth width;
        field_0x38->getWidthEntry(text[i], &width);
        field_0x4c[field_0x201] += ratio * (field_0x44 * (s32)width.w1);
        field_0xf4[field_0x201] += 3.0f;
        field_0x22d[field_0x201]++;
    }
}

cls_804AF6E4::~cls_804AF6E4() {}

// FIXME: 99.79. Regalloc + stack slots + reloc-name: the message font goes
// through r27 instead of r28 (the eight setFont calls); the random draw's
// and the two parse_next pointers' stack slots differ (0x8 / 0x10 / 0xC);
// the string offsets are relative to the section base (lbl_80475768+0x670..)
// in the original, to the string pool in ours (same addresses).
// The font is read into a local before each search, as in tu_803E1668.
cls_804AF5A0::cls_804AF5A0(MessageWindow_804AF490* window, s32 index, const char* bmgName) {
    JUTResFont* font;
    const ResTIMG* timg;

    field_0x3c0 = this;
    field_0x3b8 = window;
    field_0x18 = gGameRoot_8053AAF8->field_0xa8->getResource('????', bmgName);
    field_0x1c = NULL;
    field_0x15c = 0;
    field_0x164 = 0;
    resetFields0x168();
    field_0x160 = index;
    field_0x14c = 0;
    field_0x3ae = 7;
    field_0x3b0 = 0;
    field_0x3af = 1;
    field_0x1aa = 0;
    for (int i = 0; i < 4; i++) {
        field_0x180[i] = -1;
    }
    field_0x190 = 0;
    field_0x192 = 0;
    field_0x178 = 0;
    field_0x17c = 0;
    field_0x194 = 0;
    field_0x196 = 0;
    field_0x1a2 = 0;
    field_0x98 = 0.0f;
    field_0x9c = 0.0f;
    field_0xa0 = 0.0f;
    field_0x1ab[0] = 0;
    field_0x2ab[0] = 0;
    field_0x3ab[0] = 0;
    field_0x170 = 0;
    field_0x154 = -1;
    field_0x158 = 0xFF;
    field_0x19f = 0;
    field_0x1a9 = 0;
    field_0x3b1 = 0;
    field_0x174 = -1;
    for (int i = 0; i < 12; i++) {
        field_0x68[i] = new Entry_0x68;
        field_0x68[i]->field_0x0 = 0.0f;
        field_0x68[i]->field_0x4 = 0.0f;
        field_0x68[i]->field_0x8 = 0xFF;
    }
    for (int i = 0; i < 3; i++) {
        field_0x3b2[i] = 0;
    }
    field_0xa4.x = 0.0f;
    field_0xa4.y = 0.0f;
    field_0xa4.z = 0.0f;
    field_0x3bc = new cls_804B20F8(index, &field_0xa4, NULL, 8);

    field_0x20[0] = new J2DScreen;
    field_0x20[0]->setPriority("hukidashi.blo", 0x1100000, lbl_80529DEC.field_0x20);
    field_0x20[1] = new J2DScreen;
    field_0x20[1]->setPriority("hukidashi_wood.blo", 0x1100000, lbl_80529DEC.field_0x20);
    field_0x20[2] = new J2DScreen;
    field_0x20[2]->setPriority("hukidashi_stone.blo", 0x1100000, lbl_80529DEC.field_0x20);
    field_0x20[3] = new J2DScreen;
    field_0x20[3]->setPriority("op_zelda.blo", 0x1100000, lbl_80529DEC.field_0x20);

    timg = (const ResTIMG*)fn_804030C4('TIMG', "kk_linkGr_01.bti");
    field_0x30[0] = new J2DPicture(timg);
    timg = (const ResTIMG*)fn_804030C4('TIMG', "kk_linkRe_01.bti");
    field_0x30[1] = new J2DPicture(timg);
    timg = (const ResTIMG*)fn_804030C4('TIMG', "kk_linkBl_01.bti");
    field_0x30[2] = new J2DPicture(timg);
    timg = (const ResTIMG*)fn_804030C4('TIMG', "kk_linkPu_01.bti");
    field_0x30[3] = new J2DPicture(timg);
    timg = (const ResTIMG*)fn_804030C4('TIMG', "kk_angou_01.bti");
    field_0x30[4] = new J2DPicture(timg);
    timg = (const ResTIMG*)fn_804030C4('TIMG', "kk_angou_02.bti");
    field_0x30[5] = new J2DPicture(timg);
    timg = (const ResTIMG*)fn_804030C4('TIMG', "kk_angou_03.bti");
    field_0x30[6] = new J2DPicture(timg);
    timg = (const ResTIMG*)fn_804030C4('TIMG', "heart.bti");
    field_0x30[7] = new J2DPicture(timg);
    timg = (const ResTIMG*)fn_804030C4('TIMG', "heart_01.bti");
    field_0x30[8] = new J2DPicture(timg);
    timg = (const ResTIMG*)fn_804030C4('TIMG', "heart_02.bti");
    field_0x30[9] = new J2DPicture(timg);
    timg = (const ResTIMG*)fn_804030C4('TIMG', "heart_03.bti");
    field_0x30[10] = new J2DPicture(timg);

    field_0xb8 = field_0x20[0]->search('Pm00')->getWidth();
    field_0xbc = field_0x20[0]->search('Pm00')->getHeight();
    field_0xc0 = field_0x20[0]->search('Nmdata01')->getBounds()->i.x;
    field_0xc4 = field_0x20[0]->search('Nmdata01')->getBounds()->i.y;
    field_0xc8 = field_0x20[0]->search('Tm01')->getBounds()->i.x;
    field_0xcc = field_0x20[0]->search('Tm01')->getBounds()->i.y;
    field_0xd0 = field_0x20[0]->search('Tm02')->getBounds()->i.x - field_0x20[0]->search('Tm01')->getBounds()->i.x;
    field_0xd4 = field_0x20[0]->search('Tm02')->getBounds()->i.y - field_0x20[0]->search('Tm01')->getBounds()->i.y;
    field_0xe8 = field_0x20[0]->search('Tm01')->getWidth();
    field_0xec = field_0x20[0]->search('Tm01')->getHeight();
    field_0x11c = field_0x20[0]->search('ROOT')->getBounds()->i.x;
    field_0x120 = field_0x20[0]->search('ROOT')->getBounds()->i.y;
    field_0xf8 = field_0x20[0]->search('P1ar')->getBounds()->i.x;
    field_0xfc = field_0x20[0]->search('P1ar')->getBounds()->i.y;
    field_0x100 = field_0x20[0]->search('P1ar')->getWidth();
    field_0x104 = field_0x20[0]->search('P1ar')->getHeight();
    field_0x1a3 = field_0x20[0]->search('P1ar')->getAlpha();
    field_0x1a5 = field_0x20[0]->search('P1b01')->getAlpha();
    field_0x1a6 = field_0x20[0]->search('P1b01')->getAlpha();
    field_0x1a7 = field_0x20[0]->search('P1n02')->getAlpha();
    field_0x1a8 = field_0x20[0]->search('P1n01')->getAlpha();
    field_0xd8 = field_0x20[3]->search('Tm01')->getBounds()->i.x;
    field_0xdc = field_0x20[3]->search('Tm01')->getBounds()->i.y;
    field_0xf0 = field_0x20[3]->search('Tm01')->getWidth();
    field_0xf4 = field_0x20[3]->search('Tm01')->getHeight();
    field_0xe0 = field_0x20[3]->search('Tm02')->getBounds()->i.x - field_0x20[3]->search('Tm01')->getBounds()->i.x;
    field_0xe4 = field_0x20[3]->search('Tm02')->getBounds()->i.y - field_0x20[3]->search('Tm01')->getBounds()->i.y;
    field_0x108 = field_0x20[3]->search('P1ar')->getBounds()->i.x;
    field_0x10c = field_0x20[3]->search('P1ar')->getBounds()->i.y;
    field_0x110 = field_0x20[3]->search('P1ar')->getWidth();
    field_0x114 = field_0x20[3]->search('P1ar')->getHeight();
    field_0x1a3 = field_0x20[3]->search('P1ar')->getAlpha();

    font = lbl_8053AD78->mFont;
    static_cast<J2DTextBox*>(field_0x20[0]->search('Tm01'))->setFont(font);
    font = lbl_8053AD78->mFont;
    static_cast<J2DTextBox*>(field_0x20[1]->search('Tm01'))->setFont(font);
    font = lbl_8053AD78->mFont;
    static_cast<J2DTextBox*>(field_0x20[2]->search('Tm01'))->setFont(font);
    font = lbl_8053AD78->mFont;
    static_cast<J2DTextBox*>(field_0x20[3]->search('Tm01'))->setFont(font);
    font = lbl_8053AD78->mFont;
    static_cast<J2DTextBox*>(field_0x20[3]->search('Tm02'))->setFont(font);
    font = lbl_8053AD78->mFont;
    static_cast<J2DTextBox*>(field_0x20[0]->search('Tm02'))->setFont(font);
    font = lbl_8053AD78->mFont;
    static_cast<J2DTextBox*>(field_0x20[1]->search('Tm02'))->setFont(font);
    font = lbl_8053AD78->mFont;
    static_cast<J2DTextBox*>(field_0x20[2]->search('Tm02'))->setFont(font);
    static_cast<J2DTextBox*>(field_0x20[0]->search('Tm02'))->setBlackWhite(
        JUtility::TColor(lbl_8050DB0C.field_0x304[3], lbl_8050DB0C.field_0x304[4], lbl_8050DB0C.field_0x304[5], 0),
        JUtility::TColor(lbl_8050DB0C.field_0x304[0], lbl_8050DB0C.field_0x304[1], lbl_8050DB0C.field_0x304[2], 0xFF));
    field_0x20[3]->search('Ndata05')->hide();
    field_0x20[3]->search('Ndata01')->hide();

    timg = (const ResTIMG*)gGameRoot_8053AAF8->field_0xa4->getResource('TIMG', "kk_yazirushi_01.bti");
    field_0x5c = new J2DPicture(timg);
    field_0x5c->setBlackWhite(JUtility::TColor(0xFF, 0xFF, 0x8C, 0), JUtility::TColor(0xFF, 0xFF, 0x8C, 0xFF));
    field_0x5c->rotate(9.6f, 9.6f * 1.5f, J2DROTATE_Z, -96.0f);
    field_0x118 = 0.0f;
    field_0x198 = (s32)(11.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 10;
    field_0x19a = -0x60;
    field_0x19c = 0;

    field_0x4 = NULL;
    field_0x4 = new JMessage::TResourceContainer;
    field_0x8 = NULL;
    field_0x8 = new JMessage::TControl;
    field_0xc = NULL;
    field_0xc = new cls_804AF6D4;
    field_0x10 = NULL;
    field_0x10 = new cls_804AF774(field_0xc, field_0x8);
    field_0x10->field_0x60 = field_0x3c0;
    field_0x14 = NULL;
    field_0x14 = new cls_804AF72C(field_0xc);
    field_0x14->field_0x2bc = static_cast<J2DTextBox*>(field_0x20[0]->search('Tm01'))->getFont();
    // Local only to match: the original loads field_0x14 once for the two buffer stores.
    cls_804AF72C* layout = field_0x14;
    layout->field_0x2c0 = field_0x1ab;
    layout->field_0x2c4 = field_0x2ab;
    field_0x14->field_0x2d8 = field_0x3c0;

    // One 'Tm01' search serves both font-size loads (two searches: 99.12 %).
    J2DTextBox* text = static_cast<J2DTextBox*>(field_0x20[0]->search('Tm01'));
    field_0x60 = text->mFontSize.x;
    field_0x64 = text->mFontSize.y;
    field_0xb0 = static_cast<J2DTextBox*>(field_0x20[0]->search('Tm01'))->getLineSpace();
    fn_803944EC(1.0f);

    field_0xc->setResourceContainer(field_0x4);
    field_0x8->mBaseProcSeq = field_0x10;
    field_0x8->mBaseProcRender = field_0x14;
    JMessage::TParse parse(field_0x4);
    parse.parse(field_0x18, 0);
    if (field_0x1c != NULL) {
        JMessage::TParse_color parseColor(field_0x4);
        parseColor.parse(field_0x1c, 0);
    }
    field_0x8->reset();
    field_0x8->resetResourceCache();
}

cls_804AF6D4::~cls_804AF6D4() {}

cls_804AF774::~cls_804AF774() {}

cls_804AF72C::~cls_804AF72C() {}

cls_804AF5A0::~cls_804AF5A0() {
    for (int i = 0; i < 12; i++) {
        delete field_0x68[i];
        field_0x68[i] = NULL;
    }
    delete field_0x3bc;
    field_0x3bc = NULL;
    delete field_0x20[0];
    field_0x20[0] = NULL;
    delete field_0x20[1];
    field_0x20[1] = NULL;
    delete field_0x20[2];
    field_0x20[2] = NULL;
    delete field_0x20[3];
    field_0x20[3] = NULL;
    for (int i = 0; i < 11; i++) {
        delete field_0x30[i];
        field_0x30[i] = NULL;
    }
    delete field_0x5c;
    field_0x5c = NULL;
    field_0x4->destroyResource_all();
    delete field_0x4;
    field_0x4 = NULL;
    delete field_0x8;
    field_0x8 = NULL;
    delete field_0xc;
    field_0xc = NULL;
    delete field_0x10;
    field_0x10 = NULL;
    delete field_0x14;
    field_0x14 = NULL;
}

void cls_804AF5A0::fn_80396F94() {
    field_0x8->update();
    fn_80396884();
    fn_80395BE8();
    field_0x3b0 = 1;
}

// Pops a window pane in: its alpha rises over 10 frames; once it is full, its size
// follows a five-point curve per axis over the frame counts of the layout constants, then
// the pane is moved so that it stays centred on the box.
// FIXME(shotgun): 93.00 %, missing/extra (setAlpha argument masking, window load order)
void cls_804AF5A0::fn_80395834(J2DPane* pane) {
    s16 t0 = lbl_8050DB0C.field_0x4c[(0x286 - 0x4C) / 2];
    s16 t1 = t0 + lbl_8050DB0C.field_0x4c[(0x288 - 0x4C) / 2];
    s16 t2 = t1 + lbl_8050DB0C.field_0x4c[(0x28A - 0x4C) / 2];
    s16 t3 = t2 + lbl_8050DB0C.field_0x4c[(0x28C - 0x4C) / 2];
    s16 t4 = t3 + lbl_8050DB0C.field_0x4c[(0x28E - 0x4C) / 2];
    field_0x194++;
    if (pane->getAlpha() < field_0x1a3) {
        if (field_0x194 == 10) {
            field_0x194 = 0;
            pane->setAlpha(field_0x1a3);
        } else {
            pane->setAlpha((s32)(field_0x1a3 * ((f32)field_0x194 / 10.0f)));
        }
        return;
    }
    f32 width;
    f32 height;
    s16 frame = field_0x194;
    if (frame <= t0) {
        width = 1.0f;
        height = width;
    } else if (frame <= t1) {
        f32 dt = (f32)(t1 - t0);
        f32 df = (f32)(frame - t0);
        f32 r = (df * df) / (dt * dt);
        width = lbl_80475D78[0] + r * (lbl_80475D78[1] - lbl_80475D78[0]);
        height = lbl_80475D8C[0] + r * (lbl_80475D8C[1] - lbl_80475D8C[0]);
    } else if (frame <= t2) {
        f32 dt = (f32)(t2 - t1);
        f32 df = (f32)(frame - t1);
        f32 r = (df * df) / (dt * dt);
        width = lbl_80475D78[1] + r * (lbl_80475D78[2] - lbl_80475D78[1]);
        height = lbl_80475D8C[1] + r * (lbl_80475D8C[2] - lbl_80475D8C[1]);
    } else if (frame <= t3) {
        f32 dt = (f32)(t3 - t2);
        f32 df = (f32)(frame - t2);
        f32 r = (df * df) / (dt * dt);
        width = lbl_80475D78[2] + r * (lbl_80475D78[3] - lbl_80475D78[2]);
        height = lbl_80475D8C[2] + r * (lbl_80475D8C[3] - lbl_80475D8C[2]);
    } else if (frame <= t4) {
        f32 dt = (f32)(t4 - t3);
        f32 df = (f32)(frame - t3);
        f32 r = (df * df) / (dt * dt);
        width = lbl_80475D78[3] + r * (lbl_80475D78[4] - lbl_80475D78[3]);
        height = lbl_80475D8C[3] + r * (lbl_80475D8C[4] - lbl_80475D8C[3]);
    } else {
        width = 1.0f;
        field_0x194 = 0;
        height = width;
    }
    f32 w = width * field_0x110;
    f32 h = height * field_0x114;
    pane->resize(w, h);
    pane->move(field_0x108 + field_0x110 * 0.5f - w * 0.5f, (field_0x10c + field_0x114) - h);
}

// FIXME(shotgun): 93.03 %, missing/extra (setAlpha argument masking, window load order)
void cls_804AF5A0::fn_80395480(J2DPane* pane) {
    s16 t0 = lbl_8050DB0C.field_0x4c[(0x286 - 0x4C) / 2];
    s16 t1 = t0 + lbl_8050DB0C.field_0x4c[(0x288 - 0x4C) / 2];
    s16 t2 = t1 + lbl_8050DB0C.field_0x4c[(0x28A - 0x4C) / 2];
    s16 t3 = t2 + lbl_8050DB0C.field_0x4c[(0x28C - 0x4C) / 2];
    s16 t4 = t3 + lbl_8050DB0C.field_0x4c[(0x28E - 0x4C) / 2];
    field_0x194++;
    if (pane->getAlpha() < field_0x1a4) {
        if (field_0x194 == 10) {
            field_0x194 = 0;
            pane->setAlpha(field_0x1a4);
        } else {
            pane->setAlpha((s32)(field_0x1a4 * ((f32)field_0x194 / 10.0f)));
        }
        return;
    }
    f32 width;
    f32 height;
    s16 frame = field_0x194;
    if (frame <= t0) {
        width = 1.0f;
        height = width;
    } else if (frame <= t1) {
        f32 dt = (f32)(t1 - t0);
        f32 df = (f32)(frame - t0);
        f32 r = (df * df) / (dt * dt);
        width = lbl_80475DA0[0] + r * (lbl_80475DA0[1] - lbl_80475DA0[0]);
        height = lbl_80475DB4[0] + r * (lbl_80475DB4[1] - lbl_80475DB4[0]);
    } else if (frame <= t2) {
        f32 dt = (f32)(t2 - t1);
        f32 df = (f32)(frame - t1);
        f32 r = (df * df) / (dt * dt);
        width = lbl_80475DA0[1] + r * (lbl_80475DA0[2] - lbl_80475DA0[1]);
        height = lbl_80475DB4[1] + r * (lbl_80475DB4[2] - lbl_80475DB4[1]);
    } else if (frame <= t3) {
        f32 dt = (f32)(t3 - t2);
        f32 df = (f32)(frame - t2);
        f32 r = (df * df) / (dt * dt);
        width = lbl_80475DA0[2] + r * (lbl_80475DA0[3] - lbl_80475DA0[2]);
        height = lbl_80475DB4[2] + r * (lbl_80475DB4[3] - lbl_80475DB4[2]);
    } else if (frame <= t4) {
        f32 dt = (f32)(t4 - t3);
        f32 df = (f32)(frame - t3);
        f32 r = (df * df) / (dt * dt);
        width = lbl_80475DA0[3] + r * (lbl_80475DA0[4] - lbl_80475DA0[3]);
        height = lbl_80475DB4[3] + r * (lbl_80475DB4[4] - lbl_80475DB4[3]);
    } else {
        width = 1.0f;
        field_0x194 = 0;
        height = width;
    }
    f32 w = width * field_0x110;
    f32 h = height * field_0x114;
    pane->resize(w, h);
    pane->move(field_0x108 + field_0x110 * 0.5f - w * 0.5f, (field_0x10c + field_0x114) - h);
}

void cls_804AF5A0::fn_8039499C(bool flag) {
    if (field_0x1aa == 0) {
        f32 x = field_0xc0;
        f32 y = field_0xc4;
        f32 line;
        J2DScreen* screen = field_0x20[field_0x1a2];
        J2DTextBox* tm01 = static_cast<J2DTextBox*>(screen->search('Tm01'));
        J2DTextBox* tm02 = static_cast<J2DTextBox*>(screen->search('Tm02'));
        J2DPane* nmdata = screen->search('Nmdata01');
        if (field_0x3b0 != 0) {
            field_0x8->render();
            field_0x3b0 = 0;
        }
        if (flag) {
            screen->search('ROOT')->move(field_0x11c + field_0x98, lbl_8050DB0C.field_0x30[3] + (field_0x120 + field_0x9c));
        } else {
            field_0x98 = 0.0f;
            field_0x9c = 0.0f;
            field_0xa0 = 0.0f;
            screen->search('ROOT')->move(field_0x11c, field_0x120);
        }
        f32 offset = 0.5f * ((3.0f - field_0x164) * tm01->getLineSpace());
        if (screen == field_0x20[0] && !field_0x20[0]->search('Pm00')->isVisible()) {
            field_0x20[0]->search('Pm00')->show();
            field_0x20[0]->search('P5b02')->show();
            field_0x20[0]->search('P5ar')->show();
            field_0x20[0]->search('P4b02')->show();
            field_0x20[0]->search('P4ar')->show();
            field_0x20[0]->search('P3b02')->show();
            field_0x20[0]->search('P3ar')->show();
            field_0x20[0]->search('P2b02')->show();
            field_0x20[0]->search('P2ar')->show();
            field_0x20[0]->search('P1b02')->show();
            field_0x20[0]->search('P1ar')->show();
        }
        strcpy(tm01->getStringPtr(), field_0x1ab);
        strcpy(tm02->getStringPtr(), field_0x2ab);
        nmdata->move(x, y);
        tm01->move(field_0xc8, field_0xcc + offset);
        tm01->resize(100.0f + field_0xe8, field_0xec);
        tm02->move(field_0xc8 + field_0xd0, field_0xd4 + (field_0xcc + offset));
        tm02->resize(100.0f + field_0xe8, field_0xec);
        gGameRoot2_8053AB10->fn_8013B490();
        screen->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);

        // The pictures placed in the text (cls_804AF72C::fn_8039CDF8).
        for (int i = 0; i < 12; i++) {
            if (field_0x68[i]->field_0x8 != 0xFF) {
                f32 px = field_0x68[i]->field_0x0 + ((field_0x98 + x) + tm01->getBounds()->i.x);
                f32 py = (offset + ((field_0x9c + y) + tm01->getBounds()->i.y)) + field_0x68[i]->field_0x4 * tm01->getLineSpace();
                if (field_0x164 == 1) {
                    py -= 26.0f;
                } else if (field_0x164 == 2) {
                    py -= 13.0f;
                }
                field_0x30[field_0x68[i]->field_0x8]->draw(px, py, 29.0f, 29.0f, false, false, false);
            }
        }

        // The arrow (0x5C): it bobs over 10 frames when 0x19C is set and flips its angle at random intervals.
        if (field_0x1a0 != 0) {
            f32 ox = 0.0f; // a zero base the original still adds (a literal 0.0f + x folds away)
            f32 dx = 0.0f;
            s16 t = field_0x19c;
            int v;
            if (t < 0) {
                if (t > -5) {
                    v = -t;
                } else {
                    v = t + 10;
                    if (t == -10) {
                        field_0x19c = 0;
                    }
                }
                dx = -5.0f * (v * v) / 25.0f;
                if (field_0x19c != 0) {
                    field_0x19c--;
                }
            } else if (t > 0) {
                if (t < 5) {
                    v = t;
                } else {
                    v = 10 - t;
                    if (t == 10) {
                        field_0x19c = 0;
                    }
                }
                dx = 5.0f * (v * v) / 25.0f;
                if (field_0x19c != 0) {
                    field_0x19c++;
                }
            }
            if (field_0x1a1 == 0) {
                if (field_0x164 == 2) {
                    f32 py = (offset + ((field_0x9c + y) + tm01->getBounds()->i.y)) + field_0x130[field_0x168] * tm01->getLineSpace() - 13.0f;
                    field_0x5c->draw(ox + (field_0x124[field_0x168] + ((field_0x98 + x) + tm01->getBounds()->i.x)), py, 19.2f, 19.2f * 1.5f, false, false, false);
                } else if (field_0x164 == 3) {
                    f32 py = (offset + ((field_0x9c + y) + tm01->getBounds()->i.y)) + field_0x130[field_0x168] * tm01->getLineSpace();
                    field_0x5c->draw(ox + (field_0x124[field_0x168] + ((field_0x98 + x) + tm01->getBounds()->i.x)), py, 19.2f, 19.2f * 1.5f, false, false, false);
                }
            } else {
                line = field_0x130[field_0x168];
                if (line == 0.0f) {
                    f32 py = (offset + ((field_0x9c + y) + tm01->getBounds()->i.y)) + line * tm01->getLineSpace() - 30.0f;
                    field_0x5c->draw(ox + (dx + (field_0x124[field_0x168] + ((field_0x98 + x) + tm01->getBounds()->i.x))), py, 19.2f, 19.2f * 1.5f, false, false, false);
                } else if (line == 1.0f) {
                    f32 py = (offset + ((field_0x9c + y) + tm01->getBounds()->i.y)) + line * tm01->getLineSpace() - 13.0f;
                    field_0x5c->draw(ox + (dx + (field_0x124[field_0x168] + ((field_0x98 + x) + tm01->getBounds()->i.x))), py, 19.2f, 19.2f * 1.5f, false, false, false);
                } else if (line == 2.0f) {
                    f32 py = (offset + ((field_0x9c + y) + tm01->getBounds()->i.y)) + line * tm01->getLineSpace();
                    field_0x5c->draw(ox + (dx + (field_0x124[field_0x168] + ((field_0x98 + x) + tm01->getBounds()->i.x))), py, 19.2f, 19.2f * 1.5f, false, false, false);
                }
            }
            field_0x198--;
            if (field_0x198 <= 0) {
                if (field_0x19a == -0x60) {
                    field_0x19a = -0x54;
                } else {
                    field_0x19a = -0x60;
                }
                field_0x5c->rotate(9.6f, 9.6f * 1.5f, J2DROTATE_Z, field_0x19a);
                field_0x198 = (s32)(11.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 10;
            }
        }
    } else if (field_0x3b0 != 0) {
        field_0x8->render();
        field_0x3b0 = 0;
    }
}

// FIXME: 98.31. Missing 2 instructions around the clamp: the original copies
// the processor's frame count (mr r4,r6) and clamps the copy to 0; ours clamps
// in place (separate-variable, ternary and if/else forms tried: same or worse).
void cls_804AF5A0::fn_80394690() {
    if (field_0x3b0 != 0) {
        field_0x8->render();
        field_0x3b0 = 0;
    }
    f32 offset = 60.0f + 0.5f * ((3.0f - field_0x164) * static_cast<J2DTextBox*>(field_0x20[3]->search('Tm01'))->getLineSpace());
    if (field_0x1aa == 0) {
        char line1[0x100];
        char line2[0x100];
        strcpy(line1, field_0x1ab);
        strcpy(line2, field_0x2ab);
        if (strlen(field_0x3ab) != 0) {
            s32 frame = field_0x10->field_0x4c;
            s16 frames = lbl_8050DB0C.field_0x4c[0];
            line1[field_0x170] = 0;
            line2[field_0x170] = 0;
            if (frame > frames) {
                frame = 0;
            }
            u8 alpha = 255.0f * (frames - frame) / frames;
            char color[32];
            snprintf(color, 30, "\x1B" "CC[%08x]\x1B" "GM[0]", (field_0x154 & 0xFFFFFF00) | alpha);
            strcat(line1, color);
            strcat(line1, field_0x3ab);
            snprintf(color, 30, "\x1B" "CC[%08x]\x1B" "GM[0]", (field_0x158 & 0xFFFFFF00) | alpha);
            strcat(line2, color);
            strcat(line2, field_0x3ab);
        }
        strcpy(static_cast<J2DTextBox*>(field_0x20[3]->search('Tm01'))->getStringPtr(), line1);
        strcpy(static_cast<J2DTextBox*>(field_0x20[3]->search('Tm02'))->getStringPtr(), line2);
        field_0x20[3]->search('Tm01')->move(field_0xd8, lbl_8050DB0C.field_0x30[3] + (field_0x9c + offset));
        field_0x20[3]->search('Tm02')->move(field_0xd8 + field_0xe0, field_0xe4 + (lbl_8050DB0C.field_0x30[3] + (field_0x9c + offset)));
        gGameRoot2_8053AB10->fn_8013B490();
        field_0x20[3]->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
    }
}

void cls_804AF5A0::fn_803944EC(f32 scale) {
    field_0xb4 = scale;
    J2DTextBox::TFontSize size;
    size.x = field_0x60 * scale;
    size.y = field_0x64 * scale;
    static_cast<J2DTextBox*>(field_0x20[0]->search('Tm01'))->setFontSize(size);
    static_cast<J2DTextBox*>(field_0x20[0]->search('Tm01'))->setLineSpace(field_0xb0 * scale);
    static_cast<J2DTextBox*>(field_0x20[0]->search('Tm02'))->setFontSize(size);
    static_cast<J2DTextBox*>(field_0x20[0]->search('Tm02'))->setLineSpace(field_0xb0 * scale);
    field_0x14->setScale(scale, field_0x60);
}

void cls_804AF5A0::fn_803944C8(s32 value, u8 a, u8 b) {
    field_0x3b1 = 1;
    lbl_8053AD78->field_0x48 = value;
    lbl_8053AD78->field_0xea = a;
    lbl_8053AD78->field_0xeb = b;
}
