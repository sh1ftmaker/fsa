// Crystal (cutscene) actor: 'DCR2', spawned by tu_801F9484.cpp.
// Declared in include/game/act/tu_801F9484.h (cls_804AE088).
// Inherits from cls_8049D140 (tu_8010DF44).

#include "game/ActorBase.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/act/tu_801F9484.h"
#include "game/act/tu_8010DF44.h"
#include "game/SpriteAnim.h"
#include "game/Unk_8045A080.h"
#include "JSystem/J2D/J2DPicture.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "JSystem/JMath/JMath.h"

// FIXME(shotgun): not written, .rodata tables lbl_80474C38 (string),
// lbl_80474C68 (0x1E0 table), lbl_80474E48 (two filenames) and the .sdata/.sdata2 anim tables are not
// declared: nothing compiled here reads them yet.
// FIXME(shotgun): not written: init, vfunc_0x60 (jumptable_804AE184),
// fn_8036AC58, fn_8036AA60, fn_8036A894, fn_8036A65C, fn_80369DDC (jumptable_804AE13C),
// fn_80368CD8 (jumptable_804AE0EC), fn_80368660, fn_803683AC, fn_80368198.

// Functions are listed in descending address order (highest first, lowest last).

s32 cls_804AE088::vfunc_0x58() {
    return field_0x240;
}

// FIXME: 98.05 %, scheduling + reloc-name; behaviour clean.
cls_804AE088::cls_804AE088()
    : mNaviActor(NULL), field_0x234(1), field_0x235(1), field_0x236(0), field_0x237(0), field_0x238(1),
      field_0x23c(0), field_0x240(0), field_0x244(0), field_0x248(0), field_0x24c(0), field_0x250(0.0f),
      field_0x254(0), field_0x258(0), field_0x25c(-1), field_0x26c(0), field_0x270(0), field_0x274(0),
      field_0x278(0.0f), field_0x27c(0.0f), field_0x280(0.0f), field_0x284(0.0f), field_0x288(0.0f),
      field_0x28c(-1), field_0x290(0.0f), field_0x2f0(0), field_0x310(NULL), field_0x314(NULL) {
    field_0x350 = NULL;
    field_0x354 = NULL;
    field_0x358 = NULL;
    field_0x398 = NULL;
    for (s32 i = 0; i < 7; i++) {
        if (i < 2) {
            field_0x35c[i] = NULL;
            field_0x3b4[i] = NULL;
        }
        if (i < 3) {
            field_0x378[i] = NULL;
        }
        if (i < 5) {
            field_0x384[i] = NULL;
            field_0x3ec[i] = NULL;
            field_0x400[i] = NULL;
            field_0x294[i] = 0;
        }
        if (i < 6) {
            field_0x3bc[i] = NULL;
            field_0x3d4[i] = NULL;
            field_0x39c[i] = NULL;
            field_0x2f8[i] = 0;
        }
    }
    gAudioMgr_8053AF80->fn_80458F9C(60);
}

cls_804AE088::~cls_804AE088() {
    if (field_0x314 != NULL) {
        delete field_0x314;
    }
    field_0x314 = NULL;
    fn_8036A65C();
}

// Table lbl_804AE070 is filled by this file's static initializer; its definition is not written yet.
extern void (cls_804AE088::*lbl_804AE070[2])();

void cls_804AE088::update() {
    if (field_0x2f0 != 0) {
        field_0x2f0--;
        if (field_0x2f0 == 0xE1) {
            startSound_801F0E34(0x30082, 0);
        } else if (field_0x2f0 == 0xB4) {
            startSound_801F0E34(0x30082, 0);
            startSound_801F0E34(0x30083, 0);
        }
    }
    (this->*lbl_804AE070[field_0x23c - 4])();
}

const s32 lbl_80474C48[8] = {0x199, 0x1E7, 0x17F, 0x201, 0x1CD, 0x1B3, 0x21B, 0x21B};

void cls_804AE088::draw() {
    JGeometry::TVec3f savedPos = mPos;
    if (field_0x23c == 5 && field_0x240 >= 0x24 && field_0x240 != 0x26) {
        fn_803683AC();
        return;
    }
    cls_8049D140::draw();
    fn_8036A894();
    if (field_0x236 != 0) {
        fn_8036AC58(lbl_80474C48[field_0x23c]);
    }
    fn_80368660();
    mPos = savedPos;
}

void cls_804AE088::vfunc_0x54() {
    s32 v = fn_8011921C()->field_0x8;
    s32 arg;
    if (field_0x270 == 1) {
        return;
    }
    if (field_0x23c == 4) {
        if (v == 0) {
            arg = 2;
        } else if (v == 0x96) {
            arg = 3;
        } else if (v == 0x1FE) {
            arg = 4;
        } else if (v == 0x32A) {
            arg = 5;
        } else if (v == 0x3F2) {
            arg = 0xE;
        } else if (v == 0x51E) {
            arg = 0xF;
        } else if (v == 0x5B4) {
            arg = 0x10;
        } else if (v == 0x6E0) {
            arg = 0x11;
        } else {
            return;
        }
    } else {
        if (v == 0) {
            arg = 0x15;
        } else if (v == 0x96) {
            arg = 0x16;
        } else if (v == 0x1FE) {
            arg = 0x17;
        } else if (v == 0x32A) {
            arg = 0x18;
        } else if (v == 0x3F2) {
            arg = 0x1A;
        } else if (v == 0x51E) {
            arg = 0x1B;
        } else if (v == 0x5B4) {
            arg = 0x1C;
        } else if (v == 0x6E0) {
            arg = 0x26;
        } else {
            return;
        }
    }
    vfunc_0x60(arg);
}

s32 cls_804AE088::vfunc_0x5C() {
    s32 s = field_0x240;
    if (s == 0x11 || s == 0x26)
        return 1;
    return 0;
}
