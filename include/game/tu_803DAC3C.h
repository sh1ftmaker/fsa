#ifndef GAME_TU_803DAC3C_H
#define GAME_TU_803DAC3C_H

#include "types.h"

class J2DScreen;
class J2DPicture;
class cls_804AF86C;
class cls_804AF860;
class THPMovie_804B15D0;

// The game mode screen: vtable 0x804B0688 (one slot, the destructor
// fn_803DFC6C), constructor fn_803DFF40 (game/tu_803DAC3C.cpp); 0x120 bytes
// (__nw__(0x120) in tu_80134C04's fn_80135020 path). The constructor loads
// "gamemode.blo" and "gamemode_movie.blo" from lbl_80529DEC's field_0x2c
// archive, the GBA tile data ("gb_ch_obj_base_sch.szs", "player_sch.szs",
// ...) into lbl_8053AB10->field_0x20, and stores `this` in lbl_8053ADD0.
// tu_80134C04 calls fn_803DF810 (update) and fn_803DF624 (draw) each frame;
// both run mState through a pointer-to-member table (lbl_804B0568 /
// lbl_804B0628, 8 entries). State 7 boots "/DOL/framework.dol".
//
// "GameMode" is a provisional descriptive name (the "gamemode.blo" resource
// the constructor loads); it keeps its vtable address.
class GameMode_804B0688 {
public:
    typedef void (GameMode_804B0688::*StateFunc)();

    GameMode_804B0688();
    /* 0x08 */ virtual ~GameMode_804B0688();

    // tu_80134C04 reads it each frame: 2 while no game mode screen exists,
    // else field_0x68.
    static s32 fn_803DAC3C();
    // Sets the panes' colours for the selected entry (0-2) with alpha
    // factor `alpha`.
    void fn_803DAC58(s32 entry, f32 alpha);
    void fn_803DCCA4();

    // Draw states, lbl_804B0628[mState] (index in the comment).
    void fn_803DD19C(); // 5, 6, 7
    void fn_803DD1A0(); // 4
    void fn_803DD1A4(); // 3
    void fn_803DD3DC(); // 2
    void fn_803DD3E0(); // 1
    void fn_803DD760(); // 0
    // Update states, lbl_804B0568[mState].
    void fn_803DD764(); // 7: fades out, then boots "/DOL/framework.dol"
    void fn_803DD8E8(); // 6
    void fn_803DD93C(); // 5
    void fn_803DD970(); // 4
    void fn_803DE05C(); // 3
    void fn_803DE58C(); // 2
    void fn_803DEC44(); // 1
    void fn_803DF2B4(); // 0

    // tu_80134C04, each frame.
    void fn_803DF624(); // draw
    void fn_803DF810(); // update
    void fn_803DF9E8(); // reset

    /* 0x000 */ // vtable
    /* 0x004 */ J2DScreen* mScreen;      // "gamemode.blo"
    /* 0x008 */ J2DScreen* mMovieScreen; // "gamemode_movie.blo"
    /* 0x00C */ u8 field_0xc[0x10 - 0xC];
    /* 0x010 */ J2DPicture* field_0x10;  // "kk_black.bti"
    /* 0x014 */ cls_804AF86C* field_0x14[7];
    /* 0x030 */ cls_804AF86C* field_0x30;
    /* 0x034 */ cls_804AF86C* field_0x34[2];
    /* 0x03C */ u8 field_0x3c[0x40 - 0x3C];
    /* 0x040 */ cls_804AF86C* field_0x40;
    /* 0x044 */ cls_804AF86C* field_0x44[2];
    /* 0x04C */ u8 field_0x4c[0x50 - 0x4C];
    /* 0x050 */ cls_804AF86C* field_0x50[2];
    /* 0x058 */ cls_804AF86C* field_0x58;
    /* 0x05C */ cls_804AF86C* field_0x5c; // lbl_8053AED8->field_0x8
    /* 0x060 */ cls_804AF860* field_0x60;
    /* 0x064 */ THPMovie_804B15D0* mMovie;
    /* 0x068 */ s32 field_0x68;
    /* 0x06C */ f32 field_0x6c;
    /* 0x070 */ f32 field_0x70;
    /* 0x074 */ u8 field_0x74[0x78 - 0x74];
    /* 0x078 */ f32 field_0x78; // fade value (0-255)
    /* 0x07C */ f32 field_0x7c; // fade value (0-255)
    /* 0x080 */ s32 field_0x80;
    /* 0x084 */ s32 mState;
    /* 0x088 */ s32 field_0x88;
    /* 0x08C */ s32 field_0x8c;
    /* 0x090 */ s32 field_0x90;
    /* 0x094 */ s32 field_0x94;
    /* 0x098 */ s16 field_0x98;
    /* 0x09A */ s16 field_0x9a;
    /* 0x09C */ s16 field_0x9c;
    /* 0x09E */ u8 field_0x9e; // selected entry
    /* 0x09F */ u8 field_0x9f;
    /* 0x0A0 */ u8 mMovieState; // 0 idle, 1 start, 2 playing, 3 quit
    /* 0x0A1 */ u8 field_0xa1;
    /* 0x0A2 */ u8 field_0xa2;
    // The panes' initial alphas (J2DPane::mAlpha), read by the constructor
    // through the tag tables of the same lengths.
    /* 0x0A3 */ u8 field_0xa3[50];
    /* 0x0D5 */ u8 field_0xd5[6];
    /* 0x0DB */ u8 field_0xdb[17];
    /* 0x0EC */ u8 field_0xec[20];
    /* 0x100 */ u8 field_0x100[16]; // the 'Pmnv' table's (never read)
    /* 0x110 */ u8 field_0x110[5];
    /* 0x115 */ u8 field_0x115[5];
    /* 0x11A */ u8 field_0x11a[4]; // mMovieScreen 'Pim01', 'Pim02', 'Pim11', 'Pim12'
    /* 0x11E */ u8 field_0x11e[0x120 - 0x11E];
}; // Size: 0x120

extern GameMode_804B0688* lbl_8053ADD0;

#endif
