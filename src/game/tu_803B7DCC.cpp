#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/Map.h"
#include "game/tu_80379F84.h"
#include "game/tu_802EF76C.h"
#include "game/Message.h"
#include "game/act/tu_803DA6B4.h"
#include "game/cls_804B20F8.h"
#include "game/ActorManager.h"
#include "game/EmitterDelete.h"
#include "JSystem/J2D/J2DPicture.h"
#include "JSystem/J2D/J2DGrafContext.h"
#include "JSystem/JUtility/JUTVideo.h"

// FIXME(shotgun): 95.51 %, missing/extra (the unrolled field_0x24 clear starts with a loop-entry test)
cls_804AFAB0::cls_804AFAB0() {
    field_0x28c = -1;
    field_0x290 = -1;
    lbl_80529DEC.mHeap->getTotalFreeSize();
    lbl_8053ADA8 = this;
    gGameRoot_8053AAF8->field_0xa8->getResource('????', "rock_24_20_4i_e.bfn");
    for (s32 i = 0; i < 50; i++) {
        field_0x24[i] = NULL;
    }
    const ResTIMG* black = (const ResTIMG*)fn_804030C4('TIMG', "kk_black.bti");
    field_0x4 = new J2DPicture(black);
    field_0x4->setBlackWhite(JUtility::TColor(0x000000FF), JUtility::TColor(0x00000000));
    field_0xec.x = 0.0f;
    field_0xec.y = 0.0f;
    field_0xec.z = 0.0f;
    field_0x1c = new cls_804B20F8(0, NULL, NULL, 8);
    field_0x20 = new ActorSoundObj_804578F4(&field_0xec, NULL, 8);
    for (s32 i = 0; i < 10; i++) {
        field_0xf8[i].x = 0.0f;
        field_0xf8[i].y = 0.0f;
        field_0xf8[i].z = 0.0f;
        field_0x170[i] = -1;
    }
    if (lbl_80529DEC.field_0x118 == 9 && lbl_80529DEC.field_0x11c == 3) {
        lbl_80529DEC.field_0x118 = 7;
        lbl_80529DEC.field_0x11c = 3;
    }
}

cls_804AFAB0::~cls_804AFAB0() {
    for (s32 i = 0; i < 50; i++) {
        deleteEmitter(&field_0x24[i]);
    }
    lbl_80529DEC.mHeap->getTotalFreeSize();
    delete field_0x4;
    field_0x4 = NULL;
    delete field_0x1c;
    field_0x1c = NULL;
    delete field_0x20;
    field_0x20 = NULL;
    lbl_8053ADA8 = NULL;
}

void cls_804AFAB0::fn_803B8188() {
    field_0x296 = 0;
    field_0x1a0 = 0;
    field_0x1a4 = 0;
    field_0x1a8 = 0;
    field_0x1ac = 0;
    field_0x1b8 = 0;
    field_0x1bc = 0;
    field_0x284 = 0;
    field_0x288 = 0;
    field_0x198 = 0;
    field_0x19c = 0;
    field_0x1dc = 0;
    field_0x1c0 = 0;
    field_0x1c4 = 0;
    field_0x1c8 = 0;
    field_0x1cc = 0;
    field_0x1d0 = -1;
    field_0x1d4 = -1;
    field_0x1d8 = -1;
    field_0x297 = 0;
    field_0x298 = 0;
    field_0x29b = 0;
    field_0x29c = 0;
    field_0x29d = 0;
    field_0x29e = 0;
    field_0x29f = 0;
    if (lbl_80529DEC.field_0x118 < 8) {
        field_0x284 = lbl_80529DEC.field_0x118;
    } else {
        field_0x284 = 0;
    }
    if (lbl_80529DEC.field_0x11c < 4) {
        field_0x288 = lbl_80529DEC.field_0x11c;
    } else {
        field_0x288 = 0;
    }
    field_0x294 = 5;
    field_0x295 = 1;
    field_0x280 = 0x80;
    if (lbl_80529DEC.field_0x118 == 7 && lbl_80529DEC.field_0x11c == 3) {
        gAudioMgr_8053AF80->fn_80458F9C(0x3C);
    } else {
        gAudioMgr_8053AF80->fn_80458FF0(0x100002F, true, 0);
    }
    gGameRoot2_8053AB10->field_0x20->field_0x235c[0] = 0;
    gGameRoot2_8053AB10->field_0x20->field_0x235c[1] = 0;
    gGameRoot2_8053AB10->field_0x20->field_0x235c[2] = 0;
    gGameRoot2_8053AB10->field_0x20->field_0x235c[3] = 0;
    fn_80128C58(255);
    u32 color = 0xFFFFFF00;
    fn_8013A884(&color);
}

// FIXME(shotgun): 99.85 %, different (one branch of the inner switch)
void cls_804AFAB0::fn_803B7F6C() {
    switch (field_0x294) {
    case 0:
        field_0x280--;
        if (field_0x280 == 0) {
            field_0x294 = 1;
        }
        break;
    case 1:
        field_0x296 += 2;
        if (field_0x296 >= 254) {
            field_0x296 = 255;
            field_0x294 = 2;
            field_0x295 = 1;
            if (lbl_80529DEC.field_0x118 < 8) {
                field_0x284 = lbl_80529DEC.field_0x118;
            } else {
                field_0x284 = 0;
            }
            if (lbl_80529DEC.field_0x11c < 4) {
                field_0x288 = lbl_80529DEC.field_0x11c;
            } else {
                field_0x288 = 0;
            }
            gGameRoot2_8053AB10->field_0x20->field_0x235c[0] = 0;
            gGameRoot2_8053AB10->field_0x20->field_0x235c[1] = 0;
            gGameRoot2_8053AB10->field_0x20->field_0x235c[2] = 0;
            gGameRoot2_8053AB10->field_0x20->field_0x235c[3] = 0;
            fn_80128C58(255);
            u32 color = 0xFFFFFF00;
            fn_8013A884(&color);
        }
        break;
    case 2:
        switch (field_0x295) {
        case 0:
            break;
        case 1:
            field_0x280--;
            if (field_0x280 > 0) {
                field_0x280 = lbl_8050DB0C.field_0x4c[(0x80 - 0x4C) / 2];
                field_0x295 = 2;
                field_0x29b = 1;
            }
            break;
        }
        break;
    case 3:
        field_0x280--;
        if (field_0x280 <= 0) {
            field_0x280 = 60;
        }
        break;
    case 4:
        field_0x280--;
        if (field_0x280 <= 0) {
            field_0x280 = 60;
        }
        break;
    case 6:
        if (field_0x296 >= 245) {
            field_0x296 = 255;
            field_0x294 = 5;
        } else {
            field_0x296 += 10;
        }
        break;
    case 5:
        if (field_0x296 <= 10) {
            field_0x296 = 0;
            lbl_8053AD68->field_0x1d = 1;
        } else {
            field_0x296 -= 10;
        }
        break;
    }
}

// FIXME(shotgun): 99.77 %, reloc-name
void cls_804AFAB0::fn_803B7E64() {
    gGameRoot2_8053AB10->fn_8013B490();
    if (field_0x294 == 3 || field_0x294 == 4) {
        lbl_8053ADC8->field_0x20 = 1;
        lbl_8053ADC8->field_0x21 = 2;
    }
    field_0x4->setAlpha(field_0x296);
    field_0x4->draw(0.0f, 0.0f, 800.0f, 640.0f, false, false, false);
    JGeometry::TBox2f box(0.0f, 0.0f, (s32)JUTVideo::getManager()->getFbWidth(),
                          (s32)JUTVideo::getManager()->getEfbHeight());
    gGameRoot2_8053AB10->fn_8013B414(&box, true);
}

// FIXME(shotgun): 99.74 %, reloc-name
void cls_804AFAB0::fn_803B7DCC() {
    if (!(lbl_80529DEC.field_0x118 == 8 && lbl_80529DEC.field_0x11c == 3)) {
        gGameRoot2_8053AB10->fn_8013B490();
        field_0x4->setAlpha(field_0x296);
        field_0x4->draw(0.0f, 0.0f, 800.0f, 640.0f, false, false, false);
    }
}
