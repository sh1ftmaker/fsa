#ifndef GAME_TU_803F0A54_H
#define GAME_TU_803F0A54_H

#include "types.h"

// Vtable 0x804B0F10, constructor fn_803FCBC4 (game/tu_803F0A54.cpp, owner
// unit, not written yet); 0x354 bytes (__nw__(0x354) in fn_803A8884). Only
// what game/tu_803A85E4.cpp uses is declared.
class cls_804B0F10 {
public:
    cls_804B0F10();
    /* 0x08 */ virtual ~cls_804B0F10();
    void fn_803FC304();
    void fn_803FC46C();
    // `flag` is tested after clrlwi. 24.
    void fn_803FC5E4(u8 flag);

    /* 0x004 */ u8 field_0x4[0x350 - 0x4];
    /* 0x350 */ s32 field_0x350; // fn_803A8600 copies it to cls_804AF878::field_0xc
};

#endif
