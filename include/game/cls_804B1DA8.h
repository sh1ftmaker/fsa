#ifndef GAME_CLS_804B1DA8_H
#define GAME_CLS_804B1DA8_H

#include "types.h"

class J2DScreen;
class PaneCtrl_804AF86C;

// Vtable 0x804B1DA8 (one slot, the destructor), 0x5C bytes, defined in
// game/tu_80453AF8.cpp (not written yet). The constructor fn_80454DEC stores
// `this` in lbl_8053AF48; tu_8037ACC4 allocates it with __nw__(0x5C) when
// gGameRoot_8053AAF8->field_0x7f is clear.
class cls_804B1DA8 {
public:
    cls_804B1DA8(); // fn_80454DEC
    /* 0x08 */ virtual ~cls_804B1DA8();

    s32 fn_80453B14(); // 0x80453B14
    void fn_80454A00(); // 0x80454A00
    void fn_80454A6C(); // 0x80454A6C

    /* 0x04 */ J2DScreen* mScreen;
    /* 0x08 */ PaneCtrl_804AF86C* field_0x8;
    /* 0x0C */ PaneCtrl_804AF86C* field_0xc;
    /* 0x10 */ PaneCtrl_804AF86C* field_0x10[3];
    /* 0x1C */ s16 field_0x1c; // the selected row
    /* 0x1E */ s16 field_0x1e;
    /* 0x20 */ s16 field_0x20;
    /* 0x22 */ u8 field_0x22[0x24 - 0x22];
    /* 0x24 */ f32 field_0x24[3]; // pane positions the constructor reads
    /* 0x30 */ f32 field_0x30[3];
    /* 0x3C */ f32 field_0x3c[3];
    /* 0x48 */ f32 field_0x48[3];
    /* 0x54 */ u8 field_0x54; // fn_8037ADD8 / fn_8037AE00 / fn_8037AE30
    /* 0x55 */ u8 field_0x55;
    /* 0x56 */ u8 field_0x56[0x58 - 0x56];
    /* 0x58 */ s32 field_0x58; // fn_80453AF8 returns it
    /* 0x5C */ // size
};

// Data of game/tu_80453AF8.cpp.
extern cls_804B1DA8* lbl_8053AF48;

// 0x80453AF8 (game/tu_80453AF8.cpp): 9 without lbl_8053AF48, else its
// field_0x58.
s32 fn_80453AF8();

#endif
