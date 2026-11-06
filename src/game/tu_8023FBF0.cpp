#include "JSystem/JUtility/JUTGamePad.h"
#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/BoxTest.h"
#include "game/GameGlobals.h"
#include "game/GameMath.h"
#include "game/GameMode.h"
#include "game/Map.h"
#include "game/Effect.h"
#include "game/EmitterDelete.h"
#include "game/GameState.h"
#include "game/PlayerBit.h"
#include "game/PlayerFlags.h"
#include "game/StageFlag.h"
#include "game/act/act_small_key.h"
#include "game/ViewLayer.h"
#include "game/cls_804A71A8.h"
#include "game/cls_804AE9B0.h"
#include "game/cls_804B20F8.h"
#include "game/tu_8030AE84.h"
#include "game/tu_80379F84.h"

// Offsets of 10 along the facing (right, left, down, up), fn_80250090.
static const f32 lbl_8046AE08[4][2] = {
    {10.0f, 0.0f},
    {-10.0f, 0.0f},
    {0.0f, 10.0f},
    {0.0f, -10.0f},
};

// The point probed behind the player by facing (left, right, up, down),
// fn_8024F63C.
static const Vec lbl_8046AE28[4] = {
    {-10.0f, 0.0f, 0.0f},
    {10.0f, 0.0f, 0.0f},
    {0.0f, -10.0f, 0.0f},
    {0.0f, 10.0f, 0.0f},
};

// Offset of the box in front by facing (right, left, down, up), fn_8024F63C.
static const f32 lbl_8046AE58[4][2] = {
    {10.0f, 0.0f},
    {-10.0f, 0.0f},
    {0.0f, 10.0f},
    {0.0f, -10.0f},
};

// Steps of 16 along the facing (right, left, down, up), fn_8024CD20.
static const Vec lbl_8046AE78[4] = {
    {15.999f, 0.0f, 0.0f},
    {-15.999f, 0.0f, 0.0f},
    {0.0f, 15.999f, 0.0f},
    {0.0f, -15.999f, 0.0f},
};

// Whether the player is in, or about to enter, state 0x1B (the current or
// the requested state); a materialised bool where it is inlined (li 0 / li 1).
static inline bool inState1B(const Player_804A6F38* player) {
    bool result = false;
    if (player->field_0x1240 == 0x1b || player->field_0x1244 == 0x1b) {
        result = true;
    }
    return result;
}

// The layer queries of fn_80240DCC. Each is written as a "bool r = false;
// if (...) r = true; return r;" inline: the original zeroes the three
// results with one li and two mr (r30, then r29 and r3 copied from it),
// the copies MWCC leaves when it inlines such returns; written as plain
// locals the zeroes are li / li / li ("different", 89.6 %).
// Carried (field_0xce8) on a layer other than 8.
static inline bool carriedOffLayer8(const Player_804A6F38* player) {
    bool r = false;
    if (player->field_0xce8 && player->field_0x3b4 != 8) {
        r = true;
    }
    return r;
}

// ... and that layer lacks view bit 1 (GameView fn_802D7804).
static inline bool carriedOnHiddenLayer(const Player_804A6F38* player) {
    bool r = false;
    if (carriedOffLayer8(player) && !getGameView_802D6A9C()->fn_802D7804(player->field_0x3b4, 1)) {
        r = true;
    }
    return r;
}

// Layer 8, or a layer with view bit 1.
static inline bool layerShown(s32 layer) {
    bool r = true;
    if (layer != 8 && !getGameView_802D6A9C()->fn_802D7804(layer, 1)) {
        r = false;
    }
    return r;
}

// Carried on a hidden layer while the previous layer (field_0xce4) is shown.
static inline bool hiddenFromShownLayer(const Player_804A6F38* player) {
    bool r = false;
    if (carriedOnHiddenLayer(player) && layerShown(player->field_0xce4)) {
        r = true;
    }
    return r;
}

// The gate in front of every pad query of the player (fn_80246904 and the
// functions around it): the player flags (fn_802413F0); in group mode 4
// only the player whose turn it is (fn_80240888 == its place in line); and
// field_0x11ea.
static inline bool teamCheck(Player_804A6F38* player) {
    if (!playerFlagsGate(player)) {
        return false;
    }
    if ((player->fn_802486A0() == 4 && player->fn_80240888() != player->fn_80248778()) || !player->field_0x11ea) {
        return false;
    }
    return true;
}

// Whether pos is inside the view box of `layer` (lower edges included).
static inline bool inViewBox(const JGeometry::TVec2f& pos, s32 layer) {
    const JGeometry::TBox2f* box = layerBox(layer);
    return pos.x >= box->i.x && pos.y >= box->i.y && pos.x < box->f.x && pos.y < box->f.y;
}

// The length of v in the x/y plane, the squares unfused (see GameMath.h);
// callers pass an operator- temporary by reference, as magnitude().
static inline f32 planarDistance(const JGeometry::TVec3f& v) {
    return JGeometry::TUtil<f32>::sqrt(square(v.x) + square(v.y));
}

// The per-player cue of the update's join sequence: at frame `at` of the
// counter field_0xd38 the 'here' effect, pad rumble and sound 0x25; at
// `off` fn_8043A170. Written once per player in the original (the four
// copies differ only in the frames), so a file-local inline.
static inline void joinCue(Player_804A6F38* player, s32 at, s32 off) {
    if (player->field_0xd38 == at) {
        gGameRoot2_8053AB10->field_0x30->fn_8015F8FC(player->field_0x3b4, &player->mPos, 1);
        if (lbl_8053AD30->fn_8030A308(getPlayerInputSlot_8030AAF4(player->field_0x11ec))) {
            fn_8030381C(player->field_0x4);
        }
        if (player->field_0x3b4 >= 8) {
            fn_8030381C(player->field_0x4);
        }
        // The original converts the index to s8 (extsb) for the sound call.
        gAudioMgr_8053AF80->fn_80458880(0x25, (s8)player->field_0x4);
    } else if (player->field_0xd38 == off) {
        fn_8043A170(player->field_0x4, false);
    }
}

Player_804A6F38::Player_804A6F38(s32 index) {
    field_0x4 = index;
    field_0x1258.set(0.0f, 0.0f, 0.0f);
    field_0x1250 = new cls_804B20F8(index, &field_0x1258, NULL, 8);
    field_0x1254 = new cls_804B20F8(index, NULL, NULL, 8);
    field_0xbec = new cls_804AA8D8();
    field_0xbf0 = new cls_804A71A8(this);
    mActive = false;
}

Player_804A6F38::~Player_804A6F38() {
    fn_80241E64();
    delete field_0xbf0;
    delete field_0xbec;
    delete field_0x1254;
    delete field_0x1250;
}

// FIXME: 98.34. The original keeps the view's stage in a saved register and tests
// isStageFlagSet's result as an int (cmpwi), ours reloads it and tests the bool
// (clrlwi.). Besides the unit's float literals (@NNN vs lbl_, not yet in
// pool order), only scheduling and register numbers differ: at two of the
// seven isPlayerBitSet sites (the 2-player and the else branch) the original
// loads field_0x4 before gGameRoot_8053AAF8 (the registers after them shift by one),
// and the first 0x31-entry loop's setup makes the index 0 first and copies it
// for the byte 0 (li r5,0 / mr r3,r5), ours loads li r6,0 / li r3,0 after
// li r4,1.
// The per-slot reset, run for every slot on scene enter and stage change
// (fn_8023F78C). Picks the slot's part (field_0xcac, the partner field_0xd60,
// the pad owner field_0x11ec and the start state 0 or 1) from the game mode,
// places the player and clears nearly every per-player field.
void Player_804A6F38::fn_80254744() {
    if (field_0xd00 >= 2 && isStageFlagSet(getGameView_802D6A9C()->field_0x4, getGameView_802D6A9C()->field_0x8)) {
        field_0xd00 = 0;
        field_0xcfe = false;
    }
    if (!gGameRoot_8053AAF8->field_0x7f && !isPlayerBitSet(gGameRoot_8053AAF8, field_0x4)) {
        mActive = false;
        field_0xd60 = -1;
        field_0x11ec = -1;
        field_0x1244 = 1;
        field_0x1240 = 1;
        field_0x1248 = 1;
    } else if (gGameRoot_8053AAF8->field_0x7f && fn_802DE814() &&
               !isPlayerBitSet(gGameRoot_8053AAF8, field_0x4)) {
        mActive = false;
        field_0xd60 = -1;
        field_0x11ec = -1;
        field_0x1244 = 1;
        field_0x1240 = 1;
        field_0x1248 = 1;
    } else if (fn_802DEBF8() || !gGameRoot_8053AAF8->field_0x7e) {
        mActive = true;
        field_0x11ec = 0;
        if (field_0x4 == 0) {
            field_0xd60 = -1;
        } else {
            field_0xd60 = 0;
        }
        field_0x1244 = 0;
        field_0x1240 = 0;
        field_0x1248 = 0;
    } else if (gGameRoot_8053AAF8->fn_80138A30() == 2) {
        mActive = true;
        if (isPlayerBitSet(gGameRoot_8053AAF8, field_0x4)) {
            field_0xd60 = -1;
            field_0x11ec = field_0x4;
        } else {
            field_0xd60 = field_0x4 - 2;
            field_0x11ec = field_0xd60;
        }
        field_0x1244 = 0;
        field_0x1240 = 0;
        field_0x1248 = 0;
    } else if (gGameRoot_8053AAF8->fn_80138A30() == 3) {
        mActive = true;
        if (isPlayerBitSet(gGameRoot_8053AAF8, field_0x4)) {
            field_0xd60 = -1;
            field_0x11ec = field_0x4;
        } else {
            s32 partner = lbl_80529DEC.field_0x64;
            if (partner < 0 || partner >= 3) {
                field_0xd60 = 0;
            } else {
                field_0xd60 = partner;
            }
            field_0x11ec = field_0xd60;
        }
        field_0x1244 = 0;
        field_0x1240 = 0;
        field_0x1248 = 0;
    } else {
        mActive = true;
        field_0xd60 = -1;
        if (isPlayerBitSet(gGameRoot_8053AAF8, field_0x4)) {
            field_0x11ec = field_0x4;
            field_0x1244 = 0;
            field_0x1240 = 0;
            field_0x1248 = 0;
        } else {
            field_0x11ec = -1;
            field_0x1244 = 1;
            field_0x1240 = 1;
            field_0x1248 = 1;
        }
    }

    // A partner starts next to the player it follows, 16 units per place in the line.
    if (field_0xd60 >= 0 && !field_0xcfe) {
        field_0x2c4 = *getPlayerPos_8023E724(field_0xd60);
        if (field_0xd04) {
            field_0x2c4.y += 16.0f * fn_80248778();
        } else {
            field_0x2c4.x += 16.0f * fn_80248778();
        }
    }
    field_0x11ea = 1;
    mPos = field_0x2c4;
    field_0x2f4 = mPos;
    field_0xcbc = field_0x2f4;
    field_0xcb0 = field_0xcbc;
    field_0xcd4 = field_0xcb0;
    field_0xcc8 = field_0xcd4;
    field_0x32c = field_0xcc8;
    field_0xd1c = field_0x32c;
    if (field_0xd00 == 2) {
        mPos.z = 1.0f;
        field_0x32c.z = 1.0f;
    }
    field_0x318 = mPos;
    field_0x14 = mPos;
    field_0x20 = field_0x14;
    field_0x2c = field_0x20;
    field_0x3c0 = false;
    field_0x3c1 = false;
    field_0x3c2 = false;
    if (field_0xcfe == true) {
        field_0x120c = 4;
        field_0x1210 = 4;
    } else {
        field_0x120c = 5;
        field_0x1210 = 5;
    }
    field_0x121c = 0;
    field_0x121d = 0;
    field_0x121f = false;
    field_0x1220 = 0;
    field_0x1258 = mPos;
    field_0x434 = 0;
    field_0x435 = 0;
    field_0x2d0.zero();
    field_0x2dc.zero();
    field_0x2e8.zero();
    field_0x3b4 = 0;
    field_0x3b8 = 0;
    field_0xce4 = 0;
    field_0xce0 = 0;
    field_0x3bc = field_0x3b4;
    field_0xd28 = field_0x3b4;
    field_0x455 = 0;
    field_0x3f0 = field_0x3e4 = field_0x3dc = field_0x3e8 = field_0x3ec = field_0x3e0;
    field_0x124d = 0;
    field_0x124c = false;
    field_0x458 = 0;
    field_0x459 = 0;
    field_0x3f4 = 0;
    field_0x3f9 = 0;
    field_0x3f8 = 0;
    field_0x3fb = 0;
    field_0x3fa = 0;
    field_0x3fd = 0;
    field_0x3fc = 0;
    field_0x3fe = 0;
    field_0x3b0 = 0;
    field_0x437 = field_0x3f8;
    field_0x438 = field_0x3f9;
    field_0x439 = field_0x3fa;
    field_0x43a = field_0x3fb;
    field_0x43b = field_0x3fc;
    field_0x43c = field_0x3fd;
    field_0x3ff = 0;
    field_0x400 = 0;
    field_0x401 = 0;
    field_0x402 = 0;
    field_0x403 = 0;
    field_0x404 = 0;
    field_0x40c = 0;
    field_0x408 = 0;
    field_0x410 = 0.0f;
    if (field_0x1240 == 1) {
        field_0x3c4 = 0x31d;
    } else {
        switch (field_0x3dc) {
        case 0:
            field_0x3c4 = 0x3c;
            break;
        case 0x1c1:
            field_0x3c4 = 0x3d;
            break;
        case 1: // compared in the original's search (cmpwi 1), sharing default's code
        default:
            field_0x3c4 = 0x3e;
            break;
        case 3:
            field_0x3c4 = 0x3f;
            break;
        }
    }
    field_0x3c6 = field_0x3c4;
    field_0x458 = 0;
    field_0x45c = 0;
    field_0x43d = 0;
    field_0x3d8 = 0;
    field_0x3d9 = 0;
    field_0xca3 = -0x80;
    field_0xca4 = -0x80;
    fn_80276538();
    fn_80276078();
    field_0xbf4 = 0;
    field_0x464 = 0;
    field_0x468 = 0;
    field_0x46c = 0;
    field_0x470 = 0;
    field_0x474.set(0.0f, 0.0f, 0.0f);
    field_0x484 = 0;
    field_0x480 = 0;
    field_0x488 = 0;
    field_0x48c = 0;
    field_0x490 = 0;
    field_0x491 = 0;
    field_0x457 = 0;
    field_0xcad = false;
    field_0xcae = 0;
    field_0xcaf = 0;
    field_0xd64 = 0;
    field_0xd6c = 'NULL';
    field_0xbf0->fn_8026F710();
    field_0xce8 = false;
    field_0xce9 = 1;
    field_0xcf4 = 0x29;
    field_0xcf8 = 0x29;
    field_0xcea = 0;
    field_0xceb = 0;
    field_0xcec = 0;
    field_0xcf2 = 0;
    field_0xced = 0;
    field_0xcf1 = 0;
    field_0xcf0 = 0;
    field_0x344.set(0.0f, 0.0f, 0.0f);
    field_0x350.set(0.0f, 0.0f, 0.0f);
    field_0x35c.set(0.0f, 0.0f, 0.0f);
    field_0xd78.set(0.0f, 0.0f, 0.0f);
    field_0x414.set(0.0f, 0.0f, 0.0f);
    field_0x420.set(0.0f, 0.0f, 0.0f);
    field_0x368.set(0.0f, 0.0f, 0.0f);
    field_0x38c = 0.0f;
    field_0x390 = 0.0f;
    field_0x374.set(0.0f, 0.0f, 0.0f);
    field_0x2b8.set(0.0f, 0.0f, 0.0f);
    if (gGameRoot_8053AAF8->field_0x7f) {
        field_0xc80 = 1;
    } else {
        field_0xc80 = 0;
    }
    field_0xc84.set(1.0f, 1.0f, 1.0f);
    field_0xc90.set(1.0f, 1.0f, 1.0f);
    field_0xc9c = 0;
    fn_8027221C(0);
    field_0xcfc = 0;
    field_0xcfd = 0;
    field_0xd08 = -1;
    field_0xd0c = 0;
    field_0xd2c = 0;
    field_0xd2d = 0;
    field_0xd30 = 0;
    field_0x494 = 0;
    field_0x498 = 0;
    field_0x499 = 0;
    field_0x49c = -1;
    field_0xd34 = false;
    field_0xc00 = 0;
    field_0xd35 = 0;
    field_0xd70 = -1;
    field_0xd68 = -1;
    field_0xd88 = -1;
    field_0xd8c = -1;
    field_0xd94 = -1;
    field_0xda0 = -1;
    field_0xd98 = 0;
    field_0xd9c = -1;
    field_0xda4 = -1;
    field_0xda8 = 0;
    field_0xdac = -1;
    for (int i = 0; i < 0x50; i++) {
        field_0xdb0[i] = -1;
        field_0xef0[i] = -1;
        field_0x1030[i] = -1;
    }
    field_0x1170 = 0;
    for (int i = 0; i < 3; i++) {
        field_0xb00[i] = -1;
        field_0xb40[i] = -1;
    }
    field_0xb7c = -1;
    field_0xb74 = 0;
    for (int i = 0; i < 3; i++) {
        field_0xb0c[i] = -1;
        field_0xb4c[i] = -1;
    }
    field_0xb80 = -1;
    for (int i = 0; i < 2; i++) {
        field_0xb30[i] = -1;
    }
    field_0xb18 = -1;
    for (int i = 0; i < 3; i++) {
        field_0xb1c[i] = -1;
        field_0xb5c[i] = -1;
    }
    field_0xb78 = false;
    field_0xb28 = -1;
    field_0xb2c = -1;
    field_0xbb8 = 0;
    field_0xb38 = -1;
    field_0xb6c = -1;
    field_0xb68 = 0;
    field_0xb3c = -1;
    field_0xb70 = -1;
    field_0xb58 = -1;
    field_0xb99 = false;
    field_0xb9e = false;
    field_0xb9f = false;
    field_0xba0 = 0;
    field_0xba4 = 0;
    field_0xb9a = false;
    field_0xb9b = false;
    field_0xb9c = false;
    field_0xbb4 = -1;
    field_0xbb9 = 0;
    field_0xbba = 0;
    field_0xbbc = 3.0f;
    field_0xbd4 = 0;
    field_0xbc0 = 0;
    field_0xbac = 0;
    field_0xb79 = 0;
    field_0x1204 = 0;
    field_0xca0 = 0xff;
    field_0xca1 = 0xff;
    field_0xca2 = 0xff;
    field_0xca8 = 0;
    field_0x1264 = 0;
    field_0x11dc = 0;
    field_0x1274 = NULL;
    field_0x128c = NULL;
    field_0x12b0 = NULL;
    field_0x12b4 = NULL;
    field_0x12b8 = NULL;
    field_0x12bc = NULL;
    field_0x12c0 = -1;
    field_0x12c4 = NULL;
    field_0x12c8 = NULL;
    field_0x12cc = NULL;
    field_0x12e4 = NULL;
    field_0x1308 = NULL;
    field_0x132c = NULL;
    field_0x1350 = NULL;
    field_0x1354 = NULL;
    field_0x1358 = NULL;
    field_0x136c = NULL;
    field_0x1370 = NULL;
    field_0x135c = NULL;
    field_0x1374 = NULL;
    field_0x1378 = NULL;
    field_0x1368 = NULL;
    field_0x1360 = NULL;
    field_0x1364 = NULL;
    field_0x137c = NULL;
    field_0x1380 = NULL;
    field_0x1384 = NULL;
    field_0x1388 = NULL;
    field_0x138c = NULL;
    field_0x1390 = NULL;
    field_0x1394 = NULL;
    field_0x1398.field_0x4 = field_0x4;
    field_0x1398.field_0x8 = 0;
    field_0x13a8.field_0x4 = field_0x4;
    field_0x13a8.field_0x8 = 1;
    field_0xd74 = 0;
    field_0x11e0 = 0;
    field_0x11b0 = 0;
    fn_8027023C();
    field_0x11e9 = 0;
    field_0x1174 = 0;
    field_0x454 = false;
    field_0x456 = 0;
    field_0x452 = 0;
    field_0xd38 = 0;
    field_0xd3c = -1;
    field_0xd44 = -1;
    field_0xd4c = false;
    field_0xd4d = 0;
    field_0xd4e = false;
    field_0xd50 = 0;
    field_0x11f4 = 0;
    field_0xd54 = 0;
    field_0xd58 = 0;
    field_0x11f8 = -1;
    field_0x1230 = -1;
    field_0x1234 = 0;
    field_0xafc = 0;
    field_0xb9d = false;
    field_0xd5c = 0;
    field_0x11fc = -1;
    field_0x460 = false;
    field_0xb84 = false;
    field_0x11b4 = mPos;
    field_0x11c0 = 0;
    field_0xcee = 0;
    field_0xcef = 0;
    field_0x42c = false;
    field_0xbe0 = 0;
    field_0x430 = 0;
    field_0x42d = 0;
    field_0x43e = 0;
    field_0x43f = false;
    field_0x440 = false;
    field_0x441 = false;
    field_0x444 = 0.0f;
    field_0xbd8 = 0;
    field_0xbdc = 0;
    field_0xbe4 = 0;
    field_0xbe8 = 0;
    field_0x1214 = false;
    field_0x1216 = false;
    field_0x1215 = false;
    field_0x1217 = false;
    field_0x1218 = 0;
    field_0x1224 = 0;
    field_0x123c = 0;
    field_0x1228 = 0;
    field_0x122c = 0;
    field_0x1238 = 0;
    field_0x11ac = 0;
    field_0x492 = 0;
    field_0x1200 = false;
    field_0x1201 = 0;
    field_0x1202 = false;
    field_0x11c4 = 0;
    field_0x11c8 = 0;
    field_0x11d4 = 0;
    field_0x11d8 = 0;
    field_0x11cc = 0;
    field_0x3c8 = 0x195;
    field_0x3ca = 0x18f;
    field_0x3cc = 0;
    field_0x3d0 = 0;
    field_0x3d4 = 0;
    field_0xd90 = 0;
    field_0x394 = 0;
    field_0x1208 = 0;
    field_0xd48 = 0;
    field_0xba8 = 0;
    field_0x436 = false;
    field_0x1203 = 0;
    field_0xb94 = 0;
    field_0x448 = 3;
    field_0x44c = 0;
    field_0x13b8 = 0;
    field_0xd84 = 0;
    field_0x11f0 = 0;
    field_0x11e8 = 0;
    if (fn_802DEBF8()) {
        field_0x121e = 1;
        field_0x1214 = true;
        if (field_0xd60 >= 0) {
            field_0x1215 = true;
            mPos.set(*getPlayerPos_8023E724(field_0xd60));
            fn_80251AEC(3);
        }
        field_0x1224 = 0;
        field_0x1217 = false;
    } else {
        field_0x121e = 0;
    }
    fn_80248604();
    for (int i = 0; i < 0x31; i++) {
        // An unused copy: the original stores mPos on the stack each pass (without
        // it we are 4 instructions short).
        JGeometry::TVec3f pos = mPos;
        if (field_0xd04) {
            field_0x38[i].set(mPos.x, mPos.y + 16.0f * i / 12.0f, 0.0f);
        } else {
            field_0x38[i].set(mPos.x + 16.0f * i / 12.0f, mPos.y, 0.0f);
        }
        field_0x284[i] = 0;
    }
    if (gGameRoot_8053AAF8->field_0x7f && fn_80241794() <= 0 && !fn_80241200()) {
        if (field_0x3dc == 0) {
            field_0x338.set(16.0f + mPos.x, mPos.y, mPos.z);
        } else if (field_0x3dc == 1) {
            field_0x338.set(mPos.x - 16.0f, mPos.y, mPos.z);
        } else if (field_0x3dc == 2) {
            field_0x338.set(mPos.x, 16.0f + mPos.y, mPos.z);
        } else {
            field_0x338.set(mPos.x, mPos.y - 16.0f, mPos.z);
        }
    } else {
        field_0x338.x = mPos.x;
        field_0x338.y = mPos.y;
        field_0x338.z = mPos.z;
    }
    field_0xbec->fn_8030BD3C(field_0x4);
    if (field_0xcfe == true) {
        if (!isStageFlagSet(getGameView_802D6A9C()->field_0x4, getGameView_802D6A9C()->field_0x8)) {
            fn_8024DCD0();
            field_0x32c = mPos;
            field_0x450 = 0xc3;
            field_0xca2 = 0xff;
            field_0xca3 = -0x80;
            field_0xca4 = -0x80;
            // Both operands load mPos (0x8) in the original (mPos - field_0x32c reloads 0x32c: 98.0 %).
            const JGeometry::TVec3f& diff = mPos - mPos;
            if (PSVECMag(&diff) >= 0.1f) {
                fn_8024B2E0();
            }
            lbl_8053AD70->fn_8037AE00();
        }
    } else if (field_0xd00 == 2) {
        fn_8024DCD0();
        field_0x32c = mPos;
        field_0x450 = 0x121;
        field_0xca2 = 0xff;
        field_0xca3 = -0x80;
        field_0xca4 = -0x80;
        // As above: both operands load mPos (mPos - field_0x32c reloads 0x32c).
        const JGeometry::TVec3f& diff = mPos - mPos;
        if (PSVECMag(&diff) >= 0.1f) {
            fn_8024B2E0();
        }
        field_0xcfe = false;
        lbl_8053AD70->fn_8037AE00();
    }
}

// FIXME: 99.91. Code identical; the float literal lbl_80540098 (256/3) is @NNN until the
// unit's earlier literals are written in pool order.
void Player_804A6F38::fn_80254660() {
    field_0xc10 = getGameView_802D6A9C()->field_0x48;
    field_0xc14 = getGameView_802D6A9C()->field_0x4c;
    field_0xc18 = 1;
    field_0xb7a = false;
    field_0xb7b = false;
    if (gGameRoot_8053AAF8->field_0x7f) {
        if (field_0xd60 >= 0) {
            field_0xbf8 = 0x10;
        } else {
            field_0xbf8 = fn_8040305C(fn_804033D4(field_0x4)) * 4;
        }
    } else {
        field_0xbf8 = fn_8040305C(fn_804033D4(field_0x4)) * 4;
    }
    if (gGameRoot_8053AAF8->field_0x7f) {
        field_0xb90 = 0.0f;
    } else {
        field_0xb90 = 256.0f / 3.0f;
    }
    field_0xc08 = 0x180;
    field_0xc0c = 0x180;
    field_0xbfc = field_0xbf8 - 4;
}

// FIXME: 99.95. Code identical; lbl_80540098 (256/3) is @NNN, as in fn_80254660.
// Like fn_80254660, then clears the counters 0xC04-0xC7C.
void Player_804A6F38::fn_802544B0() {
    field_0xc10 = getGameView_802D6A9C()->field_0x48;
    field_0xc14 = getGameView_802D6A9C()->field_0x4c;
    field_0xc18 = 1;
    field_0xb7a = false;
    field_0xb7b = false;
    if (gGameRoot_8053AAF8->field_0x7f) {
        if (field_0xd60 >= 0) {
            field_0xbf8 = 0x10;
        } else {
            field_0xbf8 = fn_8040305C(fn_804033D4(field_0x4)) * 4;
        }
    } else {
        field_0xbf8 = fn_8040305C(fn_804033D4(field_0x4)) * 4;
    }
    if (gGameRoot_8053AAF8->field_0x7f) {
        field_0xb90 = 0.0f;
    } else {
        field_0xb90 = 256.0f / 3.0f;
    }
    field_0xc08 = 0x180;
    field_0xc0c = 0x180;
    field_0xbfc = field_0xbf8 - 4;
    if (gGameRoot_8053AAF8->field_0x7f) {
        if (field_0xd60 >= 0) {
            field_0xc04 = 0;
        } else {
            field_0xc04 = 0;
        }
    } else {
        field_0xc04 = 0;
    }
    for (int i = 0; i < 13; i++) {
        field_0xc1c[i] = 0;
    }
    field_0xc50 = 0;
    field_0xc54 = 0;
    field_0xc58 = 0;
    field_0xc5c = 0;
    field_0xc60 = 0;
    field_0xc64 = 0;
    field_0xc68 = 0;
    field_0xc6c = 0;
    field_0xc70 = 0;
    field_0xc74 = 0;
    field_0xc78 = 0;
    field_0xc7c = 0;
}

// FIXME: 99.99. Code identical; the float literal lbl_80540094 (18.0f) is @NNN until the
// unit's earlier literals are written in pool order.
// Update A: the player's per-frame update, called for every slot by the
// manager update fn_8023F390.
void Player_804A6F38::fn_80253E6C() {
    if (!mActive) {
        return;
    }
    if (field_0x1234 == 1 && lbl_8053AD30->fn_80309D90(getPlayerInputSlot_8030AAF4(field_0x11ec)) > 0.0f) {
        field_0x1234 = 0;
    }
    if (field_0x11f0 > 0) {
        field_0x11f0--;
    }
    if (field_0x13b8) {
        gGameRoot2_8053AB10->field_0x20->field_0x216 = 1;
    }
    if (field_0x12c4 != NULL) {
        if (gGameRoot_8053AAF8->field_0x94 == 1 || field_0x121f || field_0x454) {
            field_0x12c4->setStatus(JPAEMIT_StopDraw);
            field_0x12c4->setStatus(JPAEMIT_StopCalc);
        } else {
            field_0x12c4->clearStatus(JPAEMIT_StopDraw);
            field_0x12c4->clearStatus(JPAEMIT_StopCalc);
        }
    }
    if (field_0x12c8 != NULL) {
        if (gGameRoot_8053AAF8->field_0x94 == 1 || field_0x121f || field_0x454) {
            field_0x12c8->setStatus(JPAEMIT_StopDraw);
            field_0x12c8->setStatus(JPAEMIT_StopCalc);
        } else {
            field_0x12c8->clearStatus(JPAEMIT_StopDraw);
            field_0x12c8->clearStatus(JPAEMIT_StopCalc);
        }
    }
    if (gGameRoot_8053AAF8->field_0x94 == 1 || field_0x121f) {
        fn_80270D08();
        fn_80286688();
        fn_80265E88();
        for (int i = 0; i < 0x50; i++) {
            field_0xdb0[i] = -1;
            field_0x1030[i] = -1;
        }
        return;
    }
    field_0x2f4.set(0.0f, 0.0f, 18.0f);
    field_0x2f4 += mPos;
    field_0xc0c = field_0xc08;
    cls_804A71A8* sub = field_0xbf0;
    sub->field_0x14.x = 0.0f;
    sub->field_0x14.y = 0.0f;
    if (!gGameRoot_8053AAF8->field_0x80) {
        fn_802490A8();
    }
    fn_8026FD14();
    fn_80251D50();
    if (gGameRoot_8053AAF8->field_0x80) {
        field_0xd38++;
        field_0x1208++;
        if (field_0xd38 == 0x10) {
            spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x418, 0, 1, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x419, 0, 1, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x41a, 0, 1, NULL, NULL, true, 1.0f);
            fn_8024F0E8(0x4004e);
        } else if (field_0xd38 == 0x83) {
            field_0xc80 = 1;
            fn_8024F0E8(0x20043);
        }
        if (isPlayerBitSet(gGameRoot_8053AAF8, field_0x4)) {
            switch (field_0x4) {
            case 0:
                joinCue(this, 0xc0, 0xe0);
                break;
            case 1:
                joinCue(this, 0xe0, 0x100);
                break;
            case 2:
                joinCue(this, 0xf8, 0x118);
                break;
            case 3:
                joinCue(this, 0x108, 0x128);
                break;
            }
        }
    } else {
        fn_80285E58();
    }
    fn_8024BDBC();
    fn_80276538();
    fn_80276078();
    field_0xbec->fn_8030B774();
    fn_8024E0A0();
    if (field_0xd48 > 0) {
        field_0xd48--;
    }
}

// FIXME: 99.73 %. Register allocation only: the group loops' count / index
// (r29 / r30 vs r28 / r29), plus three literal names (@NNN until the unit's
// literals are in pool order).
// Update B (fn_8023F390, after every player's update A): a follower walks
// to its leader's spot (fn_8027FA08), 2 a frame while 0x1218 runs, else 1.5
// or 6 by the helper's flags, and arrives on it; a player held by another
// (0xD9C / 0xDAC) moves to 16 beside it; else a carried player keeps its
// position, and the rest are pushed by the players they touch.
void Player_804A6F38::fn_80252FC0() {
    if (field_0x121f) {
        return;
    }
    if (field_0xce8) {
        return;
    }
    if (field_0x454) {
        return;
    }
    if (fn_80270A8C()) {
        return;
    }
    if (fn_80241200()) {
        return;
    }
    if (fn_8025F448()) {
        return;
    }
    if (field_0x1240 == 0xe) {
        return;
    }
    if (fn_802486A0() == 4) {
        return;
    }
    if ((field_0xd60 >= 0 && field_0xd9c < 0 && field_0xdac < 0) || field_0x1216) {
        if (fn_80240B84()) {
            return;
        }
        if (field_0x1240 == 0x19) {
            return;
        }
        if (field_0x122c > 0) {
            return;
        }
        JGeometry::TVec3f target;
        fn_8027FA08(&target);
        JGeometry::TVec3f step = target - mPos;
        if (!field_0x1216) {
            step.z = 0.0f;
        }
        f32 dist = PSVECMag(&step);
        f32 speed;
        if (field_0x1218 > 0) {
            speed = 2.0f;
        } else if (field_0xbf0->anyFlag48()) {
            speed = 1.5f;
        } else {
            speed = 6.0f;
        }
        if (dist >= speed) {
            step.normalize();
            step.scale(speed);
            mPos += step;
        } else {
            mPos.x = target.x;
            mPos.y = target.y;
            mPos.z = target.z;
            field_0x1216 = false;
            if (!field_0x1214 && field_0x1218 == 0) {
                field_0x1217 = true;
            }
            if (field_0xd60 >= 0 && field_0x1214 == true) {
                field_0x1215 = true;
            }
            field_0xbec->fn_8030BD3C(field_0x4);
        }
        return;
    }
    s32 holder = field_0xd9c;
    if (holder >= 0 || field_0xdac >= 0) {
        JGeometry::TVec3f pos;
        if (holder >= 0) {
            pos.set(*getPlayerPos_8023E724(holder));
        } else {
            pos.set(*getPlayerPos_8023E724(field_0xdac));
        }
        switch (field_0x3dc) {
        case 0:
            pos.x -= 16.0f;
            break;
        case 1:
            pos.x += 16.0f;
            break;
        case 2:
            pos.y -= 16.0f;
            break;
        case 3:
            pos.y += 16.0f;
            break;
        }
        JGeometry::TVec3f step = pos - mPos;
        step.z = 0.0f;
        f32 dist = PSVECMag(&step);
        if (field_0xbf0->anyFlag48()) {
            step.normalize();
            step.scale(1.5f);
            mPos.x += step.x;
            mPos.y += step.y;
        } else if (dist >= 3.0f) {
            step.normalize();
            step.scale(3.0f);
            mPos.x += step.x;
            mPos.y += step.y;
        } else {
            mPos.x = pos.x;
            mPos.y = pos.y;
        }
        return;
    }
    for (s32 i = 0; i < 4; i++) {
        if (i != field_0x4 && fn_8022E438(i, field_0x4) && fn_8023EBB8(i)) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(fn_8023EF50(i));
            if (actor != NULL && actor->field_0x1a4 != 0) {
                mPos.x = field_0x14.x;
                mPos.y = field_0x14.y;
                mPos.z = field_0x14.z;
                return;
            }
        }
    }
    if (fn_802486A0() == 5) {
        return;
    }
    if (fn_80250D30()) {
        return;
    }
    if (viewLayerFlag(field_0x3b4)) {
        return;
    }
    if (!fn_802341D0(field_0x4)) {
        return;
    }
    f32 pushX = 0.0f;
    f32 pushY = pushX;
    for (s32 i = 0; i < 4; i++) {
        if (fn_8023F264(i) && fn_8022E438(i, field_0x4)) {
            if (fn_8022D930(i) || fn_8022D894(i) || fn_8022D9CC(i)) {
                JGeometry::TVec3f target;
                fn_8027F854(&target, i);
                JGeometry::TVec3f step = target - mPos;
                step.z = 0.0f;
                if (PSVECMag(&step) >= 5.0f) {
                    step.normalize();
                    step.scale(5.0f);
                    mPos.x += step.x;
                    mPos.y += step.y;
                } else {
                    mPos.x = target.x;
                    mPos.y = target.y;
                }
                return;
            }
            const JGeometry::TVec2f* push = fn_8022DFD4(i);
            f32 x = push->x;
            f32 y = push->y;
            if (pushX == 0.0f) {
                pushX = x;
            } else if (pushX > 0.0f) {
                if (x > pushX) {
                    pushX = x;
                }
            } else if (x < pushX) {
                pushX = x;
            }
            if (pushY == 0.0f) {
                pushY = y;
            } else if (pushY > 0.0f) {
                if (y > pushY) {
                    pushY = y;
                }
            } else if (y < pushY) {
                pushY = y;
            }
        }
    }
    if ((pushX > 0.0f && field_0xbf0->field_0x14.x >= 0.0f) || (pushX < 0.0f && field_0xbf0->field_0x14.x <= 0.0f)) {
        mPos.x += pushX - field_0xbf0->field_0x14.x;
    }
    if ((pushY > 0.0f && field_0xbf0->field_0x14.y >= 0.0f) || (pushY < 0.0f && field_0xbf0->field_0x14.y <= 0.0f)) {
        mPos.y += pushY - field_0xbf0->field_0x14.y;
    }
}

void Player_804A6F38::fn_80252FBC() {}

void Player_804A6F38::fn_80252F80(const JGeometry::TVec3f* pos, s32 a, s32 b, u8 c) {
    field_0x2c4 = *pos;
    field_0x3e0 = a;
    field_0xd00 = b;
    if (b == 1) {
        field_0xcfe = true;
    } else {
        field_0xcfe = false;
    }
    field_0xd04 = c;
}

s32 Player_804A6F38::fn_80252F74() {
    return field_0xbec->field_0x290;
}

s32 Player_804A6F38::fn_80252F68() {
    return field_0xbec->field_0x294;
}

void Player_804A6F38::fn_80252F5C(s32 arg) {
    field_0xbec->field_0x298 = arg;
}

void Player_804A6F38::fn_80252F50(s32 arg) {
    field_0xbec->field_0x29c = arg;
}

JGeometry::TVec3f* Player_804A6F38::fn_80252F48() {
    return &field_0x11b4;
}

void Player_804A6F38::fn_80252F40(s32 arg) {
    field_0xd28 = arg;
}

void Player_804A6F38::fn_80252F14(const JGeometry::TVec3f* offset) {
    field_0x2d0 += *offset;
    field_0x434 = 1;
}

// Moves field_0x2dc by `offset` and pushes the position onto the trail
// twice (fn_8024B9E8).
void Player_804A6F38::fn_80252CF0(const JGeometry::TVec3f* offset) {
    field_0x2dc += *offset;
    fn_8024B9E8();
    fn_8024B9E8();
}

// Moves field_0x2dc by `offset` and the whole trail (field_0x38) by the
// result.
void Player_804A6F38::fn_80252BD0(const JGeometry::TVec3f* offset) {
    field_0x2dc += *offset;
    field_0x435 = 1;
    for (s32 i = 0; i < 0x31; i++) {
        field_0x38[i] += field_0x2dc;
    }
}

void Player_804A6F38::fn_80252BAC(const JGeometry::TVec3f* offset) {
    field_0x2e8 += *offset;
}

// Whether the player may walk off the screen (state 0x1B): within 20 of an
// edge of the visible box, on a layer below 8, low enough (z <= 16 unless
// fn_8025F448), moving towards that edge (fn_8022C82C / fn_8022C76C, or in
// demo mode, or on a tile fn_80225EF4 lists), with the tile 8 beyond the edge
// free (not collision code 1, nor 0x22 while GameView field_0x170 is 1).
// Writes the direction (-1 / 1 in x or y) to *out, else 0, 0.
// FIXME: 99.90 %. Code identical; two literals are @NNN, not
// lbl_80540070 / lbl_80540078, until the unit's earlier literals are in pool order.
bool Player_804A6F38::fn_802527B0(JGeometry::TVec2<s32>* out) {
    out->x = 0;
    out->y = 0;
    if (!fn_80251BFC(0x1b)) {
        return false;
    }
    if (field_0x1240 == 0x1b) {
        return false;
    }
    if (field_0x454) {
        return false;
    }
    if (field_0x3b4 >= 8) {
        return false;
    }
    if (field_0x1216 == true) {
        return false;
    }
    JGeometry::TVec3f pos = mPos;
    if (!fn_8025F448() && pos.z > 16.0f) {
        return false;
    }
    JGeometry::TBox2f box;
    box = getGameView_802D6A9C()->mVisibleBox;
    if (pos.x <= box.i.x + 20.0f &&
        (gGameRoot_8053AAF8->field_0x7e || fn_8022C82C(field_0x4) < 0.0f || fn_80225EF4(&pos, 0))) {
        JGeometry::TVec2f probe;
        probe.x = box.i.x + 8.0f;
        probe.y = pos.y;
        u16 tile = fn_802265FC((const JGeometry::TVec3f*)&probe, field_0x3b4);
        if (tile == 1 || (tile == 0x22 && getGameView_802D6A9C()->field_0x170 == 1)) {
            return false;
        }
        out->x = -1;
        out->y = 0;
        return true;
    }
    if (pos.x >= box.f.x - 20.0f &&
        (gGameRoot_8053AAF8->field_0x7e || fn_8022C82C(field_0x4) > 0.0f || fn_80225EF4(&pos, 0))) {
        JGeometry::TVec2f probe;
        probe.x = box.f.x - 8.0f;
        probe.y = pos.y;
        u16 tile = fn_802265FC((const JGeometry::TVec3f*)&probe, field_0x3b4);
        if (tile == 1 || (tile == 0x22 && getGameView_802D6A9C()->field_0x170 == 1)) {
            return false;
        }
        out->x = 1;
        out->y = 0;
        return true;
    }
    if (pos.y <= box.i.y + 20.0f &&
        (gGameRoot_8053AAF8->field_0x7e || fn_8022C76C(field_0x4) > 0.0f || fn_80225EF4(&pos, 0))) {
        JGeometry::TVec2f probe;
        probe.x = pos.x;
        probe.y = box.i.y + 8.0f;
        u16 tile = fn_802265FC((const JGeometry::TVec3f*)&probe, field_0x3b4);
        if (tile == 1 || (tile == 0x22 && getGameView_802D6A9C()->field_0x170 == 1)) {
            return false;
        }
        out->x = 0;
        out->y = -1;
        return true;
    }
    if (pos.y >= box.f.y - 20.0f &&
        (gGameRoot_8053AAF8->field_0x7e || fn_8022C76C(field_0x4) < 0.0f || fn_80225EF4(&pos, 0))) {
        JGeometry::TVec2f probe;
        probe.x = pos.x;
        probe.y = box.f.y - 8.0f;
        u16 tile = fn_802265FC((const JGeometry::TVec3f*)&probe, field_0x3b4);
        if (tile == 1 || (tile == 0x22 && getGameView_802D6A9C()->field_0x170 == 1)) {
            return false;
        }
        out->x = 0;
        out->y = 1;
        return true;
    }
    return false;
}

// Keeps a player below layer 8 16 units inside the visible box (one edge
// per frame: left, right, top, bottom), unless field_0x454, fn_8025F448,
// fn_80240B84 or field_0x3b0 holds.
// FIXME: 99.6 %. Registers only (the inlined group loop's leader / self
// temporaries), plus the 16.0f literal's name (@NNN until the unit's earlier
// literals are in pool order).
void Player_804A6F38::fn_8025250C() {
    if (field_0x454) {
        return;
    }
    if (fn_8025F448()) {
        return;
    }
    if (fn_80240B84()) {
        return;
    }
    if (field_0x3b0 > 0) {
        return;
    }
    if (field_0x3b4 >= 8) {
        return;
    }
    JGeometry::TBox2f box;
    box = getGameView_802D6A9C()->mVisibleBox;
    if (mPos.x <= box.i.x + 16.0f) {
        mPos.x = box.i.x + 16.0f;
    } else if (mPos.x >= box.f.x - 16.0f) {
        mPos.x = box.f.x - 16.0f;
    } else if (mPos.y <= box.i.y + 16.0f) {
        mPos.y = box.i.y + 16.0f;
    } else if (mPos.y >= box.f.y - 16.0f) {
        mPos.y = box.f.y - 16.0f;
    }
}

// Starts walking off the screen (state 0x1B) in direction *dir (from
// fn_802527B0), unless fn_8025F448. A player taking part that does not act
// on its own (fn_80240670) first moves onto the acting player (position,
// layer, field_0x2c); then the group goes to mode 5 and the trail is reset.
// A player not taking part moves onto its leader. When the request is
// granted (fn_80251AEC), field_0x398 / field_0x3A4 take the position on the
// ground and field_0x3A4 the target 56 inside the opposite edge of the
// visible box. The player bit is a materialised bool in the original
// (neg / or / srwi before the branch), hence the local.
// FIXME: 97.09 %. Scheduling and registers only: the inlined group loop's
// leader / own index (two registers in the original, one here), and dir->x
// is loaded before the field_0x398 / 0x3A4 stores here, after them in the
// original; plus the 56.0f literal's name (lbl_80540088 sits after -1.0f,
// whose first user fn_8024F63C is unwritten).
void Player_804A6F38::fn_802520C0(const JGeometry::TVec2<s32>* dir) {
    if (fn_8025F448()) {
        return;
    }
    bool takesPart = isPlayerBitSet(gGameRoot_8053AAF8, field_0x4);
    if (takesPart) {
        if (fn_80240670()) {
            mPos.set(*getPlayerPos_8023E724(fn_8022EA0C(field_0x4)));
            field_0x3b4 = getPlayerLayer_8023DE58(fn_8022EA0C(field_0x4));
            field_0x2c.set(mPos);
        }
        field_0x1216 = false;
        fn_80248004(5, false);
        field_0x1216 = false;
        fn_80248604();
    } else {
        mPos.set(*getPlayerPos_8023E724(field_0xd60));
        field_0x3b4 = getPlayerLayer_8023DE58(field_0xd60);
        field_0x2c.set(mPos);
    }
    if (fn_80251AEC(0x1b)) {
        field_0x398.set(mPos);
        field_0x398.z = 0.0f;
        field_0x3a4.set(mPos);
        field_0x3a4.z = 0.0f;
        if (dir->x == 1 && dir->y == 0) {
            field_0x3a4.x = getGameView_802D6A9C()->mVisibleBox.i.x + 56.0f;
        } else if (dir->x == -1 && dir->y == 0) {
            field_0x3a4.x = getGameView_802D6A9C()->mVisibleBox.f.x - 56.0f;
        } else if (dir->x == 0 && dir->y == 1) {
            field_0x3a4.y = getGameView_802D6A9C()->mVisibleBox.i.y + 56.0f;
        } else if (dir->x == 0 && dir->y == -1) {
            field_0x3a4.y = getGameView_802D6A9C()->mVisibleBox.f.y - 56.0f;
        }
    }
}

// The end of the frame's movement. The trail (field_0x38 / field_0x284)
// takes the position when it moved 1 or more from the newest entry or an
// entry is airborne (z above 0; on a layer without viewLayerFlag, two
// airborne flags in a row or the last one), unless carried, field_0x454,
// field_0x11f4, field_0x435 or field_0x1216. Then field_0x20 keeps
// field_0x14; with field_0x457 the position and layer go back to
// field_0x14 / field_0x3b8, else the push field_0x2dc is applied and
// field_0x14 / field_0x3b8 take the result; then field_0x2d0 (not while
// carried) and field_0x2e8 are applied and cleared, field_0xda0 =
// field_0xd94, field_0x1170 counts down and field_0x3e4 keeps the facing.
// FIXME: 98.6 %. Registers only, in the unrolled trail shift (the index and
// address temporaries r4-r7 rotated).
void Player_804A6F38::fn_80251D50() {
    bool airborne = false;
    for (s32 i = 0; i < 0x31; i++) {
        if (viewLayerFlag(field_0x3b4)) {
            if (field_0x38[i].z > 0.0f) {
                airborne = true;
                break;
            }
            if (i == 0x30) {
                if (field_0x284[i] == true) {
                    airborne = true;
                    break;
                }
            } else if (field_0x284[i] == true && field_0x284[i + 1] == true) {
                airborne = true;
                break;
            }
        } else if (field_0x38[i].z > 0.0f) {
            airborne = true;
            break;
        }
    }
    if (magnitude(field_0x38[0] - mPos) >= 1.0f || airborne) {
        if (!field_0xce8 && !field_0x454 && field_0x11f4 == 0 && !field_0x435 && !field_0x1216) {
            for (s32 i = 0x30; i > 0; i--) {
                field_0x38[i] = field_0x38[i - 1];
                field_0x284[i] = field_0x284[i - 1];
            }
            field_0x38[0] = mPos;
            field_0x284[0] = field_0xbf0->field_0xb4;
        }
    }
    field_0x20 = field_0x14;
    if (field_0x457) {
        field_0x457 = 0;
        mPos = field_0x14;
        field_0x3b4 = field_0x3b8;
    } else {
        mPos += field_0x2dc;
        field_0x2dc.zero();
        field_0x14 = mPos;
        field_0x3b8 = field_0x3b4;
    }
    if (!field_0xce8) {
        mPos += field_0x2d0;
    }
    field_0x2d0.zero();
    mPos += field_0x2e8;
    field_0x2e8.zero();
    field_0xda0 = field_0xd94;
    if (field_0x1170 > 0) {
        field_0x1170--;
    }
    field_0x3e4 = field_0x3dc;
}

// Whether a request for `state` may be made now.
bool Player_804A6F38::fn_80251BFC(s32 state) {
    if (field_0x124d == true) {
        return false;
    }
    if (field_0x124c == true && field_0x1244 != 0 && !(field_0x1244 == 2 || field_0x1244 == 3)) {
        return false;
    }
    if (field_0x124c == true && field_0x1244 == 2 && (state == 0 || state == 3)) {
        return false;
    }
    s32 requested = field_0x1244;
    if (requested == 1 && state != 0) {
        return false;
    }
    if (field_0xcee == true) {
        return false;
    }
    if (field_0xd60 >= 0 && !field_0xce8 && field_0x1214 && state != 3) {
        return false;
    }
    if (field_0x1215 && state != 3) {
        return false;
    }
    if (field_0x1240 == state) {
        return false;
    }
    if (requested == state) {
        return false;
    }
    if (gGameRoot_8053AAF8->field_0x7e && fn_80236890()) {
        return false;
    }
    return true;
}

// Requests state `state` (field_0x1244) when fn_80251BFC allows it. A player
// not taking part (bit field_0x4 of gGameRoot_8053AAF8->mPlayerMask clear, see
// game/PlayerBit.h) with no partner asking for state 0 gets
// state 1 instead; with field_0x434 set, state 0 becomes 2.
bool Player_804A6F38::fn_80251AEC(s32 state) {
    if (!fn_80251BFC(state)) {
        return false;
    }
    if (!isPlayerBitSet(gGameRoot_8053AAF8, field_0x4) && field_0xd60 < 0 && state == 0) {
        field_0x1244 = 1;
    } else if (field_0x434 && state == 0) {
        if (field_0x1240 == 2) {
            return false;
        }
        field_0x1244 = 2;
    } else {
        field_0x1244 = state;
    }
    field_0x124c = true;
    if (state != 0 && state != 2 && state != 3) {
        field_0x124d = true;
    }
    if (field_0xd60 < 0 && field_0xcfe == true && field_0x1244 != 0) {
        field_0xcfe = false;
    }
    return true;
}

bool Player_804A6F38::fn_8025197C(bool all) {
    return field_0xbf0->anyFlag48(all);
}

u8 Player_804A6F38::fn_8025196C(s32 index) {
    return field_0xbf0->field_0x48[index];
}

bool Player_804A6F38::fn_8025190C(bool a) {
    cls_804A71A8* sub = field_0xbf0;
    if (sub->field_0x48[3] == 1) {
        return true;
    }
    if (a == true && sub->field_0x48[6] == 1) {
        return true;
    }
    if (a == true && sub->field_0x48[7] == 1) {
        return true;
    }
    return false;
}

bool Player_804A6F38::fn_802518AC(bool a) {
    cls_804A71A8* sub = field_0xbf0;
    if (sub->field_0x48[2] == 1) {
        return true;
    }
    if (a == true && sub->field_0x48[4] == 1) {
        return true;
    }
    if (a == true && sub->field_0x48[5] == 1) {
        return true;
    }
    return false;
}

bool Player_804A6F38::fn_802517F8(bool a) {
    cls_804A71A8* sub = field_0xbf0;
    if (sub->field_0x48[1] == 1) {
        return true;
    }
    if (sub->field_0x48[0] == 1) {
        return true;
    }
    if (a == true && sub->field_0x48[4] == 1) {
        return true;
    }
    if (a == true && sub->field_0x48[5] == 1) {
        return true;
    }
    if (a == true && sub->field_0x48[6] == 1) {
        return true;
    }
    if (a == true && sub->field_0x48[7] == 1) {
        return true;
    }
    return false;
}

u8 Player_804A6F38::fn_802517E4() {
    return field_0x1240 == 0x1a;
}

// Whether the player pushes (state 0x1A) with the stick along its facing
// (fn_80247330 / fn_8024712C), fn_80244630's test without the input gate.
// FIXME: 99.86 %. Registers only (the inlined group loops' leader / self
// temporaries).
bool Player_804A6F38::fn_80250FD4() {
    if (field_0x1240 == 0x1a &&
        ((field_0x3dc == 0 && fn_80247330() > 0.0f) ||
         (field_0x3dc == 1 && fn_80247330() < 0.0f) ||
         (field_0x3dc == 2 && fn_8024712C() < 0.0f) ||
         (field_0x3dc == 3 && fn_8024712C() > 0.0f))) {
        return true;
    }
    return false;
}

// Not auto-inlined: fn_80250030 and fn_80252FC0 call fn_80250D30 out of line
// in the original (docs/INLINING.md, pragma rule 1).
#pragma auto_inline off
// Whether the player pushes against something: long enough (fn_80250ACC),
// fn_80250090, state 0xE or fn_80250FD4.
// FIXME: 99.6 %. Registers only (the inlined group loop's leader / self
// temporaries, as in fn_80250ACC).
bool Player_804A6F38::fn_80250D30() {
    if (fn_80250ACC() || fn_80250090() || field_0x1240 == 0xe || fn_80250FD4()) {
        return true;
    }
    return false;
}
#pragma auto_inline reset

// Whether the push against a blocked side (field_0xbf0's field_0xa0) has
// lasted long enough: 0x12 frames for a follower, 0x14 otherwise; never when
// it is not this player's turn in group mode 4, nor for a follower in mode 5.
// FIXME: 99.5 %. Registers only: the inlined group loop's leader / self
// temporaries (r27 / r30 in the original, both r27 here).
bool Player_804A6F38::fn_80250ACC() {
    if (fn_802486A0() == 4 && fn_80240888() != fn_80248778()) {
        return false;
    }
    if (fn_802486A0() == 5 && field_0xd60 >= 0) {
        return false;
    }
    if (field_0xd60 >= 0) {
        if (field_0xbf0->field_0xa0 >= 0x12) {
            return true;
        }
    } else if (field_0xbf0->field_0xa0 >= 0x14) {
        return true;
    }
    return false;
}

// Joins another player's block carry: on the ground (mPos.z <= 0) and not a
// follower in group mode 5, keeps the partner 0xD9C while its fn_80241478
// holds, else finds a present player whose fn_80250ACC holds inside the box
// +-1 at mPos + lbl_8046AE08[facing] while the stick points along the facing
// (fn_80247330 / fn_8024712C), and takes it as 0xD9C with its grabbed actor
// (fn_8024FF84) in 0xD94 for 3 frames (0xD98).
// FIXME: 99.21 %. Scheduling and registers only (the inlined group loop's
// count / index temporaries).
bool Player_804A6F38::fn_80250090() {
    if (mPos.z > 0.0f) {
        field_0xd9c = -1;
        field_0xd98 = 0;
        return false;
    }
    if (fn_802486A0() == 5 && field_0xd60 >= 0) {
        field_0xd9c = -1;
        field_0xd98 = 0;
        return false;
    }
    if (field_0xd9c >= 0 && fn_8022D930(field_0xd9c)) {
        field_0xd98 = 3;
        return true;
    }
    JGeometry::TBox2f box(-1.0f, -1.0f, 1.0f, 1.0f);
    box.i.x += mPos.x;
    box.f.x += mPos.x;
    box.i.y += mPos.y;
    box.f.y += mPos.y;
    // The position is added field by field: addPos(mPos.x, mPos.y) loses two
    // instructions against the original.
    box.addPos(lbl_8046AE08[field_0x3dc][0], lbl_8046AE08[field_0x3dc][1]);
    for (s32 i = 0; i < 4; i++) {
        if (i == field_0x4 || !fn_8022DA68(i) || !fn_8023B0F4(i, &box, field_0x3b4, false, 0.0f)) {
            continue;
        }
        s32 facing = field_0x3dc;
        if (facing == 0) {
            if (fn_80247330() <= 0.0f) {
                continue;
            }
        } else if (facing == 1) {
            if (fn_80247330() >= 0.0f) {
                continue;
            }
        } else if (facing == 2) {
            if (fn_8024712C() >= 0.0f) {
                continue;
            }
        } else if (facing == 3) {
            if (fn_8024712C() <= 0.0f) {
                continue;
            }
        }
        if (fn_80238CB4(i) >= 0) {
            field_0xd94 = fn_80238CB4(i);
        }
        field_0xd9c = i;
        field_0xd98 = 3;
        return true;
    }
    return field_0xd98 > 0;
}

bool Player_804A6F38::fn_80250030(s32 arg) {
    if (fn_80250D30() && fn_802729F0(arg)) {
        return true;
    }
    return false;
}

bool Player_804A6F38::fn_8024FFC8(s32 arg) {
    if (fn_8024FEFC() && arg == fn_8024FF6C()) {
        return true;
    }
    return false;
}

void Player_804A6F38::fn_8024FFB4(s32 arg) {
    if (field_0xd94 < 0) {
        field_0xd94 = arg;
    }
}

void Player_804A6F38::fn_8024FF9C(s32 arg) {
    if (field_0xd94 == arg) {
        field_0xd94 = -1;
    }
}

s32 Player_804A6F38::fn_8024FF84() {
    s32 value = field_0xd94;
    if (value < 0) {
        return -1;
    }
    return value;
}

s32 Player_804A6F38::fn_8024FF6C() {
    s32 value = field_0xda4;
    if (value < 0) {
        return -1;
    }
    return value;
}

s32 Player_804A6F38::fn_8024FF54() {
    s32 value = field_0xdac;
    if (value < 0) {
        return -1;
    }
    return value;
}

bool Player_804A6F38::fn_8024FEFC() {
    bool result = false;
    if (fn_8024FEBC() || fn_8024FE78()) {
        result = true;
    }
    return result;
}

bool Player_804A6F38::fn_8024FEBC() {
    if (field_0x1240 == 0x1a && fn_80243C64()) {
        return true;
    }
    return false;
}

bool Player_804A6F38::fn_8024FE78() {
    if (fn_8024F63C()) {
        return fn_80243C64();
    }
    return false;
}

// Joins another player's push: with pad bit 0x20 held (fn_802456FC), or
// while the partner 0xDAC's pushed actor (its 0xDA4) still exists, or on a
// fresh press (fn_80245938), and not as a follower in group mode 5, finds a
// present player in front (box +-1 at mPos + lbl_8046AE58[facing], the
// point lbl_8046AE28[facing] free of fn_802265DC / fn_802684A4) facing the
// same way and held by nobody; from states 0 / 2 / 3 (0x11 unless
// fn_8025F500 / fn_8025F590) it requests the push 0x1A, else takes that
// player as 0xDAC with its pushed actor in 0xDA4 for 3 frames (0xDA8).
// FIXME: 99.07 %. Scheduling and registers only (the inlined group loops'
// index / leader temporaries).
bool Player_804A6F38::fn_8024F63C() {
    if (!fn_802456FC()) {
        if (field_0xdac >= 0) {
            if (getActorManager_801F856C()->findActorBySerial_801F666C(fn_8022D7F8(field_0xdac)) == NULL) {
                field_0xdac = -1;
                field_0xda8 = 0;
                field_0xda4 = -1;
                return false;
            }
        } else if (!fn_80245938()) {
            field_0xdac = -1;
            field_0xda8 = 0;
            field_0xda4 = -1;
            return false;
        }
    }
    if (fn_802486A0() == 5 && field_0xd60 >= 0) {
        field_0xdac = -1;
        field_0xda8 = 0;
        field_0xda4 = -1;
        return false;
    }
    const Vec& ahead = lbl_8046AE28[field_0x3dc];
    JGeometry::TVec3f pos;
    pos.x = ahead.x;
    pos.y = ahead.y;
    pos.z = ahead.z;
    pos += mPos;
    if (fn_802265DC(&pos, field_0x3b4, true) || fn_802684A4(&pos, field_0x3b4)) {
        field_0xdac = -1;
        field_0xda8 = 0;
        return false;
    }
    JGeometry::TBox2f box(-1.0f, -1.0f, 1.0f, 1.0f);
    box.i.x += mPos.x;
    box.f.x += mPos.x;
    box.i.y += mPos.y;
    box.f.y += mPos.y;
    // The position is added field by field: addPos(mPos.x, mPos.y) loses two
    // instructions against the original.
    box.addPos(lbl_8046AE58[field_0x3dc][0], lbl_8046AE58[field_0x3dc][1]);
    for (s32 i = 0; i < 4; i++) {
        if (i == field_0x4 || !fn_8022D894(i)) {
            continue;
        }
        s32 facing = field_0x3dc;
        if (facing != fn_80230CFC(i)) {
            continue;
        }
        bool held = false;
        for (s32 j = 0; j < 4; j++) {
            if (i == fn_80238EDC(j)) {
                held = true;
                break;
            }
        }
        if (held || !fn_8023B0F4(i, &box, field_0x3b4, false, 0.0f)) {
            continue;
        }
        s32 state = field_0x1240;
        if (state != 0x1a &&
            (state == 0 || state == 2 || state == 3 || (state == 0x11 && !fn_8025F500() && !fn_8025F590()))) {
            if (fn_80251AEC(0x1a)) {
                break;
            }
        }
        if (fn_8022D7F8(i) >= 0) {
            field_0xda4 = fn_8022D7F8(i);
        }
        field_0xdac = i;
        field_0xda8 = 3;
        return true;
    }
    if (field_0xda8 > 0) {
        return true;
    }
    return false;
}

bool Player_804A6F38::fn_8024F630() {
    return field_0xbf0->field_0xb4;
}

bool Player_804A6F38::fn_8024F5E4() {
    if (!fn_8025F448() && field_0x1240 == 0x19) {
        return true;
    }
    return false;
}

void Player_804A6F38::fn_8024F54C(s32 arg) {
    if (field_0x3b4 >= 8 && fn_8030381C(fn_80240628()) && arg == 0x1000002) {
        fn_803037D4(fn_80240628(), 0x10);
    } else {
        gAudioMgr_8053AF80->fn_80458F04(arg);
    }
}

// Plays cue `sound` for this player (0x20024 / 0x2002B become 0x20046 while
// fn_8022B244). With the pad flag (fn_8030A308), the player taking part and
// fn_8030381C, some cues are pad rumbles instead (fn_803037D4): 0x20034 -> 0xC
// and 0x20011 -> 4, and on a layer of 8 or more 0x30009 / 0x30004 / 0x2003A
// -> 8 / 0xB / 7 for every player taking part with fn_8030381C that is this
// one or on its layer near it (fn_8022BC38). Otherwise the sound object
// plays it: field_0x1254 in game mode 0xC, else field_0x1250 below layer 8
// and field_0x1254 from it, with the bytes 0x44 / 0x45 by the layer's view
// flag 0x800, layer 8, GameView field_0x54 and fn_802D7948.
void Player_804A6F38::fn_8024F0E8(s32 sound) {
    if ((sound == 0x20024 || sound == 0x2002b) && fn_8022B244()) {
        sound = 0x20046;
    }
    if (lbl_8053AD30->fn_8030A308(getPlayerInputSlot_8030AAF4(field_0x11ec)) &&
        isPlayerBitSet(gGameRoot_8053AAF8, field_0x4) && fn_8030381C(field_0x4)) {
        if (sound == 0x20034) {
            fn_803037D4(field_0x4, 0xc);
            return;
        }
        if (sound == 0x20011) {
            fn_803037D4(field_0x4, 4);
            return;
        }
        if (field_0x3b4 >= 8) {
            if (sound == 0x30009) {
                for (s32 i = 0; i < 4; i++) {
                    if ((i == field_0x4 || fn_8022BC38(i, field_0x3b4, &mPos)) && fn_8030381C(i) &&
                        isPlayerBitSet(gGameRoot_8053AAF8, i)) {
                        fn_803037D4(i, 8);
                    }
                }
                return;
            }
            if (sound == 0x30004) {
                for (s32 i = 0; i < 4; i++) {
                    if ((i == field_0x4 || fn_8022BC38(i, field_0x3b4, &mPos)) && fn_8030381C(i) &&
                        isPlayerBitSet(gGameRoot_8053AAF8, i)) {
                        fn_803037D4(i, 0xb);
                    }
                }
                return;
            }
            if (sound == 0x2003a) {
                for (s32 i = 0; i < 4; i++) {
                    if ((i == field_0x4 || fn_8022BC38(i, field_0x3b4, &mPos)) && fn_8030381C(i) &&
                        isPlayerBitSet(gGameRoot_8053AAF8, i)) {
                        fn_803037D4(i, 7);
                    }
                }
                return;
            }
        }
    }
    if (gGameRoot_8053AAF8->field_0x4c == 0xc) {
        field_0x1254->field_0x44 = 0;
        field_0x1254->field_0x45 = 0;
        field_0x1254->startSound(sound);
        return;
    }
    if (getGameView_802D6A9C()->fn_802D7804(field_0x3b4, 0x800)) {
        if (field_0x3b4 < 8) {
            field_0x1250->field_0x44 = 0;
            field_0x1250->field_0x45 = 0;
            field_0x1250->startSound(sound);
        } else {
            field_0x1254->field_0x44 = 1;
            field_0x1254->field_0x45 = 0;
            field_0x1254->startSound(sound);
        }
        return;
    }
    if (field_0x3b4 < 8) {
        field_0x1250->field_0x44 = 0;
        if (getGameView_802D6A9C()->fn_802D7948() == 1) {
            field_0x1250->field_0x45 = 2;
        } else {
            field_0x1250->field_0x45 = 0;
        }
        field_0x1250->startSound(sound);
    } else if (field_0x3b4 == 8) {
        field_0x1250->field_0x44 = 1;
        field_0x1250->field_0x45 = 3;
        field_0x1250->startSound(sound);
    } else {
        // The view is fetched before the layer is read (lwz 0x3B4, then 0x54).
        GameView_804A9950* view = getGameView_802D6A9C();
        if (field_0x3b4 < view->field_0x54) {
            field_0x1254->field_0x44 = 1;
            if (getGameView_802D6A9C()->fn_802D7948() == 1) {
                field_0x1254->field_0x45 = 2;
            } else {
                field_0x1254->field_0x45 = 1;
            }
            field_0x1254->startSound(sound);
        } else {
            field_0x1254->field_0x44 = 1;
            field_0x1254->field_0x45 = 2;
            field_0x1254->startSound(sound);
        }
    }
}

// Plays sound `id` for this player (AudioMgr fn_80458880); with the pad
// flag (fn_80247534) and fn_8030381C the ids 4 / 9 / 5 go to fn_803037D4 as
// 4 / 5 / 6, and id 3 on a layer of 8 or more as 0xF.
void Player_804A6F38::fn_8024EFF8(u32 id) {
    if (fn_80247534() && fn_8030381C(field_0x4)) {
        if (id == 4) {
            fn_803037D4(field_0x4, 4);
            return;
        }
        if (id == 9) {
            fn_803037D4(field_0x4, 5);
            return;
        }
        if (id == 5) {
            fn_803037D4(field_0x4, 6);
            return;
        }
    }
    if (field_0x3b4 >= 8 && fn_8030381C(field_0x4) && id == 3) {
        fn_803037D4(field_0x4, 0xf);
        return;
    }
    gAudioMgr_8053AF80->fn_80458880(id, field_0x4);
}

bool Player_804A6F38::fn_8024EFA0(bool arg) {
    s32 team = field_0xd60;
    if (team >= 0) {
        return fn_80230E1C(team, arg);
    }
    if (field_0x11dc == 0) {
        field_0x11dc = 0x70;
        field_0x11e0 = arg;
        return true;
    }
    return false;
}

// Counts field_0x11dc down (a leader or a player on its own). In its last
// 0x60 frames the flag lbl_8053ACD8[field_0x4] blinks (set while bit 0x10 of
// the count and field_0x11e0 are clear); every 32 frames (count & 0x1f ==
// 15) fn_803037D4(field_0x4, 5) when fn_8030A308 and fn_8030381C allow it,
// else sound 9. While it runs and fn_8023E10C is below 8, fn_80402FBC gets
// fn_8023E214's vector; once it has run out the flag follows the player's
// layer (< 8).
void Player_804A6F38::fn_8024EE24() {
    if (field_0xd60 >= 0) {
        return;
    }
    if (field_0x11dc > 0) {
        field_0x11dc--;
        if (field_0x11dc < 0x60) {
            if (!field_0x11e0 && !(field_0x11dc & 0x10)) {
                lbl_8053ACD8[field_0x4] = 1;
            } else {
                lbl_8053ACD8[field_0x4] = 0;
            }
            if ((field_0x11dc & 0x1f) == 0xf) {
                if (lbl_8053AD30->fn_8030A308(getPlayerInputSlot_8030AAF4(field_0x11ec)) && fn_8030381C(field_0x4)) {
                    fn_803037D4(field_0x4, 5);
                } else {
                    if (field_0x3b4 >= 8) {
                        fn_8030381C(field_0x4);
                    }
                    // The original converts the index to s8 (extsb) for the sound call.
                    gAudioMgr_8053AF80->fn_80458880(9, (s8)field_0x4);
                }
            }
        }
        if (fn_8023E10C(fn_8022EA0C(field_0x4)) < 8) {
            s32 player = field_0x4;
            fn_80402FBC(player, *fn_8023E214(fn_8022EA0C(player)));
        }
    } else if (getPlayerLayer_8023DE58(fn_8022EA0C(field_0x4)) < 8) {
        lbl_8053ACD8[field_0x4] = 1;
    } else {
        lbl_8053ACD8[field_0x4] = 0;
    }
}

u16 Player_804A6F38::fn_8024EE1C() {
    return field_0x3c4;
}

s8 Player_804A6F38::fn_8024EE14() {
    return field_0x3d8;
}

s8 Player_804A6F38::fn_8024EE0C() {
    return field_0x3d9;
}

// Adds `value` to the counter field_0xc04 (the leader's, through
// fn_802362C4, for a follower): up to 9999, and a negative value takes it
// down to 0 at most (a counter already below 0 stays). The two `for (;;)`
// are the original's halt checks (a self-branch each): the counter must be
// at most 9999 and must take a store (written +1, read back, restored).
// The +1 / read back / halt / restore check is the same construct as
// tu_8026FC20 fn_80270DDC on field_0xbfc (source form unexplained,
// docs/CRACK_PASS.md, Open questions).
void Player_804A6F38::fn_8024ED4C(s32 value) {
    if (field_0xd60 >= 0) {
        fn_802362C4(field_0xd60, value);
        return;
    }
    if (field_0xc04 > 9999) {
        for (;;) {
        }
    }
    s32 old = field_0xc04;
    field_0xc04 = old + 1;
    if (field_0xc04 == old) {
        for (;;) {
        }
    }
    field_0xc04 = old;
    if (value >= 0) {
        field_0xc04 += value;
        if (field_0xc04 > 9999) {
            field_0xc04 = 9999;
        }
    } else {
        if (field_0xc04 < 0) {
            return;
        }
        if (field_0xc04 + value >= 0) {
            field_0xc04 = field_0xc04 + value;
        } else {
            field_0xc04 = 0;
        }
    }
    if (value == 1) {
        fn_802702C4();
    }
}

void Player_804A6F38::fn_8024ECCC(s32 value) {
    if (field_0xd60 >= 0) {
        fn_8023619C(field_0xd60, value);
        return;
    }
    if (field_0xc04 > 9999) {
        for (;;) {
        }
    }
    field_0xc04 += value;
    if (field_0xc04 > 9999) {
        field_0xc04 = 9999;
    } else if (field_0xc04 < -5000) {
        field_0xc04 = -5000;
    }
    if (value == 1) {
        fn_802702C4();
    }
}

// Drops up to `value` Force Gems (no more than the counter fn_80241920 holds)
// as an ACTOR_GOLD at the player, or at the acting player when it is not
// this player's turn in group mode 4, and takes them off the counter
// (fn_8024ED4C). On a GBA layer (8 and up), or with `single`, the amount is
// rounded down to a gem value (1, 5, 20, 50, 100, 150, 200, 300, 1000, 1500,
// 2000, 3000) spawned as one gem of that kind (param kind | 0x40000, 0x70000
// in state 0x51); otherwise the amount itself (| 0x60000 in state 0x51).
// FIXME: 99.82 %. Registers only: the inlined group loop's count / index /
// leader temporaries.
void Player_804A6F38::fn_8024E694(s32 value, s32 single) {
    JGeometry::TVec3f pos = mPos;
    s32 layer = field_0x3b4;
    if (fn_802486A0() == 4 && fn_80240888() != fn_80248778()) {
        pos.set(*getPlayerPos_8023E724(fn_8022EA0C(field_0x4)));
        layer = getPlayerLayer_8023DE58(fn_8022EA0C(field_0x4));
    }
    bool state51 = false;
    if (field_0x1240 == 0x51) {
        state51 = true;
    }
    if (value > fn_80241920()) {
        value = fn_80241920();
    }
    if (field_0x3b4 >= 8) {
        if (value < 1) {
            return;
        }
        if (value < 5) {
            value = 1;
        } else if (value < 20) {
            value = 5;
        } else if (value < 50) {
            value = 20;
        } else if (value < 100) {
            value = 50;
        } else if (value < 150) {
            value = 100;
        } else if (value < 200) {
            value = 150;
        } else if (value < 300) {
            value = 200;
        } else if (value < 1000) {
            value = 300;
        } else if (value < 1500) {
            value = 1000;
        } else if (value < 2000) {
            value = 1500;
        } else if (value < 3000) {
            value = 2000;
        } else {
            value = 3000;
        }
        single = true;
    }
    // The original tests the low byte (clrlwi.): `single` is a bool there,
    // but the forwarder fn_80236074 (tu_8022AD64, generator-pinned) and the
    // symbol (Fll) declare it s32.
    if ((u8)single) {
        s32 kind;
        switch (value) {
        case 1:
            kind = 0;
            break;
        case 5:
            kind = 1;
            break;
        case 20:
            kind = 2;
            break;
        case 50:
            kind = 3;
            break;
        case 100:
            kind = 4;
            break;
        case 150:
            kind = 5;
            break;
        case 200:
            kind = 6;
            break;
        case 300:
            kind = 7;
            break;
        case 1000:
            kind = 8;
            break;
        case 1500:
            kind = 9;
            break;
        case 2000:
            kind = 10;
            break;
        case 3000:
            kind = 11;
            break;
        default:
            return;
        }
        if (state51) {
            getActorManager_801F856C()->spawnActor_801F9484(ACTOR_GOLD, layer, &pos, kind | 0x70000, -1, -1);
        } else {
            getActorManager_801F856C()->spawnActor_801F9484(ACTOR_GOLD, layer, &pos, kind | 0x40000, -1, -1);
        }
        fn_8024ED4C(-value);
        return;
    }
    if (value > fn_80241920()) {
        value = fn_80241920();
    }
    if (value <= 0) {
        return;
    }
    if (state51) {
        getActorManager_801F856C()->spawnActor_801F9484(ACTOR_GOLD, layer, &pos, value | 0x60000, -1, -1);
    } else {
        getActorManager_801F856C()->spawnActor_801F9484(ACTOR_GOLD, layer, &pos, value, -1, -1);
    }
    fn_8024ED4C(-value);
}

// Drops the held item (fn_802416C4, with its level fn_802415CC) as an
// ACTOR_LSTI at the player's position and clears it (fn_80241618(0, 1)), for
// a player taking part.
// FIXME: 99.7 %. Registers only: player / level r29 / r28 in the original,
// r28 / r29 here.
void Player_804A6F38::fn_8024E500() {
    if (!isPlayerBitSet(gGameRoot_8053AAF8, field_0x4)) {
        return;
    }
    if (fn_802416C4() == 0) {
        return;
    }
    s32 player = field_0x4;
    s32 item = fn_802416C4();
    s32 level = fn_802415CC();
    getActorManager_801F856C()->spawnActor_801F9484(ACTOR_LSTI, field_0x3b4, &mPos,
                                                    item | (level << 22 | player << 17), -1, -1);
    fn_80241618(0, 1);
}

// FIXME: 99.86. Code identical; the float literal lbl_80540080 (5.0f) is not yet in the original pool order (@NNN vs lbl_).
void Player_804A6F38::fn_8024E46C() {
    if (field_0x3b4 < 8) {
        fn_8024E694((s32)(5.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 5, 0);
    } else {
        fn_8024E694(5, 0);
    }
}

// FIXME: 99.86. Code identical; the float literal lbl_8054007C (10.0f) is not yet in the original pool order (@NNN vs lbl_).
void Player_804A6F38::fn_8024E3D8() {
    if (field_0x3b4 < 8) {
        fn_8024E694((s32)(10.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 0x14, 0);
    } else {
        fn_8024E694(0x14, 0);
    }
}

// FIXME: 99.86. Code identical; the float literal lbl_80540078 (20.0f) is not yet in the original pool order (@NNN vs lbl_).
void Player_804A6F38::fn_8024E344() {
    if (field_0x3b4 < 8) {
        fn_8024E694((s32)(20.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 0x32, 0);
    } else {
        fn_8024E694(0x32, 0);
    }
}

// Not auto-inlined: fn_80253E6C calls fn_8024E0A0 out of line in the original
// (docs/INLINING.md, pragma rule 1).
#pragma auto_inline off
// Every 60 frames (field_0x1264), in the game modes of isGameModeSet, for a
// player acting on its own with a life (fn_8024189C) of 4 or less, not in
// fn_80270A8C's state and without field_0x454: fn_803037D4(field_0x4, 6)
// when fn_8030A308 and fn_8030381C allow it, else sound 5. Then the sound
// objects' position field_0x1258 follows the player (fn_8024C378 from layer
// 8 on) and both run fn_80456154.
// FIXME: 99.3 %. Registers only, in the inlined fn_8024C378 (the view box
// corner in f31 / f30, swapped here, as in fn_8024C378 itself).
void Player_804A6F38::fn_8024E0A0() {
    field_0x1264++;
    if (field_0x1264 >= 0x3c) {
        field_0x1264 = 0;
        if (isGameModeSet() && !fn_80270A8C() && !field_0x454 && fn_8024189C() <= 4 && field_0xd60 < 0) {
            if (lbl_8053AD30->fn_8030A308(getPlayerInputSlot_8030AAF4(field_0x11ec)) && fn_8030381C(field_0x4)) {
                fn_803037D4(field_0x4, 6);
            } else {
                if (field_0x3b4 >= 8) {
                    fn_8030381C(field_0x4);
                }
                // The original converts the index to s8 (extsb) for the sound call.
                gAudioMgr_8053AF80->fn_80458880(5, (s8)field_0x4);
            }
        }
    }
    if (field_0x3b4 < 8) {
        field_0x1258 = mPos;
    } else {
        field_0x1258 = *fn_8024C378();
    }
    field_0x1250->fn_80456154();
    field_0x1254->fn_80456154();
}
#pragma auto_inline reset

bool Player_804A6F38::fn_8024E060() {
    if (field_0x454) {
        return false;
    }
    if (field_0xce8 == 1) {
        return false;
    }
    if (field_0x1216 == 1) {
        return false;
    }
    return true;
}

bool Player_804A6F38::fn_8024E03C() {
    if (field_0x48c > 0) {
        field_0x48c = 1;
        return true;
    }
    return false;
}

void Player_804A6F38::fn_8024E020(const JGeometry::TVec3f* arg) {
    field_0x2b8.set(arg->x, arg->y, arg->z);
}

JGeometry::TVec3f* Player_804A6F38::fn_8024E018() {
    return &field_0x2b8;
}

// Puts the player into the 0x454 mode (fn_8024E060 must allow it; state 0x4F
// first asks for state 0): keeps mPos in 0x32C and picks the frame 0x450 by
// state and facing (0x3DC); then 0xCA2 = 0xFF, 0x452 = 0, the emitters go
// (fn_80241E64), the 'FORM' actor ends (fn_80248FF0) and 0xCAE / 0xCAF are set.
bool Player_804A6F38::fn_8024DCD0() {
    if (!fn_8024E060()) {
        return false;
    }
    if (field_0x1240 == 0x4f) {
        fn_80251AEC(0);
    }
    field_0x454 = true;
    field_0x32c = mPos;
    field_0x455 = 0;
    field_0x450 = field_0x3c4;
    if (fn_80270A8C()) {
        field_0x450 = field_0x3c4;
    } else if (fn_8025F448()) {
        field_0x450 = field_0x3c4;
    } else if (fn_802412A4()) {
        if (field_0x3dc == 0) {
            field_0x450 = 0x1e8;
        } else if (field_0x3dc == 1) {
            field_0x450 = 0x1ec;
        } else if (field_0x3dc == 2) {
            field_0x450 = 0x1f0;
        } else {
            field_0x450 = 0x1f4;
        }
    } else if (fn_8025F684()) {
        if (field_0x3dc == 0) {
            field_0x450 = 0x2c0;
        } else if (field_0x3dc == 1) {
            field_0x450 = 0x2c4;
        } else if (field_0x3dc == 2) {
            field_0x450 = 0x2c8;
        } else {
            field_0x450 = 0x2cf;
        }
    } else if (!fn_80241200()) {
        if (field_0x3dc == 0) {
            field_0x450 = 0x3c;
        } else if (field_0x3dc == 1) {
            field_0x450 = 0x3d;
        } else if (field_0x3dc == 2) {
            field_0x450 = 0x3e;
        } else {
            field_0x450 = 0x3f;
        }
    }
    field_0xca2 = 0xff;
    field_0x452 = 0;
    fn_80241E64();
    fn_80248FF0();
    field_0xcae = 1;
    field_0xcaf = 1;
    return true;
}

// Sets the target 0x32C to pos, the frame 0x450 (unless negative) and the
// bytes 0xCA2-0xCA4; with `check`, fn_8024B2E0 when the player is at
// least 0.1 away from pos. Frame 0xC3 keeps field_0xcfe.
// FIXME: 99.9 %. Code identical; the 0.1f literal is @NNN, not
// lbl_80540074, until the earlier literals of the unit are in pool order.
bool Player_804A6F38::fn_8024DC00(const JGeometry::TVec3f* pos, s32 frame, u8 a, s8 b, s8 c, bool check) {
    field_0x32c = *pos;
    if (frame >= 0) {
        field_0x450 = frame;
    }
    field_0xca2 = a;
    field_0xca3 = b;
    field_0xca4 = c;
    if (check == true) {
        if (magnitude(mPos - *pos) >= 0.1f) {
            fn_8024B2E0();
        }
    }
    if (frame != 0xc3) {
        field_0xcfe = false;
    }
    return true;
}

// Only in state 0: sets field_0x450 to `id` (a negative id leaves it; id
// 0xE8 becomes 0x1C5 in turn-based group mode when it is not this player's
// turn), field_0xca2 to `flag` and field_0x456.
// FIXME: 99.3 %. Registers only: the inlined group loops' leader / self
// temporaries (r25 / r28 in the original, both r25 here).
bool Player_804A6F38::fn_8024DA64(s32 id, bool flag) {
    if (field_0x1240 != 0) {
        return false;
    }
    if (id == 0xe8 && fn_802486A0() == 4 && fn_80240888() != fn_80248778()) {
        field_0x450 = 0x1c5;
    } else if (id >= 0) {
        field_0x450 = id;
    }
    field_0xca2 = flag;
    field_0x456 = 1;
    return true;
}

// Sets the target 0x32C to pos and steps the frame counter 0x452 (0-23);
// 0x450 = 8 * facing + 0x452 / 3, 0xCA2 = flag; then fn_8024B2E0.
bool Player_804A6F38::fn_8024D99C(const JGeometry::TVec3f* pos, bool flag) {
    field_0x32c = *pos;
    field_0x452++;
    if (field_0x452 >= 0x18) {
        field_0x452 = 0;
    }
    if (field_0x3dc == 0) {
        field_0x450 = 0;
    } else if (field_0x3dc == 1) {
        field_0x450 = 8;
    } else if (field_0x3dc == 2) {
        field_0x450 = 0x10;
    } else {
        field_0x450 = 0x18;
    }
    field_0x450 += (u8)(field_0x452 / 3);
    field_0xca2 = flag;
    fn_8024B2E0();
    return true;
}

bool Player_804A6F38::fn_8024D98C() {
    field_0x455 = 1;
    return true;
}

// Restarts the group-spot marker of a leader (field_0x1388): not in GameRoot
// mode 0x7E, for a group in modes 0-3 (not 4 / 5); clears field_0x121d and
// spawns effect 0x35E / 0x360 / 0x35F / 0x361 by mode at the spot
// fn_8027F1C8 also uses (beside the group by 8 a place in modes 0 / 1,
// (8, 8) in mode 2, the spin centre 0x11B4 in states 9 / 0xA or 16 toward
// the player's side in mode 3), on the ground.
// FIXME: 99.65 %. Registers only (the inlined group loop's count / index
// temporaries).
void Player_804A6F38::fn_8024D134() {
    if (gGameRoot_8053AAF8->field_0x7e) {
        return;
    }
    if (field_0xd60 >= 0) {
        return;
    }
    if (fn_802486A0() == 5 || fn_802486A0() == 4) {
        return;
    }
    field_0x121d = false;
    deleteEmitter(&field_0x1388);
    JGeometry::TVec3f pos;
    pos.set(mPos);
    pos.z = 0.0f;
    switch (fn_802486A0()) {
    case 0:
        pos.x += 8.0f * (fn_802487FC() - 1);
        break;
    case 1:
        pos.y += 8.0f * (fn_802487FC() - 1);
        break;
    case 2:
        pos.x += 8.0f;
        pos.y += 8.0f;
        break;
    case 3:
        if (fn_80241284()) {
            pos.set(field_0x11b4);
        } else {
            switch (field_0x4) {
            case 0:
                pos.y += 16.0f;
                break;
            case 1:
                pos.x += 16.0f;
                break;
            case 2:
                pos.x -= 16.0f;
                break;
            case 3:
                pos.y -= 16.0f;
                break;
            }
        }
        break;
    }
    switch (fn_802486A0()) {
    case 0:
        field_0x1388 = spawnEmitter_8013CC50(field_0x3b4, &pos, 0x35e, 0, -1, NULL, NULL, true, 1.0f);
        break;
    case 1:
        field_0x1388 = spawnEmitter_8013CC50(field_0x3b4, &pos, 0x360, 0, -1, NULL, NULL, true, 1.0f);
        break;
    case 2:
        field_0x1388 = spawnEmitter_8013CC50(field_0x3b4, &pos, 0x35f, 0, -1, NULL, NULL, true, 1.0f);
        break;
    case 3:
        field_0x1388 = spawnEmitter_8013CC50(field_0x3b4, &pos, 0x361, 0, -1, NULL, NULL, true, 1.0f);
        break;
    }
}

// Clears field_0x454: with `reset` the trail is reset to the current
// position (fn_80248604), else fn_8024B2E0 runs first. Then the timers
// 0xD74 / 0x11F0 = 10 and fn_8024D134.
bool Player_804A6F38::fn_8024D058(u8 reset) {
    if (reset) {
        field_0x454 = false;
        fn_80248604();
    } else {
        fn_8024B2E0();
        field_0x454 = false;
    }
    field_0xd74 = 10;
    field_0x11f0 = 10;
    fn_8024D134();
    return true;
}

// fn_8024CAC8 for a box (act_dummy_conversation through fn_8022FEB8): the
// same tests, then the point 16 ahead of the player (by facing 0x3DC) must
// lie in `box`.
// FIXME: 99.64 %. Registers only: the inlined group loop (fn_80240670) keeps
// the leader and the own index in two registers (r22 / r25) in the original,
// one here, as in the gates (fn_802454C0 ...).
void Player_804A6F38::fn_8024CD20(const JGeometry::TBox2f* box, s32 layer, s32 serial, bool a, bool b) {
    if (field_0xd50 > 0) {
        return;
    }
    if (field_0xd44 >= 0 && field_0xd44 != serial) {
        return;
    }
    if (field_0xd3c >= 0) {
        return;
    }
    if (fn_80240670()) {
        return;
    }
    if (layer != field_0x3b4) {
        return;
    }
    if (fn_8025F448()) {
        return;
    }
    if (field_0xce8) {
        return;
    }
    if (!field_0x454 && fn_80241230()) {
        return;
    }
    if (b == true && field_0x3dc != 3) {
        return;
    }
    const Vec& step = lbl_8046AE78[field_0x3dc];
    JGeometry::TVec3f pos;
    pos.x = step.x;
    pos.y = step.y;
    pos.z = step.z;
    pos += mPos;
    pos.z = 0.0f;
    JGeometry::TBox2f point;
    point.i.x = pos.x;
    point.i.y = pos.y;
    point.f.x = pos.x;
    point.f.y = pos.y;
    if (point.intersect(*box)) {
        field_0xd3c = serial;
        field_0xd4e = a;
    }
}

// Starts a conversation (act_dummy_conversation through fn_8022FE80): keeps
// `serial` in field_0xd3c and `a` in field_0xd4e when none is pending, the
// player waits for this serial or none (field_0xd44), is on `layer`, acts on
// its own, is not carried (field_0xce8) or fn_8025F448, not in state 0xB
// unless field_0x454, and with `b` faces 3.
// FIXME: 99.53. Registers only (the inlined group loops' temporaries).
void Player_804A6F38::fn_8024CAC8(s32 layer, s32 serial, bool a, bool b) {
    if (field_0xd50 > 0) {
        return;
    }
    if (field_0xd44 >= 0 && field_0xd44 != serial) {
        return;
    }
    if (field_0xd3c >= 0) {
        return;
    }
    if (fn_80240670()) {
        return;
    }
    if (layer != field_0x3b4) {
        return;
    }
    if (fn_8025F448()) {
        return;
    }
    if (field_0xce8) {
        return;
    }
    if (!field_0x454 && fn_80241230()) {
        return;
    }
    if (b == true && field_0x3dc != 3) {
        return;
    }
    field_0xd3c = serial;
    field_0xd4e = a;
}

void Player_804A6F38::fn_8024CA20() {
    if (field_0xd50 > 0) {
        field_0xd50--;
    }
    if (!field_0xd4d) {
        if (field_0xd3c < 0) {
            field_0xd44 = -1;
            field_0xd4c = false;
        } else if (field_0xd44 < 0) {
            if (!field_0xd4e || fn_8022C1D8(field_0x4)) {
                field_0xd44 = field_0xd3c;
            }
        }
    }
    field_0xd40 = field_0xd3c;
    field_0xd3c = -1;
}

bool Player_804A6F38::fn_8024CA0C(s32 arg) {
    return field_0xd44 == arg;
}

// Whether field_0xd44 is `arg` for a player acting on its own
// (fn_80240670 false), not walking to its spot (field_0x1216) and
// without field_0xd4c.
// FIXME: 99.42. Registers only (the inlined group loops' temporaries).
bool Player_804A6F38::fn_8024C828(s32 arg) {
    if (fn_80240670()) {
        return false;
    }
    if (field_0x1216 == 1) {
        return false;
    }
    if (field_0xd4c == 1) {
        return false;
    }
    if (field_0xd44 != arg) {
        return false;
    }
    return true;
}

// When fn_8024C828(serial) allows it: sets field_0xd4c and spawns the
// player effect ACTOR_PEFF at the player, kind 0x1C when the actor `serial`
// exists at or to the right of the player (x), else 0x18 (plus the player's
// index & 3).
// FIXME: 99.6 %. Registers only (the inlined group loops' temporaries).
bool Player_804A6F38::fn_8024C5A0(s32 serial) {
    if (!fn_8024C828(serial)) {
        return false;
    }
    field_0xd4c = 1;
    ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
    if (actor == NULL || actor->mPos.x < mPos.x) {
        getActorManager_801F856C()->spawnActor_801F9484(ACTOR_PEFF, field_0x3b4, &mPos, (field_0x4 & 3) | 0x18, -1, -1);
    } else {
        getActorManager_801F856C()->spawnActor_801F9484(ACTOR_PEFF, field_0x3b4, &mPos, (field_0x4 & 3) | 0x1c, -1, -1);
    }
    return true;
}

bool Player_804A6F38::fn_8024C4C4(s32 serial) {
    if (field_0xd4c == true) {
        return false;
    }
    field_0xd4c = true;
    field_0xd44 = serial;
    field_0xd4d = 1;
    ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
    if (!actor || actor->mPos.x < mPos.x) {
        getActorManager_801F856C()->spawnActor_801F9484(ACTOR_PEFF, field_0x3b4, &mPos, (field_0x4 & 3) | 0x18, -1, -1);
    } else {
        getActorManager_801F856C()->spawnActor_801F9484(ACTOR_PEFF, field_0x3b4, &mPos, (field_0x4 & 3) | 0x1c, -1, -1);
    }
    return true;
}

bool Player_804A6F38::fn_8024C460(s32 arg) {
    if (!field_0xd4c) {
        return true;
    }
    if (field_0xd44 != arg) {
        return true;
    }
    field_0xd44 = -1;
    field_0xd3c = -1;
    field_0xd4c = false;
    field_0xd4d = 0;
    if (field_0xd74 < 3) {
        field_0xd74 = 3;
    }
    field_0xd50 = 0x14;
    return true;
}

bool Player_804A6F38::fn_8024C458() {
    return field_0xd4c;
}

bool Player_804A6F38::fn_8024C448() {
    return field_0xd40 >= 0;
}

// The position for layer 8 moved from the second view box (field_0xdc)
// into the visible box's coordinates, in field_0x1268; on other layers mPos.
// FIXME: 98.08. Registers only (f0-f3 of the box loads and the offsets).
JGeometry::TVec3f* Player_804A6F38::fn_8024C378() {
    if (field_0x3b4 == 8) {
        GameView_804A9950* view = getGameView_802D6A9C();
        f32 x = view->mVisibleBox.i.x;
        f32 y = view->mVisibleBox.i.y;
        s32 layer = field_0x3b4;
        view = getGameView_802D6A9C();
        const JGeometry::TBox2f* box = layer < 8 ? &view->mVisibleBox : &view->field_0xdc;
        x -= box->i.x;
        y -= box->i.y;
        field_0x1268.set(mPos);
        field_0x1268.x += x;
        field_0x1268.y += y;
        return &field_0x1268;
    }
    return &mPos;
}

bool Player_804A6F38::fn_8024C35C() {
    return field_0x3b4 >= 8;
}

// The call marker of a player on its own (group of one) in GameRoot mode
// 0x7F: while pad bit 0x40 is held (fn_80246B10) the effect system's marker
// 5 at the player (below layer 8 on its layer; on layer 8 offset by the
// view's top-left corner, layer 0; above it at 0xCB0, layer 0); a fresh
// press (fn_80246D1C) adds the pad rumble (fn_8030381C, as joinCue) and
// sound 0x1E. Not in 0x454 mode.
// FIXME: 99.53 %. Registers only (the inlined group loop's count / index
// temporaries).
void Player_804A6F38::fn_8024BDBC() {
    if (field_0x454) {
        return;
    }
    if (fn_80248970() >= 2) {
        return;
    }
    if (!gGameRoot_8053AAF8->field_0x7f) {
        return;
    }
    if (fn_80246B10(0x40) == 0) {
        return;
    }
    if (field_0x3b4 < 8) {
        gGameRoot2_8053AB10->field_0x30->fn_8015F8FC(field_0x3b4, &mPos, 5);
    } else if (field_0x3b4 == 8) {
        JGeometry::TVec3f pos;
        pos.set(mPos);
        GameView_804A9950* view = getGameView_802D6A9C();
        pos += JGeometry::TVec3f(view->mVisibleBox.i.x, view->mVisibleBox.i.y, 0.0f);
        gGameRoot2_8053AB10->field_0x30->fn_8015F8FC(0, &pos, 5);
    } else {
        gGameRoot2_8053AB10->field_0x30->fn_8015F8FC(0, &field_0xcb0, 5);
    }
    if (fn_80246D1C(0x40) == 0) {
        return;
    }
    if (lbl_8053AD30->fn_8030A308(getPlayerInputSlot_8030AAF4(field_0x11ec))) {
        fn_8030381C(field_0x4);
    }
    if (field_0x3b4 >= 8) {
        fn_8030381C(field_0x4);
    }
    // The original converts the index to s8 (extsb) for the sound call.
    gAudioMgr_8053AF80->fn_80458880(0x1e, (s8)field_0x4);
}

// Requests state 0x3F (once, for the first match) when another present
// player (fn_8023A51C) on this layer that this one cannot reach
// (fn_8022E438) is closer than 50 in x/y (80 with fn_8022B244, else 100 at
// its item level 2+, fn_8023725C); not while fn_80270C88 or in state 0x3F /
// 0xC; skipped with field_0xb9a, field_0xcf0 or state 0x3C current or
// requested; then clears field_0xb9d.
// FIXME: 99.61. regalloc-only: the FPRs of the paired-single distance math, and
// the literals 50 / 80 / 2 (@NNN vs lbl_ until the pool is in order). The layer is
// read through getLayer() (before getPlayerLayer_8023DE58, as the original).
void Player_804A6F38::fn_8024BBB8() {
    if (fn_80270C88() || field_0x1240 == 0x3f || field_0x1240 == 0xc) {
        return;
    }
    if (!field_0xb9a && !field_0xcf0 && field_0x1240 != 0x3c && field_0x1244 != 0x3c) {
        for (s32 i = 0; i < 4; i++) {
            if (!fn_8022E438(field_0x4, i) && fn_8023A51C(i) && getLayer() == getPlayerLayer_8023DE58(i)) {
                f32 range = 50.0f;
                if (fn_8022B244()) {
                    range = 80.0f;
                } else if (fn_8023725C(i) >= 2) {
                    range *= 2.0f;
                }
                if (planarDistance(*fn_8023A478(i) - mPos) < range) {
                    fn_80251AEC(0x3f);
                    break;
                }
            }
        }
    }
    field_0xb9d = false;
}

bool Player_804A6F38::fn_8024BBB0() {
    return field_0xce8;
}

s32 Player_804A6F38::fn_8024BB94() {
    if (field_0xce8) {
        return field_0xcf4;
    }
    return 0;
}

void Player_804A6F38::fn_8024BB88() {
    field_0xced = 1;
}

// Resets the position trail (field_0x38, 0x31 entries) and its airborne
// flags (field_0x284) to the current position, unless the player is walking
// to its spot (field_0x1216); while field_0x454 is set fn_8024B2E0 does it.
void Player_804A6F38::fn_8024BAEC() {
    if (field_0x1216) {
        return;
    }
    if (field_0x454) {
        fn_8024B2E0();
        return;
    }
    for (s32 i = 0; i < 0x31; i++) {
        field_0x38[i].x = mPos.x;
        field_0x38[i].y = mPos.y;
        field_0x38[i].z = mPos.z;
        if (mPos.z > 0.0f) {
            field_0x284[i] = true;
        } else {
            field_0x284[i] = false;
        }
    }
}

// Pushes the current position (and the helper's airborne flag 0xB4) onto
// the front of the trail, unless the player walks to its spot.
void Player_804A6F38::fn_8024B9E8() {
    if (field_0x1216) {
        return;
    }
    for (s32 i = 0x30; i > 0; i--) {
        field_0x38[i] = field_0x38[i - 1];
        field_0x284[i] = field_0x284[i - 1];
    }
    field_0x38[0] = mPos;
    field_0x284[0] = field_0xbf0->field_0xb4;
}

// The trail reset in 0x454 mode (fn_8024BAEC): a follower fills the trail
// with field_0x32c, grounded; a player alone with field_0x32c; a leader lays
// it along its group, 12 entries per follower from its own position to the
// next group member (fn_8022E438) by linear steps, the rest at the last one;
// then the airborne flags follow each entry's height.
// The 0x31 loops count with `int`: with s32 (long) MWCC tests the unrolled
// loop's entry (cmpwi 0x31; bge), which the original does not.
// FIXME: 92.26 %. Missing/extra (4 fewer): in the interpolation the original
// keeps the next member's position in registers (loaded once, stored to
// `from` at the end) and reloads `from` from the stack, and builds each step
// in two stack temporaries (x / y copied through the second, z rounded with
// frsp); ours reloads the pointer's floats every pass and writes the step
// directly. Copying the position to a local TVec3f (set / Vec constructor)
// gives 71-85 %.
void Player_804A6F38::fn_8024B2E0() {
    if (field_0x1216) {
        return;
    }
    if (field_0xd60 >= 0) {
        for (int i = 0; i < 0x31; i++) {
            field_0x38[i].x = field_0x32c.x;
            field_0x38[i].y = field_0x32c.y;
            field_0x38[i].z = field_0x32c.z;
            field_0x284[i] = false;
        }
        return;
    }
    if (fn_80248970() <= 1) {
        for (int i = 0; i < 0x31; i++) {
            field_0x38[i].x = field_0x32c.x;
            field_0x38[i].y = field_0x32c.y;
            field_0x38[i].z = field_0x32c.z;
        }
    } else {
        JGeometry::TVec3f from = mPos;
        s32 other = field_0x4 + 1;
        for (s32 n = 1; n < fn_80248970(); n++, other++) {
            while (!fn_8022E438(field_0x4, other)) {
                if (other >= 4) {
                    break;
                }
                other++;
            }
            if (other >= 4) {
                break;
            }
            const JGeometry::TVec3f* to = getPlayerPos_8023E724(other);
            for (s32 j = 0; j < 12; j++) {
                JGeometry::TVec3f step(from.x * (12 - j) / 12.0f + to->x * j / 12.0f,
                                       from.y * (12 - j) / 12.0f + to->y * j / 12.0f,
                                       from.z * (12 - j) / 12.0f + to->z * j / 12.0f);
                field_0x38[(n - 1) * 12 + j] = step;
            }
            from = *to;
        }
        for (s32 i = (fn_80248970() - 1) * 12; i < 0x31; i++) {
            field_0x38[i].x = from.x;
            field_0x38[i].y = from.y;
            field_0x38[i].z = from.z;
        }
    }
    for (int i = 0; i < 0x31; i++) {
        if (field_0x38[i].z > 0.0f) {
            field_0x284[i] = true;
        } else {
            field_0x284[i] = false;
        }
    }
}

void Player_804A6F38::fn_8024B2BC(JGeometry::TVec3f* out) {
    field_0xbf0->fn_8026947C(out);
}

void Player_804A6F38::fn_8024B298(s32 index, JGeometry::TVec3f* out) {
    field_0xbf0->fn_8026942C(index, out);
}

void Player_804A6F38::fn_8024B274() {
    field_0xbf0->fn_80269420();
}

bool Player_804A6F38::fn_8024B24C() {
    return field_0xbf0->isField98Or99Set();
}

// Enters state 0x49 with frames `frames` (field_0x1178): ends the 'FORM'
// actor (serial field_0x1230) or the held flag 0x121F, makes the group
// turn-based (fn_80248004(4, false)) and deletes the emitter at 0x1388.
bool Player_804A6F38::fn_8024B108(s32 frames) {
    if (fn_80270C88()) {
        return false;
    }
    if (fn_80251AEC(0x49)) {
        field_0x1178 = frames;
        ActorBase_804A5770* form = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x1230);
        if (form != NULL) {
            form->mAlive = false;
            field_0x1230 = -1;
        } else if (field_0x121f == true) {
            field_0x121f = false;
            if (field_0x11f0 < 5) {
                field_0x11f0 = 5;
            }
        }
        fn_80248004(4, false);
        if (field_0x1388 != NULL) {
            JPAEmitterManager* manager = getEffectSystem_8013C824()->mEmitterManager;
            manager->forceDeleteEmitter(field_0x1388);
            field_0x1388 = NULL;
        }
        return true;
    }
    return false;
}

// Leaves state 0x49 for state 0 (requestState) unless carried.
bool Player_804A6F38::fn_8024AFF8() {
    if (field_0xce8 == true) {
        return false;
    }
    if (field_0x1240 == 0x49) {
        field_0x124d = 0;
        return fn_80251AEC(0);
    }
    return false;
}

// FIXME: 99.9 %. Register allocation only (the group loops' temporaries; the
// acting-player copy of field_0x4 is r26 vs r30), plus the literal pool
// (@NNN vs lbl_80540044) until the unit's literals are in order.
// The player's input each frame (update A, fn_80253E6C): the 'FORM' call
// of a group leader and the four direction picks (fn_80248004(dir, true))
// on the pad buttons or the C stick.
void Player_804A6F38::fn_802490A8() {
    if (field_0x1220 > 0) {
        return;
    }
    s32 player = field_0x4;
    if (fn_802486A0() == 4) {
        player = fn_8022EA0C(field_0x4);
    }
    if (fn_80230188(player)) {
        return;
    }
    if (fn_80236B50(player)) {
        return;
    }
    if (getPlayerPos_8023E724(player)->z > 0.0f) {
        return;
    }
    if (fn_8023EA28(player)) {
        return;
    }
    for (s32 i = 0; i < 4; i++) {
        if (fn_8022E438(field_0x4, i)) {
            if (fn_8023E98C(i) && !fn_8022E438(i, fn_8023EC54(i))) {
                return;
            }
            if (fn_8022DEAC(i) && fn_802486A0() != 4) {
                return;
            }
            if (fn_8022D930(i)) {
                return;
            }
            if (fn_8022D894(i)) {
                return;
            }
            if (fn_8022D75C(i)) {
                return;
            }
            if (fn_80235274(i)) {
                return;
            }
            if (fn_80234CF8(i)) {
                return;
            }
            if (fn_80234C5C(i)) {
                return;
            }
            if (fn_80235744(i)) {
                return;
            }
            if (fn_802353AC(i)) {
                return;
            }
            if (fn_80233EDC(i)) {
                return;
            }
            if (fn_80234E30(i)) {
                return;
            }
        }
    }
    if (gGameRoot_8053AAF8->field_0x7e && field_0xcfe == 1) {
        return;
    }
    if (fn_8022AF5C()) {
        return;
    }
    if (field_0x121e == 1) {
        return;
    }
    JUTGamePad* pad = JUTGamePad::getGamePad(getPlayerInputSlot_8030AAF4(field_0x11ec));
    if (field_0xd60 < 0 && fn_80248970() >= 2 &&
        lbl_8053AD30->fn_80309D6C(getPlayerInputSlot_8030AAF4(field_0x11ec), 0x40)) {
        if (fn_802486A0() == 5) {
            if (!gGameRoot_8053AAF8->field_0x7e) {
                gGameRoot_8053AAF8->fn_80134FE0(1);
            }
            field_0x1230 = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_FORM, field_0x3b4, &mPos, field_0x4 & 3, -1, -1);
        } else {
            fn_80248004(5, true);
        }
        return;
    }
    if (!viewLayerFlag(field_0x3b4) && fn_80246B10(0x40)) {
        if (fn_80246D1C(8)) {
            if (!fn_80246B10(1) && !fn_80246B10(2)) {
                fn_80248004(2, true);
                return;
            }
        }
        if (fn_80246D1C(1)) {
            if (!fn_80246B10(8) && !fn_80246B10(4)) {
                fn_80248004(3, true);
                return;
            }
        }
        if (fn_80246D1C(4)) {
            if (!fn_80246B10(1) && !fn_80246B10(2)) {
                fn_80248004(1, true);
                return;
            }
        }
        if (fn_80246D1C(2)) {
            if (!fn_80246B10(8) && !fn_80246B10(4)) {
                fn_80248004(0, true);
                return;
            }
        }
    }
    bool busy = false;
    for (s32 i = 0; i < 4; i++) {
        if (fn_8022E438(field_0x4, i) && fn_8022DEAC(i)) {
            busy = true;
            break;
        }
    }
    if (lbl_8053AD30->fn_8030A308(getPlayerInputSlot_8030AAF4(field_0x11ec))) {
        return;
    }
    if (pad == NULL) {
        return;
    }
    if (busy) {
        return;
    }
    if (!(pad->getSubStickValue() > 0.5f)) {
        return;
    }
    s32 angle = pad->getSubStickAngle();
    if (angle < -0x7000 || angle >= 0x7000) {
        fn_80248004(2, true);
    } else if (angle >= -0x5000 && angle < -0x3000) {
        fn_80248004(3, true);
    } else if (angle >= -0x1000 && angle < 0x1000) {
        fn_80248004(1, true);
    } else if (angle >= 0x3000 && angle < 0x5000) {
        fn_80248004(0, true);
    }
}

void Player_804A6F38::fn_8024909C() {
    field_0x121f = 1;
}

// Clears field_0x121f (set by fn_8024909C; update B skips while it is set)
// and holds field_0x11f0 at 5 or more.
void Player_804A6F38::fn_80249070() {
    if (field_0x121f == 1) {
        field_0x121f = 0;
        if (field_0x11f0 < 5) {
            field_0x11f0 = 5;
        }
    }
}

bool Player_804A6F38::fn_80249068() {
    return field_0x121f;
}

// Ends the 'FORM' actor of serial field_0x1230 (fn_802490A8 spawns it), or
// without one clear121F_80249070.
void Player_804A6F38::fn_80248FF0() {
    ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x1230);
    if (actor != NULL) {
        actor->mAlive = false;
        field_0x1230 = -1;
    } else {
        fn_80249070();
    }
}

// Sets field_0x1214 to `flag` (state 3 requested when set, field_0x1215
// cleared when not), unless field_0x121e, field_0xbac > 0 or, for false,
// fn_8022AF5C; not while a player this one can reach (fn_8022E438) is in
// fn_8023EFEC (for true) or fn_8023EA28. The original tests `flag != true`
// first, which makes the later `flag == true` branch dead; written as is.
void Player_804A6F38::fn_80248E94(bool flag) {
    if (flag != true && field_0x121e != 1 && field_0xbac <= 0 && (flag || !fn_8022AF5C())) {
        if (flag == true) {
            for (s32 i = 0; i < 4; i++) {
                if (fn_8022E438(field_0x4, i) && fn_8023EFEC(i)) {
                    return;
                }
            }
        }
        for (s32 i = 0; i < 4; i++) {
            if (fn_8022E438(field_0x4, i) && fn_8023EA28(i)) {
                return;
            }
        }
        field_0x1214 = flag;
        if (!field_0x1214) {
            field_0x1215 = false;
        } else {
            fn_80251AEC(3);
        }
        field_0x1217 = false;
    }
}

void Player_804A6F38::fn_80248DD8() {
    field_0x1214 = true;
    if (field_0xd60 >= 0) {
        field_0x1215 = true;
        const JGeometry::TVec3f* pos = getPlayerPos_8023E724(field_0xd60);
        mPos.x = pos->x;
        mPos.y = pos->y;
        mPos.z = pos->z;
        if (fn_80251BFC(3)) {
            field_0x1244 = 3;
            field_0x124c = true;
            if (field_0xd60 < 0 && field_0xcfe == 1 && field_0x1244 != 0) {
                field_0xcfe = false;
            }
        }
    }
    field_0x1224 = 0;
    field_0x1217 = false;
}

void Player_804A6F38::fn_80248DCC() {
    field_0x1238 = 0xb4;
}

void Player_804A6F38::fn_80248DB8() {
    field_0x1217 = 0;
    field_0x1218 = 3;
}

void Player_804A6F38::fn_80248DAC() {
    field_0x121e = 0;
}

// As fn_80248A24, after first stepping the probe up to tile 0x3E
// (fn_802265FC; false when it leaves the view box first); also clears
// field_0x444.
// FIXME: 99.85. Only the literals 8.0f / 24.0f (@NNN vs lbl_80540050 /
// lbl_80540054 until the pool is in order).
bool Player_804A6F38::fn_80248B9C() {
    JGeometry::TVec2f pos;
    pos.x = mPos.x;
    pos.y = mPos.y;
    bool clamped = false;
    do {
        pos.y += 8.0f;
        if (!inViewBox(pos, field_0x3b4)) {
            return false;
        }
    } while (fn_802265FC((const JGeometry::TVec3f*)&pos, field_0x3b4) != 0x3e);
    do {
        pos.y += 8.0f;
        if (!inViewBox(pos, field_0x3b4)) {
            pos.y = layerBox(field_0x3b4)->f.y - 24.0f;
            clamped = true;
            break;
        }
    } while (fn_80226998((const JGeometry::TVec3f*)&pos, field_0x3b4) != 0x231);
    pos.y += 24.0f;
    if (fn_8026E5F8(&pos)) {
        field_0x43f = true;
        if (clamped == true) {
            field_0x440 = true;
        }
        field_0x444 = 0.0f;
        return true;
    }
    return false;
}

// Steps a probe up (y + 8) from mPos until it is on tile 0x231 (fn_80226998)
// or leaves the view box (then 24 inside its far edge, and field_0x440 is
// set too); 24 further on, fn_8026E5F8 there sets field_0x43f.
// FIXME: 99.84. Only the literals 8.0f / 24.0f (@NNN vs lbl_80540050 /
// lbl_80540054 until the pool is in order).
bool Player_804A6F38::fn_80248A24() {
    JGeometry::TVec2f pos;
    pos.x = mPos.x;
    pos.y = mPos.y;
    bool clamped = false;
    do {
        pos.y += 8.0f;
        if (!inViewBox(pos, field_0x3b4)) {
            pos.y = layerBox(field_0x3b4)->f.y - 24.0f;
            clamped = true;
            break;
        }
        fn_802265FC((const JGeometry::TVec3f*)&pos, field_0x3b4);
    } while (fn_80226998((const JGeometry::TVec3f*)&pos, field_0x3b4) != 0x231);
    pos.y += 24.0f;
    if (fn_8026E5F8(&pos)) {
        field_0x43f = true;
        if (clamped == true) {
            field_0x440 = true;
        }
        return true;
    }
    return false;
}

// The size of this player's group: itself, its leader (field_0xd60) and
// every active follower (fn_8022F514) whose leader (fn_8022F3DC) is that
// leader, or this player when it leads.
// `leader` is declared first so that it gets the highest register (r30) as in
// the original (declared in its branch it takes r28). fn_802487FC (unwritten)
// inlines this function after fn_802486A0; its direct copy zeroes count and
// index li / li in the original, li / mr in ours ("different" 94.9 %, six
// shapes).
s32 Player_804A6F38::fn_80248970() const {
    s32 leader;
    s32 count = 0;
    for (s32 i = 0; i < 4; i++) {
        if (i == field_0x4) {
            count++;
        } else if (fn_8022F514(i)) {
            leader = field_0xd60;
            if (leader >= 0) {
                if (leader == fn_8022F3DC(i)) {
                    count++;
                }
            } else {
                s32 self = field_0x4;
                if (self == fn_8022F3DC(i)) {
                    count++;
                }
            }
        } else if (i == field_0xd60) {
            count++;
        }
    }
    return count;
}

// The number of group places in use: 1 in group mode 4 (turn-based), else
// the group's size (fn_80248970).
// FIXME: 94.95 %. Registers only: the original zeroes the loop's count and
// index with two li (r29, r30), ours with li / mr (r28, r29).
s32 Player_804A6F38::fn_802487FC() const {
    if (fn_802486A0() == 4) {
        return 1;
    }
    return fn_80248970();
}

// The place of this player in its partner's line (1 for the first follower),
// or 0 without a partner. fn_80254744 inlines it and reloads both fields on
// every pass; this const copy keeps them in registers.
s32 Player_804A6F38::fn_80248778() const {
    if (field_0xd60 < 0) {
        return 0;
    }
    s32 count = 0;
    for (s32 i = 0; i < field_0x4; i++) {
        s32 partner = field_0xd60;
        if (partner == fn_8022F3DC(i)) {
            count++;
        }
    }
    return count + 1;
}

// The group's mode: 5 alone, else the leader's (fn_8022CAF8) or, leading,
// field_0x120c.
// FIXME: 97.7 %. Register allocation only: the original gives the count r29,
// the index r30 and the leader r28, ours r28 / r29 / r30. Const: the loop
// test's field_0x4 is hoisted out of the loop (r31), as in fn_80248778.
s32 Player_804A6F38::fn_802486A0() const {
    if (fn_80248970() <= 1) {
        return 5;
    }
    if (field_0xd60 >= 0) {
        return fn_8022CAF8(field_0xd60);
    }
    return field_0x120c;
}

// fn_8024BAEC, inlined: its loop shares one zero register between the index
// and the false store (li / mr) as every inlined copy does, while
// fn_8024BAEC's own body does not (li / li).
void Player_804A6F38::fn_80248604() {
    fn_8024BAEC();
}

// Sets the group's mode (0-3 the directions, 4 turn-based, 5 walk to the
// spots) for a group of two or more. A layer with viewLayerFlag forces 5
// (except 4); with 2 or 3 players taking part direction 2 becomes 1 and 3
// becomes 0. Leaving turn-based mode for this player's turn moves it to the
// acting player's position and trail. Then the reachable players' actors are
// reset (fn_8023105C / fn_802310F8), the old mode kept in field_0x1210 and,
// for a leader, the cue played (fn_8024F0E8 / fn_80230FD0) when `cue`.
// FIXME: 99.6 %. Registers only: the group loop's count / index / leader
// (r27 / r28 / r26 in the original) and the 0xCBC / 0xD1C addresses (r5 / r4).
void Player_804A6F38::fn_80248004(s32 mode, bool cue) {
    if (fn_80248970() <= 1) {
        return;
    }
    if (field_0x1216 == true) {
        return;
    }
    if (field_0xcfe == true) {
        return;
    }
    if (gGameRoot_8053AAF8->field_0x7e && field_0xcfe == true) {
        return;
    }
    if (viewLayerFlag(getPlayerLayer_8023DE58(fn_8022EA0C(field_0x4))) && mode != 4) {
        mode = 5;
    }
    if (gGameRoot_8053AAF8->fn_80138A30() == 2 || gGameRoot_8053AAF8->fn_80138A30() == 3) {
        if (mode == 2) {
            mode = 1;
        } else if (mode == 3) {
            mode = 0;
        }
    }
    if (cue == true && fn_8025F448() && !(field_0x120c == 4 && mode == 5)) {
        return;
    }
    if (!field_0x1214 && field_0x120c == mode) {
        return;
    }
    if (isPlayerBitSet(gGameRoot_8053AAF8, field_0x4) && mode != 4 && field_0x120c == 4 && fn_80240888() != 0) {
        JGeometry::TVec3f* pos = getPlayerPos_8023E724(fn_8022EA0C(field_0x4));
        s32 layer = getPlayerLayer_8023DE58(fn_8022EA0C(field_0x4));
        if (!field_0xbec->fn_8030B110(layer, *pos)) {
            s32 oldLayer = field_0x3b4;
            field_0x3b4 = getPlayerLayer_8023DE58(fn_8022EA0C(field_0x4));
            field_0xce0 = oldLayer;
            field_0xce4 = field_0x3b4;
            if (field_0x3b4 >= 8 && oldLayer < 8) {
                field_0xcb0.set(*fn_8023E32C(fn_8022EA0C(field_0x4)));
            }
            mPos.set(*getPlayerPos_8023E724(fn_8022EA0C(field_0x4)));
            field_0x2c.set(mPos);
            field_0xcbc = mPos;
            field_0xd1c = field_0xcbc;
        }
        for (s32 i = 0; i < 0x31; i++) {
            field_0x38[i].set(*fn_8023E4E4(fn_8022EA0C(field_0x4), 0));
            field_0x284[i] = fn_8023E45C(fn_8022EA0C(field_0x4), 0);
        }
        if (!getGameView_802D6A9C()->fn_802DCD0C()) {
            field_0x1216 = true;
        }
    }
    for (s32 i = 0; i < 4; i++) {
        if (fn_8022E438(field_0x4, i) && fn_8023ED8C(i) &&
            (fn_8022F478(i) || (field_0x120c == 4 && mode != 4))) {
            fn_8023105C(fn_8023ECF0(i));
            fn_802310F8(i);
        }
    }
    field_0x1210 = field_0x120c;
    field_0x120c = mode;
    field_0x1224 = 0;
    if (field_0x120c != 4) {
        fn_802382D0(field_0x4, 0);
    }
    if (isPlayerBitSet(gGameRoot_8053AAF8, field_0x4) && cue == true) {
        field_0x121d = 1;
    }
    if (field_0xd60 >= 0) {
        return;
    }
    if (mode == 4) {
        if (cue == true) {
            fn_8024F0E8(0x10010);
        }
        field_0x123c = 1;
        return;
    }
    fn_80248604();
    if (field_0x1210 == 4) {
        for (s32 i = 0; i < 4; i++) {
            if (fn_8022E438(field_0x4, i)) {
                if (cue == true) {
                    fn_80230FD0(i, 0x10011);
                }
                if (fn_8022F514(i)) {
                    fn_80238244(i);
                }
            }
        }
        return;
    }
    if (cue == true) {
        if (mode != 5) {
            fn_8024F0E8(0x20037);
        }
        for (s32 i = 0; i < 4; i++) {
            if (mode == 0) {
                fn_80230FD0(i, 0x10018);
            } else if (mode == 1) {
                fn_80230FD0(i, 0x10019);
            } else if (mode == 2) {
                fn_80230FD0(i, 0x10017);
            } else if (mode == 3) {
                fn_80230FD0(i, 0x1001A);
            }
        }
    }
}

void Player_804A6F38::fn_80247F58() {
    s32 team = field_0xd60;
    if (team >= 0 && fn_80236B50(team)) {
        if (fn_80251BFC(0x33)) {
            // positions are read as TVec2f (x, y) by the callee
            fn_80272160(0, (const JGeometry::TVec2f*)&mPos, 0x12);
        }
    } else if (fn_80239D38() && !field_0xd34 && fn_80251BFC(0x33)) {
        fn_80272160(0, (const JGeometry::TVec2f*)&mPos, 0x12);
    }
}

void Player_804A6F38::fn_80247F0C() {
    if (field_0xc9c > 0) {
        field_0xc9c--;
        return;
    }
    if (field_0xc84.z < 1.0f) {
        field_0xc84.z += 0.05f;
        if (field_0xc84.z >= 1.0f) {
            field_0xc84.z = 1.0f;
        }
    }
}

// Starts state 0xB with the timer field_0x48c (0x2D0 with GameRoot 0x7F,
// else 0x168): with manager flag 0x71 it only sets field_0xbc0 = 0x3C, and
// any of the flags 0x6C-0x72 refuses; already in state 0xB the timer
// restarts. Otherwise when fn_80251BFC allows state 0xB: turn-based group
// mode (4) is left first (fn_80248004(5, true)) and still refuses; a
// leader requests the state (fn_80251AEC), with fn_8022AD88 when the timer
// had run out (field_0x490).
// FIXME: 99.4 %. Registers only (the inlined group loops' count / index /
// leader).
bool Player_804A6F38::fn_80247C0C() {
    if (fn_8022AF5C()) {
        field_0xbc0 = 0x3c;
        return true;
    }
    if (fn_8022ADFC()) {
        return false;
    }
    if (field_0x1240 == 0xb) {
        if (gGameRoot_8053AAF8->field_0x7f) {
            field_0x48c = 0x2d0;
        } else {
            field_0x48c = 0x168;
        }
        field_0xbc0 = 0x3c;
        return true;
    }
    if (!fn_80251BFC(0xb)) {
        return false;
    }
    if (fn_802486A0() == 4) {
        fn_80248004(5, true);
    }
    if (fn_802486A0() == 4) {
        return false;
    }
    if (field_0x48c == 0) {
        field_0x490 = 1;
    }
    if (gGameRoot_8053AAF8->field_0x7f) {
        field_0x48c = 0x2d0;
    } else {
        field_0x48c = 0x168;
    }
    if (field_0xd60 < 0) {
        fn_80251AEC(0xb);
        if (field_0x490 == 1) {
            fn_8022AD88();
        }
    }
    return true;
}

// The leader's field_0x48c (fn_8023C36C) for a follower, else its own.
s32 Player_804A6F38::fn_80247BD4() {
    s32 team = field_0xd60;
    if (team >= 0) {
        return fn_8023C36C(team);
    }
    return field_0x48c;
}

// The leader's field_0xbc0 (fn_8023BEA8) for a follower, else its own.
s32 Player_804A6F38::fn_80247B9C() {
    s32 team = field_0xd60;
    if (team >= 0) {
        return fn_8023BEA8(team);
    }
    return field_0xbc0;
}

// Per frame: the field_0x48c countdown (state 0xB while it runs, back to 0
// with effect 0x503 when it ends), shared by a leader's followers; in the
// demo (fn_8022AF5C) only state 0xB and field_0xbc0. With music 0x1000019,
// fn_8022AD64 once no player's countdown runs.
// FIXME: 99.8 %. Registers only: the inlined group loops' index / leader
// temporaries.
void Player_804A6F38::fn_8024762C() {
    if (fn_8022AF5C()) {
        if (!field_0xce8 && field_0x1240 != 0xb && fn_80251AEC(0xb)) {
            field_0xbd4 = 1;
        }
        if (field_0xd60 < 0 && field_0xbc0 > 0) {
            field_0xbc0--;
        }
        return;
    }
    if (field_0x454) {
        return;
    }
    if (fn_80240B84()) {
        return;
    }
    if (field_0xce8) {
        return;
    }
    if (field_0xd60 < 0) {
        if (field_0x48c > 0) {
            field_0x48c--;
            if (field_0x491 == 1) {
                field_0x48c = 1;
            }
        }
        if (field_0x48c == 0) {
            field_0xbc0 = 0;
        } else if (field_0xbc0 > 0) {
            field_0xbc0--;
        }
    } else {
        if (fn_80247BD4() > 0 && field_0x48c == 0) {
            field_0x490 = 1;
        }
        field_0x48c = fn_80247BD4();
        field_0xbc0 = fn_80247B9C();
    }
    if (fn_80247BD4() > 0 && field_0x1240 != 0xb) {
        fn_80251AEC(0xb);
    } else if (fn_80247BD4() == 0 && field_0x1240 == 0xb) {
        if (fn_80251AEC(0) && !field_0xbd4) {
            spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x503, 1, 1, NULL, NULL, true, 1.0f);
        }
    }
    if (isPlayerBitSet(gGameRoot_8053AAF8, field_0x4) && gAudioMgr_8053AF80->field_0x58 == 0x1000019) {
        for (s32 i = 0; i < 4; i++) {
            if (fn_8023C36C(i) > 0) {
                return;
            }
        }
        fn_8022AD64();
    }
}

bool Player_804A6F38::fn_80247594(s32 arg) {
    s32 team = field_0xd60;
    if (team >= 0) {
        return fn_80237488(team, arg);
    }
    if (field_0x1201 == 1) {
        return true;
    }
    if (field_0xc08 >= arg) {
        field_0xc08 -= arg;
        field_0x1201 = 1;
        return true;
    }
    if (field_0xc08 > 0) {
        field_0xc08 = 0;
        field_0x1201 = 1;
        return true;
    }
    field_0x1202 = true;
    return false;
}

bool Player_804A6F38::fn_80247564() {
    return lbl_8053AD30->fn_8030A3B0(getPlayerInputSlot_8030AAF4(field_0x11ec));
}

u8 Player_804A6F38::fn_80247534() {
    return lbl_8053AD30->fn_8030A308(getPlayerInputSlot_8030AAF4(field_0x11ec));
}

// FIXME: 99.5 %. Register allocation only (the group loops' count / index /
// leader temporaries, as in fn_802486A0).
// The pad axis fn_80309FCC of the player's slot behind teamCheck, else 0.
f32 Player_804A6F38::fn_80247330() {
    if (teamCheck(this) == false) {
        return 0.0f;
    }
    return lbl_8053AD30->fn_80309FCC(getPlayerInputSlot_8030AAF4(field_0x11ec));
}

// FIXME: 99.5 %. Register allocation only (the group loops' count / index /
// leader temporaries, as in fn_802486A0).
// The pad axis fn_80309F20 of the player's slot behind teamCheck, else 0.
f32 Player_804A6F38::fn_8024712C() {
    if (teamCheck(this) == false) {
        return 0.0f;
    }
    return lbl_8053AD30->fn_80309F20(getPlayerInputSlot_8030AAF4(field_0x11ec));
}

// FIXME: 99.5 %. Register allocation only (the group loops' count / index /
// leader temporaries, as in fn_802486A0).
f32 Player_804A6F38::fn_80246F28() {
    // `== false`: the check's bool is materialised and tested (li 0 / li 1,
    // clrlwi.; bne to the call), as with `x == true` (docs/RULES_CARD.md).
    if (teamCheck(this) == false) {
        return 0.0f;
    }
    return lbl_8053AD30->fn_80309D90(getPlayerInputSlot_8030AAF4(field_0x11ec));
}

// FIXME: 99.5 %. Register allocation only (the group loops' count / index /
// leader temporaries, as in fn_802486A0).
u32 Player_804A6F38::fn_80246D1C(u32 mask) {
    if (teamCheck(this) == false) {
        return 0;
    }
    return lbl_8053AD30->fn_80309D6C(getPlayerInputSlot_8030AAF4(field_0x11ec), mask);
}

// FIXME: 99.5 %. Register allocation only (the group loops' count / index /
// leader temporaries, as in fn_802486A0).
u32 Player_804A6F38::fn_80246B10(u32 mask) {
    if (teamCheck(this) == false) {
        return 0;
    }
    return lbl_8053AD30->fn_80309D24(getPlayerInputSlot_8030AAF4(field_0x11ec), mask);
}

// FIXME: 99.5 %. Register allocation only (the group loops' count / index /
// leader temporaries, as in fn_802486A0).
s32 Player_804A6F38::fn_80246904(u32 mask) {
    if (teamCheck(this) == false) {
        return 0;
    }
    return lbl_8053AD30->fn_80309A78(getPlayerInputSlot_8030AAF4(field_0x11ec), mask);
}

// FIXME: 99.5 %. Register allocation only (the group loops' count / index /
// leader temporaries, as in fn_802486A0).
bool Player_804A6F38::fn_802466B4() {
    if (teamCheck(this) == false) {
        return false;
    }
    if (fn_8022B244()) {
        return false;
    }
    if (field_0x456) {
        return false;
    }
    if (field_0x11f0 > 0) {
        return false;
    }
    return lbl_8053AD30->fn_80309D6C(getPlayerInputSlot_8030AAF4(field_0x11ec), 0x200) != 0;
}

// FIXME: 99.5 %. Register allocation only (the group loops' count / index /
// leader temporaries, as in fn_802486A0).
bool Player_804A6F38::fn_80246464() {
    if (teamCheck(this) == false) {
        return false;
    }
    if (fn_8022B244()) {
        return false;
    }
    if (field_0x456) {
        return false;
    }
    if (field_0x11f0 > 0) {
        return false;
    }
    return lbl_8053AD30->fn_80309D24(getPlayerInputSlot_8030AAF4(field_0x11ec), 0x200) != 0;
}

// FIXME: 99.5 %. Register allocation only (the group loops' count / index /
// leader temporaries, as in fn_802486A0).
bool Player_804A6F38::fn_80246228() {
    if (teamCheck(this) == false) {
        return false;
    }
    if (field_0x456) {
        return false;
    }
    if (field_0x11f0 > 0) {
        return false;
    }
    return lbl_8053AD30->fn_80309D6C(getPlayerInputSlot_8030AAF4(field_0x11ec), 0x100) != 0;
}

// FIXME: 99.5 %. Register allocation only (the group loops' count / index /
// leader temporaries, as in fn_802486A0).
bool Player_804A6F38::fn_80245FEC() {
    if (teamCheck(this) == false) {
        return false;
    }
    if (field_0x456) {
        return false;
    }
    if (field_0x11f0 > 0) {
        return false;
    }
    return lbl_8053AD30->fn_80309D24(getPlayerInputSlot_8030AAF4(field_0x11ec), 0x100) != 0;
}

// FIXME: 99.5 %. Register allocation only (the group loops' count / index /
// leader temporaries, as in fn_802486A0).
bool Player_804A6F38::fn_80245DB0() {
    if (teamCheck(this) == false) {
        return false;
    }
    if (field_0x456) {
        return false;
    }
    if (field_0x11f0 > 0) {
        return false;
    }
    return lbl_8053AD30->fn_80309D6C(getPlayerInputSlot_8030AAF4(field_0x11ec), 0x100) != 0;
}

// FIXME: 99.5 %. Register allocation only (the group loops' count / index /
// leader temporaries, as in fn_802486A0).
bool Player_804A6F38::fn_80245B74() {
    if (teamCheck(this) == false) {
        return false;
    }
    if (field_0x456) {
        return false;
    }
    if (field_0x11f0 > 0) {
        return false;
    }
    return lbl_8053AD30->fn_80309A78(getPlayerInputSlot_8030AAF4(field_0x11ec), 0x100) != 0;
}

// FIXME: 99.5 %. Register allocation only (the group loops' count / index /
// leader temporaries, as in fn_802486A0).
bool Player_804A6F38::fn_80245938() {
    if (teamCheck(this) == false) {
        return false;
    }
    if (field_0x456) {
        return false;
    }
    if (field_0x11f0 > 0) {
        return false;
    }
    return lbl_8053AD30->fn_80309D6C(getPlayerInputSlot_8030AAF4(field_0x11ec), 0x20) != 0;
}

// FIXME: 99.5 %. Register allocation only (the group loops' count / index /
// leader temporaries, as in fn_802486A0).
bool Player_804A6F38::fn_802456FC() {
    if (teamCheck(this) == false) {
        return false;
    }
    if (field_0x456) {
        return false;
    }
    if (field_0x11f0 > 0) {
        return false;
    }
    return lbl_8053AD30->fn_80309D24(getPlayerInputSlot_8030AAF4(field_0x11ec), 0x20) != 0;
}

// FIXME: 99.5 %. Register allocation only (the group loops' count / index /
// leader temporaries, as in fn_802486A0).
bool Player_804A6F38::fn_802454C0() {
    if (teamCheck(this) == false) {
        return false;
    }
    if (field_0x456) {
        return false;
    }
    if (field_0x11f0 > 0) {
        return false;
    }
    return lbl_8053AD30->fn_80309A54(getPlayerInputSlot_8030AAF4(field_0x11ec), 0x20) != 0;
}

// FIXME: 99.5 %. Register allocation only (the group loops' count / index /
// leader temporaries, as in fn_802486A0).
bool Player_804A6F38::fn_80245238() {
    if (teamCheck(this) == false) {
        return false;
    }
    if (lbl_8053AD30->fn_80309D6C(getPlayerInputSlot_8030AAF4(field_0x11ec), 0x100)) {
        return true;
    }
    if (lbl_8053AD30->fn_80309D6C(getPlayerInputSlot_8030AAF4(field_0x11ec), 0x20)) {
        return true;
    }
    if (viewLayerFlag(field_0x3b4) && lbl_8053AD30->fn_80309D6C(getPlayerInputSlot_8030AAF4(field_0x11ec), 0x200)) {
        return true;
    }
    return false;
}

// FIXME: 99.5 %. Register allocation only (the group loops' count / index /
// leader temporaries, as in fn_802486A0).
bool Player_804A6F38::fn_80244FFC() {
    if (teamCheck(this) == false) {
        return false;
    }
    if (lbl_8053AD30->fn_80309D24(getPlayerInputSlot_8030AAF4(field_0x11ec), 0x100)) {
        return true;
    }
    return lbl_8053AD30->fn_80309D24(getPlayerInputSlot_8030AAF4(field_0x11ec), 0x20) != 0;
}

// FIXME: 99.9 %. Register allocation only (the group loops' count / index /
// leader temporaries, as in fn_802486A0).
// Whether the pad points along the facing 0x3DC.
bool Player_804A6F38::fn_80244630() {
    if (teamCheck(this) == false) {
        return false;
    }
    if ((field_0x3dc == 0 && fn_80247330() > 0.0f) ||
        (field_0x3dc == 1 && fn_80247330() < 0.0f) ||
        (field_0x3dc == 2 && fn_8024712C() < 0.0f) ||
        (field_0x3dc == 3 && fn_8024712C() > 0.0f)) {
        return true;
    }
    return false;
}

// FIXME: 99.9 %. Register allocation only (the group loops' count / index /
// leader temporaries, as in fn_802486A0).
// Whether the pad points against the facing 0x3DC.
bool Player_804A6F38::fn_80243C64() {
    if (teamCheck(this) == false) {
        return false;
    }
    if ((field_0x3dc == 0 && fn_80247330() < 0.0f) ||
        (field_0x3dc == 1 && fn_80247330() > 0.0f) ||
        (field_0x3dc == 2 && fn_8024712C() > 0.0f) ||
        (field_0x3dc == 3 && fn_8024712C() < 0.0f)) {
        return true;
    }
    return false;
}

// FIXME: 99.9 %. Register allocation only (the group loops' count / index /
// leader temporaries, as in fn_802486A0).
// Whether the pad points sideways for the facing 0x3DC on a flagged layer
// (viewLayerFlag): x right while facing 0 or 1 turns the player to 3.
bool Player_804A6F38::fn_80243234() {
    if (!viewLayerFlag(field_0x3b4)) {
        return false;
    }
    if ((field_0x3dc == 0 && fn_8024712C() > 0.0f) || (field_0x3dc == 1 && fn_8024712C() > 0.0f)) {
        field_0x3e8 = 3;
        field_0x3dc = 3;
        return true;
    }
    if ((field_0x3dc == 0 && fn_80247330() > 0.0f) ||
        (field_0x3dc == 1 && fn_80247330() < 0.0f) ||
        (field_0x3dc == 3 && fn_8024712C() > 0.0f)) {
        return true;
    }
    return false;
}

s32 Player_804A6F38::fn_8024322C() {
    return field_0xd08;
}

bool Player_804A6F38::fn_802431B8() {
    for (s32 i = 0; i < 4; i++) {
        if (fn_8023697C(i) >= 0) {
            if (fn_8022E438(fn_8023697C(i), field_0x4)) {
                return true;
            }
        }
    }
    return false;
}

bool Player_804A6F38::fn_80243194(s32 other) {
    return fn_8022E438(field_0x4, other);
}

// Conveyor tiles: the tile under mPos (fn_80226850) pushes field_0x414 by
// 0.05 (tiles 0xB4-0xB7, speed up to 0.5) or 0.15 (0x95-0x98, up to 2.0)
// along +x / -x / +y / -y; off them the push slows by 0.05 a frame. The
// push is added to mPos.
void Player_804A6F38::fn_80242DAC() {
    u16 tile = fn_80226850(&mPos, field_0x3b4);
    if (tile == 0xb7) {
        field_0x414.x += 0.05f;
    } else if (tile == 0xb6) {
        field_0x414.x -= 0.05f;
    } else if (tile == 0xb5) {
        field_0x414.y += 0.05f;
    } else if (tile == 0xb4) {
        field_0x414.y -= 0.05f;
    } else if (tile == 0x98) {
        field_0x414.x += 0.15f;
    } else if (tile == 0x97) {
        field_0x414.x -= 0.15f;
    } else if (tile == 0x96) {
        field_0x414.y += 0.15f;
    } else if (tile == 0x95) {
        field_0x414.y -= 0.15f;
    } else {
        f32 speed = PSVECMag(&field_0x414);
        speed -= 0.05f;
        if (speed <= 0.0f) {
            field_0x414.set(0.0f, 0.0f, 0.0f);
        } else {
            field_0x414.normalize();
            field_0x414.scale(speed);
        }
        mPos += field_0x414;
        return;
    }
    f32 speed = PSVECMag(&field_0x414);
    if (tile == 0x98 || tile == 0x97 || tile == 0x96 || tile == 0x95) {
        if (speed >= 2.0f) {
            field_0x414.normalize();
            field_0x414.scale(2.0f);
        }
    } else if (speed >= 0.5f) {
        field_0x414.normalize();
        field_0x414.scale(0.5f);
    }
    mPos += field_0x414;
}

// Clears field_0x43e (set to 1) for a player standing or walking (states
// 0, 2-4, 8, 0x4A, 0x4B or 0x12, not frames 0x1BF-0x1C5): at once when it
// leads, else once it can jump (fn_8026E5F8) to its leader's 0x324.
void Player_804A6F38::fn_80242CB0() {
    if (field_0x43e != 1) {
        return;
    }
    s32 state = field_0x1240;
    bool idle = state == 0 || state == 2 || state == 3 || state == 4 || state == 8 || state == 0x4a ||
                state == 0x4b;
    if (!idle && state != 0x12) {
        return;
    }
    bool busy = state == 1 || (field_0x3c4 >= 0x1bf && field_0x3c4 < 0x1c6);
    if (busy) {
        return;
    }
    if (field_0xd60 < 0) {
        field_0x43e = 0;
        return;
    }
    const JGeometry::TVec2f* target = fn_80238430(field_0xd60);
    JGeometry::TVec2f pos;
    pos.x = target->x;
    pos.y = target->y;
    if (fn_8026E5F8(&pos)) {
        field_0x43e = 0;
    }
}

// Counts field_0xbd8 down (fn_803037D4(index, 2) every 60 frames while
// fn_8030381C allows it); a leader with field_0xbdc whose count ran out on
// a layer of 8 or more calls fn_8028FBA0; below layer 8 the count is dropped.
void Player_804A6F38::fn_80242BC0() {
    if (field_0x454) {
        return;
    }
    if (field_0xbd8 > 0) {
        field_0xbd8--;
        if (field_0xbd8 > 0 && field_0xbd8 % 60 == 0 && fn_8030381C(field_0x4)) {
            fn_803037D4(field_0x4, 2);
        }
    }
    if (field_0xd60 < 0 && field_0xbdc && field_0xbd8 <= 0 && field_0x3b4 >= 8) {
        fn_8028FBA0();
    }
    if (field_0x3b4 < 8 && field_0xbd8 > 0) {
        field_0xbd8 = 0;
    }
}

// In states 0x4A / 0x4B (fn_802412A4), unless field_0x454 is set: counts
// field_0xbe4 down, with fn_803037D4(field_0x4, 2) every 60 frames for a
// player acting on its own (fn_80240670 false) when fn_8030381C allows it;
// once it has run out with field_0xbe8 set on layer 8 or more,
// fn_8028FC2C; below layer 8 the count is cleared.
// FIXME: 99.6 %. Registers only: the inlined group loop's leader / self
// temporaries (r27 / r30 in the original, both r27 here).
void Player_804A6F38::fn_80242940() {
    if (field_0x454) {
        return;
    }
    if (!fn_802412A4()) {
        return;
    }
    if (field_0xbe4 > 0) {
        field_0xbe4--;
        if (!fn_80240670() && field_0xbe4 > 0 && field_0xbe4 % 60 == 0 && fn_8030381C(field_0x4)) {
            fn_803037D4(field_0x4, 2);
        }
    }
    if (field_0xbe8 && field_0xbe4 <= 0 && field_0x3b4 >= 8) {
        fn_8028FC2C();
    }
    if (field_0x3b4 < 8 && field_0xbe4 > 0) {
        field_0xbe4 = 0;
    }
}

// Requests state 0x4C (keeping `arg` in field_0xd8c, matched by
// fn_802426E0) for a player acting on its own and not fn_8025F448.
// FIXME: 99.51. Registers only (the inlined group loops' temporaries, as
// fn_8024C828).
bool Player_804A6F38::fn_80242704(s32 arg) {
    if (fn_80240670()) {
        return false;
    }
    if (fn_8025F448()) {
        return false;
    }
    if (fn_80251AEC(0x4c)) {
        field_0xd8c = arg;
        return true;
    }
    return false;
}

bool Player_804A6F38::fn_802426E0(s32 serial) {
    if (field_0xd8c != serial) {
        return false;
    }
    field_0xd90 = 1;
    return true;
}

// Requests state 0x4D with pos as its target (field_0x374).
bool Player_804A6F38::fn_80242624(JGeometry::TVec3f pos) {
    if (fn_80251AEC(0x4d)) {
        field_0x374.set(pos);
        return true;
    }
    return false;
}

// Runs fn_802C1778 on the key the player holds (fn_8025F500 with a 'KEY0'
// at field_0xd68), then fn_802C1674 and fn_802C1778 on every 'KEY0' actor;
// then field_0x3b0 = 3.
void Player_804A6F38::fn_80242560() {
    if (fn_8025F500() && field_0xd6c == ACTOR_KEY0) {
        ActorBase_804A5770* held = getActorManager_801F856C()->findActorBySerial_801F666C(field_0xd68);
        if (held != NULL) {
            static_cast<SmallKey_804A8CD8*>(held)->fn_802C1778();
        }
    }
    s32 serial = -1;
    while (true) {
        serial = getActorManager_801F856C()->fn_801F78F4(ACTOR_KEY0, serial);
        if (serial < 0) {
            break;
        }
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        SmallKey_804A8CD8* key = static_cast<SmallKey_804A8CD8*>(actor);
        if (key != NULL) {
            key->fn_802C1674();
            key->fn_802C1778();
        }
    }
    field_0x3b0 = 3;
}

// Brings the player back to the acting player (lbl_8053A190): through
// fn_80270948 when fn_80270A8C holds and fn_8025F448 does not; else, when it
// is on layer 8 or more or outside the visible box, onto that player's x / y
// on layer 0. Then the saved positions 0x2C4 / 0xCB0 / 0xD1C, 0x344 / 0x368
// cleared, field_0xd54 = 10, the trail reset (fn_80248604), the held key or
// the 'KEY0' at field_0xd88 moved along (fn_802C1D30), field_0x122c = 5.
// FIXME: 99.7 %. Registers only: the box test's x / y (f2 / f1 in the
// original, f1 / f2 here) and the 0xCB0 / 0xD1C addresses (r4 / r3).
void Player_804A6F38::fn_802422D0() {
    if (fn_80270A8C() && !fn_8025F448()) {
        fn_80270948(getPlayerPos_8023E724(lbl_8053A190));
    } else {
        JGeometry::TBox2f box;
        box = getGameView_802D6A9C()->mVisibleBox;
        if (field_0x3b4 >= 8 || !containsPoint(box, mPos)) {
            mPos.x = getPlayerPos_8023E724(lbl_8053A190)->x;
            mPos.y = getPlayerPos_8023E724(lbl_8053A190)->y;
            field_0x3b4 = 0;
        }
    }
    field_0x2c4 = mPos;
    field_0xcb0 = field_0x2c4;
    field_0xd1c = field_0xcb0;
    field_0x344.set(0.0f, 0.0f, 0.0f);
    field_0x368.set(0.0f, 0.0f, 0.0f);
    field_0xd54 = 10;
    fn_80248604();
    if (fn_8025F500() && field_0xd6c == ACTOR_KEY0) {
        ActorBase_804A5770* held = getActorManager_801F856C()->findActorBySerial_801F666C(field_0xd68);
        if (held != NULL) {
            static_cast<SmallKey_804A8CD8*>(held)->fn_802C1D30(JGeometry::TVec3f(mPos), field_0x3b4);
        }
    } else {
        s32 serial = field_0xd88;
        if (serial >= 0) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (actor != NULL && actor->mActorId == ACTOR_KEY0) {
                static_cast<SmallKey_804A8CD8*>(actor)->fn_802C1D30(JGeometry::TVec3f(mPos), field_0x3b4);
            }
        }
    }
    field_0x122c = 5;
}

const JGeometry::TVec2f* Player_804A6F38::fn_802422C4() {
    return &field_0xbf0->field_0x14;
}

// Deletes all of the player's emitters (0x1274-0x1394; 0x135C-0x1370 in the
// original's order).
void Player_804A6F38::fn_80241E64() {
    deleteEmitter(&field_0x1274);
    deleteEmitter(&field_0x128c);
    deleteEmitter(&field_0x12b0);
    deleteEmitter(&field_0x12b4);
    deleteEmitter(&field_0x12b8);
    deleteEmitter(&field_0x12bc);
    deleteEmitter(&field_0x12c4);
    deleteEmitter(&field_0x12c8);
    deleteEmitter(&field_0x12cc);
    deleteEmitter(&field_0x12e4);
    deleteEmitter(&field_0x1308);
    deleteEmitter(&field_0x132c);
    deleteEmitter(&field_0x1350);
    deleteEmitter(&field_0x1354);
    deleteEmitter(&field_0x1358);
    deleteEmitter(&field_0x136c);
    deleteEmitter(&field_0x1370);
    deleteEmitter(&field_0x135c);
    deleteEmitter(&field_0x1368);
    deleteEmitter(&field_0x1360);
    deleteEmitter(&field_0x1364);
    deleteEmitter(&field_0x1374);
    deleteEmitter(&field_0x1378);
    deleteEmitter(&field_0x137c);
    deleteEmitter(&field_0x1380);
    deleteEmitter(&field_0x1384);
    deleteEmitter(&field_0x1388);
    deleteEmitter(&field_0x138c);
    deleteEmitter(&field_0x1390);
    deleteEmitter(&field_0x1394);
}

bool Player_804A6F38::fn_80241E28(s32 layer, const JGeometry::TVec3f* pos) {
    return field_0xbec->fn_8030B110(layer, *pos);
}

bool Player_804A6F38::fn_80241DD0(s32 layer, const JGeometry::TVec3f* pos) {
    if (layer < 8) {
        return gGameRoot2_8053AB10->field_0x24->fn_8030C2A8(pos);
    }
    return field_0xbec->fn_8030B084(layer, *pos);
}

// Whether the corners of `box` are in view: any of them with `overlap`, else
// all four (the camera's fn_8030C2A8 below layer 8; field_0xbec's bounds,
// fn_8030AF10, from layer 8 on). The last corner's result is converted with
// `!= false` (neg / or / srwi in the original), the others are tested.
bool Player_804A6F38::fn_80241BA0(s32 layer, const JGeometry::TBox2f* box, bool overlap) {
    if (layer < 8) {
        if (overlap == true) {
            if (gGameRoot2_8053AB10->field_0x24->fn_8030C2A8(&JGeometry::TVec3f(box->i.x, box->i.y, 0.0f))) {
                return true;
            }
            if (gGameRoot2_8053AB10->field_0x24->fn_8030C2A8(&JGeometry::TVec3f(box->i.x, box->f.y, 0.0f))) {
                return true;
            }
            if (gGameRoot2_8053AB10->field_0x24->fn_8030C2A8(&JGeometry::TVec3f(box->f.x, box->i.y, 0.0f))) {
                return true;
            }
            return gGameRoot2_8053AB10->field_0x24->fn_8030C2A8(&JGeometry::TVec3f(box->f.x, box->f.y, 0.0f)) != false;
        }
        if (!gGameRoot2_8053AB10->field_0x24->fn_8030C2A8(&JGeometry::TVec3f(box->i.x, box->i.y, 0.0f))) {
            return false;
        }
        if (!gGameRoot2_8053AB10->field_0x24->fn_8030C2A8(&JGeometry::TVec3f(box->i.x, box->f.y, 0.0f))) {
            return false;
        }
        if (!gGameRoot2_8053AB10->field_0x24->fn_8030C2A8(&JGeometry::TVec3f(box->f.x, box->i.y, 0.0f))) {
            return false;
        }
        return gGameRoot2_8053AB10->field_0x24->fn_8030C2A8(&JGeometry::TVec3f(box->f.x, box->f.y, 0.0f)) != false;
    }
    return field_0xbec->fn_8030AF10(layer, &JGeometry::TBox2f(*box), overlap);
}

// True alone (fn_80248970 <= 1) or in group mode 4; else only when every
// other player this one can reach (fn_8022E438) has fn_802349EC (byte
// 0x1215 of that player, set on arriving at its spot).
// FIXME: 97.29. Registers and scheduling only: the first group loop's count /
// index are r29 / r30 in the original, r28 / r29 here (with the stw r28 save
// moved).
bool Player_804A6F38::fn_80241964() {
    if (fn_80248970() <= 1) {
        return true;
    }
    if (fn_802486A0() == 4) {
        return true;
    }
    for (s32 i = 0; i < 4; i++) {
        if (i != field_0x4 && fn_8022E438(field_0x4, i) && !fn_802349EC(i)) {
            return false;
        }
    }
    return true;
}

s32 Player_804A6F38::fn_80241920() {
    s32 team = field_0xd60;
    if (team >= 0) {
        return fn_80236228(team);
    }
    s32 value = field_0xc04;
    // An if, not `?:`: fn_8024E694's inlined copies branch (cmpwi / blt);
    // this copy is branchless either way.
    if (value >= 0) {
        return value;
    }
    return 0;
}

s32 Player_804A6F38::fn_802418E8() {
    s32 team = field_0xd60;
    if (team >= 0) {
        return fn_80236100(team);
    }
    return field_0xc04;
}

s32 Player_804A6F38::fn_8024189C() {
    s32 team = field_0xd60;
    if (team >= 0) {
        return fn_80237774(team);
    }
    if (field_0xc00) {
        return 0;
    }
    return field_0xbfc;
}

s32 Player_804A6F38::fn_80241864() {
    s32 team = field_0xd60;
    if (team >= 0) {
        return fn_802376D8(team);
    }
    return field_0xbfc;
}

void Player_804A6F38::fn_80241804(s32 value) {
    s32 team = field_0xd60;
    if (team >= 0) {
        fn_8023764C(team, value);
    } else if (value < 0) {
        field_0xbfc = 0;
    } else if (value >= field_0xbf8) {
        field_0xbfc = field_0xbf8;
    } else {
        field_0xbfc = value;
    }
}

s32 Player_804A6F38::fn_802417CC() {
    s32 team = field_0xd60;
    if (team >= 0) {
        return fn_80237810(team);
    }
    return field_0xbf8;
}

s32 Player_804A6F38::fn_80241794() {
    s32 team = field_0xd60;
    if (team >= 0) {
        return fn_802375B0(team);
    }
    return field_0xc08;
}

// The two early-outs under !field_0x7f are one condition (beq over a `b` to the epilogue).
void Player_804A6F38::fn_802416FC(s32 arg) {
    s32 team = field_0xd60;
    if (team >= 0) {
        fn_80237524(team, arg);
        return;
    }
    if (!gGameRoot_8053AAF8->field_0x7f && (field_0xbfc <= 0 || fn_80270A8C())) {
        return;
    }
    field_0xc08 += arg;
    if (field_0xc08 > 0x180) {
        field_0xc08 = 0x180;
    }
}

s32 Player_804A6F38::fn_802416C4() {
    s32 team = field_0xd60;
    if (team >= 0) {
        return fn_802373EC(team);
    }
    return field_0xc10;
}

bool Player_804A6F38::fn_80241618(s32 item, s32 level) {
    field_0xc10 = item;
    if (field_0xc10 == 0) {
        field_0xc14 = 0;
    } else {
        fn_8024157C(level);
    }
    if (field_0xc10 != 0) {
        lbl_8053AED0->field_0x255 |= (u8)(1 << field_0x4);
        if (!gGameRoot_8053AAF8->field_0x7f && field_0x464 < 0x3c) {
            field_0x464 = 0x3c;
        }
    }
    return true;
}

s32 Player_804A6F38::fn_802415CC() {
    s32 team = field_0xd60;
    if (team >= 0) {
        return fn_8023725C(team);
    }
    if (field_0xc10 == 0) {
        return 0;
    }
    return field_0xc14;
}

// Sets field_0xc14 to arg and this player's bit in lbl_8053AED0's
// field_0x255, unless field_0xc10 is 0 (or 2 with arg 2 or more).
bool Player_804A6F38::fn_8024157C(s32 arg) {
    if (field_0xc10 == 0 || (arg >= 2 && field_0xc10 == 2)) {
        return false;
    }
    field_0xc14 = arg;
    lbl_8053AED0->field_0x255 |= (u8)(1 << field_0x4);
    return true;
}

s32 Player_804A6F38::fn_80241544() {
    s32 team = field_0xd60;
    if (team >= 0) {
        return fn_80237060(team);
    }
    return field_0xc18;
}

bool Player_804A6F38::fn_80241514(s32 value) {
    field_0xc18 = value;
    if (!gGameRoot_8053AAF8->field_0x7f) {
        if (field_0x464 < 0x3c) {
            field_0x464 = 0x3c;
        }
    }
    return true;
}

bool Player_804A6F38::fn_802414DC() {
    s32 team = field_0xd60;
    if (team >= 0) {
        return fn_80236E3C(team);
    }
    return field_0xb7a;
}

void Player_804A6F38::fn_802414D0() {
    field_0xb7a = true;
}

bool Player_804A6F38::fn_80241498() {
    s32 team = field_0xd60;
    if (team >= 0) {
        return fn_80236D14(team);
    }
    return field_0xb7b;
}

void Player_804A6F38::fn_8024148C() {
    field_0xb7b = true;
}

bool Player_804A6F38::fn_80241478() {
    return field_0x1240 == 0xe;
}

JGeometry::TVec3f* Player_804A6F38::fn_8024145C() {
    if (field_0xd64 == 1) {
        return &field_0x2f4;
    }
    return &mPos;
}

void Player_804A6F38::fn_80241454(u8 value) {
    field_0xc80 = value;
}

bool Player_804A6F38::fn_802413F0() {
    return playerFlags_802413F0(this);
}

bool Player_804A6F38::fn_802413E8() {
    return field_0x454;
}

bool Player_804A6F38::fn_802413C8() {
    return field_0x1240 == 8 || field_0x1240 == 0x37;
}

bool Player_804A6F38::fn_80241380() {
    return field_0x1240 == 0 || field_0x1240 == 2 || field_0x1240 == 3 || field_0x1240 == 4 ||
           field_0x1240 == 8 || field_0x1240 == 0x4a || field_0x1240 == 0x4b;
}

bool Player_804A6F38::fn_80241338() {
    return field_0x1240 == 4 || field_0x1240 == 8 || field_0x1240 == 0x39 || field_0x1240 == 0x44 ||
           field_0x1240 == 0x45 || field_0x1240 == 0x3d || field_0x1240 == 0x37;
}

bool Player_804A6F38::fn_802412E0() {
    return field_0x1240 == 0xd || field_0x1240 == 0xe || field_0x1240 == 0x11 || field_0x1240 == 0x12 ||
           field_0x1240 == 0x1a || field_0x1240 == 0x3c || field_0x1240 == 0x3b || field_0x1240 == 0x3a ||
           field_0x1240 == 0x47;
}

void Player_804A6F38::fn_802412D4() {
    field_0xcaf = 1;
}

bool Player_804A6F38::fn_802412CC() {
    return field_0x460;
}

u8 Player_804A6F38::fn_802412C4() {
    return field_0x458;
}

bool Player_804A6F38::fn_802412A4() {
    return field_0x1240 == 0x4a || field_0x1240 == 0x4b;
}

bool Player_804A6F38::fn_80241284() {
    return field_0x1240 == 9 || field_0x1240 == 0xa;
}

bool Player_804A6F38::fn_80241258() {
    return fn_8027218C() == 4;
}

bool Player_804A6F38::fn_80241230() {
    return field_0x1240 == 0xb || field_0x1244 == 0xb;
}

bool Player_804A6F38::fn_80241200() {
    return field_0x1240 == 1 || (field_0x3c4 >= 0x1bf && field_0x3c4 < 0x1c6);
}

bool Player_804A6F38::fn_802411EC() {
    return field_0x1240 == 1;
}

bool Player_804A6F38::fn_802411D8() {
    return field_0x1240 == 0xa;
}

bool Player_804A6F38::fn_802411C4() {
    return field_0x1240 == 0xc;
}

bool Player_804A6F38::fn_8024119C() {
    return field_0x1240 == 0xc && field_0x11e4 != 0xc;
}

// Whether the player is in state 0x39; for one following in group mode 5,
// the leader's answer (fn_8023513C). Const: its first group loop's
// field_0x4 is hoisted (r28), as in fn_80240670.
// FIXME: 98.4 %. Registers only: the group loops' count / index / leader.
bool Player_804A6F38::fn_80240F28() const {
    if (fn_802486A0() == 5 && fn_80240670()) {
        return fn_8023513C(fn_8022EA0C(field_0x4));
    }
    return field_0x1240 == 0x39;
}

bool Player_804A6F38::fn_80240F14() {
    return field_0x1240 == 0x41;
}

bool Player_804A6F38::fn_80240EF4() {
    return field_0x1240 == 0x42 || field_0x1240 == 0x43;
}

bool Player_804A6F38::fn_80240ED4() {
    return field_0x1240 == 0x1e || field_0x1240 == 0x32;
}

// FIXME: 86.20. Scheduling and registers only: the original loads
// field_0x1240 (fn_80240818) and sets `result` (r31) before saving
// registers; ours keeps `this` in r29 and `result` in r28.
bool Player_804A6F38::fn_80240DCC() {
    bool result = true;
    if (!fn_80240818() && !hiddenFromShownLayer(this)) {
        result = false;
    }
    return result;
}

bool Player_804A6F38::fn_80240DAC() {
    return field_0x1240 == 0x38 || field_0x1240 == 0x13;
}

bool Player_804A6F38::fn_80240D8C() {
    return field_0x1244 == 0x38 || field_0x1244 == 0x13;
}

bool Player_804A6F38::fn_80240D64() {
    return field_0x1240 == 0x4c || field_0x1240 == 0x4d || field_0x1240 == 0x4e;
}

bool Player_804A6F38::fn_80240D50() {
    return field_0x1240 == 0x4f;
}

// Whether the player is busy: through its leader (fn_8022EA0C's
// fn_80233EDC) when it follows, else when it is in state 0x1B. Const like
// fn_80240670: the inlined group loops hoist field_0x4.
// FIXME: 98.9 %. Registers only: the group loop's count / index (r27 / r26
// in the original, r29 / r27 here), as in fn_80240670.
bool Player_804A6F38::fn_80240B84() const {
    return fn_80240670() ? fn_80233EDC(fn_8022EA0C(field_0x4)) : inState1B(this);
}

bool Player_804A6F38::fn_80240B70() {
    return field_0x1240 == 0x50;
}

bool Player_804A6F38::fn_80240B5C() {
    return field_0x1240 == 0x51;
}

bool Player_804A6F38::fn_80240B34() {
    return field_0x1240 == 0x51 && field_0x46c >= 0xd2;
}

bool Player_804A6F38::fn_80240B20() {
    return field_0x1240 == 0x11;
}

bool Player_804A6F38::fn_80240B00() {
    return field_0x1240 == 0xf || field_0x1240 == 0x10;
}

bool Player_804A6F38::fn_80240AF8() {
    return field_0x1214;
}

bool Player_804A6F38::fn_80240AF0() {
    return field_0x1215;
}

bool Player_804A6F38::fn_80240AD4() {
    if (field_0xce8) {
        return false;
    }
    return field_0x1216;
}

// Starts the walk to the spots (mode 5) when pos is inside this leader's view
// box on `layer` (fn_8030B110, the call fn_80241E28 makes): the trail is reset
// to pos on the ground.
// The trail loop counts with `int`: with s32 (long) MWCC tests the
// unrolled loop's entry (cmpwi r5, 0x31; bge), which the original does not.
// FIXME: 99.05 %. Registers only (the inlined group loop's temporaries).
void Player_804A6F38::fn_802408C8(s32 layer, const JGeometry::TVec3f* pos) {
    if (field_0xd60 < 0 && field_0xbec->fn_8030B110(layer, *pos) && !fn_80270A8C() && !field_0xce8) {
        fn_80248004(5, false);
        if (fn_802486A0() == 5) {
            for (int i = 0; i < 0x31; i++) {
                field_0x38[i].set(*pos);
                field_0x38[i].z = 0.0f;
                field_0x284[i] = false;
            }
            field_0x1216 = true;
        }
    }
}

bool Player_804A6F38::fn_802408C0() {
    return field_0x1217;
}

// Whose turn it is in the group: the leader's (fn_80234004) or, leading,
// field_0x1224.
s32 Player_804A6F38::fn_80240888() const {
    s32 team = field_0xd60;
    if (team >= 0) {
        return fn_80234004(team);
    }
    return field_0x1224;
}

void Player_804A6F38::fn_80240878() {
    field_0x1224++;
}

bool Player_804A6F38::fn_80240870() {
    return field_0xcfe;
}

s32 Player_804A6F38::fn_80240868() {
    return field_0x4;
}

bool Player_804A6F38::fn_80240860() {
    return field_0x436;
}

bool Player_804A6F38::fn_80240840(s32 arg) {
    if (field_0x11f4 <= 0) {
        field_0x11f4 = arg;
        return true;
    }
    return false;
}

bool Player_804A6F38::fn_80240818() {
    return field_0x1240 == 0x49 || field_0x1244 == 0x49;
}

bool Player_804A6F38::fn_80240808() {
    return field_0xd60 >= 0;
}

// Whether this player does not act on its own: not taking part (bit
// field_0x4 of gGameRoot_8053AAF8->mPlayerMask clear) without a leader; in
// group mode 4 when it is not its turn; else when it has a leader.
// FIXME: 98.8 %. Registers only: the group loop's count / index / leader
// (r28 / r27 / r30 in the original, r30 / r28 / r27 here). Const: the group
// loop's field_0x4 is hoisted (r29), as in fn_80248778.
bool Player_804A6F38::fn_80240670() const {
    if (!isPlayerBitSet(gGameRoot_8053AAF8, field_0x4) && field_0xd60 < 0) {
        return true;
    }
    if (fn_802486A0() == 4) {
        return fn_80240888() != fn_80248778();
    }
    return field_0xd60 >= 0;
}

s32 Player_804A6F38::fn_8024064C() const {
    return fn_8022EA0C(field_0x4);
}

s32 Player_804A6F38::fn_80240644() {
    return field_0xd60;
}

s32 Player_804A6F38::fn_80240628() {
    s32 team = field_0xd60;
    if (team >= 0) {
        return team;
    }
    return field_0x4;
}

// Sets field_0x43e (wait for the leader, see fn_80242CB0) unless the group
// is turn-based (mode 4) or field_0x1215 is set.
// FIXME: 95.2 %. Registers only (plus the stw r29 save scheduled after the
// zeroing): the inlined group loop's index / count / leader are r31 / r30 /
// r29 in the original, r30 / r29 / r31 here.
void Player_804A6F38::fn_8024051C() {
    if (fn_802486A0() != 4 && !field_0x1215) {
        field_0x43e = 1;
    }
}

// FIXME: 99.4 %. Register allocation only: the group loop's index / count /
// leader r27 / r28 / r29 in ours vs r28 / r29 / r27 (and r30 for the turn).
// The leader's value (fn_80236768, this function for the leader) for a following
// player, else field_0xbd8 while field_0xbdc is set, else 0.
s32 Player_804A6F38::fn_80240354() {
    if (fn_80240670()) {
        return fn_80236768(fn_8022EA0C(field_0x4));
    }
    if (field_0xbdc) {
        return field_0xbd8;
    }
    return 0;
}

// FIXME: 99.4 %. Register allocation only: the group loop's index / count /
// leader r27 / r28 / r29 in ours vs r28 / r29 / r27 (and r30 for the turn).
// The leader's value (fn_802366CC, this function for the leader) for a following
// player, else field_0xbe0 while field_0x42c is set, else 0.
s32 Player_804A6F38::fn_8024018C() {
    if (fn_80240670()) {
        return fn_802366CC(fn_8022EA0C(field_0x4));
    }
    if (field_0x42c) {
        return field_0xbe0;
    }
    return 0;
}

// FIXME: 99.4 %. Register allocation only: the group loop's index / count /
// leader r27 / r28 / r29 in ours vs r28 / r29 / r27 (and r30 for the turn).
// The leader's value (fn_80236630, this function for the leader) for a following
// player, else field_0xbe4 while field_0xbe8 is set, else 0.
s32 Player_804A6F38::fn_8023FFC4() {
    if (fn_80240670()) {
        return fn_80236630(fn_8022EA0C(field_0x4));
    }
    if (field_0xbe8) {
        return field_0xbe4;
    }
    return 0;
}

s32 Player_804A6F38::fn_8023FFAC() {
    return field_0xc84.z < 1.0f;
}

void Player_804A6F38::fn_8023FF90(const JGeometry::TVec3f* arg) {
    field_0xc90.set(arg->x, arg->y, arg->z);
}

bool Player_804A6F38::fn_8023FF88() {
    return field_0x1200;
}

bool Player_804A6F38::fn_8023FF50() {
    s32 team = field_0xd60;
    if (team >= 0) {
        return fn_8023BD70(team);
    }
    return field_0x1202;
}

// FIXME: 98.7 %. Register allocation only (the group loops' count / index /
// leader temporaries, as in fn_802486A0).
// The byte 0xCA0, halved for a player following in group mode 4 (unless
// 0xCFE or 0x454 is set); 0 when the player flags are clear. Const: the
// group loops' field_0x4 is hoisted (r28, r30).
u8 Player_804A6F38::fn_8023FC5C() const {
    if (playerFlagsGate(this)) {
        if (fn_802486A0() == 4 && fn_80240670() && !field_0xcfe && !field_0x454) {
            return field_0xca0 / 2;
        }
        return field_0xca0;
    }
    return 0;
}

s32 Player_804A6F38::fn_8023FC2C() {
    if (field_0xd2d == 1) {
        return 0;
    }
    if (field_0x498 == 0) {
        return 0;
    }
    return field_0x494;
}

cls_804AA8D8* Player_804A6F38::fn_8023FC24() {
    return field_0xbec;
}

void Player_804A6F38::fn_8023FC10(s32 value) {
    if (field_0xca8 < value) {
        field_0xca8 = value;
    }
}

bool Player_804A6F38::fn_8023FC08() {
    return field_0x42c;
}

void Player_804A6F38::fn_8023FBF0() {
    if (!field_0x42c) {
        return;
    }
    field_0x42d = 1;
}
