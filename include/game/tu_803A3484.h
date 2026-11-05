#ifndef GAME_TU_803A3484_H
#define GAME_TU_803A3484_H

#include "types.h"
#include "game/cls_8016ED88.h"

class J2DScreen;
class J2DPicture;
class J2DAnmTextureSRTKey;
class J3DModel;
class J3DAnmTextureSRTKey;
class J3DAnmTevRegKey;
class JKRSolidHeap;
class JPABaseEmitter;
class THPMovie_804B15D0;

// The title screen: vtable 0x804AF820 (one slot, the destructor fn_803A56C4),
// constructor fn_803A59E0 (game/tu_803A3484.cpp); 0x158 bytes (__nw__(0x158)
// in tu_80134C04's fn_80135020 path). The constructor loads "title.blo" /
// "title.btk" and the pictures ("kk_titleCon_01.bti", the progressive-scan
// prompt "progressive_*.bti", "kk_black.bti"), builds the six cloud models
// ("w_kumo*.bmd") in its own solid heap and stores `this` in lbl_8053AD88.
// tu_80134C04 calls fn_803A34A0 (update), fn_803A44D8 (draw) and fn_803A5380
// (reset) and reads fn_803A3484 each frame.
//
// "Title" is a provisional descriptive name (the "title.blo" / "title.arc"
// resources the constructor loads); it keeps its vtable address.
class Title_804AF820 {
public:
    Title_804AF820();
    /* 0x08 */ virtual ~Title_804AF820();

    // tu_80134C04 reads it each frame: 1 while no title screen exists, else
    // field_0x154.
    static s32 fn_803A3484();
    void fn_803A34A0(); // update
    void fn_803A44D8(); // draw
    void fn_803A5380(); // reset

    // Quits the movie, picks the next one and restarts the title's timers
    // (inlined three times into fn_803A34A0, game/tu_803A3484.cpp).
    void stopMovie();
    // The 'prsstrt' blink factor: steps field_0x7c and returns the panes'
    // alpha scale (inlined twice into fn_803A44D8).
    void calcBlink(f32& blink);
    // Draws the title screen with the 'prsstrt' panes scaled by `blink`
    // (inlined twice into fn_803A44D8).
    void drawTitle(f32 blink);

    /* 0x000 */ // vtable
    /* 0x004 */ J2DScreen* mScreen;          // "title.blo"
    /* 0x008 */ J2DPicture* field_0x8;       // "kk_titleCon_01.bti"
    /* 0x00C */ J2DPicture* field_0xc;       // "progressive_choice.bti"
    /* 0x010 */ J2DPicture* field_0x10;      // "progressive_inter.bti"
    /* 0x014 */ J2DPicture* field_0x14;      // "progressive_pro.bti"
    /* 0x018 */ J2DPicture* field_0x18;      // "progressive_yes.bti"
    /* 0x01C */ J2DPicture* field_0x1c;      // "progressive_no.bti"
    /* 0x020 */ J2DPicture* field_0x20;      // "kk_black.bti"
    /* 0x024 */ J2DAnmTextureSRTKey* mAnm;   // "title.btk"
    /* 0x028 */ JPABaseEmitter* field_0x28[3];
    /* 0x034 */ JPABaseEmitter* field_0x34[4];
    /* 0x044 */ JPABaseEmitter* field_0x44;
    /* 0x048 */ THPMovie_804B15D0* mMovie;
    /* 0x04C */ f32 field_0x4c;
    /* 0x050 */ f32 field_0x50;
    /* 0x054 */ s32 field_0x54;
    /* 0x058 */ s32 field_0x58; // the movie to start next
    /* 0x05C */ s32 field_0x5c;
    /* 0x060 */ s32 field_0x60;
    /* 0x064 */ s32 field_0x64;
    /* 0x068 */ s16 field_0x68;
    /* 0x06A */ s16 field_0x6a;
    /* 0x06C */ s16 mMovieState; // 0 none, 1 playing, 2 the attract movie
    /* 0x06E */ s16 field_0x6e;
    /* 0x070 */ u8 field_0x70[0x72 - 0x70];
    /* 0x072 */ s16 field_0x72;
    /* 0x074 */ s16 field_0x74;
    /* 0x076 */ s16 field_0x76;
    /* 0x078 */ s16 field_0x78;
    /* 0x07A */ s16 field_0x7a;
    /* 0x07C */ s16 field_0x7c;
    /* 0x07E */ s16 field_0x7e;
    /* 0x080 */ s16 field_0x80;
    /* 0x082 */ s16 field_0x82;
    // Passed by address to fn_8016D398 with field_0x90 (draw).
    /* 0x084 */ s32 field_0x84;
    /* 0x088 */ s32 field_0x88;
    /* 0x08C */ u8 field_0x8c;
    /* 0x090 */ cls_8016ED88 field_0x90;
    /* 0x100 */ JKRSolidHeap* mHeap;
    // The six cloud models of lbl_804AF7D8 (bmd, btk, brk).
    /* 0x104 */ J3DModel* field_0x104[6];
    /* 0x11C */ J3DAnmTextureSRTKey* field_0x11c[6];
    /* 0x134 */ J3DAnmTevRegKey* field_0x134[6];
    /* 0x14C */ u8 field_0x14c;
    /* 0x14D */ u8 field_0x14d;
    /* 0x14E */ u8 field_0x14e; // 'prsstrt1' pane's initial alpha
    /* 0x14F */ u8 field_0x14f; // 'prsstrt2' pane's initial alpha
    /* 0x150 */ u8 field_0x150;
    /* 0x151 */ u8 field_0x151[0x154 - 0x151];
    /* 0x154 */ s32 field_0x154;
}; // Size: 0x158

extern Title_804AF820* lbl_8053AD88;

#endif
