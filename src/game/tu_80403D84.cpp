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
#include "game/cls_804B1600.h"
#include "game/cls_804B20F8.h"
#include "game/EmitterDelete.h"
#include "JSystem/J2D/J2DAnimation.h"
#include "std/math.h"
#include "JSystem/JUtility/JUTVideo.h"
#include "game/Map.h"
#include "game/SpriteParams.h"
#include "game/Effect.h"
#include "dolphin/gx.h"
#include "game/RotMatrix.h"

// Read by fn_804054B8 and fn_80405D00 (lbzx), and fn_8040B5A0 (lhax).
static const u8 lbl_80543880[8] = {3, 1, 1, 0, 1, 1, 0, 1};
static const u8 lbl_80543888[8] = {1, 3, 3, 2, 3, 3, 2, 3};
static const s16 lbl_80543890[4] = {0, 1, 2, 1};

// .rodata 0x8048C1F8-0x8048CC1C in address order: map.blo pane-tag tables (u64 J2D tags),
// the screen's s32 / s16 / u8 index tables and the rupee-digit texture names (lbl_804B1260, .data).
static const s32 lbl_8048C1F8[6][8] = {
    {288, 289, 290, 291, 292, 293, 294, 295},
    {580, 581, 582, 583, 584, 585, 586, 587},
    {0, 4, 8, 12, 16, 20, 24, 28},
    {1, 5, 9, 14, 17, 21, 25, 29},
    {2, 6, 10, 13, 18, 22, 26, 30},
    {3, 7, 11, 15, 19, 23, 27, 31},
};
static const u64 lbl_8048C2B8[8] = {
    'Pwarea01', 'Pwarea02', 'Pwarea03', 'Pwarea04', 'Pwarea05', 'Pwarea06', 'Pwarea07', 'Pwarea08',
};
static const u64 lbl_8048C2F8[32][2] = {
    {'Ppoint01', 'Ppoint02'}, {'Ppoint03', 'Ppoint04'}, {'Ppoint05', 'Ppoint06'}, {'Ppoint07', 'Ppoint08'},
    {'Ppoint09', 'Ppoint10'}, {'Ppoint11', 'Ppoint12'}, {'Ppoint13', 'Ppoint14'}, {'Ppoint15', 'Ppoint16'},
    {'Ppoint17', 'Ppoint18'}, {'Ppoint19', 'Ppoint20'}, {'Ppoint21', 'Ppoint22'}, {'Ppoint23', 'Ppoint24'},
    {'Ppoint25', 'Ppoint26'}, {'Ppoint27', 'Ppoint28'}, {'Ppoint29', 'Ppoint30'}, {'Ppoint31', 'Ppoint32'},
    {'Ppoint33', 'Ppoint34'}, {'Ppoint35', 'Ppoint36'}, {'Ppoint37', 'Ppoint38'}, {'Ppoint39', 'Ppoint40'},
    {'Ppoint41', 'Ppoint42'}, {'Ppoint43', 'Ppoint44'}, {'Ppoint45', 'Ppoint46'}, {'Ppoint47', 'Ppoint48'},
    {'Ppoint49', 'Ppoint50'}, {'Ppoint51', 'Ppoint52'}, {'Ppoint53', 'Ppoint54'}, {'Ppoint55', 'Ppoint56'},
    {'Ppoint57', 'Ppoint58'}, {'Ppoint59', 'Ppoint60'}, {'Ppoint61', 'Ppoint62'}, {'Ppoint63', 'Ppoint64'},
};
static const u64 lbl_8048C4F8[27] = {
    'Ndata11', 'Ndata12', 'Ndata13', 'Ndata14', 'Ndata15', 'Ndata16', 'Ndata17', 'Ndata18',
    'Ptink01', 'Ptink02', 'Ptink03', 'Ptink04', 'Ptink05', 'Ptink06', 'Ptink07', 'Ptink08',
    'Ttlv11', 'Ttlv12', 'Ptlvb11', 'Ptlvbk11', 'Ptlvb12', 'Ptlvbk12', 'Ptlvb13', 'Ptlvb14',
    'Ptlvb15', 'Ptlvbk13', 'Ptlvbk14',
};
static const u64 lbl_8048C5D0[9] = {
    'Penback', 'Psky', 'Pclo04', 'Pclo03', 'Pclo02', 'Pmapca', 'Pmaptou', 'Pclo01',
    'Penback1',
};
// The Plicu arrow pairs, one per stick direction (fn_804054B8 walks it from its start).
static const u64 lbl_8048C618[4][2] = {
    {'Plicu01', 'Plicu02'}, {'Plicu07', 'Plicu08'}, {'Plicu03', 'Plicu04'}, {'Plicu05', 'Plicu06'},
};
static const u64 lbl_8048C658[63] = {
    'Plv1b02', 'Plv1b01', 'Tlv01', 'Plv2b02', 'Plv2b01', 'Tlv02', 'Plv3b02', 'Plv3b01',
    'Tlv03', 'Plv4b02', 'Plv4b01', 'Tlv04', 'Plv5b02', 'Plv5b01', 'Tlv05', 'Plv6b02',
    'Plv6b01', 'Tlv06', 'Plv7b02', 'Plv7b01', 'Tlv07', 'Plv8b02', 'Plv8b01', 'Tlv08',
    'Pbsk01', 'Pdot03', 'Pdot02', 'Pdot01', 'Pbsbb05', 'Pbsbb04', 'Pbsbb03', 'Pbsbb02',
    'Pbsbb01', 'Pbsb05', 'Pbsb04', 'Pbsb03', 'Pbsb02', 'Pbsb01', 'Pbs01', 'Ppl01',
    'Ppl02', 'Ppl03', 'Ppl04', 'Ppl05', 'Ppl06', 'Ppl07', 'Ppl08', 'Ppl09',
    'Ppl10', 'Ppl11', 'Ppl12', 'Ppl13', 'Ppl14', 'Ppl15', 'Ppl16', 'Ppl17',
    'Ppl18', 'Ppl19', 'Ppl20', 'Ppl21', 'Ppl22', 'Ppl23', 'Ppl24',
};
static const f32 lbl_8048C850[7] = {
    1034.0f, 1277.0f, 1277.0f, 1034.0f, 1216.0f, 1277.0f, 1034.0f,
};
static const s16 lbl_8048C86C[8] = {
    -138, -346, 22, -103, 303, 365, 56, 0,
};
static const s16 lbl_8048C87C[8] = {
    29, -266, 235, -137, 10, -268, -130, 0,
};
static const s32 lbl_8048C88C[8][3] = {
    {0, 1, 3},
    {0, 1, 3},
    {0, 1, 3},
    {0, 1, 3},
    {0, 1, 3},
    {0, 1, 3},
    {0, 1, 3},
    {0, 2, 3},
};
static const s32 lbl_8048C8EC[8] = {
    2, 6, 10, 14, 18, 22, 26, 29,
};
static const u8 lbl_8048C90C[8][3] = {
    {3, 3, 3},
    {0, 1, 0},
    {1, 1, 1},
    {0, 0, 3},
    {1, 1, 2},
    {0, 1, 0},
    {0, 0, 0},
    {1, 1, 0},
};
static const u8 lbl_8048C924[8][3] = {
    {1, 1, 1},
    {2, 3, 2},
    {3, 3, 3},
    {2, 2, 1},
    {3, 3, 0},
    {2, 3, 2},
    {2, 2, 2},
    {3, 3, 2},
};
static const u8 lbl_8048C93C[8][2] = {
    {3, 3},
    {0, 0},
    {1, 1},
    {0, 3},
    {1, 2},
    {0, 1},
    {0, 0},
    {1, 0},
};
static const u8 lbl_8048C94C[8][2] = {
    {1, 1},
    {2, 2},
    {3, 3},
    {2, 1},
    {3, 0},
    {2, 3},
    {2, 2},
    {3, 2},
};
static const s16 lbl_8048C95C[32][2] = {
    {482, 184}, {371, 243}, {296, 272}, {164, 335}, {235, 332}, {281, 265}, {388, 268}, {373, 161},
    {155, 314}, {260, 243}, {381, 160}, {435, 156}, {318, 348}, {352, 273}, {325, 182}, {200, 175},
    {179, 207}, {267, 146}, {365, 207}, {295, 316}, {178, 315}, {206, 239}, {312, 284}, {347, 210},
    {308, 346}, {300, 257}, {250, 197}, {224, 152}, {179, 302}, {261, 252}, {372, 310}, {412, 229},
};
static const s16 lbl_8048C9DC[8][2] = {
    {161, 238}, {354, 90}, {435, 90}, {196, 105}, {295, 250}, {352, 125}, {224, 92}, {417, 129},
};
// The state handlers, indexed by field_0x350 (fn_804108B4).
static MapScreen_804B1288::StateFunc lbl_804B10F8[22] = {
    &MapScreen_804B1288::fn_8040F3AC,
    &MapScreen_804B1288::fn_8040F35C,
    &MapScreen_804B1288::fn_8040F288,
    &MapScreen_804B1288::fn_8040F16C,
    &MapScreen_804B1288::fn_8040ED20,
    &MapScreen_804B1288::fn_8040E838,
    &MapScreen_804B1288::fn_8040E720,
    &MapScreen_804B1288::fn_8040E700,
    &MapScreen_804B1288::fn_8040E3A0,
    &MapScreen_804B1288::fn_8040E298,
    &MapScreen_804B1288::fn_8040E278,
    &MapScreen_804B1288::fn_8040D3D0,
    &MapScreen_804B1288::fn_8040CE90,
    &MapScreen_804B1288::fn_8040C950,
    &MapScreen_804B1288::fn_8040BC1C,
    &MapScreen_804B1288::fn_80409FDC,
    &MapScreen_804B1288::fn_8040B93C,
    &MapScreen_804B1288::fn_8040B5A0,
    &MapScreen_804B1288::fn_8040B388,
    &MapScreen_804B1288::fn_804096E8,
    &MapScreen_804B1288::fn_80409450,
    &MapScreen_804B1288::fn_80409330,
};
// The message-box handlers, indexed by field_0x659 (fn_804108B4).
static MapScreen_804B1288::StateFunc lbl_804B1230[4] = {
    &MapScreen_804B1288::fn_80403F50,
    &MapScreen_804B1288::fn_80403EAC,
    &MapScreen_804B1288::fn_80403E24,
    &MapScreen_804B1288::fn_80403D84,
};
static const char* lbl_804B1260[10] = {
    "kk_rupyNum_00.bti",
    "kk_rupyNum_01.bti",
    "kk_rupyNum_02.bti",
    "kk_rupyNum_03.bti",
    "kk_rupyNum_04.bti",
    "kk_rupyNum_05.bti",
    "kk_rupyNum_06.bti",
    "kk_rupyNum_07.bti",
    "kk_rupyNum_08.bti",
    "kk_rupyNum_09.bti",
};
static const u64 lbl_8048CAC8[18] = {
    'P34', 'P33', 'P32', 'P31', 'P73', 'P53', 'P52', 'P51',
    'P43', 'P23', 'P62', 'P72', 'P71', 'P42', 'P41', 'P61',
    'P22', 'P21',
};
static const s32 lbl_8048CB58[4] = {
    0, 1, 2, 1,
};
static const s32 lbl_8048CB68[6] = {
    470, 488, 464, 494, 482, 476,
};
static const u8 lbl_8048CB80[6][3] = {
    {45, 120, 255}, {200, 160, 40}, {75, 255, 45}, {30, 160, 190}, {255, 65, 55}, {220, 30, 220},
};
static const s32 lbl_8048CB94[30] = {
    0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3,
    3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6,
};
static const s32 lbl_8048CC0C[4] = {
    60, 62, 61, 63,
};

// Inlined where the stage name changes (fn_804096E8 twice, fn_8040C950, fn_8040CE90); its
// debug-name literals sit in fn_804096E8's block of .sdata2, so it is an inline.
inline void MapScreen_804B1288::setStageName() {
    if (field_0x340 < 8) {
        getMessageText_803832BC(field_0x438, lbl_8048C1F8[0][field_0x340], 0, NULL, NULL);
    } else if (field_0x340 == 8) {
        strcpy(field_0x438, "Test-1");
    } else if (field_0x340 == 9) {
        strcpy(field_0x438, "ECTS");
    } else if (field_0x340 == 10) {
        strcpy(field_0x438, "E3");
    } else if (field_0x340 == 11) {
        strcpy(field_0x438, "Boss1-4");
    } else if (field_0x340 == 12) {
        strcpy(field_0x438, "Boss5-8");
    } else {
        field_0x438[0] = 0;
    }
}

bool fn_804149C0() {
    return lbl_8050EC80.getCurrentFile().field_0x67 & 1;
}

void fn_8041499C(JUtility::TColor* dst, const JUtility::TColor* src) {
    *dst = *src;
}

MapScreen_804B1288::~MapScreen_804B1288() {
    lbl_8053AEC4->fn_8042EB14();
    for (int i = 0; i < 4; i++) {
        deleteEmitter(&field_0x2cc[i]);
        deleteEmitter(&field_0x2dc[i]);
        if (field_0x2f8[i] != NULL) {
            field_0x2f8[i]->becomeInvalidEmitter();
            field_0x2f8[i] = NULL;
        }
    }
    for (int i = 0; i < 3; i++) {
        deleteEmitter(&field_0x2ec[i]);
    }
    for (int i = 0; i < 2; i++) {
        if (field_0x308[i] != NULL) {
            field_0x308[i]->becomeInvalidEmitter();
            field_0x308[i] = NULL;
        }
    }
    delete field_0x4;
    field_0x4 = NULL;
    delete field_0x8;
    field_0x8 = NULL;
    delete field_0xc;
    field_0xc = NULL;
    delete field_0x1c;
    field_0x1c = NULL;
    delete field_0x20;
    field_0x20 = NULL;
    delete field_0x10;
    field_0x10 = NULL;
    delete field_0x14;
    field_0x14 = NULL;
    delete field_0x18;
    field_0x18 = NULL;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 3; j++) {
            delete field_0x28[i][j];
            field_0x28[i][j] = NULL;
        }
    }
    for (int i = 0; i < 15; i++) {
        delete field_0x278[i];
        field_0x278[i] = NULL;
    }
    delete field_0xc8;
    field_0xc8 = NULL;
    delete field_0xcc;
    field_0xcc = NULL;
    delete field_0xd0;
    field_0xd0 = NULL;
    for (int i = 0; i < 18; i++) {
        delete field_0xd4[i];
        field_0xd4[i] = NULL;
    }
    delete field_0x11c;
    field_0x11c = NULL;
    for (int i = 0; i < 2; i++) {
        delete field_0x120[i];
        field_0x120[i] = NULL;
    }
    for (int i = 0; i < 32; i++) {
        delete field_0x128[i];
        field_0x128[i] = NULL;
    }
    for (int i = 0; i < 24; i++) {
        delete field_0x1a8[i];
        field_0x1a8[i] = NULL;
    }
    for (int i = 0; i < 8; i++) {
        delete field_0x208[i];
        field_0x208[i] = NULL;
    }
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            delete field_0x228[i][j];
            field_0x228[i][j] = NULL;
        }
    }
    delete field_0x238;
    field_0x238 = NULL;
    delete field_0x23c;
    field_0x23c = NULL;
    for (int i = 0; i < 12; i++) {
        delete field_0x240[i];
        field_0x240[i] = NULL;
    }
    delete field_0x270[0];
    field_0x270[0] = NULL;
    delete field_0x270[1];
    field_0x270[1] = NULL;
    for (int i = 0; i < 4; i++) {
        delete field_0x2b8[i];
        field_0x2b8[i] = NULL;
    }
    delete field_0x2c8;
    field_0x2c8 = NULL;
    lbl_8053ADF8 = NULL;
}

void MapScreen_804B1288::fn_80411A94(f32 alpha) {
    field_0x10->search(0x54746C763131ull /* "Ttlv11" */)->setAlpha(field_0x64b * alpha);
    field_0x10->search(0x54746C763132ull /* "Ttlv12" */)->setAlpha(field_0x64c * alpha);
    field_0x10->search(0x54746C763133ull /* "Ttlv13" */)->setAlpha(field_0x64b * (1.0f - alpha));
    field_0x10->search(0x54746C763134ull /* "Ttlv14" */)->setAlpha(field_0x64c * (1.0f - alpha));
}

void MapScreen_804B1288::fn_804111C4() {
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

void MapScreen_804B1288::fn_8040F3AC() {
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

void MapScreen_804B1288::fn_8040F35C() {
    if (field_0x11c->fn_80402940(0x14, 0, 200.0f, 0.0f)) {
        field_0x350 = 0xB;
    }
}

// FIXME: 94.91. scheduling+regalloc (+ pool literals of unwritten functions). The product
// is a local of its own: the original leaves -250 + 250 * t * t unfused (fmuls, fmuls,
// fadds), which MWCC does only across statements (also fn_8040F16C, fn_8040ED20).
void MapScreen_804B1288::fn_8040F288() {
    f32 t = (f32)field_0x358 / 90.0f;
    f32 rise = 250.0f * t * t;
    field_0x39c = -250.0f + rise;
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

// FIXME: 98.59. regalloc-only (+ pool literals). The product is its own local, as in
// fn_8040F288 (the original leaves it unfused).
void MapScreen_804B1288::fn_8040F16C() {
    field_0x434 += 0x200;
    field_0x436 += 0x400;
    field_0x3a0 = 15.0f * JMath::sincosTable_.sinShort(field_0x434);
    field_0x3a4 = 5.0f * JMath::sincosTable_.sinShort(field_0x436);
    if (field_0x358 < 90) {
        f32 t = (f32)field_0x358 / 90.0f;
        f32 rise = 250.0f * t * t;
        field_0x3a4 += -250.0f + rise;
    } else {
        field_0x2c8->startSound(JAISoundID(0x400E3));
        if (field_0x358 >= lbl_8050DB0C.field_0x4c[319] + 90) {
            field_0x358 = 0;
            field_0x350 = 0x15;
        }
    }
}

// FIXME: 99.58. regalloc-only (+ pool literals of unwritten functions). `drop` and `tt`
// are locals because the original leaves those products unfused (fmuls / fadds, fsubs),
// which MWCC does only across statements (as fn_8040F288 / fn_8040F16C).
void MapScreen_804B1288::fn_8040ED20() {
    field_0x434 += 0x200;
    field_0x436 += 0x400;
    field_0x3a0 = 15.0f * JMath::sincosTable_.sinShort(field_0x434) + lbl_8050DB0C.field_0x4c[313];
    field_0x3a4 = 5.0f * JMath::sincosTable_.sinShort(field_0x436) + lbl_8050DB0C.field_0x4c[314];
    field_0x665 = 255;
    if (lbl_8050DB0C.field_0x4c[312] == 1 && field_0x432 >= 6) {
        field_0x432 -= 6;
    }
    if (field_0x432 > lbl_8050DB0C.field_0x4c[315]) {
        if (field_0x432 == lbl_8050DB0C.field_0x4c[315] + 1) {
            field_0x2c8->startSound(JAISoundID(0x400E2));
        }
        f32 t = (f32)(field_0x432 - lbl_8050DB0C.field_0x4c[315]) / lbl_8050DB0C.field_0x4c[316];
        f32 drop = -250.0f * t * t;
        field_0x3a4 += drop;
        if (field_0x3a4 <= -250.0f) {
            field_0x3a4 += -250.0f;
        }
    } else {
        field_0x2c8->startSound(JAISoundID(0x400E3));
    }
    if (field_0x432 > lbl_8050DB0C.field_0x4c[321]) {
        s32 frame = field_0x432 - lbl_8050DB0C.field_0x4c[321];
        if (frame < lbl_8050DB0C.field_0x4c[322]) {
            f32 t = (f32)frame / lbl_8050DB0C.field_0x4c[322];
            f32 tt = t * t;
            field_0x665 = 255.0f * (1.0f - tt);
        } else {
            field_0x665 = 0;
        }
    }
    if (field_0x432 > lbl_8050DB0C.field_0x4c[317]) {
        f32 t = (f32)(field_0x432 - lbl_8050DB0C.field_0x4c[317]) / lbl_8050DB0C.field_0x4c[318];
        field_0x39c = -250.0f * t * t;
        if (field_0x39c <= -250.0f) {
            if (lbl_8050DB0C.field_0x4c[320] == 1) {
                field_0x39c = 0.0f;
                field_0x3a0 = 0.0f;
                field_0x3a4 = 0.0f;
                field_0x358 = 0;
                field_0x432 = 0;
            } else {
                field_0x39c = -250.0f;
                field_0x657 = 1;
                for (int i = field_0x340 * 4; i < field_0x340 * 4 + 4; i++) {
                    field_0x128[i]->field_0x26 = 0;
                    field_0x128[i]->mPane->setAlpha(0);
                    field_0x128[i]->setAlpha_804023D0(field_0x10->search(lbl_8048C2F8[i][1]), field_0x641);
                }
                for (int i = field_0x340 * 3; i < field_0x340 * 3 + 3; i++) {
                    field_0x1a8[i]->field_0x26 = 0;
                    field_0x1a8[i]->mPane->setAlpha(0);
                }
                field_0x208[field_0x340]->field_0x26 = 0;
                field_0x208[field_0x340]->mPane->setAlpha(0);
                field_0x350 = 0x13;
            }
        }
    }
}

void MapScreen_804B1288::fn_8040E838() {
    field_0x378++;
    if (field_0x378 >= field_0x1c->getFrameMax()) {
        field_0x378 = field_0x1c->getFrameMax() - 1;
    }
    field_0x37c++;
    if (field_0x37c >= field_0x20->getFrameMax()) {
        field_0x37c = field_0x20->getFrameMax() - 1;
    }
    field_0x1c->setFrame(field_0x378);
    field_0xc->search('Ndata08')->animationTransform();
    field_0x20->setFrame(field_0x37c);
    field_0xc->animation();
    if (field_0x378 >= field_0x364 * 200) {
        for (int i = 0; i < 18; i++) {
            field_0xd4[i]->fn_80402E70();
        }
        field_0xc->search('Ndata08')->setAnimation((J2DAnmTransform*)NULL);
        field_0x378 = field_0x364 * 200;
        field_0x37c = field_0x364 * 200;
        hideStagePanes(field_0x364);
        lbl_8050EC80.getCurrentFile().field_0x62[0] |= (u8)(1 << field_0x364);
        field_0x350 = 0xB;
        field_0x657 = 1;
        field_0x663 = 1;
    }
}

// FIXME: 92.84. scheduling+regalloc only: the original keeps `this` in r31 and the
// counter in r29 (loaded in the prologue after the saves); ours swaps them.
void MapScreen_804B1288::fn_8040E720() {
    int gone = 0;
    field_0x35c++;
    if (field_0x35c > 10) {
        if (field_0x35c == 22) {
            field_0x2c8->startSound(JAISoundID(0x30039));
        }
        for (int i = 0; i < 3; i++) {
            if (field_0x2ec[i] != NULL) {
                if (field_0x2ec[i]->isEnableDeleteEmitter()) {
                    JPAEmitterManager* manager = getEffectSystem_8013C824()->mEmitterManager;
                    manager->forceDeleteEmitter(field_0x2ec[i]);
                    gone++;
                    field_0x2ec[i] = NULL;
                }
            } else {
                gone++;
            }
        }
    }
    if (gone == 3) {
        field_0x35c = 0;
        field_0x664 = 0;
        field_0x350 = 8;
    }
}

void MapScreen_804B1288::fn_8040E700() {
    if (field_0x65a == 0) {
        field_0x664 = 0;
        field_0x350 = 8;
    }
}

// FIXME: 96.81. Missing/extra: ours extends the u16 angle again (clrlwi r0, r3, 16) before
// the third range test; the original keeps the one clrlwi'd copy in r0. Pool literals
// (3.4f, the sqrtf doubles, 50.0f, 32768/pi) are first used by unwritten functions below.
void MapScreen_804B1288::fn_8040E3A0() {
    f32 speed = 3.4f + lbl_8050DB0C.field_0x4c[311] / 100.0f;
    f32 x = lbl_8048C95C[field_0x340 * 4 + field_0x348][0];
    f32 y = lbl_8048C95C[field_0x340 * 4 + field_0x348][1];
    f32 dx = x - field_0x394;
    f32 dy = y - field_0x398;
    f32 dist = std::sqrtf(dx * dx + dy * dy);
    if (field_0x664) {
        if (dist < speed + 50.0f) {
            field_0x42c = 0;
            field_0x350 = 9;
        } else {
            field_0x394 += speed * dx / dist;
            field_0x398 += speed * dy / dist;
            field_0x2b8[0]->startSound(JAISoundID(0x20064));
        }
    } else if (dist < speed) {
        field_0x394 = x;
        field_0x398 = y;
        field_0x42c = 0;
        field_0x350 = 9;
    } else {
        field_0x394 += speed * dx / dist;
        field_0x398 += speed * dy / dist;
        field_0x2b8[0]->startSound(JAISoundID(0x20064));
    }
    if (field_0x394 == x) {
        field_0x360 = 0x10;
    } else {
        u16 angle = JMath::atanTable_.atan2_(-dx, -dy) *
                    (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180());
        if (angle < 0x2000 || angle >= 0xE000) {
            field_0x360 = 0x18;
        } else if (angle >= 0x2000 && angle < 0x6000) {
            field_0x360 = 8;
        } else if (angle >= 0x6000 && angle <= 0xA000) {
            field_0x360 = 0x10;
        } else {
            field_0x360 = 0;
        }
    }
}

void MapScreen_804B1288::fn_8040E298() {
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

void MapScreen_804B1288::fn_8040E278() {
    if (++field_0x384 > 100) {
        field_0x350 = 0x15;
    }
}

// FIXME: 92.50, different (NONMATCHING). Only the fade argument differs: the original
// computes 1 - field_0x426 * 0.125 unfused (fmuls, fsubs) and after the first
// search('Ttlv11') of the inlined fn_80411A94; ours evaluates the argument first, fused
// (fnmsubs). The rest, including the 1.0f fade and the stage-name copy, is the same.
#ifdef NONMATCHING
void MapScreen_804B1288::fn_8040CE90() {
    field_0x426++;
    if (field_0x426 >= 8) {
        setStageName();
        strcpy(((J2DTextBox*)field_0x10->search('Ttlv11'))->getStringPtr(), field_0x438);
        strcpy(((J2DTextBox*)field_0x10->search('Ttlv12'))->getStringPtr(), field_0x438);
        fn_80411A94(1.0f);
        field_0x350 = 0xB;
    } else {
        if (field_0x426 < 5) {
            f32 y = 7.0f * (field_0x426 * field_0x426) / 25.0f;
            field_0x228[1][0]->moveFromCentre_80402E08(0.0f, y);
            field_0x228[1][1]->moveFromCentre_80402E08(0.0f, y);
        } else if (field_0x426 <= 10) {
            s32 n = 10 - field_0x426;
            f32 y = 7.0f * (n * n) / 25.0f;
            field_0x228[1][0]->moveFromCentre_80402E08(0.0f, y);
            field_0x228[1][1]->moveFromCentre_80402E08(0.0f, y);
        }
        fn_80411A94(1.0f - field_0x426 * 0.125f);
    }
}
#endif

// FIXME: 92.47, different (NONMATCHING). Only the fade argument differs: the original
// computes 1 - field_0x426 * 0.125 unfused (fmuls, fsubs) and after the first
// search('Ttlv11') of the inlined fn_80411A94; ours evaluates the argument first, fused
// (fnmsubs). The rest, including the 1.0f fade and the stage-name copy, is the same.
#ifdef NONMATCHING
void MapScreen_804B1288::fn_8040C950() {
    field_0x426++;
    if (field_0x426 >= 8) {
        setStageName();
        strcpy(((J2DTextBox*)field_0x10->search('Ttlv11'))->getStringPtr(), field_0x438);
        strcpy(((J2DTextBox*)field_0x10->search('Ttlv12'))->getStringPtr(), field_0x438);
        fn_80411A94(1.0f);
        field_0x350 = 0xB;
    } else {
        if (field_0x426 < 5) {
            f32 y = -7.0f * (field_0x426 * field_0x426) / 25.0f;
            field_0x228[0][0]->moveFromCentre_80402E08(0.0f, y);
            field_0x228[0][1]->moveFromCentre_80402E08(0.0f, y);
        } else if (field_0x426 <= 10) {
            s32 n = 10 - field_0x426;
            f32 y = -7.0f * (n * n) / 25.0f;
            field_0x228[0][0]->moveFromCentre_80402E08(0.0f, y);
            field_0x228[0][1]->moveFromCentre_80402E08(0.0f, y);
        }
        fn_80411A94(1.0f - field_0x426 * 0.125f);
    }
}
#endif

// FIXME: 99.81. regalloc-only: the original loads field_0x3b8 into f0 and the pane's
// field_0xc into f2 for `offset`; ours swaps them. 0.5f / 400.0f are pooled by unwritten
// functions.
void MapScreen_804B1288::fn_8040B93C() {
    if (!field_0x4->fn_804446A4()) {
        return;
    }
    if (field_0x4->field_0x4c == 0) {
        field_0x380 = 0;
        field_0x428 = 0;
        field_0x208[field_0x340]->mPane->show();
        field_0x208[field_0x340]->mPane->setAlpha(field_0x208[field_0x340]->field_0x28);
        field_0x3b8 = field_0x208[field_0x340]->field_0xc - 400.0f;
        f32 offset = field_0x3b8 - field_0x208[field_0x340]->field_0xc;
        field_0x208[field_0x340]->moveFromCentre_80402E08(0.0f, offset);
        field_0x3ac = field_0x208[field_0x340]->mPane->mGlobalBounds.i.x +
                      field_0x208[field_0x340]->mPane->getWidth() / 2;
        field_0x3b0 = offset + (field_0x3b8 + field_0x10->search(lbl_8048C4F8[field_0x340])->getBounds()->i.y +
                                field_0x208[field_0x340]->mPane->getHeight() / 2);
        if (field_0x662 == 0) {
            field_0x350 = 0x11;
        } else {
            field_0x208[field_0x340]->moveFromCentre_80402E08(0.0f, 0.0f);
            lbl_8050EC80.getCurrentFile().field_0x5a[0] |= (u8)(1 << field_0x340);
            if (field_0x340 == 7) {
                field_0x38c |= 1 << (field_0x340 * 4 + 1);
            } else {
                field_0x38c |= 1 << (field_0x340 * 4 + 2);
            }
            lbl_8053AEC4->fn_8042EDAC(0x24F, 0);
            field_0x350 = 0x12;
        }
    } else if (field_0x658) {
        field_0x658 = 0;
        field_0x350 = 0x15;
    } else {
        if (field_0x65d == 0) {
            for (int i = 0; i < 15; i++) {
                field_0x278[i]->field_0x26 = 0;
                field_0x65d = 1;
            }
        }
        field_0x350 = 0xF;
    }
}

// FIXME: 99.68. regalloc-only: in state 1 the original loads field_0x3b8 into f0 and the
// pane's field_0xc into f4 for `offset`; ours swaps them. The pool literals 2.0f, 0.5f,
// 400.0f are first used by unwritten functions.
void MapScreen_804B1288::fn_8040B5A0() {
    field_0x380++;
    s32 bob = (field_0x380 % 36) / 9;
    switch (field_0x428) {
    case 0:
        field_0x3b8 += 2.0f;
        if (field_0x3b8 >= field_0x208[field_0x340]->field_0xc) {
            field_0x3b8 = field_0x208[field_0x340]->field_0xc;
            field_0x208[field_0x340]->field_0x22 = 0;
            field_0x208[field_0x340]->moveFromCentre_80402E08(0.0f, 0.0f);
            field_0x428 = 1;
            field_0x2c8->startSound(JAISoundID(0x30081));
        } else {
            field_0x208[field_0x340]->moveFromCentre_80402E08(
                0.0f, field_0x3b8 - field_0x208[field_0x340]->field_0xc +
                          lbl_80543890[bob] * JUTVideo::getManager()->getEfbHeight() / 192.0f);
        }
        field_0x3ac = field_0x208[field_0x340]->mPane->mGlobalBounds.i.x +
                      field_0x208[field_0x340]->mPane->getWidth() / 2;
        field_0x3b0 = field_0x3b8 + field_0x10->search(lbl_8048C4F8[field_0x340])->getBounds()->i.y +
                      field_0x208[field_0x340]->mPane->getHeight() / 2;
        break;
    case 1:
        field_0x3b8 -= 2.0f;
        if (field_0x3b8 <= field_0x208[field_0x340]->field_0xc - 400.0f) {
            lbl_8050EC80.getCurrentFile().field_0x5a[0] |= (u8)(1 << field_0x340);
            if (field_0x340 == 7) {
                field_0x38c |= 1 << (field_0x340 * 4 + 1);
            } else {
                field_0x38c |= 1 << (field_0x340 * 4 + 2);
            }
            field_0x428 = 0;
            field_0x662 = 1;
            lbl_8053AEC4->fn_8042EDAC(0x24F, 0);
            field_0x350 = 0x12;
        }
        f32 offset = field_0x3b8 - field_0x208[field_0x340]->field_0xc;
        field_0x3ac = field_0x208[field_0x340]->mPane->mGlobalBounds.i.x +
                      field_0x208[field_0x340]->mPane->getWidth() / 2;
        field_0x3b0 = offset + (field_0x3b8 + field_0x10->search(lbl_8048C4F8[field_0x340])->getBounds()->i.y +
                                field_0x208[field_0x340]->mPane->getHeight() / 2);
        break;
    }
}

void MapScreen_804B1288::fn_8040B388() {
    switch (field_0x428) {
    case 0:
        if (lbl_8053AEC4->fn_8042EC7C()) {
            field_0x428 = 1;
            gAudioMgr_8053AF80->fn_80458880(0x57, -1);
        }
        break;
    case 1:
        if (lbl_8053AD30->fn_80309C4C(0x1300, -1)) {
            if (field_0x340 == 0 || field_0x340 == 1 || field_0x340 == 2 || field_0x340 == 3 ||
                field_0x340 == 4 || field_0x340 == 5 || field_0x340 == 6)
            {
                field_0x348 = 2;
            } else {
                field_0x348 = 1;
            }
            field_0x34c = field_0x348;
            hideSubStagePanes();
            field_0x394 = lbl_8048C95C[field_0x340 * 4 + field_0x348][0];
            field_0x398 = lbl_8048C95C[field_0x340 * 4 + field_0x348][1];
            field_0x428 = 2;
        }
        break;
    case 2:
        if (lbl_8053AEC4->fn_8042EBEC()) {
            field_0x428 = 0;
            if (field_0x658) {
                field_0x658 = 0;
                field_0x350 = 0x15;
            } else {
                if (field_0x65d == 0) {
                    for (int i = 0; i < 15; i++) {
                        field_0x278[i]->field_0x26 = 0;
                        field_0x65d = 1;
                    }
                }
                field_0x350 = 0xF;
            }
        }
        break;
    }
}


// FIXME: 96.44. Missing/extra (one instruction): in the field_0xd4 loop the original
// multiplies scale * field_0x10 after fn_8040271C (keeping field_0x10 in f30), ours before
// it (an extra fmr); the table / field_0x18 loads of each division are also swapped. The
// "cleared" test after the stage branch is written in both branches: the original has
// its own copy of it in the stage 7 branch.
void MapScreen_804B1288::fn_804096E8() {
    u8 faded = false;
    u8 cleared = false;
    u8 listed = false;
    u8 moved = false;
    field_0x41e++;
    if (field_0x340 == 7) {
        if (field_0x41e >= 0) {
            // The original stores 0, then 1.
            field_0x640 = 0;
            field_0x640 = 1;
            f32 t = field_0x41e / 20.0f;
            if (t > 1.0f) {
                t = 1.0f;
            }
            fn_80408F38(1.0f - t);
            if (field_0x41e >= 20) {
                faded = true;
            }
        }
        if (field_0x41e == 1) {
            hideSubStagePanes();
        } else if (field_0x41e > 15) {
            cleared = true;
        }
    } else {
        if (field_0x340 < 7) {
            if (field_0x41e >= 0) {
                field_0xc8->fn_80402B10(20, 0, lbl_8048C86C[field_0x340], 0.0f);
                field_0xc8->fn_80402940(20, 0, lbl_8048C87C[field_0x340], 0.0f);
                faded = field_0xc8->fn_8040271C(20, lbl_8048C850[field_0x340] / field_0xc8->field_0x18, 1.0f);
                f32 scale = lbl_8048C850[field_0x340] / field_0xc8->field_0x18 - 1.0f;
                f32 dx = scale * (field_0xcc->field_0x10 - field_0xc8->field_0x10);
                f32 dy = scale * (field_0xcc->field_0x14 - field_0xc8->field_0x14);
                field_0xcc->fn_80402B10(20, 0, dx + lbl_8048C86C[field_0x340], 0.0f);
                field_0xcc->fn_80402940(20, 0, dy + lbl_8048C87C[field_0x340], 0.0f);
                field_0xcc->fn_8040271C(20, lbl_8048C850[field_0x340] / field_0xc8->field_0x18, 1.0f);
                dx = scale * (field_0xd0->field_0x10 - field_0xc8->field_0x10);
                dy = scale * (field_0xd0->field_0x14 - field_0xc8->field_0x14);
                field_0xd0->fn_80402B10(20, 0, dx + lbl_8048C86C[field_0x340], 0.0f);
                field_0xd0->fn_80402940(20, 0, dy + lbl_8048C87C[field_0x340], 0.0f);
                field_0xd0->fn_8040271C(20, lbl_8048C850[field_0x340] / field_0xc8->field_0x18, 1.0f);
                for (int i = 0; i < 18; i++) {
                    dy = scale * field_0xd4[i]->field_0x14;
                    dx = scale * field_0xd4[i]->field_0x10;
                    field_0xd4[i]->fn_8040271C(20, lbl_8048C850[field_0x340] / field_0xc8->field_0x18, 1.0f);
                    field_0xd4[i]->fn_80402B10(20, 0, dx, 0.0f);
                    field_0xd4[i]->fn_80402940(20, 0, dy, 0.0f);
                }
            }
        } else {
            faded = true;
        }
        if (field_0x41e == 1) {
            hideSubStagePanes();
        } else if (field_0x41e > 15) {
            cleared = true;
        }
    }
    s32 rows = field_0x370;
    if (rows >= 8) {
        rows = 7;
    }
    if (field_0x41e >= 0 && field_0x41e <= rows * 2 + 16) {
        for (int i = 0; i < rows + 1; i++) {
            if (field_0x41e >= i * 2) {
                field_0x28[i][0]->fn_80402B10(16, 0, -150.0f, 0.0f);
                for (int j = 0; j < 3; j++) {
                    field_0x28[i][j]->fn_80402628(16, 0, field_0x28[i][j]->field_0x28);
                }
            }
        }
    } else if (field_0x41e > rows * 2 + 16) {
        listed = true;
    }
    if (field_0x657 == 0) {
        if (field_0x41e >= 10 && field_0x41e <= 30) {
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 2; j++) {
                    moved = field_0x228[i][j]->fn_80402940(15, 0, -150.0f, 0.0f);
                }
            }
            field_0x238->fn_80402940(15, 0, -150.0f, 0.0f);
            field_0x23c->fn_80402940(15, 0, -150.0f, 0.0f);
        } else if (field_0x41e > 30) {
            moved = true;
        }
    } else {
        moved = true;
    }
    if (faded && cleared && listed && moved) {
        if (field_0x657) {
            field_0x340 = field_0x364;
            field_0x348 = 0;
            setStageName();
            strcpy(((J2DTextBox*)field_0x10->search('Ttlv11'))->getStringPtr(), field_0x438);
            strcpy(((J2DTextBox*)field_0x10->search('Ttlv12'))->getStringPtr(), field_0x438);
            setStageName();
            strcpy(((J2DTextBox*)field_0x10->search('Ttlv13'))->getStringPtr(), field_0x438);
            strcpy(((J2DTextBox*)field_0x10->search('Ttlv14'))->getStringPtr(), field_0x438);
            field_0x39c = -250.0f;
            field_0x350 = 5;
        } else {
            field_0x350 = 0xB;
        }
    }
    if (field_0x41e >= 0) {
        gAudioMgr_8053AF80->fn_80458880(0x54, -1);
    }
}

void MapScreen_804B1288::fn_80409450() {
    field_0x41e++;
    if (field_0x663 == 0) {
        if (field_0x41e == 1) {
            for (int i = 0; i < 4; i++) {
                field_0x2b8[i]->field_0x44 = 0;
                field_0x2b8[i]->field_0x45 = 0;
                field_0x2b8[i]->startSound(JAISoundID(0x10001));
            }
        } else if (field_0x41e == 0x1A) {
            for (int i = 0; i < 4; i++) {
                field_0x2b8[i]->field_0x44 = 0;
                field_0x2b8[i]->field_0x45 = 0;
                field_0x2b8[i]->startSound(JAISoundID(0x2000E));
            }
        }
    }
    if (field_0x41e >= 0x24 || field_0x663 != 0) {
        lbl_80529DEC.field_0x120 = field_0x340;
        lbl_80529DEC.field_0x124 = field_0x348;
        if ((field_0x340 == 0 && field_0x348 == 2) || (field_0x340 == 1 && field_0x348 == 2) ||
            (field_0x340 == 2 && field_0x348 == 2) || (field_0x340 == 3 && field_0x348 == 2) ||
            (field_0x340 == 4 && field_0x348 == 2) || (field_0x340 == 5 && field_0x348 == 2) ||
            (field_0x340 == 6 && field_0x348 == 2) || (field_0x340 == 7 && field_0x348 == 1))
        {
            field_0x340 = 10;
            field_0x348 = 2;
            lbl_80529DEC.field_0x24b = 0;
        }
        lbl_80529DEC.field_0x118 = field_0x340;
        lbl_80529DEC.field_0x11c = field_0x348;
        if (isStageFlagSet(field_0x340, field_0x348)) {
            lbl_80529DEC.field_0x254[0] = 1;
        } else {
            lbl_80529DEC.field_0x254[0] = 0;
        }
        field_0x350 = 0x16;
    }
}

void MapScreen_804B1288::fn_80409330() {
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

// FIXME: 99.06. scheduling+regalloc: for each rotate call the original loads field_0x1c
// before field_0x18 and multiplies the height first; ours does the width first.
void MapScreen_804B1288::fn_80409014() {
    field_0x422++;
    if (field_0x422 >= 15) {
        field_0x422 = 0;
        if (field_0x228[0][0]->mPane->mIsVisible) {
            field_0x228[0][0]->mPane->hide();
            field_0x228[0][1]->mPane->show();
            field_0x228[1][0]->mPane->hide();
            field_0x228[1][1]->mPane->show();
        } else {
            field_0x228[0][0]->mPane->show();
            field_0x228[0][1]->mPane->hide();
            field_0x228[1][0]->mPane->show();
            field_0x228[1][1]->mPane->hide();
        }
    }
    J2DPicture* picture = (J2DPicture*)field_0x23c->mPane;
    field_0x420++;
    if (field_0x420 >= 180) {
        field_0x420 = 0;
    }
    if (field_0x420 < 45) {
        f32 t = (field_0x420 * field_0x420) / 2025.0f;
        picture->setBlendRatio(1.0f - t, t, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
        picture->rotate(field_0x23c->field_0x18 * 0.5f, field_0x23c->field_0x1c * 0.5f, J2DROTATE_Z, 90.0f);
    } else if (field_0x420 < 90) {
        s32 n = field_0x420 - 45;
        f32 t = (n * n) / 2025.0f;
        picture->setBlendRatio(t, 1.0f - t, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
        picture->rotate(field_0x23c->field_0x18 * 0.5f, field_0x23c->field_0x1c * 0.5f, J2DROTATE_Z, 90.0f);
    } else if (field_0x420 < 135) {
        s32 n = field_0x420 - 90;
        f32 t = (n * n) / 2025.0f;
        picture->setBlendRatio(1.0f - t, t, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
        picture->rotate(field_0x23c->field_0x18 * 0.5f, field_0x23c->field_0x1c * 0.5f, J2DROTATE_Z, 270.0f);
    } else {
        s32 n = field_0x420 - 135;
        f32 t = (n * n) / 2025.0f;
        picture->setBlendRatio(t, 1.0f - t, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
        picture->rotate(field_0x23c->field_0x18 * 0.5f, field_0x23c->field_0x1c * 0.5f, J2DROTATE_Z, 270.0f);
    }
}

void MapScreen_804B1288::fn_80408F38(f32 alpha) {
    for (int i = 0; i < 9; i++) {
        field_0x14->search(lbl_8048C5D0[i])->setAlpha(field_0x642[i] * alpha);
    }
}

// FIXME: 90.43. Missing/extra: for the selected row's text-box move the original loads the
// y operands (getBounds()->i.y, row[0]->field_0xc, row[2]->field_0xc) before the second
// getBounds call and adds them after it (f29-f31 saved, five FPRs); ours computes y before
// that call (three saved FPRs, 8 instructions fewer). Everything else is the same.
void MapScreen_804B1288::fn_804083D8() {
    f32 scaleX = field_0x28[0][0]->field_0x18 / 72.0f;
    f32 scaleY = field_0x28[0][0]->field_0x1c / 36.0f;
    field_0x42e = 30;
    for (int i = 0; i < 8; i++) {
        if (i == field_0x340) {
            field_0x28[i][0]->fn_80402CE0(1.0f / scaleX, 1.0f / scaleY);
            field_0x28[i][1]->mPane->resize(72.0f, 36.0f);
            field_0x28[i][2]->mPane->move(
                field_0x28[i][2]->field_0x8 + (field_0x28[i][0]->field_0x8 - field_0x28[i][0]->mPane->getBounds()->i.x),
                field_0x28[i][2]->field_0xc + (field_0x28[i][0]->field_0xc - field_0x28[i][0]->mPane->getBounds()->i.y));
            ((J2DPicture*)field_0x28[i][0]->mPane)->setBlackWhite(field_0x328[3], field_0x310[3]);
            ((J2DPicture*)field_0x28[i][1]->mPane)->setBlackWhite(field_0x328[4], field_0x310[4]);
            ((J2DTextBox*)field_0x28[i][2]->mPane)->setBlackWhite(field_0x328[5], field_0x310[5]);
        } else {
            field_0x28[i][0]->fn_80402CE0(1.0f, 1.0f);
            field_0x28[i][1]->mPane->resize(field_0x28[i][1]->field_0x18, field_0x28[i][1]->field_0x1c);
            field_0x28[i][2]->mPane->move(field_0x28[i][2]->field_0x8, field_0x28[i][2]->field_0xc);
            ((J2DPicture*)field_0x28[i][0]->mPane)->setBlackWhite(field_0x328[0], field_0x310[0]);
            ((J2DPicture*)field_0x28[i][1]->mPane)->setBlackWhite(field_0x328[1], field_0x310[1]);
            ((J2DTextBox*)field_0x28[i][2]->mPane)->setBlackWhite(field_0x328[2], field_0x310[2]);
        }
    }
    if (field_0x340 < 8) {
        for (int i = 0; i < 8; i++) {
            if (i != field_0x340) {
                field_0x10->search('Ndata01')->appendChild(field_0x28[i][0]->mPane);
            }
        }
        field_0x10->search('Ndata01')->appendChild(field_0x28[field_0x340][0]->mPane);
    }
}

void MapScreen_804B1288::fn_80408114() {
    if (field_0x350 == 0xB || field_0x350 == 0xC || field_0x350 == 0xD) {
        for (int i = 0; i < 8; i++) {
            f32 alpha;
            if (i == field_0x340) {
                field_0x40e[i]++;
                if (field_0x40e[i] < 15) {
                    alpha = 155.0f * (field_0x40e[i] / 15.0f);
                    if (alpha > 150.0f) {
                        alpha = 150.0f;
                    } else if (alpha < 0.0f) {
                        alpha = 0.0f;
                    }
                } else {
                    if (field_0x40e[i] >= 115) {
                        field_0x40e[i] = 15;
                    }
                    f32 t;
                    if (field_0x40e[i] < 65) {
                        t = (field_0x40e[i] - 15) / 50.0f;
                    } else {
                        t = (100 - (field_0x40e[i] - 15)) / 50.0f;
                    }
                    alpha = 105.0f * t + 150.0f;
                    if (alpha > 255.0f) {
                        alpha = 255.0f;
                    } else if (alpha < 150.0f) {
                        alpha = 150.0f;
                    }
                }
            } else if (i == field_0x344) {
                if (field_0x40e[i] >= 15) {
                    field_0x40e[i] = 15;
                }
                if (field_0x40e[i] > 0) {
                    field_0x40e[i]--;
                }
                alpha = 155.0f * (field_0x40e[i] / 15.0f);
                if (alpha > 150.0f) {
                    alpha = 150.0f;
                } else if (alpha < 0.0f) {
                    alpha = 0.0f;
                }
            } else {
                field_0x40e[i] = 0;
                alpha = 0.0f;
            }
            field_0xc->search(lbl_8048C2B8[i])->setAlpha(alpha);
        }
    } else {
        for (int i = 0; i < 8; i++) {
            field_0x40e[i] = 0;
            field_0xc->search(lbl_8048C2B8[i])->setAlpha(0);
        }
    }
}

// FIXME: 75.38, missing/extra (NONMATCHING). Every call, field and constant is right; two
// things differ: the original computes `frame` before fn_8013B414 (ours computes it at its
// first use, after the first draw, which shifts the schedule), and it indexes the sine table
// straight from (counter % 128) (clrlslwi r0, r0, 27, 9) where sinShort's s16 argument gives
// ours slwi / extsh / rlwinm. A const method (no `counter` local) gives 59.9 %.
#ifdef NONMATCHING
void MapScreen_804B1288::fn_80406F68(f32 x, f32 y) {
    SpriteParams_801F5AE0 params;
    JGeometry::TVec3f pos;
    s32 counter = field_0x380;
    s32 frame = (counter % 40) / 10;
    JGeometry::TBox2f box(0.0f, 0.0f, 256.0f, 192.0f);
    gGameRoot2_8053AB10->fn_8013B414(&box, true);
    pos.x = x * (256.0f / (s32)JUTVideo::getManager()->getFbWidth());
    pos.y = y * (192.0f / (s32)JUTVideo::getManager()->getEfbHeight()) - 5.0f;
    pos.z = 0.0f;
    GXSetTevDirect(GX_TEVSTAGE0);
    GXSetTevDirect(GX_TEVSTAGE1);
    GXSetTevDirect(GX_TEVSTAGE2);
    GXSetTevDirect(GX_TEVSTAGE3);
    JGeometry::TVec3f bob(pos.x, pos.y + JMath::sincosTable_.sinShort((counter % 128) * 0x800), pos.z);
    params.field_0x26 = 0;
    params.field_0x0[2] = 2;
    void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
    gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&bob, res, 0xC6, &params, 1.0f, 1.0f);
    pos.x -= 16.0f;
    pos.y += 3.0f;
    params.field_0x0[2] = 3;
    s32 cel = lbl_8048CB58[frame] + 0xC0;
    res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
    gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, cel, &params, 1.0f, 1.0f);
    pos.x += 32.0f;
    params.field_0x0[2] = 4;
    cel = lbl_8048CB58[frame] + 0xC3;
    res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
    gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, cel, &params, 1.0f, 1.0f);
}
#endif

// FIXME: 96.30. Order and registers only: setRotScaleMtx's six sin / cos results land in
// other saved FPRs (target sx f28, sy f27, sz f26, cx f31, cy f30, scale f29) and its
// products are scheduled differently (unproven inline, see game/RotMatrix.h); and the
// else branch of the stage choice reloads field_0x340 where the original copies the
// compared register (mr r30, r3).
void MapScreen_804B1288::fn_804069E8() {
    SpriteParams_801F5AE0 params;
    s32 picked = -1;
    s32 stage;
    if (field_0x340 == 10 && field_0x348 == 2) {
        stage = lbl_80529DEC.field_0x120;
    } else {
        stage = field_0x340;
    }
    Mtx mtx;
    JGeometry::TVec3f rot(0.0f, 0.0f, 0.0f);
    setRotScaleMtx(mtx, rot, lbl_8050DB0C.field_0x8);
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    params.mMtx = mtx;
    for (int i = 0; i < 4; i++) {
        if (fn_80404074(stage, i, &picked)) {
            JGeometry::TVec3f pos;
            pos.x = lbl_8048C95C[stage * 4 + i][0] * (384.0f / (s32)JUTVideo::getManager()->getFbWidth());
            pos.y = lbl_8048C95C[stage * 4 + i][1] * (288.0f / (s32)JUTVideo::getManager()->getEfbHeight());
            pos.z = 0.0f;
            GXSetTevDirect(GX_TEVSTAGE0);
            GXSetTevDirect(GX_TEVSTAGE1);
            GXSetTevDirect(GX_TEVSTAGE2);
            GXSetTevDirect(GX_TEVSTAGE3);
            params.field_0x26 = 0;
            params.field_0x1d = 1;
            params.field_0x1c = 1;
            params.field_0x10.a = 255.0f * field_0x400;
            if (i == picked) {
                if (field_0x35c == 10) {
                    if (field_0x2ec[0] == NULL) {
                        field_0x2ec[0] = spawnEmitter_8013CC50(0, &pos, 0x7B, 1, 0, NULL, NULL, true, 1.0f);
                        field_0x2ec[0]->setStatus(JPAEMIT_Immortal);
                    }
                    if (field_0x2ec[1] == NULL) {
                        field_0x2ec[1] = spawnEmitter_8013CC50(0, &pos, 0x7C, 1, 0, NULL, NULL, true, 1.0f);
                        field_0x2ec[1]->setStatus(JPAEMIT_Immortal);
                    }
                    if (field_0x2ec[2] == NULL) {
                        field_0x2ec[2] = spawnEmitter_8013CC50(0, &pos, 0x7D, 1, 0, NULL, NULL, true, 1.0f);
                        field_0x2ec[2]->setStatus(JPAEMIT_Immortal);
                    }
                    if (field_0x2f8[i] != NULL) {
                        field_0x2f8[i]->becomeInvalidEmitter();
                        field_0x2f8[i] = NULL;
                    }
                }
                if (field_0x35c <= 10) {
                    void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_gc_sob.bin");
                    gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, 0x25B, &params, 1.0f, 1.0f);
                }
            } else {
                void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_gc_sob.bin");
                gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, 0x25A, &params, 1.0f, 1.0f);
            }
        }
    }
}

void MapScreen_804B1288::fn_80406634(s32 stage, bool raised) {
    SpriteParams_801F5AE0 params;
    field_0x432++;
    if (field_0x432 >= 6000) {
        field_0x432 = 0;
    }
    f32 x = lbl_8048C95C[field_0x340 * 4 + field_0x348][0];
    f32 y = lbl_8048C95C[field_0x340 * 4 + field_0x348][1];
    if (!raised) {
        y -= 50.0f;
    }
    JGeometry::TVec3f pos;
    pos.x = x * (384.0f / (s32)JUTVideo::getManager()->getFbWidth()) + field_0x3a0;
    pos.y = y * (288.0f / (s32)JUTVideo::getManager()->getEfbHeight()) + field_0x3a4;
    pos.z = 0.0f;
    GXSetTevDirect(GX_TEVSTAGE0);
    GXSetTevDirect(GX_TEVSTAGE1);
    GXSetTevDirect(GX_TEVSTAGE2);
    GXSetTevDirect(GX_TEVSTAGE3);
    params.field_0x26 = 0;
    params.field_0x1d = 1;
    params.field_0x1c = 1;
    params.field_0x10.a = 255.0f * field_0x400;
    if (field_0x308[0] == NULL) {
        field_0x308[0] = spawnEmitter_8013CC50(0, &pos, 0x32C, 1, 0, NULL, NULL, true, 1.0f);
        field_0x308[0]->setGlobalEnvColor(lbl_8048CB80[field_0x340][0], lbl_8048CB80[field_0x340][1],
                                          lbl_8048CB80[field_0x340][2]);
    } else {
        setEmitterPos_8013CB44(field_0x308[0], &pos, 1, 0, true);
    }
    if (field_0x308[1] == NULL) {
        field_0x308[1] = spawnEmitter_8013CC50(0, &pos, 0x184, 1, 0, NULL, NULL, true, 1.0f);
    } else {
        setEmitterPos_8013CB44(field_0x308[1], &pos, 1, 0, true);
    }
    s32 frame = field_0x432 % 6 + lbl_8048CB68[stage];
    void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
    gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, frame, &params, 1.0f, 1.0f);
}

// FIXME: 99.98. regalloc-only: the original keeps the render mode's fbWidth (read before
// the first search) in r24, ours in r26.
void MapScreen_804B1288::fn_80405FC0() {
    f32 offset = 384.0f * (0.5f * (field_0x10->search('Pli02')->getBounds()->i.x -
                                   field_0x10->search('Pli01')->getBounds()->i.x)) /
                 (s32)JUTVideo::getManager()->getFbWidth();
    // Computed but never used (the players are offset by `offset` both ways); only
    // its two search calls are left.
    f32 offsetY = 288.0f * (0.5f * (field_0x10->search('Pli01')->getBounds()->i.y -
                                    field_0x10->search('Pli03')->getBounds()->i.y)) /
                  (s32)JUTVideo::getManager()->getEfbHeight();
    for (int i = 0; i < 4; i++) {
        JGeometry::TVec3f pos;
        pos.x = lbl_8050DB0C.field_0x4c[274] +
                384.0f * field_0x394 / (s32)JUTVideo::getManager()->getFbWidth();
        pos.y = lbl_8050DB0C.field_0x4c[275] +
                288.0f * field_0x398 / (s32)JUTVideo::getManager()->getEfbHeight() + field_0x39c;
        pos.z = 0.0f;
        if (i % 2) {
            pos.x += offset;
        } else {
            pos.x -= offset;
        }
        if (i < 2) {
            pos.y += offset;
        } else {
            pos.y -= offset;
        }
        SpriteParams_801F5AE0 params;
        params.field_0x0[7] = i + 12;
        params.field_0x26 = 0;
        if (field_0x350 == 0x14 || field_0x350 == 0x16) {
            s32 frame = field_0x41e / 2;
            if (frame <= 13) {
                void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "player_sob.bin");
                gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, frame + 0xDB, &params, 1.0f, 1.0f);
                if (field_0x41e == 26) {
                    if (field_0x2cc[i] == NULL) {
                        JGeometry::TVec3f p(7.0f + pos.x, pos.y - 27.0f, pos.z);
                        field_0x2cc[i] = spawnEmitter_8013CC50(0, &p, 0xCB, 1, 0, NULL, NULL, true, 1.0f);
                        field_0x2cc[i]->setStatus(JPAEMIT_Immortal);
                    }
                    if (field_0x2dc[i] == NULL) {
                        JGeometry::TVec3f p(7.0f + pos.x, pos.y - 27.0f, pos.z);
                        field_0x2dc[i] = spawnEmitter_8013CC50(0, &p, 0xCC, 1, 0, NULL, NULL, true, 1.0f);
                        field_0x2dc[i]->setStatus(JPAEMIT_Immortal);
                    }
                }
            } else {
                void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "player_sob.bin");
                gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, 0xE8, &params, 1.0f, 1.0f);
            }
        } else if (field_0x350 == 2) {
            s32 frame = lbl_8048CC0C[(field_0x358 / 5) % 4];
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "player_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, frame, &params, 1.0f, 1.0f);
        } else if (field_0x350 == 4) {
            params.field_0x10.a = field_0x665;
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "player_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, 0xE8, &params, 1.0f, 1.0f);
        } else {
            if (field_0x358 >= 30) {
                field_0x358 = 0;
            }
            s32 frame = field_0x360 + lbl_8048CB94[field_0x358];
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "player_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, frame, &params, 1.0f, 1.0f);
        }
    }
}

// FIXME: 96.30. Missing/extra: the original keeps field_0x340 in r5 from the stage test
// through the table reads (lbl_80543880 / 88 / lbl_8048C90C); ours loads it again after the
// test (and before lbl_8048C90C), two instructions more. A `stage` local folds 0-5 instead
// of the original's 0 / 1-5 / 6 (98.26 %, different).
void MapScreen_804B1288::fn_80405D00(s32 dir) {
    s32 count = 0;
    for (int i = 0; i < 4; i++) {
        if (field_0x38c & (1 << (field_0x340 * 4 + i))) {
            count++;
        }
    }
    if (count == 1) {
        return;
    }
    if ((field_0x340 == 0 || field_0x340 == 1 || field_0x340 == 2 || field_0x340 == 3 || field_0x340 == 4 ||
         field_0x340 == 5 || field_0x340 == 6) &&
        !(field_0x38c & (1 << (field_0x340 * 4 + 1))))
    {
        if (field_0x348 == 0) {
            if (lbl_80543880[field_0x340] == dir) {
                field_0x348 = 2;
                field_0x34c = field_0x348;
                hideSubStagePanes();
                gAudioMgr_8053AF80->fn_80458880(0x26, -1);
            }
        } else if (field_0x348 == 2) {
            if (lbl_80543888[field_0x340] == dir) {
                field_0x348 = 0;
                field_0x34c = field_0x348;
                hideSubStagePanes();
                gAudioMgr_8053AF80->fn_80458880(0x26, -1);
            }
        }
        return;
    }
    if (field_0x348 < count - 1 && dir == lbl_8048C90C[field_0x340][field_0x348]) {
        field_0x348++;
        field_0x34c = field_0x348;
        hideSubStagePanes();
        gAudioMgr_8053AF80->fn_80458880(0x26, -1);
    }
    if (field_0x348 > 0 && dir == lbl_8048C924[field_0x340][field_0x348 - 1]) {
        field_0x348--;
        field_0x34c = field_0x348;
        hideSubStagePanes();
        gAudioMgr_8053AF80->fn_80458880(0x26, -1);
    }
}

void MapScreen_804B1288::fn_80405B10(s32 dir) {
    s32 count = 0;
    for (int i = 0; i < 4; i++) {
        if (field_0x38c & (1 << (field_0x340 * 4 + i))) {
            count++;
        }
    }
    if (count == 1) {
        return;
    }
    if (field_0x34c < count - 1 && dir == lbl_8048C93C[field_0x340][field_0x34c]) {
        field_0x34c++;
        field_0x348 = lbl_8048C88C[field_0x340][field_0x34c];
        hideSubStagePanes();
        gAudioMgr_8053AF80->fn_80458880(0x26, -1);
    }
    if (field_0x34c > 0 && dir == lbl_8048C94C[field_0x340][field_0x34c - 1]) {
        field_0x34c--;
        field_0x348 = lbl_8048C88C[field_0x340][field_0x34c];
        hideSubStagePanes();
        gAudioMgr_8053AF80->fn_80458880(0x26, -1);
    }
}

// Dead-stripped: fn_80405D00, fn_80405B10 (each followed by sound 0x26) and fn_8040B388
// inline it after every change of the sub-stage (seven identical copies).
void MapScreen_804B1288::hideSubStagePanes() {
    if (field_0x65e == 1) {
        field_0x120[0]->field_0x22 = 0;
        field_0x120[0]->field_0x26 = 0;
        field_0x120[1]->field_0x26 = 0;
        field_0x65e = 0;
    }
}

void MapScreen_804B1288::fn_804054B8() {
    if (!field_0x10->search('Ndata10')->mIsVisible) {
        field_0x10->search('Ndata10')->show();
        field_0x10->search('Ndata10')->move(field_0x394, field_0x398);
        field_0x424 = 0;
        for (int i = 0; i < 4; i++) {
            field_0x10->search(lbl_8048C618[i][0])->hide();
            field_0x10->search(lbl_8048C618[i][1])->hide();
        }
        s32 count = 0;
        for (int i = 0; i < 4; i++) {
            if (field_0x38c & (1 << (field_0x340 * 4 + i))) {
                count++;
            }
        }
        if (count > 1) {
            s32 stage = field_0x340;
            if (fn_804045F8(stage)) {
                if ((stage == 0 || stage == 1 || stage == 2 || stage == 3 || stage == 4 || stage == 5 ||
                     stage == 6) &&
                    !(field_0x38c & (1 << (stage * 4 + 1))))
                {
                    if (field_0x34c == 0) {
                        field_0x10->search(lbl_8048C618[lbl_80543880[stage]][0])->show();
                    }
                    if (field_0x34c == 2) {
                        field_0x10->search(lbl_8048C618[lbl_80543888[field_0x340]][0])->show();
                    }
                } else {
                    if (field_0x34c < count - 1) {
                        field_0x10->search(lbl_8048C618[lbl_8048C90C[stage][field_0x34c]][0])->show();
                    }
                    if (field_0x34c > 0) {
                        field_0x10->search(lbl_8048C618[lbl_8048C924[field_0x340][field_0x34c - 1]][0])
                            ->show();
                    }
                }
            } else {
                if (count == 4) {
                    count = 3;
                }
                if (field_0x34c < count - 1) {
                    field_0x10->search(lbl_8048C618[lbl_8048C93C[stage][field_0x34c]][0])->show();
                }
                if (field_0x34c > 0) {
                    field_0x10->search(lbl_8048C618[lbl_8048C94C[field_0x340][field_0x34c - 1]][0])->show();
                }
            }
        }
    }
    field_0x424++;
    if (field_0x424 >= 15) {
        field_0x424 = 0;
        for (int i = 0; i < 4; i++) {
            if (field_0x10->search(lbl_8048C618[i][0])->mIsVisible) {
                field_0x10->search(lbl_8048C618[i][0])->hide();
                field_0x10->search(lbl_8048C618[i][1])->show();
            } else if (field_0x10->search(lbl_8048C618[i][1])->mIsVisible) {
                field_0x10->search(lbl_8048C618[i][0])->show();
                field_0x10->search(lbl_8048C618[i][1])->hide();
            }
        }
    }
    for (int i = 0; i < 4; i++) {
        field_0x10->search(lbl_8048C618[i][0])
            ->move(field_0x3bc[i][0] + lbl_8050DB0C.field_0x4c[276 + i * 2],
                   field_0x3dc[i][0] + lbl_8050DB0C.field_0x4c[277 + i * 2]);
        field_0x10->search(lbl_8048C618[i][1])
            ->move(field_0x3bc[i][1] + lbl_8050DB0C.field_0x4c[276 + i * 2],
                   field_0x3dc[i][1] + lbl_8050DB0C.field_0x4c[277 + i * 2]);
    }
}

void MapScreen_804B1288::fn_80404D70() {
    field_0x370 = 0;
    if (fn_803B8EBC(7)) {
        for (int stage = 1; stage < 7; stage++) {
            hideStagePanes(stage);
        }
        field_0xcc->mPane->show();
        field_0x370 = 7;
        return;
    }
    for (int i = 0; i < 8; i++) {
        if (fn_803B8EBC(i)) {
            s32 stage = i + 1;
            if (lbl_8050EC80.getCurrentFile().field_0x62[0] & (u8)(1 << stage)) {
                hideStagePanes(stage);
                field_0x378 += 200;
                field_0x37c += 200;
            } else {
                field_0x364 = stage;
            }
            field_0x370++;
        } else {
            return;
        }
        if (i == 5) {
            field_0xcc->mPane->show();
        }
    }
}

// Dead-stripped: fn_80404D70 inlines it in both of its loops.
void MapScreen_804B1288::hideStagePanes(s32 stage) {
    switch (stage) {
    case 1:
        field_0xc->search('P21')->hide();
        field_0xc->search('P22')->hide();
        field_0xc->search('P23')->hide();
        break;
    case 2:
        field_0xc->search('P31')->hide();
        field_0xc->search('P32')->hide();
        field_0xc->search('P33')->hide();
        field_0xc->search('P34')->hide();
        break;
    case 3:
        field_0xc->search('P41')->hide();
        field_0xc->search('P42')->hide();
        field_0xc->search('P43')->hide();
        break;
    case 4:
        field_0xc->search('P51')->hide();
        field_0xc->search('P52')->hide();
        field_0xc->search('P53')->hide();
        break;
    case 5:
        field_0xc->search('P61')->hide();
        field_0xc->search('P62')->hide();
        break;
    case 6:
        field_0xc->search('P71')->hide();
        field_0xc->search('P72')->hide();
        field_0xc->search('P73')->hide();
        break;
    }
}

// FIXME: 95.74 %. The original materialises one 0 (r4) for the stores to field_0xc8-0xd0 and
// the field_0xd4 loop (loop pointer in r5) and a second li r0, 0 for field_0x228; ours uses a new
// li r0, 0 inside the loop (one instruction fewer); all else equal.
void MapScreen_804B1288::fn_804049DC() {
    if (field_0x38c & 0x10) {
        field_0xc8->field_0x20 = 0;
        field_0xc8->field_0x22 = 0;
        field_0xc8->field_0x24 = 0;
        field_0xc8->field_0x26 = 0;
        field_0xcc->field_0x20 = 0;
        field_0xcc->field_0x22 = 0;
        field_0xcc->field_0x24 = 0;
        field_0xcc->field_0x26 = 0;
        field_0xd0->field_0x20 = 0;
        field_0xd0->field_0x22 = 0;
        field_0xd0->field_0x24 = 0;
        field_0xd0->field_0x26 = 0;
        for (int i = 0; i < 18; i++) {
            field_0xd4[i]->field_0x20 = 0;
            field_0xd4[i]->field_0x22 = 0;
            field_0xd4[i]->field_0x24 = 0;
            field_0xd4[i]->field_0x26 = 0;
        }
        field_0x228[0][0]->field_0x22 = 0;
        field_0x228[0][1]->field_0x22 = 0;
        field_0x228[1][0]->field_0x22 = 0;
        field_0x228[1][1]->field_0x22 = 0;
        field_0x238->field_0x22 = 0;
        field_0x23c->field_0x22 = 0;
        field_0x41e = 0;
        if (field_0x340 <= 7) {
            for (int i = field_0x340 * 4; i < field_0x340 * 4 + 4; i++) {
                field_0x128[i]->field_0x26 = 0;
                field_0x128[i]->mPane->setAlpha(0);
                field_0x128[i]->setAlpha_804023D0(field_0x10->search(lbl_8048C2F8[i][1]), field_0x641);
            }
            for (int i = field_0x340 * 3; i < field_0x340 * 3 + 3; i++) {
                field_0x1a8[i]->field_0x26 = 0;
                field_0x1a8[i]->mPane->setAlpha(0);
            }
            field_0x208[field_0x340]->field_0x26 = 0;
            field_0x208[field_0x340]->mPane->setAlpha(0);
        }
        field_0x10->search('Ndata10')->hide();
        for (int i = 0; i < 8; i++) {
            field_0x28[i][0]->moveFromCentre_80402E08(-150.0f, 0.0f);
            field_0x28[i][0]->field_0x20 = 0;
            for (int j = 0; j < 3; j++) {
                field_0x28[i][j]->mPane->setAlpha(0);
                field_0x28[i][j]->field_0x26 = 0;
            }
        }
        field_0x350 = 0x13;
    } else {
        field_0x668 = 4;
        gAudioMgr_8053AF80->fn_80458880(0xb, -1);
    }
}

// FIXME: 95.74 %. The original materialises one 0 (r4) for the stores to field_0xc8-0xd0 and
// the field_0xd4 loop (loop pointer in r5) and a second li r0, 0 for field_0x228; ours uses a new
// li r0, 0 inside the loop (one instruction fewer); all else equal.
void MapScreen_804B1288::fn_80404648() {
    if (field_0x38c & 0x10) {
        field_0xc8->field_0x20 = 0;
        field_0xc8->field_0x22 = 0;
        field_0xc8->field_0x24 = 0;
        field_0xc8->field_0x26 = 0;
        field_0xcc->field_0x20 = 0;
        field_0xcc->field_0x22 = 0;
        field_0xcc->field_0x24 = 0;
        field_0xcc->field_0x26 = 0;
        field_0xd0->field_0x20 = 0;
        field_0xd0->field_0x22 = 0;
        field_0xd0->field_0x24 = 0;
        field_0xd0->field_0x26 = 0;
        for (int i = 0; i < 18; i++) {
            field_0xd4[i]->field_0x20 = 0;
            field_0xd4[i]->field_0x22 = 0;
            field_0xd4[i]->field_0x24 = 0;
            field_0xd4[i]->field_0x26 = 0;
        }
        field_0x228[0][0]->field_0x22 = 0;
        field_0x228[0][1]->field_0x22 = 0;
        field_0x228[1][0]->field_0x22 = 0;
        field_0x228[1][1]->field_0x22 = 0;
        field_0x238->field_0x22 = 0;
        field_0x23c->field_0x22 = 0;
        field_0x41e = 0;
        if (field_0x340 <= 7) {
            for (int i = field_0x340 * 4; i < field_0x340 * 4 + 4; i++) {
                field_0x128[i]->field_0x26 = 0;
                field_0x128[i]->mPane->setAlpha(0);
                field_0x128[i]->setAlpha_804023D0(field_0x10->search(lbl_8048C2F8[i][1]), field_0x641);
            }
            for (int i = field_0x340 * 3; i < field_0x340 * 3 + 3; i++) {
                field_0x1a8[i]->field_0x26 = 0;
                field_0x1a8[i]->mPane->setAlpha(0);
            }
            field_0x208[field_0x340]->field_0x26 = 0;
            field_0x208[field_0x340]->mPane->setAlpha(0);
        }
        for (int i = 0; i < 8; i++) {
            field_0x28[i][0]->moveFromCentre_80402E08(-150.0f, 0.0f);
            field_0x28[i][0]->field_0x20 = 0;
            for (int j = 0; j < 3; j++) {
                field_0x28[i][j]->mPane->setAlpha(0);
                field_0x28[i][j]->field_0x26 = 0;
            }
        }
        field_0x10->search('Ndata10')->hide();
        field_0x350 = 0x13;
    } else {
        field_0x668 = 4;
        gAudioMgr_8053AF80->fn_80458880(0xb, -1);
    }
}

BOOL MapScreen_804B1288::fn_804045F8(s32 bit) {
    if (gGameRoot_8053AAF8->field_0x7e) {
        return (lbl_8050EC80.getCurrentFile().field_0x5a[0] & (u8)(1 << bit)) ? TRUE : FALSE;
    }
    return FALSE;
}

bool MapScreen_804B1288::fn_80404270(s32 stage) {
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

// FIXME: 84.17. Every field, call and branch matches; only the loop differs: ours
// hoists the save row's flags address out of the unrolled loop, the original keeps
// only the row index (r12) and recomputes lbl_8050EC80 + index * 0xC8 each pass,
// which costs it r31 and a stack frame (the same as tu_804023D0's fn_80403514).
bool MapScreen_804B1288::fn_80404074(s32 stage, s32 index, s32* out) {
    s32 count = 0;
    for (int i = 0; i < 4; i++) {
        if (isStageFlagSet(stage, i)) {
            count++;
        }
    }
    s32 skip;
    if (stage == 7) {
        if (count <= 1) {
            count--;
        }
        skip = 1;
        if (field_0x388 != -1 && (field_0x350 == 6 || field_0x664 == 1)) {
            if (field_0x388 == 0) {
                *out = 2;
            } else if (field_0x388 == 1) {
                *out = 3;
            }
        }
    } else {
        if (count <= 2) {
            count--;
        }
        skip = 2;
        if (field_0x388 != -1 && (field_0x350 == 6 || field_0x664 == 1)) {
            if (field_0x388 == 0) {
                *out = 1;
            } else if (field_0x388 == 1) {
                *out = 3;
            }
        }
    }
    if (index != skip && (index > count || index == *out)) {
        return true;
    }
    return false;
}

void MapScreen_804B1288::fn_80403F50() {
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

void MapScreen_804B1288::fn_80403EAC() {
    u8 done = field_0x240[0]->fn_80402940(0x13, 0, 100.0f, 0.0f);
    for (s32 i = 1; i < 12; i++) {
        field_0x240[i]->fn_80402940(0x13, 0, 100.0f, 0.0f);
    }
    if (done) {
        field_0x42a = 100;
        field_0x659 = 2;
    }
}

void MapScreen_804B1288::fn_80403E24() {
    field_0x42a--;
    if (field_0x42a <= 0) {
        for (int i = 0; i < 12; i++) {
            field_0x240[i]->field_0x22 = 0;
        }
        field_0x659 = 3;
    }
}

void MapScreen_804B1288::fn_80403D84() {
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
void MapScreen_804B1288::slidePane(s16 frame, s16 frames) {
    f32 t = (f32)(frame * frame) / (f32)(frames * frames);
    field_0x2b4->moveFromCentre_80402E08(0.0f, -200.0f + 200.0f * t);
    field_0x2b4->mPane->setAlpha(t * field_0x2b4->field_0x28);
}
