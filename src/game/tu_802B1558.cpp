#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "JSystem/JUtility/JUTGamePad.h"
#include "JSystem/JUtility/JUTVideo.h"
#include "JSystem/JUtility/JUTXfb.h"
#include <dolphin/gx.h>
#include <dolphin/gx/GXFifo.h>
#include <dolphin/os.h>
#include <dolphin/vi.h>

// The reset path: stops the GX thread, blanks the screen and waits until the
// memory card's flag at lbl_8051C1A0 clears.
// Not built yet: libs/sdk/include/dolphin/gx/GXFifo.h (found first on the
// include path) has no GXGet/SetCurrentGXThread prototypes; they are only in
// libs/sdk_gx/include. Needs a SHARED-EDIT to the SDK header before this compiles.
#ifdef NONMATCHING
void fn_802B1558() {
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
#endif

// Reads the save-region flags in lbl_80529DEC (SaveRegion at 0x216).
// FIXME: 65%. The original keeps the lbz result in r3 and returns it straight
// from blelr (lis/addi into r3); ours loads through r4/r0 and copies with mr r3, r0.
s32 cls_804A8578::fn_802B1D60() {
    u8 index = lbl_80529DEC.mSaveRegion.f.field_0xc;
    if (index <= 3) {
        return index;
    }
    return -1;
}

bool cls_804A8578::fn_802B1D7C() {
    return lbl_80529DEC.mSaveRegion.f.field_0xb != 0;
}

void cls_804A8578::fn_802B1D98() {
    lbl_80529DEC.mSaveRegion.f.field_0xb = 0;
    field_0x6 = 0;
}

bool fn_802B1DB0() {
    return lbl_80529DEC.mSaveRegion.f.field_0xa != 0;
}

bool cls_804A8578::fn_802B1DCC() {
    return lbl_80529DEC.mSaveRegion.f.field_0x9 != 0;
}

// FIXME: 82%. The original copies the 0x20 save bytes in two 16-byte passes
// (mtctr 2; eight lbz/stb pairs twice per pass, the base in r12); ours is one
// 0x20-iteration byte loop (cmpwi 0x20), and the zero constants use other
// registers (li r6 in the original, li r0 here).
void cls_804A8578::fn_802B1DE8() {
    lbl_80529DEC.mSaveRegion.f.field_0x9 = 0;
    lbl_80529DEC.mSaveRegion.f.field_0xa = 0;
    field_0x4 = 0;
    field_0x5 = 0;
    for (s32 i = 0; i < 0x20; i++) {
        ((u8*)0x80700000)[i] = lbl_80529DEC.mSaveRegion.bytes[i];
    }
}

void fn_802B1EB0() {}

cls_804A8578::~cls_804A8578() {}

cls_804A8578::cls_804A8578() {
    lbl_8053ACA8 = this;
    field_0x4 = 0;
    field_0x5 = 0;
    field_0x6 = 0;
    field_0x7 = 0xFF;
}
