#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/cls_804B1A38.h"
#include "JSystem/J2D/J2DPane.h"
#include "JSystem/J2D/J2DPicture.h"
#include "JSystem/J2D/J2DGrafContext.h"
#include "JSystem/JUtility/JUTFont.h"
#include "JSystem/JUtility/JUTVideo.h"
#include "JSystem/JFramework/JFWDisplay.h"
#include "JSystem/J3D/J3DSys.h"
#include "JSystem/ResTIMG.h"
#include "dolphin/dvd.h"

// The disc-error screen: the same module as zeldaret/tp's d/d_error_msg.cpp
// (dDvdErrorMsg_c); its structure is borrowed from there, the bodies follow
// FSA's asm.

// An 8x8 I4 texture, all black: a ResTIMG header and its 0x20 bytes of
// image data (tp's black_tex).
struct BlackTex_80479C60 {
    /* 0x00 */ ResTIMG mHeader;
    /* 0x20 */ u8 mImage[0x20];
};

static const BlackTex_80479C60 lbl_80479C60 = {
    {
        GX_TF_I4, 0,      // format, transparency
        8, 8,             // width, height
        GX_REPEAT, GX_REPEAT,
        0, 0, 0, 0,       // no palette
        GX_FALSE, GX_FALSE, GX_FALSE, GX_FALSE,
        GX_LINEAR, GX_LINEAR,
        0, 0,             // min / max LOD
        1, 0,             // one image
        0,                // LOD bias
        0x20,             // image data offset
    },
    {0},
};

// The screen's message file (MESGbmg1: Reading the Game Disc, Disc Cover is
// open, ...; tp's msg_data) and its font (FONTbfn1; tp's font_data). Both are
// extracted from the original at build time (config.yml extract:).
static const u8 lbl_80479CA0[0x260] = {
#include "assets/game/tu_80400DC4/msg_data.inc"
};

static const u8 lbl_80479F00[0x12260] = {
#include "assets/game/tu_80400DC4/font_data.inc"
};

// The message file's INF1 block, 0x20 bytes in (after the file header): the
// offset of each message in the DAT1 block that follows it (tp's BMG_INF1).
struct BmgInf1_80479CA0 {
    /* 0x00 */ u32 mMagic; // 'INF1'
    /* 0x04 */ u32 mSize;
    /* 0x08 */ u16 mEntryNum;
    /* 0x0A */ u16 mEntrySize;
    /* 0x0C */ u8 field_0xc[0x10 - 0xC];
    /* 0x10 */ u32 mEntries[6];
};

s32 cls_804B0F90::fn_80401518() {
    static BOOL lbl_8053ADE0 = FALSE;

    s32 status = DVDGetDriveStatus();
    if (status != DVD_STATE_END && status != DVD_STATE_BUSY) {
        lbl_8053ADE0 = TRUE;
    }
    if (lbl_8053ADE0) {
        if (status == DVD_STATE_END) {
            lbl_8053ADE0 = FALSE;
        } else if (DVDGetDriveStatus() != DVD_STATE_FATAL_ERROR && lbl_8053ACA8->fn_802B1DCC()) {
            lbl_8053ACA8->fn_802B1DE8();
            lbl_8053ACA8->fn_802B1BE0(1, 0x80000000, false);
        } else {
            fn_80401318(status);
        }
    }
    if (lbl_8053ADE0 == TRUE) {
        lbl_80529DEC.field_0x250 = 1;
    }
    return lbl_8053ADE0;
}

// Defined below fn_80401518: the original's .sbss has that function's static
// first (0x8053ADE0) and this object after it (0x8053ADE8).
cls_804B0F90 lbl_8053ADE8;

void cls_804B0F90::fn_80401318(s32 status) {
    JFWDisplay::getManager()->beginRender();
    if (lbl_80529DEC.field_0x251 == 0 || lbl_80529DEC.field_0x251 > 0x5A) {
        j3dSys.drawInit();
        J2DOrthoGraph ortho(0.0f, 0.0f, (s32)JUTVideo::getManager()->getFbWidth(),
                            (s32)JUTVideo::getManager()->getEfbHeight(), -1.0f, 1.0f);
        JFWDisplay::getManager()->setClearColor(JUtility::TColor(0, 0, 0, 0));
        ortho.setPort();
        if (status == DVD_STATE_NO_DISK) {
            fn_80400DC4(2);
        } else if (status == DVD_STATE_COVER_OPEN) {
            fn_80400DC4(1);
        } else if (status == DVD_STATE_WRONG_DISK) {
            fn_80400DC4(3);
        } else if (status == DVD_STATE_RETRY) {
            fn_80400DC4(4);
        } else if (status == DVD_STATE_BUSY) {
            fn_80400DC4(0);
        } else if (status == DVD_STATE_FATAL_ERROR) {
            fn_80400DC4(5);
        }
    }
    JFWDisplay::getManager()->endRender();
    JFWDisplay::getManager()->endFrame();
    if (status == DVD_STATE_FATAL_ERROR && lbl_8053ADD8->field_0x4) {
        gAudioMgr_8053AF80->fn_804595E4(true);
    }
}

void cls_804B0F90::fn_80400DC4(u32 index) {
    const BmgInf1_80479CA0* inf1 = (const BmgInf1_80479CA0*)&lbl_80479CA0[0x20];
    // The DAT1 block follows INF1; its text starts after its 8-byte header.
    const char* msg = (const char*)inf1 + inf1->mSize + 8 + inf1->mEntries[index];
    f32 width = (s32)JUTVideo::getManager()->getFbWidth();
    J2DTextBox::TFontSize size;
    size.x = 27.0f;
    size.y = 27.0f;

    J2DTextBox tpane('TEXT1', JGeometry::TBox2f(0.0f, 0.0f, JGeometry::TVec2f(width, 200.0f)),
                     (const ResFONT*)lbl_80479F00, msg, -1, J2DHBIND_Center, J2DVBIND_Center);
    J2DTextBox spane('TEXT2', JGeometry::TBox2f(0.0f, 0.0f, JGeometry::TVec2f(width, 200.0f)),
                     (const ResFONT*)lbl_80479F00, msg, -1, J2DHBIND_Center, J2DVBIND_Center);
    J2DPicture ppane('PICT1', JGeometry::TBox2f(0.0f, 0.0f, 665.0f, 530.0f), &lbl_80479C60.mHeader,
                     NULL);

    tpane.setFontSize(size);
    tpane.setCharColor(JUtility::TColor(0xFF, 0xC8, 0x00, 0xFF));
    tpane.setGradColor(JUtility::TColor(0xFF, 0xB4, 0x00, 0xFF));
    tpane.setCharSpace(0.0f);
    tpane.setLineSpace(27.0f);
    tpane.setBlack(JUtility::TColor(0xFF, 0xFF, 0xFF, 0x00));

    spane.setFontSize(size);
    spane.setCharColor(JUtility::TColor(0x00, 0x00, 0x00, 0xC8));
    spane.setGradColor(JUtility::TColor(0x00, 0x00, 0x00, 0xC8));
    spane.setCharSpace(0.0f);
    spane.setLineSpace(27.0f);

    // Set up like tp's background, but FSA never draws it.
    ppane.setAlpha(0x82);

    JUTResFont font((const ResFONT*)lbl_80479F00, NULL);
    JUTFont* pfont = &font;

    s16 height = 27;
    f32 maxWidth = 0.0f;
    int cnt = 0;
    f32 lineWidth[6];
    for (int i = 0; i < 6; i++) {
        lineWidth[i] = 0.0f;
    }
    for (; *msg != '\0'; msg++) {
        if (*msg == '\n') {
            height += 27;
            cnt++;
        } else {
            lineWidth[cnt] += pfont->getWidth((u8)*msg);
        }
    }

    for (int i = 0; i < 6; i++) {
        if (maxWidth < lineWidth[i]) {
            maxWidth = lineWidth[i];
        }
    }

    f32 x = ((s32)JUTVideo::getManager()->getFbWidth() - maxWidth) / 2.0f;
    f32 y = (JUTVideo::getManager()->getEfbHeight() - height) / 2.0f;
    spane.draw(x + 2.0f, y + 10.0f + 2.0f, width, J2DHBIND_Left);
    tpane.draw(x, y + 10.0f, width, J2DHBIND_Left);
}
