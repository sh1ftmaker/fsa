#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/tu_803B8C74.h"
#include "game/Map.h"
#include "game/Message.h"
#include "game/SpriteParams.h"
#include "game/act/tu_80440BF4.h"
#include "game/tu_803BC6F0.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/J2D/J2DGrafContext.h"
#include "JSystem/J2D/J2DPicture.h"
#include "JSystem/JKernel/JKRArchive.h"
#include "JSystem/JUtility/JUTVideo.h"
#include "dolphin/card.h"
#include "dolphin/os/OSReset.h"
#include "dolphin/os/OSTime.h"
#include <stdio.h>

// .rodata: the pane tag tables (J2DPane::search), in address order; most
// functions address them off one base (lbl_804765D0).
static const u64 lbl_804765D0[3] = {
    'Tf1lv', 'Tf2lv', 'Tf3lv',
};
static const u64 lbl_804765E8[3] = {
    'Tf1fir', 'Tf2fir', 'Tf3fir',
};
static const u64 lbl_80476600[3] = {
    'Tf1date', 'Tf2date', 'Tf3date',
};
static const u64 lbl_80476618[81] = {
    'Pabb10', 'Pabb09', 'Pabb08',
    'Pabb07', 'Pabb06', 'Pabb05',
    'Pabb04', 'Pabb03', 'Pabb02',
    'Pabb01', 'Wbat01', 'Pbatc10',
    'Pbatc09', 'Pbatc08', 'Pbatc07',
    'Pbatc06', 'Pbatc05', 'Pbatc04',
    'Pbatc03', 'Pbatc02', 'Pbatc01',
    'Pqb10', 'Pqb09', 'Pqb08',
    'Pqb07', 'Pqb06', 'Pqb05',
    'Pqb04', 'Pqb03', 'Pqb02',
    'Pqb01', 'Pbatb09', 'Pbatb08',
    'Pbatb07', 'Pbatb06', 'Pbatb05',
    'Pbatb04', 'Pbatb03', 'Pbatb02',
    'Pbatb01', 'Tbat01', 'Wadv01',
    'Padhu03', 'Padhu02', 'Padvl02',
    'Padvl01', 'Padvw72', 'Padvw71',
    'Padvw70', 'Padvw62', 'Padvw61',
    'Padvw60', 'Padvw52', 'Padvw51',
    'Padvw50', 'Padvw42', 'Padvw41',
    'Padvw40', 'Padvw32', 'Padvw31',
    'Padvw30', 'Padvw22', 'Padvw21',
    'Padvw20', 'Padvw12', 'Padvw11',
    'Padvw10', 'Padvw74', 'Padvw73',
    'Padvw64', 'Padvw63', 'Padvw54',
    'Padvw53', 'Padvw44', 'Padvw43',
    'Padvw34', 'Padvw33', 'Padvw24',
    'Padvw23', 'Padvw14', 'Padvw13',
};
static const u64 lbl_804768A0[19] = {
    'Padhu01', 'Padvb09', 'Padvb08',
    'Padvb07', 'Padvb06', 'Padvb05',
    'Padvb04', 'Padvb03', 'Padvb02',
    'Padvb01', 'Tadv01', 'Pbli01',
    'Pbli02', 'Pbli03', 'Pbli04',
    'Pbli11', 'Pbli12', 'Pbli13',
    'Pbli14',
};
// The 'Pdb08'..'Pdb01' and 'Tdb01' panes of mScreen (fn_803CE11C fades
// them out with field_0x196).
static const u64 lbl_80476938[9] = {
    'Pdb08', 'Pdb07', 'Pdb06',
    'Pdb05', 'Pdb04', 'Pdb03',
    'Pdb02', 'Pdb01', 'Tdb01',
};
static const u64 lbl_80476980[10] = {
    'Pqb01', 'Pqb02', 'Pqb03',
    'Pqb04', 'Pqb05', 'Pqb06',
    'Pqb07', 'Pqb08', 'Pqb09',
    'Pqb10',
};
static const u64 lbl_804769D0[7] = {
    'Padvw10', 'Padvw20', 'Padvw30',
    'Padvw40', 'Padvw50', 'Padvw60',
    'Padvw70',
};
static const u64 lbl_80476A08[8] = {
    'Padvw11', 'Padvw21', 'Padvw31',
    'Padvw41', 'Padvw51', 'Padvw61',
    'Padvw71', 'Padvl01',
};
static const u64 lbl_80476A48[8] = {
    'Padvw12', 'Padvw22', 'Padvw32',
    'Padvw42', 'Padvw52', 'Padvw62',
    'Padvw72', 'Padvl02',
};
static const u64 lbl_80476A88[3] = {
    'Pf1tbb07', 'Pf2tbb07', 'Pf3tbb07',
};
static const u64 lbl_80476AA0[3] = {
    'Pf1tbb06', 'Pf2tbb06', 'Pf3tbb06',
};
static const u64 lbl_80476AB8[3] = {
    'Pf1tbb05', 'Pf2tbb05', 'Pf3tbb05',
};
static const u64 lbl_80476AD0[3] = {
    'Pf1tbb04', 'Pf2tbb04', 'Pf3tbb04',
};
static const u64 lbl_80476AE8[3] = {
    'Pf1tbb03', 'Pf2tbb03', 'Pf3tbb03',
};
static const u64 lbl_80476B00[3] = {
    'Pf1tbb02', 'Pf2tbb02', 'Pf3tbb02',
};
static const u64 lbl_80476B18[3] = {
    'Pf1tbb01', 'Pf2tbb01', 'Pf3tbb01',
};
static const u64 lbl_80476B30[3] = {
    'Pf1tb05', 'Pf2tb05', 'Pf3tb05',
};
static const u64 lbl_80476B48[3] = {
    'Pf1tb04', 'Pf2tb04', 'Pf3tb04',
};
static const u64 lbl_80476B60[3] = {
    'Pf1tb03', 'Pf2tb03', 'Pf3tb03',
};
static const u64 lbl_80476B78[3] = {
    'Pf1tb02', 'Pf2tb02', 'Pf3tb02',
};
static const u64 lbl_80476B90[3] = {
    'Pf1tb01', 'Pf2tb01', 'Pf3tb01',
};
static const u64 lbl_80476BA8[3] = {
    'Pf1t01', 'Pf2t01', 'Pf3t01',
};
static const u64 lbl_80476BC0[3] = {
    'Pf1bb05', 'Pf2bb05', 'Pf3bb05',
};
static const u64 lbl_80476BD8[3] = {
    'Pf1bb04', 'Pf2bb04', 'Pf3bb04',
};
static const u64 lbl_80476BF0[3] = {
    'Pf1bb03', 'Pf2bb03', 'Pf3bb03',
};
static const u64 lbl_80476C08[3] = {
    'Pf1bb02', 'Pf2bb02', 'Pf3bb02',
};
static const u64 lbl_80476C20[3] = {
    'Pf1bb01', 'Pf2bb01', 'Pf3bb01',
};
static const u64 lbl_80476C38[3] = {
    'Pf1b05', 'Pf2b05', 'Pf3b05',
};
static const u64 lbl_80476C50[3] = {
    'Pf1b04', 'Pf2b04', 'Pf3b04',
};
static const u64 lbl_80476C68[3] = {
    'Pf1b03', 'Pf2b03', 'Pf3b03',
};
static const u64 lbl_80476C80[3] = {
    'Pf1b02', 'Pf2b02', 'Pf3b02',
};
static const u64 lbl_80476C98[3] = {
    'Pf1b01', 'Pf2b01', 'Pf3b01',
};
static const u64 lbl_80476CB0[4] = {
    'Pstrb04', 'Pcopb04', 'Pdelb04',
    'Pbacb04',
};
static const u64 lbl_80476CD0[3] = {
    'Pf1bb06', 'Pf2bb06', 'Pf3bb06',
};
static const u64 lbl_80476CE8[3] = {
    'Pf1tbb08', 'Pf2tbb08', 'Pf3tbb08',
};
// Message ids (fn_803CE4CC).
static const s32 lbl_80476D00[8] = {
    0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33,
};
static const u64 lbl_80476D20[3] = {
    'Tf1date', 'Tf2date', 'Tf3date',
};
static const u64 lbl_80476D38[3] = {
    'Tf1lv', 'Tf2lv', 'Tf3lv',
};
static const u64 lbl_80476D50[3] = {
    'Tf1fir', 'Tf2fir', 'Tf3fir',
};
static const u64 lbl_80476D68[4] = {
    'Pbacb01', 'Pbacb02', 'Pbacb03',
    'Pbacb04',
};
static const u64 lbl_80476D88[4] = {
    'Pdelb01', 'Pdelb02', 'Pdelb03',
    'Pdelb04',
};
static const u64 lbl_80476DA8[4] = {
    'Pcopb01', 'Pcopb02', 'Pcopb03',
    'Pcopb04',
};
static const u64 lbl_80476DC8[4] = {
    'Pstrb01', 'Pstrb02', 'Pstrb03',
    'Pstrb04',
};
// fn_803BE3D8.
static const s32 lbl_80476DE8[6] = {
    0x199, 0x1E7, 0x17F, 0x201, 0x1CD, 0x1B3,
};

// The centre of mScreen's pane `tag` (as tu_803F0A54's PANE_CENTRE_X / _Y):
// the original searches the pane again for the bounds after the size.
#define PANE_CENTRE_X(dst, screen, tag)                                        \
    do {                                                                   \
        f32 size = (screen)->search(tag)->getWidth();                      \
        dst = size * 0.5f + (screen)->search(tag)->getGlobalBounds().i.x; \
    } while (0)
#define PANE_CENTRE_Y(dst, screen, tag)                                        \
    do {                                                                   \
        f32 size = (screen)->search(tag)->getHeight();                     \
        dst = size * 0.5f + (screen)->search(tag)->getGlobalBounds().i.y; \
    } while (0)

// A point of the 640 x 480 layout on the 360 x 270 sprite layer of the
// current render mode.
static inline f32 toSpriteX(f32 x) {
    return 360.0f * x / (s32)JUTVideo::getManager()->getRenderMode()->fbWidth;
}

static inline f32 toSpriteY(f32 y) {
    return 270.0f * y / (s32)JUTVideo::getManager()->getRenderMode()->efbHeight;
}

// Puts `pos` at the centre of mScreen's pane `tag`, moved by (dx, dy) in the
// layout (fn_803BE3D8's single sprites). The original adds the 0.0f x offset
// at run time (lfs 0 / fadds), which a literal `+ 0.0f` in the caller does
// not give: the offsets are parameters.
static inline void setSpritePos(FileSelect_804AFF60* self, JGeometry::TVec3f* pos, const u64* tag, f32 dx, f32 dy) {
    f32 x;
    PANE_CENTRE_X(x, self->mScreen, *tag);
    pos->x = dx + toSpriteX(x);
    f32 y;
    PANE_CENTRE_Y(y, self->mScreen, *tag);
    pos->y = toSpriteY(dy + y);
    pos->z = 0.0f;
}

// The file list's message 0x117 becomes 0x116 while
// gGameRoot_8053AAF8->field_0x7f is set; messages 0x60 and 0x64 start the
// field_0xb0 countdown (90 frames). The callers reload field_0x84 after the
// text call, so the tests read the member.
inline void FileSelect_804AFF60::setMessage(u32 msg) {
    if (msg == 0x117 && gGameRoot_8053AAF8->field_0x7f != 0) {
        msg = 0x116;
    }
    field_0x84 = msg;
    getMessageText_803832BC(((J2DTextBox*)mScreenHuuin->search('Tft01'))->getStringPtr(), field_0x84, 0, NULL, NULL);
    if (field_0x84 == 0x60 || field_0x84 == 0x64) {
        field_0xb0 = 0x5A;
    }
}

// Run each frame by fn_803CE9EC (index: mState).
static FileSelect_804AFF60::StateFunc lbl_804AFD70[40] = {
    &FileSelect_804AFF60::fn_803CE370, &FileSelect_804AFF60::fn_803CE11C,
    &FileSelect_804AFF60::fn_803CE05C, &FileSelect_804AFF60::fn_803CDEE8,
    &FileSelect_804AFF60::fn_803CD75C, &FileSelect_804AFF60::fn_803CD2DC,
    &FileSelect_804AFF60::fn_803CC760, &FileSelect_804AFF60::fn_803CC62C,
    &FileSelect_804AFF60::fn_803CC53C, &FileSelect_804AFF60::fn_803CC490,
    &FileSelect_804AFF60::fn_803CC3DC, &FileSelect_804AFF60::fn_803CC2BC,
    &FileSelect_804AFF60::fn_803CC17C, &FileSelect_804AFF60::fn_803CC0D8,
    &FileSelect_804AFF60::fn_803CC048, &FileSelect_804AFF60::fn_803CBF0C,
    &FileSelect_804AFF60::fn_803CBEE0, &FileSelect_804AFF60::fn_803CBE38,
    &FileSelect_804AFF60::fn_803CB3DC, &FileSelect_804AFF60::fn_803CABAC,
    &FileSelect_804AFF60::fn_803C9F80, &FileSelect_804AFF60::fn_803C9F14,
    &FileSelect_804AFF60::fn_803C9898, &FileSelect_804AFF60::fn_803C9338,
    &FileSelect_804AFF60::fn_803C92B8, &FileSelect_804AFF60::fn_803C8B80,
    &FileSelect_804AFF60::fn_803C7F7C, &FileSelect_804AFF60::fn_803C7CA0,
    &FileSelect_804AFF60::fn_803C70B8, &FileSelect_804AFF60::fn_803C6798,
    &FileSelect_804AFF60::fn_803C6690, &FileSelect_804AFF60::fn_803C61C4,
    &FileSelect_804AFF60::fn_803C5808, &FileSelect_804AFF60::fn_803C579C,
    &FileSelect_804AFF60::fn_803C5190, &FileSelect_804AFF60::fn_803C4628,
    &FileSelect_804AFF60::fn_803C4330, &FileSelect_804AFF60::fn_803C4054,
    &FileSelect_804AFF60::fn_803C3904, &FileSelect_804AFF60::fn_803C2E24,
};

// Sets the alphas of file row `file`'s lower panes (field_0x38: name, level,
// date and frame) to their share of the row's fade.
static inline void setRowAlphas(FileSelect_804AFF60* self, s32 file) {
    self->field_0x38[file]->setAlpha_804023D0(self->mScreenHuuin->search(lbl_80476D50[file]), self->field_0x118[13]);
    self->field_0x38[file]->setAlpha_804023D0(self->mScreenHuuin->search(lbl_80476D38[file]), self->field_0x118[14]);
    self->field_0x38[file]->setAlpha_804023D0(self->mScreenHuuin->search(lbl_80476D20[file]), self->field_0x118[15]);
    self->field_0x38[file]->setAlpha_804023D0(self->mScreenHuuin->search(lbl_80476C98[file]), self->field_0x118[16]);
    self->field_0x38[file]->setAlpha_804023D0(self->mScreenHuuin->search(lbl_80476C80[file]), self->field_0x118[17]);
    self->field_0x38[file]->setAlpha_804023D0(self->mScreenHuuin->search(lbl_80476C68[file]), self->field_0x118[18]);
    self->field_0x38[file]->setAlpha_804023D0(self->mScreenHuuin->search(lbl_80476C50[file]), self->field_0x118[19]);
    self->field_0x38[file]->setAlpha_804023D0(self->mScreenHuuin->search(lbl_80476C38[file]), self->field_0x118[20]);
    self->field_0x38[file]->setAlpha_804023D0(self->mScreenHuuin->search(lbl_80476C20[file]), self->field_0x118[21]);
    self->field_0x38[file]->setAlpha_804023D0(self->mScreenHuuin->search(lbl_80476C08[file]), self->field_0x118[22]);
    self->field_0x38[file]->setAlpha_804023D0(self->mScreenHuuin->search(lbl_80476BF0[file]), self->field_0x118[23]);
    self->field_0x38[file]->setAlpha_804023D0(self->mScreenHuuin->search(lbl_80476BD8[file]), self->field_0x118[24]);
    self->field_0x38[file]->setAlpha_804023D0(self->mScreenHuuin->search(lbl_80476BC0[file]), self->field_0x118[25]);
}

// The same for the row's upper panes (field_0x44: the tab).
static inline void setTabAlphas(FileSelect_804AFF60* self, s32 file) {
    self->field_0x44[file]->setAlpha_804023D0(self->mScreenHuuin->search(lbl_80476BA8[file]), self->field_0x118[0]);
    self->field_0x44[file]->setAlpha_804023D0(self->mScreenHuuin->search(lbl_80476B90[file]), self->field_0x118[1]);
    self->field_0x44[file]->setAlpha_804023D0(self->mScreenHuuin->search(lbl_80476B78[file]), self->field_0x118[2]);
    self->field_0x44[file]->setAlpha_804023D0(self->mScreenHuuin->search(lbl_80476B60[file]), self->field_0x118[3]);
    self->field_0x44[file]->setAlpha_804023D0(self->mScreenHuuin->search(lbl_80476B48[file]), self->field_0x118[4]);
    self->field_0x44[file]->setAlpha_804023D0(self->mScreenHuuin->search(lbl_80476B30[file]), self->field_0x118[5]);
    self->field_0x44[file]->setAlpha_804023D0(self->mScreenHuuin->search(lbl_80476B18[file]), self->field_0x118[6]);
    self->field_0x44[file]->setAlpha_804023D0(self->mScreenHuuin->search(lbl_80476B00[file]), self->field_0x118[7]);
    self->field_0x44[file]->setAlpha_804023D0(self->mScreenHuuin->search(lbl_80476AE8[file]), self->field_0x118[8]);
    self->field_0x44[file]->setAlpha_804023D0(self->mScreenHuuin->search(lbl_80476AD0[file]), self->field_0x118[9]);
    self->field_0x44[file]->setAlpha_804023D0(self->mScreenHuuin->search(lbl_80476AB8[file]), self->field_0x118[10]);
    self->field_0x44[file]->setAlpha_804023D0(self->mScreenHuuin->search(lbl_80476AA0[file]), self->field_0x118[11]);
    self->field_0x44[file]->setAlpha_804023D0(self->mScreenHuuin->search(lbl_80476A88[file]), self->field_0x118[12]);
}

// FIXME: 99.89 %. regalloc + reloc-name: the font pointer (0x154) loaded for
// each text pane lands in r26/r27 where the original uses r29; the string
// offsets differ by 0x58 (the strings of fn_803BE3D8 and fn_803CE4CC,
// unwritten, come first); .sdata2 @NNN.
FileSelect_804AFF60::FileSelect_804AFF60() {
    lbl_80529DEC.mHeap->getTotalFreeSize();
    gGameRoot_8053AAF8->field_0xff = 0;
    for (int i = 0; i < 3; i++) {
        fn_803B950C(&lbl_8050EC80.mFiles[i], &lbl_8050EC80.mBodies[i]);
    }
    lbl_80529DEC.fn_80403680();
    if (!lbl_80529DEC.field_0x242) {
        lbl_80529DEC.field_0x242 = 1;
        gGameRoot2_8053AB10->field_0x20->fn_802FF798();
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SCH ', "gb_ch_obj_base_sch.szs");
        gGameRoot2_8053AB10->field_0x20->fn_802F9714(res, 0, 0, 0, 0);
        res = gGameRoot_8053AAF8->field_0xa4->getResource('SCH ', "gb_ch_obj_tr_schE.szs");
        gGameRoot2_8053AB10->field_0x20->fn_802F9714(res, 0, 0, 0x7000, 0);
        res = gGameRoot_8053AAF8->field_0xa4->getResource('SCL ', "cu_cl_obj_base_scl.szs");
        gGameRoot2_8053AB10->field_0x20->fn_802F93F4(res, 0, 0, 0, 0, false);
        res = gGameRoot_8053AAF8->field_0xa4->getResource('SCH ', "player_sch.szs");
        gGameRoot2_8053AB10->field_0x20->fn_802F9714(res, 0, 2, 0x4000, 0);
        res = gGameRoot_8053AAF8->field_0xa4->getResource('SCL ', "cu_cl_obj_base_scl.szs");
        gGameRoot2_8053AB10->field_0x20->fn_802F93F4(res, 0, 2, 0, 0, false);
        res = gGameRoot_8053AAF8->field_0xa4->getResource('SCH ', "gals_sch.szs");
        gGameRoot2_8053AB10->field_0x20->fn_802F9714(res, 0, 3, 0, 0);
        res = gGameRoot_8053AAF8->field_0xa4->getResource('SCL ', "cu_cl_obj_base_scl.szs");
        gGameRoot2_8053AB10->field_0x20->fn_802F93F4(res, 0, 3, 0, 0, false);
        gGameRoot2_8053AB10->field_0x20->field_0x2364 = 1;
    }

    lbl_8053ADB0 = this;
    field_0x80 = 0;
    field_0x7c = 0;
    field_0x84 = 0;
    field_0xa4 = lbl_80529DEC.field_0x23a[1];
    field_0xa0 = 0;
    field_0xa8 = 0;
    field_0x113 = 0;
    field_0xac = 0;
    field_0x111 = 1;
    mPrevState = 1;
    mState = 1;
    field_0x114 = 0;
    field_0x5c = lbl_8050EC80.mFiles;
    field_0x60 = lbl_8050EC80.mBodies;
    field_0x115 = 0;
    field_0xb0 = -1;
    field_0xdc = 0;
    field_0x116 = 0;
    field_0xb4 = 0;
    field_0x117 = 0;
    field_0xda = 0;
    field_0x1a4 = 0;
    field_0x1a5 = 0;
    field_0x1a6 = 0;
    for (s32 i = 0; i < 8; i++) {
        field_0xb8[i] = fn_803B8CAC(0, i);
    }

    mScreen = new J2DScreen;
    mScreen->setPriority("file_select.blo", 0x1100000, lbl_80529DEC.field_0x30);
    mScreenHuuin = new J2DScreen;
    mScreenHuuin->setPriority("file_select_huuin.blo", 0x1100000, lbl_80529DEC.field_0x30);
    mScreenHuuin->search('Paccur02')->setAlpha(0x9C);
    mScreenHuuin->search('Pfcur05')->setAlpha(0x9C);

    const ResTIMG* img = static_cast<const ResTIMG*>(fn_804030C4('TIMG', "kk_black.bti"));
    field_0x10 = new J2DPicture(img);
    field_0x10->setBlackWhite(JUtility::TColor(0x000000FF), JUtility::TColor(0x00000000));

    field_0x14 = new PaneCtrl_804AF86C;
    field_0x14->setPane_80402F04(mScreen->search('Ndata04'));
    field_0x18 = new PaneCtrl_804AF86C;
    field_0x18->setPane_80402F04(mScreenHuuin->search('Ndata01'));
    field_0x1c = new PaneCtrl_804AF86C;
    field_0x1c->setPane_80402F04(mScreenHuuin->search('Ndatab01'));
    field_0x20 = new PaneCtrl_804AF86C;
    field_0x20->setPane_80402F04(mScreen->search('Ndata06'));
    for (s32 i = 0; i < 4; i++) {
        field_0x24[i] = new PaneCtrl_804AF86C;
        field_0x24[i]->setPane_80402F04(mScreenHuuin->search(lbl_80476CB0[i]));
    }
    field_0x34 = new PaneCtrl_804AF86C;
    field_0x34->setPane_80402F04(mScreenHuuin->search('Paccur02'));
    for (s32 i = 0; i < 3; i++) {
        field_0x38[i] = new PaneCtrl_804AF86C;
        field_0x38[i]->setPane_80402F04(mScreenHuuin->search(lbl_80476CD0[i]));
        field_0x44[i] = new PaneCtrl_804AF86C;
        field_0x44[i]->setPane_80402F04(mScreenHuuin->search(lbl_80476CE8[i]));
    }
    field_0x50 = new PaneCtrl_804AF86C;
    field_0x50->setPane_80402F04(mScreenHuuin->search('Pfcur05'));
    field_0x54 = new PaneCtrl_804AF86C;
    field_0x54->setPane_80402F04(mScreenHuuin->search('Pftbbk03'));
    field_0x58 = lbl_8053AED8->field_0x8;
    lbl_8053AED8->fn_80440D78(static_cast<const ResTIMG*>(fn_804030C4('TIMG', "kk_kettei_01.bti")));
    lbl_8053AED8->fn_80440CE4(static_cast<const ResTIMG*>(fn_804030C4('TIMG', "kk_modoru_01.bti")));

    if (gGameRoot_8053AAF8->field_0x7f) {
        mScreenHuuin->search('Pbli04')->hide();
    } else {
        mScreenHuuin->search('Pbli14')->hide();
    }

    JUTResFont* font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreen->search('Tdb01'))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreen->search('Tbat01'))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreenHuuin->search('Tadv01'))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreenHuuin->search('Tbac01'))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreenHuuin->search('Tdel01'))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreenHuuin->search('Tcop01'))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreenHuuin->search('Tstr01'))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreenHuuin->search('Tf3date'))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreenHuuin->search('Tf3lv'))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreenHuuin->search('Tf3fir'))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreenHuuin->search('Tf2date'))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreenHuuin->search('Tf2lv'))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreenHuuin->search('Tf2fir'))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreenHuuin->search('Tf1date'))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreenHuuin->search('Tf1lv'))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreenHuuin->search('Tf1fir'))->setFont(font);
    font = lbl_8053AD78->mFont;
    ((J2DTextBox*)mScreenHuuin->search('Tft01'))->setFont(font);
    // The colours of the file 1 pictures stand for all three files.
    field_0xe4 = ((J2DPicture*)mScreenHuuin->search('Pcopb01'))->getBlack();
    field_0xe0 = ((J2DPicture*)mScreenHuuin->search('Pcopb01'))->getWhite();
    field_0xfc[0] = ((J2DPicture*)mScreenHuuin->search('Pf1t01'))->getBlack();
    field_0xe8[0] = ((J2DPicture*)mScreenHuuin->search('Pf1t01'))->getWhite();
    field_0xfc[1] = ((J2DPicture*)mScreenHuuin->search('Pf1tb01'))->getBlack();
    field_0xe8[1] = ((J2DPicture*)mScreenHuuin->search('Pf1tb01'))->getWhite();
    field_0xfc[2] = ((J2DPicture*)mScreenHuuin->search('Pf1tbb01'))->getBlack();
    field_0xe8[2] = ((J2DPicture*)mScreenHuuin->search('Pf1tbb01'))->getWhite();
    field_0xfc[3] = ((J2DPicture*)mScreenHuuin->search('Pf1b01'))->getBlack();
    field_0xe8[3] = ((J2DPicture*)mScreenHuuin->search('Pf1b01'))->getWhite();
    field_0xfc[4] = ((J2DPicture*)mScreenHuuin->search('Pf1bb01'))->getBlack();
    field_0xe8[4] = ((J2DPicture*)mScreenHuuin->search('Pf1bb01'))->getWhite();
    // Read and dropped.
    ((J2DPicture*)mScreenHuuin->search('Pf1b01'))->getBlack();
    ((J2DPicture*)mScreenHuuin->search('Pf1b01'))->getWhite();
    ((J2DPicture*)mScreenHuuin->search('Pf1b05'))->getBlack();
    ((J2DPicture*)mScreenHuuin->search('Pf1b05'))->getWhite();
    field_0x118[0] = mScreenHuuin->search('Pf1t01')->getAlpha();
    field_0x118[1] = mScreenHuuin->search('Pf1tb01')->getAlpha();
    field_0x118[2] = mScreenHuuin->search('Pf1tb02')->getAlpha();
    field_0x118[3] = mScreenHuuin->search('Pf1tb03')->getAlpha();
    field_0x118[4] = mScreenHuuin->search('Pf1tb04')->getAlpha();
    field_0x118[5] = mScreenHuuin->search('Pf1tb05')->getAlpha();
    field_0x118[6] = mScreenHuuin->search('Pf1tbb01')->getAlpha();
    field_0x118[7] = mScreenHuuin->search('Pf1tbb02')->getAlpha();
    field_0x118[8] = mScreenHuuin->search('Pf1tbb03')->getAlpha();
    field_0x118[9] = mScreenHuuin->search('Pf1tbb04')->getAlpha();
    field_0x118[10] = mScreenHuuin->search('Pf1tbb05')->getAlpha();
    field_0x118[11] = mScreenHuuin->search('Pf1tbb06')->getAlpha();
    field_0x118[12] = mScreenHuuin->search('Pf1tbb07')->getAlpha();
    field_0x118[13] = mScreenHuuin->search('Tf1fir')->getAlpha();
    field_0x118[14] = mScreenHuuin->search('Tf1lv')->getAlpha();
    field_0x118[15] = mScreenHuuin->search('Tf1date')->getAlpha();
    field_0x118[16] = mScreenHuuin->search('Pf1b01')->getAlpha();
    field_0x118[17] = mScreenHuuin->search('Pf1b02')->getAlpha();
    field_0x118[18] = mScreenHuuin->search('Pf1b03')->getAlpha();
    field_0x118[19] = mScreenHuuin->search('Pf1b04')->getAlpha();
    field_0x118[20] = mScreenHuuin->search('Pf1b05')->getAlpha();
    field_0x118[21] = mScreenHuuin->search('Pf1bb01')->getAlpha();
    field_0x118[22] = mScreenHuuin->search('Pf1bb02')->getAlpha();
    field_0x118[23] = mScreenHuuin->search('Pf1bb03')->getAlpha();
    field_0x118[24] = mScreenHuuin->search('Pf1bb04')->getAlpha();
    field_0x118[25] = mScreenHuuin->search('Pf1bb05')->getAlpha();
    for (s32 i = 0; i < 81; i++) {
        field_0x132[i] = mScreen->search(lbl_80476618[i])->getAlpha();
    }
    for (s32 i = 0; i < 19; i++) {
        field_0x183[i] = mScreenHuuin->search(lbl_804768A0[i])->getAlpha();
    }
    for (s32 i = 0; i < 9; i++) {
        field_0x196[i] = mScreen->search(lbl_80476938[i])->getAlpha();
    }
    field_0x19f[0] = mScreenHuuin->search('Pbacb01')->getAlpha();
    field_0x19f[1] = mScreenHuuin->search('Pbacb02')->getAlpha();
    field_0x19f[2] = mScreenHuuin->search('Pbacb03')->getAlpha();
    field_0x19f[3] = mScreenHuuin->search('Pbacb04')->getAlpha();
    field_0x1a3 = 0;

    getMessageText_803832BC(((J2DTextBox*)mScreenHuuin->search('Tadv01'))->getStringPtr(), 0xA4, 0, NULL, NULL);
    getMessageText_803832BC(((J2DTextBox*)mScreen->search('Tbat01'))->getStringPtr(), 0xA5, 0, NULL, NULL);
    getMessageText_803832BC(((J2DTextBox*)mScreenHuuin->search('Tstr01'))->getStringPtr(), 0x118, 0, NULL, NULL);
    getMessageText_803832BC(((J2DTextBox*)mScreenHuuin->search('Tcop01'))->getStringPtr(), 0x119, 0, NULL, NULL);
    getMessageText_803832BC(((J2DTextBox*)mScreenHuuin->search('Tdel01'))->getStringPtr(), 0x11A, 0, NULL, NULL);
    getMessageText_803832BC(((J2DTextBox*)mScreenHuuin->search('Tbac01'))->getStringPtr(), 0x11B, 0, NULL, NULL);

    for (s32 i = 0; i < 3; i++) {
        field_0x70[i] = 0.0f;
        field_0x64[i] = 0.0f;
    }
    mScreen->search('Ndata06')->hide();
    mScreen->search(lbl_80476980[0])->hide();
    mScreen->search(lbl_80476980[1])->hide();
    mScreen->search('Padvw10')->hide();
    mScreen->search('Padvw11')->hide();
    mScreen->search('Padvw12')->hide();
    mScreen->search('Padvw13')->hide();
    mScreen->search('Padvw14')->hide();
    mScreen->search('Padvw20')->hide();
    mScreen->search('Padvw21')->hide();
    mScreen->search('Padvw22')->hide();
    mScreen->search('Padvw23')->hide();
    mScreen->search('Padvw24')->hide();
    mScreen->search('Padvw30')->hide();
    mScreen->search('Padvw31')->hide();
    mScreen->search('Padvw32')->hide();
    mScreen->search('Padvw33')->hide();
    mScreen->search('Padvw34')->hide();
    mScreen->search('Padvw40')->hide();
    mScreen->search('Padvw41')->hide();
    mScreen->search('Padvw42')->hide();
    mScreen->search('Padvw43')->hide();
    mScreen->search('Padvw44')->hide();
    mScreen->search('Padvw50')->hide();
    mScreen->search('Padvw51')->hide();
    mScreen->search('Padvw52')->hide();
    mScreen->search('Padvw53')->hide();
    mScreen->search('Padvw54')->hide();
    mScreen->search('Padvw60')->hide();
    mScreen->search('Padvw61')->hide();
    mScreen->search('Padvw62')->hide();
    mScreen->search('Padvw63')->hide();
    mScreen->search('Padvw64')->hide();
    mScreen->search('Padvw70')->hide();
    mScreen->search('Padvw71')->hide();
    mScreen->search('Padvw72')->hide();
    mScreen->search('Padvw73')->hide();
    mScreen->search('Padvw74')->hide();
    mScreen->search('Padvl02')->hide();
    mScreen->search('Padvl01')->hide();
    mScreenHuuin->search('Padhu01')->hide();
    mScreen->search('Padhu02')->hide();
    mScreen->search('Padhu03')->hide();
    field_0x88[0] = mScreenHuuin->search('Padhu01')->getBounds()->i.x;
    field_0x88[1] = mScreen->search('Padhu02')->getBounds()->i.x;
    field_0x88[2] = mScreen->search('Padhu03')->getBounds()->i.x;
    field_0x94[0] = mScreenHuuin->search('Padhu01')->getBounds()->i.y;
    field_0x94[1] = mScreen->search('Padhu02')->getBounds()->i.y;
    field_0x94[2] = mScreen->search('Padhu03')->getBounds()->i.y;
    mScreenHuuin->search('Padhu01')->setAlpha(0xFF);
    field_0xd8 = 0;
    field_0xda = 0;

    if (gAudioMgr_8053AF80->field_0x58 != 0x100001A) {
        gAudioMgr_8053AF80->fn_80458FF0(0x100001A, false, false);
        gAudioMgr_8053AF80->fn_80458408(0x100001A, false);
        lbl_80529DEC.field_0x248 = 0;
    } else if (lbl_80529DEC.field_0x248) {
        gAudioMgr_8053AF80->fn_80458408(0x100001A, false);
        lbl_80529DEC.field_0x248 = 0;
    }
}

FileSelect_804AFF60::~FileSelect_804AFF60() {
    lbl_80529DEC.mHeap->getTotalFreeSize();
    delete mScreen;
    delete mScreenHuuin;
    delete field_0x10;
    field_0x10 = NULL;
    delete field_0x14;
    field_0x14 = NULL;
    delete field_0x18;
    field_0x18 = NULL;
    delete field_0x1c;
    field_0x1c = NULL;
    delete field_0x20;
    field_0x20 = NULL;
    for (s32 i = 0; i < 4; i++) {
        delete field_0x24[i];
        field_0x24[i] = NULL;
    }
    delete field_0x34;
    field_0x34 = NULL;
    for (s32 i = 0; i < 3; i++) {
        delete field_0x38[i];
        field_0x38[i] = NULL;
        delete field_0x44[i];
        field_0x44[i] = NULL;
    }
    delete field_0x50;
    field_0x50 = NULL;
    delete field_0x54;
    field_0x54 = NULL;
    lbl_80529DEC.field_0x30->unmount();
    lbl_8053ADB0 = NULL;
}

// FIXME: 99.93 %. Instructions identical; only the .sdata2 literals differ
// (@NNN vs lbl_: the pool starts 0, -200, 200, which no earlier written
// function produces in that order).
// Resets the screen to its starting state (the constructor's settings, the
// title message, all rows hidden behind their start positions).
void FileSelect_804AFF60::fn_803CF504() {
    field_0x1a8 = 3;
    lbl_8053AEC0->fn_804310CC();
    field_0x80 = 0;
    field_0x7c = 0;
    field_0x84 = 0x117;
    field_0xa4 = lbl_80529DEC.field_0x23a[1];
    field_0xa0 = 0;
    field_0xa8 = 0;
    field_0x113 = 0;
    field_0xac = 0;
    field_0x111 = 1;
    mPrevState = 1;
    mState = 1;
    field_0x114 = 0;
    field_0x115 = 0;
    field_0xb0 = -1;
    field_0xdc = 0;
    field_0x116 = 0;
    field_0xb4 = 0;
    field_0x117 = 0;
    setMessage(field_0x84);
    for (s32 i = 0; i < 8; i++) {
        field_0xb8[i] = fn_803B8CAC(0, i);
    }
    field_0x14->mPane->hide();
    field_0x18->mPane->hide();
    field_0x1c->mPane->hide();
    field_0x20->mPane->hide();
    field_0x34->mPane->hide();
    for (s32 i = 0; i < 4; i++) {
        field_0x24[i]->mPane->hide();
    }
    field_0x50->mPane->show();
    for (s32 i = 0; i < 3; i++) {
        field_0x38[i]->mPane->show();
        field_0x44[i]->mPane->show();
        mScreenHuuin->search(lbl_804765D0[i])->hide();
        mScreenHuuin->search(lbl_804765E8[i])->hide();
        mScreenHuuin->search(lbl_80476600[i])->hide();
    }

    // The original inlines this block into both fn_803C24BC and fn_803CF504; no out-of-line copy exists.
    field_0x50->moveFromCentre_80402E08(-300.0f, 0.0f);
    field_0x50->field_0x20 = 0;
    field_0x50->mPane->setAlpha(0);
    field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur01'), field_0x50->field_0x28);
    field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur02'), field_0x50->field_0x28);
    field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur03'), field_0x50->field_0x28);
    field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur04'), field_0x50->field_0x28);
    field_0x50->field_0x26 = 0;

    for (s32 i = 0; i < 3; i++) {
        field_0x38[i]->moveFromCentre_80402E08(0.0f, 300.0f);
        field_0x38[i]->field_0x22 = 0;
        field_0x38[i]->mPane->setAlpha(0);
        setRowAlphas(this, i);
        field_0x38[i]->field_0x26 = 0;

        field_0x44[i]->moveFromCentre_80402E08(-300.0f, 0.0f);
        field_0x44[i]->field_0x20 = 0;
        field_0x44[i]->mPane->setAlpha(0);
        setTabAlphas(this, i);
        field_0x44[i]->field_0x26 = 0;
    }

    field_0x54->moveFromCentre_80402E08(0.0f, -200.0f);
    field_0x54->field_0x22 = 0;
    field_0x54->mPane->setAlpha(0);
    field_0x54->field_0x26 = 0;
    field_0xde = 0;
    field_0x1a4 = 0;
    field_0x1a5 = 0;
    field_0x1a6 = 0;
    field_0x58->moveFromCentre_80402E08(0.0f, -200.0f);
    field_0x58->mPane->setAlpha(0);
    field_0xd8 = 0;
    field_0xda = 0;
}

// FIXME: 99.94 %. Instructions identical; only the .sdata2 literals are @NNN
// (pool order, see fn_803C24BC).
// Draws the screen: moves the 'Padhu' highlight by the offset of the first
// empty 'Pdb' slot (field_0xb8) from 'Pdb05', draws both screens, shows the
// highlight only while the chosen file's row (field_0xa8) is up, flagged and
// has all seven fn_803B8CAC bits, then the message window and kk_black fade.
void FileSelect_804AFF60::fn_803CEED4() {
    for (int i = 0; i < 8; i++) {
        if (field_0xb8[i] == 0) {
            field_0xb4 = i;
            break;
        }
    }
    if (field_0xb4 < 7) {
        f32 dx = mScreen->search(lbl_804769D0[field_0xb4])->getBounds()->i.x
               - mScreen->search(lbl_804769D0[4])->getBounds()->i.x;
        f32 dy = mScreen->search(lbl_804769D0[field_0xb4])->getBounds()->i.y
               - mScreen->search(lbl_804769D0[4])->getBounds()->i.y;
        mScreenHuuin->search('Padhu01')->move(field_0x88[0] + dx, field_0x94[0] + dy);
        mScreen->search('Padhu02')->move(field_0x88[1] + dx, field_0x94[1] + dy);
        mScreen->search('Padhu03')->move(field_0x88[2] + dx, field_0x94[2] + dy);
    }
    gGameRoot2_8053AB10->fn_8013B490();
    mScreen->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
    if (field_0x14->mPane->isVisible() && (lbl_8050EC80.getFile(field_0xa8).mFlags & 1)) {
        // Seven separate calls in the original (a loop with a break isn't unrolled).
        if (fn_803B8CAC(field_0xa8, 0) && fn_803B8CAC(field_0xa8, 1) && fn_803B8CAC(field_0xa8, 2)
            && fn_803B8CAC(field_0xa8, 3) && fn_803B8CAC(field_0xa8, 4) && fn_803B8CAC(field_0xa8, 5)
            && fn_803B8CAC(field_0xa8, 6)) {
            if (mScreenHuuin->search('Padhu01')->isVisible()) {
                mScreenHuuin->search('Padhu01')->hide();
                mScreen->search('Padhu02')->hide();
                mScreen->search('Padhu03')->hide();
            }
        } else if (!mScreenHuuin->search('Padhu01')->isVisible()) {
            mScreenHuuin->search('Padhu01')->show();
            mScreen->search('Padhu02')->show();
            mScreen->search('Padhu03')->show();
        }
        fn_803BE3D8();
    } else if (mScreenHuuin->search('Padhu01')->isVisible()) {
        mScreenHuuin->search('Padhu01')->hide();
        mScreen->search('Padhu02')->hide();
        mScreen->search('Padhu03')->hide();
    }
    mScreenHuuin->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
    if (field_0x116 != 0) {
        field_0x10->setAlpha(field_0x1a3);
        field_0x10->draw(-1.0f, -1.0f, 611.0f, 451.0f, false, false, false);
        lbl_8053AEC0->fn_804316EC();
    }
    lbl_8053AED8->fn_80440EE8();
}

// FIXME: 99.86 %. Instructions identical; only the .sdata2 literals differ
// (@NNN vs lbl_: fn_803BC6F0, unwritten, uses 0, -200, 200 and the
// conversion doubles first).
void FileSelect_804AFF60::fn_803CE9EC() {
    field_0x1a8 = 3;
    if (field_0x116 == 1) {
        lbl_8053AEC0->fn_80431174(field_0x114);
    }
    cls_803B9940* card = &lbl_8051C1A0;
    if ((card->field_0x1ff5 == 0 || card->field_0x1ff5 == 1) && !lbl_8053ACA8->fn_802B1DCC() &&
        field_0x113 == 0 && field_0x117 == 0) {
        setState(1);
    }
    if (field_0x116 < 2) {
        (lbl_8053ADB0->*lbl_804AFD70[mState])();
    }
    if (mState != field_0x111) {
        field_0x111 = mState;
        switch (mState) {
        case 0:
        case 2:
        case 5:
        case 10:
        case 14:
        case 19:
        case 24:
        case 25:
        case 36:
            field_0x1a5 = 0;
            break;
        case 3:
        case 7:
        case 11:
        case 12:
        case 15:
        case 27:
        case 37:
            field_0x1a5 = 1;
            break;
        }
    }
    if (field_0x1a5 != field_0x1a6) {
        field_0x1a4 = 2;
    } else if (field_0x1a4 == 2) {
        field_0x1a4 = 1;
    }
    switch (field_0x1a4) {
    case 1: {
        s16 frames = lbl_8050DB0C.field_0x4c[0x8A];
        field_0xde++;
        if (field_0xde < frames) {
            f32 t = (f32)(field_0xde * field_0xde) / (f32)(frames * frames);
            field_0x58->moveFromCentre_80402E08(0.0f, 200.0f * t + -200.0f);
            field_0x58->mPane->setAlpha(t * field_0x58->field_0x28);
        } else {
            field_0xde = frames;
            field_0x58->moveFromCentre_80402E08(0.0f, 0.0f);
            field_0x58->mPane->setAlpha(field_0x58->field_0x28);
            field_0x1a4 = 0;
        }
        break;
    }
    case 2: {
        s16 frames = lbl_8050DB0C.field_0x4c[0x8A];
        field_0xde--;
        if (field_0xde > 0) {
            f32 t = (f32)(field_0xde * field_0xde) / (f32)(frames * frames);
            field_0x58->moveFromCentre_80402E08(0.0f, 200.0f * t + -200.0f);
            field_0x58->mPane->setAlpha(t * field_0x58->field_0x28);
        } else {
            field_0xde = 0;
            field_0x58->moveFromCentre_80402E08(0.0f, -200.0f);
            field_0x58->mPane->setAlpha(0);
            field_0x1a6 = field_0x1a5;
            if (field_0x1a5 == 0) {
                lbl_8053AED8->fn_80440BF4(true);
            } else {
                lbl_8053AED8->fn_80440BF4(false);
            }
            field_0x1a4 = 1;
        }
        break;
    }
    }
    if (field_0x7c != field_0x80) {
        if (field_0x116 == 0) {
            field_0x7c = field_0x80;
            if (field_0x7c != 0) {
                if (field_0x7c == 0x50 || field_0x7c == 0x53 || field_0x7c == 0x57 || field_0x7c == 0x5C ||
                    field_0x7c == 0x84 || field_0x7c == 0x256) {
                    lbl_8053AEC0->fn_80431A9C(field_0x7c, 0, &field_0x114);
                    field_0x1a3 = 0;
                } else {
                    lbl_8053AEC0->fn_80431A9C(field_0x7c, 0, NULL);
                    field_0x1a3 = 0;
                }
                field_0x116 = 2;
            }
        } else if (field_0x116 == 1) {
            field_0x116 = 3;
        }
    }
    if (field_0x116 == 2) {
        if (field_0x1a3 > 0x48) {
            field_0x1a3 = 0x50;
        } else {
            field_0x1a3 += 8;
        }
        if (lbl_8053AEC0->fn_804317AC()) {
            field_0x1a3 = 0x50;
            field_0x116 = 1;
        }
    } else if (field_0x116 == 3) {
        if (field_0x1a3 < 8) {
            field_0x1a3 = 0;
        } else {
            field_0x1a3 -= 8;
        }
        if (lbl_8053AEC0->fn_80431740()) {
            field_0x1a3 = 0;
            field_0x116 = 0;
        }
    }
    card->field_0x1ff5 = 2;
}

// The progress message of save file `file`: the first stage (0-7) it has not
// finished, or the last one's when all are done. fn_803CE4CC inlines it three
// times; the linker dropped its out-of-line copy.
static s32 getProgressMsg(s32 file) {
    int cleared = 0;
    for (int stage = 0; stage < 8; stage++) {
        if (fn_803B8CAC(file, stage)) {
            cleared++;
        } else {
            return lbl_80476D00[cleared];
        }
    }
    return lbl_80476D00[7];
}

// FIXME: 99.01 %. regalloc + reloc-name: the file index and its bit swap
// r24 / r25; `gGameRoot_8053AAF8->field_0xff != 0` is cmplwi where the original
// has cmpwi (as in fn_803CABAC); the original tests the time against the
// zero it materialises `used` from (one li fewer); the date format's
// .rodata offset is 0x38 short until fn_803BE3D8's strings exist.
// Refreshes the three file rows: a file whose checksum fails gets message
// 0x11D and its bit in field_0x115 (fn_803BD5E0); a used file shows the date
// it was chosen last and every file's progress; an empty file (or no card)
// shows message 0x6A.
void FileSelect_804AFF60::fn_803CE4CC() {
    for (int i = 0; i < 3; i++) {
        clearBadChecksum(i);
        if (gGameRoot_8053AAF8->field_0xff != 0) {
            if (fn_803BADAC(&lbl_8050EC80.getFile(i))) {
                BOOL used = FALSE;
                if (lbl_8050EC80.getFile(i).field_0x0 != 0) {
                    used = TRUE;
                }
                if (used) {
                    OSCalendarTime time;
                    OSTicksToCalendarTime(lbl_8050EC80.getFile(i).field_0x0, &time);
                    snprintf(((J2DTextBox*)mScreenHuuin->search(lbl_80476600[i]))->getStringPtr(), 30,
                             "%02d.%02d.%04d %02d:%02d:%02d", time.mon + 1, time.mday, time.year, time.hour,
                             time.min, time.sec);
                    mScreenHuuin->search(lbl_804765D0[i])->show();
                    mScreenHuuin->search(lbl_804765E8[i])->hide();
                    mScreenHuuin->search(lbl_80476600[i])->show();
                    getMessageText_803832BC(((J2DTextBox*)mScreenHuuin->search('Tf1lv'))->getStringPtr(),
                                            getProgressMsg(0), 0, NULL, NULL);
                    getMessageText_803832BC(((J2DTextBox*)mScreenHuuin->search('Tf2lv'))->getStringPtr(),
                                            getProgressMsg(1), 0, NULL, NULL);
                    getMessageText_803832BC(((J2DTextBox*)mScreenHuuin->search('Tf3lv'))->getStringPtr(),
                                            getProgressMsg(2), 0, NULL, NULL);
                } else {
                    getMessageText_803832BC(((J2DTextBox*)mScreenHuuin->search(lbl_804765E8[i]))->getStringPtr(), 0x6A,
                                            0, NULL, NULL);
                    mScreenHuuin->search(lbl_804765D0[i])->hide();
                    mScreenHuuin->search(lbl_804765E8[i])->show();
                    mScreenHuuin->search(lbl_80476600[i])->hide();
                }
                fn_803BDD44(i);
            } else {
                getMessageText_803832BC(((J2DTextBox*)mScreenHuuin->search(lbl_804765E8[i]))->getStringPtr(), 0x11D, 0,
                                        NULL, NULL);
                mScreenHuuin->search(lbl_804765D0[i])->hide();
                mScreenHuuin->search(lbl_804765E8[i])->show();
                mScreenHuuin->search(lbl_80476600[i])->hide();
                setBadChecksum(i);
                fn_803BD5E0(i);
            }
        } else {
            getMessageText_803832BC(((J2DTextBox*)mScreenHuuin->search(lbl_804765E8[i]))->getStringPtr(), 0x6A, 0, NULL,
                                    NULL);
            mScreenHuuin->search(lbl_804765D0[i])->hide();
            mScreenHuuin->search(lbl_804765E8[i])->show();
            mScreenHuuin->search(lbl_80476600[i])->hide();
            fn_803BDD44(i);
        }
    }
}

s32 FileSelect_804AFF60::fn_803CE4B0() {
    if (lbl_8053ADB0 != NULL) {
        return lbl_8053ADB0->field_0x1a8;
    }
    return 3;
}

// FIXME: 99.69. The bounds x and field_0x8 load into f2 / f1 (original f1 / f2)
// and the fsubs operands swap; 600.0f is still @NNN until fn_803BC6F0 is written.
void FileSelect_804AFF60::fn_803CE370() {
    if (field_0xb0 > 0) {
        field_0xb0--;
    }
    if (lbl_8053AD30->fn_80309C4C(0x1300, 1) != 0 || field_0xb0 == 0) {
        field_0xb0 = -1;
        field_0x80 = 0;
        setMessage(0x117);
        if (field_0x20->mPane->getBounds()->i.x - field_0x20->field_0x8 < 600.0f) {
            field_0x20->field_0x20 = 0;
            field_0x20->field_0x26 = 0;
            field_0x117 = 1;
        }
        setState(1);
        gAudioMgr_8053AF80->fn_80458880(0xB, -1);
    }
}

// FIXME: 99.50 %. As in fn_803CE370, the bounds x and field_0x8 load into
// f1 / f0 (original f0 / f1), and the loop's counter and table pointer swap
// r29 / r30; 600.0f is still @NNN.
void FileSelect_804AFF60::fn_803CE11C() {
    u32 cardState = lbl_8051C1A0.fn_803B9940(0);
    if (field_0x20->mPane->getBounds()->i.x - field_0x20->field_0x8 < 600.0f) {
        field_0x20->fn_80402B10(lbl_8050DB0C.field_0x4c[0xC3], 0, 0.0f, 600.0f);
        field_0x20->fn_80402628(lbl_8050DB0C.field_0x4c[0xC3], field_0x20->field_0x28, 0);
        for (s32 i = 0; i < 9; i++) {
            field_0x20->setAlpha_804023D0(mScreen->search(lbl_80476938[i]), field_0x196[i]);
        }
        field_0x117 = 0;
        return;
    }
    switch (cardState) {
    case 0:
        field_0x80 = 0x4C;
        mPrevState = mState;
        field_0x1a4 = 1;
        fn_803C24BC();
        mState = 2;
        lbl_8050EC80.field_0x8 = 0;
        break;
    case 8:
        field_0x80 = 0x4D;
        mPrevState = mState;
        field_0x1a4 = 1;
        mState = 2;
        break;
    case 9:
        field_0x80 = 0x4E;
        mPrevState = mState;
        field_0x1a4 = 1;
        mState = 2;
        break;
    case 10:
        field_0x80 = 0x4F;
        mPrevState = mState;
        field_0x1a4 = 1;
        mState = 2;
        break;
    case 6:
    case 7:
        field_0x80 = 0x50;
        setState(0xB);
        break;
    case 11:
    case 12:
        field_0x80 = 0x51;
        mPrevState = mState;
        field_0x1a4 = 1;
        mState = 2;
        break;
    case 2:
        lbl_8051C1A0.fn_803B9B8C();
        field_0x80 = 0;
        setState(0x11);
        break;
    case 1:
        field_0x80 = 0x57;
        setState(7);
        break;
    case 3:
    case 4:
    case 5:
    default:
        field_0x80 = 0;
        break;
    }
}

void FileSelect_804AFF60::fn_803CE05C() {
    if (lbl_8053AD30->fn_80309C4C(0x1300, 1) != 0) {
        if (field_0x7c == 0x51) {
            field_0x80 = 0x83;
        } else if (field_0x7c == 0x83) {
            field_0x80 = 0x84;
            setState(0xF);
        } else {
            if (gGameRoot_8053AAF8->field_0x7f != 0) {
                field_0x80 = 0x5C;
            } else {
                field_0x80 = 0x256;
            }
            setState(3);
        }
        gAudioMgr_8053AF80->fn_80458880(0xB, -1);
    }
}

// State 3, the "start a new game?" prompt (field_0x114: 0 yes, 1 no): left /
// right move the cursor; A on yes clears the save data and shows the file
// list (fn_803C24BC), A on no goes to state 1.
void FileSelect_804AFF60::fn_803CDEE8() {
    if (lbl_8053AD30->fn_80309C4C(1, 1) != 0) {
        if (field_0x114 != 0) {
            field_0x114--;
            gAudioMgr_8053AF80->fn_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(2, 1) != 0) {
        if (field_0x114 < 1) {
            field_0x114++;
            gAudioMgr_8053AF80->fn_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(0x100, 1) != 0) {
        if (field_0x114 == 0) {
            field_0x80 = 0;
            gGameRoot_8053AAF8->field_0xff = 0;
            for (s32 i = 0; i < 3; i++) {
                fn_803B950C(&lbl_8050EC80.mFiles[i], &lbl_8050EC80.mBodies[i]);
            }
            lbl_80529DEC.fn_80403680();
            fn_803C24BC();
            gAudioMgr_8053AF80->fn_80458880(0xB, -1);
        } else {
            field_0x80 = 0;
            setState(1);
            gAudioMgr_8053AF80->fn_80458880(0xC, -1);
        }
    }
}

// FIXME: 95.09 %. One instruction extra: the original computes
// frames + rowStep * 2 first and spills rowDelay at once (reloading it from
// the stack), ours adds rowDelay to rowStep * 2 first, and keeps frames in r17
// for the cursor's fn_80402B10 (mr r4, r17) where the original's r4 still
// holds it; registers differ from there. Literals are @NNN (see fn_803C24BC).
// State 4: slides the cursor, the three rows (bottom row first) and the
// board in (the timings are lbl_8050DB0C's 0x84..0x8A), starts the 'kk_'
// sprite fade-in, then goes to state 5.
void FileSelect_804AFF60::fn_803CD75C() {
    s16 rowDelay = lbl_8050DB0C.field_0x4c[0x84];
    s16 frames = lbl_8050DB0C.field_0x4c[0x85];
    s32 rowStep = lbl_8050DB0C.field_0x4c[0x86];
    s16 boardDelay = lbl_8050DB0C.field_0x4c[0x87];
    s16 boardFrames = lbl_8050DB0C.field_0x4c[0x88];
    s16 kkDelay = lbl_8050DB0C.field_0x4c[0x89];
    s16 kkFrames = lbl_8050DB0C.field_0x4c[0x8A];
    s32 end = rowDelay + frames;
    if (end < rowDelay + (frames + rowStep * 2)) {
        end = rowDelay + (frames + rowStep * 2);
    }
    if (end < boardDelay + boardFrames) {
        end = boardDelay + boardFrames;
    }
    if (end < kkDelay + kkFrames) {
        end = kkDelay + kkFrames;
    }

    field_0xd8++;
    if (field_0xd8 > rowDelay) {
        field_0x50->fn_80402B10(frames, 0, -300.0f, 0.0f);
    }
    for (s32 i = 0; i < 3; i++) {
        if (field_0xd8 > rowDelay + i * rowStep) {
            field_0x38[2 - i]->fn_80402940(frames, 0, 300.0f, 0.0f);
            field_0x38[2 - i]->fn_80402628(frames, 0, field_0x38[2 - i]->field_0x28);
            setRowAlphas(this, 2 - i);
            field_0x44[i]->fn_80402B10(frames, 0, -300.0f, 0.0f);
            field_0x44[i]->fn_80402628(frames, 0, field_0x44[i]->field_0x28);
            setTabAlphas(this, i);
        }
    }
    if (field_0xd8 > boardDelay) {
        field_0x54->fn_80402940(boardFrames, 0, -200.0f, 0.0f);
        field_0x54->fn_80402628(boardFrames, 0, field_0x54->field_0x28);
    }
    if (field_0xd8 == kkDelay + 1) {
        field_0x1a4 = 1;
    }
    if (field_0xd8 > end) {
        setState(5);
    }
}

// FIXME: 99.83 %. Instructions identical; only the .sdata2 literals differ
// (@NNN vs lbl_, see fn_803C24BC).
// State 5: picking a file. A moves on (to fn_803BF9C4 for a file with save
// data, else fn_803C1734), B leaves the screen, left / right move the cursor
// (field_0xa4) with a little hop of the chosen row.
void FileSelect_804AFF60::fn_803CD2DC() {
    if (lbl_8053AD30->fn_80309C4C(0x1100, 1) != 0) {
        if (hasBadChecksum(field_0xa4)) {
            field_0x114 = 1;
            setMessage(0x5E);
            getMessageText_803832BC(((J2DTextBox*)mScreen->search('Tdb01'))->getStringPtr(), 0x65, 0, NULL, NULL);
            fn_803BF9C4();
        } else {
            field_0xa0 = 0;
            fn_803C1734();
        }
        gAudioMgr_8053AF80->fn_80458880(0xB, -1);
    } else if (lbl_8053AD30->fn_80309C4C(0x200, 1) != 0) {
        if (field_0x116 != 0) {
            lbl_8053AEC0->fn_804310BC();
            field_0x1a3 = 0;
        }
        field_0x1a8 = 2;
        gAudioMgr_8053AF80->fn_80458880(0xC, -1);
    } else if (lbl_8053AD30->fn_80309A9C(4, 1) != 0) {
        gAudioMgr_8053AF80->fn_80458880(0x26, -1);
        field_0xa4++;
        if (field_0xa4 >= 3) {
            field_0xa4 = 0;
        }
        field_0xda = 1;
        field_0x50->field_0x26 = 0x1E;
    } else if (lbl_8053AD30->fn_80309A9C(8, 1) != 0) {
        gAudioMgr_8053AF80->fn_80458880(0x26, -1);
        field_0xa4--;
        if (field_0xa4 < 0) {
            field_0xa4 = 2;
        }
        field_0xda = 1;
        field_0x50->field_0x26 = 0x1E;
    }
    if (mState == 5) {
        f32 hop = 0.0f;
        if (field_0xda != 0) {
            if (field_0xda < 5) {
                hop = 5.0f * (field_0xda * field_0xda) / 25.0f;
                field_0xda++;
            } else if (field_0xda < 10) {
                hop = 5.0f * ((10 - field_0xda) * (10 - field_0xda)) / 25.0f;
                field_0xda++;
            } else {
                field_0xda = 0;
            }
        }
        for (s32 i = 0; i < 3; i++) {
            if (0.0f != hop && i == field_0xa4) {
                field_0x44[i]->moveFromCentre_80402E08(0.0f, hop);
            } else {
                field_0x44[i]->moveFromCentre_80402E08(0.0f, 0.0f);
            }
        }
        field_0x50->moveFromCentre_80402E08(0.0f, field_0x44[field_0xa4]->mPane->getBounds()->i.y - field_0x44[0]->field_0xc);
        field_0x50->fn_8040245C(0x3C, 0, field_0x50->field_0x28, 1);
        field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur01'), field_0x50->field_0x28);
        field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur02'), field_0x50->field_0x28);
        field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur03'), field_0x50->field_0x28);
        field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur04'), field_0x50->field_0x28);
    }
}

void FileSelect_804AFF60::fn_803CC62C() {
    if (lbl_8053AD30->fn_80309C4C(1, 1) != 0) {
        if (field_0x114 != 0) {
            field_0x114--;
            gAudioMgr_8053AF80->fn_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(2, 1) != 0) {
        if (field_0x114 < 1) {
            field_0x114++;
            gAudioMgr_8053AF80->fn_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(0x100, 1) != 0) {
        if (field_0x114 == 0) {
            field_0x80 = 0x59;
            setState(8);
            field_0x113 = 1;
            lbl_80529DEC.field_0x24c = 1;
            gAudioMgr_8053AF80->fn_80458880(0xB, -1);
        } else {
            field_0x80 = 0x58;
            setState(2);
            gAudioMgr_8053AF80->fn_80458880(0xC, -1);
        }
    }
}

// State 8: copies the three files into field_0x5c / field_0x60, gives each
// its checksum, hands both to the memory card manager to write (state 9 waits
// for it) and keeps the card's serial number.
void FileSelect_804AFF60::fn_803CC53C() {
    lbl_8050EC80.fn_803B93E8(field_0x5c, field_0x60, 0);
    fn_803BACF8(field_0x5c, 0);
    lbl_8050EC80.fn_803B93E8(field_0x5c, field_0x60, 1);
    fn_803BACF8(field_0x5c, 1);
    lbl_8050EC80.fn_803B93E8(field_0x5c, field_0x60, 2);
    fn_803BACF8(field_0x5c, 2);
    lbl_8051C1A0.fn_803B9D54(field_0x5c, 0x258, field_0x60, 0x1D4C, 0);
    setState(9);
    u64 serial;
    CARDGetSerialNo(lbl_8051C1A0.field_0x1ff4, &serial);
    lbl_8051C1A0.field_0x1ff8 = serial;
}

void FileSelect_804AFF60::fn_803CC490() {
    s32 result = lbl_8051C1A0.fn_803B9BDC();
    if (result == 1) {
        field_0x80 = 0x5B;
        setState(0xA);
        field_0x113 = 0;
        lbl_80529DEC.field_0x24c = 0;
        field_0xb0 = 0x5A;
    } else if (result == 2) {
        field_0x80 = 0x5A;
        fn_803CE4CC();
        setState(0);
        field_0x113 = 0;
        lbl_80529DEC.field_0x24c = 0;
    }
}

void FileSelect_804AFF60::fn_803CC3DC() {
    if (field_0xb0 > 0) {
        field_0xb0--;
    }
    if (lbl_8053AD30->fn_80309C4C(0x1300, 1) != 0 || field_0xb0 == 0) {
        field_0xb0 = -1;
        field_0x80 = 0;
        gGameRoot_8053AAF8->field_0xff = 1;
        fn_803C24BC();
        gAudioMgr_8053AF80->fn_80458880(0xB, -1);
        lbl_8050EC80.field_0x8 = lbl_8051C1A0.field_0x1ff8;
    }
}

void FileSelect_804AFF60::fn_803CC2BC() {
    if (lbl_8053AD30->fn_80309C4C(1, 1) != 0) {
        if (field_0x114 != 0) {
            field_0x114--;
            gAudioMgr_8053AF80->fn_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(2, 1) != 0) {
        if (field_0x114 < 1) {
            field_0x114++;
            gAudioMgr_8053AF80->fn_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(0x100, 1) != 0) {
        if (field_0x114 == 0) {
            field_0x80 = 0x53;
            setState(0xC);
            gAudioMgr_8053AF80->fn_80458880(0xB, -1);
        } else {
            field_0x80 = 0x52;
            setState(2);
            gAudioMgr_8053AF80->fn_80458880(0xC, -1);
        }
    }
}

void FileSelect_804AFF60::fn_803CC17C() {
    if (lbl_8053AD30->fn_80309C4C(1, 1) != 0) {
        if (field_0x114 != 0) {
            field_0x114--;
            gAudioMgr_8053AF80->fn_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(2, 1) != 0) {
        if (field_0x114 < 1) {
            field_0x114++;
            gAudioMgr_8053AF80->fn_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(0x100, 1) != 0) {
        if (field_0x114 == 0) {
            lbl_8051C1A0.fn_803B9DDC();
            field_0x80 = 0x54;
            setState(0xD);
            field_0x113 = 1;
            lbl_80529DEC.field_0x24c = 1;
            gAudioMgr_8053AF80->fn_80458880(0xB, -1);
        } else {
            field_0x80 = 0x52;
            setState(2);
            gAudioMgr_8053AF80->fn_80458880(0xC, -1);
        }
    }
}

void FileSelect_804AFF60::fn_803CC0D8() {
    s32 result = lbl_8051C1A0.fn_803B9C5C();
    if (result == 1) {
        field_0x80 = 0x55;
        setState(0xE);
        field_0x113 = 0;
        lbl_80529DEC.field_0x24c = 0;
        field_0xb0 = 0x5A;
    } else if (result == 2) {
        field_0x80 = 0x56;
        setState(0);
        field_0x113 = 0;
        lbl_80529DEC.field_0x24c = 0;
    }
}

void FileSelect_804AFF60::fn_803CC048() {
    if (field_0xb0 > 0) {
        field_0xb0--;
    }
    if (lbl_8053AD30->fn_80309C4C(0x1300, 1) != 0 || field_0xb0 == 0) {
        field_0xb0 = -1;
        field_0x80 = 0;
        setState(1);
        gAudioMgr_8053AF80->fn_80458880(0xB, -1);
    }
}

void FileSelect_804AFF60::fn_803CBF0C() {
    if (lbl_8053AD30->fn_80309C4C(1, 1) != 0) {
        if (field_0x114 != 0) {
            field_0x114--;
            gAudioMgr_8053AF80->fn_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(2, 1) != 0) {
        if (field_0x114 < 1) {
            field_0x114++;
            gAudioMgr_8053AF80->fn_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(0x100, 1) != 0) {
        if (field_0x114 == 0) {
            field_0x80 = 0;
            setState(0x10);
            gAudioMgr_8053AF80->fn_80458880(0xB, -1);
        } else {
            if (gGameRoot_8053AAF8->field_0x7f != 0) {
                field_0x80 = 0x5C;
            } else {
                field_0x80 = 0x256;
            }
            setState(3);
            gAudioMgr_8053AF80->fn_80458880(0xC, -1);
        }
    }
}

void FileSelect_804AFF60::fn_803CBEE0() {
    OSResetSystem(1, 1, 1);
}

void FileSelect_804AFF60::fn_803CBE38() {
    s32 result = lbl_8051C1A0.fn_803B9A94(field_0x5c, 0x258, field_0x60, 0x1D4C, 0);
    if (result != 0) {
        if (result == 1) {
            gGameRoot_8053AAF8->field_0xff = 1;
            fn_803C24BC();
            lbl_8050EC80.field_0x8 = lbl_8051C1A0.field_0x1ff8;
        } else if (result == 2) {
            setState(1);
        }
    }
}

// FIXME: 98.40 %. The row loop's start test `field_0xd8 > rowDelay + delay`:
// with the accumulator form (`delay += rowStep`) ours still starts the
// accumulator at rowDelay (mr r24, r17; one add fewer per pass) where the
// original keeps the add; the `field_0x38[0]->field_0xc -
// field_0x38[a4]->field_0xc` operands load into f2 / f0 (original f0 / f2);
// registers shift from there (r17-r26). The button loop's i * step form
// keeps the add, as in the original.
// Slides the other two file rows out and the command panes in, one after
// another (the timings are lbl_8050DB0C's 0x98..0x9F), then goes to state 19.
void FileSelect_804AFF60::fn_803CB3DC() {
    s16 rowDelay = lbl_8050DB0C.field_0x4c[0x98];
    s16 frames = lbl_8050DB0C.field_0x4c[0x99];
    s16 rowStep = lbl_8050DB0C.field_0x4c[0x9A];
    s16 boardDelay = lbl_8050DB0C.field_0x4c[0x9B];
    s16 boardFrames = lbl_8050DB0C.field_0x4c[0x9C];
    s16 buttonDelay = lbl_8050DB0C.field_0x4c[0x9D];
    s16 buttonFrames = lbl_8050DB0C.field_0x4c[0x9E];
    s16 buttonStep = lbl_8050DB0C.field_0x4c[0x9F];
    s32 end = rowDelay + (frames + rowStep);
    if (end < boardDelay + boardFrames) {
        end = boardDelay + boardFrames;
    }
    if (end < buttonDelay + (buttonFrames + buttonStep * 3)) {
        end = buttonDelay + (buttonFrames + buttonStep * 3);
    }

    // The two files that weren't chosen.
    s32 others[2];
    s32* other = others;
    for (s32 i = 0; i < 3; i++) {
        if (i != field_0xa4) {
            *other++ = i;
        }
    }

    field_0xd8++;
    s32 delay = 0;
    for (s32 i = 0; i < 2; i++) {
        if (field_0xd8 > rowDelay + delay) {
            field_0x38[others[i]]->fn_80402B10(frames, 0, 0.0f, 600.0f);
            field_0x38[others[i]]->fn_80402628(frames, field_0x38[others[i]]->field_0x28, 0);
            setRowAlphas(this, others[i]);
            field_0x44[others[i]]->fn_80402B10(frames, 0, 0.0f, 600.0f);
            field_0x44[others[i]]->fn_80402628(frames, field_0x44[others[i]]->field_0x28, 0);
            setTabAlphas(this, others[i]);
        }
        // The running delay keeps the original's add; `i * rowStep` scored worse here (97.94 %).
        delay += rowStep;
    }
    // The chosen row moves up to the first row's place.
    if (field_0xa4 != 0 && field_0xd8 > rowDelay) {
        field_0x38[field_0xa4]->fn_80402940(frames, 0, 0.0f,
                                            field_0x38[0]->field_0xc - field_0x38[field_0xa4]->field_0xc);
        field_0x44[field_0xa4]->fn_80402940(frames, 0, 0.0f,
                                            field_0x44[0]->field_0xc - field_0x44[field_0xa4]->field_0xc);
    }
    if (field_0xd8 > boardDelay) {
        field_0x14->fn_80402B10(boardFrames, 0, 600.0f, 0.0f);
        field_0x14->fn_80402628(boardFrames, 0, field_0x14->field_0x28);
        field_0x18->fn_80402B10(boardFrames, 0, 600.0f, 0.0f);
        field_0x18->fn_80402628(boardFrames, 0, field_0x18->field_0x28);
        field_0x1c->fn_80402B10(boardFrames, 0, 600.0f, 0.0f);
        field_0x1c->fn_80402628(boardFrames, 0, field_0x1c->field_0x28);
        for (s32 j = 0; j < 81; j++) {
            field_0x14->setAlpha_804023D0(mScreen->search(lbl_80476618[j]), field_0x132[j]);
        }
        for (s32 j = 0; j < 19; j++) {
            field_0x14->setAlpha_804023D0(mScreenHuuin->search(lbl_804768A0[j]), field_0x183[j]);
        }
    }
    for (s32 i = 0; i < 4; i++) {
        if (field_0xd8 > buttonDelay + i * buttonStep) {
            field_0x24[i]->fn_80402940(buttonFrames, 0, 100.0f, 0.0f);
            field_0x24[i]->fn_80402628(buttonFrames, 0, field_0x24[i]->field_0x28);
            switch (i) {
            case 0:
                for (s32 j = 0; j < 4; j++) {
                    field_0x24[i]->setAlpha_804023D0(mScreenHuuin->search(lbl_80476DC8[j]), field_0x19f[j]);
                }
                break;
            case 1:
                for (s32 j = 0; j < 4; j++) {
                    field_0x24[i]->setAlpha_804023D0(mScreenHuuin->search(lbl_80476DA8[j]), field_0x19f[j]);
                }
                break;
            case 2:
                for (s32 j = 0; j < 4; j++) {
                    field_0x24[i]->setAlpha_804023D0(mScreenHuuin->search(lbl_80476D88[j]), field_0x19f[j]);
                }
                break;
            case 3:
                for (s32 j = 0; j < 4; j++) {
                    field_0x24[i]->setAlpha_804023D0(mScreenHuuin->search(lbl_80476D68[j]), field_0x19f[j]);
                }
                break;
            }
        }
    }
    if (field_0xd8 > end) {
        setState(0x13);
    }
}

// FIXME: 98.37 %. The two `gGameRoot_8053AAF8->field_0xff != 0` tests compile to
// cmplwi where the original has cmpwi (u8, bool and s8 fields all differ:
// probably read through an int-returning accessor); the four setBlackWhite calls' by-value copies take the
// stack slots in the opposite order (sp 0x8 upwards in the original); the
// bob's `frame * frame` is scheduled after the 0x4330 store; the final
// `field_0xa0 == 3` test lays out its two arms (0xC / 0xB sound) the other
// way round (same logic); .sdata2 @NNN.
// The file menu (state 19): A runs the highlighted command field_0xa0 (0:
// start the chosen file, 1: copy, 2: delete, 3: back), B goes back, up /
// down move the cursor (field_0xa0, skipping commands an empty file lacks);
// the cursor pane field_0x34 bobs for ten frames after a move.
void FileSelect_804AFF60::fn_803CABAC() {
    if (lbl_8053AD30->fn_80309C4C(0x1100, 1) != 0) {
        switch (field_0xa0) {
        case 0: {
            lbl_8050EC80.mCurrentFile = field_0xa4;
            // OSGetTime() runs before the file is looked up (the original
            // reads mCurrentFile back after the call).
            s64 time = OSGetTime();
            lbl_8050EC80.getCurrentFile().field_0x0 = time;
            if (field_0x116 != 0) {
                lbl_8053AEC0->fn_804310BC();
                field_0x1a3 = 0;
            }
            lbl_80529DEC.fn_80403514();
            field_0x1a8 = 4;
            setState(40);
            lbl_80529DEC.field_0x23a[1] = field_0xa4;
            break;
        }
        case 1:
            if (gGameRoot_8053AAF8->field_0xff != 0) {
                if (field_0xa4 == 0) {
                    if (lbl_8050EC80.getFile(1).field_0x0 == 0) {
                        field_0xa0 = 1;
                    } else {
                        field_0xa0 = 2;
                    }
                }
                if (field_0xa4 == 1) {
                    if (lbl_8050EC80.getFile(2).field_0x0 == 0) {
                        field_0xa0 = 0;
                    } else {
                        field_0xa0 = 2;
                    }
                } else {
                    if (lbl_8050EC80.getFile(0).field_0x0 == 0) {
                        field_0xa0 = 0;
                    } else {
                        field_0xa0 = 1;
                    }
                }
                setMessage(0x61);
                fn_803C1340();
                break;
            }
            return;
        case 2:
            if (gGameRoot_8053AAF8->field_0xff != 0 && lbl_8050EC80.getFile(field_0xa4).field_0x0 != 0) {
                field_0x114 = 1;
                setMessage(0x5E);
                field_0x34->mPane->setAlpha(0);
                field_0x34->setAlpha_804023D0(mScreenHuuin->search('Paccur01'), field_0x34->field_0x28);
                field_0x34->field_0x26 = 0;
                ((J2DPicture*)mScreenHuuin->search('Pcopb01'))->setBlackWhite(field_0xe4, field_0xe0);
                ((J2DPicture*)mScreenHuuin->search('Pcopb02'))->setBlackWhite(field_0xe4, field_0xe0);
                ((J2DPicture*)mScreenHuuin->search('Pdelb01'))->setBlackWhite(field_0xe4, field_0xe0);
                ((J2DPicture*)mScreenHuuin->search('Pdelb02'))->setBlackWhite(field_0xe4, field_0xe0);
                for (int i = 0; i < 4; i++) {
                    field_0x24[i]->field_0x22 = 0;
                    field_0x24[i]->field_0x26 = 0;
                }
                field_0xd8 = 0;
                field_0xda = 0;
                setState(31);
                break;
            }
            return;
        case 3:
            fn_803C21A8();
            break;
        }
        if (field_0xa0 == 3) {
            gAudioMgr_8053AF80->fn_80458880(0xC, -1);
        } else {
            gAudioMgr_8053AF80->fn_80458880(0xB, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(0x200, 1) != 0) {
        fn_803C21A8();
        gAudioMgr_8053AF80->fn_80458880(0xC, -1);
    } else if (lbl_8053AD30->fn_80309A9C(2, 1) != 0) {
        gAudioMgr_8053AF80->fn_80458880(0x26, -1);
        if (lbl_8050EC80.getFile(field_0xa4).field_0x0 == 0) {
            if (field_0xa0 == 0) {
                field_0xa0 = 3;
            } else {
                field_0xa0 = 0;
            }
        } else {
            field_0xa0++;
            if (field_0xa0 >= 4) {
                field_0xa0 = 0;
            }
        }
        field_0xda = 1;
        field_0x34->field_0x26 = 30;
    } else if (lbl_8053AD30->fn_80309A9C(1, 1) != 0) {
        gAudioMgr_8053AF80->fn_80458880(0x26, -1);
        if (lbl_8050EC80.getFile(field_0xa4).field_0x0 == 0) {
            if (field_0xa0 == 0) {
                field_0xa0 = 3;
            } else {
                field_0xa0 = 0;
            }
        } else {
            field_0xa0--;
            if (field_0xa0 < 0) {
                field_0xa0 = 3;
            }
        }
        field_0xda = 1;
        field_0x34->field_0x26 = 30;
    }

    if (mState == 19) {
        s16 frame = field_0xda;
        f32 bob = 0.0f;
        if (frame != 0) {
            if (frame < 5) {
                bob = 5.0f * (frame * frame) / 25.0f;
                field_0xda = frame + 1;
            } else if (frame < 10) {
                bob = 5.0f * ((10 - frame) * (10 - frame)) / 25.0f;
                field_0xda = frame + 1;
            } else {
                field_0xda = 0;
            }
        }
        for (int i = 0; i < 4; i++) {
            if (bob != 0.0f && i == field_0xa0) {
                field_0x24[i]->moveFromCentre_80402E08(0.0f, bob);
            } else {
                field_0x24[i]->moveFromCentre_80402E08(0.0f, 0.0f);
            }
        }
        field_0x34->moveFromCentre_80402E08(field_0x24[field_0xa0]->field_0x8 - field_0x24[0]->field_0x8, bob);
        field_0x34->fn_8040245C(0x3C, 0, field_0x34->field_0x28, 1);
        field_0x34->setAlpha_804023D0(mScreenHuuin->search('Paccur01'), field_0x34->field_0x28);
    }
}

void FileSelect_804AFF60::fn_803C9F14() {
    s32 result = lbl_8051C1A0.fn_803B9BDC();
    if (result == 2) {
        field_0x84 = 0;
        field_0x80 = 0x69;
        setState(0);
    } else if (result == 1) {
        fn_803C0A34();
    }
}

void FileSelect_804AFF60::fn_803C92B8() {
    if (field_0xb0 > 0) {
        field_0xb0--;
    }
    if (lbl_8053AD30->fn_80309C4C(0x1300, 1) != 0 || field_0xb0 == 0) {
        field_0xb0 = -1;
        fn_803C0018();
        gAudioMgr_8053AF80->fn_80458880(0xB, -1);
    }
}

// FIXME: 99.85 %. Instructions identical; only the .sdata2 literals differ
// (@NNN vs lbl_: the literals of the unwritten earlier functions come first).
void FileSelect_804AFF60::fn_803C6690() {
    s16 delay = lbl_8050DB0C.field_0x4c[0xC0];
    s16 frames = lbl_8050DB0C.field_0x4c[0xC1];
    s32 end = delay + frames;
    field_0xd8++;
    if (field_0xd8 > delay) {
        for (s32 i = 0; i < 3; i++) {
            field_0x38[i]->fn_80402940(frames, 0, field_0x70[i], 0.0f);
            field_0x44[i]->fn_80402940(frames, 0, field_0x64[i], 0.0f);
        }
        fn_803BC6F0(field_0xa4, 1.0f - (f32)(field_0xd8 - delay) / (f32)frames);
    }
    if (field_0xd8 > end) {
        setState(5);
    }
}

void FileSelect_804AFF60::fn_803C579C() {
    s32 result = lbl_8051C1A0.fn_803B9BDC();
    if (result == 2) {
        field_0x84 = 0;
        field_0x80 = 0x68;
        setState(0);
    } else if (result == 1) {
        fn_803BF004();
    }
}

// FIXME: 99.95 %. Instructions identical; only the .sdata2 literals differ
// (pool order, see fn_803C24BC).
// Back from the delete prompt (state 36), after its message or a button:
// refreshes the rows (fn_803CE4CC), puts the title message back, hides the
// file cursor and the command cursor, and goes to state 38.
void FileSelect_804AFF60::fn_803C4330() {
    if (field_0xb0 > 0) {
        field_0xb0--;
    }
    if (lbl_8053AD30->fn_80309C4C(0x1300, 1) != 0 || field_0xb0 == 0) {
        field_0xb0 = -1;
        fn_803CE4CC();
        setMessage(0x117);
        field_0x50->moveFromCentre_80402E08(-300.0f, 0.0f);
        field_0x50->field_0x20 = 0;
        field_0x50->mPane->setAlpha(0);
        field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur01'), field_0x50->field_0x28);
        field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur02'), field_0x50->field_0x28);
        field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur03'), field_0x50->field_0x28);
        field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur04'), field_0x50->field_0x28);
        for (int i = 0; i < 3; i++) {
            field_0x38[i]->field_0x20 = 0;
            field_0x38[i]->field_0x22 = 0;
            field_0x38[i]->field_0x26 = 0;
            field_0x44[i]->field_0x20 = 0;
            field_0x44[i]->field_0x22 = 0;
            field_0x44[i]->field_0x26 = 0;
        }
        field_0x34->mPane->setAlpha(0);
        field_0x34->setAlpha_804023D0(mScreenHuuin->search('Paccur01'), field_0x34->field_0x28);
        field_0xd8 = 0;
        field_0xda = 0;
        setState(38);
        gAudioMgr_8053AF80->fn_80458880(0xB, -1);
    }
}

// FIXME: 96.12 %. Scheduling and registers only, in the cursor bob: the
// original computes `(10 - frame)` into r0 and schedules both squares'
// `lis 0x4330` / stack store before the mullw (same as fn_803CABAC); storing
// field_0xda first (95.49) or a shared inline (95.96) was worse. .sdata2 @NNN.
// The delete prompt (state 37): up / down move the cursor field_0x114
// between its two buttons, A confirms (0: fn_803BF3BC, 1: back out through
// fn_803BF698); the cursor bobs as in fn_803CABAC.
void FileSelect_804AFF60::fn_803C4054() {
    if (lbl_8053AD30->fn_80309C4C(1, 1) != 0) {
        if (field_0x114 != 0) {
            field_0x114--;
            field_0xda = 1;
            field_0x34->field_0x26 = 30;
            gAudioMgr_8053AF80->fn_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(2, 1) != 0) {
        if (field_0x114 < 1) {
            field_0x114++;
            field_0xda = 1;
            field_0x34->field_0x26 = 30;
            gAudioMgr_8053AF80->fn_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(0x100, 1) != 0) {
        if (field_0x114 == 0) {
            fn_803BF3BC();
            gAudioMgr_8053AF80->fn_80458880(0xB, -1);
        } else {
            field_0x80 = 0;
            fn_803BF698();
            gAudioMgr_8053AF80->fn_80458880(0xC, -1);
        }
    }

    if (mState == 37) {
        s16 frame = field_0xda;
        f32 bob = 0.0f;
        if (frame != 0) {
            if (frame < 5) {
                bob = 5.0f * (frame * frame) / 25.0f;
                field_0xda = frame + 1;
            } else if (frame < 10) {
                bob = 5.0f * ((10 - frame) * (10 - frame)) / 25.0f;
                field_0xda = frame + 1;
            } else {
                field_0xda = 0;
            }
        }
        for (int i = 0; i < 2; i++) {
            if (bob != 0.0f && i == field_0x114) {
                field_0x24[i]->moveFromCentre_80402E08(0.0f, bob);
            } else {
                field_0x24[i]->moveFromCentre_80402E08(0.0f, 0.0f);
            }
        }
        field_0x34->moveFromCentre_80402E08(field_0x24[field_0x114]->field_0x8 - field_0x24[0]->field_0x8, bob);
        field_0x34->fn_8040245C(0x3C, 0, field_0x34->field_0x28, 1);
        field_0x34->setAlpha_804023D0(mScreenHuuin->search('Paccur01'), field_0x34->field_0x28);
    }
}

// FIXME: 98.27 % (try.py: different, same length). One instruction moves:
// the original keeps &others in r29 from the fill loop on (`mr r3, r29` for
// the fill pointer) and walks it in the row loop; ours computes &others
// again before the row loop (addi r29, r1, 0x8). Every field, call and
// branch is the original's. A running `delay += step` instead of
// `i * step` folds the start add (98.16, 3 instructions fewer). .sdata2 @NNN.
// Closes the file menu (state 39): the board (or the copy board when the
// file holds save data) and the first two command buttons slide out, the
// other two rows slide back in and the chosen row returns to its place, one
// after another (the timings are lbl_8050DB0C's 0xD4..0xDB); then state 5.
void FileSelect_804AFF60::fn_803C2E24() {
    s16 boardDelay = lbl_8050DB0C.field_0x4c[0xD4];
    s16 boardFrames = lbl_8050DB0C.field_0x4c[0xD5];
    s16 buttonDelay = lbl_8050DB0C.field_0x4c[0xD6];
    s16 buttonFrames = lbl_8050DB0C.field_0x4c[0xD7];
    s16 buttonStep = lbl_8050DB0C.field_0x4c[0xD8];
    s16 rowDelay = lbl_8050DB0C.field_0x4c[0xD9];
    s16 frames = lbl_8050DB0C.field_0x4c[0xDA];
    s16 rowStep = lbl_8050DB0C.field_0x4c[0xDB];
    s32 end = boardDelay + boardFrames;
    if (end < buttonDelay + (buttonFrames + buttonStep * 3)) {
        end = buttonDelay + (buttonFrames + buttonStep * 3);
    }
    if (end < rowDelay + (frames + rowStep)) {
        end = rowDelay + (frames + rowStep);
    }

    // The two files that weren't chosen.
    s32 others[2];
    s32* other = others;
    for (int i = 0; i < 3; i++) {
        if (i != field_0xa4) {
            *other++ = i;
        }
    }

    field_0xd8++;
    if (field_0xd8 > boardDelay) {
        if (hasBadChecksum(field_0xa4)) {
            field_0x20->fn_80402B10(boardFrames, 0, 0.0f, 600.0f);
            field_0x20->fn_80402628(boardFrames, field_0x20->field_0x28, 0);
            for (int j = 0; j < 9; j++) {
                field_0x20->setAlpha_804023D0(mScreen->search(lbl_80476938[j]), field_0x196[j]);
            }
        } else {
            field_0x14->fn_80402B10(boardFrames, 0, 0.0f, 600.0f);
            field_0x14->fn_80402628(boardFrames, field_0x14->field_0x28, 0);
            field_0x18->fn_80402B10(boardFrames, 0, 0.0f, 600.0f);
            field_0x18->fn_80402628(boardFrames, field_0x18->field_0x28, 0);
            field_0x1c->fn_80402B10(boardFrames, 0, 0.0f, 600.0f);
            field_0x1c->fn_80402628(boardFrames, field_0x1c->field_0x28, 0);
            for (int j = 0; j < 81; j++) {
                field_0x14->setAlpha_804023D0(mScreen->search(lbl_80476618[j]), field_0x132[j]);
            }
            for (int j = 0; j < 19; j++) {
                field_0x14->setAlpha_804023D0(mScreenHuuin->search(lbl_804768A0[j]), field_0x183[j]);
            }
        }
    }
    // Only the first two buttons, although `end` allows for four.
    for (int i = 0; i < 2; i++) {
        if (field_0xd8 > buttonDelay + i * buttonStep) {
            field_0x24[i]->fn_80402940(buttonFrames, 0, 0.0f, 100.0f);
            field_0x24[i]->fn_80402628(buttonFrames, field_0x24[i]->field_0x28, 0);
            switch (i) {
            case 0:
                for (int j = 0; j < 4; j++) {
                    field_0x24[i]->setAlpha_804023D0(mScreenHuuin->search(lbl_80476DC8[j]), field_0x19f[j]);
                }
                break;
            case 1:
                for (int j = 0; j < 4; j++) {
                    field_0x24[i]->setAlpha_804023D0(mScreenHuuin->search(lbl_80476DA8[j]), field_0x19f[j]);
                }
                break;
            case 2:
                for (int j = 0; j < 4; j++) {
                    field_0x24[i]->setAlpha_804023D0(mScreenHuuin->search(lbl_80476D88[j]), field_0x19f[j]);
                }
                break;
            case 3:
                for (int j = 0; j < 4; j++) {
                    field_0x24[i]->setAlpha_804023D0(mScreenHuuin->search(lbl_80476D68[j]), field_0x19f[j]);
                }
                break;
            }
        }
    }
    for (int i = 0; i < 2; i++) {
        if (field_0xd8 > rowDelay + i * rowStep) {
            field_0x38[others[i]]->fn_80402B10(frames, 0, 600.0f, 0.0f);
            field_0x38[others[i]]->fn_80402628(frames, 0, field_0x38[others[i]]->field_0x28);
            setRowAlphas(this, others[i]);
            field_0x44[others[i]]->fn_80402B10(frames, 0, 600.0f, 0.0f);
            field_0x44[others[i]]->fn_80402628(frames, 0, field_0x44[others[i]]->field_0x28);
            setTabAlphas(this, others[i]);
        }
    }
    // The chosen row moves back down from the first row's place.
    if (field_0xa4 != 0 && field_0xd8 > rowDelay) {
        field_0x38[field_0xa4]->fn_80402940(frames, 0, field_0x38[0]->field_0xc - field_0x38[field_0xa4]->field_0xc,
                                            0.0f);
        field_0x44[field_0xa4]->fn_80402940(frames, 0, field_0x44[0]->field_0xc - field_0x44[field_0xa4]->field_0xc,
                                            0.0f);
    }
    if (field_0xd8 > end) {
        setState(5);
    }
}

// FIXME: 99.93 %. Instructions identical; only the .sdata2 literals differ
// (@NNN vs lbl_: the pool starts 0, -200, 200, which no earlier written
// function produces in that order).
// Shows the three file rows: refreshes them (fn_803CE4CC), puts up the
// title message, slides the row panes in from their start positions with
// alpha 0 and goes to state 4.
void FileSelect_804AFF60::fn_803C24BC() {
    fn_803CE4CC();
    setMessage(0x117);
    field_0x14->mPane->hide();
    field_0x18->mPane->hide();
    field_0x1c->mPane->hide();
    field_0x34->mPane->hide();
    for (s32 i = 0; i < 4; i++) {
        field_0x24[i]->mPane->hide();
    }
    field_0x50->mPane->show();
    for (s32 i = 0; i < 3; i++) {
        field_0x38[i]->mPane->show();
        field_0x44[i]->mPane->show();
    }

    // The original inlines this block into both fn_803C24BC and fn_803CF504; no out-of-line copy exists.
    field_0x50->moveFromCentre_80402E08(-300.0f, 0.0f);
    field_0x50->field_0x20 = 0;
    field_0x50->mPane->setAlpha(0);
    field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur01'), field_0x50->field_0x28);
    field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur02'), field_0x50->field_0x28);
    field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur03'), field_0x50->field_0x28);
    field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur04'), field_0x50->field_0x28);
    field_0x50->field_0x26 = 0;

    for (s32 i = 0; i < 3; i++) {
        field_0x38[i]->moveFromCentre_80402E08(0.0f, 300.0f);
        field_0x38[i]->field_0x22 = 0;
        field_0x38[i]->mPane->setAlpha(0);
        setRowAlphas(this, i);
        field_0x38[i]->field_0x26 = 0;

        field_0x44[i]->moveFromCentre_80402E08(-300.0f, 0.0f);
        field_0x44[i]->field_0x20 = 0;
        field_0x44[i]->mPane->setAlpha(0);
        setTabAlphas(this, i);
        field_0x44[i]->field_0x26 = 0;
    }

    field_0x54->moveFromCentre_80402E08(0.0f, -200.0f);
    field_0x54->field_0x22 = 0;
    field_0x54->mPane->setAlpha(0);
    field_0x54->field_0x26 = 0;
    field_0xd8 = 0;
    field_0xda = 0;
    setState(4);
}

// fn_803CABAC (the file menu), closing it: rereads the files (fn_803CE4CC),
// shows the file list's message (0x117 / 0x116 as fn_803C0018), slides the
// file cursor off to the left and fades it, stops the rows, the board's
// movers and the four buttons; state 6.
void FileSelect_804AFF60::fn_803C21A8() {
    fn_803CE4CC();
    setMessage(0x117);
    field_0x50->moveFromCentre_80402E08(-300.0f, 0.0f);
    field_0x50->field_0x20 = 0;
    field_0x50->mPane->setAlpha(0);
    field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur01'), field_0x50->field_0x28);
    field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur02'), field_0x50->field_0x28);
    field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur03'), field_0x50->field_0x28);
    field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur04'), field_0x50->field_0x28);
    for (s32 i = 0; i < 3; i++) {
        field_0x38[i]->field_0x20 = 0;
        field_0x38[i]->field_0x22 = 0;
        field_0x38[i]->field_0x26 = 0;
        field_0x44[i]->field_0x20 = 0;
        field_0x44[i]->field_0x22 = 0;
        field_0x44[i]->field_0x26 = 0;
    }
    field_0x14->field_0x20 = 0;
    field_0x14->field_0x26 = 0;
    field_0x18->field_0x20 = 0;
    field_0x18->field_0x26 = 0;
    field_0x1c->field_0x20 = 0;
    field_0x1c->field_0x26 = 0;
    field_0x34->mPane->setAlpha(0);
    field_0x34->setAlpha_804023D0(mScreenHuuin->search('Paccur01'), field_0x34->field_0x28);
    for (s32 i = 0; i < 4; i++) {
        field_0x24[i]->field_0x22 = 0;
        field_0x24[i]->field_0x26 = 0;
    }
    field_0xd8 = 0;
    field_0xda = 0;
    setState(6);
}

// FIXME: 99.33 %. Register allocation only: the table loops' counter and
// pointer swap (r26 / r27 and r29 / r27 in the original); a function-wide
// `int i` was worse (98.85). .sdata2 @NNN.
// Opens the file menu for the chosen file field_0xa4 (state 18): the 'Pdb'
// marks of its flags, the command buttons (copy and delete greyed out for an
// empty file), the board panes faded out at their start positions, and the
// command labels.
void FileSelect_804AFF60::fn_803C1734() {
    for (int i = 0; i < 8; i++) {
        if (fn_803B90E4(field_0xa4, i)) {
            mScreen->search(lbl_80476980[i + 2])->hide();
        } else {
            mScreen->search(lbl_80476980[i + 2])->show();
        }
    }
    field_0xa8 = field_0xa4;
    for (int i = 0; i < 8; i++) {
        field_0xb8[i] = fn_803B8CAC(field_0xa4, i);
    }
    field_0x14->mPane->show();
    field_0x18->mPane->show();
    field_0x1c->mPane->show();
    field_0x34->mPane->show();
    for (int i = 0; i < 4; i++) {
        field_0x24[i]->mPane->show();
    }
    if (lbl_8050EC80.getFile(field_0xa4).field_0x0 == 0) {
        ((J2DPicture*)mScreenHuuin->search('Pcopb01'))
            ->setBlackWhite(JUtility::TColor(0x50, 0x50, 0x50, 0), JUtility::TColor(0xB4, 0xB4, 0xB4, 0xFF));
        ((J2DPicture*)mScreenHuuin->search('Pcopb02'))
            ->setBlackWhite(JUtility::TColor(0x50, 0x50, 0x50, 0), JUtility::TColor(0xB4, 0xB4, 0xB4, 0xFF));
        ((J2DPicture*)mScreenHuuin->search('Pdelb01'))
            ->setBlackWhite(JUtility::TColor(0x50, 0x50, 0x50, 0), JUtility::TColor(0xB4, 0xB4, 0xB4, 0xFF));
        ((J2DPicture*)mScreenHuuin->search('Pdelb02'))
            ->setBlackWhite(JUtility::TColor(0x50, 0x50, 0x50, 0), JUtility::TColor(0xB4, 0xB4, 0xB4, 0xFF));
    } else {
        ((J2DPicture*)mScreenHuuin->search('Pcopb01'))->setBlackWhite(field_0xe4, field_0xe0);
        ((J2DPicture*)mScreenHuuin->search('Pcopb02'))->setBlackWhite(field_0xe4, field_0xe0);
        ((J2DPicture*)mScreenHuuin->search('Pdelb01'))->setBlackWhite(field_0xe4, field_0xe0);
        ((J2DPicture*)mScreenHuuin->search('Pdelb02'))->setBlackWhite(field_0xe4, field_0xe0);
    }

    field_0x50->mPane->setAlpha(0);
    field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur01'), field_0x50->field_0x28);
    field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur02'), field_0x50->field_0x28);
    field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur03'), field_0x50->field_0x28);
    field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur04'), field_0x50->field_0x28);
    field_0x50->field_0x26 = 0;
    for (int i = 0; i < 3; i++) {
        field_0x38[i]->field_0x20 = 0;
        field_0x38[i]->field_0x22 = 0;
        field_0x38[i]->field_0x26 = 0;
        field_0x44[i]->field_0x20 = 0;
        field_0x44[i]->field_0x22 = 0;
        field_0x44[i]->field_0x26 = 0;
    }

    field_0x14->moveFromCentre_80402E08(600.0f, 0.0f);
    field_0x14->field_0x20 = 0;
    field_0x14->mPane->setAlpha(0);
    field_0x18->moveFromCentre_80402E08(600.0f, 0.0f);
    field_0x18->field_0x20 = 0;
    field_0x18->mPane->setAlpha(0);
    field_0x1c->moveFromCentre_80402E08(600.0f, 0.0f);
    field_0x1c->field_0x20 = 0;
    field_0x1c->mPane->setAlpha(0);
    for (int i = 0; i < 81; i++) {
        field_0x14->setAlpha_804023D0(mScreen->search(lbl_80476618[i]), field_0x132[i]);
    }
    for (int i = 0; i < 19; i++) {
        field_0x14->setAlpha_804023D0(mScreenHuuin->search(lbl_804768A0[i]), field_0x183[i]);
    }
    field_0x14->field_0x26 = 0;
    field_0x18->field_0x26 = 0;
    field_0x1c->field_0x26 = 0;

    field_0x34->mPane->setAlpha(0);
    field_0x34->setAlpha_804023D0(mScreenHuuin->search('Paccur01'), field_0x34->field_0x28);
    field_0x34->field_0x26 = 0;
    for (int i = 0; i < 4; i++) {
        field_0x24[i]->moveFromCentre_80402E08(0.0f, 100.0f);
        field_0x24[i]->field_0x22 = 0;
        field_0x24[i]->mPane->setAlpha(0);
        switch (i) {
        case 0:
            for (int j = 0; j < 4; j++) {
                field_0x24[i]->setAlpha_804023D0(mScreenHuuin->search(lbl_80476DC8[j]), field_0x19f[j]);
            }
            break;
        case 1:
            for (int j = 0; j < 4; j++) {
                field_0x24[i]->setAlpha_804023D0(mScreenHuuin->search(lbl_80476DA8[j]), field_0x19f[j]);
            }
            break;
        case 2:
            for (int j = 0; j < 4; j++) {
                field_0x24[i]->setAlpha_804023D0(mScreenHuuin->search(lbl_80476D88[j]), field_0x19f[j]);
            }
            break;
        case 3:
            for (int j = 0; j < 4; j++) {
                field_0x24[i]->setAlpha_804023D0(mScreenHuuin->search(lbl_80476D68[j]), field_0x19f[j]);
            }
            break;
        }
        field_0x24[i]->field_0x26 = 0;
    }

    getMessageText_803832BC(((J2DTextBox*)mScreenHuuin->search('Tstr01'))->getStringPtr(), 0x118, 0, NULL, NULL);
    getMessageText_803832BC(((J2DTextBox*)mScreenHuuin->search('Tcop01'))->getStringPtr(), 0x119, 0, NULL, NULL);
    getMessageText_803832BC(((J2DTextBox*)mScreenHuuin->search('Tdel01'))->getStringPtr(), 0x11A, 0, NULL, NULL);
    getMessageText_803832BC(((J2DTextBox*)mScreenHuuin->search('Tbac01'))->getStringPtr(), 0x11B, 0, NULL, NULL);
    field_0xd8 = 0;
    field_0xda = 0;
    setState(18);
}

// Not auto-inlined: fn_803C92B8 calls fn_803C0018 out of line in the original
// (docs/INLINING.md, pragma rule 1).
#pragma auto_inline off
// Called by fn_803C92B8 (state 24): shows the file list's message (0x117, or
// 0x116 with gGameRoot_8053AAF8->field_0x7f set), hides the file cursor
// ('Pfcur01'..'04') and 'Paccur01', stops the rows' and buttons' movers,
// state 29.
void FileSelect_804AFF60::fn_803C0018() {
    setMessage(0x117);
    field_0x50->mPane->setAlpha(0);
    field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur01'), field_0x50->field_0x28);
    field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur02'), field_0x50->field_0x28);
    field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur03'), field_0x50->field_0x28);
    field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur04'), field_0x50->field_0x28);
    for (s32 i = 0; i < 3; i++) {
        field_0x38[i]->field_0x20 = 0;
        field_0x38[i]->field_0x22 = 0;
        field_0x38[i]->field_0x26 = 0;
        field_0x44[i]->field_0x20 = 0;
        field_0x44[i]->field_0x22 = 0;
        field_0x44[i]->field_0x26 = 0;
    }
    field_0x34->mPane->setAlpha(0);
    field_0x34->setAlpha_804023D0(mScreenHuuin->search('Paccur01'), field_0x34->field_0x28);
    field_0x14->field_0x20 = 0;
    field_0x14->field_0x26 = 0;
    field_0x18->field_0x20 = 0;
    field_0x18->field_0x26 = 0;
    field_0x1c->field_0x20 = 0;
    field_0x1c->field_0x26 = 0;
    field_0xd8 = 0;
    field_0xda = 0;
    setState(29);
}
#pragma auto_inline reset

// FIXME: 98.49 %. Register allocation and scheduling only: `this` lands in
// r28 (original r31) and the loop registers shift (r26..r30); the 9-pane loop
// counter / pointer are r28 / r27 in the original. With the counters declared
// per loop each case of the switch re-loads j = 0 (96.49). .sdata2 @NNN.
// Opens the copy board for the chosen file (state 32): the target cursor
// field_0x50 and the board field_0x20 with its 'Pdb' panes faded in from
// their start position, the command buttons back at their colours, and the
// copy labels; the timings are lbl_8050DB0C's 0xCA..0xCD.
void FileSelect_804AFF60::fn_803BF9C4() {
    // Function-scope counters: declared per loop, each switch case re-loads j = 0 (96.49 %).
    int i;
    int j;
    field_0x20->mPane->show();
    field_0x34->mPane->show();
    field_0x50->mPane->setAlpha(0);
    field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur01'), field_0x50->field_0x28);
    field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur02'), field_0x50->field_0x28);
    field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur03'), field_0x50->field_0x28);
    field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur04'), field_0x50->field_0x28);
    ((J2DPicture*)mScreenHuuin->search('Pcopb01'))->setBlackWhite(field_0xe4, field_0xe0);
    ((J2DPicture*)mScreenHuuin->search('Pcopb02'))->setBlackWhite(field_0xe4, field_0xe0);
    ((J2DPicture*)mScreenHuuin->search('Pdelb01'))->setBlackWhite(field_0xe4, field_0xe0);
    ((J2DPicture*)mScreenHuuin->search('Pdelb02'))->setBlackWhite(field_0xe4, field_0xe0);
    for (i = 0; i < 3; i++) {
        field_0x38[i]->field_0x20 = 0;
        field_0x38[i]->field_0x22 = 0;
        field_0x38[i]->field_0x26 = 0;
        field_0x44[i]->field_0x20 = 0;
        field_0x44[i]->field_0x22 = 0;
        field_0x44[i]->field_0x26 = 0;
    }

    field_0x20->moveFromCentre_80402E08(600.0f, 0.0f);
    field_0x20->field_0x20 = 0;
    field_0x20->mPane->setAlpha(field_0x20->field_0x28);
    for (i = 0; i < 9; i++) {
        field_0x20->setAlpha_804023D0(mScreen->search(lbl_80476938[i]), field_0x196[i]);
    }
    field_0x20->field_0x26 = 0;

    field_0x34->mPane->setAlpha(0);
    field_0x34->setAlpha_804023D0(mScreenHuuin->search('Paccur01'), field_0x34->field_0x28);
    field_0x34->field_0x26 = 0;
    for (i = 0; i < 4; i++) {
        field_0x24[i]->mPane->show();
        field_0x24[i]->moveFromCentre_80402E08(0.0f, 100.0f);
        field_0x24[i]->field_0x22 = 0;
        field_0x24[i]->mPane->setAlpha(0);
        field_0x24[i]->field_0x26 = 0;
        switch (i) {
        case 0:
            for (j = 0; j < 4; j++) {
                field_0x24[i]->setAlpha_804023D0(mScreenHuuin->search(lbl_80476DC8[j]), field_0x19f[j]);
            }
            break;
        case 1:
            for (j = 0; j < 4; j++) {
                field_0x24[i]->setAlpha_804023D0(mScreenHuuin->search(lbl_80476DA8[j]), field_0x19f[j]);
            }
            break;
        case 2:
            for (j = 0; j < 4; j++) {
                field_0x24[i]->setAlpha_804023D0(mScreenHuuin->search(lbl_80476D88[j]), field_0x19f[j]);
            }
            break;
        case 3:
            for (j = 0; j < 4; j++) {
                field_0x24[i]->setAlpha_804023D0(mScreenHuuin->search(lbl_80476D68[j]), field_0x19f[j]);
            }
            break;
        }
    }

    getMessageText_803832BC(((J2DTextBox*)mScreenHuuin->search('Tstr01'))->getStringPtr(), 0x11E, 0, NULL, NULL);
    getMessageText_803832BC(((J2DTextBox*)mScreenHuuin->search('Tcop01'))->getStringPtr(), 0x11F, 0, NULL, NULL);
    s16 delay = lbl_8050DB0C.field_0x4c[0xCA];
    s16 frames = lbl_8050DB0C.field_0x4c[0xCB];
    s16 step = lbl_8050DB0C.field_0x4c[0xCC];
    field_0xd8 = lbl_8050DB0C.field_0x4c[0xCD];
    s16 end = delay + (frames + step);
    if (field_0xd8 < end) {
        field_0xd8 = end;
    }
    field_0xda = 0;
    setState(32);
}

// FIXME: 99.95. Instructions identical; .sdata2 literal names only.
// fn_803C4054 (the delete prompt), the other answer: rereads the files
// (fn_803CE4CC), shows the file list's message (0x117 / 0x116 as
// fn_803C0018), slides the file cursor off to the left and fades it, stops
// the rows, the board or the buttons and the first two buttons; state 39.
void FileSelect_804AFF60::fn_803BF698() {
    fn_803CE4CC();
    setMessage(0x117);
    field_0x50->moveFromCentre_80402E08(-300.0f, 0.0f);
    field_0x50->field_0x20 = 0;
    field_0x50->mPane->setAlpha(0);
    field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur01'), field_0x50->field_0x28);
    field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur02'), field_0x50->field_0x28);
    field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur03'), field_0x50->field_0x28);
    field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur04'), field_0x50->field_0x28);
    for (s32 i = 0; i < 3; i++) {
        field_0x38[i]->field_0x20 = 0;
        field_0x38[i]->field_0x22 = 0;
        field_0x38[i]->field_0x26 = 0;
        field_0x44[i]->field_0x20 = 0;
        field_0x44[i]->field_0x22 = 0;
        field_0x44[i]->field_0x26 = 0;
    }
    field_0x34->mPane->setAlpha(0);
    field_0x34->setAlpha_804023D0(mScreenHuuin->search('Paccur01'), field_0x34->field_0x28);
    if (hasBadChecksum(field_0xa4)) {
        field_0x20->field_0x20 = 0;
        field_0x20->field_0x26 = 0;
    } else {
        field_0x14->field_0x20 = 0;
        field_0x14->field_0x26 = 0;
        field_0x18->field_0x20 = 0;
        field_0x18->field_0x26 = 0;
        field_0x1c->field_0x20 = 0;
        field_0x1c->field_0x26 = 0;
    }
    field_0x24[0]->field_0x22 = 0;
    field_0x24[0]->field_0x26 = 0;
    field_0x24[1]->field_0x22 = 0;
    field_0x24[1]->field_0x26 = 0;
    field_0xd8 = 0;
    field_0xda = 0;
    setState(39);
}

// FIXME: 99.62. Regalloc only: the 'Pdb' loop's counter and table pointer
// swap r29 / r30 (an alpha local doesn't change it).
// fn_803C4054 (the delete prompt), one answer: the file cursor slides off to
// the left and fades, the board (field_0x20) comes back (in from the right
// with its 'Pdb' panes' alphas unless the file's checksum failed), the first
// two buttons stop; state 34.
void FileSelect_804AFF60::fn_803BF3BC() {
    field_0x20->mPane->show();
    field_0x50->moveFromCentre_80402E08(-300.0f, 0.0f);
    field_0x50->field_0x20 = 0;
    field_0x50->mPane->setAlpha(0);
    field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur01'), field_0x50->field_0x28);
    field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur02'), field_0x50->field_0x28);
    field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur03'), field_0x50->field_0x28);
    field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur04'), field_0x50->field_0x28);
    if (hasBadChecksum(field_0xa4)) {
        field_0x20->field_0x20 = 0;
        field_0x20->field_0x26 = 0;
    } else {
        field_0x14->field_0x20 = 0;
        field_0x14->field_0x26 = 0;
        field_0x18->field_0x20 = 0;
        field_0x18->field_0x26 = 0;
        field_0x1c->field_0x20 = 0;
        field_0x1c->field_0x26 = 0;
        field_0x20->moveFromCentre_80402E08(600.0f, 0.0f);
        field_0x20->field_0x20 = 0;
        field_0x20->mPane->setAlpha(0);
        for (s32 i = 0; i < 9; i++) {
            field_0x20->setAlpha_804023D0(mScreen->search(lbl_80476938[i]), field_0x196[i]);
        }
        field_0x20->field_0x26 = 0;
    }
    field_0x34->mPane->setAlpha(0);
    field_0x34->setAlpha_804023D0(mScreenHuuin->search('Paccur01'), field_0x34->field_0x28);
    field_0x34->field_0x26 = 0;
    field_0x24[0]->field_0x22 = 0;
    field_0x24[0]->field_0x26 = 0;
    field_0x24[1]->field_0x22 = 0;
    field_0x24[1]->field_0x26 = 0;
    field_0xd8 = 0;
    field_0xda = 0;
    setState(34);
}

// FIXME: 99.66. Instructions identical; only the .sdata2 literal names differ
// (the pool's leading 0, -200, 200, see the progress notes).
// The board's enemy sprites (fn_803CEED4 draws them after the screens).
// field_0xdc runs 0..53 for their animation. With field_0xb8[6] set every
// one of the eight 'Padvw?1' / 'Padvw?2' pane pairs gets its enemy twice:
// black at 'Padvw?2' (a shadow) and in colour at 'Padvw?1', each with its
// pane's alpha (the eighth pair, 'Padvl01' / 'Padvl02', shows the player).
// Otherwise each of the first seven whose field_0xb8 flag is set gets one
// animated sprite at its 'Padvw?0' pane.
void FileSelect_804AFF60::fn_803BE3D8() {
    field_0xdc++;
    if (field_0xdc >= 54) {
        field_0xdc = 0;
    }
    if (field_0xb8[6] != 0) {
        SpriteParams_801F5AE0 shadow;
        SpriteParams_801F5AE0 params;
        for (s32 i = 0; i < 8; i++) {
            JGeometry::TVec3f shadowPos;
            f32 x;
            PANE_CENTRE_X(x, mScreen, lbl_80476A48[i]);
            shadowPos.x = toSpriteX(x);
            f32 y;
            PANE_CENTRE_Y(y, mScreen, lbl_80476A48[i]);
            shadowPos.y = toSpriteY(13.0f + y);
            shadowPos.z = 0.0f;
            JGeometry::TVec3f pos;
            PANE_CENTRE_X(x, mScreen, lbl_80476A08[i]);
            pos.x = toSpriteX(x);
            PANE_CENTRE_Y(y, mScreen, lbl_80476A08[i]);
            pos.y = toSpriteY(13.0f + y);
            pos.z = 0.0f;
            shadow.field_0x26 = 0;
            shadow.field_0x10.r = 0;
            shadow.field_0x10.g = 0;
            shadow.field_0x10.b = 0;
            shadow.field_0x10.a = mScreen->search(lbl_80476A48[i])->getAlpha();
            params.field_0x10.a = mScreen->search(lbl_80476A08[i])->getAlpha();
            params.field_0x26 = 0;
            JGeometry::TBox2f box(0.0f, 0.0f, 360.0f, 270.0f);
            gGameRoot2_8053AB10->fn_8013B414(&box, true);
            if (i == 6) {
                void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
                gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&shadowPos, res, 0xAB, &shadow, 1.0f, 1.0f);
                res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
                gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, 0xAB, &params, 1.0f, 1.0f);
            } else if (i == 7) {
                shadow.field_0x0[7] = 12;
                params.field_0x0[7] = 12;
                void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "player_sob.bin");
                gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&shadowPos, res, 0x10, &shadow, 1.0f, 1.0f);
                res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "player_sob.bin");
                gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, 0x10, &params, 1.0f, 1.0f);
            } else {
                s32 frame = lbl_80476DE8[i] + 8;
                void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_gc_sob.bin");
                gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&shadowPos, res, frame, &shadow, 1.0f, 1.0f);
                frame = lbl_80476DE8[i] + 8;
                res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_gc_sob.bin");
                gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, frame, &params, 1.0f, 1.0f);
            }
        }
    } else {
        for (s32 i = 0; i < 8; i++) {
            if (i < 7 && field_0xb8[i] != 0) {
                SpriteParams_801F5AE0 params;
                params.field_0x26 = 0;
                params.field_0x10.a = mScreen->search(lbl_804769D0[i])->getAlpha();
                JGeometry::TVec3f pos;
                if (i == 6) {
                    setSpritePos(this, &pos, &lbl_804769D0[i], 0.0f, 10.0f);
                    JGeometry::TBox2f box(0.0f, 0.0f, 360.0f, 270.0f);
                    gGameRoot2_8053AB10->fn_8013B414(&box, true);
                    s32 frame = (field_0xdc % 27) / 9 + 0x21B;
                    void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_gc_sob.bin");
                    gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, frame, &params, 1.0f, 1.0f);
                } else {
                    setSpritePos(this, &pos, &lbl_804769D0[i], 0.0f, 12.0f);
                    JGeometry::TBox2f box(0.0f, 0.0f, 360.0f, 270.0f);
                    gGameRoot2_8053AB10->fn_8013B414(&box, true);
                    s32 frame = field_0xdc / 9 + lbl_80476DE8[i];
                    void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_gc_sob.bin");
                    gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, frame, &params, 1.0f, 1.0f);
                }
            }
        }
    }
    gGameRoot2_8053AB10->fn_8013B490();
}

// FIXME: 99.58 %. Stack slots only: the original puts the first call's
// by-value copies above the group colours (sp 0xC8) and numbers each group's
// call temporaries upwards (0x7C, 0x84, ...), ours the other way round. Same
// copy pattern as fn_803BC6F0.
// Restores file row `file`'s pictures to the colours the constructor read
// (field_0xe8 / field_0xfc), group by group as fn_803BC6F0 fades them.
void FileSelect_804AFF60::fn_803BDD44(s32 file) {
    ((J2DPicture*)mScreenHuuin->search(lbl_80476BA8[file]))->setBlackWhite(field_0xfc[0], field_0xe8[0]);

    JUtility::TColor black1 = field_0xfc[1];
    JUtility::TColor white1 = field_0xe8[1];
    ((J2DPicture*)mScreenHuuin->search(lbl_80476B90[file]))->setBlackWhite(black1, white1);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476B78[file]))->setBlackWhite(black1, white1);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476B60[file]))->setBlackWhite(black1, white1);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476B48[file]))->setBlackWhite(black1, white1);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476B30[file]))
        ->setBlackWhite(JUtility::TColor(black1.r, black1.g, black1.b, 0xFF), white1);

    JUtility::TColor black2 = field_0xfc[2];
    JUtility::TColor white2 = field_0xe8[2];
    ((J2DPicture*)mScreenHuuin->search(lbl_80476B18[file]))->setBlackWhite(black2, white2);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476B00[file]))->setBlackWhite(black2, white2);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476AE8[file]))->setBlackWhite(black2, white2);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476AD0[file]))->setBlackWhite(black2, white2);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476AB8[file]))->setBlackWhite(black2, white2);

    JUtility::TColor black3 = field_0xfc[3];
    JUtility::TColor white3 = field_0xe8[3];
    ((J2DPicture*)mScreenHuuin->search(lbl_80476C98[file]))->setBlackWhite(black3, white3);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476C80[file]))->setBlackWhite(black3, white3);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476C68[file]))->setBlackWhite(black3, white3);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476C50[file]))->setBlackWhite(black3, white3);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476C38[file]))
        ->setBlackWhite(JUtility::TColor(black3.r, black3.g, black3.b, 0xFF), white3);

    JUtility::TColor black4 = field_0xfc[4];
    JUtility::TColor white4 = field_0xe8[4];
    ((J2DPicture*)mScreenHuuin->search(lbl_80476C20[file]))->setBlackWhite(black4, white4);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476C08[file]))->setBlackWhite(black4, white4);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476BF0[file]))->setBlackWhite(black4, white4);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476BD8[file]))->setBlackWhite(black4, white4);
}

// FIXME: 99.65. Stack slots only: the original puts white0 at 0xD0 and its
// by-value copy at 0xD4 (black0 0xD8 / 0xDC), ours white0 at 0xDC.
// black1 / black3 are copy-initialised: the original stores a second copy of
// each (0xCC, 0xB8) before the group's first call and rebuilds the last
// call's opaque black from it; white and the other groups have no copy.
// Gives file row `file`'s pictures their highlighted colours, the ones
// fn_803BC6F0 fades them to at alpha 1 (group by group, the same panes as
// fn_803BDD44).
void FileSelect_804AFF60::fn_803BD5E0(s32 file) {
    JUtility::TColor white0(0x80, 0x80, 0x80, 0xFF);
    JUtility::TColor black0(0x00, 0x00, 0x00, 0x00);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476BA8[file]))->setBlackWhite(black0, white0);

    JUtility::TColor white1(0xB4, 0xB4, 0xB4, 0xFF);
    JUtility::TColor black1 = JUtility::TColor(0x82, 0x82, 0x82, 0x00);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476B90[file]))->setBlackWhite(black1, white1);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476B78[file]))->setBlackWhite(black1, white1);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476B60[file]))->setBlackWhite(black1, white1);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476B48[file]))->setBlackWhite(black1, white1);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476B30[file]))
        ->setBlackWhite(JUtility::TColor(black1.r, black1.g, black1.b, 0xFF), white1);

    JUtility::TColor white2(0x80, 0x80, 0x80, 0xFF);
    JUtility::TColor black2(0x50, 0x50, 0x50, 0x00);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476B18[file]))->setBlackWhite(black2, white2);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476B00[file]))->setBlackWhite(black2, white2);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476AE8[file]))->setBlackWhite(black2, white2);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476AD0[file]))->setBlackWhite(black2, white2);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476AB8[file]))->setBlackWhite(black2, white2);

    JUtility::TColor white3(0x82, 0x82, 0x82, 0xFF);
    JUtility::TColor black3 = JUtility::TColor(0x64, 0x64, 0x64, 0x00);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476C98[file]))->setBlackWhite(black3, white3);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476C80[file]))->setBlackWhite(black3, white3);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476C68[file]))->setBlackWhite(black3, white3);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476C50[file]))->setBlackWhite(black3, white3);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476C38[file]))
        ->setBlackWhite(JUtility::TColor(black3.r, black3.g, black3.b, 0xFF), white3);

    JUtility::TColor white4(0xB4, 0xB4, 0xB4, 0xFF);
    JUtility::TColor black4(0xB4, 0xB4, 0xB4, 0x00);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476C20[file]))->setBlackWhite(black4, white4);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476C08[file]))->setBlackWhite(black4, white4);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476BF0[file]))->setBlackWhite(black4, white4);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476BD8[file]))->setBlackWhite(black4, white4);
}

// FIXME: 98.65 %. 8 instructions missing: in groups 1-4 the original copies
// black / white into the group copies (sp 0xE4 / 0xE0 ...) through a second
// temporary (sp 0x20 / 0x24 ...: one extra word store each), which plain
// copies don't produce; stack offsets shift from there. The .sdata2 literals
// are @NNN (the pool's leading 0, -200, 200 come from no written function).
// Fades file `file`'s row pictures between their colours and grey:
// alpha 0 keeps the colours read by the constructor, alpha 1 sets each
// group's black and white to fixed grey levels.
void FileSelect_804AFF60::fn_803BC6F0(s32 file, f32 alpha) {
    if (alpha > 1.0f) {
        alpha = 1.0f;
    }
    if (alpha < 0.0f) {
        alpha = 0.0f;
    }
    JUtility::TColor black;
    JUtility::TColor white;

    black.r = field_0xfc[0].r + (0 - field_0xfc[0].r) * alpha;
    black.g = field_0xfc[0].g + (0 - field_0xfc[0].g) * alpha;
    black.b = field_0xfc[0].b + (0 - field_0xfc[0].b) * alpha;
    black.a = 0;
    white.r = field_0xe8[0].r + (0x80 - field_0xe8[0].r) * alpha;
    white.g = field_0xe8[0].g + (0x80 - field_0xe8[0].g) * alpha;
    white.b = field_0xe8[0].b + (0x80 - field_0xe8[0].b) * alpha;
    white.a = 0xFF;
    ((J2DPicture*)mScreenHuuin->search(lbl_80476BA8[file]))->setBlackWhite(black, white);

    black.r = field_0xfc[1].r + (0x82 - field_0xfc[1].r) * alpha;
    black.g = field_0xfc[1].g + (0x82 - field_0xfc[1].g) * alpha;
    black.b = field_0xfc[1].b + (0x82 - field_0xfc[1].b) * alpha;
    black.a = 0;
    white.r = field_0xe8[1].r + (0xB4 - field_0xe8[1].r) * alpha;
    white.g = field_0xe8[1].g + (0xB4 - field_0xe8[1].g) * alpha;
    white.b = field_0xe8[1].b + (0xB4 - field_0xe8[1].b) * alpha;
    white.a = 0xFF;
    // The original copies the group colours through a per-group temporary (sp 0xE4 / 0xE0 ...).
    JUtility::TColor black1 = black;
    JUtility::TColor white1 = white;
    ((J2DPicture*)mScreenHuuin->search(lbl_80476B90[file]))->setBlackWhite(black1, white1);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476B78[file]))->setBlackWhite(black1, white1);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476B60[file]))->setBlackWhite(black1, white1);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476B48[file]))->setBlackWhite(black1, white1);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476B30[file]))
        ->setBlackWhite(JUtility::TColor(black1.r, black1.g, black1.b, 0xFF), white1);

    black.r = field_0xfc[2].r + (0x50 - field_0xfc[2].r) * alpha;
    black.g = field_0xfc[2].g + (0x50 - field_0xfc[2].g) * alpha;
    black.b = field_0xfc[2].b + (0x50 - field_0xfc[2].b) * alpha;
    black.a = 0;
    white.r = field_0xe8[2].r + (0x80 - field_0xe8[2].r) * alpha;
    white.g = field_0xe8[2].g + (0x80 - field_0xe8[2].g) * alpha;
    white.b = field_0xe8[2].b + (0x80 - field_0xe8[2].b) * alpha;
    white.a = 0xFF;
    // The original copies the group colours through a per-group temporary (sp 0xE4 / 0xE0 ...).
    JUtility::TColor black2 = black;
    JUtility::TColor white2 = white;
    ((J2DPicture*)mScreenHuuin->search(lbl_80476B18[file]))->setBlackWhite(black2, white2);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476B00[file]))->setBlackWhite(black2, white2);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476AE8[file]))->setBlackWhite(black2, white2);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476AD0[file]))->setBlackWhite(black2, white2);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476AB8[file]))->setBlackWhite(black2, white2);

    black.r = field_0xfc[3].r + (0x82 - field_0xfc[3].r) * alpha;
    black.g = field_0xfc[3].g + (0x82 - field_0xfc[3].g) * alpha;
    black.b = field_0xfc[3].b + (0x82 - field_0xfc[3].b) * alpha;
    black.a = 0;
    white.r = field_0xe8[3].r + (0xB4 - field_0xe8[3].r) * alpha;
    white.g = field_0xe8[3].g + (0xB4 - field_0xe8[3].g) * alpha;
    white.b = field_0xe8[3].b + (0xB4 - field_0xe8[3].b) * alpha;
    white.a = 0xFF;
    // The original copies the group colours through a per-group temporary (sp 0xE4 / 0xE0 ...).
    JUtility::TColor black3 = black;
    JUtility::TColor white3 = white;
    ((J2DPicture*)mScreenHuuin->search(lbl_80476C98[file]))->setBlackWhite(black3, white3);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476C80[file]))->setBlackWhite(black3, white3);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476C68[file]))->setBlackWhite(black3, white3);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476C50[file]))->setBlackWhite(black3, white3);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476C38[file]))
        ->setBlackWhite(JUtility::TColor(black3.r, black3.g, black3.b, 0xFF), white3);

    black.r = field_0xfc[4].r + (0xB4 - field_0xfc[4].r) * alpha;
    black.g = field_0xfc[4].g + (0xB4 - field_0xfc[4].g) * alpha;
    black.b = field_0xfc[4].b + (0xB4 - field_0xfc[4].b) * alpha;
    black.a = 0;
    white.r = field_0xe8[4].r + (0xB4 - field_0xe8[4].r) * alpha;
    white.g = field_0xe8[4].g + (0xB4 - field_0xe8[4].g) * alpha;
    white.b = field_0xe8[4].b + (0xB4 - field_0xe8[4].b) * alpha;
    white.a = 0xFF;
    // The original copies the group colours through a per-group temporary (sp 0xE4 / 0xE0 ...).
    JUtility::TColor black4 = black;
    JUtility::TColor white4 = white;
    ((J2DPicture*)mScreenHuuin->search(lbl_80476C20[file]))->setBlackWhite(black4, white4);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476C08[file]))->setBlackWhite(black4, white4);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476BF0[file]))->setBlackWhite(black4, white4);
    ((J2DPicture*)mScreenHuuin->search(lbl_80476BD8[file]))->setBlackWhite(black4, white4);
}
