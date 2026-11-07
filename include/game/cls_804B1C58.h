#ifndef GAME_CLS_804B1C58_H
#define GAME_CLS_804B1C58_H

#include "types.h"

class J2DScreen;
class PaneCtrl_804AF86C;

// Named after its vtable, 0x804B1C58, stored by its constructor fn_804485C4
// (game/tu_80447FAC.cpp, which also stores the object in lbl_8053AF10). The
// 0x30-byte object cls_804B1920 keeps at 0x2A8 (fn_8043F504 allocates it).
// Only the member cls_804B1920 calls is declared.
class cls_804B1C58 {
public:
    cls_804B1C58(); // fn_804485C4
    virtual ~cls_804B1C58(); // fn_804484C8
    // Owner: game/tu_80447FAC.cpp (0x80448334). Called with only the object by
    // cls_804B1920::fn_8043EDF8.
    void fn_80448334();
    // Owner: game/tu_80447FAC.cpp (0x80447FAC). Called with only the object by
    // cls_804B1920::fn_8043D8B4.
    void fn_80447FAC();
    // Owner: game/tu_80447FAC.cpp (0x804482EC). Called with only the object by
    // cls_804B1920::fn_8043DC98 (its draw).
    void fn_804482EC();

    /* 0x00 */ // vtable
    /* 0x04 */ J2DScreen* mScreen;
    /* 0x08 */ PaneCtrl_804AF86C* field_0x8[4]; // per player
    /* 0x18 */ PaneCtrl_804AF86C* field_0x18[4];
    /* 0x28 */ u8 field_0x28[4];
    /* 0x2C */ u8 field_0x2c;
}; // Size: 0x30

extern cls_804B1C58* lbl_8053AF10;

#endif
