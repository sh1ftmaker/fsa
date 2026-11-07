#ifndef GAME_CLS_804B16A8_H
#define GAME_CLS_804B16A8_H

#include "types.h"

class J2DScreen;
class PaneCtrl_804AF86C;

// Vtable 0x804B16A8, constructor fn_804372C4 (game/tu_80434370.cpp, owner
// unit); 0x70 bytes (__nw__(0x70) in tu_803F0A54's and
// tu_803E1668's constructors). A J2DScreen (0x4) with a state machine run
// through the pointer-to-member table lbl_804B1690 indexed by the byte 0x6B.
class cls_804B16A8 {
public:
    cls_804B16A8();
    /* 0x08 */ virtual ~cls_804B16A8();

    void fn_80434370();
    void fn_80434724();
    void fn_80435E20();
    void fn_804361A8();
    void fn_8043651C();
    // 0x80436668: draws the screen at (0, 0) with gGameRoot2_8053AB10's grafcontext,
    // then lbl_8053AED8's (fn_80440EE8).
    void fn_80436668();
    // 0x804366B8: runs lbl_804B1690[field_0x6b], then returns whether
    // field_0x6b is 2 (subfic / cntlzw / srwi: a word, not a bool).
    s32 fn_804366B8();
    // 0x8043670C: starts the screen; the argument is stored at 0x6C.
    void fn_8043670C(u8 arg);

    /* 0x04 */ J2DScreen* mScreen;
    /* 0x08 */ PaneCtrl_804AF86C* field_0x8;
    /* 0x0C */ PaneCtrl_804AF86C* field_0xc[2];
    /* 0x14 */ PaneCtrl_804AF86C* field_0x14;
    /* 0x18 */ PaneCtrl_804AF86C* field_0x18;
    /* 0x1C */ PaneCtrl_804AF86C* field_0x1c[6];
    /* 0x34 */ PaneCtrl_804AF86C* field_0x34;
    /* 0x38 */ PaneCtrl_804AF86C* field_0x38;
    /* 0x3C */ f32 field_0x3c;
    /* 0x40 */ f32 field_0x40;
    /* 0x44 */ f32 field_0x44;
    /* 0x48 */ f32 field_0x48;
    /* 0x4C */ f32 field_0x4c;
    /* 0x50 */ f32 field_0x50;
    /* 0x54 */ s32 field_0x54;
    /* 0x58 */ s16 field_0x58;
    /* 0x5A */ s16 field_0x5a;
    /* 0x5C */ s16 field_0x5c;
    /* 0x5E */ s16 field_0x5e;
    /* 0x60 */ s16 field_0x60;
    /* 0x62 */ s16 field_0x62;
    /* 0x64 */ s16 field_0x64;
    /* 0x66 */ s16 field_0x66;
    /* 0x68 */ u8 field_0x68;
    /* 0x69 */ u8 field_0x69;
    /* 0x6A */ u8 field_0x6a;
    /* 0x6B */ u8 field_0x6b;
    /* 0x6C */ u8 field_0x6c;
}; // Size: 0x70

#endif
