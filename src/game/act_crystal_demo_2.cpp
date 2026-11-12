// Crystal (cutscene) actor: 'DCR2', spawned by tu_801F9484.cpp.
// Declared in include/game/act/tu_801F9484.h (cls_804AE088).
// Inherits from cls_8049D140 (tu_8010DF44).

#include "game/ActorBase.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameRootSub20.h"
#include "game/RotMatrix.h"
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
// fn_8036AC58, fn_8036A65C, fn_80369DDC (jumptable_804AE13C),
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

// Table lbl_80474C68 (0x1E0 bytes) is read by the original; its definition is not written yet.
extern const s32 lbl_80474C68[120];

// FIXME: 75.8 %, the original keeps lbl_80474C38 in a register as table base (we index lbl_80474C68).
void cls_804AE088::fn_8036AA60() {
    Mtx mtx;
    PSMTXTrans(mtx, mPos.x, mPos.y, mPos.z);
    JGeometry::TVec3f axisX(1.0f, 0.0f, 0.0f);
    fn_8010F668(mtx, &axisX, 0.08726646f);
    Mtx rot;
    for (s32 i = 0; i < 3; i++) {
        for (s32 j = 0; j < 4; j++) {
            rot[i][j] = 0.0f;
        }
    }
    JGeometry::TVec3f axisZ(0.0f, 0.0f, 1.0f);
    fn_8010F668(rot, &axisZ, field_0x280);
    PSMTXConcat(mtx, rot, mtx);
    if (field_0x238 != 0) {
        field_0x254++;
    }
    switch (field_0x244) {
    case 0:
    case 2:
        if (field_0x254 >= lbl_80474C68[12 + field_0x244]) {
            field_0x254 = lbl_80474C68[9 + field_0x244];
        }
        break;
    case 1:
        if (field_0x254 >= lbl_80474C68[13]) {
            field_0x244 = 2;
        }
        break;
    }
    s32 frame = field_0x254;
    GameRootSub20_801902C0::Crystal& crystal = fn_801902C0()->mCrystals[lbl_80474C68[23 + field_0x23c]];
    if (crystal.mModel != NULL && crystal.mActive == 0) {
        crystal.mActive = 1;
        crystal.mPos.x = 0.5f;
        crystal.mPos.y = 0.5f;
        crystal.mPos.z = 0.5f;
        PSMTXCopy(mtx, crystal.mMtx);
        crystal.field_0x48 = -1.0f;
        crystal.field_0x4c = frame;
        crystal.field_0x54 = 0;
        crystal.field_0x58 = 2;
    }
}

// Not auto-inlined: draw calls fn_8036A894 out of line in the original (docs/INLINING.md, pragma rule 1).
#pragma auto_inline off
void cls_804AE088::fn_8036A894() {
    if (field_0x24c > 0) {
        SpriteParams_801F5AE0 params;
        Mtx mtx;
        PSMTXTrans(mtx, 0.0f, 0.0f, 0.0f);
        mtx[0][0] = 96.0f;
        mtx[0][1] = 0.0f;
        mtx[0][2] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = 1.0f;
        mtx[1][2] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = 96.0f;
        params.field_0x14.r = 0;
        params.field_0x14.g = 0;
        params.field_0x14.b = 0;
        params.field_0x14.a = field_0x24c;
        params.field_0x1d = -1;
        params.field_0x2d = 1;
        params.mMtx = mtx;
        JGeometry::TVec3f pos = mPos;
        pos.z = 0.0f;
        if (field_0x4 < 8) {
            gGameRoot2_8053AB10->field_0x20->fn_802F745C(&pos, field_0x310, &params);
        }
    }
}
#pragma auto_inline reset
