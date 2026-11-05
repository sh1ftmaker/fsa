#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "JSystem/JFramework/JFWDisplay.h"
#include "JSystem/JFramework/JFWSystem.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JUtility/JUTGamePad.h"
#include "JSystem/JUtility/JUTProcBar.h"
#include <dolphin/dvd.h>
#include <dolphin/os.h>

// The game's entry point (symbol "main", 0x801344C0): sets up the framework,
// the game pads, the heaps, the loader and the memory card, then runs the
// frame loop forever.
//
// FIXME: 94%. (A nested 4x8 copy loop reaches 96% but only steers the unroll; rejected.) (1) The target allocates the main heap to r30 and the audio
// object to r31 (we swap them, and route the heap through r0 once); (2) the
// prologue's constants (1, 0x200000, 0x40000) use other volatiles, and the
// save-region copy loop strength-reduces to two running pointers where the
// target keeps one index (r7) added to the state's base and 0x80700000; (3) after
// fn_803FFAA8 the target materialises mState >= 4 as a value (li 4; subi;
// orc; srwi 1; subf; srwi. 31) where we emit cmplwi 4 / blt.
int main() {
    JFWSystem::setMaxStdHeap(1);
    JFWSystem::setSysHeapSize(0x200000);
    JFWSystem::setFifoBufSize(0x40000);

    // The save region survives a reset; it is checked and preset here.
    for (int i = 0; i < 0x20; i++) {
        lbl_80529DEC.mSaveRegion.bytes[i] = ((u8*)0x80700000)[i];
    }
    OSSetSaveRegion((void*)0x80700000, (void*)0x8070001F);
    if (lbl_80529DEC.mSaveRegion.f.magic0 != 0xA759 || lbl_80529DEC.mSaveRegion.f.magic1 != 0xDC96) {
        lbl_80529DEC.mSaveRegion.f.magic0 = 0xA759;
        lbl_80529DEC.mSaveRegion.f.magic1 = 0xDC96;
        lbl_80529DEC.mSaveRegion.f.field_0x4[0] = 0;
        lbl_80529DEC.mSaveRegion.f.field_0x4[1] = 0;
        lbl_80529DEC.mSaveRegion.f.field_0x4[2] = 0;
        lbl_80529DEC.mSaveRegion.f.field_0x4[3] = 0;
        lbl_80529DEC.mSaveRegion.f.field_0x4[4] = 0;
        lbl_80529DEC.mSaveRegion.f.field_0x9 = 0;
        lbl_80529DEC.mSaveRegion.f.field_0xa = 0;
        lbl_80529DEC.mSaveRegion.f.field_0xb = 0;
        lbl_80529DEC.mSaveRegion.f.field_0xc = 0xFF;
        lbl_80529DEC.mSaveRegion.f.field_0xd = 0;
        lbl_80529DEC.mSaveRegion.f.field_0xf = 0;
        lbl_80529DEC.mSaveRegion.f.field_0x10 = 0;
    }
    if (OSGetResetCode() == 0) {
        lbl_80529DEC.mSaveRegion.f.field_0xb = 0;
        lbl_80529DEC.mSaveRegion.f.field_0xc = 0xFF;
        lbl_80529DEC.mSaveRegion.f.field_0xd = 0;
        lbl_80529DEC.mSaveRegion.f.field_0xf = 0;
        lbl_80529DEC.mSaveRegion.f.field_0x10 = 0;
    }
    if (lbl_80529DEC.mSaveRegion.f.field_0x10 == 1) {
        if (OSGetProgressiveMode() == 1) {
            JFWSystem::setRenderMode(&lbl_8048C1B8);
        } else {
            JFWSystem::setRenderMode(&lbl_8048C17C);
        }
    } else {
        JFWSystem::setRenderMode(&lbl_8048C17C);
    }
    JFWSystem::init();
    JUTGamePad::CRumble::setEnabled(0xF0000000);

    JUTGamePad pad0(JUTGamePad::Port1);
    JUTGamePad pad1(JUTGamePad::Port2);
    JUTGamePad pad2(JUTGamePad::Port3);
    JUTGamePad pad3(JUTGamePad::Port4);
    pad1._A8 = 0;
    pad2._A8 = 0;
    pad3._A8 = 0;

    JFWDisplay::createManager(NULL, JKRHeap::getSystemHeap(), JUTXfb::DoubleBuffer, true);
    JFWDisplay::getManager()->clearAllXfb();
    lbl_8051C1A0.fn_803BA980();

    JKRSolidHeap* solidHeap = JKRSolidHeap::create(0x180000, JKRHeap::getRootHeap(), false);
    JKRExpHeap* mainHeap = JKRExpHeap::create(0xFFFFFFFF, JKRHeap::getRootHeap(), false);
    AudioMgr_8045A080* audio = new (mainHeap, 0) AudioMgr_8045A080();
    mainHeap->becomeCurrentHeap();
    lbl_80529DEC.field_0x4 = solidHeap;
    lbl_80529DEC.mHeap = mainHeap;
    lbl_80529DEC.field_0xc = audio;
    JUTProcBar::getManager()->setVisible(false);
    JUTProcBar::getManager()->setHeapBarVisible(false);

    JKRExpHeap* threadHeap = JKRExpHeap::create(0x800, mainHeap, false);
    fn_80401FEC(OSGetThreadPriority(OSGetCurrentThread()) - 1, threadHeap, 0);
    lbl_8053AAF0 = new (mainHeap, 0) cls_804B0F50();

    bool flag = false;
    for (;;) {
        lbl_8051C1A0.fn_803B9E2C();
        if (DVDGetDriveStatus() != -1) {
            JUTGamePad::read();
        }
        if (!lbl_8053ACA8->fn_802B1DCC()) {
            if (lbl_8053ACA8->fn_802B1D7C()) {
                s32 port = lbl_8053ACA8->fn_802B1D60();
                if (port != 0 && port != 1 && port != 2 && port != 3) {
                    port = -1;
                }
                if (!JUTGamePad::getGamePad(port)->isPushing3ButtonReset()) {
                    lbl_8053ACA8->fn_802B1D98();
                }
            }
        }
        if (lbl_80529DEC.field_0x251 != 0 && lbl_80529DEC.field_0x251 <= 0x5A) {
            lbl_80529DEC.field_0x251++;
        }
        if (lbl_8053ADE8.fn_80401518() != 0) {
            continue;
        }
        if (lbl_8053AAF0->mState >= 4) {
            JFWDisplay::getManager()->beginRender();
            if (lbl_80529DEC.field_0x251 == 0) {
                gGameRoot_8053AAF8->fn_80136828();
            }
            JFWDisplay::getManager()->endRender();
            gGameRoot_8053AAF8->fn_80137300();
            JFWDisplay::getManager()->endFrame();
            continue;
        }
        JFWDisplay::getManager()->beginRender();
        lbl_8053AAF0->fn_803FF420();
        JFWDisplay::getManager()->endRender();
        lbl_8053AAF0->fn_803FFAA8();
        if (lbl_8053AAF0->mState >= 4) {
            lbl_8053AAF0->fn_803FF2C4();
            gGameRoot_8053AAF8 = new GameRoot_8049EF20();
            gGameRoot_8053AAF8->fn_80138C9C();
            JFWDisplay::getManager()->endFrame();
            continue;
        }
        if (!flag) {
            flag = true;
        }
        JFWDisplay::getManager()->endFrame();
    }
}
