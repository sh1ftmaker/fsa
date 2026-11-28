#include "game/GameAudio.h"
#include "game/EmitterDelete.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/act/act_other_characters_ending_80362634.h"
#include "game/Effect.h"
#include "game/SpriteParams.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "JSystem/JMath/JMath.h"
#include <float.h>
#include <math.h>

static const s32 lbl_80474320[76] = {
    940, 944, 936, 932, 924, 928, 920, 916, 960, 954, 948, 966, 1078, 1080, 1076, 1074,
    1156, 1158, 1154, 1152, 1168, 1170, 1166, 1164, 1180, 1182, 1178, 1176, 1194, 1196, 1192, 1190,
    1212, 1214, 1210, 1208, 1226, 1228, 1224, 1222, 1240, 1242, 1238, 1236, 1254, 1256, 1252, 1250,
    1268, 1270, 1266, 1264, 1282, 1284, 1280, 1278, 1294, 1296, 1292, 1290, 1306, 1308, 1304, 1302,
    1320, 1322, 1318, 1316, 1332, 1334, 1330, 1328, 898, 900, 896, 894,
};
static const s32 lbl_80474450[42] = {20, 2, 20, 2, 12, 2, 15, 0, 15, 0, 15, 0, 15, 0, 15, 0, 15, 0, 15, 0, 15, 0, 15, 0, 15, 0, 15, 0, 15, 0, 15, 0, 15, 0, 20, 0, 20, 0, 20, 0, 15, 2};
// The walk's leg lengths (fn_80364A88) and the frame-length / pattern byte lists (fn_80364F00).
static const f32 lbl_804744F8[5] = {80.0f, 80.0f, 224.0f, 80.0f, 80.0f};
static const f32 lbl_8047450C[5] = {112.0f, 80.0f, 160.0f, 80.0f, 112.0f};
static const f32 lbl_80474520[5] = {144.0f, 80.0f, 96.0f, 80.0f, 144.0f};
static const f32 lbl_80474534[5] = {176.0f, 80.0f, 32.0f, 80.0f, 176.0f};
// Read by fn_80364A88: the direction/animation words per heading (16-byte rows), then a row of
// field_0x26c values for the idle state (all inside the original's lbl_80474534 / lbl_80474A5C ranges).
static const s32 lbl_80474548[5][4] = {
    {851, 854, 848, 845},
    {904, 907, 901, 898},
    {865, 868, 862, 859},
    {879, 882, 876, 873},
    {843, 904, 857, 871},
};
static const s32 lbl_80474598[4] = {6, 9, 3, 0};
static const s32 lbl_804745A8[4] = {910, 913, 907, 904};
static const s32 lbl_804745B8[150] = {
    960, 954, 948, 966, 708, 710, 706, 704, 256, 258, 254, 252, 240, 242, 238, 236,
    248, 250, 246, 244, 211, 219, 203, 195, 902, 839, 974, 976, 890, 892, 895, 968,
    970, 972, 978, 980, 983, 986, 920, 1018, 842, 1017, 1019, 726, 728, 724, 722, 746,
    748, 744, 742, 135, 139, 131, 127, 119, 123, 115, 111, 83, 86, 80, 77, 1078,
    1080, 1076, 1074, 1144, 1148, 1156, 1158, 1154, 1152, 1168, 1170, 1166, 1164, 1180, 1182, 1178,
    1176, 1194, 1196, 1192, 1190, 1212, 1214, 1210, 1208, 1226, 1228, 1224, 1222, 1240, 1242, 1238,
    1236, 1254, 1256, 1252, 1250, 1268, 1270, 1266, 1264, 1282, 1284, 1280, 1278, 1294, 1296, 1292,
    1290, 1306, 1308, 1304, 1302, 1320, 1322, 1318, 1316, 1332, 1334, 1330, 1328, 288, 290, 293,
    292, 294, 296, 299, 298, 300, 306, 312, 314, 326, 328, 330, 332, 338, 340, 304,
    1141, 898, 900, 896, 894, 1335,
};
static const s8 lbl_80474810[12] = {8, 8, 8, 8, 8, 8, 8, 8, -1, 0, 0, 0};
static const s8 lbl_8047481C[24] = {5, 5, 5, 5, 5, 5, 5, 5, 10, 15, 15, 5, 5, 5, 5, 5, 5, 5, 5, 10, 15, 15, -1, 0};
static const s8 lbl_80474834[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 0, 0};
static const s8 lbl_80474840[24] = {0, 1, 0, 1, 0, 1, 0, 1, 0, 3, 2, 4, 5, 4, 5, 4, 5, 4, 5, 4, 3, 2, 0, 0};
static const s32 lbl_80474858[4] = {959, 962, 956, 953}; // sprite per heading (the original's table is 0x204 long)
// Read by init (0x80365B5C). Per field_0x238 of state 4 (125 entries) and of state 0x11 (25):
// the index into lbl_804ADED0 (field_0x270), into lbl_804ADF08 (field_0x274) and field_0x258.
static const s8 lbl_80474868[125] = {
    0, 0, 0, 0, 13, 13, 13, 13, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 1,
    3, 4, 4, 8, 5, 5, 8, 6, 6, 8, 7, 7, 9, 10, 10, 10, 10, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 11, 11, 11, 11, 11, 11, 11, 11, 12, 12, 12, 12, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};
static const s8 lbl_804748E8[25] = {8, 8, 10, 10, 8, 8, 10, 10, 8, 8, 8, 8, 8,
                                    8, 8, 8, 8, 8, 8, 0, 8, 8, 8, 8, 8};
extern const s8 lbl_80474904[14] = {3, 2, 8, 4, 2, 4, 2, 4, 2, 22, 1, 2, 4, 2};
static const s8 lbl_80474914[125] = {
    0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 1,
    3, 1, 1, 1, 3, 3, 1, 3, 3, 1, 3, 3, 4, 5, 5, 5, 5, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 6, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};
static const s8 lbl_80474994[25] = {1, 1, 5, 5, 1, 1, 5, 5, 1, 1, 1, 1, 1,
                                    1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1};
static const s8 lbl_804749B0[125] = {
    2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
static const s8 lbl_80474A30[25] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                    1, 1, 1, 1, 1, 1, 0, 2, 2, 2, 2, 0};
static const f32 lbl_80474A4C[4] = {0.9f, 1.0f, 0.8f, 0.7f};
static const s32 lbl_80474A5C[3] = {907, 904, 910};
static const s32 lbl_80474A68[4] = {0, 1, 2, 1};
static const s32 lbl_80474A78[4] = {9, 7, 9, 7};
static const s32 lbl_80474A88[4] = {10, 7, 10, 7};
static const s32 lbl_80474A98[16] = {4, 4, 6, 4, 4, 4, 6, 4, 4, 4, 6, 4, 4, 4, 6, 4};
static const s32 lbl_80474AD8[32] = {0, 1, 2, 3, 9, 6, 7, 8, 0, 1, 2, 3, 9, 6, 7, 8,
                                     5, 6, 7, 8, 4, 1, 2, 3, 5, 6, 7, 8, 4, 1, 2, 3};
static const s32 lbl_80474B58[4] = {937, 945, 929, 921};
static const s32 lbl_80474B68[8] = {3, 3, 4, 3, 3, 3, 4, 3};
static const s32 lbl_80474B88[6] = {7, 10, 10, 7, 10, 10};
static const s32 lbl_80474BA0[6] = {0, 1, 2, 0, 3, 4};
static const f32 lbl_80474BB8[3] = {6.0f, 8.0f, 6.0f};
static const s32 lbl_80474BC4[4] = {0, 1, 0, 2};


static const s8 lbl_80542A78[4] = {7, 5, 7, -1};
static const s8 lbl_80542A7C[4] = {0, 1, 2, 0};
static const s8 lbl_80542A80[4] = {12, 12, 12, -1};
static const s8 lbl_80542A84[4] = {20, 20, -1, 0};
static const s8 lbl_80542A88[8] = {15, 10, 15, 10, -1, 0, 0, 0};
static const s8 lbl_80542A90[4] = {8, 10, -1, 0};
static const s8 lbl_80542A94[8] = {7, 7, 12, 7, -1, 0, 0, 0};
static const s8 lbl_80542A9C[4] = {10, 8, -1, 0};
static const s8 lbl_80542AA0[8] = {20, 2, 20, 2, -1, 0, 0, 0};
static const s8 lbl_80542AA8[4] = {15, 15, -1, 0};
static const s8 lbl_80542AAC[4] = {10, -1, 0, 0};
static const s8 lbl_80542AB0[4] = {16, 16, -1, 0};
static const s8 lbl_80542AB4[8] = {8, 8, 8, 8, -1, 0, 0, 0};
static const s8 lbl_80542ABC[4] = {50, 8, -1, 0};
static const s8 lbl_80542AC0[4] = {0, 1, 2, 0};
static const s8 lbl_80542AC4[4] = {0, 1, 0, 0};
static const s8 lbl_80542AC8[4] = {0, 1, 2, 1};
static const s8 lbl_80542ACC[4] = {0, 0, 0, 0};
static const s8 lbl_80542AD0[4] = {0, 1, 0, 0};
static const s8 lbl_80542AD4[4] = {0, 1, 0, 2};
static const s8 lbl_80542AD8[8] = {10, 7, 10, 7, -1, 0, 0, 0};
// the number of entries of each lbl_804ADF08 list
static const s8 lbl_80542AE0[8] = {3, 2, 9, 4, 22, 1, 2, 4};
static const u16 lbl_80542AE8[4] = {0x1bd, 0x1be, 0x1bf, 0x1c0};

void (OtherCharactersEnding_804ADF28::*lbl_804ADC38[18])() = {
    &OtherCharactersEnding_804ADF28::fn_803653EC,
    &OtherCharactersEnding_804ADF28::fn_80365274,
    &OtherCharactersEnding_804ADF28::fn_80365220,
    &OtherCharactersEnding_804ADF28::fn_8036509C,
    &OtherCharactersEnding_804ADF28::fn_80364F60,
    &OtherCharactersEnding_804ADF28::fn_80364D0C,
    &OtherCharactersEnding_804ADF28::fn_80364A88,
    &OtherCharactersEnding_804ADF28::fn_80364900,
    &OtherCharactersEnding_804ADF28::fn_803645B4,
    &OtherCharactersEnding_804ADF28::fn_80364340,
    &OtherCharactersEnding_804ADF28::fn_80363FFC,
    &OtherCharactersEnding_804ADF28::fn_80363F7C,
    &OtherCharactersEnding_804ADF28::fn_80363880,
    &OtherCharactersEnding_804ADF28::fn_80363664,
    &OtherCharactersEnding_804ADF28::fn_80363360,
    &OtherCharactersEnding_804ADF28::fn_8036335C,
    &OtherCharactersEnding_804ADF28::fn_80362C44,
    &OtherCharactersEnding_804ADF28::fn_80364F00,
};

void (OtherCharactersEnding_804ADF28::*lbl_804ADDE8[18])() = {
    &OtherCharactersEnding_804ADF28::fn_80362B74,
    &OtherCharactersEnding_804ADF28::fn_80362680,
    &OtherCharactersEnding_804ADF28::fn_80362B14,
    &OtherCharactersEnding_804ADF28::fn_80362680,
    &OtherCharactersEnding_804ADF28::fn_80362680,
    &OtherCharactersEnding_804ADF28::fn_80362680,
    &OtherCharactersEnding_804ADF28::fn_80362680,
    &OtherCharactersEnding_804ADF28::fn_803628D4,
    &OtherCharactersEnding_804ADF28::fn_80362680,
    &OtherCharactersEnding_804ADF28::fn_80362680,
    &OtherCharactersEnding_804ADF28::fn_80362680,
    &OtherCharactersEnding_804ADF28::fn_80362680,
    &OtherCharactersEnding_804ADF28::fn_80362680,
    &OtherCharactersEnding_804ADF28::fn_80362680,
    &OtherCharactersEnding_804ADF28::fn_80362680,
    &OtherCharactersEnding_804ADF28::fn_80362680,
    &OtherCharactersEnding_804ADF28::fn_80362680,
    &OtherCharactersEnding_804ADF28::fn_80362680,
};

// .data 0x804ADEC0..0x804ADF28 (the tail of the original's lbl_804ADDE8).
static const f32* lbl_804ADEC0[4] = {lbl_804744F8, lbl_8047450C, lbl_80474520, lbl_80474534};
static const s8* lbl_804ADED0[14] = {
    lbl_80542A80, lbl_80542A84, lbl_80474810, lbl_80542A88, lbl_80542A90, lbl_80542A94, lbl_80542A9C,
    lbl_80542AA0, lbl_80542AA8, lbl_8047481C, lbl_80542AAC, lbl_80542AB0, lbl_80542AB4, lbl_80542ABC,
};
static const s8* lbl_804ADF08[8] = {
    lbl_80542AC0, lbl_80542AC4, lbl_80474834, lbl_80542AC8, lbl_80474840, lbl_80542ACC, lbl_80542AD0, lbl_80542AD4,
};

static JGeometry::TVec3f lbl_8050C028[4] = {
    JGeometry::TVec3f(0.0f, 1.3f, 0.0f),  JGeometry::TVec3f(0.0f, -1.3f, 0.0f),
    JGeometry::TVec3f(-1.3f, 0.0f, 0.0f), JGeometry::TVec3f(1.3f, 0.0f, 0.0f),
};
static JGeometry::TVec3f lbl_8050C058[4] = {
    JGeometry::TVec3f(0.0f, 0.3f, 0.0f),  JGeometry::TVec3f(0.0f, -0.3f, 0.0f),
    JGeometry::TVec3f(-0.3f, 0.0f, 0.0f), JGeometry::TVec3f(0.3f, 0.0f, 0.0f),
};
// bss 0x8050C088: four unit directions, filled by the unit's static initialiser.
static JGeometry::TVec3f lbl_8050C088[4] = {
    JGeometry::TVec3f(0.0f, 1.0f, 0.0f),
    JGeometry::TVec3f(0.0f, -1.0f, 0.0f),
    JGeometry::TVec3f(-1.0f, 0.0f, 0.0f),
    JGeometry::TVec3f(1.0f, 0.0f, 0.0f),
};

// Destructor at 0x80366D44.
// FIXME(shotgun): 97.82 %, regalloc-only; behaviour clean.
OtherCharactersEnding_804ADF28::~OtherCharactersEnding_804ADF28() {
    if (field_0x234 == 3) {
        deleteEmitter(&field_0x300);
    }
    if (field_0x234 == 0x10) {
        deleteEmitter(&field_0x304);
        deleteEmitter(&field_0x308);
        for (int i = 0; i < 4; i++) {
            deleteEmitter(&field_0x30c[i]);
        }
    }
}

// Still differs: the original keeps the switch value in a register for case 4/0x11's test (ours reloads field_0x234;
// a local for it leaves only case 5's loop start, where the original copies i into the slot offset and stores i's 0).
// FIXME: 99.34 %, missing/extra; behaviour drift.
void OtherCharactersEnding_804ADF28::init() {
    s32 i;
    field_0x234 = mSpawnParam_0x90 & 0x1f;
    field_0x238 = (mSpawnParam_0x90 >> 5) & 0x7f;
    field_0x23c = (mSpawnParam_0x90 >> 12) & 3;
    field_0x230 = (mSpawnParam_0x90 >> 14) & 0x3f;
    field_0x248 = 0;
    field_0x24c = -1;
    field_0x25c = 0;
    field_0x260 = 0;
    field_0x264 = 0;
    field_0x268 = 0;
    field_0x26c = 0;
    field_0x270 = NULL;
    field_0x274 = NULL;
    field_0x278 = 0;
    field_0x300 = NULL;
    field_0x304 = NULL;
    field_0x308 = NULL;
    for (i = 0; i < 4; i++) {
        field_0x30c[i] = NULL;
    }
    switch (field_0x234) {
    case 0:
        field_0x248 = 0xd0;
        field_0x238 = -1;
        field_0x258 = 2;
        field_0x26c = 0x38b;
        field_0x260 = 8;
        field_0x25c = 8;
        mVel_0x3c.set(-0.75f, 0.0f, 0.0f);
        for (int i = 0; i < 3; i++) {
            field_0x2a0[i] = mPos + JGeometry::TVec3f(16.0f * i + 20.0f, 0.0f, 0.0f);
            field_0x27c[i] = field_0x2a0[i];
            field_0x2c4[i].zero();
            field_0x2e8[i] = 0x38b;
        }
        break;
    case 1:
        field_0x238 = 0;
        field_0x258 = 2;
        field_0x26c = 0x3ad;
        field_0x260 = 8;
        field_0x25c = 8;
        field_0x248 = 100;
        mVel_0x3c.y = 0.25f;
        break;
    case 3:
        field_0x238 = 0;
        field_0x258 = 3;
        field_0x26c = 0xd5;
        field_0x248 = 0x86;
        field_0x260 = 8;
        field_0x300 = spawnEmitter_8013CC50(mLayer_0x4, &mPos, 0x555, -1, 0, NULL, NULL, true, 1.0f);
        break;
    case 4:
    case 0x11:
        if (field_0x234 == 4) {
            field_0x26c = lbl_804745B8[field_0x238];
            field_0x270 = lbl_804ADED0[lbl_80474868[field_0x238]];
            field_0x274 = lbl_804ADF08[lbl_80474914[field_0x238]];
            if (field_0x238 == 0x1d || field_0x238 == 0x1e) {
                field_0x25c = field_0x270[0];
                field_0x264 = field_0x274[0];
            } else {
                field_0x268 = randInt_801CD664(lbl_80542AE0[lbl_80474914[field_0x238]]);
                field_0x25c = randInt_801CD664(field_0x270[field_0x268] + 1);
                field_0x264 = field_0x274[field_0x268];
            }
            if (mSpawnParam_0x90 & 0x80000000) {
                field_0x258 = 3;
            } else {
                field_0x258 = lbl_804749B0[field_0x238];
            }
            if (field_0x238 >= 4 && field_0x238 <= 7) {
                field_0x25c = randInt_801CD664(50);
            }
            if (field_0x238 == 4) {
                field_0x278 = randInt_801CD664(2) != 0;
            }
        } else {
            field_0x26c = lbl_804745B8[field_0x238 + 125];
            field_0x270 = lbl_804ADED0[lbl_804748E8[field_0x238]];
            field_0x274 = lbl_804ADF08[lbl_80474994[field_0x238]];
            field_0x268 = randInt_801CD664(lbl_80542AE0[lbl_80474994[field_0x238]]);
            field_0x25c = randInt_801CD664(field_0x270[field_0x268] + 1);
            field_0x264 = field_0x274[field_0x268];
            if (mSpawnParam_0x90 & 0x80000000) {
                field_0x258 = 3;
            } else {
                field_0x258 = lbl_80474A30[field_0x238];
            }
        }
        break;
    case 5: {
        field_0x24c = -1;
        s32 end = getActorManager_801F856C()->getActorEnd();
        field_0x23c = 0;
        for (i = 0; i < end; i++) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->getActor(i);
            if (actor != NULL && actor->mActorId == ACTOR_EOWL) {
                field_0x24c = i;
                s32 slot = field_0x24c;
                ActorBase_804A5770* owl = getActorManager_801F856C()->getActor(slot);
                if (owl == NULL) {
                    field_0x24c = -1;
                } else {
                    pool_80542B58(owl);
                }
                field_0x26c = lbl_80474320[field_0x238 * 4 + field_0x23c];
                field_0x25c = field_0x260 = lbl_80474450[field_0x238 * 2];
                field_0x258 = lbl_80474450[field_0x238 * 2 + 1];
                break;
            }
        }
        field_0x26c = lbl_80474320[field_0x238 * 4 + field_0x23c];
        field_0x25c = field_0x260 = lbl_80474450[field_0x238 * 2];
        field_0x258 = lbl_80474450[field_0x238 * 2 + 1];
        break;
    }
    case 6:
        field_0x23c = 0;
        mVel_0x3c.set(lbl_80474A4C[field_0x238], 0.0f, 0.0f);
        field_0x248 = 0x20;
        field_0x2f4 = lbl_804ADEC0[field_0x238];
        field_0x2f8 = 0.0f;
        field_0x26c = lbl_80474548[field_0x238][3];
        field_0x264 = 1;
        field_0x258 = 0;
        break;
    case 7:
        mVel_0x3c.zero();
        if ((field_0x238 & 1) == 0) {
            mVel_0x3c.x = 0.8f;
            field_0x264 = 0;
            field_0x23c = 3;
        } else {
            mVel_0x3c.x = -0.8f;
            field_0x264 = 5;
            field_0x23c = 2;
        }
        field_0x238 = 0;
        field_0x26c = 0x38e;
        field_0x25c = 4;
        field_0x258 = 0;
        break;
    case 8:
        field_0x238 = 0;
        field_0x26c = 0x3a9;
        field_0x258 = 0;
        mMapBox_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
        break;
    case 9: {
        field_0x238 = 0;
        field_0x258 = 3;
        field_0x248 = randInt_801CD664(60);
        JGeometry::TVec3f off;
        JGeometry::TVec3f oldPos = mPos;
        mMapBox_0x80.set(-4.0f, -4.0f, 4.0f, 4.0f);
        for (s32 i = 0; i < 60; i++) {
            off.x = (f32)randInt_801CD664(380) - 190.0f;
            off.y = (f32)randInt_801CD664(280) - 140.0f;
            off.z = 0.0f;
            mPos.add(off);
            if (pool_80542B20()) {
                mPos = oldPos;
                i--;
            } else {
                u32 param = randInt_801CD664(8) | 0x30000;
                getActorManager_801F856C()->spawnActor_801F9484('RUPY', mLayer_0x4, &mPos, param, -1, -1);
                mPos = oldPos;
            }
        }
        break;
    }
    case 10:
        field_0x23c = 3;
        if (field_0x238 == 0) {
            field_0x26c = 0x32f;
        } else {
            field_0x26c = 0x33b;
        }
        field_0x26c += lbl_80474598[field_0x23c];
        field_0x270 = lbl_80542A78;
        field_0x274 = lbl_80542A7C;
        field_0x25c = field_0x270[0];
        field_0x264 = field_0x274[0];
        field_0x258 = 0;
        mVel_0x3c.set(lbl_8050C028[field_0x23c]);
        break;
    case 11:
        field_0x238 = 0;
        field_0x26c = 0x375;
        field_0x25c = 7;
        field_0x258 = 0;
        mVel_0x3c.set(0.0f, 0.5f, 0.0f);
        break;
    case 12: {
        field_0x24c = -1;
        field_0x27c[0] = mPos;
        s32 end = getActorManager_801F856C()->getActorEnd();
        for (i = 0; i < end; i++) {
            EndingOwl_804ADF28* actor = static_cast<EndingOwl_804ADF28*>(getActorManager_801F856C()->getActor(i));
            if (actor != NULL && actor->mActorId == ACTOR_EOWL && actor->mAlive == true && actor->field_0x278 == 0) {
                pool_80542B4C(i, actor);
                break;
            }
        }
        field_0x23c = 0;
        s32 slot = field_0x24c;
        if (slot >= 0) {
            ActorBase_804A5770* owl = getActorManager_801F856C()->getActor(slot);
            if (owl != NULL) {
                JGeometry::TVec3f target = owl->mPos;
                if (PSVECDistance(&mPos, &owl->mPos) >= 60.0f) {
                    field_0x24c = -1;
                    mVel_0x3c.zero();
                    field_0x264 = 1;
                    field_0x268 = 0;
                } else {
                    field_0x240 = (s16)(JMath::atanTable_.atan2_(owl->mPos.y - mPos.y, owl->mPos.x - mPos.x) * 10430.378f);
                    mVel_0x3c.x = 0.5f * JMath::sincosTable_.cosShort(field_0x240);
                    mVel_0x3c.y = 0.5f * JMath::sincosTable_.sinShort(field_0x240);
                    u16 angle = (s32)(JMath::atanTable_.atan2_(mVel_0x3c.y, mVel_0x3c.x) * 10430.378f);
                    if (angle >= 0x2000 && angle < 0x6000) {
                        field_0x23c = 0;
                    } else if (angle >= 0x6000 && angle < 0xa000) {
                        field_0x23c = 2;
                    } else if (angle >= 0xa000 && angle < 0xe000) {
                        field_0x23c = 1;
                    } else {
                        field_0x23c = 3;
                    }
                }
            }
        }
        field_0x26c = lbl_80474858[field_0x23c];
        field_0x270 = lbl_80542AD8;
        field_0x274 = lbl_80542AC8;
        field_0x25c = field_0x270[0];
        field_0x264 = field_0x274[0];
        field_0x268 = 0;
        field_0x258 = 0;
        break;
    }
    case 13:
        field_0x23c = randInt_801CD664(5);
        if (field_0x23c == 4) {
            field_0x23c = randInt_801CD664(4);
            mVel_0x3c.zero();
        } else {
            mVel_0x3c.set(lbl_8050C088[field_0x23c]);
        }
        field_0x26c = lbl_804745B8[field_0x23c + 20];
        field_0x270 = lbl_80474810;
        field_0x274 = lbl_80474834;
        field_0x25c = field_0x270[0];
        field_0x264 = field_0x274[0];
        field_0x268 = 0;
        field_0x258 = 1;
        field_0x248 = randInt_801CD664(20) + 30;
        mMapBox_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
        break;
    case 14: {
        field_0x27c[0] = mPos;
        field_0x240 = randInt_801CD664(0x10000);
        mVel_0x3c.x = 0.3f * JMath::sincosTable_.cosShort(field_0x240);
        mVel_0x3c.y = 0.3f * JMath::sincosTable_.sinShort(field_0x240);
        u16 angle = (s32)(JMath::atanTable_.atan2_(mVel_0x3c.y, mVel_0x3c.x) * 10430.378f);
        if (angle >= 0x2000 && angle < 0x6000) {
            field_0x23c = 0;
        } else if (angle >= 0x6000 && angle < 0xa000) {
            field_0x23c = 2;
        } else if (angle >= 0xa000 && angle < 0xe000) {
            field_0x23c = 1;
        } else {
            field_0x23c = 3;
        }
        field_0x248 = randInt_801CD664(20) + 0x40;
        field_0x26c = lbl_804745A8[field_0x23c];
        field_0x260 = 12;
        field_0x25c = 12;
        field_0x258 = 2;
        break;
    }
    case 16: {
        mPos.x -= 48.0f;
        field_0x238 = 0;
        mVel_0x3c.zero();
        field_0x26c = 0xa3;
        field_0x248 = 0xf0;
        field_0x260 = 7;
        field_0x258 = 2;
        spawnRoomEmitters();
        s32 end = getActorManager_801F856C()->getActorEnd();
        for (i = 0; i < end; i++) {
            EndingOwl_804ADF28* actor = static_cast<EndingOwl_804ADF28*>(getActorManager_801F856C()->getActor(i));
            if (actor != NULL && actor->mActorId == ACTOR_EOWL && actor->mAlive == true) {
                field_0x2fc = actor;
                break;
            }
        }
        break;
    }
    }
}

void OtherCharactersEnding_804ADF28::update() {
    if (field_0x230 == 0) {
        if (field_0x248 != 0) {
            field_0x248--;
        }
        if (field_0x25c != 0) {
            field_0x25c--;
        }
        (this->*lbl_804ADC38[field_0x234])();
    }
}

void OtherCharactersEnding_804ADF28::draw() {
    (this->*lbl_804ADDE8[field_0x234])();
}

void OtherCharactersEnding_804ADF28::fn_803653EC() {
    if (field_0x238 < 0) {
        if (field_0x248 >= 0x80) {
            if (field_0x25c == 0) {
                field_0x25c = field_0x260;
                field_0x268++;
                if (field_0x268 >= 4) {
                    field_0x268 = 0;
                }
                field_0x264 = lbl_80474BC4[field_0x268];
            }
            mPos.add(mVel_0x3c);
            field_0x27c[0].add(mVel_0x3c);
            field_0x27c[1].add(mVel_0x3c);
            field_0x27c[2].add(mVel_0x3c);
            if (field_0x248 == 0x80) {
                mVel_0x3c.x = mVel_0x3c.y = mVel_0x3c.z = 0.0f;
                field_0x26c = 0x388;
                field_0x264 = 0;
            }
            return;
        }
        if (field_0x248 == 0) {
            field_0x238 = 0;
        } else {
            return;
        }
    }
    if (field_0x25c == 0) {
        field_0x25c = field_0x260;
        field_0x268++;
        if (field_0x268 >= 4) {
            field_0x268 = 0;
        }
        field_0x264 = lbl_80474BC4[field_0x268];
    }
    if (field_0x248 == 0) {
        field_0x248 = 0x80;
        switch (field_0x238 & 3) {
        case 0: {
            field_0x26c = 0x391;
            static JGeometry::TVec3f lbl_8050C0B8[3] = {
                JGeometry::TVec3f(-16.0f, 16.0f, 0.0f),
                JGeometry::TVec3f(16.0f, 16.0f, 0.0f),
                JGeometry::TVec3f(0.0f, 32.0f, 0.0f),
            };
            for (int i = 0; i < 3; i++) {
                field_0x2a0[i] = mPos + lbl_8050C0B8[i];
                field_0x2c4[i] = JGeometry::TVec3f((field_0x2a0[i].x - field_0x27c[i].x) / 10.0f,
                                                   (field_0x2a0[i].y - field_0x27c[i].y) / 10.0f, 0.0f);
                field_0x2e8[i] = lbl_80474A5C[i];
            }
            break;
        }
        case 1: {
            field_0x26c = 0x388;
            for (int i = 0; i < 3; i++) {
                field_0x2a0[i] = mPos + JGeometry::TVec3f(16.0f * i + 16.0f, 0.0f, 0.0f);
                field_0x2c4[i] = JGeometry::TVec3f((field_0x2a0[i].x - field_0x27c[i].x) / 10.0f,
                                                   (field_0x2a0[i].y - field_0x27c[i].y) / 10.0f, 0.0f);
                field_0x2e8[i] = 0x388;
            }
            break;
        }
        case 2: {
            field_0x26c = 0x38E;
            for (int i = 0; i < 3; i++) {
                field_0x2a0[i] = mPos + JGeometry::TVec3f(0.0f, 16.0f * i + 16.0f, 0.0f);
                field_0x2c4[i] = JGeometry::TVec3f((field_0x2a0[i].x - field_0x27c[i].x) / 10.0f,
                                                   (field_0x2a0[i].y - field_0x27c[i].y) / 10.0f, 0.0f);
                field_0x2e8[i] = 0x38E;
            }
            break;
        }
        case 3: {
            field_0x26c = 0x391;
            static JGeometry::TVec3f lbl_8050C0DC[3] = {
                JGeometry::TVec3f(16.0f, 0.0f, 0.0f),
                JGeometry::TVec3f(0.0f, 16.0f, 0.0f),
                JGeometry::TVec3f(16.0f, 16.0f, 0.0f),
            };
            for (int i = 0; i < 3; i++) {
                field_0x2a0[i] = mPos + lbl_8050C0DC[i];
                field_0x2c4[i] = JGeometry::TVec3f((field_0x2a0[i].x - field_0x27c[i].x) / 10.0f,
                                                   (field_0x2a0[i].y - field_0x27c[i].y) / 10.0f, 0.0f);
                field_0x2e8[i] = 0x391;
            }
            break;
        }
        }
        field_0x238++;
    } else {
        field_0x27c[0].add(field_0x2c4[0]);
        field_0x27c[1].add(field_0x2c4[1]);
        field_0x27c[2].add(field_0x2c4[2]);
        if (field_0x248 == 0x76) {
            field_0x27c[0] = field_0x2a0[0];
            field_0x2c4[0].x = field_0x2c4[0].y = field_0x2c4[0].z = 0.0f;
            field_0x27c[1] = field_0x2a0[1];
            field_0x2c4[1].x = field_0x2c4[1].y = field_0x2c4[1].z = 0.0f;
            field_0x27c[2] = field_0x2a0[2];
            field_0x2c4[2].x = field_0x2c4[2].y = field_0x2c4[2].z = 0.0f;
        }
    }
}

void OtherCharactersEnding_804ADF28::fn_80365274() {
    mPos.add(mVel_0x3c);
    switch (field_0x238) {
    case 0:
    case 2:
        if (field_0x25c == 0) {
            field_0x25c = field_0x260;
            field_0x268++;
            if (field_0x268 >= 4) {
                field_0x268 = 0;
            }
            field_0x264 = lbl_80474BC4[field_0x268];
        }
        if (field_0x248 == 0) {
            field_0x248 = 0xc6;
            mVel_0x3c.z = 0.0f;
            mVel_0x3c.y = 0.0f;
            mVel_0x3c.x = 0.0f;
            field_0x26c = 0x3ac;
            field_0x264 = 0;
            if (field_0x238 == 2) {
                mAlive = false;
            } else {
                field_0x238++;
            }
        }
        break;
    case 1:
        if (field_0x248 == 0x8a) {
            field_0x26c = 0x3a4;
        } else if (field_0x248 == 0x72) {
            field_0x26c = 0x3a8;
        } else if (field_0x248 == 0x5a) {
            field_0x26c = 0x3a4;
        } else if (field_0x248 == 0) {
            field_0x238++;
            field_0x26c = 0x3b1;
            field_0x264 = 0;
            JGeometry::TVec3f bob(0.0f, -0.25f, 0.0f);
            mVel_0x3c = bob;
            field_0x248 = 100;
        }
        break;
    }
}

void OtherCharactersEnding_804ADF28::fn_80365220() {
    if (field_0x248 == 0) {
        field_0x248 = 0x40;
        field_0x238++;
    }
    if ((field_0x238 & 1) == 0) {
        mPos.y += 0.03f;
    } else {
        mPos.y -= 0.03f;
    }
}

void OtherCharactersEnding_804ADF28::fn_8036509C() {
    if (field_0x248 != 0) {
        if (field_0x248 <= 14) {
            if (field_0x248 == 14) {
                if (field_0x300 != NULL) {
                    field_0x300->becomeInvalidEmitter();
                    field_0x300 = NULL;
                }
                field_0x258 = 0;
                field_0x264 = 11;
                spawnEmitter_8013CC50(mLayer_0x4, &mPos, 0x15b, 0, 1, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(mLayer_0x4, &mPos, 0x15c, -1, 0, NULL, NULL, true, 1.0f);
            } else if (field_0x248 == 10) {
                field_0x264 = 10;
            } else if (field_0x248 == 8) {
                field_0x264 = 9;
            } else if (field_0x248 == 6) {
                field_0x264 = 8;
            } else if (field_0x248 == 4) {
                field_0x264 = 2;
            } else if (field_0x248 == 2) {
                field_0x264 = 1;
            }
        }
        if (field_0x248 == 1) {
            field_0x25c = randInt_801CD664(9);
            field_0x264 = 0;
        }
    } else if (field_0x25c == 0) {
        field_0x25c = 8;
        field_0x264++;
        if (field_0x264 >= 4) {
            field_0x264 = 0;
        }
    }
}

void OtherCharactersEnding_804ADF28::fn_80364F60() {
    if (field_0x238 != 5) {
        s32 oldAnim = field_0x264;
        if (field_0x25c == 0) {
            field_0x268++;
            if (field_0x270[field_0x268] < 0) {
                field_0x268 = 0;
            }
            field_0x25c = field_0x270[field_0x268];
            field_0x264 = field_0x274[field_0x268];
        }
        if (oldAnim != field_0x264 && field_0x238 >= 4 && field_0x238 <= 7 && !field_0x264) {
            field_0x25c += randInt_801CD664(5) * 6;
        }
        if (field_0x238 == 0x1d || field_0x238 == 0x1e) {
            if (field_0x268 == 2) {
                if (field_0x25c == 12) {
                    mVel_0x3c.z = 2.7692308f;
                } else {
                    mVel_0x3c.z -= 0.42603552f;
                }
                mPos.z += mVel_0x3c.z;
            } else {
                mPos.z = 0.0f;
                mVel_0x3c.z = 0.0f;
            }
        }
    }
}

void OtherCharactersEnding_804ADF28::fn_80364F00() {
    if (field_0x25c == 0) {
        field_0x268++;
        if (field_0x270[field_0x268] < 0) {
            field_0x268 = 0;
        }
        field_0x25c = field_0x270[field_0x268];
        field_0x264 = field_0x274[field_0x268];
    }
}

// FIXME(shotgun): 99.92 %, regalloc-only; behaviour clean.
void OtherCharactersEnding_804ADF28::fn_80364D0C() {
    ActorBase_804A5770* owl;
    s32 slot = field_0x24c;
    if (slot < 0) {
        s32 end = getActorManager_801F856C()->mActorEnd;
        for (s32 i = 0; i < end; i++) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[i];
            if (actor != NULL && actor->mActorId == ACTOR_EOWL) {
                field_0x24c = i;
                break;
            }
        }
    } else {
        s32 oldDir = field_0x23c;
        owl = getActorManager_801F856C()->getActor(slot);
        if (owl == NULL) {
            field_0x24c = -1;
        } else {
            pool_80542B58(owl);
        }
        if (field_0x23c != oldDir) {
            field_0x26c = lbl_80474320[field_0x238 * 4 + field_0x23c];
            field_0x25c = lbl_80474450[field_0x238 * 2];
        }
    }
    if (field_0x25c == 0) {
        field_0x25c = field_0x260;
        field_0x264 ^= 1;
    }
}

void OtherCharactersEnding_804ADF28::fn_80364A88() {
    if (field_0x248 == 0) {
        f32 speed = PSVECMag(&mVel_0x3c);
        mPos.add(mVel_0x3c);
        field_0x2f8 += speed;
        if (field_0x2f4 != NULL) {
            if (field_0x2f8 >= *field_0x2f4) {
                field_0x2f8 = 0.0f;
                field_0x2f4++;
                field_0x23c++;
                if (field_0x23c == 1) {
                    mVel_0x3c.x = 0.0f;
                    mVel_0x3c.y = -1.0f * speed;
                    mVel_0x3c.z = 0.0f;
                    field_0x26c = lbl_80474548[field_0x238][1];
                } else if (field_0x23c == 2) {
                    mVel_0x3c.x = speed;
                    mVel_0x3c.y = 0.0f;
                    mVel_0x3c.z = 0.0f;
                    field_0x26c = lbl_80474548[field_0x238][3];
                } else if (field_0x23c == 3) {
                    mVel_0x3c.x = 0.0f;
                    mVel_0x3c.y = speed;
                    mVel_0x3c.z = 0.0f;
                    field_0x26c = lbl_80474548[field_0x238][0];
                } else if (field_0x23c == 4) {
                    mVel_0x3c.x = speed;
                    mVel_0x3c.y = 0.0f;
                    mVel_0x3c.z = 0.0f;
                    field_0x26c = lbl_80474548[field_0x238][3];
                } else {
                    field_0x26c = lbl_80474548[4][field_0x238];
                    field_0x260 = 15;
                    field_0x25c = 15;
                    if (field_0x238 == 1) {
                        field_0x264 = 1;
                    } else {
                        field_0x264 = 0;
                    }
                    mVel_0x3c.x = mVel_0x3c.y = mVel_0x3c.z = 0.0f;
                    field_0x2f4 = NULL;
                }
            }
            if (field_0x25c == 0) {
                field_0x268++;
                if (field_0x268 >= 4) {
                    field_0x268 = 0;
                }
                if (field_0x238 == 1) {
                    field_0x25c = lbl_80474A78[field_0x268];
                } else {
                    field_0x25c = lbl_80474A88[field_0x268];
                }
                field_0x264 = lbl_80474A68[field_0x268];
            }
        } else if (field_0x238 != 1 && field_0x25c == 0) {
            field_0x25c = field_0x260;
            field_0x264 ^= 1;
        }
    }
}

void OtherCharactersEnding_804ADF28::fn_80364900() {
    if (field_0x238 == 0) {
        mPos.add(mVel_0x3c);
        if (field_0x25c != 0) {
            return;
        }
        field_0x268++;
        if (field_0x268 >= 0x10) {
            field_0x238 = 1;
            field_0x264 = 0;
            field_0x268 = 0;
            field_0x26c = 0x2b1;
            mVel_0x3c.z = 5.0f;
            mVel_0x3c.x = 0.0f;
            return;
        }
        field_0x25c = lbl_80474A98[field_0x268];
        if (mVel_0x3c.x < 0.0f) {
            field_0x264 = lbl_80474AD8[field_0x268 + 16];
        } else {
            field_0x264 = lbl_80474AD8[field_0x268];
        }
    } else {
        mVel_0x3c.z -= 0.5f;
        mPos.add(mVel_0x3c);
        if (mPos.z < 0.0f) {
            field_0x238 = 0;
            mPos.z = 0.0f;
            mVel_0x3c.z = 0.0f;
            field_0x26c = 0x38e;
            field_0x25c = 4;
            if (field_0x23c == 3) {
                field_0x23c = 2;
                mVel_0x3c.x = -0.8f;
                field_0x264 = 5;
                field_0x25c = 4;
            } else {
                field_0x23c = 3;
                mVel_0x3c.x = 0.8f;
                field_0x26c = 0x38e;
                field_0x264 = 0;
            }
        }
    }
}

void OtherCharactersEnding_804ADF28::fn_803645B4() {
    JGeometry::TVec3f target;
    if (field_0x248 == 0 || field_0x238 != 0) {
        if (field_0x238 == 0) {
            if (0 <= field_0x24c) {
                ActorBase_804A5770* owl = getActorManager_801F856C()->getActor(field_0x24c);
                if (owl != NULL) {
                    owl->mAlive = false;
                }
            }
            s32 end = getActorManager_801F856C()->getActorEnd();
            for (s32 i = 0; i < end; i++) {
                ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[i];
                if (actor != NULL && actor->mActorId == 'RUPY') {
                    target = actor->mPos;
                    target.y += 4.0f;
                    if (actor->mPos.z == 0.0f && PSVECDistance(&mPos, &target) < 80.0f &&
                        PSVECDistance(&mPos, &target) > 30.0f) {
                        field_0x238 = 1;
                        mVel_0x3c.x = (target.x - mPos.x) / 40.0f;
                        mVel_0x3c.y = (target.y - mPos.y) / 40.0f;
                        field_0x24c = i;
                        field_0x264 = 0;
                        field_0x268 = 0;
                        field_0x25c = 3;
                        u16 angle = (s16)(JMath::atanTable_.atan2_(mVel_0x3c.y, mVel_0x3c.x) * (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180()));
                        if (angle >= 0x2000 && angle < 0x6000) {
                            field_0x23c = 0;
                        } else if (angle >= 0x6000 && angle < 0xa000) {
                            field_0x23c = 2;
                        } else if (angle >= 0xa000 && angle < 0xe000) {
                            field_0x23c = 1;
                        } else {
                            field_0x23c = 3;
                        }
                        field_0x26c = lbl_80474B58[field_0x23c];
                        field_0x248 = 0x3c;
                        break;
                    }
                }
            }
        } else {
            mPos.add(mVel_0x3c);
            sweepMapBox_801F2618(NULL, false);
            ActorBase_804A5770* actor = getActorManager_801F856C()->getActor(field_0x24c);
            if (actor == NULL) {
                field_0x24c = -1;
                field_0x238 = 0;
                field_0x264 = 0;
                field_0x268 = 0;
            } else {
                if (field_0x25c == 0) {
                    field_0x264++;
                    if (field_0x264 >= 8) {
                        field_0x264 = 0;
                    }
                    field_0x25c = lbl_80474B68[field_0x264];
                }
                target = actor->mPos;
                target.y += 4.0f;
                if (PSVECDistance(&mPos, &target) < 4.0f) {
                    field_0x238 = 0;
                    field_0x248 = 30;
                    field_0x264 = 0;
                } else if (field_0x248 == 0) {
                    field_0x238 = 0;
                    field_0x248 = 10;
                    field_0x264 = 0;
                }
            }
        }
    }
}

void OtherCharactersEnding_804ADF28::fn_80364340() {
    if (field_0x248 == 0) {
        JGeometry::TVec3f off;
        JGeometry::TVec3f oldPos = mPos;
        off.x = (f32)randInt_801CD664(380) - 190.0f;
        off.y = (f32)randInt_801CD664(380) - 140.0f;
        off.z = 0.0f;
        mPos.add(off);
        if (pool_80542B20()) {
            mPos = oldPos;
        } else {
            field_0x248 = randInt_801CD664(50) + 60;
            off.z = 100.0f;
            mPos = oldPos;
            u32 param = randInt_801CD664(8) | 0x250000;
            const JGeometry::TVec3f& spawnPos = mPos + off;
            getActorManager_801F856C()->spawnActor_801F9484('RUPY', mLayer_0x4, &spawnPos, param, -1, -1);
        }
    }
}

// FIXME: 99.71 %, regalloc-only; behaviour clean.
void OtherCharactersEnding_804ADF28::fn_80363FFC() {
    s32 tileX;
    s32 tileY;
    if (mVel_0x3c.x == 0.0f) {
        tileX = (s32)mPos.x;
        if (mVel_0x3c.y < 0.0f) {
            tileY = (s32)(mPos.y - 24.0f);
        } else {
            tileY = (s32)(24.0f + mPos.y);
        }
    } else {
        tileY = (s32)mPos.y;
        if (mVel_0x3c.x < 0.0f) {
            tileX = (s32)(mPos.x - 24.0f);
        } else {
            tileX = (s32)(24.0f + mPos.x);
        }
    }
    if (mVel_0x3c.z == 0.0f && mPos.z == 0.0f) {
        s32 cell = ((tileY & 0x100) << 1) | (tileX & 0x100) | (tileY & 0xF0) | ((tileX >> 4) & 0xF);
        field_0x27a = gGameRoot2_8053AB10->field_0x20->getTile_802F9908(mLayer_0x4, cell) & 0x3FF;
        if (field_0x27a == 0x1a8) {
            field_0x264 = 0;
            mVel_0x3c.z = 3.0f;
        }
    } else {
        if (mPos.z > 0.0f) {
            mVel_0x3c.z -= 0.125f;
        } else {
            mPos.z = 0.0f;
            mVel_0x3c.z = 0.0f;
        }
        field_0x264 = 0;
        field_0x25c = 1;
    }
    JGeometry::TVec3f center(1808.0f, 3296.0f, 0.0f);
    if (mPos.x >= center.x) {
        if (mPos.y < center.y) {
            s32 angle = JMath::atanTable_.atan2_(mPos.y - center.y, mPos.x - center.x) * 10430.378f;
            f32 dist = PSVECDistance(&mPos, &center);
            s32 delta = 65536.0f * (JMath::fastReciprocal(6.283184f * dist) * 1.3f);
            angle += delta;
            mPos.x = center.x + dist * JMath::sincosTable_.cosShort(angle);
            mPos.y = center.y + dist * JMath::sincosTable_.sinShort(angle);
            if ((u16)angle >= 0xe000) {
                field_0x23c = 0;
                if (field_0x238 == 0) {
                    field_0x26c = 0x32f;
                } else {
                    field_0x26c = 0x33b;
                }
                field_0x26c += lbl_80474598[field_0x23c];
            }
            mVel_0x3c.z = 0.0f;
            mVel_0x3c.y = 0.0f;
            mVel_0x3c.x = 0.0f;
        } else {
            mVel_0x3c.set(lbl_8050C028[0]);
        }
    }
    mPos.add(mVel_0x3c);
    if (field_0x25c == 0) {
        field_0x268++;
        if (field_0x270[field_0x268] < 0) {
            field_0x268 = 0;
        }
        field_0x25c = field_0x270[field_0x268];
        field_0x264 = field_0x274[field_0x268];
    }
}

void OtherCharactersEnding_804ADF28::fn_80363F7C() {
    if (field_0x25c == 0) {
        field_0x268++;
        if (field_0x268 >= 6) {
            field_0x268 = 0;
        }
        field_0x25c = lbl_80474B88[field_0x268];
        field_0x264 = lbl_80474BA0[field_0x268];
    }
    mPos.add(mVel_0x3c);
}

// FIXME(shotgun): 99.59 %, regalloc-only; behaviour clean.
void OtherCharactersEnding_804ADF28::fn_80363880() {
    s32 i = field_0x24c;
    if (i >= 0) {
        ActorBase_804A5770* owl = getActorManager_801F856C()->getActor(i);
        if (owl != NULL) {
            JGeometry::TVec3f target = owl->mPos;
            if (PSVECDistance(&mPos, &target) >= 100.0f) {
                field_0x24c = -1;
                mVel_0x3c.x = mVel_0x3c.y = mVel_0x3c.z = 0.0f;
                field_0x264 = 1;
                field_0x268 = 0;
                field_0x248 = 0x20;
            } else {
                target.z = 0.0f;
                if (PSVECDistance(&mPos, &target) >= 10.0f) {
                    field_0x240 = (s16)(JMath::atanTable_.atan2_(target.y - mPos.y, target.x - mPos.x) * 10430.378f);
                    mVel_0x3c.x = 0.5f * JMath::sincosTable_.cosShort(field_0x240);
                    mVel_0x3c.y = 0.5f * JMath::sincosTable_.sinShort(field_0x240);
                }
                u16 angle = (s32)(JMath::atanTable_.atan2_(mVel_0x3c.y, mVel_0x3c.x) * 10430.378f);
                if (angle >= 0x2000 && angle < 0x6000) {
                    field_0x23c = 0;
                } else if (angle >= 0x6000 && angle < 0xa000) {
                    field_0x23c = 2;
                } else if (angle >= 0xa000 && angle < 0xe000) {
                    field_0x23c = 1;
                } else {
                    field_0x23c = 3;
                }
                field_0x26c = lbl_80474858[field_0x23c];
            }
        } else {
            field_0x24c = -1;
            mVel_0x3c.x = mVel_0x3c.y = mVel_0x3c.z = 0.0f;
            field_0x264 = 1;
            field_0x268 = 0;
            field_0x248 = 0x20;
        }
    } else {
        s32 end = getActorManager_801F856C()->getActorEnd();
        for (i = 0; i < end; i++) {
            EndingOwl_804ADF28* actor = static_cast<EndingOwl_804ADF28*>(getActorManager_801F856C()->getActor(i));
            if (actor != NULL && actor->mActorId == ACTOR_EOWL && actor->mAlive == true && actor->field_0x278 == 0) {
                pool_80542B4C(i, actor);
                break;
            }
        }
        if (field_0x24c < 0) {
            if (field_0x248 == 0) {
                s32 r = randInt_801CD664(3);
                if (r == 0) {
                    field_0x24c = -1;
                    mVel_0x3c.x = mVel_0x3c.y = mVel_0x3c.z = 0.0f;
                    field_0x264 = 1;
                    field_0x268 = 0;
                    field_0x248 = 0x20;
                } else {
                    if (mVel_0x3c.squared() <= 32.0f * FLT_EPSILON) {
                        field_0x240 = randInt_801CD664(0x10000);
                    } else if (r == 1) {
                        field_0x240 += 0x1000;
                    } else {
                        field_0x240 -= 0x1000;
                    }
                    mVel_0x3c.x = 0.5f * JMath::sincosTable_.cosShort(field_0x240);
                    mVel_0x3c.y = 0.5f * JMath::sincosTable_.sinShort(field_0x240);
                    field_0x248 = 0x40;
                }
            }
            if (PSVECDistance(&mPos, &field_0x27c[0]) >= 60.0f) {
                field_0x248 = 0x80;
                field_0x240 = (s16)(JMath::atanTable_.atan2_(field_0x27c[0].y - mPos.y, field_0x27c[0].x - mPos.x) * 10430.378f);
                mVel_0x3c.x = 0.5f * JMath::sincosTable_.cosShort(field_0x240);
                mVel_0x3c.y = 0.5f * JMath::sincosTable_.sinShort(field_0x240);
            }
        }
    }
    if (!(mVel_0x3c.squared() <= 32.0f * FLT_EPSILON)) {
        u16 angle = (s32)(JMath::atanTable_.atan2_(mVel_0x3c.y, mVel_0x3c.x) * 10430.378f);
        if (angle >= 0x2000 && angle < 0x6000) {
            field_0x23c = 0;
        } else if (angle >= 0x6000 && angle < 0xa000) {
            field_0x23c = 2;
        } else if (angle >= 0xa000 && angle < 0xe000) {
            field_0x23c = 1;
        } else {
            field_0x23c = 3;
        }
        field_0x26c = lbl_80474858[field_0x23c];
        mPos.add(mVel_0x3c);
        if (field_0x25c == 0) {
            field_0x268++;
            if (field_0x270[field_0x268] < 0) {
                field_0x268 = 0;
            }
            field_0x25c = field_0x270[field_0x268];
            field_0x264 = field_0x274[field_0x268];
        }
    }
}

void OtherCharactersEnding_804ADF28::fn_80363664() {
    if (field_0x248 == 0) {
        if (mVel_0x3c.squared() <= 32.0f * FLT_EPSILON) {
            s32 oldFrame = field_0x26c;
            do {
                field_0x23c = randInt_801CD664(4);
                mVel_0x3c.set(lbl_8050C088[field_0x23c]);
                field_0x248 = randInt_801CD664(20) + 30;
                field_0x26c = lbl_804745B8[field_0x23c + 20];
            } while (field_0x26c == oldFrame);
        } else {
            mVel_0x3c.zero();
            field_0x248 = randInt_801CD664(10) + 15;
            field_0x264 = 0;
        }
    }
    if (!(mVel_0x3c.squared() <= 32.0f * FLT_EPSILON)) {
        if (sweepMapBox_801F2618(NULL, false)) {
            mVel_0x3c.zero();
            field_0x248 = randInt_801CD664(10) + 15;
            field_0x264 = 0;
        } else {
            mPos.add(mVel_0x3c);
            if (field_0x25c == 0) {
                field_0x268++;
                if (field_0x270[field_0x268] < 0) {
                    field_0x268 = 0;
                }
                field_0x25c = field_0x270[field_0x268];
                field_0x264 = field_0x274[field_0x268];
            }
        }
    }
}

void OtherCharactersEnding_804ADF28::fn_80363360() {
    if (field_0x248 == 0) {
        s32 r = randInt_801CD664(3);
        if (r == 0) {
            mVel_0x3c.z = 0.0f;
            mVel_0x3c.y = 0.0f;
            mVel_0x3c.x = 0.0f;
            field_0x264 = 0;
            field_0x268 = 0;
            field_0x248 = 0x20;
        } else {
            if (mVel_0x3c.squared() <= 32.0f * FLT_EPSILON) {
                field_0x240 = randInt_801CD664(0x10000);
            } else if (r == 1) {
                field_0x240 += 0x1000;
            } else {
                field_0x240 -= 0x1000;
            }
            mVel_0x3c.x = 0.3f * JMath::sincosTable_.cosShort(field_0x240);
            mVel_0x3c.y = 0.3f * JMath::sincosTable_.sinShort(field_0x240);
            field_0x248 = 0x40;
        }
    }
    if (PSVECDistance(&mPos, &field_0x27c[0]) >= 45.0f) {
        field_0x248 = 0x80;
        field_0x240 = (s16)(JMath::atanTable_.atan2_(field_0x27c[0].y - mPos.y, field_0x27c[0].x - mPos.x) * (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180()));
        mVel_0x3c.x = 0.3f * JMath::sincosTable_.cosShort(field_0x240);
        mVel_0x3c.y = 0.3f * JMath::sincosTable_.sinShort(field_0x240);
    }
    if (!(mVel_0x3c.squared() <= 32.0f * FLT_EPSILON)) {
        u16 angle = (s32)(JMath::atanTable_.atan2_(mVel_0x3c.y, mVel_0x3c.x) * (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180()));
        if (angle >= 0x2000 && angle < 0x6000) {
            field_0x23c = 0;
        } else if (angle >= 0x6000 && angle < 0xa000) {
            field_0x23c = 2;
        } else if (angle >= 0xa000 && angle < 0xe000) {
            field_0x23c = 1;
        } else {
            field_0x23c = 3;
        }
        field_0x26c = lbl_804745A8[field_0x23c];
        if (field_0x25c == 0) {
            field_0x25c = field_0x260;
            field_0x268++;
            if (field_0x268 >= 4) {
                field_0x268 = 0;
            }
            field_0x264 = lbl_80474BC4[field_0x268];
        }
        mPos.add(mVel_0x3c);
    }
}

void OtherCharactersEnding_804ADF28::fn_8036335C() {}

// The first non-zero fn_80309D6C result over the four players, else 0: fn_80362C44's
// loop leaves with the result in r3 and loads 0 after the last player.
static inline u32 anyPlayerInput(u32 mask) {
    for (s32 player = 0; player < 4; player++) {
        u32 result = lbl_8053AD30->fn_80309D6C(getPlayerInputSlot_8030AAF4(player), mask);
        if (result != 0) {
            return result;
        }
    }
    return 0;
}

// Still differs: the original keeps the 0 that clears field_0x30c[i] in r26 (shared with case 6's
// player counter) and colours i before the two loop pointers; the jump table's name.
// FIXME: 97.90 %, different; behaviour drift.
void OtherCharactersEnding_804ADF28::fn_80362C44() {
    s32 i;
    s32 skip = 0;
    JGeometry::TVec3f pos = mPos;
    switch (field_0x238) {
    case 0:
        if (field_0x248 == 0) {
            field_0x26c = 0x9b;
            field_0x25c = field_0x260;
            mVel_0x3c.x = 0.6f;
            mVel_0x3c.y = 0.0f;
            mVel_0x3c.z = 0.0f;
            field_0x2f8 = 0.0f;
            field_0x238++;
        }
        break;
    case 1:
        mPos.add(mVel_0x3c);
        if (field_0x25c == 0) {
            field_0x25c = field_0x260;
            field_0x264++;
            field_0x264 &= 7;
        }
        field_0x2f8 += mVel_0x3c.x;
        if (field_0x2f8 >= 160.0f) {
            field_0x238++;
            mVel_0x3c.x = 0.0f;
            mVel_0x3c.y = -0.6f;
            mVel_0x3c.z = 0.0f;
            field_0x26c = 0xb3;
            field_0x2f8 = 0.0f;
        }
        break;
    case 2:
        mPos.add(mVel_0x3c);
        if (field_0x25c == 0) {
            field_0x25c = field_0x260;
            field_0x264++;
            field_0x264 &= 7;
        }
        field_0x2f8 = JMAAbs(mVel_0x3c.y) + field_0x2f8;
        if (field_0x2f8 >= 160.0f) {
            field_0x238++;
            mVel_0x3c.z = 0.0f;
            mVel_0x3c.y = 0.0f;
            mVel_0x3c.x = 0.0f;
            field_0x264 = 0;
            field_0x248 = 0x3c;
        }
        break;
    case 3:
        if (field_0x248 == 0) {
            field_0x248 = 0x5c;
            field_0x2fc->field_0x294 = 1;
            field_0x238++;
        }
        break;
    case 4:
        if (field_0x248 != 0) {
            if (field_0x248 >= 0x3c) {
                pos.y -= (f32)(0x20 - (field_0x248 - 0x3c));
            } else {
                pos.y -= 32.0f;
            }
            if (field_0x248 == 1) {
                mVel_0x3c.x = 0.0f;
                mVel_0x3c.y = -0.4f;
                mVel_0x3c.z = 0.0f;
                field_0x26c = 0xb3;
                field_0x2f8 = 0.0f;
            }
        } else {
            mPos.add(mVel_0x3c);
            if (field_0x25c == 0) {
                field_0x25c = field_0x260;
                field_0x264++;
                field_0x264 &= 7;
            }
            skip = 1;
            field_0x2f8 = JMAAbs(mVel_0x3c.y) + field_0x2f8;
            if (field_0x2f8 >= 80.0f) {
                field_0x248 = 300;
                field_0x250 = (f32)gGameRoot2_8053AB10->field_0x2c->field_0xc;
                field_0x254 = field_0x250 / (f32)field_0x248;
                JGeometry::TVec3f effectPos((f32)(s32)(mPos.x * 0.001953125f) * 512.0f + 256.0f,
                                            (f32)(s32)(mPos.y / 384.0f) * 384.0f + 192.0f, 0.0f);
                static const u16 lbl_80542AF0[4] = {0x4A4, 0x4A5, 0x4A6, 0x4A7};
                for (i = 0; i < 4; i++) {
                    spawnEmitter_8013CC50(0, &effectPos, lbl_80542AF0[i], 0, 0, NULL, NULL, true, 1.0f);
                    deleteEmitter(&field_0x30c[i]);
                }
                effectPos.zero();
                spawnEmitter_8013CC50(0, &effectPos, 0x55a, 0, 0, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(0, &effectPos, 0x55b, 0, 0, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(0, &effectPos, 0x55c, 0, 0, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(0, &effectPos, 0x55d, 0, 0, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(0, &effectPos, 0x55e, 0, 0, NULL, NULL, true, 1.0f);
                field_0x304 = spawnEmitter_8013CC50(0, &effectPos, 0x558, 0, 0, NULL, NULL, true, 1.0f);
                field_0x308 = spawnEmitter_8013CC50(0, &effectPos, 0x559, 0, 0, NULL, NULL, true, 1.0f);
                field_0x238++;
            }
        }
        break;
    case 5:
        skip = 1;
        if (field_0x250 > 0.0f) {
            field_0x250 -= field_0x254;
            if (field_0x250 <= 0.0f) {
                field_0x250 = 0.0f;
                field_0x254 = 0.0f;
            }
        }
        gGameRoot2_8053AB10->field_0x2c->field_0xc = (u8)field_0x250;
        if (field_0x248 == 0) {
            fn_80119DD0()->fn_80119958();
            gGameRoot2_8053AB10->field_0x2c->field_0xc = 0;
            field_0x238++;
            field_0x244 = 0;
            field_0x248 = 500;
        }
        break;
    case 6:
        field_0x244++;
        gGameRoot2_8053AB10->field_0x2c->field_0xc = 0;
        if (field_0x248 == 0) {
            skip = 1;
            if (anyPlayerInput(0x100) != 0) {
                field_0x238++;
            }
        }
        break;
    case 7:
        gGameRoot2_8053AB10->field_0x2c->field_0xc = 0;
        skip = 1;
        getEffectSystem_8013C824()->mEmitterManager->forceDeleteEmitter(field_0x304);
        getEffectSystem_8013C824()->mEmitterManager->forceDeleteEmitter(field_0x308);
        gGameRoot_8053AAF8->fn_8013670C(1);
        field_0x238++;
        break;
    default:
        gGameRoot2_8053AB10->field_0x2c->field_0xc = 0;
        skip = 1;
        break;
    }
    if (skip == 0) {
        field_0x2fc->mPos = pos;
    }
}

void OtherCharactersEnding_804ADF28::fn_80362B74() {
    drawSprite_801F06F0("enemy_both2_sob.bin", field_0x26c + field_0x264, NULL, 7, 1.0f, 1.0f);
    JGeometry::TVec3f savedPos = mPos;
    for (int i = 0; i < 3; i++) {
        mPos = field_0x27c[i];
        drawSprite_801F06F0("enemy_both2_sob.bin", field_0x2e8[i] + field_0x264, NULL, 7, 1.0f, 1.0f);
    }
    mPos = savedPos;
}

void OtherCharactersEnding_804ADF28::fn_80362B14() {
    void* res = getBossArchiveResource_804032E8('TIMG', "fune.bti");
    if (mLayer_0x4 < 8) {
        gGameRoot2_8053AB10->field_0x20->drawTimgImage_802F745C(&mPos, res, NULL);
    }
}

void OtherCharactersEnding_804ADF28::fn_803628D4() {
    JGeometry::TVec3f savedPos = mPos;
    Mtx identity;
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    if ((field_0x268 & 7) >= 5) {
        mtx[0][3] = 0.0f;
        mtx[1][3] = 0.0f;
        mtx[2][3] = 0.0f;
        mtx[0][0] = 1.0f;
        mtx[0][1] = 0.0f;
        mtx[0][2] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = 1.0f;
        mtx[1][2] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = -1.0f;
        params.mMtx = mtx;
        mPos.y -= lbl_80474BB8[(field_0x268 & 7) - 5];
    }
    drawSprite_801F06F0("enemy_gc_sob.bin", field_0x26c + field_0x264, &params, -1, 1.0f, 1.0f);
    mPos = savedPos;
    identity[0][0] = 1.0f;
    identity[0][1] = 0.0f;
    identity[0][2] = 0.0f;
    identity[1][0] = 0.0f;
    identity[1][1] = 1.0f;
    identity[1][2] = 0.0f;
    identity[2][0] = 0.0f;
    identity[2][1] = 0.0f;
    identity[2][2] = 1.0f;
    params.mMtx = identity;
    s32 frame = field_0x26c + field_0x264;
    if (mSpriteParams.mMtx == NULL) {
        drawShadow_801EF830(mLayer_0x4, &mPos, &params, 7, "enemy_gc_sob.bin", frame, NULL);
    }
}

void OtherCharactersEnding_804ADF28::fn_80362680() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    params.field_0x1f = 0;
    if (field_0x234 == 0x10 || field_0x234 == 8) {
        params.field_0x1d = 1;
    }
    if (field_0x234 == 4) {
        if (field_0x238 == 0x1d || field_0x238 == 0x1e) {
            params.field_0x26 = 0;
        }
        if (field_0x238 >= 0x26 && field_0x238 <= 0x29) {
            params.field_0x10.a = 0x66;
        }
    }
    if (field_0x278 == 1) {
        mtx[0][3] = 0.0f;
        mtx[1][3] = 0.0f;
        mtx[2][3] = 0.0f;
        mtx[0][0] = -1.0f;
        mtx[0][1] = 0.0f;
        mtx[0][2] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = 1.0f;
        mtx[1][2] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = 1.0f;
        params.mMtx = mtx;
    }
    if (field_0x258 == 0) {
        drawSprite_801F06F0("enemy_gc_sob.bin", field_0x26c + field_0x264, &params, 7, 1.0f, 1.0f);
    } else if (field_0x258 == 1) {
        drawSprite_801F06F0("enemy_both_sob.bin", field_0x26c + field_0x264, &params, 7, 1.0f, 1.0f);
    } else if (field_0x258 == 2) {
        drawSprite_801F06F0("enemy_both2_sob.bin", field_0x26c + field_0x264, &params, 7, 1.0f, 1.0f);
    }
}

// The four functions below are plain functions that init and the state steps inline and whose own
// copies the linker dropped: the original's .sdata2 has their literals ahead of fn_80362680's
// -1.0f, in the order spawnRoomEmitters (192.0f ... 1.0f), isBlocked (2600.0f ... 216.0f),
// chaseOwl (100.0f, 10430.378f, 0.8f), faceOwl (80.0f), so they were generated here, bottom up
// (tools/pool.py).

// Faces the owl when it is nearer than 80, else down (init's state 5 and fn_80364D0C).
// R2 pool helper (inlined at 2 call sites); was faceOwl.
void OtherCharactersEnding_804ADF28::pool_80542B58(ActorBase_804A5770* owl) {
    if (PSVECDistance(&mPos, &owl->mPos) >= 80.0f) {
        field_0x23c = 0;
    } else {
        field_0x240 = (s16)(JMath::atanTable_.atan2_(owl->mPos.y - mPos.y, owl->mPos.x - mPos.x) * 10430.378f);
        u16 angle = field_0x240;
        if (angle >= 0x2000 && angle < 0x6000) {
            field_0x23c = 0;
        } else if (angle >= 0x6000 && angle < 0xa000) {
            field_0x23c = 2;
        } else if (angle >= 0xa000 && angle < 0xe000) {
            field_0x23c = 1;
        } else {
            field_0x23c = 3;
        }
    }
}

// Takes the owl in actor slot i as the target when it is nearer than 100 and starts walking to it
// at speed 0.8 (init's state 12 and fn_80363880).
// R2 pool helper (inlined at 2 call sites); was chaseOwl.
void OtherCharactersEnding_804ADF28::pool_80542B4C(s32 i, EndingOwl_804ADF28* actor) {
    if (PSVECDistance(&mPos, &actor->mPos) < 100.0f) {
        field_0x24c = i;
        JGeometry::TVec3f target = actor->mPos;
        field_0x240 = (s16)(JMath::atanTable_.atan2_(actor->mPos.y - mPos.y, actor->mPos.x - mPos.x) * 10430.378f);
        mVel_0x3c.x = 0.8f * JMath::sincosTable_.cosShort(field_0x240);
        mVel_0x3c.y = 0.8f * JMath::sincosTable_.sinShort(field_0x240);
        u16 angle = (s32)(JMath::atanTable_.atan2_(mVel_0x3c.y, mVel_0x3c.x) * 10430.378f);
        if (angle >= 0x2000 && angle < 0x6000) {
            field_0x23c = 0;
        } else if (angle >= 0x6000 && angle < 0xa000) {
            field_0x23c = 2;
        } else if (angle >= 0xa000 && angle < 0xe000) {
            field_0x23c = 1;
        } else {
            field_0x23c = 3;
        }
        field_0x26c = lbl_80474858[field_0x23c];
        field_0x270 = lbl_80542AD8;
        field_0x274 = lbl_80542AC8;
        field_0x25c = field_0x270[0];
        field_0x264 = field_0x274[0];
        field_0x258 = 0;
    }
}

// Whether mPos lies outside the area the rupees may land in: its bounds, the two cut corners and
// the two circles of radius 78 (init's state 9 and fn_80364340).
// R2 pool helper (inlined at 2 call sites); was isBlocked.
bool OtherCharactersEnding_804ADF28::pool_80542B20() {
    if (mPos.x < 2600.0f || mPos.x > 3016.0f) {
        return true;
    }
    if (mPos.y < 40.0f || mPos.y > 328.0f) {
        return true;
    }
    if (mPos.x + mPos.y < 2704.0f) {
        return true;
    }
    if (mPos.x > 2984.0f && mPos.y < 120.0f) {
        return true;
    }
    JGeometry::TVec3f c(2688.0f, 120.0f, 0.0f);
    if (PSVECDistance(&mPos, &c) < 78.0f) {
        return true;
    }
    c.set(2944.0f, 216.0f, 0.0f);
    if (PSVECDistance(&mPos, &c) < 78.0f) {
        return true;
    }
    return false;
}

// The four emitters of lbl_80542AE8 at the centre of the room (init's state 16).
// FAKEMATCH(crack-pass): pool helper failing R2 (1 call site, 8-line definition): split out only where the .sdata2 order needs it.
void OtherCharactersEnding_804ADF28::spawnRoomEmitters() {
    // the centre of the room (512 x 384) the actor is in
    JGeometry::TVec3f center((s32)(mPos.x / 512.0f) * 512.0f + 256.0f,
                             (s32)(mPos.y / 384.0f) * 384.0f + 192.0f, 0.0f);
    for (int i = 0; i < 4; i++) {
        field_0x30c[i] = spawnEmitter_8013CC50(mLayer_0x4, &center, lbl_80542AE8[i], 0, 0, NULL, NULL, true, 1.0f);
    }
}

void OtherCharactersEnding_804ADF28::fn_80362634() {
    deleteEmitter(&field_0x300);
}
