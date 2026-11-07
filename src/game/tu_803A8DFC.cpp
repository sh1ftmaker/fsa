#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/GameRootSub20.h"
#include "game/tu_802EF76C.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/EmitterDelete.h"
#include "game/Map.h"
#include "game/SpriteParams.h"
#include "game/act/tu_803DA6B4.h"
#include "game/tu_803B8C74.h"
#include "game/Message.h"
#include "game/cls_804B20F8.h"
#include "game/tu_80379F84.h"
#include "game/tu_803A8DFC.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/J2D/J2DPicture.h"
#include "JSystem/J2D/J2DGrafContext.h"
#include "JSystem/J2D/J2DAnmLoader.h"
#include "JSystem/J3D/J3DAnmTextureSRTKey.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "JSystem/JUtility/JUTVideo.h"
#include "dolphin/os.h"
#include <stdio.h>

// .rodata: J2DPane::search tags the unit's functions index (address order).
static const u64 lbl_80476208[13] = {
    'Tmmt01', 'Tmmt02', 'Pmmw01', 'Pmmw02', 'Pmmw03', 'Pmmw04', 'Pmmw05',
    'Pmmw06', 'Pmmw07', 'Pmmw08', 'Pmmw09', 'Pmmwk01', 'Pmmwk02',
};
static const u64 lbl_80476270[4][3] = {
    {'P1agb01', 'P1agbb01', 'P1agbb02'},
    {'P2agb01', 'P2agbb01', 'P2agbb02'},
    {'P3agb01', 'P3agbb01', 'P3agbb02'},
    {'P4agb01', 'P4agbb01', 'P4agbb02'},
};
static const u64 lbl_804762D0[4][2] = {
    {'P1ok01', 'P1ok02'},
    {'P2ok01', 'P2ok02'},
    {'P3ok01', 'P3ok02'},
    {'P4ok01', 'P4ok02'},
};
static const u64 lbl_80476310[8] = {
    'Ndatar01', 'Ndatar21', 'Ndatar12', 'Ndatar22', 'Ndatar13', 'Ndatar23', 'Ndatar14', 'Ndatar24',
};

// FIXME: 99.85. Operands + reloc-name only: .rodata string offsets (one addi) and .sdata2
// names differ (the unit's other .rodata objects, function-local tables of unwritten
// functions, come first in the original).
// The OSReport arguments read the text box's string length through getStringAllocByte()
// (u16, as zeldaret/tp's J2DTextBox: the original loads it with lhz after an mr).
ResultScreen_804AF9A0::ResultScreen_804AF9A0() {
    lbl_80529DEC.mHeap->getTotalFreeSize();
    lbl_8053ADA0 = this;
    for (int i = 0; i < 30; i++) {
        field_0x1c8[i] = NULL;
    }
    for (int i = 0; i < 4; i++) {
        field_0x188[i] = NULL;
        field_0x198[i] = NULL;
        field_0x1a8[i] = NULL;
        field_0x1b8[i] = NULL;
    }
    field_0x4[0] = new J2DScreen;
    field_0x4[0]->setPriority("result_4P.blo", 0x1100000, lbl_8053AD68->field_0x18);
    field_0x10[0] = new J2DScreen;
    field_0x10[0]->setPriority("result_score_4P.blo", 0x1100000, lbl_8053AD68->field_0x18);
    OSReport("Tmmt01===>%d\n", static_cast<J2DTextBox*>(field_0x10[0]->search('Tmmt01'))->getStringAllocByte());
    OSReport("Tmmt02===>%d\n", static_cast<J2DTextBox*>(field_0x10[0]->search('Tmmt02'))->getStringAllocByte());
    field_0x1c = new J2DScreen;
    field_0x1c->setPriority("result_back.blo", 0x1100000, lbl_8053AD68->field_0x18);
    field_0x4[1] = new J2DScreen;
    field_0x4[1]->setPriority("result_3P.blo", 0x1100000, lbl_8053AD68->field_0x18);
    field_0x10[1] = new J2DScreen;
    field_0x10[1]->setPriority("result_score_3P.blo", 0x1100000, lbl_8053AD68->field_0x18);
    OSReport("Tmmt01===>%d\n", static_cast<J2DTextBox*>(field_0x10[1]->search('Tmmt01'))->getStringAllocByte());
    OSReport("Tmmt02===>%d\n", static_cast<J2DTextBox*>(field_0x10[1]->search('Tmmt02'))->getStringAllocByte());
    field_0x4[2] = new J2DScreen;
    field_0x4[2]->setPriority("result_2P.blo", 0x1100000, lbl_8053AD68->field_0x18);
    field_0x10[2] = new J2DScreen;
    field_0x10[2]->setPriority("result_score_2P.blo", 0x1100000, lbl_8053AD68->field_0x18);
    field_0x20 = new J2DScreen;
    field_0x20->setPriority("result_title.blo", 0x1100000, lbl_8053AD68->field_0x18);
    OSInitFastCast();
    field_0x24[0] = static_cast<J2DAnmTextureSRTKey*>(J2DAnmLoaderDataBase::load(fn_80403104('SCRN', "result_4P.btk")));
    field_0x4[0]->setAnimation(field_0x24[0]);
    field_0x24[1] = static_cast<J2DAnmTextureSRTKey*>(J2DAnmLoaderDataBase::load(fn_80403104('SCRN', "result_3P.btk")));
    field_0x4[1]->setAnimation(field_0x24[1]);
    field_0x24[2] = static_cast<J2DAnmTextureSRTKey*>(J2DAnmLoaderDataBase::load(fn_80403104('SCRN', "result_2P.btk")));
    field_0x4[2]->setAnimation(field_0x24[2]);
    const ResTIMG* tex = (const ResTIMG*)fn_804030C4('TIMG', "kk_black.bti");
    field_0x170 = new J2DPicture(tex);
    field_0x170->setBlackWhite(JUtility::TColor(0x000000FF), JUtility::TColor(0x00000000));

    field_0x3f0 = field_0x4[0]->search('Pltest4')->getHeight() / 2 +
                  ((field_0x4[0]->search('Prpst04')->getBounds()->i.y +
                    (field_0x4[0]->search('Ndata04')->getBounds()->i.y + field_0x4[0]->search('Pltest4')->getBounds()->i.y)) -
                   (field_0x4[0]->search('Ndata05')->getBounds()->i.y + field_0x4[0]->search('Prpst04')->getBounds()->i.y));

    field_0x3f4 = field_0x20->search('Pttlbk05')->getBounds()->i.y;
    field_0x3f8 = field_0x20->search('Pembsb06')->getBounds()->i.y;
    field_0x3fc = field_0x20->search('Prmhb06')->getBounds()->i.y;
    field_0x400 = field_0x20->search('Pllosb06')->getBounds()->i.y;
    field_0x414 = field_0x20->search('Pwinbk05')->getBounds()->i.y;
    field_0x418 = field_0x20->search('Pdrwbk05')->getBounds()->i.y;
    field_0x408 = field_0x20->search('Plhbk02')->getBounds()->i.y;
    field_0x404 = field_0x20->search('Pdhbk02')->getBounds()->i.y;
    field_0x40c = field_0x20->search('Pfobk02')->getBounds()->i.y;
    field_0x410 = field_0x20->search('Phabk02')->getBounds()->i.y;
    field_0x70 = field_0x20->search('Pttlbk05');
    field_0x74 = field_0x20->search('Pdrwbk05');
    field_0x78 = field_0x20->search('Pwinbk05');
    field_0x7c = field_0x20->search('Pwnr01');
    field_0x80 = field_0x20->search('Pwnr02');
    field_0x8c = field_0x20->search('Pembsb06');
    field_0x90 = field_0x20->search('Prmhb06');
    field_0x94 = field_0x20->search('Pllosb06');
    field_0x98 = field_0x20->search('Plhbk02');
    field_0x9c = field_0x20->search('Pdhbk02');
    field_0xa0 = field_0x20->search('Phabk02');
    field_0xa4 = field_0x20->search('Pfobk02');
    field_0x70->show();
    field_0x74->hide();
    field_0x78->hide();
    field_0x7c->hide();
    field_0x80->hide();
    field_0x8c->hide();
    field_0x90->hide();
    field_0x94->hide();
    field_0x98->hide();
    field_0x9c->hide();
    field_0xa4->hide();
    field_0xa0->hide();
    field_0x244.x = 0.0f;
    field_0x244.y = 0.0f;
    field_0x244.z = 0.0f;
    for (int i = 0; i < 4; i++) {
        field_0x174[i] = new cls_804B20F8(i, NULL, NULL, 8);
    }
    field_0x184 = new ActorSoundObj_804578F4(&field_0x244, NULL, 8);
    for (int i = 0; i < 10; i++) {
        field_0x250[i].x = 0.0f;
        field_0x250[i].y = 0.0f;
        field_0x250[i].z = 0.0f;
        field_0x2c8[i] = -1;
    }
    if (lbl_80529DEC.field_0x118 == 9 && lbl_80529DEC.field_0x11c == 3) {
        lbl_80529DEC.field_0x118 = 7;
        lbl_80529DEC.field_0x11c = 3;
    }
}

ResultScreen_804AF9A0::~ResultScreen_804AF9A0() {
    for (int i = 0; i < 30; i++) {
        deleteEmitter(&field_0x1c8[i]);
    }
    for (int i = 0; i < 4; i++) {
        if (field_0x1a8[i] != NULL) {
            field_0x1a8[i]->becomeInvalidEmitter();
            field_0x1a8[i] = NULL;
        }
        if (field_0x188[i] != NULL) {
            field_0x188[i]->becomeInvalidEmitter();
            field_0x188[i] = NULL;
        }
        if (field_0x198[i] != NULL) {
            field_0x198[i]->becomeInvalidEmitter();
            field_0x198[i] = NULL;
        }
        if (field_0x1b8[i] != NULL) {
            field_0x1b8[i]->becomeInvalidEmitter();
            field_0x1b8[i] = NULL;
        }
    }
    lbl_80529DEC.mHeap->getTotalFreeSize();
    for (int i = 0; i < 3; i++) {
        delete field_0x4[i];
        field_0x4[i] = NULL;
        delete field_0x10[i];
        field_0x10[i] = NULL;
    }
    delete field_0x20;
    field_0x20 = NULL;
    delete field_0x1c;
    field_0x1c = NULL;
    for (int i = 0; i < 3; i++) {
        delete field_0x24[i];
        field_0x24[i] = NULL;
    }
    delete field_0x170;
    field_0x170 = NULL;
    for (int i = 0; i < 4; i++) {
        delete field_0x174[i];
        field_0x174[i] = NULL;
    }
    delete field_0x184;
    field_0x184 = NULL;
    lbl_8053ADA0 = NULL;
}

// FIXME: reloc-name only (instructions identical): the .sdata2 literals / jump table are
// not yet in the original order (most of the unit is unwritten).
void ResultScreen_804AF9A0::fn_803B6768() {
    s32 count = 0;
    for (int i = 0; i < 4; i++) {
        if (gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << i)) {
            count++;
        }
    }
    switch (count) {
    case 2:
        field_0x450 = 2;
        field_0x454 = 2;
        fn_803B38E8();
        break;
    case 3:
        field_0x450 = 1;
        field_0x454 = 3;
        fn_803B42C0();
        break;
    default:
        field_0x450 = 0;
        field_0x454 = 4;
        fn_803B5378();
        break;
    }
    f32 x = field_0x70->getBounds()->i.x;
    field_0x70->move(x, field_0x3f4);
    x = field_0x8c->getBounds()->i.x;
    field_0x8c->move(x, field_0x3f8);
    x = field_0x94->getBounds()->i.x;
    field_0x94->move(x, field_0x400);
    x = field_0x90->getBounds()->i.x;
    field_0x90->move(x, field_0x3fc);
    field_0x4c2 = 0;
    field_0x440 = 1;
    field_0x444 = 0;
    field_0x448 = 0;
    field_0x44c = 0;
    field_0x43c = 0;
    field_0x438 = 0;
    field_0x458 = 0;
    field_0x45a = 0;
    field_0x45c = -1;
    field_0x45e = -1;
    field_0x4db = 0;
    field_0x4dc = 0;
    field_0x4dd = 0xFF;
    for (int i = 0; i < 4; i++) {
        field_0x320[i] = 0;
        field_0x350[i] = 0; // field_0x350 twice (not field_0x330) in the original
        field_0x340[i] = 0;
        field_0x350[i] = 0;
        field_0x360[i] = 0;
        field_0x370[i] = 0;
        field_0x380[i] = 0;
        field_0x390[i] = 0;
        field_0x3a0[i] = 0;
        field_0x3b0[i] = 0;
        field_0x4ac[i] = 0;
        field_0x3c0[i] = 62;
        field_0x3d0[i] = 0;
        field_0x3e0[i] = 0;
        field_0x4b0[i] = 0;
        field_0x4b4[i] = 0xFF;
        field_0x4b8[i] = 0xFF;
        field_0x4bc[i] = 0xFF;
        field_0x460[i] = 0;
        if (gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << i)) {
            // The pane's centre in the 256 x 192 sprite space.
            field_0x2f0[i].x = 256.0f *
                               (field_0x158[i]->getWidth() / 2 +
                                (field_0x168->getBounds()->i.x + field_0x158[i]->getBounds()->i.x)) /
                               (s32)JUTVideo::getManager()->getFbWidth();
            field_0x2f0[i].y = 192.0f *
                               (field_0x3f0 + (field_0x168->getBounds()->i.y + field_0x158[i]->getHeight()) -
                                526.0f - 18.0f) /
                               (s32)JUTVideo::getManager()->getEfbHeight();
            field_0x2f0[i].z = 0.0f;
        }
    }
    field_0x4c3 = 0;
    field_0x4c4 = 0;
    field_0x4c5 = 0;
    field_0x4c6 = 0;
    field_0x4c8 = 0xFF;
    field_0x4c7 = 0;
    field_0x434 = 1200;
    s32 stage = lbl_80529DEC.field_0x118;
    if ((stage == 0 && lbl_80529DEC.field_0x11c == 2) || (stage == 1 && lbl_80529DEC.field_0x11c == 2) ||
        (stage == 2 && lbl_80529DEC.field_0x11c == 2) || (stage == 3 && lbl_80529DEC.field_0x11c == 2) ||
        (stage == 4 && lbl_80529DEC.field_0x11c == 2) || (stage == 5 && lbl_80529DEC.field_0x11c == 2) ||
        (stage == 6 && lbl_80529DEC.field_0x11c == 2) || (stage == 7 && lbl_80529DEC.field_0x11c == 1) ||
        (stage == 10 && lbl_80529DEC.field_0x11c == 2) || lbl_80529DEC.field_0x237 == 8) {
        field_0x4c0 = 5;
    } else {
        field_0x4c0 = 2;
    }
    field_0x4c1 = 1;
    field_0x440 = 128;
    if (lbl_80529DEC.field_0x118 == 7 && lbl_80529DEC.field_0x11c == 3) {
        gAudioMgr_8053AF80->fn_80458F9C(60);
    } else if (field_0x4c0 == 5) {
        gAudioMgr_8053AF80->fn_80458FF0(0x100002F, false, 0);
    } else {
        gAudioMgr_8053AF80->fn_80458FF0(0x1000005, true, 0);
    }
    gGameRoot2_8053AB10->field_0x20->field_0x235c[0] = 0;
    gGameRoot2_8053AB10->field_0x20->field_0x235c[1] = 0;
    gGameRoot2_8053AB10->field_0x20->field_0x235c[2] = 0;
    gGameRoot2_8053AB10->field_0x20->field_0x235c[3] = 0;
    fn_80128C58(0xFF);
    u32 color = 0xFFFFFF00;
    fn_8013A884(&color);
}

// FIXME: reloc-name only (instructions identical): the .sdata2 literals / jump table are
// not yet in the original order (most of the unit is unwritten).
// Every case loops over the players with an empty body (the original's
// code is the same: whatever each case did per player was compiled out).
void ResultScreen_804AF9A0::fn_803B35F8(s32 index) {
    switch (index) {
    case 0:
        for (s32 i = 0; i < field_0x454; i++) {
        }
        break;
    case 1:
        for (s32 i = 0; i < field_0x454; i++) {
        }
        break;
    case 2:
        for (s32 i = 0; i < field_0x454; i++) {
        }
        break;
    case 3:
        for (s32 i = 0; i < field_0x454; i++) {
        }
        break;
    case 4:
        for (s32 i = 0; i < field_0x454; i++) {
        }
        break;
    case 5:
        for (s32 i = 0; i < field_0x454; i++) {
        }
        break;
    case 6:
        for (s32 i = 0; i < field_0x454; i++) {
        }
        break;
    case 7:
        for (s32 i = 0; i < field_0x454; i++) {
        }
        break;
    case 8:
        for (s32 i = 0; i < field_0x454; i++) {
        }
        break;
    }
}

// FIXME: reloc-name only (instructions identical): the .sdata2 literals / jump table are
// not yet in the original order (most of the unit is unwritten).
// Every case loops over the players with an empty body (the original's
// code is the same: whatever each case did per player was compiled out).
void ResultScreen_804AF9A0::fn_803B3358(s32 index) {
    switch (index) {
    case 0:
        for (s32 i = 0; i < field_0x454; i++) {
        }
        break;
    case 1:
        for (s32 i = 0; i < field_0x454; i++) {
        }
        break;
    case 2:
        for (s32 i = 0; i < field_0x454; i++) {
        }
        break;
    case 3:
        for (s32 i = 0; i < field_0x454; i++) {
        }
        break;
    case 4:
        for (s32 i = 0; i < field_0x454; i++) {
        }
        break;
    case 5:
        for (s32 i = 0; i < field_0x454; i++) {
        }
        break;
    case 6:
        for (s32 i = 0; i < field_0x454; i++) {
        }
        break;
    case 7:
        for (s32 i = 0; i < field_0x454; i++) {
        }
        break;
    }
}

// FIXME: reloc-name only (instructions identical): the .sdata2 literals / jump table are
// not yet in the original order (most of the unit is unwritten).
void ResultScreen_804AF9A0::fn_803B31AC() {
    s32 stage = lbl_80529DEC.field_0x118;
    if ((stage == 0 && lbl_80529DEC.field_0x11c == 2) || (stage == 1 && lbl_80529DEC.field_0x11c == 2) ||
        (stage == 2 && lbl_80529DEC.field_0x11c == 2) || (stage == 3 && lbl_80529DEC.field_0x11c == 2) ||
        (stage == 4 && lbl_80529DEC.field_0x11c == 2) || (stage == 5 && lbl_80529DEC.field_0x11c == 2) ||
        (stage == 6 && lbl_80529DEC.field_0x11c == 2) || (stage == 7 && lbl_80529DEC.field_0x11c == 1) ||
        (stage == 8 && lbl_80529DEC.field_0x11c == 3) || (stage == 10 && lbl_80529DEC.field_0x11c == 2) ||
        lbl_80529DEC.field_0x237 == 8) {
        return;
    }
    gGameRoot2_8053AB10->fn_8013B490();
    field_0x170->setAlpha(field_0x4c2);
    field_0x170->draw(0.0f, 0.0f, 800.0f, 640.0f, false, false, false);
}

void ResultScreen_804AF9A0::fn_803B2FB0() {
    for (s32 i = 0; i < field_0x454; i++) {
        if (lbl_8053AD30->fn_80309D24(getPlayerInputSlot_8030AAF4(i), 0x40)) {
            // bit 1 of field_0x4b0[i] (extrwi): listed in field_0x4b4.
            if (!((field_0x4b0[i] >> 1) & 1)) {
                u8 count = 0;
                for (s32 j = 0; j < field_0x454; j++) {
                    if (field_0x4b4[j] != 0xFF) {
                        count++;
                    }
                }
                field_0x4b4[count] = i;
                gAudioMgr_8053AF80->fn_80458880(0x1E, -1);
                field_0x4b0[i] |= 2;
                if (field_0x3c0[i] == 0xE7) {
                    field_0x174[i]->startSound(0x10008);
                }
            }
        } else if ((field_0x4b0[i] >> 1) & 1) {
            for (s32 j = 0; j < field_0x454; j++) {
                if (field_0x4b4[j] == (u8)i) {
                    // The guard is in the original asm (dropping it loses 4 instructions).
                    if (j < field_0x454 - 1) {
                        for (s32 k = j; k < field_0x454 - 1; k++) {
                            field_0x4b4[k] = field_0x4b4[k + 1];
                        }
                    }
                    field_0x4b4[field_0x454 - 1] = 0xFF;
                }
            }
            field_0x4b0[i] &= ~2;
        }
    }
    for (s32 i = 0; i < field_0x454; i++) {
        if (field_0x4b4[i] != 0xFF) {
            gGameRoot2_8053AB10->field_0x30->fn_8015F8FC(0, &field_0x2f0[field_0x4b4[i]], 5);
        }
    }
}

// FIXME: reloc-name only (instructions identical): the .sdata2 literals / jump table are
// not yet in the original order (most of the unit is unwritten).
void ResultScreen_804AF9A0::fn_803B0158() {
    field_0x440--;
    if (lbl_8053AD30->fn_80309C4C(0x100, -1)) {
        if (field_0x440 >= 60) {
            field_0x70->hide();
            field_0x8c->show();
            // x is a float local before each move() (as tu_803E1668).
            f32 x = field_0x70->getBounds()->i.x;
            field_0x70->move(x, field_0x3f4);
            x = field_0x8c->getBounds()->i.x;
            field_0x8c->move(x, field_0x3f8);
            gAudioMgr_8053AF80->fn_80458880(0x2C, -1);
        }
        field_0x440 = 0;
    }
    s32 timer = field_0x440; // local copy: reading field_0x440 each time changes the codegen
    if (timer == 0) {
        field_0x440 = 150;
        field_0x4c4 = 0;
        field_0x4c6 = 0;
        for (s32 i = 0; i < field_0x454; i++) {
            field_0x3c0[i] = 62;
        }
        field_0x4c1 = 9;
    } else if (timer >= 138) {
        f32 offset = -150.0f * ((f32)(12 - (timer - 138)) / 12.0f);
        f32 x = field_0x70->getBounds()->i.x;
        field_0x70->move(x, field_0x3f4 + offset);
        if (field_0x440 == 138) {
            field_0x70->hide();
            field_0x8c->show();
            x = field_0x8c->getBounds()->i.x;
            field_0x8c->move(x, field_0x3f8 + offset);
        }
    } else if (timer >= 128) {
        f32 x = field_0x8c->getBounds()->i.x;
        f32 offset = -150.0f * ((f32)(timer - 128) / 12.0f);
        field_0x8c->move(x, field_0x3f8 + offset);
        if (field_0x440 == 128) {
            x = field_0x70->getBounds()->i.x;
            field_0x70->move(x, field_0x3f4);
            gAudioMgr_8053AF80->fn_80458880(0x2C, -1);
        }
    }
}

// Not auto-inlined: fn_803AA8F8 calls fn_803AF604 out of line in the original
// (docs/INLINING.md, pragma rule 1).
#pragma auto_inline off
void ResultScreen_804AF9A0::fn_803AF604() {
    field_0x440--;
    if (field_0x440 > 10) {
        if (field_0x440 > 180) {
            if (field_0x440 % 2 == 0) {
                field_0x4c7++;
                gAudioMgr_8053AF80->fn_80458880(0x20, -1);
            }
        } else if (field_0x440 > 120) {
            if (field_0x440 % 3 == 0) {
                field_0x4c7++;
                gAudioMgr_8053AF80->fn_80458880(0x20, -1);
            }
        } else if (field_0x440 > 60) {
            if (field_0x440 % 4 == 0) {
                field_0x4c7++;
                gAudioMgr_8053AF80->fn_80458880(0x20, -1);
            }
        } else if (field_0x440 % 5 == 0) {
            field_0x4c7++;
            gAudioMgr_8053AF80->fn_80458880(0x20, -1);
        }
    } else if (field_0x440 == 10) {
        gAudioMgr_8053AF80->fn_80458880(0x21, -1);
    }
    if (field_0x4c7 >= 9) {
        field_0x4c7 = 0;
    }
    if (field_0x440 == 0) {
        fn_803B35F8(field_0x4c7);
        field_0x440 = 150;
        field_0x4c1 = 13;
    }
}
#pragma auto_inline reset

// FIXME: reloc-name only (instructions identical): the .sdata2 literals / jump table are
// not yet in the original order (most of the unit is unwritten).
void ResultScreen_804AF9A0::fn_803AEC78() {
    field_0x440--;
    if (lbl_8053AD30->fn_80309C4C(0x100, -1)) {
        if (field_0x440 >= 60) {
            field_0x8c->hide();
            field_0x90->show();
            f32 x = field_0x8c->getBounds()->i.x;
            field_0x8c->move(x, field_0x3f8);
            x = field_0x90->getBounds()->i.x;
            field_0x90->move(x, field_0x3fc);
            gAudioMgr_8053AF80->fn_80458880(0x2C, -1);
        }
        field_0x440 = 0;
    }
    s32 timer = field_0x440; // local copy: reading field_0x440 each time changes the codegen
    if (timer == 0) {
        field_0x440 = 150;
        field_0x4c4 = 0;
        field_0x4c6 = 0;
        for (s32 i = 0; i < field_0x454; i++) {
            field_0x3c0[i] = 62;
        }
        field_0x4c1 = 16;
    } else if (timer >= 138) {
        f32 offset = -150.0f * ((f32)(12 - (timer - 138)) / 12.0f);
        f32 x = field_0x8c->getBounds()->i.x;
        field_0x8c->move(x, field_0x3f8 + offset);
        if (field_0x440 == 138) {
            field_0x8c->hide();
            field_0x90->show();
            x = field_0x90->getBounds()->i.x;
            field_0x90->move(x, field_0x3fc + offset);
        }
    } else if (timer >= 128) {
        f32 x = field_0x90->getBounds()->i.x;
        f32 offset = -150.0f * ((f32)(timer - 128) / 12.0f);
        field_0x90->move(x, field_0x3fc + offset);
        if (field_0x440 == 128) {
            x = field_0x90->getBounds()->i.x;
            field_0x90->move(x, field_0x3fc);
            gAudioMgr_8053AF80->fn_80458880(0x2C, -1);
        }
    }
}

// FIXME: reloc-name only (instructions identical): the .sdata2 literals / jump table are
// not yet in the original order (most of the unit is unwritten).
void ResultScreen_804AF9A0::fn_803AE308() {
    field_0x440--;
    if (lbl_8053AD30->fn_80309C4C(0x100, -1)) {
        if (field_0x440 >= 60) {
            field_0x90->hide();
            f32 x = field_0x90->getBounds()->i.x;
            field_0x90->move(x, field_0x3fc);
            field_0x94->show();
            x = field_0x94->getBounds()->i.x;
            field_0x94->move(x, field_0x400);
            gAudioMgr_8053AF80->fn_80458880(0x2C, -1);
        }
        field_0x440 = 0;
    }
    s32 timer = field_0x440; // local copy: reading field_0x440 each time changes the codegen
    if (timer == 0) {
        field_0x440 = 150;
        field_0x4c4 = 0;
        field_0x4c6 = 0;
        for (s32 i = 0; i < field_0x454; i++) {
            field_0x3c0[i] = 62;
        }
        field_0x4c1 = 19;
    } else if (timer >= 138) {
        f32 offset = -150.0f * ((f32)(12 - (timer - 138)) / 12.0f);
        f32 x = field_0x90->getBounds()->i.x;
        field_0x90->move(x, field_0x3fc + offset);
        if (field_0x440 == 138) {
            field_0x90->hide();
            field_0x94->show();
            x = field_0x94->getBounds()->i.x;
            field_0x94->move(x, field_0x400 + offset);
        }
    } else if (timer >= 128) {
        f32 x = field_0x94->getBounds()->i.x;
        f32 offset = -150.0f * ((f32)(timer - 128) / 12.0f);
        field_0x94->move(x, field_0x400 + offset);
        if (field_0x440 == 128) {
            x = field_0x90->getBounds()->i.x;
            field_0x90->move(x, field_0x3fc);
            gAudioMgr_8053AF80->fn_80458880(0x2C, -1);
        }
    }
}

// FIXME: reloc-name only (instructions identical): the .sdata2 literals / jump table are
// not yet in the original order (most of the unit is unwritten).
void ResultScreen_804AF9A0::fn_803ADC0C() {
    field_0x440--;
    if (lbl_8053AD30->fn_80309C4C(0x100, -1)) {
        if (field_0x440 >= 60) {
            field_0x94->hide();
            field_0xa0->show();
            f32 x = field_0x94->getBounds()->i.x;
            field_0x94->move(x, field_0x400);
            x = field_0xa0->getBounds()->i.x;
            field_0xa0->move(x, field_0x410);
            gAudioMgr_8053AF80->fn_80458880(0x2C, -1);
        }
        field_0x440 = 0;
    }
    s32 timer = field_0x440; // local copy: reading field_0x440 each time changes the codegen
    if (timer == 0) {
        field_0x440 = 150;
        field_0x4c4 = 0;
        field_0x4c6 = 0;
        for (s32 i = 0; i < field_0x454; i++) {
            field_0x3c0[i] = 62;
        }
        field_0x4c1 = 21;
    } else if (timer >= 138) {
        f32 offset = -150.0f * ((f32)(12 - (timer - 138)) / 12.0f);
        f32 x = field_0x94->getBounds()->i.x;
        field_0x94->move(x, field_0x400 + offset);
        if (field_0x440 == 138) {
            field_0x94->hide();
            field_0xa0->show();
            x = field_0xa0->getBounds()->i.x;
            field_0xa0->move(x, field_0x410 + offset);
        }
    } else if (timer >= 128) {
        f32 x = field_0xa0->getBounds()->i.x;
        f32 offset = -150.0f * ((f32)(timer - 128) / 12.0f);
        field_0xa0->move(x, field_0x410 + offset);
        if (field_0x440 == 128) {
            x = field_0xa0->getBounds()->i.x;
            field_0xa0->move(x, field_0x410);
            gAudioMgr_8053AF80->fn_80458880(0x2C, -1);
        }
    }
}

// FIXME: reloc-name only (instructions identical): the .sdata2 literals / jump table are
// not yet in the original order (most of the unit is unwritten).
void ResultScreen_804AF9A0::fn_803AD220() {
    field_0x440--;
    if (lbl_8053AD30->fn_80309C4C(0x100, -1)) {
        if (field_0x440 >= 60) {
            field_0x9c->hide();
            f32 x = field_0x9c->getBounds()->i.x;
            field_0x9c->move(x, field_0x404);
            field_0x98->show();
            x = field_0x98->getBounds()->i.x;
            field_0x98->move(x, field_0x408);
            gAudioMgr_8053AF80->fn_80458880(0x2C, -1);
        }
        field_0x440 = 0;
    }
    s32 timer = field_0x440; // local copy: reading field_0x440 each time changes the codegen
    if (timer == 0) {
        field_0x440 = 150;
        field_0x4c4 = 0;
        field_0x4c6 = 0;
        for (s32 i = 0; i < field_0x454; i++) {
            field_0x3c0[i] = 62;
        }
        field_0x4c1 = 26;
    } else if (timer >= 138) {
        f32 offset = -150.0f * ((f32)(12 - (timer - 138)) / 12.0f);
        f32 x = field_0x9c->getBounds()->i.x;
        field_0x9c->move(x, field_0x404 + offset);
        if (field_0x440 == 138) {
            field_0x9c->hide();
            field_0x98->show();
            x = field_0x98->getBounds()->i.x;
            field_0x98->move(x, field_0x408 + offset);
        }
    } else if (timer >= 128) {
        f32 x = field_0x98->getBounds()->i.x;
        f32 offset = -150.0f * ((f32)(timer - 128) / 12.0f);
        field_0x98->move(x, field_0x408 + offset);
        if (field_0x440 == 128) {
            x = field_0x9c->getBounds()->i.x;
            field_0x9c->move(x, field_0x404);
            gAudioMgr_8053AF80->fn_80458880(0x2C, -1);
        }
    }
}

// FIXME: 98.43. Registers, and in state 21 the original sets `done` to 0 and copies it
// into i (li; mr) where ours loads 0 twice; also .sdata2 / jump table / ids names (the
// literal pool is not yet in the original order).
void ResultScreen_804AF9A0::fn_803AA8F8() {
    switch (field_0x4c1) {
    case 0:
        break;
    case 1:
        field_0x440--;
        if (field_0x440 == 0) {
            field_0x440 = 150;
            if (field_0x450 < 2) {
                // The original scales the alphas by a float variable that is 0 here
                // (lfs 0.0f, fmuls at run time); a literal 0.0f factor folds away.
                f32 rate = 0.0f;
                field_0x10[field_0x450]->search('Ndata01')->show();
                field_0x10[field_0x450]->search('Ndata02')->show();
                field_0x10[field_0x450]->search('Pmmwk02')->show();
                if (field_0x450 < 2) {
                    for (int i = 0; i < 13; i++) {
                        field_0x10[field_0x450]->search(lbl_80476208[i])->setAlpha(field_0x4c9[i] * rate);
                    }
                }
                for (s32 i = 0; i < field_0x454; i++) {
                    if (field_0x450 < 2) {
                        for (int j = 0; j < 3; j++) {
                            field_0x10[field_0x450]->search(lbl_80476270[i][j])->setAlpha(field_0x4d6[j] * rate);
                        }
                    }
                    if (field_0x450 < 2) {
                        for (int j = 0; j < 2; j++) {
                            field_0x10[field_0x450]->search(lbl_804762D0[i][j])->setAlpha(field_0x4d9[j] * rate);
                        }
                    }
                    field_0x4b8[i] = 0xFF;
                    field_0x460[i] = 0;
                }
                field_0x4c1 = 2;
            } else {
                field_0x4c1 = 8;
            }
        }
        break;
    case 2:
        fn_803B1B60();
        break;
    case 3:
        fn_803B14F8();
        break;
    case 4:
        fn_803B0FD4();
        break;
    case 5:
        break;
    case 6:
        fn_803B0978();
        break;
    case 7:
        fn_803B03E8();
        break;
    case 8:
        fn_803B0158();
        break;
    case 9:
        fn_803AFCF0();
        break;
    case 10:
        fn_803AFA78();
        break;
    case 11:
        fn_803AF79C();
        break;
    case 12:
        fn_803AF604();
        break;
    case 13:
        fn_803AF1B0();
        break;
    case 14:
        fn_803AEF08();
        break;
    case 15:
        fn_803AEC78();
        break;
    case 16:
        fn_803AE810();
        break;
    case 17:
        fn_803AE598();
        break;
    case 18:
        fn_803AE308();
        break;
    case 19:
        fn_803ADE9C();
        break;
    case 20:
        fn_803ADC0C();
        break;
    case 21: {
        // Count each player's field_0x370 up (by 2) to its target.
        s32 done = 0;
        for (s32 i = 0; i < field_0x454; i++) {
            if (field_0x370[i] < fn_80403070(fn_80403414(i))) {
                field_0x370[i] += 2;
            }
            if (field_0x370[i] >= fn_80403070(fn_80403414(i))) {
                field_0x370[i] = fn_80403070(fn_80403414(i));
                done++;
            }
        }
        if (done >= field_0x454) {
            field_0x4c8 = 0xFF;
            field_0x438 = 0;
            field_0x440 = 150;
            gAudioMgr_8053AF80->fn_80458880(0xF, -1);
            field_0x438 = 0;
            if (field_0x450 < 2) {
                field_0x4c1 = 22;
            } else {
                field_0x4c1 = 28;
            }
        } else {
            if (done != field_0x438) {
                field_0x438 = done;
                gAudioMgr_8053AF80->fn_80458880(0x10, -1);
            }
            gAudioMgr_8053AF80->fn_80458880(0xE, -1);
        }
        break;
    }
    case 22:
        fn_803AD894();
        break;
    case 23:
        fn_803AD4B0();
        break;
    case 24:
        field_0x440--;
        if (field_0x4dc < 254) {
            field_0x4dc += 2;
        } else {
            field_0x4dc = 0xFF;
        }
        if (field_0x440 == 0) {
            field_0x440 = 150;
            field_0x4c1 = 25;
        }
        break;
    case 25:
        fn_803AD220();
        break;
    case 26:
        fn_803ACDA4();
        break;
    case 27:
        field_0x440--;
        if (field_0x4dd > 1) {
            field_0x4dd -= 2;
        } else {
            field_0x4dd = 0;
        }
        if (field_0x440 == 149) {
            static const u16 ids[4] = {0x477, 0x479, 0x476, 0x478};
            for (s32 i = 0; i < field_0x454; i++) {
                if (field_0x4db & (u8)(1 << (i + 4))) {
                    if (field_0x188[i] == NULL) {
                        field_0x188[i] = spawnEmitter_8013CC50(0, &field_0x2f0[i], ids[i], 0, -1, NULL, NULL, true, 1.0f);
                    }
                    if (field_0x198[i] == NULL) {
                        field_0x198[i] = spawnEmitter_8013CC50(0, &field_0x2f0[i], 0x475, 0, -1, NULL, NULL, true, 1.0f);
                    }
                }
            }
        }
        if (field_0x440 == 0) {
            for (s32 i = 0; i < field_0x454; i++) {
                if (field_0x188[i] != NULL) {
                    field_0x188[i]->becomeInvalidEmitter();
                    field_0x188[i] = NULL;
                }
                if (field_0x198[i] != NULL) {
                    field_0x198[i]->becomeInvalidEmitter();
                    field_0x198[i] = NULL;
                }
                // Set once per player in the original.
                field_0x440 = 150;
                field_0x4c1 = 28;
            }
        }
        break;
    case 28:
        fn_803AC998();
        break;
    case 29:
        fn_803AC6FC();
        break;
    case 30:
        fn_803AC154();
        break;
    case 31: {
        // 'Pfobk02' -> 'Pttlbk05', then state 32 (as fn_803B0158, no skip).
        field_0x440--;
        s32 timer = field_0x440; // local copy, as fn_803B0158
        if (timer == 0) {
            field_0x440 = 150;
            gAudioMgr_8053AF80->fn_80458880(0x2C, -1);
            field_0x440 = 150;
            field_0x4c1 = 32;
        } else if (timer >= 138) {
            f32 offset = -150.0f * ((f32)(12 - (timer - 138)) / 12.0f);
            f32 x = field_0xa4->getBounds()->i.x;
            field_0xa4->move(x, field_0x40c + offset);
            if (field_0x440 == 138) {
                field_0xa4->hide();
                field_0x70->show();
                x = field_0x70->getBounds()->i.x;
                field_0x70->move(x, field_0x3f4 + offset);
            }
        } else if (timer >= 128) {
            f32 x = field_0x70->getBounds()->i.x;
            f32 offset = -150.0f * ((f32)(timer - 128) / 12.0f);
            field_0x70->move(x, field_0x3f4 + offset);
            if (field_0x440 == 128) {
                // field_0x70's x, but field_0xa4 is moved (as in the original).
                x = field_0x70->getBounds()->i.x;
                field_0xa4->move(x, field_0x3f4);
            }
        }
        break;
    }
    case 32:
        fn_803ABCEC();
        break;
    case 33:
        fn_803AB494();
        break;
    }
    for (s32 i = 0; i < field_0x454; i++) {
        field_0x3e0[i] = field_0x320[i] + field_0x340[i] + field_0x350[i] + field_0x360[i] + field_0x370[i] -
                         field_0x380[i] + field_0x3a0[i] + field_0x3b0[i];
        if (field_0x3e0[i] < -5000) {
            field_0x3e0[i] = -5000;
        }
        f32 offset;
        if (field_0x3e0[i] < -500) {
            offset = -500 * 150.0f / field_0x434 * lbl_8050DB0C.field_0x4c[(0x2B0 - 0x4C) / 2] / 3000.0f;
        } else if (field_0x3e0[i] > 3000) {
            offset = 3000 * 150.0f / field_0x434 * lbl_8050DB0C.field_0x4c[(0x2B0 - 0x4C) / 2] / 3000.0f;
        } else {
            offset = lbl_8050DB0C.field_0x4c[(0x2B0 - 0x4C) / 2] * (150.0f * field_0x3e0[i] / field_0x434) / 3000.0f;
        }
        f32 x = field_0x158[i]->getBounds()->i.x;
        field_0x158[i]->move(x, field_0x3f0 - offset);
        if (field_0x3e0[i] > 9999) {
            field_0x3e0[i] = 9999;
        }
        field_0x2f0[i].x = 256.0f *
                           (field_0x158[i]->getWidth() / 2 +
                            (field_0x168->getBounds()->i.x + field_0x158[i]->getBounds()->i.x)) /
                           (s32)JUTVideo::getManager()->getFbWidth();
        field_0x2f0[i].y = 192.0f *
                           ((field_0x168->getBounds()->i.y + field_0x158[i]->getBounds()->f.y) - 526.0f - 18.0f) /
                           (s32)JUTVideo::getManager()->getEfbHeight();
        field_0x2f0[i].z = 0.0f;
    }
    fn_803B2FB0();
}

// FIXME: 98.67. Registers (the second player loop), and each of the two ifs of the
// clamp of lbl_8050EC80.getCurrentFile().field_0x10 goes through an extra `addi 0x30`
// pointer (the original keeps the file row address and stores at 0x30: the save-data
// row shape of docs/CRACK_PASS.md "Save-data layout"); also .sdata2 names.
void ResultScreen_804AF9A0::fn_803AA1F8() {
    if (field_0x4c8 != 0xFF) {
        s32 rank = 0;
        field_0x444--;
        if (field_0x444 == 0) {
            for (s32 i = 0; i < field_0x454; i++) {
                if (i == field_0x4ac[rank]) {
                    if (field_0x1b8[i] == NULL) {
                        JGeometry::TVec3f pos;
                        pos.x = field_0x158[i]->getWidth() / 2 + field_0x158[i]->getGlobalBounds().i.x;
                        pos.y = 0.0f;
                        pos.z = 0.0f;
                        field_0x1b8[i] = spawnEmitter_8013CC50(0, &pos, 0x241, 0, 0, NULL, NULL, true, 1.0f);
                    }
                    if (field_0x4c8 > rank) {
                        rank++;
                    }
                }
            }
            field_0x444 = 240;
        }
    }
    for (s32 i = 0; i < field_0x454; i++) {
        if (field_0x3d0[i] != 0) {
            field_0x3d0[i]++;
            if (field_0x3d0[i] < 8) {
                field_0x3c0[i] = 0xC4;
            } else if (field_0x3d0[i] < 16) {
                field_0x3c0[i] = 0xC3;
            } else if (field_0x3d0[i] < 24) {
                field_0x3c0[i] = 0xC4;
            } else {
                field_0x3c0[i] = 0xC3;
                field_0x3d0[i] = 0;
            }
        } else if (field_0x3c0[i] == 0xC3) {
            if (lbl_8053AD30->fn_80309D6C(getPlayerInputSlot_8030AAF4(i), 0x100)) {
                field_0x3d0[i]++;
                field_0x3c0[i] = 0xC4;
                field_0x174[field_0x4ac[i]]->startSound(0x20032);
            }
        }
    }
    for (s32 i = 0; i < field_0x454; i++) {
        field_0x3e0[i] = field_0x320[i] + field_0x340[i] + field_0x350[i] + field_0x360[i] + field_0x370[i] -
                         field_0x380[i] + field_0x3a0[i] + field_0x3b0[i];
        if (field_0x3e0[i] < -5000) {
            field_0x3e0[i] = -5000;
        }
        f32 offset;
        if (field_0x3e0[i] < -500) {
            offset = -500 * 150.0f / field_0x434 * lbl_8050DB0C.field_0x4c[(0x2B0 - 0x4C) / 2] / 3000.0f;
        } else if (field_0x3e0[i] > 3000) {
            offset = 3000 * 150.0f / field_0x434 * lbl_8050DB0C.field_0x4c[(0x2B0 - 0x4C) / 2] / 3000.0f;
        } else {
            offset = lbl_8050DB0C.field_0x4c[(0x2B0 - 0x4C) / 2] * (150.0f * field_0x3e0[i] / field_0x434) / 3000.0f;
        }
        f32 x = field_0x158[i]->getBounds()->i.x;
        field_0x158[i]->move(x, field_0x3f0 - offset);
        if (field_0x3e0[i] > 9999) {
            field_0x3e0[i] = 9999;
        }
        field_0x2f0[i].x = 256.0f *
                           (field_0x158[i]->getWidth() / 2 +
                            (field_0x168->getBounds()->i.x + field_0x158[i]->getBounds()->i.x)) /
                           (s32)JUTVideo::getManager()->getFbWidth();
        field_0x2f0[i].y = 192.0f *
                           ((field_0x168->getBounds()->i.y + field_0x158[i]->getBounds()->f.y) - 526.0f - 18.0f) /
                           (s32)JUTVideo::getManager()->getEfbHeight();
        field_0x2f0[i].z = 0.0f;
    }
    fn_803B2FB0();
    if (field_0x4c8 != 0xFF) {
        fn_803B236C();
        for (int i = 0; i < 10; i++) {
            if (field_0x2c8[i] >= 0) {
                field_0x2c8[i]--;
                if (field_0x2c8[i] <= -1) {
                    field_0x244.x = field_0x250[i].x;
                    field_0x244.y = field_0x250[i].y;
                    field_0x244.z = field_0x250[i].z;
                    field_0x2c8[i] = -1;
                    field_0x184->startSound(0x3002A);
                    break;
                }
            }
        }
    }
    if (lbl_8053AD30->fn_80309C4C(0x1100, -1)) {
        lbl_8050EC80.getCurrentFile().field_0x10 += field_0x320[0] + field_0x320[1] + field_0x320[2] + field_0x320[3];
        if (lbl_8050EC80.getCurrentFile().field_0x10 > 99999) {
            lbl_8050EC80.getCurrentFile().field_0x10 = 99999;
        }
        if (lbl_8050EC80.getCurrentFile().field_0x10 < -99999) {
            lbl_8050EC80.getCurrentFile().field_0x10 = -99999;
        }
        field_0x4c0 = 4;
        gAudioMgr_8053AF80->fn_80458880(0xB, -1);
    }
    for (s32 i = 0; i < 30; i++) {
        if (field_0x1c8[i] != NULL && field_0x1c8[i]->isEnableDeleteEmitter()) {
            JPAEmitterManager* manager = getEffectSystem_8013C824()->mEmitterManager;
            manager->forceDeleteEmitter(field_0x1c8[i]);
            field_0x1c8[i] = NULL;
        }
    }
}


// FIXME: 98.71. Registers (the second player loop), and each of the two ifs of the
// clamp of lbl_8050EC80.getCurrentFile().field_0x10 goes through an extra `addi 0x30`
// pointer (the original keeps the file row address and stores at 0x30: the save-data
// row shape of docs/CRACK_PASS.md "Save-data layout"); also .sdata2 names.
void ResultScreen_804AF9A0::fn_803A9AB8() {
    if (field_0x4c8 != 0xFF) {
        s32 rank = 0;
        field_0x444--;
        if (field_0x444 == 0) {
            for (s32 i = 0; i < field_0x454; i++) {
                if (i == field_0x4ac[rank]) {
                    if (field_0x1b8[i] == NULL) {
                        JGeometry::TVec3f pos;
                        pos.x = field_0x158[i]->getWidth() / 2 + field_0x158[i]->getGlobalBounds().i.x;
                        pos.y = 0.0f;
                        pos.z = 0.0f;
                        field_0x1b8[i] = spawnEmitter_8013CC50(0, &pos, 0x241, 0, 0, NULL, NULL, true, 1.0f);
                    }
                    if (field_0x4c8 > rank) {
                        rank++;
                    }
                }
            }
            field_0x444 = 240;
        }
    }
    for (s32 i = 0; i < field_0x454; i++) {
        if (field_0x3d0[i] != 0) {
            field_0x3d0[i]++;
            if (field_0x3d0[i] < 8) {
                field_0x3c0[i] = 0xC4;
            } else if (field_0x3d0[i] < 16) {
                field_0x3c0[i] = 0xC3;
            } else if (field_0x3d0[i] < 24) {
                field_0x3c0[i] = 0xC4;
            } else {
                field_0x3c0[i] = 0xC3;
                field_0x3d0[i] = 0;
            }
        } else if (field_0x3c0[i] == 0xC3) {
            if (lbl_8053AD30->fn_80309D6C(getPlayerInputSlot_8030AAF4(i), 0x100)) {
                field_0x3d0[i]++;
                field_0x3c0[i] = 0xC4;
                field_0x174[field_0x4ac[i]]->startSound(0x20032);
            }
        }
    }
    for (s32 i = 0; i < field_0x454; i++) {
        field_0x3e0[i] = field_0x320[i] + field_0x340[i] + field_0x350[i] + field_0x360[i] + field_0x370[i] -
                         field_0x380[i] + field_0x3a0[i] + field_0x3b0[i];
        if (field_0x3e0[i] < -5000) {
            field_0x3e0[i] = -5000;
        }
        f32 offset;
        if (field_0x3e0[i] < -500) {
            offset = -500 * 150.0f / field_0x434 * lbl_8050DB0C.field_0x4c[(0x2B0 - 0x4C) / 2] / 3000.0f;
        } else if (field_0x3e0[i] > 3000) {
            offset = 3000 * 150.0f / field_0x434 * lbl_8050DB0C.field_0x4c[(0x2B0 - 0x4C) / 2] / 3000.0f;
        } else {
            offset = lbl_8050DB0C.field_0x4c[(0x2B0 - 0x4C) / 2] * (150.0f * field_0x3e0[i] / field_0x434) / 3000.0f;
        }
        f32 x = field_0x158[i]->getBounds()->i.x;
        field_0x158[i]->move(x, field_0x3f0 - offset);
        if (field_0x3e0[i] > 9999) {
            field_0x3e0[i] = 9999;
        }
        field_0x2f0[i].x = 256.0f *
                           (field_0x158[i]->getWidth() / 2 +
                            (field_0x168->getBounds()->i.x + field_0x158[i]->getBounds()->i.x)) /
                           (s32)JUTVideo::getManager()->getFbWidth();
        field_0x2f0[i].y = 192.0f *
                           ((field_0x168->getBounds()->i.y + field_0x158[i]->getBounds()->f.y) - 526.0f - 18.0f) /
                           (s32)JUTVideo::getManager()->getEfbHeight();
        field_0x2f0[i].z = 0.0f;
    }
    fn_803B2FB0();
    if (field_0x4c8 != 0xFF) {
        fn_803B236C();
        for (int i = 0; i < 10; i++) {
            if (field_0x2c8[i] >= 0) {
                field_0x2c8[i]--;
                if (field_0x2c8[i] <= -1) {
                    field_0x244.x = field_0x250[i].x;
                    field_0x244.y = field_0x250[i].y;
                    field_0x244.z = field_0x250[i].z;
                    field_0x2c8[i] = -1;
                    field_0x184->startSound(0x3002A);
                    break;
                }
            }
        }
    }
    if (lbl_8053AD30->fn_80309C4C(0x1100, -1)) {
        lbl_8050EC80.getCurrentFile().field_0x10 += field_0x320[0] + field_0x320[1] + field_0x320[2] + field_0x320[3];
        if (lbl_8050EC80.getCurrentFile().field_0x10 > 99999) {
            lbl_8050EC80.getCurrentFile().field_0x10 = 99999;
        }
        if (lbl_8050EC80.getCurrentFile().field_0x10 < -99999) {
            lbl_8050EC80.getCurrentFile().field_0x10 = -99999;
        }
        field_0x4c0 = 6;
        gAudioMgr_8053AF80->fn_80458880(0xB, -1);
        for (s32 i = 0; i < 30; i++) {
            deleteEmitter(&field_0x1c8[i]);
        }
    } else {
        for (s32 i = 0; i < 30; i++) {
            if (field_0x1c8[i] != NULL && field_0x1c8[i]->isEnableDeleteEmitter()) {
                JPAEmitterManager* manager = getEffectSystem_8013C824()->mEmitterManager;
                manager->forceDeleteEmitter(field_0x1c8[i]);
                field_0x1c8[i] = NULL;
            }
        }
    }
}

// FIXME: 98.53. The original unrolls the field_0x1b8 loop of mode 6 in two halves (the
// second through a base register this + 8, one addi more); ours unrolls it flat. Also
// .sdata2 / jump table names (literal pool not yet in the original order).
void ResultScreen_804AF9A0::fn_803A9830() {
    switch (field_0x4c0) {
    case 0:
        field_0x440--;
        if (field_0x440 == 0) {
            field_0x4c0 = 1;
        }
        break;
    case 1:
        field_0x4c2 += 2;
        if (field_0x4c2 >= 254) {
            field_0x4c2 = 255;
            field_0x4c0 = 2;
            field_0x4c1 = 1;
            gGameRoot2_8053AB10->field_0x20->field_0x235c[0] = 0;
            gGameRoot2_8053AB10->field_0x20->field_0x235c[1] = 0;
            gGameRoot2_8053AB10->field_0x20->field_0x235c[2] = 0;
            gGameRoot2_8053AB10->field_0x20->field_0x235c[3] = 0;
            fn_80128C58(0xFF);
            u32 color = 0xFFFFFF00;
            fn_8013A884(&color);
        }
        break;
    case 2:
        fn_803AA8F8();
        break;
    case 3:
        fn_803AA1F8();
        break;
    case 4:
        fn_803A9AB8();
        break;
    case 6:
        if (field_0x4c2 >= 245) {
            field_0x4c2 = 255;
            if (lbl_80529DEC.field_0x24d == 1) {
                field_0x4c0 = 5;
                for (int i = 0; i < 4; i++) {
                    if (field_0x1b8[i] != NULL) {
                        field_0x1b8[i]->becomeInvalidEmitter();
                        field_0x1b8[i] = NULL;
                    }
                }
            } else {
                field_0x4c0 = 9;
            }
        } else {
            field_0x4c2 += 10;
        }
        break;
    case 5:
        if (field_0x4c2 <= 10) {
            field_0x4c2 = 0;
            lbl_8053AD68->field_0x1d = 1;
        } else {
            field_0x4c2 -= 10;
        }
        break;
    }
    field_0x44c++;
    if (field_0x44c >= field_0x24[field_0x450]->getFrameMax()) {
        field_0x44c = 0;
    }
    field_0x24[field_0x450]->setFrame(field_0x44c);
    field_0x4[field_0x450]->animation();
}

// FIXME: reloc-name only (instructions identical): the .sdata2 literals / jump table are
// not yet in the original order (most of the unit is unwritten).
void ResultScreen_804AF9A0::fn_803A97A0() {
    gGameRoot2_8053AB10->fn_8013B490();
    if (field_0x4c0 >= 2) {
        field_0x16c->setAlpha(255 - field_0x4c2);
    } else {
        field_0x16c->setAlpha(0);
    }
    field_0x1c->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
}

// FIXME: 99.66. Regalloc (the sprite frame of the second loop is in r21, the original
// r23) and .sdata2 names (literal pool not yet in the original order).
void ResultScreen_804AF9A0::fn_803A8F5C() {
    gGameRoot2_8053AB10->fn_8013B490();
    if (field_0x4c0 >= 2) {
        // Each player's score, digit by digit; leading zeros are hidden and
        // a minus sign goes in front of the first digit shown.
        for (s32 i = 0; i < field_0x454; i++) {
            s32 score = field_0x3e0[i];
            bool shown = false;
            bool negative = false;
            if (score < 0) {
                score *= -1;
                negative = true;
            }
            field_0xa8[i]->hide();
            field_0xf8[i]->hide();
            s32 digit = score % 10000 / 1000;
            if (digit != 0) {
                field_0xb8[i]->show();
                field_0x108[i]->show();
                snprintf(field_0xb8[i]->getStringPtr(), 2, "%1d", digit);
                snprintf(field_0x108[i]->getStringPtr(), 2, "%1d", digit);
                if (negative) {
                    field_0xa8[i]->show();
                    field_0xf8[i]->show();
                    snprintf(field_0xa8[i]->getStringPtr(), 2, "-");
                    snprintf(field_0xf8[i]->getStringPtr(), 2, "-");
                }
                shown = true;
            } else {
                field_0xb8[i]->hide();
                field_0x108[i]->hide();
            }
            digit = score % 1000 / 100;
            if (digit != 0 || shown) {
                field_0xc8[i]->show();
                field_0x118[i]->show();
                snprintf(field_0xc8[i]->getStringPtr(), 2, "%1d", digit);
                snprintf(field_0x118[i]->getStringPtr(), 2, "%1d", digit);
                if (!shown) {
                    if (negative) {
                        field_0xb8[i]->show();
                        field_0x108[i]->show();
                        snprintf(field_0xb8[i]->getStringPtr(), 2, "-");
                        snprintf(field_0x108[i]->getStringPtr(), 2, "-");
                    }
                    shown = true;
                }
            } else {
                field_0xc8[i]->hide();
                field_0x118[i]->hide();
            }
            digit = score % 100 / 10;
            if (digit != 0 || shown) {
                field_0xd8[i]->show();
                field_0x128[i]->show();
                snprintf(field_0xd8[i]->getStringPtr(), 2, "%1d", digit);
                snprintf(field_0x128[i]->getStringPtr(), 2, "%1d", digit);
                if (!shown) {
                    if (negative) {
                        field_0xc8[i]->show();
                        field_0x118[i]->show();
                        snprintf(field_0xc8[i]->getStringPtr(), 2, "-");
                        snprintf(field_0x118[i]->getStringPtr(), 2, "-");
                    }
                    shown = true;
                }
            } else {
                field_0xd8[i]->hide();
                field_0x128[i]->hide();
            }
            digit = score % 10;
            snprintf(field_0xe8[i]->getStringPtr(), 2, "%1d", digit);
            snprintf(field_0x138[i]->getStringPtr(), 2, "%1d", digit);
            if (!shown && negative) {
                field_0xd8[i]->show();
                field_0x128[i]->show();
                snprintf(field_0xd8[i]->getStringPtr(), 2, "-");
                snprintf(field_0x128[i]->getStringPtr(), 2, "-");
            }
        }
    }
    field_0x4[field_0x450]->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
    JGeometry::TBox2f box(0.0f, 0.0f, 256.0f, 192.0f);
    gGameRoot2_8053AB10->fn_8013B414(&box, true);
    for (s32 i = 0; i < field_0x454; i++) {
        SpriteParams_801F5AE0 params;
        params.field_0x0[7] = i + 12;
        params.field_0x26 = 1;
        if (field_0x4c0 >= 3) {
            params.field_0x1d = -1;
            params.field_0x1c = -1;
        }
        s32 frame = field_0x3c0[i];
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "player_sob.bin");
        gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&field_0x2f0[i], res, frame, &params, 1.0f, 1.0f);
        if (field_0x4db & (u8)(1 << i)) {
            SpriteParams_801F5AE0 params2;
            if (field_0x4db & (u8)(1 << (i + 4))) {
                params2.field_0x10.a = field_0x4dd;
            } else {
                params2.field_0x10.a = field_0x4dc;
            }
            params2.field_0x0[7] = 7;
            params2.field_0x26 = 1;
            if (field_0x4c0 >= 3) {
                params2.field_0x1d = -1;
                params2.field_0x1c = -1;
            }
            frame = field_0x3c0[i];
            res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "player_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&field_0x2f0[i], res, frame, &params2, 1.0f, 1.0f);
        }
    }
    gGameRoot2_8053AB10->fn_8013B490();
    field_0x10[field_0x450]->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
    field_0x20->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
    if (field_0x4c0 == 3 || field_0x4c0 == 4) {
        lbl_8053ADC8->field_0x20 = 1;
        lbl_8053ADC8->field_0x21 = 4;
    }
    field_0x170->setAlpha(field_0x4c2);
    field_0x170->draw(0.0f, 0.0f, 800.0f, 640.0f, false, false, false);
}

s32 ResultScreen_804AF9A0::fn_803A8DFC(s32 player) {
    s32 other = 0;
    switch (player) {
    case 0:
        switch (fn_8037EFB4(player)) {
        case 0:
            other = 1;
            break;
        case 1:
            other = 2;
            break;
        case 2:
            other = 3;
            break;
        }
        break;
    case 1:
        switch (fn_8037EFB4(player)) {
        case 0:
            other = 0;
            break;
        case 1:
            other = 2;
            break;
        case 2:
            other = 3;
            break;
        }
        break;
    case 2:
        switch (fn_8037EFB4(player)) {
        case 0:
            other = 0;
            break;
        case 1:
            other = 1;
            break;
        case 2:
            other = 3;
            break;
        }
        break;
    case 3:
        switch (fn_8037EFB4(player)) {
        case 0:
            other = 0;
            break;
        case 1:
            other = 1;
            break;
        case 2:
            other = 2;
            break;
        }
        break;
    }
    return other;
}
