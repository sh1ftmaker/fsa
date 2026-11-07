#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "JSystem/JKernel/JKRArchive.h"
#include "JSystem/J2D/J2DPrint.h"
#include "JSystem/JUtility/JUTGamePad.h"
#include "JSystem/JUtility/JUTVideo.h"
#include "game/GameState.h"
#include "game/cls_804B1A38.h"

// game/tu_80134C04.cpp: the C3ButtonReset callback.
void fn_80139994(int port, void* arg);

// FIXME(shotgun): 99.9 %, reloc-name (the archive path string)
cls_804B0F50::cls_804B0F50() {
    lbl_8053AAF0 = this;
    if (lbl_80529DEC.mSaveRegion.f.field_0xd == 1) {
        field_0x7a = 1;
        lbl_80529DEC.mSaveRegion.f.field_0xd = 0;
    } else {
        field_0x7a = 0;
    }
    if (lbl_80529DEC.mSaveRegion.f.field_0xe == 1) {
        if (lbl_80529DEC.mSaveRegion.f.field_0xf != 1) {
            field_0x7a = 1;
        }
        lbl_80529DEC.mSaveRegion.f.field_0xe = 0;
    }
    J2DPrint::setBuffer(0x400);
    if (OSGetResetCode() == 0 || field_0x7a != 0) {
        if (VIGetDTVStatus() == 0) {
            OSSetProgressiveMode(0);
            JUTVideo::sManager->setRenderMode(&lbl_8048C17C);
        } else {
            JUTVideo::sManager->setRenderMode(&lbl_8048C17C);
        }
    }
    field_0x60 = 0;
    field_0x6e = 0;
    field_0x64 = 0.0f;
    field_0x76 = 0;
    field_0x70 = 0;
    field_0x72 = 0;
    field_0x68 = 0.0f;
    field_0x74 = 0x30;
    field_0x79 = 0;
    mState = 0;
    field_0x4 = 0;
    field_0x8 = 0;
    field_0x10 = 0;
    field_0xc = new cls_804A8578();
    field_0x14 = new cls_804B1A38();
    field_0x18 = NULL;
    field_0x1c = NULL;
    field_0x20 = NULL;
    field_0x24 = NULL;
    field_0x28 = NULL;
    field_0x2c = NULL;
    field_0x30 = NULL;
    field_0xc->fn_802B1EB0();
    field_0x44 = fn_80401E10("/GC4Sword_usa/agb/client_thread.arc", 0, lbl_80529DEC.mHeap);
    lbl_8053ACA8->fn_802B1DE8();
    JUTGamePad::clearResetOccurred();
    JUTGamePad::setResetCallback(fn_80139994, NULL);
}

// FIXME(shotgun): 100 %
cls_804B0F50::~cls_804B0F50() {
    lbl_80529DEC.field_0x18->unmount();
    delete field_0xc;
    field_0xc = NULL;
    delete field_0x14;
    field_0x14 = NULL;
    fn_803FF2C4();
    lbl_80529DEC.mSaveRegion.f.field_0xf = 0;
}

// Frees the loader's archives.
// FIXME(shotgun): 100 %
void cls_804B0F50::fn_803FF2C4() {
    if (field_0x18 != NULL) {
        delete field_0x18;
        field_0x18 = NULL;
    }
    if (field_0x1c != NULL) {
        delete field_0x1c;
        field_0x1c = NULL;
    }
    if (field_0x20 != NULL) {
        delete field_0x20;
        field_0x20 = NULL;
    }
    if (field_0x24 != NULL) {
        delete field_0x24;
        field_0x24 = NULL;
    }
    if (field_0x28 != NULL) {
        delete field_0x28;
        field_0x28 = NULL;
    }
    if (field_0x2c != NULL) {
        delete field_0x2c;
        field_0x2c = NULL;
    }
    if (field_0x30 != NULL) {
        delete field_0x30;
        field_0x30 = NULL;
    }
}
