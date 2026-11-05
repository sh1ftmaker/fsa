#ifndef GAME_TU_80447618_H
#define GAME_TU_80447618_H

// Code: game/tu_80447618.cpp. Named after its vtable, 0x804B1C20 (only the
// virtual destructor, fn_80447B34, in its first slot); the constructor is
// fn_80447C30, which stores the object in lbl_8053AF08. The object is the
// 0x30-byte field at 0x2A4 of tu_8043A0E8's object (cls_804B1920, whose
// fn_8043F504 allocates it), and that object's functions call into it
// (fn_80447618, fn_80447958, fn_804479A0).

#include "types.h"

class J2DScreen;
class PaneCtrl_804AF86C;

class cls_804B1C20 {
public:
    cls_804B1C20();
    virtual ~cls_804B1C20();

    // Draws the screen: gGameRoot2_8053AB10->fn_8013B490(), then mScreen at (0, 0)
    // with gGameRoot2_8053AB10's J2DOrthoGraph as the grafcontext.
    void fn_80447958();
    // 0x80447618: called with only the object by cls_804B1920::fn_8043D8B4.
    void fn_80447618();
    // 0x804479A0: called with only the object by cls_804B1920::fn_8043EDF8.
    void fn_804479A0();

    /* 0x00 */ // vtable
    /* 0x04 */ J2DScreen* mScreen; // "mg_kokko.blo"
    /* 0x08 */ PaneCtrl_804AF86C* field_0x8[4];  // one per pane tag 'Ndata01', 'Ndata04', 'Ndata07', 'Ndata10'
    /* 0x18 */ PaneCtrl_804AF86C* field_0x18[4]; // one per pane tag 'Ndata20'..'Ndata23'
    /* 0x28 */ u8 field_0x28[5];            // cleared by the constructor; indexed by tu_8034AA40
}; // Size: 0x30

// The object at lbl_8053AF08 (game/tu_80447618.cpp, constructor fn_80447C30;
// the auto split at 0x8053AF08 defines it). tu_8034AA40 indexes its byte
// array at 0x28 and ORs bits into 0x2C.
extern cls_804B1C20* lbl_8053AF08;

#endif
