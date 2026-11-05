#ifndef GAME_TU_803BC6F0_H
#define GAME_TU_803BC6F0_H

#include "types.h"
#include "JSystem/JUtility/TColor.h"

class J2DScreen;
class J2DPicture;
class cls_804AF86C;

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

    // Called by fn_803CBE38, fn_803CC3DC, fn_803CDEE8 (not written yet).
    void fn_803C24BC();
    void fn_803BF004();
    void fn_803C0018();
    void fn_803C0A34();

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
    void fn_803CF504();

    void setState(u8 state) {
        mPrevState = mState;
        mState = state;
    }

    /* 0x000 */ // vtable
    /* 0x004 */ J2DScreen* mScreen;      // "file_select.blo"
    /* 0x008 */ J2DScreen* mScreenHuuin; // "file_select_huuin.blo"
    /* 0x00C */ u8 field_0xc[0x10 - 0xC];
    /* 0x010 */ J2DPicture* field_0x10;  // "kk_black.bti"
    /* 0x014 */ cls_804AF86C* field_0x14;
    /* 0x018 */ cls_804AF86C* field_0x18;
    /* 0x01C */ cls_804AF86C* field_0x1c;
    /* 0x020 */ cls_804AF86C* field_0x20;
    /* 0x024 */ cls_804AF86C* field_0x24[4];
    /* 0x034 */ cls_804AF86C* field_0x34;
    /* 0x038 */ cls_804AF86C* field_0x38[3];
    /* 0x044 */ cls_804AF86C* field_0x44[3];
    /* 0x050 */ cls_804AF86C* field_0x50;
    /* 0x054 */ cls_804AF86C* field_0x54;
    /* 0x058 */ cls_804AF86C* field_0x58; // lbl_8053AED8->field_0x8
    /* 0x05C */ void* field_0x5c; // &lbl_8050EC80[0] + 0x20 (memory card copy source)
    /* 0x060 */ void* field_0x60; // &lbl_8050EC80[0] + 0x280
    /* 0x064 */ u8 field_0x64[0x7C - 0x64];
    /* 0x07C */ u32 field_0x7c; // message shown
    /* 0x080 */ u32 field_0x80; // message to show
    /* 0x084 */ u32 field_0x84;
    /* 0x088 */ u8 field_0x88[0xA0 - 0x88];
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
    /* 0x0E0 */ s32 field_0xe0;
    /* 0x0E4 */ s32 field_0xe4;
    /* 0x0E8 */ JUtility::TColor field_0xe8[5];
    /* 0x0FC */ JUtility::TColor field_0xfc[5];
    /* 0x110 */ u8 mState;
    /* 0x111 */ u8 field_0x111; // last state seen by fn_803CE9EC
    /* 0x112 */ u8 mPrevState;
    /* 0x113 */ u8 field_0x113;
    /* 0x114 */ u8 field_0x114;
    /* 0x115 */ u8 field_0x115;
    /* 0x116 */ u8 field_0x116; // message window: 0 idle, 1 opening, 2 open, 3 closing
    /* 0x117 */ u8 field_0x117;
    /* 0x118 */ u8 field_0x118[0x1A3 - 0x118];
    /* 0x1A3 */ u8 field_0x1a3;
    /* 0x1A4 */ u8 field_0x1a4; // 'kk_' sprite fade: 0 idle, 1 in, 2 out
    /* 0x1A5 */ u8 field_0x1a5;
    /* 0x1A6 */ u8 field_0x1a6;
    /* 0x1A7 */ u8 field_0x1a7;
    /* 0x1A8 */ s32 field_0x1a8;
}; // Size: 0x1AC

extern FileSelect_804AFF60* lbl_8053ADB0;

#endif
