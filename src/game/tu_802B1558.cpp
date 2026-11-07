#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "JSystem/JUtility/JUTGamePad.h"
#include "JSystem/JUtility/JUTVideo.h"
#include "JSystem/JUtility/JUTXfb.h"

void fn_80139994(int port, void* arg);
extern u8 lbl_8053ACD4[4];

cls_804A8578::cls_804A8578() {
    lbl_8053ACA8 = this;
    field_0x4 = 0;
    field_0x5 = 0;
    field_0x6 = 0;
    field_0x7 = 0xFF;
}

cls_804A8578::~cls_804A8578() {}

void cls_804A8578::fn_802B1EB0() {}

// Copies the save region to 0x80700000, the area main registers with
// OSSetSaveRegion (see GameState.h), so it survives the reset.
// FIXME: 87.54 %. Same unrolled loop (mtctr 2, eight byte copies twice per
// pass); the original re-adds the index to the GameState base for every
// source pass (add r4, r5, r12), ours steps a source pointer by 0x10, and the
// registers differ.
void cls_804A8578::fn_802B1DE8() {
    lbl_80529DEC.mSaveRegion.f.field_0x9 = 0;
    lbl_80529DEC.mSaveRegion.f.field_0xa = 0;
    field_0x4 = 0;
    field_0x5 = 0;
    for (u32 i = 0; i < sizeof(lbl_80529DEC.mSaveRegion); i++) {
        ((u8*)0x80700000)[i] = lbl_80529DEC.mSaveRegion.bytes[i];
    }
}

bool cls_804A8578::fn_802B1DCC() {
    return lbl_80529DEC.mSaveRegion.f.field_0x9 != 0;
}

bool cls_804A8578::fn_802B1DB0() {
    return lbl_80529DEC.mSaveRegion.f.field_0xa != 0;
}

void cls_804A8578::fn_802B1D98() {
    lbl_80529DEC.mSaveRegion.f.field_0xb = 0;
    field_0x6 = 0;
}

bool cls_804A8578::fn_802B1D7C() {
    return lbl_80529DEC.mSaveRegion.f.field_0xb != 0;
}

// Reads the save-region flags in lbl_80529DEC (SaveRegion at 0x216).
s32 cls_804A8578::fn_802B1D60() {
    u32 index = lbl_80529DEC.mSaveRegion.f.field_0xc;
    if (index > 3) {
        index = -1;
    }
    return index;
}

// Stops the pads, the GX thread and the video before a reset.
static inline void shutdownForReset() {
    JUTXfb::getManager()->clearIndex();
    JUTGamePad::clearForReset();
    VIWaitForRetrace();
    VIWaitForRetrace();
    OSThread* gxThread = GXGetCurrentGXThread();
    BOOL enabled = OSDisableInterrupts();
    if (gxThread != OSGetCurrentThread()) {
        OSCancelThread(gxThread);
        GXSetCurrentGXThread();
    }
    GXFlush();
    GXAbortFrame();
    GXDrawDone();
    OSRestoreInterrupts(enabled);
    JUTVideo::destroyManager();
    GXSetDrawDoneCallback(NULL);
    VISetBlack(TRUE);
    VIFlush();
    VIWaitForRetrace();
    while (lbl_8051C1A0.field_0x1fcc != 0) {
        VIWaitForRetrace();
    }
    LCDisable();
}

// The save region goes to the area OSSetSaveRegion registered, so it survives the reset.
static inline void copySaveRegion() {
    for (u32 i = 0; i < sizeof(lbl_80529DEC.mSaveRegion); i++) {
        ((u8*)0x80700000)[i] = lbl_80529DEC.mSaveRegion.bytes[i];
    }
}

// FIXME(shotgun): 92.8 %, missing/extra
// 0x802B1BE0
void cls_804A8578::fn_802B1BE0(s32 reset, u32 code, bool menu) {
    __PADDisableRecalibration(FALSE);
    shutdownForReset();
    lbl_80529DEC.mSaveRegion.f.field_0x4[1] = 0xA5;
    copySaveRegion();
    OSResetSystem(reset, code, menu);
    while (true) {
        VIWaitForRetrace();
    }
}

// FIXME(shotgun): 89.8 %, missing/extra
// 0x802B1610
void cls_804A8578::fn_802B1610(s32 port, void* arg) {
    if (lbl_80529DEC.mSaveRegion.f.field_0x9 != 0) {
        return;
    }
    __PADDisableRecalibration(FALSE);
    if (port == -1) {
        JUTGamePad::clearForReset();
        JUTGamePad::CRumble::setEnabled(0xF0000000);
    } else {
        if (lbl_80529DEC.mSaveRegion.f.field_0xb != 0) {
            JUTGamePad::C3ButtonReset::sResetOccurred = false;
            JUTGamePad::C3ButtonReset::sCallback = fn_80139994;
            JUTGamePad::C3ButtonReset::sCallbackArg = NULL;
            return;
        }
        lbl_80529DEC.mSaveRegion.f.field_0xb = 1;
        lbl_8053ACA8->field_0x6 = 1;
        if ((u8)port <= 3) {
            lbl_80529DEC.mSaveRegion.f.field_0xc = port;
            lbl_8053ACA8->field_0x7 = port;
        } else {
            lbl_80529DEC.mSaveRegion.f.field_0xc = 0xFF;
            lbl_8053ACA8->field_0x7 = 0xFF;
        }
        JUTGamePad::clearForReset();
        JUTGamePad::CRumble::setEnabled(0xF0000000);
    }
    if (!DVDCheckDisk()) {
        __PADDisableRecalibration(FALSE);
        shutdownForReset();
        lbl_80529DEC.mSaveRegion.f.field_0x4[1] = 0xA5;
        copySaveRegion();
        OSResetSystem(TRUE, 0x80000000, FALSE);
        while (true) {
            VIWaitForRetrace();
        }
    }
    if (lbl_80529DEC.field_0x238 != 0xFF) {
        lbl_80529DEC.mSaveRegion.f.field_0xd = 1;
        copySaveRegion();
        __PADDisableRecalibration(FALSE);
        shutdownForReset();
        lbl_80529DEC.mSaveRegion.f.field_0x4[1] = 0xA5;
        copySaveRegion();
        OSResetSystem(FALSE, 0, FALSE);
        while (true) {
            VIWaitForRetrace();
        }
    }
    if (lbl_8053AAF0->mState < 4) {
        lbl_80529DEC.mSaveRegion.f.field_0xe = 1;
    }
    lbl_80529DEC.mSaveRegion.f.field_0x9 = 1;
    field_0x4 = 1;
    if (lbl_8053AAF0->field_0x79 != 0) {
        s32 connected = 0;
        s32 idle = 0;
        for (s32 i = 0; i < 4; i++) {
            if (lbl_8053AD30->fn_8030A308(getPlayerInputSlot_8030AAF4(i)) == 1) {
                connected++;
                if (lbl_8053ACD4[i] == 0) {
                    idle++;
                }
            }
        }
        if (connected == idle) {
            lbl_80529DEC.mSaveRegion.f.field_0xa = 1;
            field_0x5 = 1;
        }
    }
    copySaveRegion();
}

// FIXME(shotgun): 99.5 %, regalloc-only
// 0x802B1558
void cls_804A8578::fn_802B1558() {
    shutdownForReset();
}
