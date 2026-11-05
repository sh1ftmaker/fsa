#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/Message.h"
#include "game/cls_804A8390.h"
#include "game/cls_804AE9B0.h"
#include "game/cls_804AF480.h"
#include "game/cls_804AF878.h"
#include "game/cls_804B19F0.h"
#include "game/act/tu_803DA6B4.h"
#include "game/act/tu_80440BF4.h"
#include "game/tu_80379F84.h"
#include "game/tu_803A3484.h"
#include "game/tu_803BC6F0.h"
#include "game/tu_803DAC3C.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/J2D/J2DPane.h"
#include "JSystem/J2D/J2DPicture.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "JSystem/JUtility/JUTVideo.h"
#include <string.h>
#include "JSystem/JUtility/JUTException.h"
#include "JSystem/JUtility/JUTGamePad.h"

void fn_80139958();

// The boss archives by stage and level (lbl_80529DEC's field_0x118 /
// field_0x11c; fn_80135A04).
static const char* lbl_8049ED88[13][4] = {
    "boss010.arc", "boss011.arc", "boss200.arc", "boss013.arc",
    "boss020.arc", "boss021.arc", "boss201.arc", "boss023.arc",
    "boss030.arc", "boss031.arc", "boss202.arc", "boss033.arc",
    "boss040.arc", "boss041.arc", "boss203.arc", "boss043.arc",
    "boss050.arc", "boss051.arc", "boss204.arc", "boss053.arc",
    "boss060.arc", "boss061.arc", "boss205.arc", "boss063.arc",
    "boss070.arc", "boss071.arc", "boss206.arc", "boss073.arc",
    "boss080.arc", "boss207.arc", "boss081.arc", "boss083.arc",
    "boss002.arc", "boss001.arc", "boss000.arc", "boss990.arc",
    "boss004.arc", "boss005.arc", "boss006.arc", "boss991.arc",
    "boss007.arc", "boss008.arc", "boss009.arc", "boss992.arc",
    "boss090.arc", "boss091.arc", "boss092.arc", "boss093.arc",
    "boss094.arc", "boss095.arc", "boss096.arc", "boss097.arc",
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

void GameRoot_8049EF20::fn_80134CC0(s32 value) {
    field_0x108 = value;
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

void GameRoot_8049EF20::fn_80134D14(u8 value) {
    gGameRoot_8053AAF8->field_0x88 = value;
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

void GameRoot_8049EF20::fn_80134DC0() {
    if (field_0x86 == 0) {
        field_0x86 = 1;
    } else {
        field_0x86 = 0;
    }
}

void GameRoot_8049EF20::fn_80134FBC(u8 value) {
    field_0x96 = value;
}

void GameRoot_8049EF20::fn_80134FC4(u8 value) {
    field_0x94 = value;
    if (field_0x94 == 1) {
        return;
    }
    field_0x95 = 1;
}

// FIXME: 93.1. The target reloads field_0x94 into r4 and passes it on unconverted
// (no li r4, 1); ours loads it into r0 and materialises the true.
void GameRoot_8049EF20::fn_80134FE0(u8 value) {
    field_0x94 = value;
    if (field_0x94 == 1) {
        gAudioMgr_8053AF80->fn_80458658(true);
    } else {
        field_0x95 = 1;
    }
}

// Creates the screen object of game mode `mode` unless it exists; the menu
// screens are made in field_0x98 under heap group ID 1.
// FIXME: 99.9. Code identical; the switch's jump table is @NNN, not
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

// Deletes the screen object of game mode `mode` (see fn_80135020).
// FIXME: 99.9. Code identical; jump table @NNN vs jumptable_8049EF60 (as fn_80135020).
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

// Switches to game mode `mode`: deletes the other modes' screens, reports the
// heap and starts loading the mode's archives (fn_80401E10) or screen.
// FIXME: 99.8. Code identical; jump table @NNN vs jumptable_8049EF94 (as fn_80135020).
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
                strcat(path, lbl_8049ED88[lbl_80529DEC.field_0x118][lbl_80529DEC.field_0x11c]);
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

// Whether the game mode (field_0x4c) is one of 0 - 6, 9 or 12.
// FIXME: 85.9. The target tests mode == 0 separately and then folds 1-6 into one
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

void fn_801365A8() {
    fn_80021840();
}

void GameRoot_8049EF20::fn_801365C8(u8 value) {
    field_0xfe = value;
    if (value != 1) {
        return;
    }
    field_0xfc = 0;
    field_0xf8 = 0;
}

void GameRoot_8049EF20::fn_801365E8(s32 a, s32 b, s32 c) {
    field_0x50 = 7;
    field_0x58 = 1;
    lbl_8053ACA0->fn_802AC8AC(c);
    lbl_80529DEC.field_0x118 = a;
    lbl_80529DEC.field_0x11c = b;
    field_0x84 = 1;
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

void GameRoot_8049EF20::fn_80136798(s32 a, s32 b) {
    field_0x50 = a;
    field_0x58 = 1;
    lbl_8053ACA0->fn_802AC8AC(b);
    field_0x84 = 0;
}

void GameRoot_8049EF20::fn_801367E0(s32 value) {
    if (value == 10) {
        lbl_8053ACA0->fn_802AC980();
    }
    field_0x4c = value;
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

// Called once by main after the constructor: resets the per-game state and
// picks the first game mode (the title after power-on, see SaveRegion 0x4/0x5).
// FIXME: 99.9. Code identical; its .sdata2 floats (0.0f, 1.0f, 50.0f) are @NNN
// until the unit's earlier float users are written and .sdata2 is split, and
// ours numbers 50.0f before get_ufloat_1's 1.0f (the original pools 1.0f first).
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

// FIXME: 99.65. Instructions identical; the string offsets off lbl_804652F0
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

void fn_80139958() {
    lbl_8053AD68->fn_8037A708();
}

// The 2-bit field of field_0x83 for `index` (0-3), as the caller reads it.
s32 fn_8013997C(s32 index) {
    return (gGameRoot_8053AAF8->field_0x83 >> (index * 2)) & 3;
}

// JUTGamePad C3ButtonReset callback (JUTResetBtnCb).
void fn_80139994(int port, void* arg) {
    lbl_8053ACA8->fn_802B1610(port, arg);
}
