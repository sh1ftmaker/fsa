#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/Message.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/J2D/J2DGrafContext.h"
#include "JSystem/J2D/J2DPicture.h"
#include <string.h>
#include <stdio.h>
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
#include "JSystem/J2D/J2DAnmLoader.h"
#include "JSystem/JKernel/JKRFileLoader.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "dolphin/os.h"
#include "game/ActorManager.h"

// Read by fn_804054B8 and fn_80405D00 (lbzx), and fn_8040B5A0 (lhax).
static const u8 lbl_80543880[8] = {3, 1, 1, 0, 1, 1, 0, 1};
static const u8 lbl_80543888[8] = {1, 3, 3, 2, 3, 3, 2, 3};
static const s16 lbl_80543890[4] = {0, 1, 2, 1};

// .rodata 0x8048C1F8-0x8048CC1C in address order: map.blo pane-tag tables (u64 J2D tags),
// the screen's s32 / s16 / u8 index tables and the rupee-digit texture names (lbl_804B1260, .data).
// Message ids per stage: the stage names (setStageName), 0x244.. (580..), and the four texts
// fn_8040873C reads. Six separate arrays: written as one s32[6][8] the code changes
// (fn_8040873C 96.0 -> 94.7 %, fn_8040BC1C 97.7 -> 95.9 %).
static const s32 lbl_8048C1F8[8] = {288, 289, 290, 291, 292, 293, 294, 295};
static const s32 lbl_8048C218[8] = {580, 581, 582, 583, 584, 585, 586, 587};
static const s32 lbl_8048C238[8] = {0, 4, 8, 12, 16, 20, 24, 28};
static const s32 lbl_8048C258[8] = {1, 5, 9, 14, 17, 21, 25, 29};
static const s32 lbl_8048C278[8] = {2, 6, 10, 13, 18, 22, 26, 30};
static const s32 lbl_8048C298[8] = {3, 7, 11, 15, 19, 23, 27, 31};
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

inline bool MapScreen_804B1288::isCursorState() {
    if (field_0x350 == 2 || field_0x350 == 3 || field_0x350 == 4 || field_0x350 == 6 ||
        field_0x350 == 7 || field_0x350 == 8 || field_0x350 == 9 || field_0x350 == 15 ||
        field_0x350 == 16 || field_0x350 == 17 || field_0x350 == 18 || field_0x350 == 20 ||
        field_0x350 == 21 || field_0x350 == 22)
    {
        return true;
    }
    return false;
}

// Inline (fn_8040873C, fn_8040BC1C twice: each copy has its own name buffer): for stages
// 0-7, the picture w0<stage>_0<sub>.bti into Pplan01 / Pplan02 and the four texts of
// field_0x564.
inline void MapScreen_804B1288::loadStageInfo() {
    if (field_0x340 != 8 && field_0x340 != 9 && field_0x340 != 10 && field_0x340 != 11 && field_0x340 != 12) {
        char name[11];
        snprintf(name, sizeof(name), "w0%d_0%d.bti", field_0x340 + 1, field_0x348 + 1);
        ResTIMG* picture = (ResTIMG*)fn_80403184('TIMG', name);
        ((J2DPicture*)field_0x10->search('Pplan01'))->remove((u8)0);
        ((J2DPicture*)field_0x10->search('Pplan01'))->append(picture, 1.0f);
        ((J2DPicture*)field_0x10->search('Pplan01'))->setBlendRatio(0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
        ((J2DPicture*)field_0x10->search('Pplan02'))->remove((u8)0);
        ((J2DPicture*)field_0x10->search('Pplan02'))->append(picture, 1.0f);
        ((J2DPicture*)field_0x10->search('Pplan02'))->setBlendRatio(0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
        getMessageText_803832BC(field_0x564[0], lbl_8048C238[field_0x340], 0, NULL, NULL);
        getMessageText_803832BC(field_0x564[1], lbl_8048C258[field_0x340], 0, NULL, NULL);
        getMessageText_803832BC(field_0x564[2], lbl_8048C278[field_0x340], 0, NULL, NULL);
        getMessageText_803832BC(field_0x564[3], lbl_8048C298[field_0x340], 0, NULL, NULL);
    }
}

// Inlined where the stage name changes (fn_804096E8 twice, fn_8040C950, fn_8040CE90); its
// debug-name literals sit in fn_804096E8's block of .sdata2, so it is an inline.
inline void MapScreen_804B1288::setStageName() {
    if (field_0x340 < 8) {
        getMessageText_803832BC(field_0x438, lbl_8048C1F8[field_0x340], 0, NULL, NULL);
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

// Not auto-inlined: the constructor calls fn_804149C0 out of line in the original (docs/INLINING.md, pragma rule 1).
#pragma auto_inline off
bool fn_804149C0() {
    return lbl_8050EC80.getCurrentFile().field_0x67 & 1;
}
#pragma auto_inline reset

void fn_8041499C(JUtility::TColor* dst, const JUtility::TColor* src) {
    *dst = *src;
}

// FIXME(shotgun): 94.36 %, missing/extra + reloc-name
MapScreen_804B1288::MapScreen_804B1288() {
    lbl_8053ADF8 = this;
    lbl_80529DEC.field_0x23e[1] = 0;
    field_0x40c = 0;
    JKRExpHeap* heap = lbl_80529DEC.mHeap;
    OSReport("***** heap check\x81\x40\x98\x41\x91\xb1\x8b\xf3\x82\xab\x97\x65\x97\xca\x81\x40%fK "
             "\x8e\x63\x82\xe8\x8b\xf3\x82\xab\x97\x65\x97\xca\x81\x40%fK *****\n",
             heap->getFreeSize() * (1.0f / 1024.0f), heap->getTotalFreeSize() * (1.0f / 1024.0f));
    field_0x4 = new cls_804B1B60;
    field_0x8 = new cls_804B0288(lbl_80529DEC.field_0x34, false);
    field_0xc = new J2DScreen;
    field_0xc->setPriority("map.blo", 0x1100000, lbl_80529DEC.field_0x34);
    OSInitFastCast();
    field_0x1c = static_cast<J2DAnmTransform*>(
        J2DAnmLoaderDataBase::load(JKRFileLoader::getGlbResource("map.bck", lbl_80529DEC.field_0x34)));
    field_0xc->search('Ndata08')->setAnimation(field_0x1c);
    field_0x20 = static_cast<J2DAnmColor*>(
        J2DAnmLoaderDataBase::load(JKRFileLoader::getGlbResource("map.bpk", lbl_80529DEC.field_0x34)));
    field_0xc->setAnimation(field_0x20);
    field_0x10 = new J2DScreen;
    field_0x10->setPriority("map_area.blo", 0x1100000, lbl_80529DEC.field_0x34);
    field_0x14 = new J2DScreen;
    field_0x14->setPriority("map_ten.blo", 0x1100000, lbl_80529DEC.field_0x34);
    field_0x18 = new J2DScreen;
    field_0x18->setPriority("main_message_window.blo", 0x1100000, lbl_80529DEC.field_0x20);
    field_0x10->search('Ndata10')->hide();
    field_0x10->search('Pli01')->hide();
    field_0x10->search('Pli02')->hide();
    field_0x10->search('Pli03')->hide();
    field_0x10->search('Pli04')->hide();
    field_0x10->search('Pms02')->hide();
    field_0x10->search('Pms03')->hide();
    if (gGameRoot_8053AAF8->field_0x7e == 1) {
        field_0x10->search('Ndata08')->show();
    } else {
        field_0x10->search('Ndata08')->hide();
    }
    if (fn_804149C0()) {
        field_0x10->search('Ndata03')->show();
        s32 tens = fn_803B8C90() / 10;
        s32 ones = fn_803B8C90() % 10;
        const ResTIMG* digit = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1260[tens]));
        static_cast<J2DPicture*>(field_0x10->search('Pfsk01'))->changeTexture(digit, 0);
        static_cast<J2DPicture*>(field_0x10->search('Pfs01'))->changeTexture(digit, 0);
        digit = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1260[ones]));
        static_cast<J2DPicture*>(field_0x10->search('Pfsk02'))->changeTexture(digit, 0);
        static_cast<J2DPicture*>(field_0x10->search('Pfs02'))->changeTexture(digit, 0);
    } else {
        field_0x10->search('Ndata03')->hide();
    }
    field_0x10->search('Pphu01')->hide();
    field_0x10->search('Pp01')->hide();
    field_0x10->search('Pp02')->hide();
    field_0x10->search('Pp03')->hide();
    field_0x10->search('Pp04')->hide();
    static_cast<J2DTextBox*>(field_0x10->search('Ttlv11'))->setFont(lbl_8053AD78->mFont);
    static_cast<J2DTextBox*>(field_0x10->search('Ttlv12'))->setFont(lbl_8053AD78->mFont);
    static_cast<J2DTextBox*>(field_0x10->search('Ttlv13'))->setFont(lbl_8053AD78->mFont);
    static_cast<J2DTextBox*>(field_0x10->search('Ttlv14'))->setFont(lbl_8053AD78->mFont);
    for (s32 i = 0; i < 8; i++) {
        static_cast<J2DTextBox*>(field_0x10->search(lbl_8048C658[i * 3 + 2]))->setFont(lbl_8053AD78->mFont);
        getMessageText_803832BC(static_cast<J2DTextBox*>(field_0x10->search(lbl_8048C658[i * 3 + 2]))->getStringPtr(),
                                lbl_8048C218[i], 0, NULL, NULL);
    }
    for (s32 i = 0; i < 8; i++) {
        getMessageText_803832BC(field_0x474[i], lbl_8048C1F8[i], 0, NULL, NULL);
    }
    static_cast<J2DTextBox*>(field_0x18->search('Tmt02'))->setFont(lbl_8053AD78->mFont);
    static_cast<J2DTextBox*>(field_0x18->search('Tmt01'))->setFont(lbl_8053AD78->mFont);
    field_0x404 = field_0x10->search('Plvcu')->getBounds()->i.x - field_0x10->search(lbl_8048C658[3])->getBounds()->i.x;
    field_0x408 = field_0x10->search('Plvcu')->getBounds()->i.y - field_0x10->search(lbl_8048C658[3])->getBounds()->i.y;
    field_0x10->search('Plvcu')->setAlpha(0);
    f32 y = 0.5f * (field_0x10->search(lbl_8048C658[0])->getBounds()->i.y + field_0x10->search(lbl_8048C658[6])->getBounds()->i.y);
    field_0x10->search(lbl_8048C658[3])->move(field_0x10->search(lbl_8048C658[0])->getBounds()->i.x, y);
    field_0x10->search(lbl_8048C658[4])->move(field_0x10->search(lbl_8048C658[1])->getBounds()->i.x,
                                              field_0x10->search(lbl_8048C658[1])->getBounds()->i.y);
    field_0x10->search(lbl_8048C658[5])->move(field_0x10->search(lbl_8048C658[2])->getBounds()->i.x,
                                              field_0x10->search(lbl_8048C658[2])->getBounds()->i.y);
    for (s32 i = 0; i < 3; i++) {
        field_0x10->search(lbl_8048C658[i + 3])->resize(field_0x10->search(lbl_8048C658[i])->getWidth(),
                                                        field_0x10->search(lbl_8048C658[i])->getHeight());
    }
    for (s32 i = 0; i < 8; i++) {
        for (s32 j = 0; j < 3; j++) {
            field_0x28[i][j] = new PaneCtrl_804AF86C;
            field_0x28[i][j]->setPane_80402F04(field_0x10->search(lbl_8048C658[i * 3 + j]));
        }
    }
    for (s32 i = 0; i < 15; i++) {
        field_0x278[i] = new PaneCtrl_804AF86C;
        field_0x278[i]->setPane_80402F04(field_0x10->search(lbl_8048C658[24 + i]));
        for (s32 j = 0; j < 20; j++) {
            field_0x278[i]->fn_80402628(20, field_0x278[i]->field_0x28, 0);
        }
    }
    field_0xc8 = new PaneCtrl_804AF86C;
    field_0xc8->setPane_80402F04(field_0xc->search('Ptestmap'));
    field_0xcc = new PaneCtrl_804AF86C;
    field_0xcc->setPane_80402F04(field_0xc->search('Ptower'));
    field_0xcc->mPane->hide();
    field_0xd0 = new PaneCtrl_804AF86C;
    field_0xd0->setPane_80402F04(field_0xc->search('Ndata08'));
    for (s32 i = 0; i < 18; i++) {
        field_0xd4[i] = new PaneCtrl_804AF86C;
        field_0xd4[i]->setPane_80402F04(field_0xc->search(lbl_8048CAC8[i]));
    }
    field_0x11c = new PaneCtrl_804AF86C;
    field_0x11c->setPane_80402F04(field_0x10->search('Ndata07'));
    field_0x120[0] = new PaneCtrl_804AF86C;
    field_0x120[0]->setPane_80402F04(field_0x10->search('Pplan02'));
    field_0x120[1] = new PaneCtrl_804AF86C;
    field_0x120[1]->setPane_80402F04(field_0x10->search('Pplan01'));
    for (s32 i = 0; i < 32; i++) {
        field_0x128[i] = new PaneCtrl_804AF86C;
        field_0x128[i]->setPane_80402F04(field_0x10->search(lbl_8048C2F8[i][0]));
        if (i != 2 && i != 6 && i != 10 && i != 14 && i != 18 && i != 22 && i != 26 && i != 29) {
            static_cast<J2DPicture*>(field_0x10->search(lbl_8048C2F8[i][0]))
                ->setBlackWhite(JUtility::TColor(255, 255, 120, 0), JUtility::TColor(255, 255, 120, 255));
            static_cast<J2DPicture*>(field_0x10->search(lbl_8048C2F8[i][1]))
                ->setBlackWhite(JUtility::TColor(255, 255, 120, 0), JUtility::TColor(255, 255, 120, 255));
        }
    }
    for (s32 i = 0; i < 24; i++) {
        field_0x1a8[i] = new PaneCtrl_804AF86C;
        field_0x1a8[i]->setPane_80402F04(field_0x10->search(lbl_8048C658[39 + (i / 3) * 3 + i % 3]));
    }
    for (s32 i = 0; i < 8; i++) {
        field_0x208[i] = new PaneCtrl_804AF86C;
        field_0x208[i]->setPane_80402F04(field_0x10->search(lbl_8048C4F8[8 + i]));
    }
    field_0x228[0][0] = new PaneCtrl_804AF86C;
    field_0x228[0][0]->setPane_80402F04(field_0x10->search('Plvcu01'));
    field_0x228[0][1] = new PaneCtrl_804AF86C;
    field_0x228[0][1]->setPane_80402F04(field_0x10->search('Plvcu02'));
    field_0x228[1][0] = new PaneCtrl_804AF86C;
    field_0x228[1][0]->setPane_80402F04(field_0x10->search('Plvcu03'));
    field_0x228[1][1] = new PaneCtrl_804AF86C;
    field_0x228[1][1]->setPane_80402F04(field_0x10->search('Plvcu04'));
    field_0x238 = new PaneCtrl_804AF86C;
    field_0x238->setPane_80402F04(field_0x10->search('Plvse'));
    field_0x23c = new PaneCtrl_804AF86C;
    field_0x23c->setPane_80402F04(field_0x10->search('Pms01'));
    for (s32 i = 0; i < 12; i++) {
        field_0x240[i] = new PaneCtrl_804AF86C;
    }
    field_0x240[0]->setPane_80402F04(field_0x18->search('Pmmwk02'));
    field_0x240[1]->setPane_80402F04(field_0x18->search('Pmmwk01'));
    field_0x240[2]->setPane_80402F04(field_0x18->search('Pmmw09'));
    field_0x240[3]->setPane_80402F04(field_0x18->search('Pmmw08'));
    field_0x240[4]->setPane_80402F04(field_0x18->search('Pmmw07'));
    field_0x240[5]->setPane_80402F04(field_0x18->search('Pmmw06'));
    field_0x240[6]->setPane_80402F04(field_0x18->search('Pmmw05'));
    field_0x240[7]->setPane_80402F04(field_0x18->search('Pmmw04'));
    field_0x240[8]->setPane_80402F04(field_0x18->search('Pmmw03'));
    field_0x240[9]->setPane_80402F04(field_0x18->search('Pmmw02'));
    field_0x240[10]->setPane_80402F04(field_0x18->search('Pmmw01'));
    field_0x240[11]->setPane_80402F04(field_0x18->search('Pmmtp'));
    static_cast<J2DPicture*>(field_0x240[11]->mPane)->setBlackWhite(JUtility::TColor(0), JUtility::TColor(0));
    field_0x270[0] = new PaneCtrl_804AF86C;
    field_0x270[0]->setPane_80402F04(field_0x18->search('Tmt01'));
    field_0x270[1] = new PaneCtrl_804AF86C;
    field_0x270[1]->setPane_80402F04(field_0x18->search('Tmt02'));
    field_0x42a = 0;
    field_0x42c = 0;
    field_0x659 = 0;
    field_0x65a = 0;
    field_0x5dc[0] = 0;
    field_0x390 = 0;
    for (s32 i = 0; i < 12; i++) {
        field_0x240[i]->moveFromCentre_80402E08(0.0f, 100.0f);
    }
    for (s32 i = 0; i < 4; i++) {
        field_0x2b8[i] = new cls_804B20F8(i, NULL, NULL, 8);
    }
    field_0x2c8 = new ActorSoundObj_804578F4(NULL, NULL, 8);
    const ResTIMG* plan = static_cast<const ResTIMG*>(fn_80403184('TIMG', "w01_01.bti"));
    static_cast<J2DPicture*>(field_0x10->search('Pplan01'))->append(plan, 1.0f);
    plan = static_cast<const ResTIMG*>(fn_80403184('TIMG', "w01_01.bti"));
    static_cast<J2DPicture*>(field_0x10->search('Pplan02'))->append(plan, 1.0f);
    static_cast<J2DPicture*>(field_0x23c->mPane)->append(static_cast<const ResTIMG*>(fn_80403184('TIMG', "kk_mStick_02.bti")), 1.0f);
    static_cast<J2DPicture*>(field_0x23c->mPane)->setBlendRatio(1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    field_0x2b4 = lbl_8053AED8->field_0x8;
    lbl_8053AED8->fn_80440D78(static_cast<const ResTIMG*>(fn_804030C4('TIMG', "kk_kettei_01.bti")));
    lbl_8053AED8->fn_80440CE4(static_cast<const ResTIMG*>(fn_804030C4('TIMG', "kk_modoru_01.bti")));
    for (s32 i = 0; i < 2; i++) {
        for (s32 j = 0; j < 2; j++) {
            JUtility::TColor white = static_cast<J2DPicture*>(field_0x28[i][j]->mPane)->getWhite();
            fn_8041499C(&field_0x310[i * 3 + j], &white);
            JUtility::TColor black = static_cast<J2DPicture*>(field_0x28[i][j]->mPane)->getBlack();
            fn_8041499C(&field_0x328[i * 3 + j], &black);
            field_0x328[i * 3 + j].a = 0xFF;
        }
    }
    for (s32 i = 0; i < 2; i++) {
        JUtility::TColor white = static_cast<J2DTextBox*>(field_0x28[i][2]->mPane)->getWhite();
        fn_8041499C(&field_0x310[i * 3 + 2], &white);
        JUtility::TColor black = static_cast<J2DTextBox*>(field_0x28[i][2]->mPane)->getBlack();
        fn_8041499C(&field_0x328[i * 3 + 2], &black);
    }
    field_0x641 = field_0x10->search('Ppoint02')->getAlpha();
    for (s32 i = 0; i < 4; i++) {
        for (s32 j = 0; j < 2; j++) {
            field_0x3bc[i][j] = field_0x10->search(lbl_8048C618[i][j])->getBounds()->i.x;
            field_0x3dc[i][j] = field_0x10->search(lbl_8048C618[i][j])->getBounds()->i.y;
        }
    }
    for (s32 i = 0; i < 11; i++) {
        field_0x64b[i] = field_0x10->search(lbl_8048C4F8[16 + i])->getAlpha();
    }
    for (s32 i = 0; i < 9; i++) {
        field_0x642[i] = field_0x14->search(lbl_8048C5D0[i])->getAlpha();
    }
    field_0x65b = 0xFF;
    for (s32 i = 0; i < 4; i++) {
        field_0x2cc[i] = NULL;
        field_0x2dc[i] = NULL;
        field_0x2f8[i] = NULL;
    }
    for (s32 i = 0; i < 3; i++) {
        field_0x2ec[i] = NULL;
    }
    for (s32 i = 0; i < 2; i++) {
        field_0x308[i] = NULL;
    }
    field_0x348 = lbl_80529DEC.field_0x11c;
    field_0x340 = lbl_80529DEC.field_0x118;
    field_0x438[0] = 0;
    field_0x456[0] = 0;
    field_0x3a8 = 1.0f;
    field_0x3b0 = 0.0f;
    field_0x3ac = 0.0f;
    field_0x3b8 = 0.0f;
    field_0x3b4 = 0.0f;
    field_0x400 = 0.0f;
    field_0x3fc = 0.0f;
    field_0x65c = 0;
    field_0x420 = 0;
    field_0x422 = 0;
    field_0x424 = 0;
    field_0x426 = 0;
    field_0x428 = 0;
    field_0x378 = 0;
    field_0x37c = 0;
    field_0x364 = 0;
    field_0x368 = -1;
    field_0x36c = -1;
    field_0x370 = 0;
    field_0x374 = 0;
    field_0x3a8 = 1.0f;
    field_0x640 = 0;
    field_0x41e = 0;
    field_0x358 = 0;
    field_0x35c = 0;
    field_0x380 = 0;
    field_0x38c = 0;
    field_0x384 = 0;
    field_0x388 = -1;
    field_0x41e = 0;
    field_0x360 = 0x10;
    field_0x42e = 0;
    field_0x394 = 0.0f;
    field_0x398 = 0.0f;
    field_0x39c = 0.0f;
    field_0x3a0 = 0.0f;
    field_0x3a4 = 0.0f;
    field_0x350 = 0;
    field_0x354 = field_0x350;
    for (s32 i = 0; i < 8; i++) {
        field_0x40e[i] = 0;
    }
    field_0x640 = 0;
    field_0x661 = 0;
    field_0x65f = 0;
    field_0x660 = 0;
    field_0x430 = 0;
    field_0x662 = 0;
    field_0x663 = 0;
    field_0x664 = 0;
    field_0x432 = 0;
    field_0x436 = 0;
    field_0x434 = 0;
    field_0x665 = 0;
    field_0x65d = 0;
    field_0x65e = 0;
    if (gGameRoot_8053AAF8->field_0x7e) {
        field_0x656 = 0;
    } else {
        field_0x656 = 1;
    }
    field_0x657 = 0;
    field_0x658 = 0;
    if (!lbl_80529DEC.field_0x23e[1]) {
        bool opened = false;
        for (s32 stage = 0; stage < 8; stage++) {
            for (s32 sub = 0; sub < 4; sub++) {
                s32 bit = stage * 4 + sub;
                if ((stage == 0 && sub == 2) || (stage == 1 && sub == 2) || (stage == 2 && sub == 2) ||
                    (stage == 3 && sub == 2) || (stage == 4 && sub == 2) || (stage == 5 && sub == 2) ||
                    (stage == 6 && sub == 2) || (stage == 7 && sub == 1)) {
                    if (fn_804045F8(stage)) {
                        field_0x38c |= 1 << bit;
                    }
                } else if (fn_8019CE40(stage, sub)) {
                    field_0x38c |= 1 << bit;
                } else if (!opened) {
                    field_0x38c |= 1 << bit;
                    opened = true;
                }
            }
        }
    } else {
        field_0x38c = -1;
    }
    field_0xc8->moveFromCentre_80402E08(0.0f, 0.0f);
    field_0xc8->fn_80402CE0(1.0f, 1.0f);
    field_0xc8->mPane->setAlpha(field_0xc8->field_0x28);
    field_0xc8->field_0x20 = 0;
    field_0xc8->field_0x22 = 0;
    field_0xc8->field_0x24 = 0;
    field_0xc8->field_0x26 = 0;
    field_0xcc->moveFromCentre_80402E08(0.0f, 0.0f);
    field_0xcc->fn_80402CE0(1.0f, 1.0f);
    field_0xcc->mPane->setAlpha(field_0xcc->field_0x28);
    field_0xcc->field_0x20 = 0;
    field_0xcc->field_0x22 = 0;
    field_0xcc->field_0x24 = 0;
    field_0xcc->field_0x26 = 0;
    field_0xd0->moveFromCentre_80402E08(0.0f, 0.0f);
    field_0xd0->fn_80402CE0(1.0f, 1.0f);
    field_0xd0->mPane->setAlpha(field_0xd0->field_0x28);
    field_0xd0->field_0x20 = 0;
    field_0xd0->field_0x22 = 0;
    field_0xd0->field_0x24 = 0;
    field_0xd0->field_0x26 = 0;
    for (s32 i = 0; i < 18; i++) {
        field_0xd4[i]->moveFromCentre_80402E08(0.0f, 0.0f);
        field_0xd4[i]->fn_80402CE0(1.0f, 1.0f);
        field_0xd4[i]->mPane->setAlpha(field_0xd4[i]->field_0x28);
        field_0xd4[i]->field_0x20 = 0;
        field_0xd4[i]->field_0x22 = 0;
        field_0xd4[i]->field_0x24 = 0;
        field_0xd4[i]->field_0x26 = 0;
    }
    field_0x11c->moveFromCentre_80402E08(0.0f, 200.0f);
    field_0x11c->field_0x22 = 0;
    fn_80411A94(1.0f);
    if (!lbl_80529DEC.field_0x23e[1] && !(field_0x38c & 0x10)) {
        for (s32 i = 0; i < 2; i++) {
            field_0x228[i][0]->mPane->hide();
            field_0x228[i][1]->mPane->hide();
            for (s32 j = 0; j < 2; j++) {
                field_0x228[i][j]->field_0x22 = 0;
                field_0x228[i][j]->moveFromCentre_80402E08(0.0f, -150.0f);
            }
        }
        field_0x238->mPane->hide();
        field_0x23c->mPane->hide();
    } else {
        for (s32 i = 0; i < 2; i++) {
            field_0x228[i][0]->mPane->show();
            field_0x228[i][1]->mPane->hide();
            for (s32 j = 0; j < 2; j++) {
                field_0x228[i][j]->field_0x22 = 0;
                field_0x228[i][j]->moveFromCentre_80402E08(0.0f, -150.0f);
            }
        }
        field_0x238->mPane->show();
        field_0x23c->mPane->show();
    }
    field_0x238->field_0x22 = 0;
    field_0x238->moveFromCentre_80402E08(0.0f, -150.0f);
    field_0x23c->field_0x22 = 0;
    field_0x23c->moveFromCentre_80402E08(0.0f, -150.0f);
    u8 done;
    do {
        done = field_0x120[0]->fn_80402940(15, 0, 0.0f, -200.0f);
        for (s32 i = 0; i < 2; i++) {
            field_0x120[i]->fn_80402628(15, field_0x120[i]->field_0x28, 0);
        }
    } while (!done);
    for (s32 i = 0; i < 8; i++) {
        field_0xc->search(lbl_8048C2B8[i])->setAlpha(0);
    }
    field_0x2b4->moveFromCentre_80402E08(0.0f, -200.0f);
    field_0x2b4->field_0x22 = 0;
    for (s32 i = 0; i < 32; i++) {
        field_0x128[i]->field_0x26 = 0;
        field_0x128[i]->mPane->setAlpha(0);
        field_0x128[i]->setAlpha_804023D0(field_0x10->search(lbl_8048C2F8[i][1]), field_0x641);
    }
    for (s32 i = 0; i < 24; i++) {
        field_0x1a8[i]->field_0x26 = 0;
        field_0x1a8[i]->mPane->setAlpha(0);
    }
    for (s32 i = 0; i < 8; i++) {
        field_0x208[i]->field_0x26 = 0;
        field_0x208[i]->mPane->setAlpha(0);
    }
    if (!fn_803B8EBC(7)) {
        static_cast<J2DPicture*>(field_0x14->search('Psky'))
            ->setBlackWhite(JUtility::TColor(20, 50, 100, 255), JUtility::TColor(20, 50, 100, 255));
        static_cast<J2DPicture*>(field_0x14->search('Pclo01'))
            ->setBlackWhite(JUtility::TColor(0, 30, 90, 0), JUtility::TColor(0, 30, 90, 255));
        static_cast<J2DPicture*>(field_0x14->search('Pclo02'))
            ->setBlackWhite(JUtility::TColor(40, 90, 170, 0), JUtility::TColor(40, 90, 170, 255));
        static_cast<J2DPicture*>(field_0x14->search('Pclo03'))
            ->setBlackWhite(JUtility::TColor(40, 100, 190, 0), JUtility::TColor(40, 100, 190, 255));
        static_cast<J2DPicture*>(field_0x14->search('Pclo04'))
            ->setBlackWhite(JUtility::TColor(40, 96, 200, 0), JUtility::TColor(40, 96, 200, 255));
        static_cast<J2DPicture*>(field_0x14->search('Pmaptou'))
            ->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(30, 72, 139, 255));
        static_cast<J2DPicture*>(field_0x14->search('Pmapca'))
            ->setBlackWhite(JUtility::TColor(0, 0, 30, 0), JUtility::TColor(100, 200, 255, 255));
    }
    fn_80408F38(0.0f);
    // field_0x1b4[3] (0x1C0): the free heap size the map screen saw last.
    if (lbl_80529DEC.field_0x1b4[3] != 0 && lbl_80529DEC.field_0x1b4[3] != lbl_80529DEC.mHeap->getTotalFreeSize()) {
        OSReport("\x1b[31m\x83\x81\x83\x82\x83\x8a\x82\xaa\x89\xf0\x95\xfa\x82\xb3\x82\xea\x82\xc4\x82\xa2\x82\xdc\x82\xb9\x82\xf1"
                 "(GAME) %8d \x1b[m\n",
                 lbl_80529DEC.field_0x1b4[3] - lbl_80529DEC.mHeap->getTotalFreeSize());
    }
    lbl_80529DEC.field_0x1b4[3] = lbl_80529DEC.mHeap->getTotalFreeSize();
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
    field_0x10->search(0x54746C763131ull /* "Ttlv11" */)->setAlpha(field_0x64b[0] * alpha);
    field_0x10->search(0x54746C763132ull /* "Ttlv12" */)->setAlpha(field_0x64b[1] * alpha);
    field_0x10->search(0x54746C763133ull /* "Ttlv13" */)->setAlpha(field_0x64b[0] * (1.0f - alpha));
    field_0x10->search(0x54746C763134ull /* "Ttlv14" */)->setAlpha(field_0x64b[1] * (1.0f - alpha));
}

// FIXME: 95.84. Missing/extra (two instructions, one saved register): in the stage 10 /
// sub-stage 2 branch the original reads lbl_80529DEC.field_0x120 twice before the two
// 'Pphu01' size searches (r29 / r31) and indexes lbl_8048C9DC with them after the
// 'Ndata06' calls; ours reads it after those calls. All else is the same.
void MapScreen_804B1288::fn_80411230() {
    gGameRoot2_8053AB10->fn_8013B490();
    if (field_0x340 < 7 && !isStageFlagSet(field_0x340, 3) && (isCursorState() || field_0x350 == 0x13)) {
        if (!field_0x10->search('Pphu01')->mIsVisible) {
            field_0x10->search('Pphu01')->show();
        }
        field_0x10->search('Pphu01')->setAlpha(field_0x65b * field_0x3fc);
        f32 height = field_0x10->search('Pphu01')->getHeight();
        f32 width = field_0x10->search('Pphu01')->getWidth();
        field_0x10->search('Pphu01')
            ->move(lbl_8048C9DC[field_0x340][0] - field_0x10->search('Ndata06')->getBounds()->i.x - width / 2,
                   lbl_8048C9DC[field_0x340][1] - field_0x10->search('Ndata06')->getBounds()->i.y - height / 2);
    } else if (field_0x340 == 10 && field_0x348 == 2 && lbl_80529DEC.field_0x120 < 7 &&
               !isStageFlagSet(lbl_80529DEC.field_0x120, 3) && isCursorState())
    {
        if (!field_0x10->search('Pphu01')->mIsVisible) {
            field_0x10->search('Pphu01')->show();
        }
        field_0x10->search('Pphu01')->setAlpha(field_0x65b * field_0x3fc);
        f32 height = field_0x10->search('Pphu01')->getHeight();
        f32 width = field_0x10->search('Pphu01')->getWidth();
        field_0x10->search('Pphu01')
            ->move(lbl_8048C9DC[lbl_80529DEC.field_0x120][0] - field_0x10->search('Ndata06')->getBounds()->i.x -
                       width / 2,
                   lbl_8048C9DC[lbl_80529DEC.field_0x120][1] - field_0x10->search('Ndata06')->getBounds()->i.y -
                       height / 2);
    } else if (field_0x10->search('Pphu01')->mIsVisible == true) {
        field_0x10->search('Pphu01')->hide();
    }
    field_0xc->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
    field_0x14->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
    field_0x10->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
    {
        JGeometry::TBox2f box(0.0f, 0.0f, 384.0f, 288.0f);
        gGameRoot2_8053AB10->fn_8013B414(&box, true);
    }
    if (field_0x340 <= 7 || (field_0x340 == 10 && field_0x348 == 2)) {
        if (isCursorState() || field_0x350 == 0x13) {
            field_0x358++;
            fn_804069E8();
            if (field_0x350 != 0x13) {
                if (field_0x663) {
                    fn_80406634(field_0x340 - 1, true);
                } else {
                    fn_80405FC0();
                    if (field_0x350 == 4) {
                        fn_80406634(field_0x340, false);
                    }
                }
            }
        }
    }
    if (field_0x350 == 0x11) {
        fn_80406F68(field_0x3ac, field_0x3b0);
        JGeometry::TBox2f box(0.0f, 0.0f, 384.0f, 288.0f);
        gGameRoot2_8053AB10->fn_8013B414(&box, true);
    }
    if (field_0x350 == 0x10) {
        field_0x4->fn_80444650();
    } else if (field_0x350 == 0x12) {
        lbl_8053AEC4->fn_8042EB98();
    }
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

void MapScreen_804B1288::fn_804108B4() {
    (lbl_8053ADF8->*lbl_804B10F8[field_0x350])();
    (lbl_8053ADF8->*lbl_804B1230[field_0x659])();
    fn_80408114();
    if (isCursorState()) {
        fn_8040732C();
        if (field_0x65c == 0) {
            field_0x3fc += 0.04f;
            if (field_0x3fc >= 1.0f) {
                field_0x3fc = 1.0f;
                field_0x65c = 1;
            }
        } else if (field_0x65c == 1) {
            field_0x3fc -= 0.0043137255f;
            if (field_0x3fc <= 0.78431374f) {
                field_0x3fc = 0.78431374f;
                field_0x65c = 2;
            }
        } else {
            field_0x3fc += 0.0043137255f;
            if (field_0x3fc >= 1.0f) {
                field_0x3fc = 1.0f;
                field_0x65c = 1;
            }
        }
        if (field_0x400 < 1.0f) {
            field_0x400 += 0.04f;
            if (field_0x400 > 1.0f) {
                field_0x400 = 1.0f;
            }
        } else {
            field_0x400 = 1.0f;
        }
    } else {
        field_0x65c = 0;
        if (field_0x3fc > 0.0f) {
            field_0x3fc -= 1.0f / lbl_8050DB0C.field_0x4c[307];
            if (field_0x3fc < 0.0f) {
                field_0x3fc = 0.0f;
            }
        } else {
            field_0x3fc = 0.0f;
        }
        if (field_0x400 > 0.0f) {
            field_0x400 -= 1.0f / lbl_8050DB0C.field_0x4c[307];
            if (field_0x400 < 0.0f) {
                field_0x400 = 0.0f;
            }
        } else {
            field_0x400 = 0.0f;
        }
    }
    if (lbl_80529DEC.field_0x23e[1]) {
        fn_80409014();
    } else if (field_0x38c & 0x10) {
        fn_80409014();
    }
    if (field_0x350 == 0x16) {
        field_0x668 = 7;
        lbl_80529DEC.field_0x242 = 0;
        lbl_8053AD78->field_0xe9 = 0;
        lbl_8053AD78->field_0x44 = 0;
        lbl_80529DEC.field_0x242 = 0;
    }
    if (field_0x65d) {
        for (int i = 0; i < 15; i++) {
            field_0x278[i]->fn_80402628(20, 0, field_0x278[i]->field_0x28);
        }
    } else {
        for (int i = 0; i < 15; i++) {
            field_0x278[i]->fn_80402628(20, field_0x278[i]->field_0x28, 0);
        }
    }
    if (field_0x65e) {
        field_0x120[0]->fn_80402940(20, 0, -200.0f, 0.0f);
        for (int i = 0; i < 2; i++) {
            field_0x120[i]->fn_80402628(20, 0, field_0x120[i]->field_0x28);
        }
    } else {
        field_0x120[0]->fn_80402940(15, 0, 0.0f, -200.0f);
        for (int i = 0; i < 2; i++) {
            field_0x120[i]->fn_80402628(15, field_0x120[i]->field_0x28, 0);
        }
    }
    if (field_0x340 < 8) {
        field_0x10->search('Plvcu')->move(field_0x404 + field_0x10->search(lbl_8048C658[field_0x340 * 3])->getBounds()->i.x,
                                          field_0x408 + field_0x10->search(lbl_8048C658[field_0x340 * 3])->getBounds()->i.y);
        if (!isCursorState() || field_0x42e != 0) {
            field_0x42e++;
            if (field_0x42e >= 60) {
                field_0x42e = 0;
            }
        }
        f32 t;
        if (field_0x42e < 30) {
            t = (field_0x42e * field_0x42e) / 900.0f;
        } else {
            t = 1.0f - (field_0x42e - 30.0f) * (field_0x42e - 30.0f) / 900.0f;
        }
        field_0x10->search('Plvcu')->setAlpha(156.0f * t);
    } else {
        field_0x10->search('Plvcu')->setAlpha(0);
    }
    if (field_0x350 != field_0x354) {
        field_0x354 = field_0x350;
        switch (field_0x350) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 7:
        case 8:
        case 9:
        case 10:
        case 14:
        case 17:
        case 18:
        case 19:
        case 20:
        case 22:
            field_0x65f = 4;
            break;
        case 11:
        case 12:
        case 13:
        case 15:
        case 16:
            field_0x65f = 3;
            break;
        case 21:
            field_0x65f = 4;
            break;
        }
    }
    switch (field_0x65f) {
    case 1: {
        s16 frames = lbl_8050DB0C.field_0x4c[138];
        field_0x430++;
        if (field_0x430 < frames) {
            slidePane(field_0x430, frames);
        } else {
            field_0x430 = frames;
            field_0x2b4->moveFromCentre_80402E08(0.0f, 0.0f);
            field_0x2b4->mPane->setAlpha(field_0x2b4->field_0x28);
            field_0x65f = 0;
        }
        break;
    }
    case 2:
    case 3:
    case 4: {
        s16 frames = lbl_8050DB0C.field_0x4c[138];
        field_0x430--;
        if (field_0x430 > 0) {
            slidePane(field_0x430, frames);
        } else {
            field_0x430 = 0;
            field_0x2b4->moveFromCentre_80402E08(0.0f, -200.0f);
            field_0x2b4->mPane->setAlpha(0);
            field_0x660 = field_0x65f;
            if (field_0x65f == 2) {
                lbl_8053AED8->fn_80440BF4(false);
            } else {
                lbl_8053AED8->fn_80440BF4(true);
            }
            if (field_0x65f != 4) {
                field_0x65f = 1;
            }
        }
        break;
    }
    }
    for (int i = 0; i < 4; i++) {
        field_0x2b8[i]->fn_80456154();
    }
}

// FIXME(shotgun): 93.56 %, missing/extra (field_0x348 reloads) + pool literals
void MapScreen_804B1288::fn_8040F480() {
    u8 mode = 0;
    field_0x668 = 6;
    lbl_8053AEC4->fn_8042EB24();
    fn_8037AA58();
    switch (lbl_80529DEC.field_0x237) {
    case 5:
    case 9:
        field_0x348 = lbl_80529DEC.field_0x11c;
        field_0x340 = lbl_80529DEC.field_0x118;
        if (!(lbl_8050EC80.getCurrentFile().field_0x65 & 2)) {
            mode = 2;
        } else {
            mode = 0;
        }
        break;
    case 6:
        mode = 2;
        field_0x348 = lbl_80529DEC.field_0x11c;
        field_0x340 = lbl_80529DEC.field_0x118;
        break;
    case 7:
        mode = 1;
        field_0x348 = lbl_80529DEC.field_0x11c;
        field_0x340 = lbl_80529DEC.field_0x118;
        break;
    }
    if (!lbl_80529DEC.field_0x23e[1]) {
        fn_80404D70();
        if (lbl_80529DEC.field_0x237 == 5 && !mode) {
            if (fn_803B8EBC(7)) {
                field_0x340 = 0;
            } else {
                field_0x340 = field_0x370;
                // As in the original: the test can never be true.
                if (field_0x340 < 0 && field_0x340 >= 8) {
                    field_0x340 = 0;
                }
            }
            field_0x348 = 0;
        }
    } else {
        field_0x370 = 12;
        for (int stage = 1; stage < 7; stage++) {
            hideStagePanes(stage);
        }
        field_0xcc->mPane->show();
        if (mode == 2) {
            mode = 1;
        }
    }
    setStageName();
    strcpy(((J2DTextBox*)field_0x10->search('Ttlv11'))->getStringPtr(), field_0x438);
    strcpy(((J2DTextBox*)field_0x10->search('Ttlv12'))->getStringPtr(), field_0x438);
    setStageName();
    strcpy(((J2DTextBox*)field_0x10->search('Ttlv13'))->getStringPtr(), field_0x438);
    strcpy(((J2DTextBox*)field_0x10->search('Ttlv14'))->getStringPtr(), field_0x438);
    if (!fn_804045F8(field_0x340)) {
        field_0x34c = field_0x348;
        if (field_0x348 == 3) {
            field_0x34c = 2;
        }
        if (field_0x340 == 7 && (field_0x348 == 2 || field_0x348 == 3)) {
            field_0x34c = field_0x348 - 1;
        }
    } else {
        field_0x34c = field_0x348;
    }
    if (field_0x340 != 8 && field_0x340 != 9 && field_0x340 != 10 && field_0x340 != 11 && field_0x340 != 12) {
        getMessageText_803832BC(field_0x564[0], lbl_8048C238[field_0x340], 0, NULL, NULL);
        getMessageText_803832BC(field_0x564[1], lbl_8048C258[field_0x340], 0, NULL, NULL);
        getMessageText_803832BC(field_0x564[2], lbl_8048C278[field_0x340], 0, NULL, NULL);
        getMessageText_803832BC(field_0x564[3], lbl_8048C298[field_0x340], 0, NULL, NULL);
    }
    field_0x344 = field_0x340;
    fn_804083D8();
    for (int i = 0; i < 8; i++) {
        if (i > field_0x370) {
            field_0x28[i][0]->mPane->hide();
            field_0x208[i]->mPane->hide();
        } else if (!fn_804045F8(i)) {
            field_0x208[i]->mPane->hide();
        }
    }
    if (field_0x340 < 8) {
        field_0x394 = lbl_8048C95C[field_0x340 * 4 + field_0x348][0];
        field_0x398 = lbl_8048C95C[field_0x340 * 4 + field_0x348][1];
    }
    field_0x1c->setFrame(field_0x378);
    field_0xc->search('Ndata08')->animationTransform();
    field_0x20->setFrame(field_0x37c);
    field_0xc->animation();
    if (mode != 2 || field_0x364 == 0) {
        for (int i = 0; i < 18; i++) {
            field_0xd4[i]->fn_80402E70();
        }
        field_0xc->search('Ndata08')->setAnimation((J2DAnmTransform*)NULL);
    }
    switch (mode) {
    case 0:
        if (field_0x38c & 0x10) {
            if (field_0x65d == 0) {
                for (int i = 0; i < 15; i++) {
                    field_0x278[i]->field_0x26 = 0;
                    field_0x65d = 1;
                }
            }
            field_0x350 = 0;
            break;
        }
        fn_8040873C();
        if (fn_80404270(field_0x340)) {
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
            if (field_0x65d == 0) {
                for (int i = 0; i < 15; i++) {
                    field_0x278[i]->field_0x26 = 0;
                    field_0x65d = 1;
                }
            }
            field_0x350 = 0xF;
        }
        break;
    case 1:
        fn_8040873C();
        if (fn_80404270(field_0x340)) {
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
            if (field_0x65d == 0) {
                for (int i = 0; i < 15; i++) {
                    field_0x278[i]->field_0x26 = 0;
                    field_0x65d = 1;
                }
            }
            field_0x350 = 0xF;
        }
        break;
    case 2:
        if (field_0x364 != 0 && field_0x364 < 7) {
            field_0x340 = field_0x364 - 1;
            field_0x348 = 3;
            setStageName();
            strcpy(((J2DTextBox*)field_0x10->search('Ttlv11'))->getStringPtr(), field_0x438);
            strcpy(((J2DTextBox*)field_0x10->search('Ttlv12'))->getStringPtr(), field_0x438);
            setStageName();
            strcpy(((J2DTextBox*)field_0x10->search('Ttlv13'))->getStringPtr(), field_0x438);
            strcpy(((J2DTextBox*)field_0x10->search('Ttlv14'))->getStringPtr(), field_0x438);
            fn_8040873C();
            field_0x39c = 0.0f;
            field_0x3a0 = 0.0f;
            field_0x3a4 = 0.0f;
            field_0x358 = 0;
            field_0x350 = 4;
        } else if (!(lbl_8050EC80.getCurrentFile().field_0x65 & 2) && !(lbl_8050EC80.getCurrentFile().mFlags & 1)) {
            field_0x39c = -250.0f;
            lbl_8050EC80.getCurrentFile().field_0x65 |= 2;
            fn_8040873C();
            field_0x350 = 2;
        } else if (field_0x348 == 0) {
            if (field_0x65d == 0) {
                for (int i = 0; i < 15; i++) {
                    field_0x278[i]->field_0x26 = 0;
                    field_0x65d = 1;
                }
            }
            field_0x657 = 1;
            field_0x350 = 1;
        } else {
            fn_8040873C();
            if (field_0x340 == 7) {
                if (field_0x348 == 2) {
                    field_0x394 = lbl_8048C95C[field_0x340 * 4][0];
                    field_0x398 = lbl_8048C95C[field_0x340 * 4][1];
                    if (!isStageFlagSet(field_0x340, field_0x348) && !lbl_80529DEC.field_0x254[0]) {
                        field_0x388 = 0;
                    }
                } else {
                    field_0x394 = lbl_8048C95C[field_0x340 * 4 + 2][0];
                    field_0x398 = lbl_8048C95C[field_0x340 * 4 + 2][1];
                    if (!isStageFlagSet(field_0x340, field_0x348) && !lbl_80529DEC.field_0x254[0]) {
                        field_0x388 = 1;
                    }
                }
            } else if (field_0x348 == 1) {
                field_0x394 = lbl_8048C95C[field_0x340 * 4][0];
                field_0x398 = lbl_8048C95C[field_0x340 * 4][1];
                if (!isStageFlagSet(field_0x340, field_0x348) && !lbl_80529DEC.field_0x254[0]) {
                    field_0x388 = 0;
                }
            } else {
                field_0x394 = lbl_8048C95C[field_0x340 * 4 + 1][0];
                field_0x398 = lbl_8048C95C[field_0x340 * 4 + 1][1];
                if (!isStageFlagSet(field_0x340, field_0x348) && !lbl_80529DEC.field_0x254[0]) {
                    field_0x388 = 1;
                }
            }
            field_0x350 = 8;
            if (field_0x388 != -1) {
                field_0x664 = 1;
            }
        }
        break;
    }
    lbl_80529DEC.field_0x237 = 0;
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
    // Kept as its own local to match: the original leaves the product unfused (fmuls, fmuls, fadds).
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
        // Kept as its own local to match: the original leaves the product unfused.
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
        // Kept as its own local to match: the original leaves the product unfused.
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
            // Kept as its own local to match: the original leaves the product unfused.
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

// FIXME: 96.8. Missing/extra: ours extends the u16 angle again (clrlwi r0, r3, 16) before
// the third range test; the original keeps the one clrlwi'd copy in r0 (a u32 local
// holding the (u16) value gives scheduling+regalloc, 97.3 %, but no precedent proves it). Pool literals
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

// FIXME(shotgun): 98.96 %, regalloc
void MapScreen_804B1288::fn_8040D3D0() {
    if (lbl_8053AD30->fn_80309C4C(0x100, -1) || field_0x657) {
        field_0x348 = 0;
        field_0x34c = 0;
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
        loadStageInfo();
        if (!field_0x657) {
            gAudioMgr_8053AF80->fn_80458880(0xb, -1);
        } else {
            fn_804083D8();
            setStageName();
            strcpy(((J2DTextBox*)field_0x10->search('Ttlv13'))->getStringPtr(), field_0x438);
            strcpy(((J2DTextBox*)field_0x10->search('Ttlv14'))->getStringPtr(), field_0x438);
            fn_80411A94(1.0f);
        }
        if (field_0x65d == 0 && field_0x657 == 0) {
            for (int i = 0; i < 15; i++) {
                field_0x278[i]->field_0x26 = 0;
                field_0x65d = 1;
            }
        }
        for (int i = 0; i < 8; i++) {
            field_0x28[i][0]->moveFromCentre_80402E08(0.0f, 0.0f);
            field_0x28[i][0]->field_0x20 = 0;
            for (int j = 0; j < 3; j++) {
                field_0x28[i][j]->mPane->setAlpha(field_0x28[i][j]->field_0x28);
                field_0x28[i][j]->field_0x26 = 0;
            }
        }
        field_0x350 = 0xE;
    } else if (lbl_8053AD30->fn_80309C4C(0x1000, -1)) {
        if (gGameRoot_8053AAF8->field_0x7e == 1) {
            lbl_80529DEC.field_0x237 = 3;
            field_0x668 = 4;
            lbl_80529DEC.field_0x118 = field_0x340;
            lbl_80529DEC.field_0x11c = field_0x348;
            gAudioMgr_8053AF80->fn_80458880(0xb, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(0x200, -1)) {
        field_0x668 = 4;
        gAudioMgr_8053AF80->fn_80458880(0xb, -1);
    } else if (lbl_8053AD30->fn_80309B74(4, -1) || lbl_8053AD30->fn_80309B74(1, -1)) {
        field_0x344 = field_0x340;
        field_0x340--;
        if (field_0x340 < 0) {
            field_0x340 = field_0x370;
        }
        fn_804083D8();
        if (field_0x370 > 0) {
            setStageName();
            strcpy(((J2DTextBox*)field_0x10->search('Ttlv13'))->getStringPtr(), field_0x438);
            strcpy(((J2DTextBox*)field_0x10->search('Ttlv14'))->getStringPtr(), field_0x438);
            fn_80411A94(1.0f);
            field_0x426 = 0;
            field_0x350 = 0xC;
            gAudioMgr_8053AF80->fn_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309B74(8, -1) || lbl_8053AD30->fn_80309B74(2, -1)) {
        field_0x344 = field_0x340;
        field_0x340++;
        if (field_0x340 >= field_0x370 + 1) {
            field_0x340 = 0;
        }
        fn_804083D8();
        if (field_0x370 > 0) {
            setStageName();
            strcpy(((J2DTextBox*)field_0x10->search('Ttlv13'))->getStringPtr(), field_0x438);
            strcpy(((J2DTextBox*)field_0x10->search('Ttlv14'))->getStringPtr(), field_0x438);
            fn_80411A94(1.0f);
            field_0x426 = 0;
            field_0x350 = 0xD;
            gAudioMgr_8053AF80->fn_80458880(0x26, -1);
        }
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

// FIXME: 97.68. scheduling+regalloc (+ pool literals of unwritten functions); the
// "cleared" test after the stage branch is in both branches, as in fn_804096E8.
void MapScreen_804B1288::fn_8040BC1C() {
    u8 faded = false;
    u8 moved = false;
    u8 cleared = false;
    u8 listed = false;
    field_0x41e++;
    if (field_0x340 == 7) {
        if (field_0x41e >= 15) {
            field_0x640 = 1;
            f32 t = (field_0x41e - 15) / 30.0f;
            if (t > 1.0f) {
                t = 1.0f;
            }
            fn_80408F38(t);
            if (field_0x41e >= 45) {
                faded = true;
            }
        }
        if (field_0x41e == 21) {
            if (field_0x65e == 0) {
                field_0x120[0]->field_0x22 = 0;
                field_0x120[0]->field_0x26 = 0;
                field_0x120[1]->field_0x26 = 0;
                loadStageInfo();
                field_0x65e = 1;
            }
        } else if (field_0x41e > 40) {
            cleared = true;
        }
    } else {
        if (field_0x340 < 7) {
            if (field_0x41e >= 15) {
                field_0xc8->fn_80402B10(30, 0, 0.0f, lbl_8048C86C[field_0x340]);
                field_0xc8->fn_80402940(30, 0, 0.0f, lbl_8048C87C[field_0x340]);
                faded = field_0xc8->fn_8040271C(30, 1.0f, lbl_8048C850[field_0x340] / field_0xc8->field_0x18);
                f32 scale = lbl_8048C850[field_0x340] / field_0xc8->field_0x18 - 1.0f;
                f32 dx = scale * (field_0xcc->field_0x10 - field_0xc8->field_0x10);
                f32 dy = scale * (field_0xcc->field_0x14 - field_0xc8->field_0x14);
                field_0xcc->fn_80402B10(30, 0, 0.0f, dx + lbl_8048C86C[field_0x340]);
                field_0xcc->fn_80402940(30, 0, 0.0f, dy + lbl_8048C87C[field_0x340]);
                field_0xcc->fn_8040271C(30, 1.0f, lbl_8048C850[field_0x340] / field_0xc8->field_0x18);
                dx = scale * (field_0xd0->field_0x10 - field_0xc8->field_0x10);
                dy = scale * (field_0xd0->field_0x14 - field_0xc8->field_0x14);
                field_0xd0->fn_80402B10(30, 0, 0.0f, dx + lbl_8048C86C[field_0x340]);
                field_0xd0->fn_80402940(30, 0, 0.0f, dy + lbl_8048C87C[field_0x340]);
                field_0xd0->fn_8040271C(30, 1.0f, lbl_8048C850[field_0x340] / field_0xc8->field_0x18);
                for (int i = 0; i < 18; i++) {
                    dx = scale * field_0xd4[i]->field_0x10;
                    dy = scale * field_0xd4[i]->field_0x14;
                    field_0xd4[i]->fn_80402B10(30, 0, 0.0f, dx);
                    field_0xd4[i]->fn_80402940(30, 0, 0.0f, dy);
                    field_0xd4[i]->fn_8040271C(30, 1.0f, lbl_8048C850[field_0x340] / field_0xc8->field_0x18);
                }
            }
        } else {
            faded = true;
        }
        if (field_0x41e == 21) {
            if (field_0x65e == 0) {
                field_0x120[0]->field_0x22 = 0;
                field_0x120[0]->field_0x26 = 0;
                field_0x120[1]->field_0x26 = 0;
                loadStageInfo();
                field_0x65e = 1;
            }
        } else if (field_0x41e > 40) {
            cleared = true;
        }
    }
    if (field_0x657 == 0) {
        if (field_0x41e >= 0 && field_0x41e <= 15) {
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 2; j++) {
                    moved = field_0x228[i][j]->fn_80402940(15, 0, 0.0f, -150.0f);
                }
            }
            field_0x238->fn_80402940(15, 0, 0.0f, -150.0f);
            field_0x23c->fn_80402940(15, 0, 0.0f, -150.0f);
        } else if (field_0x41e > 15) {
            moved = true;
        }
    } else {
        moved = true;
    }
    s32 rows = field_0x370;
    if (rows >= 8) {
        rows = 7;
    }
    if (field_0x41e >= 0 && field_0x41e <= rows * 5 + 20) {
        for (int i = 0; i < rows + 1; i++) {
            if (field_0x41e >= i * 5) {
                field_0x28[rows - i][0]->fn_80402B10(20, 0, 0.0f, -150.0f);
                for (int j = 0; j < 3; j++) {
                    field_0x28[rows - i][j]->fn_80402628(20, field_0x28[rows - i][j]->field_0x28, 0);
                }
            }
        }
    } else if (field_0x41e > rows * 5 + 20) {
        listed = true;
    }
    if (faded && moved && cleared && listed) {
        if (field_0x340 <= 7) {
            f32 x = lbl_8048C95C[field_0x340 * 4 + field_0x348][0];
            f32 y = lbl_8048C95C[field_0x340 * 4 + field_0x348][1];
            field_0x394 = x;
            field_0x398 = y;
        } else {
            field_0x394 = field_0x398 = 0.0f;
        }
        if (field_0x663) {
            field_0x2c8->startSound(JAISoundID(0x400E4));
            field_0x350 = 3;
            field_0x358 = 0;
        } else if (fn_80404270(field_0x340)) {
            if (field_0x657) {
                field_0x658 = 1;
            }
            field_0x350 = 0x10;
            if (field_0x65d == 1) {
                for (int i = 0; i < 15; i++) {
                    field_0x278[i]->field_0x26 = 0;
                    field_0x65d = 0;
                }
            }
            field_0x4->fn_804446F0(0);
            gAudioMgr_8053AF80->fn_80458880(0x56, -1);
        } else if (field_0x657) {
            field_0x350 = 0x15;
        } else {
            field_0x350 = 0xF;
        }
        field_0x657 = 0;
    }
    if (field_0x41e >= 15) {
        gAudioMgr_8053AF80->fn_80458880(0x55, -1);
    }
}

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


// FIXME(shotgun): 99.65 %, pool literals + regalloc
void MapScreen_804B1288::fn_80409FDC() {
    f32 x = 0.0f;
    f32 y = 0.0f;
    if (field_0x340 <= 7) {
        f32 speed = 3.4f + lbl_8050DB0C.field_0x4c[311] / 100.0f;
        x = lbl_8048C95C[field_0x340 * 4 + field_0x348][0];
        y = lbl_8048C95C[field_0x340 * 4 + field_0x348][1];
        f32 dx = x - field_0x394;
        f32 dy = y - field_0x398;
        f32 dist = std::sqrtf(dx * dx + dy * dy);
        if (dist < speed) {
            field_0x394 = x;
            field_0x398 = y;
            if (!field_0x65e) {
                field_0x120[0]->field_0x22 = 0;
                field_0x120[0]->field_0x26 = 0;
                field_0x120[1]->field_0x26 = 0;
                loadStageInfo();
                field_0x65e = 1;
            }
        } else {
            field_0x394 += speed * dx / dist;
            field_0x398 += speed * dy / dist;
            field_0x2b8[0]->startSound(JAISoundID(0x20064));
        }
        field_0x10->search('Ndata10')->move(field_0x394, field_0x398);
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
    if (field_0x340 <= 7 && field_0x394 == x) {
        fn_804054B8();
    } else if (field_0x10->search('Ndata10')->mIsVisible) {
        field_0x10->search('Ndata10')->hide();
    }
    if (field_0x40c) {
        if (lbl_8053AD30->fn_80309A9C(8, -1)) {
            field_0x348--;
            if (field_0x348 < 0) {
                field_0x348 = 3;
            }
            field_0x34c = field_0x348;
            hideSubStagePanes();
            gAudioMgr_8053AF80->fn_80458880(0x26, -1);
        } else if (lbl_8053AD30->fn_80309A9C(4, -1)) {
            field_0x348++;
            if (field_0x348 >= 4) {
                field_0x348 = 0;
            }
            field_0x34c = field_0x348;
            hideSubStagePanes();
            gAudioMgr_8053AF80->fn_80458880(0x26, -1);
        } else if (lbl_8053AD30->fn_80309C4C(0x1000, -1)) {
            if (gGameRoot_8053AAF8->field_0x7e == 1) {
                lbl_80529DEC.field_0x237 = 4;
                field_0x668 = 4;
                lbl_80529DEC.field_0x118 = field_0x340;
                lbl_80529DEC.field_0x11c = field_0x348;
                gAudioMgr_8053AF80->fn_80458880(0xb, -1);
            }
        } else if (lbl_8053AD30->fn_80309C4C(0x100, -1)) {
            field_0x394 = x;
            field_0x398 = y;
            field_0x120[0]->moveFromCentre_80402E08(0.0f, 0.0f);
            for (int i = 0; i < 2; i++) {
                field_0x120[i]->mPane->setAlpha(field_0x120[i]->field_0x28);
            }
            loadStageInfo();
            field_0x65e = 1;
            gAudioMgr_8053AF80->fn_80458F9C(10);
            field_0x41e = 0;
            field_0x10->search('Ndata10')->hide();
            field_0x350 = 0x14;
            gAudioMgr_8053AF80->fn_80458880(0xb, -1);
        } else if (lbl_8053AD30->fn_80309C4C(0x200, -1)) {
            if (field_0x663) {
                for (int i = 0; i < 2; i++) {
                    if (field_0x308[i] != NULL) {
                        field_0x308[i]->becomeInvalidEmitter();
                        field_0x308[i] = NULL;
                    }
                }
                field_0x663 = 0;
                field_0x39c = 0.0f;
            }
            fn_804049DC();
        }
    } else if (lbl_8053AD30->fn_80309A9C(8, -1)) {
        if (field_0x340 <= 7) {
            if (fn_804045F8(field_0x340)) {
                fn_80405D00(0);
            } else {
                fn_80405B10(0);
            }
        } else {
            field_0x348--;
            field_0x34c = field_0x348;
            if (field_0x348 < 0) {
                field_0x348 = 0;
                field_0x34c = field_0x348;
            } else {
                hideSubStagePanes();
                gAudioMgr_8053AF80->fn_80458880(0x26, -1);
            }
        }
    } else if (lbl_8053AD30->fn_80309A9C(4, -1)) {
        if (field_0x340 <= 7) {
            if (fn_804045F8(field_0x340)) {
                fn_80405D00(2);
            } else {
                fn_80405B10(2);
            }
        } else {
            field_0x348++;
            field_0x34c = field_0x348;
            if (field_0x348 >= 4) {
                field_0x348 = 3;
                field_0x34c = field_0x348;
            } else {
                hideSubStagePanes();
                gAudioMgr_8053AF80->fn_80458880(0x26, -1);
            }
        }
    } else if (lbl_8053AD30->fn_80309A9C(1, -1)) {
        if (field_0x340 <= 7) {
            if (fn_804045F8(field_0x340)) {
                fn_80405D00(3);
            } else {
                fn_80405B10(3);
            }
        }
    } else if (lbl_8053AD30->fn_80309A9C(2, -1)) {
        if (field_0x340 <= 7) {
            if (fn_804045F8(field_0x340)) {
                fn_80405D00(1);
            } else {
                fn_80405B10(1);
            }
        }
    } else if (lbl_8053AD30->fn_80309C4C(0x1000, -1)) {
        if (gGameRoot_8053AAF8->field_0x7e == 1) {
            lbl_80529DEC.field_0x237 = 4;
            field_0x668 = 4;
            lbl_80529DEC.field_0x118 = field_0x340;
            lbl_80529DEC.field_0x11c = field_0x348;
            gAudioMgr_8053AF80->fn_80458880(0xb, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(0x100, -1)) {
        field_0x394 = x;
        field_0x398 = y;
        field_0x120[0]->moveFromCentre_80402E08(0.0f, 0.0f);
        for (int i = 0; i < 2; i++) {
            field_0x120[i]->mPane->setAlpha(field_0x120[i]->field_0x28);
        }
        loadStageInfo();
        field_0x65e = 1;
        gAudioMgr_8053AF80->fn_80458F9C(10);
        field_0x41e = 0;
        field_0x10->search('Ndata10')->hide();
        field_0x350 = 0x14;
        gAudioMgr_8053AF80->fn_80458880(0xb, -1);
    } else if (lbl_8053AD30->fn_80309C4C(0x200, -1)) {
        if (field_0x663) {
            for (int i = 0; i < 2; i++) {
                if (field_0x308[i] != NULL) {
                    field_0x308[i]->becomeInvalidEmitter();
                    field_0x308[i] = NULL;
                }
            }
            field_0x663 = 0;
            field_0x39c = 0.0f;
        }
        fn_80404648();
    }
    if (field_0x663 != 0) {
        field_0x434 += 0x200;
        field_0x436 += 0x400;
        field_0x3a0 = 15.0f * JMath::sincosTable_.sinShort(field_0x434);
        field_0x3a4 = 5.0f * JMath::sincosTable_.sinShort(field_0x436);
        field_0x2c8->startSound(JAISoundID(0x400E3));
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

// FIXME: 96.04. scheduling+regalloc: the zoom divisions load field_0x18 before the
// lbl_8048C850 entry in the original, and registers differ around fn_80403184. The tail after the
// stage branch is in both branches: the original has its own copy in the stage 7 branch.
void MapScreen_804B1288::fn_8040873C() {
    if (field_0x340 == 7) {
        fn_80408F38(1.0f);
        field_0x120[0]->moveFromCentre_80402E08(0.0f, 0.0f);
        for (int i = 0; i < 2; i++) {
            field_0x120[i]->mPane->setAlpha(field_0x120[i]->field_0x28);
        }
        field_0x65e = 1;
    } else {
        if (field_0x340 < 7) {
            field_0xc8->moveFromCentre_80402E08(lbl_8048C86C[field_0x340], lbl_8048C87C[field_0x340]);
            f32 zoom = lbl_8048C850[field_0x340] / field_0xc8->field_0x18;
            field_0xc8->fn_80402CE0(zoom, zoom);
            f32 scale = lbl_8048C850[field_0x340] / field_0xc8->field_0x18 - 1.0f;
            f32 dx = scale * (field_0xcc->field_0x10 - field_0xc8->field_0x10);
            f32 dy = scale * (field_0xcc->field_0x14 - field_0xc8->field_0x14);
            field_0xcc->moveFromCentre_80402E08(dx + lbl_8048C86C[field_0x340], dy + lbl_8048C87C[field_0x340]);
            zoom = lbl_8048C850[field_0x340] / field_0xc8->field_0x18;
            field_0xcc->fn_80402CE0(zoom, zoom);
            dx = scale * (field_0xd0->field_0x10 - field_0xc8->field_0x10);
            dy = scale * (field_0xd0->field_0x14 - field_0xc8->field_0x14);
            field_0xd0->moveFromCentre_80402E08(dx + lbl_8048C86C[field_0x340], dy + lbl_8048C87C[field_0x340]);
            zoom = lbl_8048C850[field_0x340] / field_0xc8->field_0x18;
            field_0xd0->fn_80402CE0(zoom, zoom);
            for (int i = 0; i < 18; i++) {
                field_0xd4[i]->moveFromCentre_80402E08(scale * field_0xd4[i]->field_0x10,
                                                       scale * field_0xd4[i]->field_0x14);
                zoom = lbl_8048C850[field_0x340] / field_0xc8->field_0x18;
                field_0xd4[i]->fn_80402CE0(zoom, zoom);
            }
        }
        field_0x120[0]->moveFromCentre_80402E08(0.0f, 0.0f);
        for (int i = 0; i < 2; i++) {
            field_0x120[i]->mPane->setAlpha(field_0x120[i]->field_0x28);
        }
        field_0x65e = 1;
    }
    for (int i = 0; i < 8; i++) {
        field_0x28[i][0]->moveFromCentre_80402E08(-150.0f, 0.0f);
        field_0x28[i][0]->field_0x20 = 0;
        for (int j = 0; j < 3; j++) {
            field_0x28[i][j]->mPane->setAlpha(0);
        }
    }
    field_0x11c->moveFromCentre_80402E08(0.0f, 0.0f);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            field_0x228[i][j]->moveFromCentre_80402E08(0.0f, -150.0f);
        }
    }
    field_0x238->moveFromCentre_80402E08(0.0f, -150.0f);
    field_0x23c->moveFromCentre_80402E08(0.0f, -150.0f);
    if (field_0x340 <= 7) {
        if (field_0x350 != 6) {
            f32 x = lbl_8048C95C[field_0x340 * 4 + field_0x348][0];
            f32 y = lbl_8048C95C[field_0x340 * 4 + field_0x348][1];
            field_0x394 = x;
            field_0x398 = y;
        }
    } else {
        field_0x394 = field_0x398 = 0.0f;
    }
    if (fn_804045F8(field_0x340)) {
        field_0x208[field_0x340]->mPane->setAlpha(field_0x208[field_0x340]->field_0x28);
    }
    field_0x400 = 1.0f;
    loadStageInfo();
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

// FIXME(shotgun): 99.35 %, regalloc
void MapScreen_804B1288::fn_8040732C() {
    s32 picked = -1;
    if (field_0x340 > 7) {
        return;
    }
    if (!gGameRoot_8053AAF8->field_0x7e) {
        for (int i = field_0x340 * 4; i < field_0x340 * 4 + 4; i++) {
            if (field_0x38c & (1 << i)) {
                if (fn_804045F8(field_0x340)) {
                    field_0x128[i]->fn_8040245C(100, 0, field_0x128[i]->field_0x28, 0);
                    field_0x128[i]->setAlpha_804023D0(field_0x10->search(lbl_8048C2F8[i][1]), field_0x641);
                } else if (i != lbl_8048C8EC[field_0x340] && !fn_80404074(i / 4, i % 4, &picked)) {
                    field_0x128[i]->fn_8040245C(100, 0, field_0x128[i]->field_0x28, 0);
                    field_0x128[i]->setAlpha_804023D0(field_0x10->search(lbl_8048C2F8[i][1]), field_0x641);
                }
            }
        }
        return;
    }
    for (int i = field_0x340 * 3; i < field_0x340 * 3 + 3; i++) {
        s32 stage = i / 3;
        u8 rank;
        if (stage < 0 || stage >= 8) {
            rank = 0xFF;
        } else {
            s32 sub = i % 3;
            if (sub < 0 || sub >= 3) {
                rank = 0xFF;
            } else {
                rank = lbl_80529DEC.field_0x1fc[stage][sub];
            }
        }
        s32 sub = i % 3;
        if (stage == 7) {
            if (sub > 0) {
                sub++;
            }
        } else if (sub > 1) {
            sub++;
        }
        s32 point = stage * 4 + sub;
        switch (rank) {
        case 0:
            ((J2DPicture*)field_0x1a8[i]->mPane)->setBlackWhite(JUtility::TColor(0, 50, 0, 0), JUtility::TColor(255, 255, 255, 255));
            ((J2DPicture*)field_0x1a8[i]->mPane)->setCornerColor(JUtility::TColor(127, 248, 37, 255));
            ((J2DPicture*)field_0x10->search(lbl_8048C2F8[point][0]))
                ->setBlackWhite(JUtility::TColor(100, 255, 50, 0), JUtility::TColor(100, 255, 50, 255));
            ((J2DPicture*)field_0x10->search(lbl_8048C2F8[point][1]))
                ->setBlackWhite(JUtility::TColor(100, 255, 50, 0), JUtility::TColor(100, 255, 50, 255));
            break;
        case 1:
            ((J2DPicture*)field_0x1a8[i]->mPane)->setBlackWhite(JUtility::TColor(50, 0, 0, 0), JUtility::TColor(255, 255, 255, 255));
            ((J2DPicture*)field_0x1a8[i]->mPane)->setCornerColor(JUtility::TColor(255, 150, 120, 255));
            ((J2DPicture*)field_0x10->search(lbl_8048C2F8[point][0]))
                ->setBlackWhite(JUtility::TColor(255, 100, 50, 0), JUtility::TColor(255, 100, 50, 255));
            ((J2DPicture*)field_0x10->search(lbl_8048C2F8[point][1]))
                ->setBlackWhite(JUtility::TColor(255, 100, 50, 0), JUtility::TColor(255, 100, 50, 255));
            break;
        case 2:
            ((J2DPicture*)field_0x1a8[i]->mPane)->setBlackWhite(JUtility::TColor(0, 10, 40, 0), JUtility::TColor(255, 255, 255, 255));
            ((J2DPicture*)field_0x1a8[i]->mPane)->setCornerColor(JUtility::TColor(120, 240, 255, 255));
            ((J2DPicture*)field_0x10->search(lbl_8048C2F8[point][0]))
                ->setBlackWhite(JUtility::TColor(50, 220, 240, 0), JUtility::TColor(50, 220, 240, 255));
            ((J2DPicture*)field_0x10->search(lbl_8048C2F8[point][1]))
                ->setBlackWhite(JUtility::TColor(50, 220, 240, 0), JUtility::TColor(50, 220, 240, 255));
            break;
        case 3:
            ((J2DPicture*)field_0x1a8[i]->mPane)->setBlackWhite(JUtility::TColor(20, 0, 20, 0), JUtility::TColor(255, 255, 255, 255));
            ((J2DPicture*)field_0x1a8[i]->mPane)->setCornerColor(JUtility::TColor(220, 150, 255, 255));
            ((J2DPicture*)field_0x10->search(lbl_8048C2F8[point][0]))
                ->setBlackWhite(JUtility::TColor(200, 100, 240, 0), JUtility::TColor(200, 100, 240, 255));
            ((J2DPicture*)field_0x10->search(lbl_8048C2F8[point][1]))
                ->setBlackWhite(JUtility::TColor(200, 100, 240, 0), JUtility::TColor(200, 100, 240, 255));
            break;
        default:
            ((J2DPicture*)field_0x10->search(lbl_8048C2F8[point][0]))
                ->setBlackWhite(JUtility::TColor(255, 255, 120, 0), JUtility::TColor(255, 255, 120, 255));
            ((J2DPicture*)field_0x10->search(lbl_8048C2F8[point][1]))
                ->setBlackWhite(JUtility::TColor(255, 255, 120, 0), JUtility::TColor(255, 255, 120, 255));
            break;
        }
        if (rank < 4) {
            // The original passes field_0x128[i]'s byte 0x28 (same index as field_0x1a8).
            field_0x1a8[i]->fn_80402628(100, 0, field_0x128[i]->field_0x28);
        }
    }
    for (int i = field_0x340 * 4; i < field_0x340 * 4 + 4; i++) {
        if (field_0x38c & (1 << i)) {
            if (fn_804045F8(field_0x340)) {
                field_0x128[i]->fn_8040245C(100, 0, field_0x128[i]->field_0x28, 0);
                field_0x128[i]->setAlpha_804023D0(field_0x10->search(lbl_8048C2F8[i][1]), field_0x641);
            } else if (i != lbl_8048C8EC[field_0x340] && !fn_80404074(i / 4, i % 4, &picked)) {
                field_0x128[i]->fn_8040245C(100, 0, field_0x128[i]->field_0x28, 0);
                field_0x128[i]->setAlpha_804023D0(field_0x10->search(lbl_8048C2F8[i][1]), field_0x641);
            }
        }
    }
    PaneCtrl_804AF86C* ctrl = field_0x208[field_0x340];
    if (ctrl->mPane->getAlpha() != ctrl->field_0x28) {
        ctrl->fn_80402628(50, 0, ctrl->field_0x28);
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

// FIXME: 96.30, different (NONMATCHING): setRotScaleMtx's six sin / cos results land in
// other saved FPRs (target sx f28, sy f27, sz f26, cx f31, cy f30, scale f29) and its
// products are scheduled differently (unproven inline, see game/RotMatrix.h); and the
// else branch of the stage choice reloads field_0x340 where the original copies the
// compared register (mr r30, r3).
#ifdef NONMATCHING
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
#endif

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

// Not auto-inlined: fn_80409FDC calls fn_80405B10 out of line in the original
// (docs/INLINING.md, pragma rule 1).
#pragma auto_inline off
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
#pragma auto_inline reset

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
