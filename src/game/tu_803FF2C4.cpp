#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "JSystem/JKernel/JKRArchive.h"
#include "JSystem/J2D/J2DGrafContext.h"
#include "JSystem/J2D/J2DPicture.h"
#include "JSystem/J2D/J2DPrint.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/JFramework/JFWDisplay.h"
#include "JSystem/JUtility/JUTGamePad.h"
#include "JSystem/JUtility/JUTVideo.h"
#include "game/GameState.h"
#include "game/Player.h"
#include "game/cls_804B1A38.h"
#include <dolphin/os.h>

// game/tu_80134C04.cpp: the C3ButtonReset callback.
void fn_80139994(int port, void* arg);
// game/tu_803006E8.cpp: starts the GBA client threads.
void fn_803007D0();

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
    field_0x4 = NULL;
    field_0x8 = NULL;
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

// FIXME(shotgun): 97.4 %, different + reloc-name
void cls_804B0F50::fn_803FFAA8() {
    if (field_0x79 == 0) {
        cls_804B0FC0* cmd = field_0x44;
        if (cmd->field_0x4 == 1) {
            lbl_80529DEC.field_0x58 = cmd->field_0x14;
            delete cmd;
            fn_803007D0();
            field_0x4 = new cls_804AA8C8();
            lbl_80529DEC.field_0x5c = field_0x4;
            field_0x8 = new cls_804AA8B8();
            lbl_80529DEC.field_0x60 = field_0x8;
            field_0x4->fn_8030AB38();
            field_0x8->fn_80308E90();
            field_0x34 = fn_80401E10("/GC4Sword_usa/logo.arc", 0, lbl_80529DEC.mHeap);
            field_0x79 = 1;
        }
        if (lbl_8053ACA8->fn_802B1DCC()) {
            lbl_8053ACA8->fn_802B1BE0(0, 0, false);
        }
        return;
    }

    if (field_0x79 == 1) {
        cls_804B0FC0* cmd = field_0x34;
        if (cmd->field_0x4 == 1) {
            lbl_80529DEC.field_0x18 = cmd->field_0x14;
            delete cmd;
            field_0x18 = new J2DScreen();
            field_0x18->setPriority("ZelPac_NIN_logo.blo", 0, lbl_80529DEC.field_0x18);
            ((J2DPicture*)field_0x18->search('NIN_Logo'))->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0xDC, 0, 0, 0xFF));
            const ResTIMG* tex = (const ResTIMG*)fn_80403244('TIMG', "progressive_choice.bti");
            field_0x1c = new J2DPicture(tex);
            tex = (const ResTIMG*)fn_80403244('TIMG', "progressive_inter.bti");
            field_0x20 = new J2DPicture(tex);
            tex = (const ResTIMG*)fn_80403244('TIMG', "progressive_pro.bti");
            field_0x24 = new J2DPicture(tex);
            tex = (const ResTIMG*)fn_80403244('TIMG', "progressive_yes.bti");
            field_0x28 = new J2DPicture(tex);
            tex = (const ResTIMG*)fn_80403244('TIMG', "progressive_no.bti");
            field_0x2c = new J2DPicture(tex);
            tex = (const ResTIMG*)fn_80403244('TIMG', "progressive_no.bti");
            field_0x2c = new J2DPicture(tex);
            tex = (const ResTIMG*)fn_80403244('TIMG', "kk_titleLogo_01.bti");
            field_0x30 = new J2DPicture(tex);
            if ((lbl_80529DEC.mSaveRegion.f.field_0x4[0] == 0xA7 || lbl_80529DEC.mSaveRegion.f.field_0x4[1] == 0xA5) && field_0x7a == 0) {
                field_0x6c = 4;
                lbl_80529DEC.field_0x238 = 0xFF;
                lbl_80529DEC.mSaveRegion.f.field_0x10 = 1;
                lbl_80529DEC.field_0x249[0] = 1;
            } else {
                field_0x6c = 0;
                if (VIGetDTVStatus() != 0) {
                    if (OSGetResetCode() == 0 || field_0x7a != 0) {
                        lbl_80529DEC.field_0x238 = 0;
                    } else if (lbl_80529DEC.field_0x238 != 0xFF) {
                        lbl_80529DEC.field_0x238 = 0;
                    }
                } else {
                    lbl_80529DEC.field_0x238 = 0xFF;
                    lbl_80529DEC.mSaveRegion.f.field_0x10 = 1;
                    OSSetProgressiveMode(0);
                }
            }
            field_0x79 = 2;
        } else {
            if (lbl_8053ACA8->fn_802B1DCC()) {
                lbl_8053ACA8->fn_802B1BE0(0, 0, false);
            }
            return;
        }
    }

    if (lbl_8053ACA8->fn_802B1DCC()) {
        lbl_8053ACA8->fn_802B1BE0(0, 0, false);
    }
    field_0x14->fn_80442810();
    if (field_0x14->field_0x4 != 0) {
        field_0x4->fn_8030A43C();
        field_0x8->fn_80304FB0();
    }
    if (field_0x14->field_0x4 != 0 && lbl_80529DEC.field_0xc->fn_804580DC(0) && lbl_80529DEC.field_0xc->fn_804580DC(1) && field_0x79 == 2) {
        OSReport("archive load\n");
        field_0x38 = fn_80401E10("/GC4Sword_usa/data.arc", 0, lbl_80529DEC.mHeap);
        field_0x3c = fn_80401E10("/GC4Sword_usa/msgus.arc", 0, lbl_80529DEC.mHeap);
        field_0x40 = fn_80401E10("/GC4Sword_usa/main2D.arc", 0, lbl_80529DEC.mHeap);
        field_0x48 = fn_80401BA4("/GC4Sword_usa/title.arc", NULL, 0, lbl_80529DEC.mHeap);
        field_0x54 = fn_80401BA4("/GC4Sword_usa/result.arc", NULL, 0, lbl_80529DEC.mHeap);
        field_0x58 = fn_80401BA4("/GC4Sword_usa/result1.arc", NULL, 0, lbl_80529DEC.mHeap);
        field_0x5c = fn_80401BA4("/GC4Sword_usa/result2.arc", NULL, 0, lbl_80529DEC.mHeap);
        field_0x79 = 3;
    }

    if (field_0x14->field_0x4 == 0) {
        return;
    }

    if (lbl_80529DEC.field_0x238 == 1) {
        if (SIProbe(0) != SI_GBA) {
            field_0x74++;
            if (field_0x74 >= 48) {
                lbl_80529DEC.field_0x238 = 0xFF;
                lbl_80529DEC.mSaveRegion.f.field_0x10 = 1;
                OSSetProgressiveMode(0);
            }
        } else {
            field_0x74 = 0;
        }
    } else if (lbl_80529DEC.field_0x238 == 0) {
        if (SIProbe(0) == SI_GBA) {
            field_0x74 = 0;
            lbl_80529DEC.field_0x238 = 1;
        } else if (field_0x74 > 0) {
            field_0x74--;
            if (field_0x74 <= 0) {
                if (OSGetProgressiveMode() == OS_PROGRESSIVE_MODE_ON ||
                    lbl_8053AD30->fn_80309D24(getPlayerInputSlot_8030AAF4(0), 0x200) != 0) {
                    lbl_80529DEC.field_0x238 = 2;
                    field_0x72 = 0;
                }
            }
        } else {
            lbl_80529DEC.field_0x238 = 0xFF;
            lbl_80529DEC.mSaveRegion.f.field_0x10 = 1;
            OSSetProgressiveMode(0);
            field_0x72 = 0;
        }
    }

    switch (field_0x6c) {
    case 0:
        field_0x64 += 8.5f;
        if (field_0x64 >= 255.0f) {
            field_0x64 = 255.0f;
            field_0x6c = 1;
        }
        break;
    case 1:
        field_0x6e++;
        if (field_0x6e >= 60) {
            if (lbl_80529DEC.field_0x238 == 0xFF || lbl_80529DEC.field_0x238 == 1 || lbl_80529DEC.field_0x238 == 4 || lbl_80529DEC.field_0x238 == 11) {
                field_0x6e = 0;
                field_0x6c = 2;
            } else {
                field_0x6e = 60;
            }
        }
        break;
    case 2:
        field_0x64 -= 8.5f;
        if (field_0x64 <= 0.0f) {
            field_0x64 = 0.0f;
            field_0x6c = 3;
        }
        break;
    case 3:
        field_0x64 = 0.0f;
        if (lbl_80529DEC.field_0x238 == 0xFF || lbl_80529DEC.field_0x238 == 1) {
            field_0x6c = 4;
        }
        break;
    }

    if (field_0x79 == 3 && field_0x38->field_0x4 == 1 && field_0x3c->field_0x4 == 1 && field_0x40->field_0x4 == 1 &&
        field_0x48->field_0x4 == 1 && field_0x54->field_0x4 == 1 && field_0x58->field_0x4 == 1 && field_0x5c->field_0x4 == 1) {
        lbl_80529DEC.field_0x14 = field_0x38->field_0x14;
        lbl_80529DEC.field_0x1c = field_0x3c->field_0x14;
        lbl_80529DEC.field_0x20 = field_0x40->field_0x14;
        lbl_80529DEC.field_0x24 = field_0x48->mArchive;
        lbl_80529DEC.field_0x38 = field_0x54->mArchive;
        lbl_80529DEC.field_0x40 = field_0x58->mArchive;
        lbl_80529DEC.field_0x44 = field_0x5c->mArchive;
        delete field_0x38;
        delete field_0x40;
        delete field_0x3c;
        delete field_0x48;
        delete field_0x54;
        delete field_0x58;
        delete field_0x5c;
        field_0x79 = 4;
        mState = 1;
    }

    if (mState == 1) {
        lbl_8051C1A0.fn_803B9B3C();
        mState = 2;
    } else if (mState == 2) {
        if (lbl_8051C1A0.fn_803B9A24() != 0) {
            mState = 3;
        }
    }

    if (field_0x6c >= 4 && mState == 3) {
        mState = 4;
    }

    switch (lbl_80529DEC.field_0x238) {
    case 0:
    case 1:
        break;
    case 2:
        field_0x68 += 8.5f;
        if (field_0x68 >= 255.0f) {
            field_0x68 = 255.0f;
            lbl_80529DEC.field_0x238 = 3;
        }
        break;
    case 3:
        field_0x72++;
        if (lbl_8053AD30->fn_80309D6C(getPlayerInputSlot_8030AAF4(0), 0x100) != 0 || field_0x72 >= 600) {
            lbl_80529DEC.field_0x238 = 4;
        } else if (lbl_8053AD30->fn_80309D6C(getPlayerInputSlot_8030AAF4(0), 2) != 0) {
            field_0x76 = 1;
            field_0x70 = 30;
        } else if (lbl_8053AD30->fn_80309D6C(getPlayerInputSlot_8030AAF4(0), 1) != 0) {
            field_0x76 = 0;
            field_0x70 = 30;
        }
        field_0x70++;
        if (field_0x70 > 60) {
            field_0x70 = 0;
        }
        break;
    case 4:
        if (field_0x6c == 2) {
            field_0x68 -= 8.5f;
            if (field_0x68 <= 0.0f) {
                field_0x68 = 0.0f;
                if (field_0x76 == 0) {
                    field_0x70 = 0;
                    if (OSGetProgressiveMode() == OS_PROGRESSIVE_MODE_ON) {
                        lbl_80529DEC.field_0x238 = 6;
                        OSSetProgressiveMode(OS_PROGRESSIVE_MODE_ON);
                    } else {
                        lbl_80529DEC.field_0x238 = 5;
                        OSSetProgressiveMode(OS_PROGRESSIVE_MODE_ON);
                    }
                } else if (OSGetProgressiveMode() == OS_PROGRESSIVE_MODE_ON) {
                    lbl_80529DEC.field_0x238 = 9;
                    field_0x6e = 0;
                    field_0x6c = 0;
                    OSSetProgressiveMode(OS_PROGRESSIVE_MODE_OFF);
                } else {
                    lbl_80529DEC.field_0x238 = 9;
                    field_0x6e = 0;
                    field_0x6c = 0;
                }
            }
        }
        break;
    case 5:
        lbl_80529DEC.field_0x238 = 7;
        break;
    case 6:
        lbl_80529DEC.field_0x238 = 8;
        break;
    case 7:
    case 8:
        field_0x70++;
        if (lbl_80529DEC.field_0x251 > 90 && field_0x70 >= 90) {
            lbl_80529DEC.field_0x238 = 9;
            field_0x6e = 0;
            field_0x6c = 0;
            lbl_80529DEC.field_0x251 = 0;
        }
        break;
    case 9:
        field_0x68 += 8.5f;
        if (field_0x68 >= 255.0f) {
            field_0x68 = 255.0f;
            lbl_80529DEC.field_0x238 = 10;
            field_0x70 = 0;
        }
        break;
    case 10:
        field_0x70++;
        if (field_0x70 >= 120) {
            field_0x68 = 120.0f;
            lbl_80529DEC.field_0x238 = 11;
        }
        break;
    case 11:
        field_0x68 -= 8.5f;
        if (field_0x68 <= 0.0f) {
            field_0x68 = 0.0f;
            lbl_80529DEC.field_0x238 = 0xFF;
            lbl_80529DEC.mSaveRegion.f.field_0x10 = 1;
        }
        break;
    }
}

// FIXME(shotgun): 98.8 %, missing/extra + reloc-name
void cls_804B0F50::fn_803FF420() {
    if (field_0x79 <= 1) {
        return;
    }
    if ((lbl_80529DEC.mSaveRegion.f.field_0x4[0] != 0xA7 && lbl_80529DEC.mSaveRegion.f.field_0x4[1] != 0xA5) || field_0x7a != 0) {
        J2DOrthoGraph ortho(0.0f, 0.0f, JUTVideo::getManager()->getFbWidth(), JUTVideo::getManager()->getEfbHeight(), -1.0f, 1.0f);
        JFWDisplay::getManager()->setClearColor(JUtility::TColor(0, 0, 0, 0));
        ortho.setPort();
        field_0x18->search('NIN_Logo')->setAlpha(field_0x64);
        field_0x18->draw(0.0f, 0.0f, &ortho);
        switch (lbl_80529DEC.field_0x238) {
        case 2:
        case 3:
        case 4: {
            field_0x1c->setAlpha(field_0x68);
            field_0x1c->draw(113.0f, 281.0f, false, false, false);
            f32 t;
            if (field_0x70 < 30) {
                t = field_0x70 / 30.0f;
            } else {
                t = (60.0f - field_0x70) / 30.0f;
            }
            u8 r = 255.0f * t;
            u8 g = 200.0f * t;
            if (field_0x76 == 0) {
                field_0x28->setBlackWhite(JUtility::TColor(r, g, 0, 0), JUtility::TColor(0xFF, 0xC8, 0, 0xFF));
                field_0x2c->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0xA0, 0xA0, 0xA0, 0xFF));
            } else {
                field_0x28->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0xA0, 0xA0, 0xA0, 0xFF));
                field_0x2c->setBlackWhite(JUtility::TColor(r, g, 0, 0), JUtility::TColor(0xFF, 0xC8, 0, 0xFF));
            }
            field_0x28->setAlpha(field_0x68);
            field_0x28->draw(211.0f, 372.0f, false, false, false);
            field_0x2c->setAlpha(field_0x68);
            field_0x2c->draw(350.0f, 372.0f, false, false, false);
            break;
        }
        case 5:
        case 6:
            JUTVideo::getManager()->setRenderMode(&lbl_8048C1B8);
            lbl_80529DEC.field_0x251 = 1;
            break;
        case 9:
            if (OSGetProgressiveMode() == OS_PROGRESSIVE_MODE_OFF) {
                JUTVideo::getManager()->setRenderMode(&lbl_8048C17C);
            }
        case 10:
        case 11:
            if (field_0x76 == 0) {
                field_0x24->setAlpha(field_0x68);
                field_0x24->draw(153.0f, 309.0f, false, false, false);
            } else {
                field_0x20->setAlpha(field_0x68);
                field_0x20->draw(153.0f, 309.0f, false, false, false);
            }
            break;
        }
    } else {
        J2DOrthoGraph ortho(0.0f, 0.0f, JUTVideo::getManager()->getFbWidth(), JUTVideo::getManager()->getEfbHeight(), -1.0f, 1.0f);
        JFWDisplay::getManager()->setClearColor(JUtility::TColor(0, 0, 0, 0));
        ortho.setPort();
        field_0x30->draw(105.0f, 78.0f, 400.0f, 240.0f, false, false, false);
        lbl_80529DEC.mSaveRegion.f.field_0xf = 1;
    }
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
