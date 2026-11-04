#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/tu_80448940.h"
#include "JSystem/J2D/J2DGrafContext.h"
#include "JSystem/J2D/J2DPicture.h"
#include "JSystem/J2D/J2DScreen.h"

// FSA's J2DPane has a byte at 0xB0 that the shared header doesn't name (the
// same byte tu_8044998C.cpp writes).
struct J2DPaneB0 {
    u8 field_0x0[0xB0];
    u8 field_0xb0;
};

static inline u8 getPaneByteB0(J2DPane* pane) {
    return ((J2DPaneB0*)pane)->field_0xb0;
}

static inline void setPaneByteB0(J2DPane* pane, u8 value) {
    ((J2DPaneB0*)pane)->field_0xb0 = value;
}

// Pane tags of four groups of fourteen (8-byte, zero-padded) names: group i
// is 'Pp<i+1>i01'.., 'Ndata0<..>' at [6] and 'Ndata2<i>' at [13]; the
// constructor searches [6] and [13] of each group.
static const u64 lbl_80490940[4][14] = {
    {
        0x0000507031693031ull, 0x0000507031693032ull, 0x0050703169623031ull,
        0x0050703169623032ull, 0x0050703169623033ull, 0x0050703169623034ull,
        0x004E646174613031ull, 0x00000050316F3031ull, 0x00000050316F3032ull,
        0x0000005031743031ull, 0x00005031746B3031ull, 0x0000005031743032ull,
        0x00005031746B3032ull, 0x004E646174613230ull,
    },
    {
        0x0000507032693031ull, 0x0000507032693032ull, 0x0050703269623031ull,
        0x0050703269623032ull, 0x0050703269623033ull, 0x0050703269623034ull,
        0x004E646174613034ull, 0x00000050326F3031ull, 0x00000050326F3032ull,
        0x0000005032743031ull, 0x00005032746B3031ull, 0x0000005032743032ull,
        0x00005032746B3032ull, 0x004E646174613231ull,
    },
    {
        0x0000507033693031ull, 0x0000507033693032ull, 0x0050703369623031ull,
        0x0050703369623032ull, 0x0050703369623033ull, 0x0050703369623034ull,
        0x004E646174613037ull, 0x00000050336F3031ull, 0x00000050336F3032ull,
        0x0000005033743031ull, 0x00005033746B3031ull, 0x0000005033743032ull,
        0x00005033746B3032ull, 0x004E646174613232ull,
    },
    {
        0x0000507034693031ull, 0x0000507034693032ull, 0x0050703469623031ull,
        0x0050703469623032ull, 0x0050703469623033ull, 0x0050703469623034ull,
        0x004E646174613130ull, 0x00000050346F3031ull, 0x00000050346F3032ull,
        0x0000005034743031ull, 0x00005034746B3031ull, 0x0000005034743032ull,
        0x00005034746B3032ull, 0x004E646174613233ull,
    },
};

// Pane tags, as u64: 'Ptim01', 'Ptim02', 'Ptmd01', 'Ptmdk01', then 'Pt01',
// 'Ptk01', 'Pt02', 'Ptk02', 'Pt03', 'Ptk03', 'Pt04', 'Ptk04', 'Ndata00'.
static const u64 lbl_80490B00[13] = {
    0x00005074696D3031ull, 0x00005074696D3032ull, 0x000050746D643031ull,
    0x0050746D646B3031ull, 0x0000000050743031ull, 0x00000050746B3031ull,
    0x0000000050743032ull, 0x00000050746B3032ull, 0x0000000050743033ull,
    0x00000050746B3033ull, 0x0000000050743034ull, 0x00000050746B3034ull,
    0x004E646174613030ull,
};

// The 'TIMG' names of the digit pictures (kk_rupyNum_00 to _09).
static const char* lbl_804B1C68[10] = {
    "kk_rupyNum_00.bti", "kk_rupyNum_01.bti", "kk_rupyNum_02.bti",
    "kk_rupyNum_03.bti", "kk_rupyNum_04.bti", "kk_rupyNum_05.bti",
    "kk_rupyNum_06.bti", "kk_rupyNum_07.bti", "kk_rupyNum_08.bti",
    "kk_rupyNum_09.bti",
};

cls_804B1C90::cls_804B1C90() {
    lbl_8053AF18 = this;
    mScreen = new J2DScreen();
    mScreen->setPriority("mg_geldman.blo", 0x1100000, lbl_80529DEC.field_0x10);
    field_0x8 = new cls_804AF86C();
    field_0x8->fn_80402F04(mScreen->search(0x004E646174613030ull /* "Ndata00" */));
    for (s32 i = 0; i < 4; i++) {
        field_0xc[i] = new cls_804AF86C();
        field_0xc[i]->fn_80402F04(mScreen->search(lbl_80490940[i][6]));
        field_0x1c[i] = new cls_804AF86C();
        field_0x1c[i]->fn_80402F04(mScreen->search(lbl_80490940[i][13]));
    }
    for (s32 i = 0; i < 4; i++) {
        field_0x2c[i] = 0;
    }
    field_0x30 = 0;
    field_0x31 = 0;
    field_0x32 = 0;
}

// FIXME: 91.28 %. In the slot loop the original divides field_0x2c[i] by ten
// twice, once for the units (kept in r26 across the first fn_804030C4 call)
// and once for the index; ours shares one division and computes the units
// after that call. Its stack frame is 0x30 with r25 saved (the original 0x20,
// r26 to r31), so the registers of this and i also differ.
void cls_804B1C90::fn_80448940() {
    field_0x8->fn_80402E08(0.0f, lbl_8053AED0->fn_8043D514());
    for (s32 i = 0; i < 4; i++) {
        if (i < 2) {
            field_0xc[i]->fn_80402E08(0.0f, lbl_8053AED0->fn_8043D514());
            field_0x1c[i]->fn_80402E08(0.0f, lbl_8053AED0->fn_8043D514());
        } else {
            field_0xc[i]->fn_80402E08(0.0f, lbl_8053AED0->fn_8043D57C());
            field_0x1c[i]->fn_80402E08(0.0f, lbl_8053AED0->fn_8043D57C());
        }
        const u64* row = lbl_80490940[i];
        u8 bit = 1 << i;
        if (field_0x32 & bit) {
            if (getPaneByteB0(mScreen->search(row[6])) == 0) {
                setPaneByteB0(mScreen->search(row[6]), 1);
                setPaneByteB0(mScreen->search(row[13]), 1);
            }
        } else {
            if (getPaneByteB0(mScreen->search(row[6])) != 0) {
                setPaneByteB0(mScreen->search(row[6]), 0);
                setPaneByteB0(mScreen->search(row[13]), 0);
            }
        }
        if (getPaneByteB0(mScreen->search(row[6])) != 0) {
            if (field_0x2c[i] >= 100) {
                field_0x2c[i] = 99;
            }
            s32 value = field_0x2c[i];
            s32 units = value - (value / 10) * 10;
            const ResTIMG* res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1C68[value / 10]));
            static_cast<J2DPicture*>(mScreen->search(row[9]))->changeTexture(res, 0);
            static_cast<J2DPicture*>(mScreen->search(row[10]))->changeTexture(res, 0);
            res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1C68[units]));
            static_cast<J2DPicture*>(mScreen->search(row[11]))->changeTexture(res, 0);
            static_cast<J2DPicture*>(mScreen->search(row[12]))->changeTexture(res, 0);
        }
    }
    if (field_0x32 & 0x10) {
        if (getPaneByteB0(mScreen->search(lbl_80490B00[12])) == 0) {
            setPaneByteB0(mScreen->search(lbl_80490B00[12]), 1);
        }
    } else {
        if (getPaneByteB0(mScreen->search(lbl_80490B00[12])) != 0) {
            setPaneByteB0(mScreen->search(lbl_80490B00[12]), 0);
        }
    }
    if (getPaneByteB0(mScreen->search(lbl_80490B00[12])) != 0) {
        if (field_0x30 >= 100) {
            field_0x30 = 99;
            field_0x31 = 99;
        }
        if (field_0x31 >= 100) {
            field_0x31 = 99;
        }
        s32 a = field_0x30;
        s32 b = field_0x31;
        s32 aUnits = a - (a / 10) * 10;
        s32 bTens = b / 10;
        const ResTIMG* res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1C68[a / 10]));
        static_cast<J2DPicture*>(mScreen->search(lbl_80490B00[4]))->changeTexture(res, 0);
        static_cast<J2DPicture*>(mScreen->search(lbl_80490B00[5]))->changeTexture(res, 0);
        res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1C68[aUnits]));
        static_cast<J2DPicture*>(mScreen->search(lbl_80490B00[6]))->changeTexture(res, 0);
        static_cast<J2DPicture*>(mScreen->search(lbl_80490B00[7]))->changeTexture(res, 0);
        res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1C68[bTens]));
        static_cast<J2DPicture*>(mScreen->search(lbl_80490B00[8]))->changeTexture(res, 0);
        static_cast<J2DPicture*>(mScreen->search(lbl_80490B00[9]))->changeTexture(res, 0);
        res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1C68[b % 10]));
        static_cast<J2DPicture*>(mScreen->search(lbl_80490B00[10]))->changeTexture(res, 0);
        static_cast<J2DPicture*>(mScreen->search(lbl_80490B00[11]))->changeTexture(res, 0);
    }
    field_0x32 = 0;
}

void cls_804B1C90::fn_80449070() {
    lbl_8053AB10->fn_8013B490();
    mScreen->draw(0.0f, 0.0f, lbl_8053AB10->field_0x10);
}

void cls_804B1C90::fn_804490B8() {
    const ResTIMG* res;
    for (s32 i = 0; i < 4; i++) {
        u8 bit = 1 << i;
        field_0x32 &= ~bit;
        res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1C68[0]));
        static_cast<J2DPicture*>(mScreen->search(lbl_80490940[i][9]))->changeTexture(res, 0);
        static_cast<J2DPicture*>(mScreen->search(lbl_80490940[i][10]))->changeTexture(res, 0);
        static_cast<J2DPicture*>(mScreen->search(lbl_80490940[i][11]))->changeTexture(res, 0);
        static_cast<J2DPicture*>(mScreen->search(lbl_80490940[i][12]))->changeTexture(res, 0);
    }
    field_0x32 &= ~0x10;
    res = static_cast<const ResTIMG*>(fn_804030C4('TIMG', lbl_804B1C68[0]));
    static_cast<J2DPicture*>(mScreen->search(lbl_80490B00[4]))->changeTexture(res, 0);
    static_cast<J2DPicture*>(mScreen->search(lbl_80490B00[5]))->changeTexture(res, 0);
    static_cast<J2DPicture*>(mScreen->search(lbl_80490B00[6]))->changeTexture(res, 0);
    static_cast<J2DPicture*>(mScreen->search(lbl_80490B00[7]))->changeTexture(res, 0);
    static_cast<J2DPicture*>(mScreen->search(lbl_80490B00[8]))->changeTexture(res, 0);
    static_cast<J2DPicture*>(mScreen->search(lbl_80490B00[9]))->changeTexture(res, 0);
    static_cast<J2DPicture*>(mScreen->search(lbl_80490B00[10]))->changeTexture(res, 0);
    static_cast<J2DPicture*>(mScreen->search(lbl_80490B00[11]))->changeTexture(res, 0);
    for (s32 i = 0; i < 4; i++) {
        setPaneByteB0(mScreen->search(lbl_80490940[i][6]), 0);
        setPaneByteB0(mScreen->search(lbl_80490940[i][13]), 0);
    }
    setPaneByteB0(mScreen->search(lbl_80490B00[12]), 0);
}

cls_804B1C90::~cls_804B1C90() {
    delete mScreen;
    mScreen = NULL;
    delete field_0x8;
    field_0x8 = NULL;
    for (s32 i = 0; i < 4; i++) {
        delete field_0xc[i];
        field_0xc[i] = NULL;
        delete field_0x1c[i];
        field_0x1c[i] = NULL;
    }
    lbl_8053AF18 = NULL;
}
