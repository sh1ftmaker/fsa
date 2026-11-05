#ifndef GAME_ACT_TU_80440BF4_H
#define GAME_ACT_TU_80440BF4_H

// Code: game/tu_80440BF4.cpp. Created in game/tu_80134C04.cpp (0x80139460,
// __nw__ 0x14 into its field_0x3c), requested through lbl_8053AED8 by
// tu_803D7098, tu_80403D84, tu_80414F6C, tu_803A60E4 and others.

#include "types.h"

class J2DScreen;
struct ResTIMG;
class PaneCtrl_804AF86C;

// A button-icon overlay built from "button.blo". Named after its vtable,
// 0x804B1988 (only the destructor, fn_80440F30); the constructor is
// fn_80440FDC, which stores the object in lbl_8053AED8. The panes are the
// seven 'Pba01'..'Pba04' / 'Pbb01'..'Pbb03' ones; the callers pass textures
// from main2D.arc (fn_804030C4) to the 'Pba04'/'Pbb02' pictures.
class cls_804B1988 {
public:
    cls_804B1988();
    virtual ~cls_804B1988();

    // Shows or hides the 'Pbb03' pane.
    void fn_80440BF4(bool show);
    // Shows or hides the 'Pba04' pane.
    void fn_80440C6C(bool show);
    // Shows 'Pbb03' and swaps the 'Pbb02' picture's texture for tex.
    void fn_80440CE4(const ResTIMG* tex);
    // Shows 'Pba04' and swaps the 'Pba02' picture's texture for tex.
    void fn_80440D78(const ResTIMG* tex);
    // Sets the seven panes' alpha to their initial alpha times alpha.
    void fn_80440E0C(f32 alpha);
    // Draws the screen.
    void fn_80440EE8();

    /* 0x00 */ // vtable
    /* 0x04 */ J2DScreen* mScreen;
    /* 0x08 */ PaneCtrl_804AF86C* field_0x8; // 'Ndata01' pane (tu_803D7098 reads it)
    /* 0x0C */ u8 mAlpha[7];            // initial alpha of the seven panes
    /* 0x13 */ u8 pad_0x13;
}; // Size: 0x14

extern cls_804B1988* lbl_8053AED8;

#endif
