#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/save_data_803B8C74.h"
#include "game/Map.h"
#include "game/Message.h"
#include "game/SpriteParams.h"
#include "game/act/button_icons_80440BF4.h"
#include "game/file_select_803BC6F0.h"
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
// Message ids (refreshFileRows_803CE4CC).
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

// The centre of mScreen's pane `tag` (as connect_screen_803F0A54's PANE_CENTRE_X / _Y):
// the original searches the pane again for the bounds after the size.
#define PANE_CENTRE_X(dst, screen, tag)                                        \
    do {                                                                   \
        f32 size = (screen)->search(tag)->getWidth();                      \
        dst = size / 2.0f + (screen)->search(tag)->getGlobalBounds().i.x; \
    } while (0)
#define PANE_CENTRE_Y(dst, screen, tag)                                        \
    do {                                                                   \
        f32 size = (screen)->search(tag)->getHeight();                     \
        dst = size / 2.0f + (screen)->search(tag)->getGlobalBounds().i.y; \
    } while (0)

// The three loops that give a pane group back the alphas saved in the constructor (field_0x196,
// field_0x132, field_0x183), through the group's control. They stood in this file 40 times, written
// out; the original has them as inline functions: its loop counter is in the first register and the
// table pointer the compiler makes in the next, which is the order of a counter created at inlining
// (docs/REGALLOC.md, "the FileSelect pane loops"). Provisional names (always inlined), after their
// tag tables.
static inline void restoreAlphas_80476938(FileSelect_804AFF60* self) {
    for (s32 i = 0; i < 9; i++) {
        self->field_0x20->setAlpha_804023D0(self->mScreen->search(lbl_80476938[i]), self->field_0x196[i]);
    }
}

static inline void restoreAlphas_80476618(FileSelect_804AFF60* self) {
    for (s32 i = 0; i < 81; i++) {
        self->field_0x14->setAlpha_804023D0(self->mScreen->search(lbl_80476618[i]), self->field_0x132[i]);
    }
}

static inline void restoreAlphas_804768A0(FileSelect_804AFF60* self) {
    for (s32 i = 0; i < 19; i++) {
        self->field_0x14->setAlpha_804023D0(self->mScreenHuuin->search(lbl_804768A0[i]), self->field_0x183[i]);
    }
}

// The four pane alphas of button i, back to the kept values (field_0x19f).
static inline void restoreButtonAlphas(FileSelect_804AFF60* self, s32 i) {
    switch (i) {
    case 0:
        for (s32 j = 0; j < 4; j++) {
            self->field_0x24[i]->setAlpha_804023D0(self->mScreenHuuin->search(lbl_80476DC8[j]), self->field_0x19f[j]);
        }
        break;
    case 1:
        for (s32 j = 0; j < 4; j++) {
            self->field_0x24[i]->setAlpha_804023D0(self->mScreenHuuin->search(lbl_80476DA8[j]), self->field_0x19f[j]);
        }
        break;
    case 2:
        for (s32 j = 0; j < 4; j++) {
            self->field_0x24[i]->setAlpha_804023D0(self->mScreenHuuin->search(lbl_80476D88[j]), self->field_0x19f[j]);
        }
        break;
    case 3:
        for (s32 j = 0; j < 4; j++) {
            self->field_0x24[i]->setAlpha_804023D0(self->mScreenHuuin->search(lbl_80476D68[j]), self->field_0x19f[j]);
        }
        break;
    }
}

// A point of the 640 x 480 layout on the 360 x 270 sprite layer of the
// current render mode.
static inline f32 toSpriteX(f32 x) {
    return 360.0f * x / (s32)JUTVideo::getManager()->getRenderMode()->fbWidth;
}

static inline f32 toSpriteY(f32 y) {
    return 270.0f * y / (s32)JUTVideo::getManager()->getRenderMode()->efbHeight;
}

// Makes that 360 x 270 sprite layer the ortho projection (before each of
// fn_803BE3D8's sprite draws; provisional name, always inlined).
static inline void setSpriteOrtho() {
    JGeometry::TBox2f box(0.0f, 0.0f, 360.0f, 270.0f);
    gGameRoot2_8053AB10->fn_8013B414(&box, true);
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

// The copy / delete buttons get their saved black and white back. Inlined in
// fn_803CABAC: the eight by-value colour copies take their stack slots from
// sp 0x8 upwards there, the order of an inlined body (the callers that spell
// the four calls out have them from the top downwards).
inline void FileSelect_804AFF60::resetButtonColors() {
    ((J2DPicture*)mScreenHuuin->search('Pcopb01'))->setBlackWhite(field_0xe4, field_0xe0);
    ((J2DPicture*)mScreenHuuin->search('Pcopb02'))->setBlackWhite(field_0xe4, field_0xe0);
    ((J2DPicture*)mScreenHuuin->search('Pdelb01'))->setBlackWhite(field_0xe4, field_0xe0);
    ((J2DPicture*)mScreenHuuin->search('Pdelb02'))->setBlackWhite(field_0xe4, field_0xe0);
}

static void pool_80543458(FileSelect_804AFF60* self, s16 frames);

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

// Colour file row `file`'s picture groups 1-4 (the groups fadeFileRow_803BC6F0 fades);
// groups 1 and 3 end in a picture whose black is made opaque.
static inline void setGroup1Colors(FileSelect_804AFF60* self, s32 file, JUtility::TColor black, JUtility::TColor white) {
    ((J2DPicture*)self->mScreenHuuin->search(lbl_80476B90[file]))->setBlackWhite(black, white);
    ((J2DPicture*)self->mScreenHuuin->search(lbl_80476B78[file]))->setBlackWhite(black, white);
    ((J2DPicture*)self->mScreenHuuin->search(lbl_80476B60[file]))->setBlackWhite(black, white);
    ((J2DPicture*)self->mScreenHuuin->search(lbl_80476B48[file]))->setBlackWhite(black, white);
    ((J2DPicture*)self->mScreenHuuin->search(lbl_80476B30[file]))
        ->setBlackWhite(JUtility::TColor(black.r, black.g, black.b, 0xFF), white);
}

static inline void setGroup2Colors(FileSelect_804AFF60* self, s32 file, JUtility::TColor black, JUtility::TColor white) {
    ((J2DPicture*)self->mScreenHuuin->search(lbl_80476B18[file]))->setBlackWhite(black, white);
    ((J2DPicture*)self->mScreenHuuin->search(lbl_80476B00[file]))->setBlackWhite(black, white);
    ((J2DPicture*)self->mScreenHuuin->search(lbl_80476AE8[file]))->setBlackWhite(black, white);
    ((J2DPicture*)self->mScreenHuuin->search(lbl_80476AD0[file]))->setBlackWhite(black, white);
    ((J2DPicture*)self->mScreenHuuin->search(lbl_80476AB8[file]))->setBlackWhite(black, white);
}

static inline void setGroup3Colors(FileSelect_804AFF60* self, s32 file, JUtility::TColor black, JUtility::TColor white) {
    ((J2DPicture*)self->mScreenHuuin->search(lbl_80476C98[file]))->setBlackWhite(black, white);
    ((J2DPicture*)self->mScreenHuuin->search(lbl_80476C80[file]))->setBlackWhite(black, white);
    ((J2DPicture*)self->mScreenHuuin->search(lbl_80476C68[file]))->setBlackWhite(black, white);
    ((J2DPicture*)self->mScreenHuuin->search(lbl_80476C50[file]))->setBlackWhite(black, white);
    ((J2DPicture*)self->mScreenHuuin->search(lbl_80476C38[file]))
        ->setBlackWhite(JUtility::TColor(black.r, black.g, black.b, 0xFF), white);
}

static inline void setGroup4Colors(FileSelect_804AFF60* self, s32 file, JUtility::TColor black, JUtility::TColor white) {
    ((J2DPicture*)self->mScreenHuuin->search(lbl_80476C20[file]))->setBlackWhite(black, white);
    ((J2DPicture*)self->mScreenHuuin->search(lbl_80476C08[file]))->setBlackWhite(black, white);
    ((J2DPicture*)self->mScreenHuuin->search(lbl_80476BF0[file]))->setBlackWhite(black, white);
    ((J2DPicture*)self->mScreenHuuin->search(lbl_80476BD8[file]))->setBlackWhite(black, white);
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

// FIXME: 99.92 %, regalloc-only; behaviour clean.
// Regalloc only: the font pointer (0x154) loaded for each
// text pane lands in r26/r27 where the original uses r29.
FileSelect_804AFF60::FileSelect_804AFF60() {
    lbl_80529DEC.mHeap->getTotalFreeSize();
    gGameRoot_8053AAF8->field_0xff = 0;
    for (int i = 0; i < 3; i++) {
        initSaveFile_803B950C(&lbl_8050EC80.mFiles[i], &lbl_8050EC80.mBodies[i]);
    }
    lbl_80529DEC.fn_80403680();
    if (!lbl_80529DEC.field_0x242) {
        lbl_80529DEC.field_0x242 = 1;
        gGameRoot2_8053AB10->field_0x20->fn_802FF798();
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SCH ', "gb_ch_obj_base_sch.szs");
        gGameRoot2_8053AB10->field_0x20->loadSpriteImage_802F9714(res, 0, 0, 0, 0);
        res = gGameRoot_8053AAF8->field_0xa4->getResource('SCH ', "gb_ch_obj_tr_schE.szs");
        gGameRoot2_8053AB10->field_0x20->loadSpriteImage_802F9714(res, 0, 0, 0x7000, 0);
        res = gGameRoot_8053AAF8->field_0xa4->getResource('SCL ', "cu_cl_obj_base_scl.szs");
        gGameRoot2_8053AB10->field_0x20->loadSpritePalette_802F93F4(res, 0, 0, 0, 0, false);
        res = gGameRoot_8053AAF8->field_0xa4->getResource('SCH ', "player_sch.szs");
        gGameRoot2_8053AB10->field_0x20->loadSpriteImage_802F9714(res, 0, 2, 0x4000, 0);
        res = gGameRoot_8053AAF8->field_0xa4->getResource('SCL ', "cu_cl_obj_base_scl.szs");
        gGameRoot2_8053AB10->field_0x20->loadSpritePalette_802F93F4(res, 0, 2, 0, 0, false);
        res = gGameRoot_8053AAF8->field_0xa4->getResource('SCH ', "gals_sch.szs");
        gGameRoot2_8053AB10->field_0x20->loadSpriteImage_802F9714(res, 0, 3, 0, 0);
        res = gGameRoot_8053AAF8->field_0xa4->getResource('SCL ', "cu_cl_obj_base_scl.szs");
        gGameRoot2_8053AB10->field_0x20->loadSpritePalette_802F93F4(res, 0, 3, 0, 0, false);
        gGameRoot2_8053AB10->field_0x20->mDrawSprites_0x2364 = 1;
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
    mScreen->setPriority("file_select.blo", 0x1100000, lbl_80529DEC.mFileArchive_0x30);
    mScreenHuuin = new J2DScreen;
    mScreenHuuin->setPriority("file_select_huuin.blo", 0x1100000, lbl_80529DEC.mFileArchive_0x30);
    mScreenHuuin->search('Paccur02')->setAlpha(0x9C);
    mScreenHuuin->search('Pfcur05')->setAlpha(0x9C);

    const ResTIMG* img = static_cast<const ResTIMG*>(getMain2DResource_804030C4('TIMG', "kk_black.bti"));
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
    lbl_8053AED8->fn_80440D78(static_cast<const ResTIMG*>(getMain2DResource_804030C4('TIMG', "kk_kettei_01.bti")));
    lbl_8053AED8->fn_80440CE4(static_cast<const ResTIMG*>(getMain2DResource_804030C4('TIMG', "kk_modoru_01.bti")));

    if (gGameRoot_8053AAF8->field_0x7f) {
        mScreenHuuin->search('Pbli04')->hide();
    } else {
        mScreenHuuin->search('Pbli14')->hide();
    }

    JUTResFont* font = lbl_8053AD78->getFont();
    ((J2DTextBox*)mScreen->search('Tdb01'))->setFont(font);
    font = lbl_8053AD78->getFont();
    ((J2DTextBox*)mScreen->search('Tbat01'))->setFont(font);
    font = lbl_8053AD78->getFont();
    ((J2DTextBox*)mScreenHuuin->search('Tadv01'))->setFont(font);
    font = lbl_8053AD78->getFont();
    ((J2DTextBox*)mScreenHuuin->search('Tbac01'))->setFont(font);
    font = lbl_8053AD78->getFont();
    ((J2DTextBox*)mScreenHuuin->search('Tdel01'))->setFont(font);
    font = lbl_8053AD78->getFont();
    ((J2DTextBox*)mScreenHuuin->search('Tcop01'))->setFont(font);
    font = lbl_8053AD78->getFont();
    ((J2DTextBox*)mScreenHuuin->search('Tstr01'))->setFont(font);
    font = lbl_8053AD78->getFont();
    ((J2DTextBox*)mScreenHuuin->search('Tf3date'))->setFont(font);
    font = lbl_8053AD78->getFont();
    ((J2DTextBox*)mScreenHuuin->search('Tf3lv'))->setFont(font);
    font = lbl_8053AD78->getFont();
    ((J2DTextBox*)mScreenHuuin->search('Tf3fir'))->setFont(font);
    font = lbl_8053AD78->getFont();
    ((J2DTextBox*)mScreenHuuin->search('Tf2date'))->setFont(font);
    font = lbl_8053AD78->getFont();
    ((J2DTextBox*)mScreenHuuin->search('Tf2lv'))->setFont(font);
    font = lbl_8053AD78->getFont();
    ((J2DTextBox*)mScreenHuuin->search('Tf2fir'))->setFont(font);
    font = lbl_8053AD78->getFont();
    ((J2DTextBox*)mScreenHuuin->search('Tf1date'))->setFont(font);
    font = lbl_8053AD78->getFont();
    ((J2DTextBox*)mScreenHuuin->search('Tf1lv'))->setFont(font);
    font = lbl_8053AD78->getFont();
    ((J2DTextBox*)mScreenHuuin->search('Tf1fir'))->setFont(font);
    font = lbl_8053AD78->getFont();
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
        gAudioMgr_8053AF80->startBgm_80458FF0(0x100001A, false, false);
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
    lbl_80529DEC.mFileArchive_0x30->unmount();
    lbl_8053ADB0 = NULL;
}

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
    gGameRoot2_8053AB10->setPort_8013B490();
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
    lbl_8053AED8->draw_80440EE8();
}

void FileSelect_804AFF60::fn_803CE9EC() {
    field_0x1a8 = 3;
    if (field_0x116 == 1) {
        lbl_8053AEC0->fn_80431174(field_0x114);
    }
    CardMgr_804AFB48* card = &lbl_8051C1A0;
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
            pool_80543458(this, frames);
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
            pool_80543458(this, frames);
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
// finished, or the last one's when all are done. refreshFileRows_803CE4CC inlines it three
// times; the linker dropped its out-of-line copy.
// R2 pool helper (inlined at 3 call sites); was getProgressMsg.
static s32 inl_803CE4CC(s32 file) {
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

// Refreshes the three file rows: a file whose checksum fails gets message
// 0x11D and its bit in field_0x115 (fn_803BD5E0); a used file shows the date
// it was chosen last and every file's progress; an empty file (or no card)
// shows message 0x6A.
void FileSelect_804AFF60::refreshFileRows_803CE4CC() {
    for (s32 i = 0; i < 3; i++) {
        clearBadChecksum(i);
        // (s32): the original tests field_0xff signed (lbz + cmpwi), as in fn_803CABAC.
        if ((s32)gGameRoot_8053AAF8->field_0xff != 0) {
            if (isSaveChecksumValid_803BADAC(&lbl_8050EC80.getFile(i))) {
                if (lbl_8050EC80.getFile(i).isUsed()) {
                    OSCalendarTime time;
                    OSTicksToCalendarTime(lbl_8050EC80.getFile(i).field_0x0, &time);
                    snprintf(((J2DTextBox*)mScreenHuuin->search(lbl_80476600[i]))->getStringPtr(), 30,
                             "%02d.%02d.%04d %02d:%02d:%02d", time.mon + 1, time.mday, time.year, time.hour,
                             time.min, time.sec);
                    mScreenHuuin->search(lbl_804765D0[i])->show();
                    mScreenHuuin->search(lbl_804765E8[i])->hide();
                    mScreenHuuin->search(lbl_80476600[i])->show();
                    getMessageText_803832BC(((J2DTextBox*)mScreenHuuin->search('Tf1lv'))->getStringPtr(),
                                            inl_803CE4CC(0), 0, NULL, NULL);
                    getMessageText_803832BC(((J2DTextBox*)mScreenHuuin->search('Tf2lv'))->getStringPtr(),
                                            inl_803CE4CC(1), 0, NULL, NULL);
                    getMessageText_803832BC(((J2DTextBox*)mScreenHuuin->search('Tf3lv'))->getStringPtr(),
                                            inl_803CE4CC(2), 0, NULL, NULL);
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

// FIXME: 99.75 %, regalloc-only; behaviour clean.
// The bounds x and field_0x8 load into f2 / f1 (original f1 / f2)
// and the fsubs operands swap.
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
        gAudioMgr_8053AF80->playSystemSound_80458880(0xB, -1);
    }
}

// FIXME: 99.87 %, regalloc-only; behaviour clean.
// As in fn_803CE370, the bounds x and field_0x8 load into
// f1 / f0 (original f0 / f1), and the loop's counter and table pointer swap
// r29 / r30.
void FileSelect_804AFF60::fn_803CE11C() {
    u32 cardState = lbl_8051C1A0.fn_803B9940(0);
    if (field_0x20->mPane->getBounds()->i.x - field_0x20->field_0x8 < 600.0f) {
        field_0x20->slideX_80402B10(lbl_8050DB0C.field_0x4c[0xC3], 0, 0.0f, 600.0f);
        field_0x20->animateAlpha_80402628(lbl_8050DB0C.field_0x4c[0xC3], field_0x20->field_0x28, 0);
        restoreAlphas_80476938(this);
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
        gAudioMgr_8053AF80->playSystemSound_80458880(0xB, -1);
    }
}

// State 3, the "start a new game?" prompt (field_0x114: 0 yes, 1 no): left /
// right move the cursor; A on yes clears the save data and shows the file
// list (fn_803C24BC), A on no goes to state 1.
void FileSelect_804AFF60::fn_803CDEE8() {
    if (lbl_8053AD30->fn_80309C4C(1, 1) != 0) {
        if (field_0x114 != 0) {
            field_0x114--;
            gAudioMgr_8053AF80->playSystemSound_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(2, 1) != 0) {
        if (field_0x114 < 1) {
            field_0x114++;
            gAudioMgr_8053AF80->playSystemSound_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(0x100, 1) != 0) {
        if (field_0x114 == 0) {
            field_0x80 = 0;
            gGameRoot_8053AAF8->field_0xff = 0;
            for (s32 i = 0; i < 3; i++) {
                initSaveFile_803B950C(&lbl_8050EC80.mFiles[i], &lbl_8050EC80.mBodies[i]);
            }
            lbl_80529DEC.fn_80403680();
            fn_803C24BC();
            gAudioMgr_8053AF80->playSystemSound_80458880(0xB, -1);
        } else {
            field_0x80 = 0;
            setState(1);
            gAudioMgr_8053AF80->playSystemSound_80458880(0xC, -1);
        }
    }
}

// Registers rotated (`this` in r15 instead of r31) and `frames` loaded into r17
// instead of r4 + mr r16, r4.
// State 4: slides the cursor, the three rows (bottom row first) and the
// board in (the timings are lbl_8050DB0C's 0x84..0x8A), starts the 'kk_'
// sprite fade-in, then goes to state 5.
// FIXME: 96.74 %, scheduling+regalloc; behaviour clean.
void FileSelect_804AFF60::fn_803CD75C() {
    s16 rowDelay = lbl_8050DB0C.field_0x4c[0x84];
    s16 frames = lbl_8050DB0C.field_0x4c[0x85];
    s32 rowStep = lbl_8050DB0C.field_0x4c[0x86];
    s32 rowDuration = frames + rowStep * 2;
    s16 boardDelay = lbl_8050DB0C.field_0x4c[0x87];
    s16 boardFrames = lbl_8050DB0C.field_0x4c[0x88];
    s16 kkDelay = lbl_8050DB0C.field_0x4c[0x89];
    s16 kkFrames = lbl_8050DB0C.field_0x4c[0x8A];
    s32 end = rowDelay + frames;
    if (end < rowDelay + rowDuration) {
        end = rowDelay + rowDuration;
    }
    if (end < boardDelay + boardFrames) {
        end = boardDelay + boardFrames;
    }
    if (end < kkDelay + kkFrames) {
        end = kkDelay + kkFrames;
    }

    field_0xd8++;
    if (field_0xd8 > rowDelay) {
        field_0x50->slideX_80402B10(frames, 0, -300.0f, 0.0f);
    }
    for (s32 i = 0; i < 3; i++) {
        if (field_0xd8 > rowDelay + i * rowStep) {
            field_0x38[2 - i]->animateMoveY_80402940(frames, 0, 300.0f, 0.0f);
            field_0x38[2 - i]->animateAlpha_80402628(frames, 0, field_0x38[2 - i]->field_0x28);
            setRowAlphas(this, 2 - i);
            field_0x44[i]->slideX_80402B10(frames, 0, -300.0f, 0.0f);
            field_0x44[i]->animateAlpha_80402628(frames, 0, field_0x44[i]->field_0x28);
            setTabAlphas(this, i);
        }
    }
    if (field_0xd8 > boardDelay) {
        field_0x54->animateMoveY_80402940(boardFrames, 0, -200.0f, 0.0f);
        field_0x54->animateAlpha_80402628(boardFrames, 0, field_0x54->field_0x28);
    }
    if (field_0xd8 == kkDelay + 1) {
        field_0x1a4 = 1;
    }
    if (field_0xd8 > end) {
        setState(5);
    }
}

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
        gAudioMgr_8053AF80->playSystemSound_80458880(0xB, -1);
    } else if (lbl_8053AD30->fn_80309C4C(0x200, 1) != 0) {
        if (field_0x116 != 0) {
            lbl_8053AEC0->fn_804310BC();
            field_0x1a3 = 0;
        }
        field_0x1a8 = 2;
        gAudioMgr_8053AF80->playSystemSound_80458880(0xC, -1);
    } else if (lbl_8053AD30->fn_80309A9C(4, 1) != 0) {
        gAudioMgr_8053AF80->playSystemSound_80458880(0x26, -1);
        field_0xa4++;
        if (field_0xa4 >= 3) {
            field_0xa4 = 0;
        }
        field_0xda = 1;
        field_0x50->field_0x26 = 0x1E;
    } else if (lbl_8053AD30->fn_80309A9C(8, 1) != 0) {
        gAudioMgr_8053AF80->playSystemSound_80458880(0x26, -1);
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
        field_0x50->pulseAlpha_8040245C(0x3C, 0, field_0x50->field_0x28, 1);
        field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur01'), field_0x50->field_0x28);
        field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur02'), field_0x50->field_0x28);
        field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur03'), field_0x50->field_0x28);
        field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur04'), field_0x50->field_0x28);
    }
}

// FIXME: 99.18 %, regalloc-only; behaviour clean.
// (try.py: different, same length). The &others register, as
// fn_803C2E24 / fn_803C9F80. Every field, call and branch is the original's.
//
// State 6 (the file menu closed, from fn_803C21A8): as fn_803C2E24, the board
// and the four buttons slide out, the other two rows slide back in and the
// chosen row returns to its place (timings lbl_8050DB0C
// field_0x4c[0x8E..0x95]); at the end the copy / delete buttons get their
// colours back, then state 5.
void FileSelect_804AFF60::fn_803CC760() {
    s32 boardDelay = lbl_8050DB0C.field_0x4c[0x8E];
    s32 boardFrames = lbl_8050DB0C.field_0x4c[0x8F];
    s32 buttonDelay = lbl_8050DB0C.field_0x4c[0x90];
    s32 buttonFrames = lbl_8050DB0C.field_0x4c[0x91];
    s32 buttonStep = lbl_8050DB0C.field_0x4c[0x92];
    s32 rowDelay = lbl_8050DB0C.field_0x4c[0x93];
    s32 frames = lbl_8050DB0C.field_0x4c[0x94];
    s32 rowStep = lbl_8050DB0C.field_0x4c[0x95];
    s32 end = boardDelay + boardFrames;
    if (end < buttonDelay + buttonFrames + buttonStep * 3) {
        end = buttonDelay + buttonFrames + buttonStep * 3;
    }
    if (end < rowDelay + frames + rowStep) {
        end = rowDelay + frames + rowStep;
    }

    // The two files that weren't chosen.
    s32 others[2];
    s32* other = others;
    s32* dst = others;
    for (int i = 0; i < 3; i++) {
        if (i != field_0xa4) {
            *dst++ = i;
        }
    }

    field_0xd8++;
    if (field_0xd8 > boardDelay) {
        field_0x14->slideX_80402B10(boardFrames, 0, 0.0f, 600.0f);
        field_0x14->animateAlpha_80402628(boardFrames, field_0x14->field_0x28, 0);
        field_0x18->slideX_80402B10(boardFrames, 0, 0.0f, 600.0f);
        field_0x18->animateAlpha_80402628(boardFrames, field_0x18->field_0x28, 0);
        field_0x1c->slideX_80402B10(boardFrames, 0, 0.0f, 600.0f);
        field_0x1c->animateAlpha_80402628(boardFrames, field_0x1c->field_0x28, 0);
        restoreAlphas_80476618(this);
        restoreAlphas_804768A0(this);
    }
    for (s32 i = 0; i < 4; i++) {
        if (field_0xd8 > buttonDelay + i * buttonStep) {
            field_0x24[i]->animateMoveY_80402940(buttonFrames, 0, 0.0f, 100.0f);
            field_0x24[i]->animateAlpha_80402628(buttonFrames, field_0x24[i]->field_0x28, 0);
            restoreButtonAlphas(this, i);
        }
    }
    for (int i = 0; i < 2; i++, other++) {
        if (field_0xd8 > rowDelay + i * rowStep) {
            field_0x38[*other]->slideX_80402B10(frames, 0, 600.0f, 0.0f);
            field_0x38[*other]->animateAlpha_80402628(frames, 0, field_0x38[*other]->field_0x28);
            setRowAlphas(this, *other);
            field_0x44[*other]->slideX_80402B10(frames, 0, 600.0f, 0.0f);
            field_0x44[*other]->animateAlpha_80402628(frames, 0, field_0x44[*other]->field_0x28);
            setTabAlphas(this, *other);
        }
    }
    // The chosen row moves back down from the first row's place.
    if (field_0xa4 != 0 && field_0xd8 > rowDelay) {
        field_0x38[field_0xa4]->animateMoveY_80402940(frames, 0, field_0x38[0]->field_0xc - field_0x38[field_0xa4]->field_0xc,
                                            0.0f);
        field_0x44[field_0xa4]->animateMoveY_80402940(frames, 0, field_0x44[0]->field_0xc - field_0x44[field_0xa4]->field_0xc,
                                            0.0f);
    }
    if (field_0xd8 > end) {
        ((J2DPicture*)mScreenHuuin->search('Pcopb01'))->setBlackWhite(field_0xe4, field_0xe0);
        ((J2DPicture*)mScreenHuuin->search('Pcopb02'))->setBlackWhite(field_0xe4, field_0xe0);
        ((J2DPicture*)mScreenHuuin->search('Pdelb01'))->setBlackWhite(field_0xe4, field_0xe0);
        ((J2DPicture*)mScreenHuuin->search('Pdelb02'))->setBlackWhite(field_0xe4, field_0xe0);
        setState(5);
    }
}

void FileSelect_804AFF60::fn_803CC62C() {
    if (lbl_8053AD30->fn_80309C4C(1, 1) != 0) {
        if (field_0x114 != 0) {
            field_0x114--;
            gAudioMgr_8053AF80->playSystemSound_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(2, 1) != 0) {
        if (field_0x114 < 1) {
            field_0x114++;
            gAudioMgr_8053AF80->playSystemSound_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(0x100, 1) != 0) {
        if (field_0x114 == 0) {
            field_0x80 = 0x59;
            setState(8);
            field_0x113 = 1;
            lbl_80529DEC.field_0x24c = 1;
            gAudioMgr_8053AF80->playSystemSound_80458880(0xB, -1);
        } else {
            field_0x80 = 0x58;
            setState(2);
            gAudioMgr_8053AF80->playSystemSound_80458880(0xC, -1);
        }
    }
}

// State 8: copies the three files into field_0x5c / field_0x60, gives each
// its checksum, hands both to the memory card manager to write (state 9 waits
// for it) and keeps the card's serial number.
void FileSelect_804AFF60::fn_803CC53C() {
    lbl_8050EC80.fn_803B93E8(field_0x5c, field_0x60, 0);
    writeSaveChecksum_803BACF8(field_0x5c, 0);
    lbl_8050EC80.fn_803B93E8(field_0x5c, field_0x60, 1);
    writeSaveChecksum_803BACF8(field_0x5c, 1);
    lbl_8050EC80.fn_803B93E8(field_0x5c, field_0x60, 2);
    writeSaveChecksum_803BACF8(field_0x5c, 2);
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
        refreshFileRows_803CE4CC();
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
        gAudioMgr_8053AF80->playSystemSound_80458880(0xB, -1);
        lbl_8050EC80.field_0x8 = lbl_8051C1A0.field_0x1ff8;
    }
}

void FileSelect_804AFF60::fn_803CC2BC() {
    if (lbl_8053AD30->fn_80309C4C(1, 1) != 0) {
        if (field_0x114 != 0) {
            field_0x114--;
            gAudioMgr_8053AF80->playSystemSound_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(2, 1) != 0) {
        if (field_0x114 < 1) {
            field_0x114++;
            gAudioMgr_8053AF80->playSystemSound_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(0x100, 1) != 0) {
        if (field_0x114 == 0) {
            field_0x80 = 0x53;
            setState(0xC);
            gAudioMgr_8053AF80->playSystemSound_80458880(0xB, -1);
        } else {
            field_0x80 = 0x52;
            setState(2);
            gAudioMgr_8053AF80->playSystemSound_80458880(0xC, -1);
        }
    }
}

void FileSelect_804AFF60::fn_803CC17C() {
    if (lbl_8053AD30->fn_80309C4C(1, 1) != 0) {
        if (field_0x114 != 0) {
            field_0x114--;
            gAudioMgr_8053AF80->playSystemSound_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(2, 1) != 0) {
        if (field_0x114 < 1) {
            field_0x114++;
            gAudioMgr_8053AF80->playSystemSound_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(0x100, 1) != 0) {
        if (field_0x114 == 0) {
            lbl_8051C1A0.fn_803B9DDC();
            field_0x80 = 0x54;
            setState(0xD);
            field_0x113 = 1;
            lbl_80529DEC.field_0x24c = 1;
            gAudioMgr_8053AF80->playSystemSound_80458880(0xB, -1);
        } else {
            field_0x80 = 0x52;
            setState(2);
            gAudioMgr_8053AF80->playSystemSound_80458880(0xC, -1);
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
        gAudioMgr_8053AF80->playSystemSound_80458880(0xB, -1);
    }
}

void FileSelect_804AFF60::fn_803CBF0C() {
    if (lbl_8053AD30->fn_80309C4C(1, 1) != 0) {
        if (field_0x114 != 0) {
            field_0x114--;
            gAudioMgr_8053AF80->playSystemSound_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(2, 1) != 0) {
        if (field_0x114 < 1) {
            field_0x114++;
            gAudioMgr_8053AF80->playSystemSound_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(0x100, 1) != 0) {
        if (field_0x114 == 0) {
            field_0x80 = 0;
            setState(0x10);
            gAudioMgr_8053AF80->playSystemSound_80458880(0xB, -1);
        } else {
            if (gGameRoot_8053AAF8->field_0x7f != 0) {
                field_0x80 = 0x5C;
            } else {
                field_0x80 = 0x256;
            }
            setState(3);
            gAudioMgr_8053AF80->playSystemSound_80458880(0xC, -1);
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

// Registers only: `frames` and two temporaries of the inlined alpha helpers
// are rotated (r26/r27/r28), and the `field_0xc` subtraction swaps f0/f2.
// Slides the other two file rows out and the command panes in, one after
// another (the timings are lbl_8050DB0C's 0x98..0x9F), then goes to state 19.
// FIXME: 99.47 %, regalloc-only; behaviour clean.
void FileSelect_804AFF60::fn_803CB3DC() {
    s16 rowDelay = lbl_8050DB0C.field_0x4c[0x98];
    s16 frames = lbl_8050DB0C.field_0x4c[0x99];
    s16 rowStep = lbl_8050DB0C.field_0x4c[0x9A];
    s16 boardDelay = lbl_8050DB0C.field_0x4c[0x9B];
    s16 boardFrames = lbl_8050DB0C.field_0x4c[0x9C];
    s16 buttonDelay = lbl_8050DB0C.field_0x4c[0x9D];
    s16 buttonFrames = lbl_8050DB0C.field_0x4c[0x9E];
    s16 buttonStep = lbl_8050DB0C.field_0x4c[0x9F];
    s32 end = rowDelay + frames + rowStep;
    if (end < boardDelay + boardFrames) {
        end = boardDelay + boardFrames;
    }
    if (end < buttonDelay + buttonFrames + buttonStep * 3) {
        end = buttonDelay + buttonFrames + buttonStep * 3;
    }

    // The two files that weren't chosen.
    s32 others[2];
    s32* other = others;
    s32* dst = others;
    for (s32 i = 0; i < 3; i++) {
        if (i != field_0xa4) {
            *dst++ = i;
        }
    }

    field_0xd8++;
    for (int i = 0; i < 2; i++, other++) {
        if (field_0xd8 > rowDelay + i * rowStep) {
            field_0x38[*other]->slideX_80402B10(frames, 0, 0.0f, 600.0f);
            field_0x38[*other]->animateAlpha_80402628(frames, field_0x38[*other]->field_0x28, 0);
            setRowAlphas(this, *other);
            field_0x44[*other]->slideX_80402B10(frames, 0, 0.0f, 600.0f);
            field_0x44[*other]->animateAlpha_80402628(frames, field_0x44[*other]->field_0x28, 0);
            setTabAlphas(this, *other);
        }
    }
    // The chosen row moves up to the first row's place.
    if (field_0xa4 != 0 && field_0xd8 > rowDelay) {
        field_0x38[field_0xa4]->animateMoveY_80402940(frames, 0, 0.0f,
                                            field_0x38[0]->getY() - field_0x38[field_0xa4]->getY());
        field_0x44[field_0xa4]->animateMoveY_80402940(frames, 0, 0.0f,
                                            field_0x44[0]->getY() - field_0x44[field_0xa4]->getY());
    }
    if (field_0xd8 > boardDelay) {
        field_0x14->slideX_80402B10(boardFrames, 0, 600.0f, 0.0f);
        field_0x14->animateAlpha_80402628(boardFrames, 0, field_0x14->field_0x28);
        field_0x18->slideX_80402B10(boardFrames, 0, 600.0f, 0.0f);
        field_0x18->animateAlpha_80402628(boardFrames, 0, field_0x18->field_0x28);
        field_0x1c->slideX_80402B10(boardFrames, 0, 600.0f, 0.0f);
        field_0x1c->animateAlpha_80402628(boardFrames, 0, field_0x1c->field_0x28);
        restoreAlphas_80476618(this);
        restoreAlphas_804768A0(this);
    }
    for (s32 i = 0; i < 4; i++) {
        if (field_0xd8 > buttonDelay + i * buttonStep) {
            field_0x24[i]->animateMoveY_80402940(buttonFrames, 0, 100.0f, 0.0f);
            field_0x24[i]->animateAlpha_80402628(buttonFrames, 0, field_0x24[i]->field_0x28);
            restoreButtonAlphas(this, i);
        }
    }
    if (field_0xd8 > end) {
        setState(0x13);
    }
}

// The file menu (state 19): A runs the highlighted command field_0xa0 (0:
// start the chosen file, 1: copy, 2: delete, 3: back), B goes back, up /
// down move the cursor (field_0xa0, skipping commands an empty file lacks);
// the cursor pane field_0x34 bobs for ten frames after a move.
// field_0xff is compared as a signed int here and in refreshFileRows_803CE4CC (lbz +
// cmpwi at all three tests; a u8, bool or s8 member gives cmplwi / extsb).
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
            if ((s32)gGameRoot_8053AAF8->field_0xff != 0) {
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
            if ((s32)gGameRoot_8053AAF8->field_0xff != 0 && lbl_8050EC80.getFile(field_0xa4).field_0x0 != 0) {
                field_0x114 = 1;
                setMessage(0x5E);
                field_0x34->mPane->setAlpha(0);
                field_0x34->setAlpha_804023D0(mScreenHuuin->search('Paccur01'), field_0x34->field_0x28);
                field_0x34->field_0x26 = 0;
                resetButtonColors();
                for (s32 i = 0; i < 4; i++) {
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
        if (field_0xa0 != 3) {
            gAudioMgr_8053AF80->playSystemSound_80458880(0xB, -1);
        } else {
            gAudioMgr_8053AF80->playSystemSound_80458880(0xC, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(0x200, 1) != 0) {
        fn_803C21A8();
        gAudioMgr_8053AF80->playSystemSound_80458880(0xC, -1);
    } else if (lbl_8053AD30->fn_80309A9C(2, 1) != 0) {
        gAudioMgr_8053AF80->playSystemSound_80458880(0x26, -1);
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
        gAudioMgr_8053AF80->playSystemSound_80458880(0x26, -1);
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
        f32 bob = 0.0f;
        if (field_0xda != 0) {
            if (field_0xda < 5) {
                bob = 5.0f * (field_0xda * field_0xda) / 25.0f;
                field_0xda++;
            } else if (field_0xda < 10) {
                bob = 5.0f * ((10 - field_0xda) * (10 - field_0xda)) / 25.0f;
                field_0xda++;
            } else {
                field_0xda = 0;
            }
        }
        for (s32 i = 0; i < 4; i++) {
            if (bob != 0.0f && i == field_0xa0) {
                field_0x24[i]->moveFromCentre_80402E08(0.0f, bob);
            } else {
                field_0x24[i]->moveFromCentre_80402E08(0.0f, 0.0f);
            }
        }
        field_0x34->moveFromCentre_80402E08(field_0x24[field_0xa0]->field_0x8 - field_0x24[0]->field_0x8, bob);
        field_0x34->pulseAlpha_8040245C(0x3C, 0, field_0x34->field_0x28, 1);
        field_0x34->setAlpha_804023D0(mScreenHuuin->search('Paccur01'), field_0x34->field_0x28);
    }
}

// FIXME: 99.13 %, scheduling+regalloc; behaviour clean.
// (try.py: different, same length; registers shift from it).
// As fn_803C2E24: the original keeps &others in r27 from the fill loop on
// (`mr r3, r27`, a separate fill pointer) and walks it in the row loop.
// Registers and instruction order still differ.
// State 20 (copy, from fn_803C1340): the board slides out while row
// field_0xa4 takes its highlighted colours (fadeFileRow_803BC6F0), the four buttons
// slide out, and the other two rows slide back in, spaced around the middle
// of the second and third rows' places (their new targets field_0x70 /
// field_0x64); row field_0xa4's target becomes the first row's place (timings
// lbl_8050DB0C field_0x4c[0xA2..0xA9]); state 25 (picking the destination).
void FileSelect_804AFF60::fn_803C9F80() {
    s16 boardDelay = lbl_8050DB0C.field_0x4c[0xA2];
    s16 boardFrames = lbl_8050DB0C.field_0x4c[0xA3];
    s16 buttonDelay = lbl_8050DB0C.field_0x4c[0xA4];
    s16 buttonFrames = lbl_8050DB0C.field_0x4c[0xA5];
    s16 buttonStep = lbl_8050DB0C.field_0x4c[0xA6];
    s16 rowDelay = lbl_8050DB0C.field_0x4c[0xA7];
    s16 rowFrames = lbl_8050DB0C.field_0x4c[0xA8];
    s16 rowStep = lbl_8050DB0C.field_0x4c[0xA9];
    s32 end = boardDelay + boardFrames;
    if (end < buttonDelay + (buttonFrames + buttonStep * 3)) {
        end = buttonDelay + (buttonFrames + buttonStep * 3);
    }
    if (end < rowDelay + (rowFrames + rowStep)) {
        end = rowDelay + (rowFrames + rowStep);
    }

    // The two files that weren't chosen.
    s32 others[2];
    s32* other = others;
    s32* dst = others;
    for (s32 i = 0; i < 3; i++) {
        if (i != field_0xa4) {
            *dst++ = i;
        }
    }

    field_0xd8++;
    if (field_0xd8 > boardDelay) {
        field_0x14->slideX_80402B10(boardFrames, 0, 0.0f, 600.0f);
        field_0x14->animateAlpha_80402628(boardFrames, field_0x14->field_0x28, 0);
        field_0x18->slideX_80402B10(boardFrames, 0, 0.0f, 600.0f);
        field_0x18->animateAlpha_80402628(boardFrames, field_0x18->field_0x28, 0);
        field_0x1c->slideX_80402B10(boardFrames, 0, 0.0f, 600.0f);
        field_0x1c->animateAlpha_80402628(boardFrames, field_0x1c->field_0x28, 0);
        restoreAlphas_80476618(this);
        restoreAlphas_804768A0(this);
        if (field_0xd8 <= boardDelay + boardFrames) {
            fadeFileRow_803BC6F0(field_0xa4, (f32)(field_0xd8 - boardDelay) / (f32)boardFrames);
        }
    }
    for (s32 i = 0; i < 4; i++) {
        if (field_0xd8 > buttonDelay + i * buttonStep) {
            field_0x24[i]->animateMoveY_80402940(buttonFrames, 0, 0.0f, 100.0f);
            field_0x24[i]->animateAlpha_80402628(buttonFrames, field_0x24[i]->field_0x28, 0);
            restoreButtonAlphas(this, i);
        }
    }
    // The middle of the second and third rows' places.
    f32 midRow = (field_0x38[1]->field_0xc + field_0x38[2]->field_0xc) * 0.5f;
    f32 midTab = (field_0x44[1]->field_0xc + field_0x44[2]->field_0xc) * 0.5f;
    for (s32 i = 0; i < 2; i++, other++) {
        if (field_0xd8 > rowDelay + i * rowStep) {
            if (*other == 0) {
                field_0x70[0] = midRow - field_0x38[0]->field_0xc;
                field_0x64[0] = midTab - field_0x44[0]->field_0xc;
                field_0x38[*other]->moveFromCentre_80402E08(0.0f, field_0x70[0]);
                field_0x44[*other]->moveFromCentre_80402E08(0.0f, field_0x64[0]);
            } else if (*other == 1) {
                field_0x70[1] = (midRow - field_0x38[1]->field_0xc) + i * (field_0x38[2]->field_0xc - field_0x38[1]->field_0xc);
                field_0x64[1] = (midTab - field_0x44[1]->field_0xc) + i * (field_0x44[2]->field_0xc - field_0x44[1]->field_0xc);
                field_0x38[*other]->moveFromCentre_80402E08(0.0f, field_0x70[1]);
                field_0x44[*other]->moveFromCentre_80402E08(0.0f, field_0x64[1]);
            } else {
                field_0x70[2] = midRow - field_0x38[1]->field_0xc;
                field_0x64[2] = midTab - field_0x44[1]->field_0xc;
                field_0x38[*other]->moveFromCentre_80402E08(0.0f, field_0x70[2]);
                field_0x44[*other]->moveFromCentre_80402E08(0.0f, field_0x64[2]);
            }
            field_0x38[*other]->slideX_80402B10(rowFrames, 0, 600.0f, 0.0f);
            field_0x38[*other]->animateAlpha_80402628(rowFrames, 0, field_0x38[*other]->field_0x28);
            setRowAlphas(this, *other);
            field_0x44[*other]->slideX_80402B10(rowFrames, 0, 600.0f, 0.0f);
            field_0x44[*other]->animateAlpha_80402628(rowFrames, 0, field_0x44[*other]->field_0x28);
            setTabAlphas(this, *other);
        }
    }
    field_0x70[field_0xa4] = field_0x38[0]->field_0xc - field_0x38[field_0xa4]->field_0xc;
    field_0x64[field_0xa4] = field_0x44[0]->field_0xc - field_0x44[field_0xa4]->field_0xc;
    if (field_0xd8 > end) {
        setState(25);
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

// FIXME: 99.58 %, regalloc-only; behaviour clean.
// Scheduling and registers only, the timing sums as in
// fn_803C5190.
// State 22 (copy, after the copy prompt's yes): as fn_803C5190, the board (or
// the copy board) slides out while row field_0xa4 fades back (fadeFileRow_803BC6F0);
// at its end the copy board sits at the second row's height and messages
// 0x63 / 'Tdb01' 0x67 come up; the buttons slide out and the copy board comes
// in (timings lbl_8050DB0C field_0x4c[0xC2..0xC8]). Then file field_0xa4 is
// copied over file field_0xa0, its checksum written and the card write
// started; state 21 waits for it.
void FileSelect_804AFF60::fn_803C9898() {
    s32 boardDelay = lbl_8050DB0C.field_0x4c[0xC2];
    s32 boardFrames = lbl_8050DB0C.field_0x4c[0xC3];
    s32 buttonDelay = lbl_8050DB0C.field_0x4c[0xC4];
    s32 buttonFrames = lbl_8050DB0C.field_0x4c[0xC5];
    s32 buttonStep = lbl_8050DB0C.field_0x4c[0xC6];
    s16 inDelay = lbl_8050DB0C.field_0x4c[0xC7];
    s32 inFrames = lbl_8050DB0C.field_0x4c[0xC8];
    s32 boardEnd = boardDelay + boardFrames;
    s32 end = buttonDelay + buttonFrames + buttonStep;
    if (inDelay < boardEnd) {
        inDelay = boardEnd;
    }
    if (end < inDelay + inFrames) {
        end = inDelay + inFrames;
    }

    field_0xd8++;
    if (field_0xd8 > boardDelay && field_0xd8 <= boardEnd) {
        if (hasBadChecksum(field_0xa0)) {
            field_0x20->slideX_80402B10(boardFrames, 0, 0.0f, 600.0f);
            field_0x20->animateAlpha_80402628(boardFrames, field_0x20->field_0x28, 0);
            restoreAlphas_80476938(this);
            if (field_0xd8 == boardEnd) {
                field_0x20->moveFromCentre_80402E08(600.0f, field_0x38[1]->field_0xc - field_0x38[0]->field_0xc);
                field_0x20->field_0x20 = 0;
                field_0x20->mPane->setAlpha(0);
                restoreAlphas_80476938(this);
                field_0x20->field_0x26 = 0;
            }
        } else {
            field_0x14->slideX_80402B10(boardFrames, 0, 0.0f, 600.0f);
            field_0x14->animateAlpha_80402628(boardFrames, field_0x14->field_0x28, 0);
            field_0x18->slideX_80402B10(boardFrames, 0, 0.0f, 600.0f);
            field_0x18->animateAlpha_80402628(boardFrames, field_0x18->field_0x28, 0);
            field_0x1c->slideX_80402B10(boardFrames, 0, 0.0f, 600.0f);
            field_0x1c->animateAlpha_80402628(boardFrames, field_0x1c->field_0x28, 0);
            restoreAlphas_80476618(this);
            restoreAlphas_804768A0(this);
        }
        fadeFileRow_803BC6F0(field_0xa4, 1.0f - (f32)(field_0xd8 - boardDelay) / (f32)boardFrames);
        if (field_0xd8 == boardEnd) {
            setMessage(0x63);
            getMessageText_803832BC(((J2DTextBox*)mScreen->search('Tdb01'))->getStringPtr(), 0x67, 0, NULL, NULL);
        }
    }
    for (int i = 0; i < 2; i++) {
        if (field_0xd8 > buttonDelay + i * buttonStep) {
            field_0x24[i]->animateMoveY_80402940(buttonFrames, 0, 0.0f, 100.0f);
            field_0x24[i]->animateAlpha_80402628(buttonFrames, field_0x24[i]->field_0x28, 0);
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
    if (field_0xd8 > inDelay) {
        field_0x20->slideX_80402B10(inFrames, 0, 600.0f, 0.0f);
        field_0x20->animateAlpha_80402628(inFrames, 0, field_0x20->field_0x28);
        restoreAlphas_80476938(this);
    }
    if (field_0xd8 > end) {
        memcpy(&field_0x5c[field_0xa0], &field_0x5c[field_0xa4], sizeof(SaveFile_803B950C));
        writeSaveChecksum_803BACF8(field_0x5c, field_0xa0);
        lbl_8051C1A0.fn_803B9CCC(field_0x5c, 0x258, 0);
        setState(21);
    }
}

void FileSelect_804AFF60::fn_803C9338() {
    s16 rowDelay = lbl_8050DB0C.field_0x4c[0xD0];
    s16 rowFrames = lbl_8050DB0C.field_0x4c[0xD1];
    s16 boardDelay = lbl_8050DB0C.field_0x4c[0xD2];
    s16 boardFrames = lbl_8050DB0C.field_0x4c[0xD3];
    s32 end = rowDelay + rowFrames;
    if (end < boardDelay + boardFrames) {
        end = boardDelay + boardFrames;
    }
    field_0xd8++;
    if (field_0xd8 > rowDelay) {
        field_0x38[field_0xa0]->animateMoveY_80402940(rowFrames, 0, field_0x38[0]->field_0xc - field_0x38[field_0xa0]->field_0xc,
                                            field_0x38[1]->field_0xc - field_0x38[field_0xa0]->field_0xc);
        field_0x38[field_0xa0]->animateAlpha_80402628(rowFrames, 0, field_0x38[field_0xa0]->field_0x28);
        setRowAlphas(this, field_0xa0);
        field_0x14->animateMoveY_80402940(rowFrames, 0, 0.0f, field_0x38[1]->field_0xc - field_0x38[0]->field_0xc);
        field_0x14->animateAlpha_80402628(rowFrames, 0, field_0x14->field_0x28);
        field_0x18->animateMoveY_80402940(rowFrames, 0, 0.0f, field_0x38[1]->field_0xc - field_0x38[0]->field_0xc);
        field_0x18->animateAlpha_80402628(rowFrames, 0, field_0x18->field_0x28);
        field_0x1c->animateMoveY_80402940(rowFrames, 0, 0.0f, field_0x38[1]->field_0xc - field_0x38[0]->field_0xc);
        field_0x1c->animateAlpha_80402628(rowFrames, 0, field_0x1c->field_0x28);
        restoreAlphas_80476618(this);
        restoreAlphas_804768A0(this);
    }
    if (field_0xd8 > boardDelay) {
        field_0x20->slideX_80402B10(boardFrames, 0, 0.0f, -600.0f);
        field_0x20->animateAlpha_80402628(boardFrames, field_0x20->field_0x28, 0);
        restoreAlphas_80476938(this);
    }
    if (field_0xd8 > end) {
        setState(24);
    }
}

void FileSelect_804AFF60::fn_803C92B8() {
    if (field_0xb0 > 0) {
        field_0xb0--;
    }
    if (lbl_8053AD30->fn_80309C4C(0x1300, 1) != 0 || field_0xb0 == 0) {
        field_0xb0 = -1;
        fn_803C0018();
        gAudioMgr_8053AF80->playSystemSound_80458880(0xB, -1);
    }
}

// State 25 (copy): picking the destination file. Up / down move field_0xa0
// over the three files, skipping the source file field_0xa4; A asks to copy
// (message 0x62, 'Tdb01' 0x66 when the destination's checksum failed) and
// opens the destination's board (fn_803C05D8); B puts the title message back,
// hides the file cursor and goes to state 30. The tabs and the cursor bob as
// in fn_803CD2DC.
void FileSelect_804AFF60::fn_803C8B80() {
    if (lbl_8053AD30->fn_80309A9C(8, 1) != 0) {
        field_0xa0--;
        if (field_0xa0 < 0) {
            field_0xa0 = 2;
        }
        if (field_0xa0 == field_0xa4) {
            field_0xa0--;
            if (field_0xa0 < 0) {
                field_0xa0 = 2;
            }
        }
        if (field_0xa0 == field_0xa4) {
            field_0xa0--;
            if (field_0xa0 < 0) {
                field_0xa0 = 2;
            }
        }
        field_0xda = 1;
        field_0x50->field_0x26 = 0x1E;
        gAudioMgr_8053AF80->playSystemSound_80458880(0x26, -1);
    } else if (lbl_8053AD30->fn_80309A9C(4, 1) != 0) {
        field_0xa0++;
        if (field_0xa0 > 2) {
            field_0xa0 = 0;
        }
        if (field_0xa0 == field_0xa4) {
            field_0xa0++;
            if (field_0xa0 > 2) {
                field_0xa0 = 0;
            }
        }
        if (field_0xa0 == field_0xa4) {
            field_0xa0++;
            if (field_0xa0 > 2) {
                field_0xa0 = 0;
            }
        }
        field_0xda = 1;
        field_0x50->field_0x26 = 0x1E;
        gAudioMgr_8053AF80->playSystemSound_80458880(0x26, -1);
    } else if (lbl_8053AD30->fn_80309C4C(0x1100, 1) != 0) {
        field_0x114 = 1;
        setMessage(0x62);
        if (hasBadChecksum(field_0xa0)) {
            getMessageText_803832BC(((J2DTextBox*)mScreen->search('Tdb01'))->getStringPtr(), 0x66, 0, NULL, NULL);
        } else {
            field_0x80 = 0;
        }
        gAudioMgr_8053AF80->playSystemSound_80458880(0xB, -1);
        fn_803C05D8();
    } else if (lbl_8053AD30->fn_80309C4C(0x200, 1) != 0) {
        field_0x80 = 0;
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
        field_0xd8 = 0;
        field_0xda = 0;
        setState(30);
        gAudioMgr_8053AF80->playSystemSound_80458880(0xC, -1);
    }

    if (mState == 25) {
        f32 bob = 0.0f;
        if (field_0xda != 0) {
            if (field_0xda < 5) {
                bob = 5.0f * (field_0xda * field_0xda) / 25.0f;
                field_0xda++;
            } else if (field_0xda < 10) {
                bob = 5.0f * ((10 - field_0xda) * (10 - field_0xda)) / 25.0f;
                field_0xda++;
            } else {
                field_0xda = 0;
            }
        }
        for (s32 i = 0; i < 3; i++) {
            if (0.0f != bob && i == field_0xa0) {
                field_0x44[i]->moveFromCentre_80402E08(0.0f, field_0x64[i] + bob);
            } else {
                field_0x44[i]->moveFromCentre_80402E08(0.0f, field_0x64[i]);
            }
        }
        field_0x50->moveFromCentre_80402E08(0.0f, field_0x44[field_0xa0]->mPane->getBounds()->i.y - field_0x44[0]->field_0xc);
        field_0x50->pulseAlpha_8040245C(0x3C, 0, field_0x50->field_0x28, 1);
        field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur01'), field_0x50->field_0x28);
        field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur02'), field_0x50->field_0x28);
        field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur03'), field_0x50->field_0x28);
        field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur04'), field_0x50->field_0x28);
    }
}

// Scheduling and registers only, the timing sums as in
// fn_803C6798 (`buttonStep * 1` forms change nothing).
// State 26 (copy, from fn_803C05D8): the destination's board (or the copy
// board) slides in at the second row's height and the two buttons come in;
// the row that takes no part in the copy slides out and destination row
// field_0xa0 moves to the second row's place, which becomes its new target
// (field_0x70 / field_0x64) at the end (timings lbl_8050DB0C
// field_0x4c[0xAC..0xB2]); state 27 (the copy prompt).
void FileSelect_804AFF60::fn_803C7F7C() {
    s16 boardDelay = lbl_8050DB0C.field_0x4c[0xAC];
    s16 boardFrames = lbl_8050DB0C.field_0x4c[0xAD];
    s16 buttonDelay = lbl_8050DB0C.field_0x4c[0xAE];
    s16 buttonFrames = lbl_8050DB0C.field_0x4c[0xAF];
    s16 buttonStep = lbl_8050DB0C.field_0x4c[0xB0];
    s16 rowDelay = lbl_8050DB0C.field_0x4c[0xB1];
    s16 rowFrames = lbl_8050DB0C.field_0x4c[0xB2];
    s32 end = boardDelay + boardFrames;
    if (end < buttonDelay + buttonFrames + buttonStep) {
        end = buttonDelay + buttonFrames + buttonStep;
    }
    if (end < rowDelay + rowFrames) {
        end = rowDelay + rowFrames;
    }

    field_0xd8++;
    if (field_0xd8 > boardDelay) {
        if (hasBadChecksum(field_0xa0)) {
            field_0x20->moveFromCentre_80402E08(0.0f, field_0x38[1]->field_0xc - field_0x38[0]->field_0xc);
            field_0x20->slideX_80402B10(boardFrames, 0, 600.0f, 0.0f);
            field_0x20->animateAlpha_80402628(boardFrames, 0, field_0x20->field_0x28);
            restoreAlphas_80476938(this);
        } else {
            field_0x14->moveFromCentre_80402E08(0.0f, field_0x38[1]->field_0xc - field_0x38[0]->field_0xc);
            field_0x14->slideX_80402B10(boardFrames, 0, 600.0f, 0.0f);
            field_0x14->animateAlpha_80402628(boardFrames, 0, field_0x14->field_0x28);
            field_0x18->moveFromCentre_80402E08(0.0f, field_0x38[1]->field_0xc - field_0x38[0]->field_0xc);
            field_0x18->slideX_80402B10(boardFrames, 0, 600.0f, 0.0f);
            field_0x18->animateAlpha_80402628(boardFrames, 0, field_0x18->field_0x28);
            field_0x1c->moveFromCentre_80402E08(0.0f, field_0x38[1]->field_0xc - field_0x38[0]->field_0xc);
            field_0x1c->slideX_80402B10(boardFrames, 0, 600.0f, 0.0f);
            field_0x1c->animateAlpha_80402628(boardFrames, 0, field_0x1c->field_0x28);
            restoreAlphas_80476618(this);
            restoreAlphas_804768A0(this);
        }
    }
    for (s32 i = 0; i < 2; i++) {
        if (field_0xd8 > buttonDelay + i * buttonStep) {
            field_0x24[i]->animateMoveY_80402940(buttonFrames, 0, 100.0f, 0.0f);
            field_0x24[i]->animateAlpha_80402628(buttonFrames, 0, field_0x24[i]->field_0x28);
            restoreButtonAlphas(this, i);
        }
    }
    for (s32 i = 0; i < 3; i++) {
        if (i != field_0xa4) {
            if (i != field_0xa0) {
                if (field_0xd8 > rowDelay) {
                    field_0x38[i]->moveFromCentre_80402E08(0.0f, field_0x70[i]);
                    field_0x44[i]->moveFromCentre_80402E08(0.0f, field_0x64[i]);
                    field_0x38[i]->slideX_80402B10(rowFrames, 0, 0.0f, 600.0f);
                    field_0x38[i]->animateAlpha_80402628(rowFrames, field_0x38[i]->field_0x28, 0);
                    setRowAlphas(this, i);
                    field_0x44[i]->slideX_80402B10(rowFrames, 0, 0.0f, 600.0f);
                    field_0x44[i]->animateAlpha_80402628(rowFrames, field_0x44[i]->field_0x28, 0);
                    setTabAlphas(this, i);
                }
            } else if (field_0xd8 > rowDelay) {
                field_0x38[i]->animateMoveY_80402940(rowFrames, 0, field_0x70[i], field_0x38[1]->field_0xc - field_0x38[i]->getY());
                field_0x44[i]->animateMoveY_80402940(rowFrames, 0, field_0x64[i], field_0x44[1]->field_0xc - field_0x44[i]->getY());
            }
        }
    }
    if (field_0xd8 > end) {
        field_0x70[field_0xa0] = field_0x38[1]->field_0xc - field_0x38[field_0xa0]->getY();
        field_0x64[field_0xa0] = field_0x44[1]->field_0xc - field_0x44[field_0xa0]->getY();
        setState(27);
    }
}

// The copy prompt (state 27): as the delete prompt fn_803C4054, up / down
// move the cursor field_0x114 between two buttons, A confirms (0:
// fn_803C1054, 1: back out through fn_803C02CC); the cursor bobs.
void FileSelect_804AFF60::fn_803C7CA0() {
    if (lbl_8053AD30->fn_80309C4C(1, 1) != 0) {
        if (field_0x114 != 0) {
            field_0x114--;
            field_0xda = 1;
            field_0x34->field_0x26 = 30;
            gAudioMgr_8053AF80->playSystemSound_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(2, 1) != 0) {
        if (field_0x114 < 1) {
            field_0x114++;
            field_0xda = 1;
            field_0x34->field_0x26 = 30;
            gAudioMgr_8053AF80->playSystemSound_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(0x100, 1) != 0) {
        if (field_0x114 == 0) {
            fn_803C1054();
            gAudioMgr_8053AF80->playSystemSound_80458880(0xB, -1);
        } else {
            field_0x80 = 0;
            fn_803C02CC();
            gAudioMgr_8053AF80->playSystemSound_80458880(0xC, -1);
        }
    }

    if (mState == 27) {
        f32 bob = 0.0f;
        if (field_0xda != 0) {
            if (field_0xda < 5) {
                bob = 5.0f * (field_0xda * field_0xda) / 25.0f;
                field_0xda++;
            } else if (field_0xda < 10) {
                bob = 5.0f * ((10 - field_0xda) * (10 - field_0xda)) / 25.0f;
                field_0xda++;
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
        field_0x34->pulseAlpha_8040245C(0x3C, 0, field_0x34->field_0x28, 1);
        field_0x34->setAlpha_804023D0(mScreenHuuin->search('Paccur01'), field_0x34->field_0x28);
    }
}

void FileSelect_804AFF60::fn_803C70B8() {
    s16 boardDelay = lbl_8050DB0C.field_0x4c[0xB6];
    s16 boardFrames = lbl_8050DB0C.field_0x4c[0xB7];
    s16 buttonDelay = lbl_8050DB0C.field_0x4c[0xB8];
    s16 buttonFrames = lbl_8050DB0C.field_0x4c[0xB9];
    s16 buttonStep = lbl_8050DB0C.field_0x4c[0xBA];
    s16 rowDelay = lbl_8050DB0C.field_0x4c[0xBB];
    s16 rowFrames = lbl_8050DB0C.field_0x4c[0xBC];
    s32 end = boardDelay + boardFrames;
    if (end < buttonDelay + buttonFrames + buttonStep) {
        end = buttonDelay + buttonFrames + buttonStep;
    }
    if (end < rowDelay + rowFrames) {
        end = rowDelay + rowFrames;
    }

    field_0xd8++;
    if (field_0xd8 > boardDelay) {
        if (hasBadChecksum(field_0xa0)) {
            field_0x20->moveFromCentre_80402E08(0.0f, field_0x38[1]->field_0xc - field_0x38[0]->field_0xc);
            field_0x20->slideX_80402B10(boardFrames, 0, 0.0f, 600.0f);
            field_0x20->animateAlpha_80402628(boardFrames, field_0x20->field_0x28, 0);
            restoreAlphas_80476938(this);
        } else {
            field_0x14->moveFromCentre_80402E08(0.0f, field_0x38[1]->field_0xc - field_0x38[0]->field_0xc);
            field_0x14->slideX_80402B10(boardFrames, 0, 0.0f, 600.0f);
            field_0x14->animateAlpha_80402628(boardFrames, field_0x14->field_0x28, 0);
            field_0x18->moveFromCentre_80402E08(0.0f, field_0x38[1]->field_0xc - field_0x38[0]->field_0xc);
            field_0x18->slideX_80402B10(boardFrames, 0, 0.0f, 600.0f);
            field_0x18->animateAlpha_80402628(boardFrames, field_0x18->field_0x28, 0);
            field_0x1c->moveFromCentre_80402E08(0.0f, field_0x38[1]->field_0xc - field_0x38[0]->field_0xc);
            field_0x1c->slideX_80402B10(boardFrames, 0, 0.0f, 600.0f);
            field_0x1c->animateAlpha_80402628(boardFrames, field_0x1c->field_0x28, 0);
            restoreAlphas_80476618(this);
            restoreAlphas_804768A0(this);
        }
    }
    for (s32 i = 0; i < 2; i++) {
        if (field_0xd8 > buttonDelay + i * buttonStep) {
            field_0x24[i]->animateMoveY_80402940(buttonFrames, 0, 0.0f, 100.0f);
            field_0x24[i]->animateAlpha_80402628(buttonFrames, field_0x24[i]->field_0x28, 0);
            restoreButtonAlphas(this, i);
        }
    }
    if (field_0xd8 > rowDelay) {
        for (s32 i = 0; i < 3; i++) {
            if (i != field_0xa4 && i != field_0xa0) {
                field_0x38[i]->moveFromCentre_80402E08(0.0f, 0.0f);
                field_0x44[i]->moveFromCentre_80402E08(0.0f, 0.0f);
                field_0x38[i]->slideX_80402B10(rowFrames, 0, 600.0f, 0.0f);
                field_0x38[i]->animateAlpha_80402628(rowFrames, 0, field_0x38[i]->field_0x28);
                setRowAlphas(this, i);
                field_0x44[i]->slideX_80402B10(rowFrames, 0, 600.0f, 0.0f);
                field_0x44[i]->animateAlpha_80402628(rowFrames, 0, field_0x44[i]->field_0x28);
                setTabAlphas(this, i);
            } else {
                field_0x38[i]->animateMoveY_80402940(rowFrames, 0, field_0x70[i], 0.0f);
                field_0x44[i]->animateMoveY_80402940(rowFrames, 0, field_0x64[i], 0.0f);
            }
        }
        fadeFileRow_803BC6F0(field_0xa4, 1.0f - (f32)(field_0xd8 - rowDelay) / (f32)rowFrames);
    }
    if (field_0xd8 > end) {
        setState(5);
    }
}

void FileSelect_804AFF60::fn_803C6798() {
    s16 boardDelay = lbl_8050DB0C.field_0x4c[0xB6];
    s16 boardFrames = lbl_8050DB0C.field_0x4c[0xB7];
    s16 buttonDelay = lbl_8050DB0C.field_0x4c[0xB8];
    s16 buttonFrames = lbl_8050DB0C.field_0x4c[0xB9];
    s16 buttonStep = lbl_8050DB0C.field_0x4c[0xBA];
    s16 rowDelay = lbl_8050DB0C.field_0x4c[0xBB];
    s16 rowFrames = lbl_8050DB0C.field_0x4c[0xBC];
    s32 end = boardDelay + boardFrames;
    if (end < buttonDelay + buttonFrames + buttonStep) {
        end = buttonDelay + buttonFrames + buttonStep;
    }
    if (end < rowDelay + rowFrames) {
        end = rowDelay + rowFrames;
    }

    field_0xd8++;
    if (field_0xd8 > boardDelay) {
        field_0x14->moveFromCentre_80402E08(0.0f, field_0x38[1]->field_0xc - field_0x38[0]->field_0xc);
        field_0x14->slideX_80402B10(boardFrames, 0, 0.0f, 600.0f);
        field_0x14->animateAlpha_80402628(boardFrames, field_0x14->field_0x28, 0);
        field_0x18->moveFromCentre_80402E08(0.0f, field_0x38[1]->field_0xc - field_0x38[0]->field_0xc);
        field_0x18->slideX_80402B10(boardFrames, 0, 0.0f, 600.0f);
        field_0x18->animateAlpha_80402628(boardFrames, field_0x18->field_0x28, 0);
        field_0x1c->moveFromCentre_80402E08(0.0f, field_0x38[1]->field_0xc - field_0x38[0]->field_0xc);
        field_0x1c->slideX_80402B10(boardFrames, 0, 0.0f, 600.0f);
        field_0x1c->animateAlpha_80402628(boardFrames, field_0x1c->field_0x28, 0);
        restoreAlphas_80476618(this);
        restoreAlphas_804768A0(this);
    }
    if (field_0xd8 > rowDelay) {
        for (s32 i = 0; i < 3; i++) {
            if (i != field_0xa4 && i != field_0xa0) {
                field_0x38[i]->moveFromCentre_80402E08(0.0f, 0.0f);
                field_0x44[i]->moveFromCentre_80402E08(0.0f, 0.0f);
                field_0x38[i]->slideX_80402B10(rowFrames, 0, 600.0f, 0.0f);
                field_0x38[i]->animateAlpha_80402628(rowFrames, 0, field_0x38[i]->field_0x28);
                setRowAlphas(this, i);
                field_0x44[i]->slideX_80402B10(rowFrames, 0, 600.0f, 0.0f);
                field_0x44[i]->animateAlpha_80402628(rowFrames, 0, field_0x44[i]->field_0x28);
                setTabAlphas(this, i);
            } else {
                field_0x38[i]->animateMoveY_80402940(rowFrames, 0, field_0x70[i], 0.0f);
                field_0x44[i]->animateMoveY_80402940(rowFrames, 0, field_0x64[i], 0.0f);
            }
        }
    }
    if (field_0xd8 > end) {
        setState(5);
    }
}

void FileSelect_804AFF60::fn_803C6690() {
    s16 delay = lbl_8050DB0C.field_0x4c[0xC0];
    s16 frames = lbl_8050DB0C.field_0x4c[0xC1];
    s32 end = delay + frames;
    field_0xd8++;
    if (field_0xd8 > delay) {
        for (s32 i = 0; i < 3; i++) {
            field_0x38[i]->animateMoveY_80402940(frames, 0, field_0x70[i], 0.0f);
            field_0x44[i]->animateMoveY_80402940(frames, 0, field_0x64[i], 0.0f);
        }
        fadeFileRow_803BC6F0(field_0xa4, 1.0f - (f32)(field_0xd8 - delay) / (f32)frames);
    }
    if (field_0xd8 > end) {
        setState(5);
    }
}

// FIXME: 98.50 %, regalloc-only; behaviour clean.
// Registers only: `end` sits in r19 in the original (r28 in ours), which
// shifts the other saved registers by one.
// State 31: unless it came from state 5, the four buttons slide down and fade
// out one after another (the first two get their labels back, messages 0x11E
// / 0x11F, as they finish); then the first two slide back in (timings
// lbl_8050DB0C field_0x4c[0xCA..0xCF]); state 37 at the end.
void FileSelect_804AFF60::fn_803C61C4() {
    s16 outDelay = lbl_8050DB0C.field_0x4c[0xCA];
    s16 outFrames = lbl_8050DB0C.field_0x4c[0xCB];
    s16 outStep = lbl_8050DB0C.field_0x4c[0xCC];
    s16 inDelay = lbl_8050DB0C.field_0x4c[0xCD];
    s16 inFrames = lbl_8050DB0C.field_0x4c[0xCE];
    s16 inStep = lbl_8050DB0C.field_0x4c[0xCF];
    if (inDelay < outDelay + outStep + outFrames + 1) {
        inDelay = outDelay + outStep + outFrames + 1;
    }
    s32 end = inDelay + (inStep + inFrames);
    field_0xd8++;
    if (mPrevState != 5) {
        for (s32 i = 0; i < 4; i++) {
            if (field_0xd8 > outDelay + i * outStep) {
                s32 done = outFrames + (outDelay + i * outStep);
                if (field_0xd8 <= done) {
                    field_0x24[i]->animateMoveY_80402940(outFrames, 0, 0.0f, 100.0f);
                    field_0x24[i]->animateAlpha_80402628(outFrames, field_0x24[i]->field_0x28, 0);
                    restoreButtonAlphas(this, i);
                    if (field_0xd8 == done) {
                        if (i == 0) {
                            getMessageText_803832BC(((J2DTextBox*)mScreenHuuin->search('Tstr01'))->getStringPtr(), 0x11E, 0, NULL, NULL);
                            field_0x24[i]->field_0x22 = 0;
                            field_0x24[i]->field_0x26 = 0;
                        } else if (i == 1) {
                            getMessageText_803832BC(((J2DTextBox*)mScreenHuuin->search('Tcop01'))->getStringPtr(), 0x11F, 0, NULL, NULL);
                            field_0x24[i]->field_0x22 = 0;
                            field_0x24[i]->field_0x26 = 0;
                        }
                    }
                }
            }
        }
    }
    for (s32 i = 0; i < 2; i++) {
        if (field_0xd8 > inDelay + i * inStep) {
            field_0x24[i]->animateMoveY_80402940(inFrames, 0, 100.0f, 0.0f);
            field_0x24[i]->animateAlpha_80402628(inFrames, 0, field_0x24[i]->field_0x28);
            restoreButtonAlphas(this, i);
        }
    }
    if (field_0xd8 > end) {
        setState(37);
    }
}

// FIXME: 99.44 %, regalloc-only; behaviour clean.
// Registers only: `frames` and two temporaries of the inlined alpha helpers
// are rotated (r26/r27/r28). The original keeps &others in one register for
// both loops: one pointer fills the array, a second walks it.
//
// State 32 (from fn_803BF9C4, a file whose checksum failed): as fn_803CB3DC
// (same timings, lbl_8050DB0C field_0x4c[0x98..0x9F]), the other two rows
// slide out and the chosen row moves up, then the copy board and the first
// two buttons come in; state 37 (the delete prompt).
void FileSelect_804AFF60::fn_803C5808() {
    s16 rowDelay = lbl_8050DB0C.field_0x4c[0x98];
    s16 frames = lbl_8050DB0C.field_0x4c[0x99];
    s16 rowStep = lbl_8050DB0C.field_0x4c[0x9A];
    s16 boardDelay = lbl_8050DB0C.field_0x4c[0x9B];
    s16 boardFrames = lbl_8050DB0C.field_0x4c[0x9C];
    s16 buttonDelay = lbl_8050DB0C.field_0x4c[0x9D];
    s16 buttonFrames = lbl_8050DB0C.field_0x4c[0x9E];
    s16 buttonStep = lbl_8050DB0C.field_0x4c[0x9F];
    s32 end = rowDelay + frames + rowStep;
    if (end < boardDelay + boardFrames) {
        end = boardDelay + boardFrames;
    }
    if (end < buttonDelay + buttonFrames + buttonStep * 3) {
        end = buttonDelay + buttonFrames + buttonStep * 3;
    }

    // The two files that weren't chosen.
    s32 others[2];
    s32* other = others;
    s32* dst = others;
    for (s32 i = 0; i < 3; i++) {
        if (i != field_0xa4) {
            *dst++ = i;
        }
    }

    field_0xd8++;
    for (int i = 0; i < 2; i++, other++) {
        if (field_0xd8 > rowDelay + i * rowStep) {
            field_0x38[*other]->slideX_80402B10(frames, 0, 0.0f, 600.0f);
            field_0x38[*other]->animateAlpha_80402628(frames, field_0x38[*other]->field_0x28, 0);
            setRowAlphas(this, *other);
            field_0x44[*other]->slideX_80402B10(frames, 0, 0.0f, 600.0f);
            field_0x44[*other]->animateAlpha_80402628(frames, field_0x44[*other]->field_0x28, 0);
            setTabAlphas(this, *other);
        }
    }
    // The chosen row moves up to the first row's place.
    if (field_0xa4 != 0 && field_0xd8 > rowDelay) {
        field_0x38[field_0xa4]->animateMoveY_80402940(frames, 0, 0.0f,
                                            field_0x38[0]->getY() - field_0x38[field_0xa4]->getY());
        field_0x44[field_0xa4]->animateMoveY_80402940(frames, 0, 0.0f,
                                            field_0x44[0]->getY() - field_0x44[field_0xa4]->getY());
    }
    if (field_0xd8 > boardDelay) {
        field_0x20->slideX_80402B10(boardFrames, 0, 600.0f, 0.0f);
        field_0x20->animateAlpha_80402628(boardFrames, 0, field_0x20->field_0x28);
        restoreAlphas_80476938(this);
    }
    // Only the first two buttons, although `end` allows for four.
    for (s32 i = 0; i < 2; i++) {
        if (field_0xd8 > buttonDelay + i * buttonStep) {
            field_0x24[i]->animateMoveY_80402940(buttonFrames, 0, 100.0f, 0.0f);
            field_0x24[i]->animateAlpha_80402628(buttonFrames, 0, field_0x24[i]->field_0x28);
            restoreButtonAlphas(this, i);
        }
    }
    if (field_0xd8 > end) {
        setState(37);
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

// FIXME: 98.94 %, scheduling+regalloc; behaviour clean.
// Scheduling and registers only: the original adds
// buttonFrames + buttonStep before it loads buttonDelay (ours adds
// buttonStep + buttonDelay first), and the timings take other registers.
//
// State 34, after the delete prompt's yes: the board (or the copy board when
// the file's checksum failed) slides out, message 0x5F and 'Tdb01' 0x67 come
// up as it leaves; the first two buttons slide out one after another and the
// copy board comes back in (timings lbl_8050DB0C field_0x4c[0xE8..0xEE]).
// Then the file is cleared (lbl_8050EC80.fn_803B933C), its checksum written
// and the card write started; state 33 waits for it.
void FileSelect_804AFF60::fn_803C5190() {
    s16 boardDelay = lbl_8050DB0C.field_0x4c[0xE8];
    s16 boardFrames = lbl_8050DB0C.field_0x4c[0xE9];
    s16 buttonDelay = lbl_8050DB0C.field_0x4c[0xEA];
    s16 buttonFrames = lbl_8050DB0C.field_0x4c[0xEB];
    s16 buttonStep = lbl_8050DB0C.field_0x4c[0xEC];
    s16 inDelay = lbl_8050DB0C.field_0x4c[0xED];
    s16 inFrames = lbl_8050DB0C.field_0x4c[0xEE];
    s32 boardEnd = boardDelay + boardFrames;
    s32 end = buttonDelay + (buttonFrames + buttonStep);
    if (inDelay < boardEnd) {
        inDelay = boardEnd;
    }
    if (end < inDelay + inFrames) {
        end = inDelay + inFrames;
    }

    field_0xd8++;
    if (field_0xd8 > boardDelay && field_0xd8 <= boardEnd) {
        if (hasBadChecksum(field_0xa4)) {
            field_0x20->slideX_80402B10(boardFrames, 0, 0.0f, 600.0f);
            field_0x20->animateAlpha_80402628(boardFrames, field_0x20->field_0x28, 0);
            restoreAlphas_80476938(this);
            if (field_0xd8 == boardEnd) {
                field_0x20->moveFromCentre_80402E08(600.0f, 0.0f);
                field_0x20->field_0x20 = 0;
                field_0x20->mPane->setAlpha(0);
                restoreAlphas_80476938(this);
                field_0x20->field_0x26 = 0;
            }
        } else {
            field_0x14->slideX_80402B10(boardFrames, 0, 0.0f, 600.0f);
            field_0x14->animateAlpha_80402628(boardFrames, field_0x14->field_0x28, 0);
            field_0x18->slideX_80402B10(boardFrames, 0, 0.0f, 600.0f);
            field_0x18->animateAlpha_80402628(boardFrames, field_0x18->field_0x28, 0);
            field_0x1c->slideX_80402B10(boardFrames, 0, 0.0f, 600.0f);
            field_0x1c->animateAlpha_80402628(boardFrames, field_0x1c->field_0x28, 0);
            restoreAlphas_80476618(this);
            restoreAlphas_804768A0(this);
        }
        if (field_0xd8 == boardEnd) {
            setMessage(0x5F);
            getMessageText_803832BC(((J2DTextBox*)mScreen->search('Tdb01'))->getStringPtr(), 0x67, 0, NULL, NULL);
        }
    }
    for (int i = 0; i < 2; i++) {
        if (field_0xd8 > buttonDelay + i * buttonStep) {
            field_0x24[i]->animateMoveY_80402940(buttonFrames, 0, 0.0f, 100.0f);
            field_0x24[i]->animateAlpha_80402628(buttonFrames, field_0x24[i]->field_0x28, 0);
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
    if (field_0xd8 > inDelay) {
        field_0x20->slideX_80402B10(inFrames, 0, 600.0f, 0.0f);
        field_0x20->animateAlpha_80402628(inFrames, 0, field_0x20->field_0x28);
        restoreAlphas_80476938(this);
    }
    if (field_0xd8 > end) {
        lbl_8050EC80.fn_803B933C(field_0x5c, field_0xa4);
        writeSaveChecksum_803BACF8(field_0x5c, field_0xa4);
        lbl_8051C1A0.fn_803B9CCC(field_0x5c, 0x258, 0);
        setState(33);
    }
}

// State 35 (after the delete's card write, from fn_803BF004): row field_0xa4
// slides out between the first and third rows' places (fading its colours
// back when its checksum failed); at its end it waits at the right with
// message 0x60 and the rows are refreshed (refreshFileRows_803CE4CC). The copy board
// slides out and the row comes back in (timings lbl_8050DB0C
// field_0x4c[0xDE..0xE3]); state 36.
void FileSelect_804AFF60::fn_803C4628() {
    s16 outDelay = lbl_8050DB0C.field_0x4c[0xDE];
    s16 outFrames = lbl_8050DB0C.field_0x4c[0xDF];
    s16 boardDelay = lbl_8050DB0C.field_0x4c[0xE0];
    s16 boardFrames = lbl_8050DB0C.field_0x4c[0xE1];
    s16 inDelay = lbl_8050DB0C.field_0x4c[0xE2];
    s16 inFrames = lbl_8050DB0C.field_0x4c[0xE3];
    s32 outEnd = outDelay + outFrames;
    if (inDelay < outEnd) {
        inDelay = outEnd;
    }
    s32 end = boardDelay + boardFrames;
    if (end < inDelay + inFrames) {
        end = inDelay + inFrames;
    }

    field_0xd8++;
    if (field_0xd8 > outDelay && field_0xd8 <= outEnd) {
        field_0x38[field_0xa4]->animateMoveY_80402940(outFrames, 0, field_0x38[0]->field_0xc - field_0x38[field_0xa4]->field_0xc,
                                            field_0x38[2]->field_0xc - field_0x38[field_0xa4]->field_0xc);
        field_0x38[field_0xa4]->animateAlpha_80402628(outFrames, field_0x38[field_0xa4]->field_0x28, 0);
        setRowAlphas(this, field_0xa4);
        if (field_0xd8 == outEnd) {
            field_0x38[field_0xa4]->moveFromCentre_80402E08(600.0f, field_0x38[0]->getY() - field_0x38[field_0xa4]->getY());
            field_0x38[field_0xa4]->field_0x20 = 0;
            field_0x38[field_0xa4]->mPane->setAlpha(0);
            setRowAlphas(this, field_0xa4);
            field_0x38[field_0xa4]->field_0x26 = 0;
            setMessage(0x60);
            refreshFileRows_803CE4CC();
            field_0x80 = 0;
        }
        if (hasBadChecksum(field_0xa4) != false) {
            fadeFileRow_803BC6F0(field_0xa4, 1.0f - (f32)(field_0xd8 - outDelay) / (f32)outFrames);
        }
    }
    if (field_0xd8 > boardDelay) {
        field_0x20->slideX_80402B10(boardFrames, 0, 0.0f, 600.0f);
        field_0x20->animateAlpha_80402628(boardFrames, field_0x20->field_0x28, 0);
        restoreAlphas_80476938(this);
    }
    if (field_0xd8 > inDelay) {
        field_0x38[field_0xa4]->slideX_80402B10(inFrames, 0, 600.0f, 0.0f);
        field_0x38[field_0xa4]->animateAlpha_80402628(inFrames, 0, field_0x38[field_0xa4]->field_0x28);
        setRowAlphas(this, field_0xa4);
    }
    if (field_0xd8 > end) {
        setState(36);
    }
}

// Back from the delete prompt (state 36), after its message or a button:
// refreshes the rows (refreshFileRows_803CE4CC), puts the title message back, hides the
// file cursor and the command cursor, and goes to state 38.
void FileSelect_804AFF60::fn_803C4330() {
    if (field_0xb0 > 0) {
        field_0xb0--;
    }
    if (lbl_8053AD30->fn_80309C4C(0x1300, 1) != 0 || field_0xb0 == 0) {
        field_0xb0 = -1;
        refreshFileRows_803CE4CC();
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
        gAudioMgr_8053AF80->playSystemSound_80458880(0xB, -1);
    }
}

// The delete prompt (state 37): up / down move the cursor field_0x114
// between its two buttons, A confirms (0: fn_803BF3BC, 1: back out through
// fn_803BF698); the cursor bobs as in fn_803CABAC.
void FileSelect_804AFF60::fn_803C4054() {
    if (lbl_8053AD30->fn_80309C4C(1, 1) != 0) {
        if (field_0x114 != 0) {
            field_0x114--;
            field_0xda = 1;
            field_0x34->field_0x26 = 30;
            gAudioMgr_8053AF80->playSystemSound_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(2, 1) != 0) {
        if (field_0x114 < 1) {
            field_0x114++;
            field_0xda = 1;
            field_0x34->field_0x26 = 30;
            gAudioMgr_8053AF80->playSystemSound_80458880(0x26, -1);
        }
    } else if (lbl_8053AD30->fn_80309C4C(0x100, 1) != 0) {
        if (field_0x114 == 0) {
            fn_803BF3BC();
            gAudioMgr_8053AF80->playSystemSound_80458880(0xB, -1);
        } else {
            field_0x80 = 0;
            fn_803BF698();
            gAudioMgr_8053AF80->playSystemSound_80458880(0xC, -1);
        }
    }

    if (mState == 37) {
        f32 bob = 0.0f;
        if (field_0xda != 0) {
            if (field_0xda < 5) {
                bob = 5.0f * (field_0xda * field_0xda) / 25.0f;
                field_0xda++;
            } else if (field_0xda < 10) {
                bob = 5.0f * ((10 - field_0xda) * (10 - field_0xda)) / 25.0f;
                field_0xda++;
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
        field_0x34->pulseAlpha_8040245C(0x3C, 0, field_0x34->field_0x28, 1);
        field_0x34->setAlpha_804023D0(mScreenHuuin->search('Paccur01'), field_0x34->field_0x28);
    }
}

// FIXME: 99.17 %, regalloc-only; behaviour clean.
// Registers only: the original has frames in r26 and the row's index
// temporaries in r28/r27 (ours r28 and r27/r26).
// State 38 (after a delete, from fn_803C4330): the two rows that weren't
// chosen slide back in and the chosen row returns to its place, as the end of
// fn_803C2E24 (same timings, lbl_8050DB0C field_0x4c[0xD9..0xDB]); state 5.
void FileSelect_804AFF60::fn_803C3904() {
    s32 others[2];
    s32* row = others;
    s16 frames = lbl_8050DB0C.field_0x4c[0xDA];
    s16 rowStep = lbl_8050DB0C.field_0x4c[0xDB];
    s16 rowDelay = lbl_8050DB0C.field_0x4c[0xD9];
    s32 end = rowDelay + frames + rowStep;

    // The two files that weren't chosen.
    s32* other = others;
    for (int i = 0; i < 3; i++) {
        if (i != field_0xa4) {
            *other++ = i;
        }
    }

    field_0xd8++;
    for (int i = 0; i < 2; i++, row++) {
        if (field_0xd8 > rowDelay + i * rowStep) {
            field_0x38[*row]->slideX_80402B10(frames, 0, 600.0f, 0.0f);
            field_0x38[*row]->animateAlpha_80402628(frames, 0, field_0x38[*row]->field_0x28);
            setRowAlphas(this, *row);
            field_0x44[*row]->slideX_80402B10(frames, 0, 600.0f, 0.0f);
            field_0x44[*row]->animateAlpha_80402628(frames, 0, field_0x44[*row]->field_0x28);
            setTabAlphas(this, *row);
        }
    }
    // The chosen row moves back down from the first row's place.
    if (field_0xa4 != 0 && field_0xd8 > rowDelay) {
        field_0x38[field_0xa4]->animateMoveY_80402940(frames, 0, field_0x38[0]->field_0xc - field_0x38[field_0xa4]->field_0xc,
                                            0.0f);
        field_0x44[field_0xa4]->animateMoveY_80402940(frames, 0, field_0x44[0]->field_0xc - field_0x44[field_0xa4]->field_0xc,
                                            0.0f);
    }
    if (field_0xd8 > end) {
        setState(5);
    }
}

// FIXME: 99.17 %, regalloc-only; behaviour clean.
// (try.py: different, same length). One instruction moves:
// the original keeps &others in r29 from the fill loop on (`mr r3, r29` for
// the fill pointer) and walks it in the row loop; ours computes &others
// again before the row loop (addi r29, r1, 0x8). Every field, call and
// branch is the original's. A running `delay += step` instead of
// `i * step` folds the start add (98.16, 3 instructions fewer).
// Closes the file menu (state 39): the board (or the copy board when the
// file holds save data) and the first two command buttons slide out, the
// other two rows slide back in and the chosen row returns to its place, one
// after another (the timings are lbl_8050DB0C's 0xD4..0xDB); then state 5.
void FileSelect_804AFF60::fn_803C2E24() {
    s32 boardDelay = lbl_8050DB0C.field_0x4c[0xD4];
    s32 boardFrames = lbl_8050DB0C.field_0x4c[0xD5];
    s32 buttonDelay = lbl_8050DB0C.field_0x4c[0xD6];
    s32 buttonFrames = lbl_8050DB0C.field_0x4c[0xD7];
    s32 buttonStep = lbl_8050DB0C.field_0x4c[0xD8];
    s32 rowDelay = lbl_8050DB0C.field_0x4c[0xD9];
    s32 frames = lbl_8050DB0C.field_0x4c[0xDA];
    s32 rowStep = lbl_8050DB0C.field_0x4c[0xDB];
    s32 end = boardDelay + boardFrames;
    if (end < buttonDelay + buttonFrames + buttonStep * 3) {
        end = buttonDelay + buttonFrames + buttonStep * 3;
    }
    if (end < rowDelay + frames + rowStep) {
        end = rowDelay + frames + rowStep;
    }

    // The two files that weren't chosen.
    s32 others[2];
    s32* other = others;
    s32* dst = others;
    for (int i = 0; i < 3; i++) {
        if (i != field_0xa4) {
            *dst++ = i;
        }
    }

    field_0xd8++;
    if (field_0xd8 > boardDelay) {
        if (hasBadChecksum(field_0xa4)) {
            field_0x20->slideX_80402B10(boardFrames, 0, 0.0f, 600.0f);
            field_0x20->animateAlpha_80402628(boardFrames, field_0x20->field_0x28, 0);
            restoreAlphas_80476938(this);
        } else {
            field_0x14->slideX_80402B10(boardFrames, 0, 0.0f, 600.0f);
            field_0x14->animateAlpha_80402628(boardFrames, field_0x14->field_0x28, 0);
            field_0x18->slideX_80402B10(boardFrames, 0, 0.0f, 600.0f);
            field_0x18->animateAlpha_80402628(boardFrames, field_0x18->field_0x28, 0);
            field_0x1c->slideX_80402B10(boardFrames, 0, 0.0f, 600.0f);
            field_0x1c->animateAlpha_80402628(boardFrames, field_0x1c->field_0x28, 0);
            restoreAlphas_80476618(this);
            restoreAlphas_804768A0(this);
        }
    }
    // Only the first two buttons, although `end` allows for four.
    for (s32 i = 0; i < 2; i++) {
        if (field_0xd8 > buttonDelay + i * buttonStep) {
            field_0x24[i]->animateMoveY_80402940(buttonFrames, 0, 0.0f, 100.0f);
            field_0x24[i]->animateAlpha_80402628(buttonFrames, field_0x24[i]->field_0x28, 0);
            restoreButtonAlphas(this, i);
        }
    }
    for (int i = 0; i < 2; i++, other++) {
        if (field_0xd8 > rowDelay + i * rowStep) {
            field_0x38[*other]->slideX_80402B10(frames, 0, 600.0f, 0.0f);
            field_0x38[*other]->animateAlpha_80402628(frames, 0, field_0x38[*other]->field_0x28);
            setRowAlphas(this, *other);
            field_0x44[*other]->slideX_80402B10(frames, 0, 600.0f, 0.0f);
            field_0x44[*other]->animateAlpha_80402628(frames, 0, field_0x44[*other]->field_0x28);
            setTabAlphas(this, *other);
        }
    }
    // The chosen row moves back down from the first row's place.
    if (field_0xa4 != 0 && field_0xd8 > rowDelay) {
        field_0x38[field_0xa4]->animateMoveY_80402940(frames, 0, field_0x38[0]->field_0xc - field_0x38[field_0xa4]->field_0xc,
                                            0.0f);
        field_0x44[field_0xa4]->animateMoveY_80402940(frames, 0, field_0x44[0]->field_0xc - field_0x44[field_0xa4]->field_0xc,
                                            0.0f);
    }
    if (field_0xd8 > end) {
        setState(5);
    }
}

// Shows the three file rows: refreshes them (refreshFileRows_803CE4CC), puts up the
// title message, slides the row panes in from their start positions with
// alpha 0 and goes to state 4.
void FileSelect_804AFF60::fn_803C24BC() {
    refreshFileRows_803CE4CC();
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

// fn_803CABAC (the file menu), closing it: rereads the files (refreshFileRows_803CE4CC),
// shows the file list's message (0x117 / 0x116 as fn_803C0018), slides the
// file cursor off to the left and fades it, stops the rows, the board's
// movers and the four buttons; state 6.
void FileSelect_804AFF60::fn_803C21A8() {
    refreshFileRows_803CE4CC();
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
    for (s32 i = 0; i < 4; i++) {
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
    restoreAlphas_80476618(this);
    restoreAlphas_804768A0(this);
    field_0x14->field_0x26 = 0;
    field_0x18->field_0x26 = 0;
    field_0x1c->field_0x26 = 0;

    field_0x34->mPane->setAlpha(0);
    field_0x34->setAlpha_804023D0(mScreenHuuin->search('Paccur01'), field_0x34->field_0x28);
    field_0x34->field_0x26 = 0;
    for (s32 i = 0; i < 4; i++) {
        field_0x24[i]->moveFromCentre_80402E08(0.0f, 100.0f);
        field_0x24[i]->field_0x22 = 0;
        field_0x24[i]->mPane->setAlpha(0);
        restoreButtonAlphas(this, i);
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

// fn_803CABAC (the file menu), the copy entry: the file cursor slides off to
// the left and fades, the copy / delete buttons get the 'Pcopb01' colours,
// the rows, board and buttons stop and the rows' target x (field_0x64 /
// field_0x70) are cleared; state 20.
void FileSelect_804AFF60::fn_803C1340() {
    field_0x50->moveFromCentre_80402E08(-300.0f, 0.0f);
    field_0x50->field_0x20 = 0;
    field_0x50->mPane->setAlpha(0);
    field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur01'), field_0x50->field_0x28);
    field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur02'), field_0x50->field_0x28);
    field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur03'), field_0x50->field_0x28);
    field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur04'), field_0x50->field_0x28);
    ((J2DPicture*)mScreenHuuin->search('Pcopb01'))->setBlackWhite(field_0xe4, field_0xe0);
    ((J2DPicture*)mScreenHuuin->search('Pcopb02'))->setBlackWhite(field_0xe4, field_0xe0);
    ((J2DPicture*)mScreenHuuin->search('Pdelb01'))->setBlackWhite(field_0xe4, field_0xe0);
    ((J2DPicture*)mScreenHuuin->search('Pdelb02'))->setBlackWhite(field_0xe4, field_0xe0);
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
    field_0x34->field_0x26 = 0;
    for (s32 i = 0; i < 4; i++) {
        field_0x24[i]->field_0x22 = 0;
        field_0x24[i]->field_0x26 = 0;
    }
    for (s32 i = 0; i < 3; i++) {
        field_0x70[i] = 0.0f;
        field_0x64[i] = 0.0f;
    }
    field_0xd8 = 0;
    field_0xda = 0;
    setState(20);
}

// Not auto-inlined: fn_803C7CA0 calls fn_803C1054 out of line in the original
// (docs/INLINING.md, pragma rule 1).
#pragma auto_inline off
void FileSelect_804AFF60::fn_803C1054() {
    field_0x20->mPane->show();
    field_0x50->moveFromCentre_80402E08(-300.0f, 0.0f);
    field_0x50->field_0x20 = 0;
    field_0x50->mPane->setAlpha(0);
    field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur01'), field_0x50->field_0x28);
    field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur02'), field_0x50->field_0x28);
    field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur03'), field_0x50->field_0x28);
    field_0x50->setAlpha_804023D0(mScreenHuuin->search('Pfcur04'), field_0x50->field_0x28);
    if (hasBadChecksum(field_0xa0)) {
        field_0x20->field_0x20 = 0;
        field_0x20->field_0x26 = 0;
    } else {
        field_0x14->field_0x20 = 0;
        field_0x14->field_0x26 = 0;
        field_0x18->field_0x20 = 0;
        field_0x18->field_0x26 = 0;
        field_0x1c->field_0x20 = 0;
        field_0x1c->field_0x26 = 0;
        field_0x20->moveFromCentre_80402E08(600.0f, field_0x38[1]->field_0xc - field_0x38[0]->field_0xc);
        field_0x20->field_0x20 = 0;
        field_0x20->mPane->setAlpha(0);
        restoreAlphas_80476938(this);
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
    setState(22);
}
#pragma auto_inline reset

// Called by fn_803C9F14 after the copy's card write: shows the copied file
// field_0xa0's board ('Pqb03'..'Pqb10' as in fn_803C05D8, its stage flags into
// field_0xa8 / field_0xb8), message 0x64, refreshes the rows (refreshFileRows_803CE4CC),
// resets the three boards' movers and alphas, puts row field_0xa0 back at the
// first row's place with its alphas, then state 23 (sound 0x5B).
void FileSelect_804AFF60::fn_803C0A34() {
    for (s32 i = 0; i < 8; i++) {
        if (fn_803B90E4(field_0xa0, i)) {
            mScreen->search(lbl_80476980[i + 2])->hide();
        } else {
            mScreen->search(lbl_80476980[i + 2])->show();
        }
    }
    field_0xa8 = field_0xa0;
    for (s32 i = 0; i < 8; i++) {
        field_0xb8[i] = fn_803B8CAC(field_0xa0, i);
    }
    setMessage(0x64);
    refreshFileRows_803CE4CC();
    field_0x80 = 0;
    field_0x14->moveFromCentre_80402E08(0.0f, 0.0f);
    field_0x14->field_0x22 = 0;
    field_0x14->mPane->setAlpha(0);
    field_0x18->moveFromCentre_80402E08(0.0f, 0.0f);
    field_0x18->field_0x22 = 0;
    field_0x18->mPane->setAlpha(0);
    field_0x1c->moveFromCentre_80402E08(0.0f, 0.0f);
    field_0x1c->field_0x22 = 0;
    field_0x1c->mPane->setAlpha(0);
    restoreAlphas_80476618(this);
    restoreAlphas_804768A0(this);
    field_0x14->field_0x26 = 0;
    field_0x18->field_0x26 = 0;
    field_0x1c->field_0x26 = 0;
    field_0x20->field_0x20 = 0;
    field_0x20->field_0x26 = 0;
    field_0x38[field_0xa0]->moveFromCentre_80402E08(0.0f, field_0x38[0]->getY() - field_0x38[field_0xa0]->getY());
    field_0x38[field_0xa0]->field_0x22 = 0;
    field_0x38[field_0xa0]->field_0x26 = 0;
    field_0x38[field_0xa0]->mPane->setAlpha(field_0x38[field_0xa0]->field_0x28);
    setRowAlphas(this, field_0xa0);
    field_0xd8 = 0;
    field_0xda = 0;
    setState(23);
    gAudioMgr_8053AF80->playSystemSound_80458880(0x5B, -1);
}

// Not auto-inlined: fn_803C8B80 calls fn_803C05D8 out of line in the original
// (docs/INLINING.md, pragma rule 1).
#pragma auto_inline off
void FileSelect_804AFF60::fn_803C05D8() {
    for (s32 i = 0; i < 8; i++) {
        if (fn_803B90E4(field_0xa0, i)) {
            mScreen->search(lbl_80476980[i + 2])->hide();
        } else {
            mScreen->search(lbl_80476980[i + 2])->show();
        }
    }
    field_0xa8 = field_0xa0;
    for (s32 i = 0; i < 8; i++) {
        field_0xb8[i] = fn_803B8CAC(field_0xa0, i);
    }
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
    field_0x34->field_0x26 = 0;
    if (hasBadChecksum(field_0xa0)) {
        field_0x20->mPane->show();
        field_0x20->moveFromCentre_80402E08(600.0f, 0.0f);
        field_0x20->field_0x20 = 0;
        field_0x20->mPane->setAlpha(0);
        restoreAlphas_80476938(this);
        field_0x20->field_0x26 = 0;
    } else {
        field_0x14->field_0x20 = 0;
        field_0x14->field_0x26 = 0;
        field_0x18->field_0x20 = 0;
        field_0x18->field_0x26 = 0;
        field_0x1c->field_0x20 = 0;
        field_0x1c->field_0x26 = 0;
    }
    getMessageText_803832BC(((J2DTextBox*)mScreenHuuin->search('Tstr01'))->getStringPtr(), 0x11E, 0, NULL, NULL);
    getMessageText_803832BC(((J2DTextBox*)mScreenHuuin->search('Tcop01'))->getStringPtr(), 0x11F, 0, NULL, NULL);
    field_0x24[0]->field_0x22 = 0;
    field_0x24[0]->field_0x26 = 0;
    field_0x24[1]->field_0x22 = 0;
    field_0x24[1]->field_0x26 = 0;
    field_0xd8 = 0;
    field_0xda = 0;
    setState(26);
}
#pragma auto_inline reset

// Not auto-inlined: fn_803C7CA0 calls fn_803C02CC out of line in the original
// (docs/INLINING.md, pragma rule 1).
#pragma auto_inline off
// fn_803C7CA0 (state 27), the second answer: back to the file list's message,
// the file cursor fades, the rows, the board (file field_0xa0's checksum
// failed) or the buttons and the first two buttons stop; state 28.
void FileSelect_804AFF60::fn_803C02CC() {
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
    if (hasBadChecksum(field_0xa0)) {
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
    setState(28);
}
#pragma auto_inline reset

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

// Opens the copy board for the chosen file (state 32): the target cursor
// field_0x50 and the board field_0x20 with its 'Pdb' panes faded in from
// their start position, the command buttons back at their colours, and the
// copy labels; the timings are lbl_8050DB0C's 0xCA..0xCD.
void FileSelect_804AFF60::fn_803BF9C4() {
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
    for (int i = 0; i < 3; i++) {
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
    restoreAlphas_80476938(this);
    field_0x20->field_0x26 = 0;

    field_0x34->mPane->setAlpha(0);
    field_0x34->setAlpha_804023D0(mScreenHuuin->search('Paccur01'), field_0x34->field_0x28);
    field_0x34->field_0x26 = 0;
    for (s32 i = 0; i < 4; i++) {
        field_0x24[i]->mPane->show();
        field_0x24[i]->moveFromCentre_80402E08(0.0f, 100.0f);
        field_0x24[i]->field_0x22 = 0;
        field_0x24[i]->mPane->setAlpha(0);
        field_0x24[i]->field_0x26 = 0;
        restoreButtonAlphas(this, i);
    }

    getMessageText_803832BC(((J2DTextBox*)mScreenHuuin->search('Tstr01'))->getStringPtr(), 0x11E, 0, NULL, NULL);
    getMessageText_803832BC(((J2DTextBox*)mScreenHuuin->search('Tcop01'))->getStringPtr(), 0x11F, 0, NULL, NULL);
    s16 end = lbl_8050DB0C.field_0x4c[0xCA] + lbl_8050DB0C.field_0x4c[0xCC] + lbl_8050DB0C.field_0x4c[0xCB];
    field_0xd8 = lbl_8050DB0C.field_0x4c[0xCD];
    if (field_0xd8 < end) {
        field_0xd8 = end;
    }
    field_0xda = 0;
    setState(32);
}

// fn_803C4054 (the delete prompt), the other answer: rereads the files
// (refreshFileRows_803CE4CC), shows the file list's message (0x117 / 0x116 as
// fn_803C0018), slides the file cursor off to the left and fades it, stops
// the rows, the board or the buttons and the first two buttons; state 39.
void FileSelect_804AFF60::fn_803BF698() {
    refreshFileRows_803CE4CC();
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
        restoreAlphas_80476938(this);
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

// Not auto-inlined: fn_803C579C calls fn_803BF004 out of line in the original
// (docs/INLINING.md, pragma rule 1).
#pragma auto_inline off
// FIXME: 97.00 %, scheduling+regalloc; behaviour clean.
// Registers and scheduling only: the original keeps `this` in
// r31 and the table base in r30 (ours r28 / r31) and loads the two rows' y
// into f0 / f2 (ours f2 / f0); a hand-inlined row-alpha block with a `file`
// local is the same.
// fn_803C579C (state 33), once the card manager is done: the board stops,
// row field_0xa4 moves up to the first row's place and gets its panes'
// alphas back; state 35, sound 0x62.
void FileSelect_804AFF60::fn_803BF004() {
    field_0x20->field_0x20 = 0;
    field_0x20->field_0x26 = 0;
    field_0x38[field_0xa4]->moveFromCentre_80402E08(0.0f, field_0x38[0]->field_0xc - field_0x38[field_0xa4]->field_0xc);
    field_0x38[field_0xa4]->field_0x22 = 0;
    field_0x38[field_0xa4]->field_0x26 = 0;
    field_0x38[field_0xa4]->mPane->setAlpha(field_0x38[field_0xa4]->field_0x28);
    setRowAlphas(this, field_0xa4);
    field_0xd8 = 0;
    field_0xda = 0;
    setState(35);
    gAudioMgr_8053AF80->playSystemSound_80458880(0x62, -1);
}
#pragma auto_inline reset

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
            setSpriteOrtho();
            if (i == 6) {
                void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
                gGameRoot2_8053AB10->field_0x20->drawSobSprite_802F86CC(&shadowPos, res, 0xAB, &shadow, 1.0f, 1.0f);
                res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
                gGameRoot2_8053AB10->field_0x20->drawSobSprite_802F86CC(&pos, res, 0xAB, &params, 1.0f, 1.0f);
            } else if (i == 7) {
                shadow.field_0x0[7] = 12;
                params.field_0x0[7] = 12;
                void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "player_sob.bin");
                gGameRoot2_8053AB10->field_0x20->drawSobSprite_802F86CC(&shadowPos, res, 0x10, &shadow, 1.0f, 1.0f);
                res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "player_sob.bin");
                gGameRoot2_8053AB10->field_0x20->drawSobSprite_802F86CC(&pos, res, 0x10, &params, 1.0f, 1.0f);
            } else {
                s32 frame = lbl_80476DE8[i] + 8;
                void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_gc_sob.bin");
                gGameRoot2_8053AB10->field_0x20->drawSobSprite_802F86CC(&shadowPos, res, frame, &shadow, 1.0f, 1.0f);
                frame = lbl_80476DE8[i] + 8;
                res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_gc_sob.bin");
                gGameRoot2_8053AB10->field_0x20->drawSobSprite_802F86CC(&pos, res, frame, &params, 1.0f, 1.0f);
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
                    setSpriteOrtho();
                    s32 frame = (field_0xdc % 27) / 9 + 0x21B;
                    void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_gc_sob.bin");
                    gGameRoot2_8053AB10->field_0x20->drawSobSprite_802F86CC(&pos, res, frame, &params, 1.0f, 1.0f);
                } else {
                    setSpritePos(this, &pos, &lbl_804769D0[i], 0.0f, 12.0f);
                    setSpriteOrtho();
                    s32 frame = field_0xdc / 9 + lbl_80476DE8[i];
                    void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_gc_sob.bin");
                    gGameRoot2_8053AB10->field_0x20->drawSobSprite_802F86CC(&pos, res, frame, &params, 1.0f, 1.0f);
                }
            }
        }
    }
    gGameRoot2_8053AB10->setPort_8013B490();
}

// Restores file row `file`'s pictures to the colours the constructor read
// (field_0xe8 / field_0xfc), group by group as fadeFileRow_803BC6F0 fades them.
void FileSelect_804AFF60::fn_803BDD44(s32 file) {
    ((J2DPicture*)mScreenHuuin->search(lbl_80476BA8[file]))->setBlackWhite(field_0xfc[0], field_0xe8[0]);

    setGroup1Colors(this, file, field_0xfc[1], field_0xe8[1]);
    setGroup2Colors(this, file, field_0xfc[2], field_0xe8[2]);
    setGroup3Colors(this, file, field_0xfc[3], field_0xe8[3]);
    setGroup4Colors(this, file, field_0xfc[4], field_0xe8[4]);
}

// Gives file row `file`'s pictures their highlighted colours, the ones
// fadeFileRow_803BC6F0 fades them to at alpha 1 (group by group, the same panes as
// fn_803BDD44).
void FileSelect_804AFF60::fn_803BD5E0(s32 file) {
    ((J2DPicture*)mScreenHuuin->search(lbl_80476BA8[file]))
        ->setBlackWhite(JUtility::TColor(0x00, 0x00, 0x00, 0x00), JUtility::TColor(0x80, 0x80, 0x80, 0xFF));

    setGroup1Colors(this, file, JUtility::TColor(0x82, 0x82, 0x82, 0x00), JUtility::TColor(0xB4, 0xB4, 0xB4, 0xFF));
    setGroup2Colors(this, file, JUtility::TColor(0x50, 0x50, 0x50, 0x00), JUtility::TColor(0x80, 0x80, 0x80, 0xFF));
    setGroup3Colors(this, file, JUtility::TColor(0x64, 0x64, 0x64, 0x00), JUtility::TColor(0x82, 0x82, 0x82, 0xFF));
    setGroup4Colors(this, file, JUtility::TColor(0xB4, 0xB4, 0xB4, 0x00), JUtility::TColor(0xB4, 0xB4, 0xB4, 0xFF));
}

// Fades file `file`'s row pictures between their colours and grey:
// alpha 0 keeps the colours read by the constructor, alpha 1 sets each
// group's black and white to fixed grey levels.
void FileSelect_804AFF60::fadeFileRow_803BC6F0(s32 file, f32 alpha) {
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
    setGroup1Colors(this, file, black, white);

    black.r = field_0xfc[2].r + (0x50 - field_0xfc[2].r) * alpha;
    black.g = field_0xfc[2].g + (0x50 - field_0xfc[2].g) * alpha;
    black.b = field_0xfc[2].b + (0x50 - field_0xfc[2].b) * alpha;
    black.a = 0;
    white.r = field_0xe8[2].r + (0x80 - field_0xe8[2].r) * alpha;
    white.g = field_0xe8[2].g + (0x80 - field_0xe8[2].g) * alpha;
    white.b = field_0xe8[2].b + (0x80 - field_0xe8[2].b) * alpha;
    white.a = 0xFF;
    setGroup2Colors(this, file, black, white);

    black.r = field_0xfc[3].r + (0x82 - field_0xfc[3].r) * alpha;
    black.g = field_0xfc[3].g + (0x82 - field_0xfc[3].g) * alpha;
    black.b = field_0xfc[3].b + (0x82 - field_0xfc[3].b) * alpha;
    black.a = 0;
    white.r = field_0xe8[3].r + (0xB4 - field_0xe8[3].r) * alpha;
    white.g = field_0xe8[3].g + (0xB4 - field_0xe8[3].g) * alpha;
    white.b = field_0xe8[3].b + (0xB4 - field_0xe8[3].b) * alpha;
    white.a = 0xFF;
    setGroup3Colors(this, file, black, white);

    black.r = field_0xfc[4].r + (0xB4 - field_0xfc[4].r) * alpha;
    black.g = field_0xfc[4].g + (0xB4 - field_0xfc[4].g) * alpha;
    black.b = field_0xfc[4].b + (0xB4 - field_0xfc[4].b) * alpha;
    black.a = 0;
    white.r = field_0xe8[4].r + (0xB4 - field_0xe8[4].r) * alpha;
    white.g = field_0xe8[4].g + (0xB4 - field_0xe8[4].g) * alpha;
    white.b = field_0xe8[4].b + (0xB4 - field_0xe8[4].b) * alpha;
    white.a = 0xFF;
    setGroup4Colors(this, file, black, white);
}

// One step of the 'kk_' fade (fn_803CE9EC's cases 1 and 2 have the same
// inlined code): field_0x58 slides from -200 to 0 and fades in with the
// square of field_0xde / frames. Its out-of-line copy is dropped by the
// linker (as getProgressMsg); it is compiled first (the bottom of the file),
// which is where the unit's .sdata2 pool gets its leading 0, -200, 200 and
// the two conversion doubles (with it, every literal is the original's).
// R2 pool helper (inlined at 2 call sites); was setFadeStep.
static void pool_80543458(FileSelect_804AFF60* self, s16 frames) {
    f32 t = (f32)(self->field_0xde * self->field_0xde) / (f32)(frames * frames);
    self->field_0x58->moveFromCentre_80402E08(0.0f, 200.0f * t + -200.0f);
    self->field_0x58->mPane->setAlpha(t * self->field_0x58->field_0x28);
}
