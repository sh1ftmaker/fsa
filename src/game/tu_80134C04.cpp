#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/GameMode.h"
#include "game/Message.h"
#include "game/GameView.h"
#include "game/cls_804A8390.h"
#include "game/cls_804AE9B0.h"
#include "game/cls_804AF480.h"
#include "game/cls_804AF878.h"
#include "game/cls_804B19F0.h"
#include "game/act/tu_803DA6B4.h"
#include "game/act/tu_80440BF4.h"
#include "game/tu_80379F84.h"
#include "game/tu_803A3484.h"
#include "game/tu_803B8C74.h"
#include "game/StageFlag.h"
#include "game/tu_803BC6F0.h"
#include "game/tu_803DAC3C.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/J2D/J2DPane.h"
#include "JSystem/J2D/J2DPicture.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "JSystem/JUtility/JUTVideo.h"
#include "JSystem/JFramework/JFWDisplay.h"
#include "JSystem/J3D/J3DSys.h"
#include <string.h>
#include "JSystem/JUtility/JUTException.h"
#include "JSystem/JUtility/JUTGamePad.h"

void fn_80139958();
void fn_80139D70(); // game/tu_80139BE8.cpp

// The boss archive names (by lbl_80529DEC's field_0x118 / field_0x11c;
// fn_80135A04) and the pad rumble patterns, one .data object.
struct cls_8049ED88 {
    const char* boss[13][4];
    u8 lbl_8049EE58[0x88];
    u8 lbl_8049EEE0[0x10];
    u8 lbl_8049EEF0[0x10];
    u8 lbl_8049EF00[0x10];
    u8 lbl_8049EF10[0x10];
};

static cls_8049ED88 lbl_8049ED88 = {
    {
    {"boss010.arc", "boss011.arc", "boss200.arc", "boss013.arc"},
    {"boss020.arc", "boss021.arc", "boss201.arc", "boss023.arc"},
    {"boss030.arc", "boss031.arc", "boss202.arc", "boss033.arc"},
    {"boss040.arc", "boss041.arc", "boss203.arc", "boss043.arc"},
    {"boss050.arc", "boss051.arc", "boss204.arc", "boss053.arc"},
    {"boss060.arc", "boss061.arc", "boss205.arc", "boss063.arc"},
    {"boss070.arc", "boss071.arc", "boss206.arc", "boss073.arc"},
    {"boss080.arc", "boss207.arc", "boss081.arc", "boss083.arc"},
    {"boss002.arc", "boss001.arc", "boss000.arc", "boss990.arc"},
    {"boss004.arc", "boss005.arc", "boss006.arc", "boss991.arc"},
    {"boss007.arc", "boss008.arc", "boss009.arc", "boss992.arc"},
    {"boss090.arc", "boss091.arc", "boss092.arc", "boss093.arc"},
    {"boss094.arc", "boss095.arc", "boss096.arc", "boss097.arc"},
    },
    {
    0x04, 0x1A, 0x00, 0x00, 0x02, 0x02, 0x08, 0x21, 0x10, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
    0x88, 0x88, 0x42, 0x04, 0x02, 0x00, 0x20, 0x10, 0x08, 0x22, 0x25, 0x2A, 0xAA, 0xAA, 0xAB, 0x55,
    0x6A, 0xB5, 0x55, 0x55, 0x55, 0x55, 0x52, 0xAA, 0xB7, 0x77, 0xBD, 0xEF, 0xBF, 0x7F, 0xBF, 0xFD,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xDF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFD, 0xFF, 0xBF, 0x7B, 0xDD, 0xB6, 0xAA, 0xA5, 0x52, 0xAA, 0xAA, 0xAA, 0xAA,
    0xB5, 0xB5, 0xB6, 0xDA, 0xDD, 0xB6, 0xDB, 0x6D, 0xB5, 0xAA, 0xA5, 0x24, 0x92, 0x25, 0x29, 0x56,
    0xAD, 0xAD, 0x6D, 0x55, 0x52, 0x89, 0x24, 0x94, 0x92, 0x94, 0xAB, 0x55, 0x54, 0x92, 0x49, 0x29,
    0x25, 0x29, 0x25, 0x24, 0x89, 0x24, 0x48, 0x88, 0x88, 0x88, 0x89, 0x25, 0x25, 0x24, 0x92, 0x48,
    0x84, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    },
    {
    0x00, 0x5B, 0x00, 0x00, 0x81, 0x02, 0x04, 0x10, 0x20, 0x10, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
    },
    {
    0x00, 0x5B, 0x00, 0x08, 0x89, 0x12, 0x44, 0x91, 0x10, 0x84, 0x08, 0x04, 0x00, 0x00, 0x00, 0x00,
    },
    {
    0x00, 0x5B, 0x00, 0x22, 0x4A, 0x52, 0x94, 0xA4, 0x92, 0x22, 0x10, 0x20, 0x02, 0x00, 0x00, 0x00,
    },
    {
    0x00, 0x5B, 0x00, 0x10, 0x92, 0x4A, 0x52, 0x94, 0xA4, 0x88, 0x84, 0x10, 0x00, 0x40, 0x00, 0x00,
    },
};

// The stage music (sequence IDs) by GameView_804A9950::field_0x34
// (fn_80138B7C).
static const u32 lbl_804652F0[39] = {
    0x01000000, 0x01000003, 0x0100000B, 0x0100000E, 0x01000010, 0x01000014,
    0x0100000A, 0x01000009, 0x01000008, 0x01000012, 0x01000011, 0x01000015,
    0x01000018, 0x01000016, 0x01000013, 0x0100000C, 0x01000004, 0x01000004,
    0x0100001B, 0x0100001C, 0x01000023, 0x01000025, 0x01000007, 0x01000017,
    0x01000021, 0x01000019, 0x01000024, 0x01000002, 0x0100000D, 0x01000008,
    0x01000001, 0x01000029, 0x01000009, 0x01000026, 0x01000018, 0x0100000A,
    0x0100002A, 0x0100002E, 0x01000030,
};

// Forward declarations.
void fn_80134C04(OSError error, OSContext* context, u32 dsisr, u32 dar);

// JUTGamePad C3ButtonReset callback (JUTResetBtnCb).
void fn_80139994(int port, void* arg) {
    lbl_8053ACA8->fn_802B1610(port, arg);
}

// The 2-bit field of field_0x83 for `index` (0-3), as the caller reads it.
// Not auto-inlined: GameRoot_8049EF20::fn_80137300 calls fn_8013997C out of line in the original
// (three bl; docs/INLINING.md, pragma rule 1).
#pragma auto_inline off
s32 fn_8013997C(s32 index) {
    return (gGameRoot_8053AAF8->field_0x83 >> (index * 2)) & 3;
}
#pragma auto_inline reset

// Not auto-inlined: GameRoot_8049EF20::fn_80135020 calls fn_80139958 out of line in the original
// (docs/INLINING.md, pragma rule 1).
#pragma auto_inline off
void fn_80139958() {
    lbl_8053AD68->fn_8037A708();
}
#pragma auto_inline reset

// FIXME: 99.86 %, reloc-name; behaviour clean.
// Instructions identical; the string offsets off lbl_804652F0
// differ (0x324 vs 0x5E8: the strings of the unwritten fn_80135494 - fn_80137300
// come first in the original .rodata) and the .sdata2 floats / "" are @NNN.
GameRoot_8049EF20::GameRoot_8049EF20() : mRandom(0) {
    gGameRoot_8053AAF8 = this;
    field_0x9c = lbl_80529DEC.field_0x4;
    field_0x98 = lbl_80529DEC.mHeap;
    field_0xa0 = lbl_80529DEC.field_0xc;
    field_0xa4 = lbl_80529DEC.field_0x14;
    field_0xa8 = lbl_80529DEC.field_0x1c;
    field_0x98->becomeCurrentHeap();
    field_0x18 = new (field_0x98, 0) MessageManager_804AF070();
    gGameRoot2_8053AB10 = new (field_0x98, 0) GameRoot2_8049F108(field_0x98);
    field_0x8 = lbl_80529DEC.field_0x5c;
    field_0xc = lbl_80529DEC.field_0x60;
    field_0x10 = new (field_0x98, 0) cls_804A8390();
    field_0x34 = new (field_0x98, 0) cls_804B04F8();
    field_0x38 = new (field_0x98, 0) cls_804B19F0();
    field_0x3c = new (field_0x98, 0) cls_804B1988();
    field_0x40 = new (field_0x98, 0) cls_804B1634();
    field_0xac = NULL;
    field_0xb0 = NULL;
    field_0x4 = NULL;
    field_0x1c = NULL;
    field_0x28 = NULL;
    field_0x24 = NULL;
    field_0x2c = NULL;
    field_0x30 = NULL;
    field_0xfd = 0;
    field_0x84 = 0;
    field_0x54 = 0;
    field_0x85 = 0;
    field_0x74 = 0;
    field_0x78 = 0;
    field_0x86 = 0;
    field_0x88 = 0;
    field_0x94 = 0;
    field_0x95 = 0;
    field_0x96 = 0;
    field_0x10c = 0;
    field_0x108 = 0;
    field_0x7c = 0;
    u8 group = field_0x98->changeGroupID(1);
    field_0x20 = new (field_0x98, 0) Title_804AF820();
    field_0x98->changeGroupID(group);
    field_0x87 = 0;
    ResTIMG* img = (ResTIMG*)fn_804030C4('TIMG', "kk_rival_01.bti");
    field_0xb8 = new (field_0x98, 0) J2DPicture(img);
    field_0xb8->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0xFF, 0xD2, 0, 0xFF));
    img = (ResTIMG*)gGameRoot_8053AAF8->field_0xa4->getResource('TIMG', "");
    field_0xbc = new (field_0x98, 0) J2DPicture(img);
    if (fn_80021840() == 2) {
        field_0xc0 = new (field_0x98, 0) J2DTextBox('TEXT1',
            JGeometry::TBox2f(-30000.0f, 210.0f, 60000.0f + (s32)JUTVideo::getManager()->getFbWidth(), 180.0f),
            "rock_24_20_4i_j.bfn", field_0xc8, 0x28, J2DHBIND_Center, J2DVBIND_Center);
        field_0xc4 = new (field_0x98, 0) J2DTextBox('TEXT2',
            JGeometry::TBox2f(-29997.0f, 213.0f, 60000.0f + (s32)JUTVideo::getManager()->getFbWidth(), 180.0f),
            "rock_24_20_4i_j.bfn", field_0xc8, 0x28, J2DHBIND_Center, J2DVBIND_Center);
    } else {
        field_0xc0 = new (field_0x98, 0) J2DTextBox('TEXT1',
            JGeometry::TBox2f(-30000.0f, 210.0f, 60000.0f + (s32)JUTVideo::getManager()->getFbWidth(), 180.0f),
            "rock_24_20_4i_e.bfn", field_0xc8, 0x28, J2DHBIND_Center, J2DVBIND_Center);
        field_0xc4 = new (field_0x98, 0) J2DTextBox('TEXT2',
            JGeometry::TBox2f(-29997.0f, 213.0f, 60000.0f + (s32)JUTVideo::getManager()->getFbWidth(), 180.0f),
            "rock_24_20_4i_e.bfn", field_0xc8, 0x28, J2DHBIND_Center, J2DVBIND_Center);
    }
    field_0xc0->setBlackWhite(JUtility::TColor(0x50502800), JUtility::TColor(0xFFFF50FF));
    field_0xc4->setFontColor(JUtility::TColor(0x000000FF), JUtility::TColor(0x000000FF));
    field_0x104 = NULL;
    JUTException::appendMapFile("GC4Swords.MAP");
    JUTException::setPreUserCallback(fn_80134C04);
}

// Stops the rumble, deletes the screens and objects the constructor and
// fn_80135020 made, unmounts the archives and destroys both heaps.
// FIXME: 99.31 %, missing/extra; behaviour drift.
// Only the null test before deleting field_0x104 is missing
// (cmplwi / beq): the original's type for it is a class with a non-virtual
// destructor, unknown yet (nothing but the constructor's NULL stores it).
GameRoot_8049EF20::~GameRoot_8049EF20() {
    JUTGamePad* pad = JUTGamePad::getGamePad(0);
    field_0x10c = 0;
    field_0x108 = 0;
    JUTGamePad::CRumble::stopMotor(pad->mPortNum, false);
    pad->mRumble.stopPatternedRumble(pad->mPortNum);
    delete field_0x104;
    if (field_0x4 != NULL) {
        delete field_0x4;
        field_0x4 = NULL;
    }
    delete field_0x18;
    field_0x18 = NULL;
    delete gGameRoot2_8053AB10;
    delete field_0x8;
    delete field_0xc;
    delete field_0x10;
    if (field_0x20 != NULL) {
        delete field_0x20;
        field_0x20 = NULL;
    }
    if (field_0x28 != NULL) {
        delete field_0x28;
        field_0x28 = NULL;
    }
    if (field_0x24 != NULL) {
        delete field_0x24;
        field_0x28 = NULL; // sic: the original clears field_0x28 here, not field_0x24
    }
    if (field_0x2c != NULL) {
        delete field_0x2c;
        field_0x2c = NULL;
    }
    if (field_0x30 != NULL) {
        delete field_0x30;
        field_0x30 = NULL;
    }
    if (field_0x1c != NULL) {
        delete field_0x1c;
        field_0x1c = NULL;
    }
    // These four archives are JKRAramArchives (the unmountFixed the original calls).
    static_cast<JKRAramArchive*>(lbl_80529DEC.field_0x24)->unmountFixed();
    static_cast<JKRAramArchive*>(lbl_80529DEC.field_0x38)->unmountFixed();
    static_cast<JKRAramArchive*>(lbl_80529DEC.field_0x40)->unmountFixed();
    static_cast<JKRAramArchive*>(lbl_80529DEC.field_0x44)->unmountFixed();
    delete field_0x34;
    field_0x34 = NULL;
    delete field_0x38;
    field_0x38 = NULL;
    delete field_0x3c;
    field_0x3c = NULL;
    delete field_0x40;
    field_0x40 = NULL;
    field_0xa4->unmount();
    field_0xa8->unmount();
    lbl_80529DEC.field_0x20->unmount();
    fn_803007A0();
    delete field_0xa0;
    field_0x9c->destroy();
    delete field_0xb8;
    field_0xb8 = NULL;
    delete field_0xbc;
    field_0xbc = NULL;
    delete field_0xc0;
    field_0xc0 = NULL;
    delete field_0xc4;
    field_0xc4 = NULL;
    field_0x98->destroy();
}

// Called once by main after the constructor: resets the per-game state and
// picks the first game mode (the title after power-on, see SaveRegion 0x4/0x5).
void GameRoot_8049EF20::fn_80138C9C() {
    field_0xa0->field_0x5 = 4;
    gGameRoot2_8053AB10->fn_8013BE04();
    field_0x10->fn_802ACB58();
    fn_80119DD0()->fn_80119D48();
    fn_8011921C()->fn_801190E4();
    field_0x5c = 0;
    field_0x94 = 0;
    field_0x96 = 0;
    field_0x10c = 0;
    field_0x108 = 0;
    field_0x58 = 0;
    if (lbl_80529DEC.mSaveRegion.f.field_0x4[0] != 0xA7) {
        lbl_80529DEC.mSaveRegion.f.field_0x4[0] = 0xA7;
        field_0x4c = 1;
    } else {
        if (lbl_80529DEC.mSaveRegion.f.field_0x4[1] != 0xA5) {
            lbl_80529DEC.field_0x249[0] = 1;
            fn_80136798(2, 0);
        } else {
            fn_80136798(1, 0);
        }
        lbl_80529DEC.mSaveRegion.f.field_0x4[1] = 0;
    }
    field_0x60 = 0;
    field_0x64 = 0;
    field_0x68 = 0;
    mRandom.setSeed(0x12D5DA0);
    field_0x70 = 0;
    field_0x80 = 0;
    field_0x81 = 0;
    field_0x7e = 1;
    field_0x7f = 1;
    mPlayerMask = 0;
    field_0x83 = 0;
    field_0xff = 0;
    field_0xf0 = 0.0f;
    field_0x86 = (s32)(50.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) & 1;
    field_0xf4 = getMessageText_803832BC(field_0xc8, 0x42, 0, NULL, NULL);
    strcpy(field_0xc4->getStringPtr(), field_0xc8);
    strcpy(field_0xc0->getStringPtr(), field_0xc8);
}

// Starts the music of the current stage (GameView_804A9950::field_0x34).
void GameRoot_8049EF20::fn_80138B7C() {
    s32 count = getGameView_802D6A9C()->field_0x1328c;
    s32 id = getGameView_802D6A9C()->field_0x34;
    if (id == 0x11) {
        return;
    }
    if (gGameRoot_8053AAF8->field_0x7f == 1) {
        if (id == 0x14 || id == 0x1E) {
            field_0xa0->fn_80458F04(lbl_804652F0[id]);
        } else {
            field_0xa0->fn_80458FF0(lbl_804652F0[id], false, count);
            GameView_804A9950* view = getGameView_802D6A9C();
            view->field_0x1328c = count + 1;
        }
    } else if (id != 0x19) {
        if (field_0x86 == 0) {
            field_0xa0->fn_80458FF0(0x1000004, false, false);
        } else {
            field_0xa0->fn_80458FF0(0x100000D, false, false);
        }
    } else {
        field_0xa0->fn_80458FF0(0x1000019, false, false);
    }
}

bool fn_80138AB0() {
    if (gGameRoot_8053AAF8->field_0x7e == 1) {
        if (lbl_8053AD30->fn_8030A308(getPlayerInputSlot_8030AAF4(0)) && lbl_8053AD30->fn_8030A308(getPlayerInputSlot_8030AAF4(1)) &&
            lbl_8053AD30->fn_8030A308(getPlayerInputSlot_8030AAF4(2)) && lbl_8053AD30->fn_8030A308(getPlayerInputSlot_8030AAF4(3)))
        {
            return true;
        }
    } else if (lbl_8053AD30->fn_8030A308(getPlayerInputSlot_8030AAF4(0))) {
        return true;
    }
    return false;
}

// The number of players that take part: the set bits 0-3 of mPlayerMask.
s32 GameRoot_8049EF20::fn_80138A30() {
    s32 count = 0;
    for (s32 i = 0; i < 4; i++) {
        if (gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << i)) {
            count++;
        }
    }
    return count;
}

// The per-frame update (the counterpart of fn_80136828): polls the pads, then runs the screen of the current
// mode (field_0x4c) and, once it asks for another one (field_0x50), the fade and load sequence of field_0x58.
//
// FIXME(shotgun): 95.59 %, missing/extra + reloc-name; behaviour drift.
// (one fn_802DEBF8 call folded; the saved-register set differs;
// the target loads field_0x4 into r3 before each fn_80139D70 call).
void GameRoot_8049EF20::fn_80137300() {
    field_0x5c++;
    field_0x8->fn_8030A43C();
    gGameRoot_8053AAF8->field_0x85 = 0;
    JUTGamePad* pad = JUTGamePad::getGamePad(0);
    if (field_0x108 != 0) {
        if (field_0x10c == 0) {
            JUTGamePad::CRumble::startMotor(pad->mPortNum);
        }
        field_0x108--;
    } else {
        JUTGamePad::CRumble::stopMotor(pad->mPortNum, false);
    }
    if (field_0x58 < 5) {
        if (lbl_8053ACA8->fn_802B1DCC() && lbl_80529DEC.field_0x24c == 0) {
            field_0x58 = 5;
        }
    }
    u8 loading = field_0xfd;
    if (loading == 0 && (field_0x58 == 0 || field_0x58 == 2 || field_0x58 == 3)) {
        if (lbl_80529DEC.field_0x24c == 0) {
            s32 mode = field_0x4c;
            if (mode == 7 || (mode >= 8 && mode <= 0xF) || (mode >= 0x11 && mode <= 0x12) || mode == 6) {
                s32 joiner;
                s32 i = 0;
                for (;;) {
                    if (fn_8013997C(i) != 0) {
                        if (!lbl_8053AD30->fn_8030A2E4(getPlayerInputSlot_8030AAF4(i)) &&
                            !lbl_8053AD30->fn_8030A364(getPlayerInputSlot_8030AAF4(i))) {
                            joiner = i + 1;
                            break;
                        }
                        if (fn_8013997C(i) == 1 && !lbl_8053AD30->fn_8030A2E4(getPlayerInputSlot_8030AAF4(i))) {
                            joiner = i + 1;
                            break;
                        }
                        if (fn_8013997C(i) == 2 && !lbl_8053AD30->fn_8030A364(getPlayerInputSlot_8030AAF4(i))) {
                            joiner = i + 1;
                            break;
                        }
                    }
                    i++;
                    if (i >= 4) {
                        joiner = 0;
                        break;
                    }
                }
                u8 allowed = 1;
                if (field_0x4c == 0xC) {
                    if (field_0x1c != NULL) {
                        if (fn_8037A120() >= 5) {
                            allowed = 0;
                        }
                    }
                } else {
                    u8 inSet;
                    if (gGameRoot_8053AAF8->field_0x7f != 0) {
                        inSet = isGameModeSetA(gGameRoot_8053AAF8->field_0x4c);
                    } else {
                        inSet = isGameModeSetB(gGameRoot_8053AAF8->field_0x4c);
                    }
                    if (inSet) {
                        if (getGameView_802D6A9C()->fn_802DCD0C() == 1 && field_0xfc != 0) {
                            allowed = 0;
                        } else {
                            u32 seq = gAudioMgr_8053AF80->field_0x58 - 0x1000000;
                            if (fn_802DEBF8() && (seq == 0x2C || seq == 0x1D || seq - 0x1F <= 1 || seq == 0x2B)) {
                                allowed = 0;
                            } else if (fn_802DEBA0()) {
                                u32 seq2 = gAudioMgr_8053AF80->field_0x58 - 0x1000000;
                                if (seq2 == 0x26 || seq2 == 0x28) {
                                    allowed = 0;
                                }
                            }
                        }
                    }
                }
                if (joiner != 0 && allowed) {
                    lbl_8053AEE0->fn_80442254(field_0x4c, joiner);
                    field_0x4c = 0x16;
                }
            }
        }
        switch (field_0x4c) {
        case 0:
            if (lbl_8053AAF0->mState >= 4) {
                field_0x50 = 1;
                field_0x58 = 1;
                lbl_8053ACA0->fn_802AC8AC(0);
                field_0x84 = 0;
            }
            break;
        case 1:
            if (field_0x87 == 0) {
                field_0x87 = 1;
                if (lbl_8053AD88 != NULL) {
                    lbl_8053AD88->fn_803A5380();
                }
            }
            gGameRoot2_8053AB10->fn_8013BBE0();
            if (lbl_8053AD88 != NULL) {
                lbl_8053AD88->fn_803A34A0();
            }
            if (Title_804AF820::fn_803A3484() != 1) {
                field_0x50 = Title_804AF820::fn_803A3484();
                field_0x58 = 1;
                lbl_8053ACA0->fn_802AC8AC(0);
                field_0x84 = 0;
                field_0x60 = 0;
            }
            break;
        case 2:
            if (field_0x60 == 0) {
                field_0x60 = 1;
                lbl_8053ADD0->fn_803DF9E8();
            }
            lbl_8053ADD0->fn_803DF810();
            gGameRoot2_8053AB10->fn_8013BBE0();
            if (GameMode_804B0688::fn_803DAC3C() != 2) {
                if (GameMode_804B0688::fn_803DAC3C() == 1 || GameMode_804B0688::fn_803DAC3C() == 6) {
                    field_0x50 = GameMode_804B0688::fn_803DAC3C();
                    field_0x58 = 1;
                    lbl_8053ACA0->fn_802AC8AC(0);
                    field_0x84 = 0;
                } else {
                    field_0x50 = GameMode_804B0688::fn_803DAC3C();
                    field_0x58 = 1;
                    lbl_8053ACA0->fn_802AC8AC(0);
                    field_0x84 = 0;
                }
                field_0x60 = 0;
            }
            break;
        case 3:
            if (field_0x60 == 0) {
                field_0x60 = 1;
                lbl_8053ADB0->fn_803CF504();
            }
            lbl_8053ADB0->fn_803CE9EC();
            if (FileSelect_804AFF60::fn_803CE4B0() != 3) {
                if (FileSelect_804AFF60::fn_803CE4B0() == 1 || FileSelect_804AFF60::fn_803CE4B0() == 6) {
                    field_0x50 = FileSelect_804AFF60::fn_803CE4B0();
                    field_0x58 = 1;
                    lbl_8053ACA0->fn_802AC8AC(0);
                    field_0x84 = 0;
                } else {
                    field_0x50 = FileSelect_804AFF60::fn_803CE4B0();
                    field_0x58 = 1;
                    lbl_8053ACA0->fn_802AC8AC(0);
                    field_0x84 = 0;
                }
                field_0x60 = 0;
            }
            break;
        case 4:
            if (field_0x60 == 0) {
                field_0x60 = 1;
                lbl_8053AD98->fn_803A86A4();
            }
            lbl_8053AD98->fn_803A8600();
            gGameRoot2_8053AB10->fn_8013BBE0();
            if (fn_803A85E4() != 4) {
                if (fn_803A85E4() == 1 || fn_803A85E4() == 6) {
                    field_0x50 = fn_803A85E4();
                    field_0x58 = 1;
                    lbl_8053ACA0->fn_802AC8AC(0);
                    field_0x84 = 0;
                } else {
                    field_0x50 = fn_803A85E4();
                    field_0x58 = 1;
                    lbl_8053ACA0->fn_802AC8AC(0);
                    field_0x84 = 0;
                }
                field_0x60 = 0;
            }
            break;
        case 5:
            if (field_0x60 == 0) {
                field_0x60 = 1;
                lbl_8053AD90->fn_803A7280();
            }
            lbl_8053AD90->fn_803A6608();
            if (fn_803A65EC() != 5) {
                field_0x50 = fn_803A65EC();
                field_0x58 = 1;
                lbl_8053ACA0->fn_802AC8AC(0);
                field_0x84 = 0;
            }
            break;
        case 6:
            if (field_0x60 == 0) {
                field_0x60 = 1;
            }
            lbl_8053AD80->fn_803900C8();
            gGameRoot2_8053AB10->fn_8013BBE0();
            if (fn_8039003C() != 6) {
                field_0x60 = 0;
                field_0x64 = 0;
                field_0x50 = fn_8039003C();
                field_0x58 = 1;
                lbl_8053ACA0->fn_802AC8AC(0);
                field_0x84 = 0;
            }
            break;
        case 7:
        case 8:
            switch (field_0x60) {
            case 0:
                if (gGameRoot_8053AAF8->field_0x7e != 0) {
                    field_0x64 = 0x12C;
                } else {
                    field_0x64 = 10;
                }
                field_0x60++;
                field_0x8c = lbl_8050DB0C.field_0x4c[10];
                field_0x90 = 30.0f;
                field_0x68 = 0xFF;
                field_0x6c = 0xFF;
                if (gGameRoot_8053AAF8->field_0x7e != 0) {
                    field_0x80 = 1;
                    field_0xa0->fn_80458880(0x23, -1);
                }
                break;
            case 1:
                field_0x64--;
                if (field_0x64 == 0) {
                    field_0x64 = 0x28;
                    field_0x60++;
                    if (gGameRoot_8053AAF8->field_0x7e != 0) {
                        field_0xa0->fn_80458880(0x24, -1);
                    }
                }
                if (field_0x8c >= 1.0f) {
                    field_0x8c -= 50.0f / lbl_8050DB0C.field_0x4c[11];
                    if (field_0x8c < 1.0f) {
                        field_0x8c = 1.0f;
                    }
                }
                break;
            case 2:
                field_0x64--;
                if (field_0x64 == 0) {
                    field_0x64 = 0x1E;
                    field_0x60++;
                }
                if (field_0x68 != 0) {
                    field_0x68 -= 10;
                    if (field_0x68 < 0) {
                        field_0x68 = 0;
                    }
                }
                if (field_0x90 > 1.0f) {
                    field_0x90 -= 10.0f;
                    if (field_0x90 < 1.0f) {
                        field_0x90 = 1.0f;
                        fn_80138B7C();
                    }
                }
            case 3:
                if (field_0x60 == 3) {
                    field_0x6c -= 6;
                    if (field_0x6c <= 0) {
                        field_0x6c = 0;
                        field_0x64 = 0;
                        field_0x60++;
                    }
                }
                break;
            case 4:
                fn_8043A0E8(false);
                field_0x60++;
                field_0x80 = 0;
                break;
            }
            gGameRoot2_8053AB10->fn_8013BBE0();
            field_0x4->fn_80139D9C();
            fn_80139D70();
            break;
        case 9:
            lbl_8053AD70->fn_8037AEAC();
            gGameRoot2_8053AB10->fn_8013BBE0();
            field_0x4c = fn_8037AE60();
            if (field_0x4c != 9 && field_0x4c != 7) {
                field_0x4c = 9;
                field_0x50 = fn_8037AE60();
                field_0x58 = 1;
                lbl_8053ACA0->fn_802AC8AC(0);
                field_0x84 = 0;
                field_0x60 = 0;
                for (s32 player = 0; player < 4; player++) {
                    fn_803037D4(player, 0x12);
                }
            }
            break;
        case 10:
            gGameRoot2_8053AB10->fn_8013BBE0();
            fn_803B8C74(lbl_80529DEC.field_0x24f);
            if (gGameRoot_8053AAF8->field_0x7f != 0 && gGameRoot_8053AAF8->field_0x7e == 0) {
                s32 a = lbl_80529DEC.field_0x118;
                s32 b = lbl_80529DEC.field_0x11c;
                if (a < 8 && b < 4) {
                    setStageFlag(a, b);
                }
                if (a < 8) {
                    s32 next = b + 1;
                    if (next > 3) {
                        s32 nextRow = a + 1;
                        next = 0;
                        if (nextRow >= 8) {
                            nextRow = 0;
                        }
                        lbl_80529DEC.field_0x118 = nextRow;
                    } else if ((a == 0 && next == 2) || (a == 1 && next == 2) || (a == 2 && next == 2) ||
                               (a == 3 && next == 2) || (a == 4 && next == 2) || (a == 5 && next == 2) ||
                               (a == 6 && next == 2) || (a == 7 && next == 1)) {
                        next++;
                    }
                    lbl_80529DEC.field_0x11c = next;
                }
                lbl_80529DEC.field_0x237 = 6;
                field_0x50 = 6;
                field_0x58 = 1;
                lbl_8053ACA0->fn_802AC8AC(0);
                field_0x84 = 0;
            } else {
                field_0x50 = 0xC;
                field_0x58 = 1;
                lbl_8053ACA0->fn_802AC8AC(0);
                field_0x84 = 0;
            }
            lbl_8053AED0->fn_8043D644();
            break;
        case 11:
            gGameRoot2_8053AB10->fn_8013BBE0();
            field_0x4->fn_80139D9C();
            fn_80139D70();
            lbl_8053AED0->fn_8043D644();
            switch (lbl_8053AEC8->fn_80438E94()) {
            case 7:
                field_0x4->fn_80139BE8();
                gGameRoot2_8053AB10->fn_8013B4C0();
                field_0x50 = 0xC;
                field_0x58 = 1;
                lbl_8053ACA0->fn_802AC8AC(0);
                field_0x84 = 0;
                lbl_80529DEC.field_0x237 = 8;
                field_0x60 = 0;
                break;
            case 5:
                gGameRoot2_8053AB10->fn_8013BBE0();
                field_0x4->fn_80139D9C();
                lbl_8053AEC8->fn_80438E10(0);
                if (lbl_80529DEC.field_0x1ac != 0) {
                    field_0xa0->fn_80458FF0(lbl_80529DEC.field_0x1ac, false, 0);
                }
                lbl_80529DEC.field_0x253 = 0;
                field_0x4c = 7;
                break;
            }
            break;
        case 12:
            gGameRoot2_8053AB10->fn_8013BBE0();
            fn_8038D570();
            lbl_8053AD68->fn_8037A174();
            if (fn_8037A120() == 7) {
                if (lbl_80529DEC.field_0x24d != 0) {
                    lbl_80529DEC.field_0x24d = 0;
                    field_0x50 = 7;
                    field_0x58 = 1;
                    lbl_8053ACA0->fn_802AC8AC(0);
                    lbl_80529DEC.field_0x118 = 9;
                    lbl_80529DEC.field_0x11c = 3;
                    field_0x84 = 1;
                } else {
                    field_0x50 = 1;
                    field_0x58 = 1;
                    lbl_8053ACA0->fn_802AC8AC(0);
                    field_0x84 = 0;
                    gGameRoot2_8053AB10->fn_8013B4C0();
                    field_0xa0->fn_80458F9C(0xA);
                }
            } else if (fn_8037A120() == 8 || fn_8037A120() == 9) {
                if (gGameRoot_8053AAF8->field_0x7e != 0) {
                    if (gGameRoot_8053AAF8->field_0x7f != 0 &&
                        ((lbl_80529DEC.field_0x118 == 0 && lbl_80529DEC.field_0x11c == 2) ||
                         (lbl_80529DEC.field_0x118 == 1 && lbl_80529DEC.field_0x11c == 2) ||
                         (lbl_80529DEC.field_0x118 == 2 && lbl_80529DEC.field_0x11c == 2) ||
                         (lbl_80529DEC.field_0x118 == 3 && lbl_80529DEC.field_0x11c == 2) ||
                         (lbl_80529DEC.field_0x118 == 4 && lbl_80529DEC.field_0x11c == 2) ||
                         (lbl_80529DEC.field_0x118 == 5 && lbl_80529DEC.field_0x11c == 2) ||
                         (lbl_80529DEC.field_0x118 == 6 && lbl_80529DEC.field_0x11c == 2) ||
                         (lbl_80529DEC.field_0x118 == 7 && lbl_80529DEC.field_0x11c == 1) ||
                         (lbl_80529DEC.field_0x118 == 0xA && lbl_80529DEC.field_0x11c == 2) ||
                         lbl_80529DEC.field_0x237 == 8)) {
                        lbl_80529DEC.field_0x237 = 7;
                    } else {
                        lbl_80529DEC.field_0x237 = 6;
                    }
                } else if (lbl_80529DEC.field_0x237 == 8) {
                    lbl_80529DEC.field_0x237 = 7;
                } else {
                    lbl_80529DEC.field_0x237 = 6;
                }
                field_0x50 = 6;
                field_0x58 = 1;
                lbl_8053ACA0->fn_802AC8AC(0);
                field_0x84 = 0;
                gGameRoot2_8053AB10->fn_8013B4C0();
                field_0xa0->fn_80458F9C(0xA);
            }
            break;
        case 13:
            getGameView_802D6A9C()->fn_802DBD60();
            field_0x50 = 7;
            field_0x58 = 1;
            lbl_8053ACA0->fn_802AC8AC(0);
            field_0x84 = 0;
            break;
        case 14:
            getGameView_802D6A9C()->fn_802DBD60();
            field_0xa0->fn_80458880(0x38, -1);
            fn_80138B7C();
            field_0x50 = 7;
            field_0x58 = 1;
            lbl_8053ACA0->fn_802AC8AC(0);
            field_0x84 = 0;
            break;
        case 15:
            getGameView_802D6A9C()->fn_802DB6F0();
            field_0x50 = 7;
            field_0x58 = 1;
            lbl_8053ACA0->fn_802AC8AC(0);
            field_0x84 = 0;
            break;
        case 16:
            getGameView_802D6A9C()->fn_802DA8DC();
            field_0x50 = 7;
            field_0x58 = 1;
            lbl_8053ACA0->fn_802AC8AC(0);
            field_0x84 = 0;
            break;
        case 17:
            getGameView_802D6A9C()->fn_802DA788();
            field_0x4c = 0x12;
            break;
        case 18:
            if (!getGameView_802D6A9C()->fn_802DA630()) {
                field_0x58 = 4;
                field_0x4c = 7;
            }
            gGameRoot2_8053AB10->fn_8013BBE0();
            field_0x4->fn_80139D9C();
            break;
        case 19:
            gGameRoot2_8053AB10->fn_8013BBE0();
            field_0x4->fn_80139D9C();
            getGameView_802D6A9C()->fn_802DA010();
            field_0x50 = 7;
            field_0x58 = 1;
            lbl_8053ACA0->fn_802AC8AC(0);
            field_0x84 = 0;
            break;
        case 20:
            if (!getGameView_802D6A9C()->fn_802DA000()) {
                field_0x58 = 4;
                field_0x4c = 7;
            }
            gGameRoot2_8053AB10->fn_8013BBE0();
            field_0x4->fn_80139D9C();
            break;
        case 22: {
            s32 result = lbl_8053AEE0->fn_80441A1C();
            if (result == 1) {
                field_0x50 = result;
                field_0x58 = 1;
                lbl_8053ACA0->fn_802AC8AC(0);
                field_0x84 = 0;
                field_0x60 = 0;
                field_0x64 = 0;
            } else {
                field_0x4c = result;
            }
            break;
        }
        }
        if (field_0x58 == 2) {
            field_0x58 = 4;
        }
    } else if (loading != 0) {
        if (loading == 1) {
            fn_80135A04(field_0x50);
            field_0xfd = 2;
        }
        fn_80135494(field_0x50);
    } else if (lbl_8053ACA0->fn_802AC8B4()) {
        s32 state = field_0x58;
        if (state == 1) {
            field_0x7c = 0;
            if (field_0x4c == 9 || field_0x4c == 0x16 || field_0x84 != 0) {
                if (field_0x4 != NULL) {
                    field_0x4->fn_80139BE8();
                }
                gGameRoot2_8053AB10->fn_8013B4C0();
            }
            fn_80135A04(field_0x50);
            field_0x58 = 8;
            fn_80135494(field_0x50);
        } else if (state == 8) {
            fn_80135494(field_0x50);
        } else if (state == 2) {
            field_0x58 = 4;
        } else if (state == 4) {
            field_0x58 = 0;
        } else if (state == 5) {
            field_0xa0->fn_80458F9C(0xA);
            if (lbl_8053ACA8->fn_802B1DB0()) {
                field_0xa0->fn_804595E4(false);
            } else {
                field_0xa0->fn_804595E4(true);
            }
            field_0x58 = 6;
        } else if (state == 6) {
            if (field_0xa0->fn_8045959C() == 2) {
                field_0x58 = 7;
            }
        } else if (state == 7) {
            lbl_80529DEC.mSaveRegion.bytes[5] = 0xA5;
            if (lbl_8053ACA8->fn_802B1DB0()) {
                if (field_0x4 != NULL) {
                    field_0x4->fn_80139BE8();
                    delete field_0x4;
                    field_0x4 = NULL;
                }
                delete field_0x1c;
                field_0x1c = NULL;
                delete field_0x28;
                field_0x28 = NULL;
                delete field_0x24;
                field_0x24 = NULL;
                delete field_0x2c;
                field_0x2c = NULL;
                delete field_0x30;
                field_0x30 = NULL;
                delete field_0x20;
                field_0x20 = NULL;
                if (field_0x20 == NULL) {
                    u8 group = field_0x98->changeGroupID(1);
                    field_0x20 = new (field_0x98, 0) Title_804AF820();
                    field_0x98->changeGroupID(group);
                    field_0x87 = 0;
                }
                field_0xa0->fn_80459508();
                field_0x4c = 1;
                field_0x60 = 0;
                field_0x64 = 0;
                field_0x58 = 2;
                getEffectSystem_8013C824()->mEmitterManager->forceDeleteAllEmitter();
                lbl_8053ACA8->fn_802B1DE8();
                JUTGamePad::C3ButtonReset::sResetOccurred = false;
                JUTGamePad::C3ButtonReset::sCallback = fn_80139994;
                JUTGamePad::C3ButtonReset::sCallbackArg = NULL;
            } else {
                lbl_8053ACA8->fn_802B1BE0(0, 0, false);
            }
        }
    }
    field_0xa0->fn_80459674();
    if (field_0x95 == 1) {
        field_0x95 = 0;
        gAudioMgr_8053AF80->fn_80458658(false);
    }
    gGameRoot2_8053AB10->fn_8013BBBC();
    field_0xc->fn_80304FB0();
    gGameRoot_8053AAF8->mRandom.get();
}

// main calls it each frame between beginRender and endRender: draws the screen
// of the current mode (field_0x4c), then the fade objects.
// FIXME: 99.98 %, reloc-name; behaviour drift.
void GameRoot_8049EF20::fn_80136828() {
    j3dSys.drawInit();
    if (field_0x58 != 8 && field_0xfd != 2) {
        switch (field_0x4c) {
        case 1:
            if (lbl_8053AD88 != NULL) {
                lbl_8053AD88->fn_803A44D8();
            }
            gGameRoot2_8053AB10->fn_8013B4F8();
            break;
        case 2:
            lbl_8053ADD0->fn_803DF624();
            break;
        case 3:
            lbl_8053ADB0->fn_803CEED4();
            break;
        case 4:
            gGameRoot2_8053AB10->fn_8013BB64();
            gGameRoot2_8053AB10->fn_8013BB18();
            lbl_8053AD98->fn_803A8664();
            gGameRoot2_8053AB10->fn_8013B71C();
            gGameRoot2_8053AB10->fn_8013B618();
            break;
        case 5:
            lbl_8053AD90->fn_803A71F0();
            break;
        case 6:
            gGameRoot2_8053AB10->fn_8013BB64();
            gGameRoot2_8053AB10->fn_8013BB18();
            lbl_8053AD80->fn_8039013C();
            gGameRoot2_8053AB10->fn_8013B71C();
            gGameRoot2_8053AB10->fn_8013B618();
            lbl_8053AD80->fn_80390108();
            break;
        case 7:
        case 8:
            JFWDisplay::getManager()->setClearColor(JUtility::TColor(0x19, 0x19, 0x19, 0));
            gGameRoot2_8053AB10->fn_8013BB64();
            field_0x4->fn_80139D3C();
            gGameRoot2_8053AB10->fn_8013BB18();
            field_0x4->fn_80139CC4();
            gGameRoot2_8053AB10->fn_8013B71C();
            field_0x4->fn_80139C84();
            gGameRoot2_8053AB10->fn_8013B618();
            field_0x4->fn_80139C5C();
            lbl_8053AED0->fn_8043D39C();
            lbl_8053AED0->fn_8043DB58();
            lbl_8053ADC8->fn_803DA6B4();
            if (field_0x58 == 0) {
                switch (field_0x60) {
                case 0:
                case 1:
                case 2:
                case 3:
                    if (gGameRoot_8053AAF8->field_0x7e) {
                        if (!gGameRoot_8053AAF8->field_0x7f) {
                            // h declared first: the original keeps w and h in the other two saved FPRs.
                            f32 h, w;
                            w = 413.0f * field_0x8c;
                            h = 38.0f * field_0x8c;
                            field_0xb8->setAlpha(field_0x68);
                            field_0xb8->draw(304.0f - w / 2, 225.0f - h / 2, w, h, false, false, false);
                        } else {
                            J2DTextBox::TFontSize size;
                            size.x = 30.0f * field_0x8c;
                            size.y = 60.0f * field_0x8c;
                            field_0xc0->setFontSize(size);
                            field_0xc4->setFontSize(size);
                            if (gGameRoot_8053AAF8->field_0x7f) {
                                field_0xf4 = getMessageText_803832BC(field_0xc8, 0x42, 0, NULL, NULL);
                            } else {
                                field_0xf4 = getMessageText_803832BC(field_0xc8, 0x41, 0, NULL, NULL);
                            }
                            f32 y = size.y / 2 + 180.0f;
                            strcpy(field_0xc4->getStringPtr(), field_0xc8);
                            strcpy(field_0xc0->getStringPtr(), field_0xc8);
                            field_0xc4->setAlpha(field_0x68);
                            field_0xc0->setAlpha(field_0x68);
                            gGameRoot2_8053AB10->fn_8013B490();
                            field_0xc4->draw(-29997.0f, 3.0f + y,
                                             60000.0f + (s32)JUTVideo::getManager()->getFbWidth(),
                                             J2DHBIND_Center);
                            field_0xc0->draw(-30000.0f, y,
                                             60000.0f + (s32)JUTVideo::getManager()->getFbWidth(),
                                             J2DHBIND_Center);
                        }
                        if (field_0x60 >= 2) {
                            // h declared first: the original keeps w and h in the other two saved FPRs.
                            f32 h, w;
                            w = 256.0f * field_0x90;
                            h = 128.0f * field_0x90;
                            field_0xbc->setBlackWhite(JUtility::TColor(0x50282800), JUtility::TColor(0xFF5050FF));
                            field_0xbc->setAlpha(field_0x6c);
                            field_0xbc->draw(328.0f - w / 2, 164.0f - h / 2, w, h, false, false, false);
                        }
                    }
                    break;
                case 4:
                    break;
                }
            }
            break;
        case 9:
            JFWDisplay::getManager()->setClearColor(JUtility::TColor(0x19, 0x19, 0x19, 0));
            gGameRoot2_8053AB10->fn_8013BB64();
            field_0x4->fn_80139D3C();
            gGameRoot2_8053AB10->fn_8013BB18();
            field_0x4->fn_80139CC4();
            gGameRoot2_8053AB10->fn_8013B71C();
            field_0x4->fn_80139C84();
            gGameRoot2_8053AB10->fn_8013B618();
            field_0x4->fn_80139C5C();
            lbl_8053AED0->fn_8043D39C();
            lbl_8053AED0->fn_8043DB58();
            lbl_8053AD70->fn_8037AF80();
            gGameRoot2_8053AB10->fn_8013B4F8();
            lbl_8053AD70->fn_8037AF4C();
            break;
        case 10:
            JFWDisplay::getManager()->setClearColor(JUtility::TColor(0, 0, 0, 0));
            gGameRoot2_8053AB10->fn_8013BB64();
            field_0x4->fn_80139D3C();
            gGameRoot2_8053AB10->fn_8013BB18();
            if (!gGameRoot_8053AAF8->field_0x7f || gGameRoot_8053AAF8->field_0x7e == 1) {
                lbl_8053ACA0->fn_802AC98C();
            }
            field_0x4->fn_80139CC4();
            gGameRoot2_8053AB10->fn_8013B71C();
            field_0x4->fn_80139C84();
            gGameRoot2_8053AB10->fn_8013B618();
            field_0x4->fn_80139C5C();
            if (gGameRoot_8053AAF8->field_0x7f == 1 && gGameRoot_8053AAF8->field_0x7e == 0) {
                lbl_8053ACA0->fn_802AC98C();
            }
            break;
        case 11:
            JFWDisplay::getManager()->setClearColor(JUtility::TColor(0x19, 0x19, 0x19, 0));
            gGameRoot2_8053AB10->fn_8013BB64();
            field_0x4->fn_80139D3C();
            gGameRoot2_8053AB10->fn_8013BB18();
            field_0x4->fn_80139CC4();
            gGameRoot2_8053AB10->fn_8013B71C();
            field_0x4->fn_80139C84();
            gGameRoot2_8053AB10->fn_8013B538();
            lbl_8053AEC8->fn_804399A0();
            gGameRoot2_8053AB10->fn_8013B4F8();
            field_0x4->fn_80139C5C();
            lbl_8053ADC8->fn_803DA6B4();
            break;
        case 12:
            JFWDisplay::getManager()->setClearColor(JUtility::TColor(0, 0, 0, 0));
            if (gGameRoot_8053AAF8->field_0x7f) {
                gGameRoot2_8053AB10->fn_8013BB64();
                lbl_8053AD68->fn_8037A664();
                gGameRoot2_8053AB10->fn_8013BAF4();
                lbl_8053AD68->fn_8037A580();
                gGameRoot2_8053AB10->fn_8013B71C();
                gGameRoot2_8053AB10->fn_8013B618();
                lbl_8053ADC8->fn_803DA6B4();
            } else {
                gGameRoot2_8053AB10->fn_8013BB64();
                gGameRoot2_8053AB10->fn_8013B71C();
                lbl_8053AD68->fn_8037A664();
                gGameRoot2_8053AB10->fn_8013BAF4();
                lbl_8053AD68->fn_8037A580();
                gGameRoot2_8053AB10->fn_8013B618();
                lbl_8053ADC8->fn_803DA6B4();
            }
            break;
        case 13:
        case 14:
            gGameRoot2_8053AB10->fn_8013BB64();
            field_0x4->fn_80139D3C();
            gGameRoot2_8053AB10->fn_8013BB18();
            field_0x4->fn_80139CC4();
            gGameRoot2_8053AB10->fn_8013B71C();
            field_0x4->fn_80139C84();
            gGameRoot2_8053AB10->fn_8013B618();
            field_0x4->fn_80139C5C();
            break;
        case 17:
        case 18:
            JFWDisplay::getManager()->setClearColor(JUtility::TColor(0x19, 0x19, 0x19, 0));
            gGameRoot2_8053AB10->fn_8013BB64();
            field_0x4->fn_80139D3C();
            gGameRoot2_8053AB10->fn_8013BB18();
            field_0x4->fn_80139CC4();
            gGameRoot2_8053AB10->fn_8013B71C();
            field_0x4->fn_80139C84();
            gGameRoot2_8053AB10->fn_8013B618();
            break;
        case 22:
            JFWDisplay::getManager()->setClearColor(JUtility::TColor(0x19, 0x19, 0x19, 0));
            gGameRoot2_8053AB10->fn_8013BB64();
            if (field_0x1c != NULL) {
                if (gGameRoot_8053AAF8->field_0x7f) {
                    gGameRoot2_8053AB10->fn_8013BB64();
                    lbl_8053AD68->fn_8037A664();
                    gGameRoot2_8053AB10->fn_8013BAF4();
                    lbl_8053AD68->fn_8037A580();
                    gGameRoot2_8053AB10->fn_8013B71C();
                    gGameRoot2_8053AB10->fn_8013B618();
                    lbl_8053ADC8->fn_803DA6B4();
                } else {
                    gGameRoot2_8053AB10->fn_8013BB64();
                    gGameRoot2_8053AB10->fn_8013B71C();
                    lbl_8053AD68->fn_8037A664();
                    gGameRoot2_8053AB10->fn_8013BAF4();
                    lbl_8053AD68->fn_8037A580();
                    gGameRoot2_8053AB10->fn_8013B618();
                    lbl_8053ADC8->fn_803DA6B4();
                }
            } else {
                if (field_0x4 != NULL) {
                    field_0x4->fn_80139D3C();
                }
                gGameRoot2_8053AB10->fn_8013BB18();
                if (field_0x4 != NULL) {
                    field_0x4->fn_80139CC4();
                }
                if (field_0x30 != NULL) {
                    lbl_8053AD80->fn_8039013C();
                }
                gGameRoot2_8053AB10->fn_8013B71C();
                if (field_0x4 != NULL) {
                    field_0x4->fn_80139C84();
                }
                gGameRoot2_8053AB10->fn_8013B618();
                if (field_0x30 != NULL) {
                    lbl_8053AD80->fn_80390108();
                }
                if (field_0x4 != NULL) {
                    field_0x4->fn_80139C5C();
                    lbl_8053AED0->fn_8043D39C();
                    lbl_8053AED0->fn_8043DB58();
                    if (lbl_8053AD70->field_0xc) {
                        lbl_8053AD70->fn_8037AF80();
                        gGameRoot2_8053AB10->fn_8013B4F8();
                        lbl_8053AD70->fn_8037AF4C();
                    }
                }
            }
            fn_80119DD0()->fn_80119628();
            fn_80119DD0()->fn_801194C4();
            lbl_8053ADC8->fn_803DA6B4();
            lbl_8053AEE0->fn_80442200();
            break;
        }
    }
    if (field_0x58 != 0) {
        lbl_8053ACA0->fn_802ACA64();
    }
    if (field_0x4c != 22) {
        fn_80119DD0()->fn_80119628();
        fn_80119DD0()->fn_801194C4();
    }
}

void GameRoot_8049EF20::fn_801367E0(s32 value) {
    if (value == 10) {
        lbl_8053ACA0->fn_802AC980();
    }
    field_0x4c = value;
}

void GameRoot_8049EF20::fn_80136798(s32 a, s32 b) {
    field_0x50 = a;
    field_0x58 = 1;
    lbl_8053ACA0->fn_802AC8AC(b);
    field_0x84 = 0;
}

void GameRoot_8049EF20::fn_8013670C(s32 value) {
    field_0x60 = 0;
    field_0x64 = 0;
    if (lbl_80529DEC.field_0x118 == 10 && lbl_80529DEC.field_0x11c == 2) {
        lbl_80529DEC.field_0x118 = lbl_80529DEC.field_0x120;
        lbl_80529DEC.field_0x11c = lbl_80529DEC.field_0x124;
        lbl_80529DEC.field_0x237 = 7;
    }
    field_0x50 = value;
    field_0x58 = 1;
    lbl_8053ACA0->fn_802AC8AC(0);
    field_0x84 = 0;
}

void GameRoot_8049EF20::fn_8013665C(s32 a, s32 b) {
    fn_801365E8(a, b, 0);
    if (a == 10 && b == 2) {
        if (field_0x4c == 9) {
            lbl_80529DEC.field_0x24b = 2;
        } else {
            lbl_80529DEC.field_0x24b = 1;
        }
        lbl_80529DEC.field_0x237 = 7;
    }
}

void GameRoot_8049EF20::fn_801365E8(s32 a, s32 b, s32 c) {
    field_0x50 = 7;
    field_0x58 = 1;
    lbl_8053ACA0->fn_802AC8AC(c);
    lbl_80529DEC.field_0x118 = a;
    lbl_80529DEC.field_0x11c = b;
    field_0x84 = 1;
}

void GameRoot_8049EF20::fn_801365C8(u8 value) {
    field_0xfe = value;
    if (value != 1) {
        return;
    }
    field_0xfc = 0;
    field_0xf8 = 0;
}

s32 fn_801365A8() {
    return fn_80021840();
}

void GameRoot_8049EF20::fn_80136500() {
    u8 stopped = gGameRoot_8053AAF8->field_0x94;
    if (gGameRoot_8053AAF8->field_0x96 != 0) {
        stopped = true;
    }
    if (gGameRoot_8053AAF8->field_0x7e == 0 && fn_8037F1E4(0)) {
        stopped = true;
    }
    if (field_0xfe == 1 && !stopped) {
        field_0xfc++;
        if (field_0xfc >= 60) {
            field_0xfc = 0;
            field_0xf8++;
        }
    }
}

// FIXME(shotgun): 97.67 %, missing/extra; behaviour clean.
// 0x80136364
void GameRoot_8049EF20::fn_80136364() {
    if (field_0xfe != 1 || getGameView_802D6A9C()->fn_802DCD0C() || gGameRoot_8053AAF8->field_0x4c == GAME_MODE_B) {
        return;
    }
    if (lbl_8053AD30->fn_80309C4C(0x1000, -1) == 0) {
        return;
    }
    if (lbl_8053AD70->fn_8037AD34() || lbl_8053AD70->fn_8037ACC4()) {
        return;
    }
    if (gGameRoot_8053AAF8->field_0x85 == 1) {
        if (field_0x54 == 7) {
            s32 sub = field_0x78;
            s32 stage = field_0x74;
            field_0x50 = 7;
            field_0x58 = 1;
            lbl_8053ACA0->fn_802AC8AC(0);
            lbl_80529DEC.field_0x118 = stage;
            lbl_80529DEC.field_0x11c = sub;
            field_0x84 = 1;
        } else {
            field_0x50 = field_0x54;
            field_0x58 = 1;
            lbl_8053ACA0->fn_802AC8AC(0);
            field_0x84 = 0;
        }
    } else if (!fn_802DEBF8() && lbl_8053AD70->fn_8037ADD8()) {
        field_0x4c = 9;
        lbl_8053AD70->fn_8037ADA4();
        lbl_8053AD70->field_0xc = 1;
        field_0xa0->fn_80458880(1, -1);
        field_0xa0->fn_804593D4();
        JUTGamePad* pad = JUTGamePad::getGamePad(0);
        field_0x10c = 0;
        field_0x108 = 0;
        pad->mRumble.stopMotor(pad->getPortNum(), false);
        pad->mRumble.stopPatternedRumble(pad->getPortNum());
    }
}

// Whether the game mode (field_0x4c) is one of 0 - 6, 9 or 12.
// FIXME: 85.88 %, missing/extra; behaviour drift.
// The target tests mode == 0 separately and then folds 1-6 into one
// range (subi; cmplwi 5), reusing r3 for the mode; ours folds 0-6 into one range.
bool GameRoot_8049EF20::fn_80136320() {
    if (gGameRoot_8053AAF8 != NULL) {
        s32 mode = field_0x4c;
        if (mode == 0 || (mode >= 1 && mode <= 6) || mode == 9 || mode == 12) {
            return true;
        }
    }
    return false;
}

// Switches to game mode `mode`: deletes the other modes' screens, reports the
// heap and starts loading the mode's archives (fn_80401E10) or screen.
// FIXME: 99.78 %, reloc-name; behaviour drift.
// Code identical; jump table @NNN vs jumptable_8049EF94 (as fn_80135020).
void GameRoot_8049EF20::fn_80135A04(s32 mode) {
    if (field_0x84 != 0) {
        fn_801352F4(field_0x4c);
        field_0x84 = 0;
    }
    switch (mode) {
    case 7:
        if (field_0x4 == NULL) {
            fn_801352F4(1);
            fn_801352F4(2);
            fn_801352F4(3);
            fn_801352F4(4);
            fn_801352F4(6);
            fn_801352F4(12);
            OSReport("Game　連続空き容量　%fK 残り空き容量　%fK\n",
                     field_0x98->getFreeSize() / 1024.0f, (s32)field_0x98->getTotalFreeSize() / 1024.0f);
            if (gGameRoot_8053AAF8->field_0x7f != 0) {
                char path[32];
                if (fn_80021840() == 2) {
                    strcpy(path, "/GC4Sword/Boss/");
                } else if (fn_80021840() == 0 || fn_80021840() == 1) {
                    strcpy(path, "/GC4Sword_usa/Boss/");
                }
                strcat(path, lbl_8049ED88.boss[lbl_80529DEC.field_0x118][lbl_80529DEC.field_0x11c]);
                field_0xac = fn_80401E10(path, 0, lbl_80529DEC.mHeap);
            } else if (fn_80021840() == 2) {
                field_0xac = fn_80401E10("/GC4Sword/Boss/boss500.arc", 0, lbl_80529DEC.mHeap);
            } else if (fn_80021840() == 0 || fn_80021840() == 1) {
                field_0xac = fn_80401E10("/GC4Sword_usa/Boss/boss500.arc", 0, lbl_80529DEC.mHeap);
            }
            if (fn_80021840() == 2) {
                field_0xb0 = fn_80401E10("/GC4Sword/game2D.arc", 0, lbl_80529DEC.mHeap);
            } else if (fn_80021840() == 0 || fn_80021840() == 1) {
                field_0xb0 = fn_80401E10("/GC4Sword_usa/game2D.arc", 0, lbl_80529DEC.mHeap);
            }
        }
        break;
    case 12:
        getEffectSystem_8013C824()->mEmitterManager->forceDeleteAllEmitter();
        fn_801352F4(1);
        fn_801352F4(2);
        fn_801352F4(3);
        fn_801352F4(4);
        fn_801352F4(6);
        fn_801352F4(7);
        OSReport("Result　連続空き容量　%fK 残り空き容量　%fK\n",
                 field_0x98->getFreeSize() / 1024.0f, (s32)field_0x98->getTotalFreeSize() / 1024.0f);
        fn_80135020(12);
        break;
    case 1:
        fn_801352F4(7);
        fn_801352F4(2);
        fn_801352F4(3);
        fn_801352F4(4);
        fn_801352F4(6);
        fn_801352F4(12);
        OSReport("Title　連続空き容量　%fK 残り空き容量　%fK\n",
                 field_0x98->getFreeSize() / 1024.0f, (s32)field_0x98->getTotalFreeSize() / 1024.0f);
        fn_80135020(1);
        break;
    case 2:
    case 5:
        if (field_0x4c == 2 || field_0x4c == 5) {
            break;
        }
        fn_801352F4(7);
        fn_801352F4(1);
        fn_801352F4(3);
        fn_801352F4(4);
        fn_801352F4(6);
        fn_801352F4(12);
        OSReport("ModeSelect　連続空き容量　%fK 残り空き容量　%fK\n",
                 field_0x98->getFreeSize() / 1024.0f, (s32)field_0x98->getTotalFreeSize() / 1024.0f);
        if (fn_80021840() == 2) {
            field_0xac = fn_80401E10("/GC4Sword/gamemode.arc", 0, lbl_80529DEC.mHeap);
        } else if (fn_80021840() == 0 || fn_80021840() == 1) {
            field_0xac = fn_80401E10("/GC4Sword_usa/gamemode.arc", 0, lbl_80529DEC.mHeap);
        }
        field_0xb0 = NULL;
        break;
    case 3:
        fn_801352F4(7);
        fn_801352F4(1);
        fn_801352F4(2);
        fn_801352F4(4);
        fn_801352F4(6);
        fn_801352F4(12);
        OSReport("FileSelect　連続空き容量　%fK 残り空き容量　%fK\n",
                 field_0x98->getFreeSize() / 1024.0f, (s32)field_0x98->getTotalFreeSize() / 1024.0f);
        if (fn_80021840() == 2) {
            field_0xac = fn_80401E10("/GC4Sword/file.arc", 0, lbl_80529DEC.mHeap);
        } else if (fn_80021840() == 0 || fn_80021840() == 1) {
            field_0xac = fn_80401E10("/GC4Sword_usa/file.arc", 0, lbl_80529DEC.mHeap);
        }
        field_0xb0 = NULL;
        break;
    case 4:
        fn_801352F4(7);
        fn_801352F4(1);
        fn_801352F4(2);
        fn_801352F4(3);
        fn_801352F4(6);
        fn_801352F4(12);
        OSReport("Menu　連続空き容量　%fK 残り空き容量　%fK\n",
                 field_0x98->getFreeSize() / 1024.0f, (s32)field_0x98->getTotalFreeSize() / 1024.0f);
        if (fn_80021840() == 2) {
            field_0xac = fn_80401E10("/GC4Sword/entry.arc", 0, lbl_80529DEC.mHeap);
        } else if (fn_80021840() == 0 || fn_80021840() == 1) {
            field_0xac = fn_80401E10("/GC4Sword_usa/entry.arc", 0, lbl_80529DEC.mHeap);
        }
        field_0xb0 = NULL;
        break;
    case 6:
        fn_801352F4(7);
        fn_801352F4(1);
        fn_801352F4(2);
        fn_801352F4(3);
        fn_801352F4(4);
        fn_801352F4(12);
        OSReport("MapSelect　連続空き容量　%fK 残り空き容量　%fK\n",
                 field_0x98->getFreeSize() / 1024.0f, (s32)field_0x98->getTotalFreeSize() / 1024.0f);
        if (fn_80021840() == 2) {
            if (gGameRoot_8053AAF8->field_0x7f != 0) {
                field_0xac = fn_80401E10("/GC4Sword/map1.arc", 0, lbl_80529DEC.mHeap);
            } else {
                field_0xac = fn_80401E10("/GC4Sword/map2.arc", 0, lbl_80529DEC.mHeap);
            }
        } else if (fn_80021840() == 0 || fn_80021840() == 1) {
            if (gGameRoot_8053AAF8->field_0x7f != 0) {
                field_0xac = fn_80401E10("/GC4Sword_usa/map1.arc", 0, lbl_80529DEC.mHeap);
            } else {
                field_0xac = fn_80401E10("/GC4Sword_usa/map2.arc", 0, lbl_80529DEC.mHeap);
            }
        }
        field_0xb0 = NULL;
        break;
    }
}

// The stage music request of the stage-change path (also for mode 0xE).
static inline void playStageMusic(GameRoot_8049EF20* self) {
    u8 count = getGameView_802D6A9C()->field_0x1328c;
    s32 sound = getGameView_802D6A9C()->field_0x34;
    if (sound == 0x11) {
        return;
    }
    if (gGameRoot_8053AAF8->field_0x7f == 1) {
        if (sound == 0x14 || sound == 0x1E) {
            self->field_0xa0->fn_80458F04(lbl_804652F0[sound]);
        } else {
            self->field_0xa0->fn_80458FF0(lbl_804652F0[sound], false, count);
            getGameView_802D6A9C()->field_0x1328c = count + 1;
        }
    } else if (sound != 0x19) {
        if (self->field_0x86 == 0) {
            self->field_0xa0->fn_80458FF0(0x1000004, false, 0);
        } else {
            self->field_0xa0->fn_80458FF0(0x100000D, false, 0);
        }
    } else {
        self->field_0xa0->fn_80458FF0(0x1000019, false, 0);
    }
}

// FIXME(shotgun): 93.69 %, missing/extra; behaviour drift.
// 0x80135494
void GameRoot_8049EF20::fn_80135494(s32 mode) {
    bool changed = false;
    bool ready = false;
    if (mode == 2 && field_0x4c != 2 && field_0x4c != 5) {
        ready = true;
    } else if (mode == 5 && field_0x4c != 2 && field_0x4c != 5) {
        ready = true;
    } else if (mode == 3 || mode == 4 || mode == 6) {
        ready = true;
    } else if (mode == 7 && field_0x4 == 0) {
        ready = true;
    }
    if (ready) {
        if (field_0xac->field_0x4 != 1) {
            return;
        }
        if (field_0xb0 != NULL && field_0xb0->field_0x4 != 1) {
            return;
        }
        switch (mode) {
        case 7:
            lbl_80529DEC.field_0x10 = field_0xac->field_0x14;
            lbl_80529DEC.field_0x4c = field_0xb0->field_0x14;
            fn_80135020(7);
            break;
        case 2:
        case 5:
            if (field_0x4c != 2 && field_0x4c != 5) {
                lbl_80529DEC.field_0x2c = field_0xac->field_0x14;
                fn_80135020(2);
            }
            break;
        case 3:
            lbl_80529DEC.field_0x30 = field_0xac->field_0x14;
            fn_80135020(3);
            break;
        case 4:
            lbl_80529DEC.field_0x28 = field_0xac->field_0x14;
            fn_80135020(4);
            break;
        case 6:
            lbl_80529DEC.field_0x34 = field_0xac->field_0x14;
            fn_80135020(6);
            lbl_8053AD80->fn_80390088();
            break;
        }
        delete field_0xac;
        delete field_0xb0;
        changed = true;
    } else if (mode == 12 && lbl_80529DEC.field_0x24d == 1 && field_0x7c < 0x78) {
        field_0x7c++;
        return;
    }
    if (!changed) {
        if (field_0x50 == 1 || field_0x50 == 5 || field_0x50 == 2 || field_0x50 == 12) {
            field_0x60 = 0;
        }
    }
    field_0x4c = field_0x50;
    if (field_0x4c == 1 || field_0x4c == 6) {
        gGameRoot2_8053AB10->field_0x2c->fn_80158B3C();
    }
    field_0x64 = 0;
    if (field_0xfd == 0) {
        if (field_0x50 == 0x11) {
            field_0x58 = 3;
        } else {
            field_0x58 = 2;
        }
    } else {
        field_0xfd = 0;
    }
    if (field_0x4c != 7) {
        return;
    }
    if (changed) {
        gGameRoot_8053AAF8->field_0xfe = 1;
        gGameRoot_8053AAF8->field_0xfc = 0;
        gGameRoot_8053AAF8->field_0xf8 = 0;
        if (getGameView_802D6A9C()->field_0x3c != 0) {
            field_0x60 = 0;
            return;
        }
        field_0x60 = 4;
        if (getGameView_802D6A9C()->field_0x4 == 0 && getGameView_802D6A9C()->field_0x8 == 0) {
            if (getGameView_802D6A9C()->field_0x4 != 0 || getGameView_802D6A9C()->field_0x8 != 0) {
                return;
            }
            if (!(lbl_8050EC80.getCurrentFile().mFlags & 1)) {
                return;
            }
        }
        playStageMusic(this);
    } else if (field_0x50 == 0xE) {
        field_0x60 = 4;
        playStageMusic(this);
    }
}

// Deletes the screen object of game mode `mode` (see fn_80135020).
// FIXME: 99.86 %, reloc-name; behaviour drift.
// Code identical; jump table @NNN vs jumptable_8049EF60 (as fn_80135020).
void GameRoot_8049EF20::fn_801352F4(s32 mode) {
    switch (mode) {
    case 7:
        if (field_0x4 != NULL) {
            field_0x4->fn_80139BE8();
            delete field_0x4;
            field_0x4 = NULL;
        }
        break;
    case 12:
        if (field_0x1c != NULL) {
            delete field_0x1c;
            field_0x1c = NULL;
        }
        break;
    case 1:
        if (field_0x20 != NULL) {
            delete field_0x20;
            field_0x20 = NULL;
        }
        break;
    case 2:
    case 5:
        if (field_0x28 != NULL) {
            delete field_0x28;
            field_0x28 = NULL;
        }
        break;
    case 3:
        if (field_0x24 != NULL) {
            delete field_0x24;
            field_0x24 = NULL;
        }
        break;
    case 4:
        if (field_0x2c != NULL) {
            delete field_0x2c;
            field_0x2c = NULL;
        }
        break;
    case 6:
        if (field_0x30 != NULL) {
            delete field_0x30;
            field_0x30 = NULL;
        }
        break;
    }
}

// Creates the screen object of game mode `mode` unless it exists; the menu
// screens are made in field_0x98 under heap group ID 1.
// FIXME: 99.92 %, reloc-name; behaviour drift.
// Code identical; the switch's jump table is @NNN, not
// jumptable_8049EF2C (the .data order puts it after the vtable, project-wide
// jump-table order issue, docs/GAME_PLAN.md).
void GameRoot_8049EF20::fn_80135020(s32 mode) {
    switch (mode) {
    case 7:
        if (field_0x4 == NULL) {
            field_0x4 = new (field_0x98, 0) GameRootSub04_801F856C(field_0x98);
            gGameRoot2_8053AB10->fn_8013BE04();
            // The original loads both values before getGameView_802D6A9C() (level first).
            if (gGameRoot_8053AAF8->field_0x7f != 0) {
                s32 level = lbl_80529DEC.field_0x11c;
                s32 stage = lbl_80529DEC.field_0x118;
                GameView_804A9950* view = getGameView_802D6A9C();
                view->field_0x4 = stage;
                view->field_0x8 = level;
            } else {
                s32 level = lbl_80529DEC.field_0x12c;
                s32 stage = lbl_80529DEC.field_0x128;
                GameView_804A9950* view = getGameView_802D6A9C();
                view->field_0x4 = stage;
                view->field_0x8 = level;
            }
            field_0x4->fn_8013A1A4();
        }
        break;
    case 12:
        if (field_0x1c == NULL) {
            u8 group = field_0x98->changeGroupID(1);
            field_0x1c = new (field_0x98, 0) cls_804AE9A0();
            field_0x98->changeGroupID(group);
            fn_80139958();
        }
        break;
    case 1:
        if (field_0x20 == NULL) {
            u8 group = field_0x98->changeGroupID(1);
            field_0x20 = new (field_0x98, 0) Title_804AF820();
            field_0x98->changeGroupID(group);
            field_0x87 = 0;
        }
        break;
    case 2:
    case 5:
        if (field_0x28 == NULL) {
            u8 group = field_0x98->changeGroupID(1);
            field_0x28 = new (field_0x98, 0) GameMode_804B0688();
            field_0x98->changeGroupID(group);
        }
        break;
    case 3:
        if (field_0x24 == NULL) {
            u8 group = field_0x98->changeGroupID(1);
            field_0x24 = new (field_0x98, 0) FileSelect_804AFF60();
            field_0x98->changeGroupID(group);
        }
        break;
    case 4:
        if (field_0x2c == NULL) {
            u8 group = field_0x98->changeGroupID(1);
            field_0x2c = new (field_0x98, 0) cls_804AF878();
            field_0x98->changeGroupID(group);
        }
        break;
    case 6:
        if (field_0x30 == NULL) {
            u8 group = field_0x98->changeGroupID(1);
            field_0x30 = new (field_0x98, 0) cls_804AF480();
            field_0x98->changeGroupID(group);
        }
        break;
    }
}

// FIXME: 93.12 %, missing/extra; behaviour clean.
// The target reloads field_0x94 into r4 and passes it on unconverted
// (no li r4, 1); ours loads it into r0 and materialises the true.
void GameRoot_8049EF20::fn_80134FE0(u8 value) {
    field_0x94 = value;
    if (field_0x94 == 1) {
        gAudioMgr_8053AF80->fn_80458658(true);
    } else {
        field_0x95 = 1;
    }
}

void GameRoot_8049EF20::fn_80134FC4(u8 value) {
    field_0x94 = value;
    if (field_0x94 == 1) {
        return;
    }
    field_0x95 = 1;
}

void GameRoot_8049EF20::fn_80134FBC(u8 value) {
    field_0x96 = value;
}

// FIXME: 99.79 %, arg_mismatch on the rumble pattern offsets (0xD0, 0x158..0x188); behaviour clean.
// 0x80134DE4
void GameRoot_8049EF20::fn_80134DE4(s32 kind, s32 frames) {
    cls_8049ED88* table = &lbl_8049ED88;
    if (lbl_80529DEC.field_0x246 == 0) {
        return;
    }
    JUTGamePad* pad = JUTGamePad::getGamePad(0);
    field_0x10c = 0;
    if (kind == 1) {
        field_0x108 = 0x1E;
    } else if (kind == 2) {
        field_0x108 = 0x3C;
    } else if (kind == 3) {
        field_0x108 = 0x5A;
    } else if (kind == 4) {
        field_0x108 = 0x78;
    } else if (kind == 5) {
        pad->mRumble.startPatternedRumble(table->lbl_8049EE58, JUTGamePad::CRumble::ERumble(0), 0x3C);
        field_0x10c = 1;
        field_0x108 = 0x41A;
    } else if (kind == 6) {
        pad->mRumble.startPatternedRumble(table->lbl_8049EEE0, JUTGamePad::CRumble::ERumble(0), 0x3C);
        field_0x10c = 1;
        field_0x108 = 0x5B;
    } else if (kind == 7) {
        pad->mRumble.startPatternedRumble(table->lbl_8049EEF0, JUTGamePad::CRumble::ERumble(0), 0x3C);
        field_0x10c = 1;
        field_0x108 = 0x5B;
    } else if (kind == 8) {
        pad->mRumble.startPatternedRumble(table->lbl_8049EF00, JUTGamePad::CRumble::ERumble(0), 0x3C);
        field_0x10c = 1;
        field_0x108 = 0x5B;
    } else if (kind == 9) {
        pad->mRumble.startPatternedRumble(table->lbl_8049EF10, JUTGamePad::CRumble::ERumble(0), 0x3C);
        field_0x10c = 1;
        field_0x108 = 0x5B;
    } else if (kind == 10) {
        s32 value = frames;
        if (value == 0) {
            value = 0x78;
        }
        field_0x108 = value;
    } else {
        field_0x108 = kind;
    }
}

void GameRoot_8049EF20::fn_80134DC0() {
    if (field_0x86 == 0) {
        field_0x86 = 1;
    } else {
        field_0x86 = 0;
    }
}

void GameRoot_8049EF20::fn_80134D20(bool arg) {
    field_0x4c = 9;
    lbl_8053AD70->fn_8037ADA4();
    lbl_8053AD70->field_0xc = 1;
    field_0xa0->fn_80458880(1, -1);
    if (arg) {
        field_0xa0->fn_804593D4();
    }
    JUTGamePad* pad = JUTGamePad::getGamePad(0);
    field_0x10c = 0;
    field_0x108 = 0;
    JUTGamePad::CRumble::stopMotor(pad->mPortNum, false);
    pad->mRumble.stopPatternedRumble(pad->mPortNum);
}

void GameRoot_8049EF20::fn_80134D14(u8 value) {
    gGameRoot_8053AAF8->field_0x88 = value;
}

// Whether the pause fade object (gGameRoot2_8053AB10->0x2C) is idle, while paused.
u8 GameRoot_8049EF20::fn_80134CC8() {
    bool paused;
    if (field_0x4c == 11) {
        paused = true;
    } else {
        paused = field_0x88 == 2;
    }
    if (paused) {
        return gGameRoot2_8053AB10->field_0x2c->field_0xc == 0;
    }
    return false;
}

void GameRoot_8049EF20::fn_80134CC0(s32 value) {
    field_0x108 = value;
}

// The JUTException pre-user callback the constructor installs: the crash
// screen waits until pad 1 enters Up, A, Left, X, Down, Y, Right, B, Z, Start.
void fn_80134C04(OSError error, OSContext* context, u32 dsisr, u32 dar) {
    static const u16 lbl_804655FC[] = {
        PAD_BUTTON_UP, PAD_BUTTON_A,     PAD_BUTTON_LEFT, PAD_BUTTON_X,  PAD_BUTTON_DOWN, PAD_BUTTON_Y,
        PAD_BUTTON_RIGHT, PAD_BUTTON_B, PAD_TRIGGER_Z,   PAD_BUTTON_START, 0,
    };
    int i = 0;
    JUTException::waitTime(200);
    JUTException::getManager()->setGamePad((JUTGamePad*)-1);
    while (lbl_804655FC[i] != 0) {
        JUTException::waitTime(100);
        u32 trigger;
        JUTException::getManager()->readPad(&trigger, NULL);
        if (trigger != 0) {
            if (trigger == lbl_804655FC[i]) {
                i++;
            } else {
                i = 0;
            }
        }
    }
    JUTException::getManager()->setGamePad(NULL);
}
