#ifndef GAME_TU_803BC6F0_H
#define GAME_TU_803BC6F0_H

#include "types.h"
#include "JSystem/JUtility/TColor.h"
#include "game/tu_803B8C74.h"

class J2DScreen;
class J2DPicture;
class PaneCtrl_804AF86C;

// The file select screen: vtable 0x804AFF60 (one slot, the destructor
// fn_803CFFBC), constructor fn_803D0264 (game/tu_803BC6F0.cpp); 0x1AC bytes
// (__nw__(0x1AC) in tu_80134C04's fn_80135020 path). The constructor loads
// "file_select.blo" and "file_select_huuin.blo" from lbl_80529DEC's
// field_0x30 archive and stores `this` in lbl_8053ADB0 (the destructor
// clears it). Three save files (the 'Pf1..', 'Pf2..', 'Pf3..' and 'Tf1date'
// panes) are read from and written to the memory card through
// lbl_8051C1A0 (cls_803B9940). fn_803CE9EC runs the state field_0x110
// through the pointer-to-member table lbl_804AFD70 (40 entries).
//
// "FileSelect" is a provisional descriptive name (the "file_select.blo"
// resource the constructor loads); it keeps its vtable address.
class FileSelect_804AFF60 {
public:
    typedef void (FileSelect_804AFF60::*StateFunc)();

    FileSelect_804AFF60();
    /* 0x08 */ virtual ~FileSelect_804AFF60();

    // Fades the 'kk_' sprites (fn_803C6690 passes field_0xa4 and
    // 1 - progress).
    void fn_803BC6F0(s32 arg, f32 alpha);
    // fn_803CE4CC: back to the constructor's colours.
    void fn_803BDD44(s32 file);
    // fn_803CEED4: after the highlight update.
    void fn_803BE3D8();
    // Called by fn_803CBE38, fn_803CC3DC, fn_803CDEE8.
    void fn_803C24BC();
    void fn_803BF004();
    // fn_803C4054: the delete prompt's two answers.
    void fn_803BF3BC();
    void fn_803BF698();
    // fn_803CD2DC: the chosen file has save data (field_0x115 bit set).
    void fn_803BF9C4();
    void fn_803C0018();
    // fn_803C7CA0 (state 27): its two answers.
    void fn_803C02CC();
    void fn_803C1054();
    void fn_803C0A34();
    // fn_803C8B80: opens file field_0xa0's board (state 26).
    void fn_803C05D8();
    // fn_803CD2DC: the chosen file is empty.
    void fn_803C1734();
    // fn_803CABAC: the copy menu's open / close.
    void fn_803C1340();
    void fn_803C21A8();

    // State functions, lbl_804AFD70[state] (index in the comment).
    void fn_803C2E24(); // 39
    void fn_803C3904(); // 38
    void fn_803C4054(); // 37
    void fn_803C4330(); // 36
    void fn_803C4628(); // 35
    void fn_803C5190(); // 34
    void fn_803C579C(); // 33
    void fn_803C5808(); // 32
    void fn_803C61C4(); // 31
    void fn_803C6690(); // 30
    void fn_803C6798(); // 29
    void fn_803C70B8(); // 28
    void fn_803C7CA0(); // 27
    void fn_803C7F7C(); // 26
    void fn_803C8B80(); // 25
    void fn_803C92B8(); // 24
    void fn_803C9338(); // 23
    void fn_803C9898(); // 22
    void fn_803C9F14(); // 21
    void fn_803C9F80(); // 20
    void fn_803CABAC(); // 19
    void fn_803CB3DC(); // 18
    void fn_803CBE38(); // 17
    void fn_803CBEE0(); // 16: resets the console (OSResetSystem)
    void fn_803CBF0C(); // 15
    void fn_803CC048(); // 14
    void fn_803CC0D8(); // 13
    void fn_803CC17C(); // 12
    void fn_803CC2BC(); // 11
    void fn_803CC3DC(); // 10
    void fn_803CC490(); // 9
    void fn_803CC53C(); // 8
    void fn_803CC62C(); // 7
    void fn_803CC760(); // 6
    void fn_803CD2DC(); // 5
    void fn_803CD75C(); // 4
    void fn_803CDEE8(); // 3
    void fn_803CE05C(); // 2
    void fn_803CE11C(); // 1
    void fn_803CE370(); // 0

    // tu_80134C04 reads it each frame: 3 while no file select screen
    // exists, else field_0x1a8.
    static s32 fn_803CE4B0();
    void fn_803CE4CC();
    // tu_80134C04, each frame: runs the state, the 'kk_' sprite fade and the
    // message window.
    void fn_803CE9EC();
    void fn_803CEED4();
    // Sets up the rows of a file whose checksum failed (fn_803CE4CC).
    void fn_803BD5E0(s32 file);
    void fn_803CF504();

    // field_0x115 has a bit per file whose checksum failed (fn_803CE4CC sets
    // it when fn_803BADAC returns false and clears it otherwise).
    bool hasBadChecksum(s32 file) {
        u8 bit = 1 << file; // the u8 temporary gives the original's clrlwi before the and.
        return field_0x115 & bit;
    }
    void setBadChecksum(s32 file) {
        u8 bit = 1 << file;
        field_0x115 |= bit;
    }
    void clearBadChecksum(s32 file) {
        u8 bit = 1 << file;
        field_0x115 &= ~bit;
    }

    void setState(u8 state) {
        mPrevState = mState;
        mState = state;
    }
    // Shows message `msg` in mScreenHuuin's 'Tft01' (defined in
    // tu_803BC6F0.cpp; ten callers there have the same inlined code).
    inline void setMessage(u32 msg);

    /* 0x000 */ // vtable
    /* 0x004 */ J2DScreen* mScreen;      // "file_select.blo"
    /* 0x008 */ J2DScreen* mScreenHuuin; // "file_select_huuin.blo"
    /* 0x00C */ u8 field_0xc[0x10 - 0xC];
    /* 0x010 */ J2DPicture* field_0x10;  // "kk_black.bti"
    /* 0x014 */ PaneCtrl_804AF86C* field_0x14;
    /* 0x018 */ PaneCtrl_804AF86C* field_0x18;
    /* 0x01C */ PaneCtrl_804AF86C* field_0x1c;
    /* 0x020 */ PaneCtrl_804AF86C* field_0x20;
    /* 0x024 */ PaneCtrl_804AF86C* field_0x24[4];
    /* 0x034 */ PaneCtrl_804AF86C* field_0x34;
    /* 0x038 */ PaneCtrl_804AF86C* field_0x38[3];
    /* 0x044 */ PaneCtrl_804AF86C* field_0x44[3];
    /* 0x050 */ PaneCtrl_804AF86C* field_0x50;
    /* 0x054 */ PaneCtrl_804AF86C* field_0x54;
    /* 0x058 */ PaneCtrl_804AF86C* field_0x58; // lbl_8053AED8->field_0x8
    /* 0x05C */ SaveFile_803B950C* field_0x5c; // lbl_8050EC80.mFiles (memory card copy source)
    /* 0x060 */ SaveFileBody_803B950C* field_0x60; // lbl_8050EC80.mBodies
    /* 0x064 */ f32 field_0x64[3]; // field_0x44[i] target x (fn_803C6690)
    /* 0x070 */ f32 field_0x70[3]; // field_0x38[i] target x (fn_803C6690)
    /* 0x07C */ u32 field_0x7c; // message shown
    /* 0x080 */ u32 field_0x80; // message to show
    /* 0x084 */ u32 field_0x84;
    // Bounds of the 'Padhu01' (mScreenHuuin), 'Padhu02' and 'Padhu03'
    // (mScreen) panes, read by the constructor.
    /* 0x088 */ f32 field_0x88[3]; // getBounds()->i.x
    /* 0x094 */ f32 field_0x94[3]; // getBounds()->i.y
    /* 0x0A0 */ s32 field_0xa0;
    /* 0x0A4 */ s32 field_0xa4; // lbl_80529DEC.field_0x23b
    /* 0x0A8 */ s32 field_0xa8;
    /* 0x0AC */ s32 field_0xac;
    /* 0x0B0 */ s32 field_0xb0; // countdown; -1 when idle
    /* 0x0B4 */ s32 field_0xb4;
    /* 0x0B8 */ s32 field_0xb8[8]; // fn_803B8CAC(0, i)
    /* 0x0D8 */ s16 field_0xd8;
    /* 0x0DA */ s16 field_0xda;
    /* 0x0DC */ s16 field_0xdc;
    /* 0x0DE */ s16 field_0xde; // 'kk_' sprite fade frame
    // Colours of mScreenHuuin's pictures (J2DPicture getWhite / getBlack):
    // 'Pcopb01' in 0xE0 / 0xE4; 'Pf1t01', 'Pf1tb01', 'Pf1tbb01', 'Pf1b01'
    // and 'Pf1bb01' in field_0xe8 / field_0xfc.
    /* 0x0E0 */ JUtility::TColor field_0xe0; // white
    /* 0x0E4 */ JUtility::TColor field_0xe4; // black
    /* 0x0E8 */ JUtility::TColor field_0xe8[5]; // white
    /* 0x0FC */ JUtility::TColor field_0xfc[5]; // black
    /* 0x110 */ u8 mState;
    /* 0x111 */ u8 field_0x111; // last state seen by fn_803CE9EC
    /* 0x112 */ u8 mPrevState;
    /* 0x113 */ u8 field_0x113;
    /* 0x114 */ u8 field_0x114;
    /* 0x115 */ u8 field_0x115; // bit per file whose checksum failed (hasBadChecksum)
    /* 0x116 */ u8 field_0x116; // message window: 0 idle, 1 opening, 2 open, 3 closing
    /* 0x117 */ u8 field_0x117;
    // The panes' initial alphas (J2DPane::mAlpha), read by the constructor:
    // 'Pf1t01', 'Pf1tb01'..'05', 'Pf1tbb01'..'07', 'Tf1fir', 'Tf1lv',
    // 'Tf1date', 'Pf1b01'..'05', 'Pf1bb01'..'05' of mScreenHuuin.
    /* 0x118 */ u8 field_0x118[26];
    /* 0x132 */ u8 field_0x132[81]; // mScreen, lbl_80476618
    /* 0x183 */ u8 field_0x183[19]; // mScreenHuuin, lbl_804768A0
    /* 0x196 */ u8 field_0x196[9]; // alpha of the 'Pdb' / 'Tdb01' panes (fn_803CE11C)
    /* 0x19F */ u8 field_0x19f[4]; // mScreenHuuin 'Pbacb01'..'04'

    /* 0x1A3 */ u8 field_0x1a3;
    /* 0x1A4 */ u8 field_0x1a4; // 'kk_' sprite fade: 0 idle, 1 in, 2 out
    /* 0x1A5 */ u8 field_0x1a5;
    /* 0x1A6 */ u8 field_0x1a6;
    /* 0x1A7 */ u8 field_0x1a7;
    /* 0x1A8 */ s32 field_0x1a8;
}; // Size: 0x1AC

extern FileSelect_804AFF60* lbl_8053ADB0;

#endif
