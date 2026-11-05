#ifndef GAME_CLS_804B16A8_H
#define GAME_CLS_804B16A8_H

#include "types.h"

// Vtable 0x804B16A8, constructor fn_804372C4 (game/tu_80434370.cpp, owner
// unit, not written yet); 0x70 bytes (__nw__(0x70) in tu_803F0A54's and
// tu_803E1668's constructors). A J2DScreen (0x4) with a state machine run
// through the pointer-to-member table lbl_804B1690 indexed by the byte 0x6B.
// Only what game/tu_803F0A54.cpp uses is declared.
class cls_804B16A8 {
public:
    cls_804B16A8();
    /* 0x08 */ virtual ~cls_804B16A8();

    // 0x80436668: draws the screen at (0, 0) with gGameRoot2_8053AB10's grafcontext,
    // then lbl_8053AED8's (fn_80440EE8).
    void fn_80436668();
    // 0x804366B8: runs lbl_804B1690[field_0x6b], then returns whether
    // field_0x6b is 2 (subfic / cntlzw / srwi: a word, not a bool).
    s32 fn_804366B8();
    // 0x8043670C: starts the screen; the argument is stored at 0x6C.
    void fn_8043670C(u8 arg);

    /* 0x04 */ u8 field_0x4[0x70 - 0x4];
}; // Size: 0x70

#endif
