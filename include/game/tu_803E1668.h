#ifndef GAME_TU_803E1668_H
#define GAME_TU_803E1668_H

#include "types.h"

// Vtable 0x804B0B00, constructor fn_803EE254 (game/tu_803E1668.cpp, owner
// unit, not written yet); 0x364 bytes (__nw__(0x364) in fn_803A8884). Only
// what game/tu_803A85E4.cpp uses is declared.
class cls_804B0B00 {
public:
    cls_804B0B00();
    /* 0x08 */ virtual ~cls_804B0B00();
    void fn_803ED83C();
    void fn_803EDA2C();
    // `mode` is tested after clrlwi 24 (switch on 0, 1, 2).
    void fn_803EDBF0(u8 mode);

    /* 0x004 */ u8 field_0x4[0x360 - 0x4];
    /* 0x360 */ s32 field_0x360; // fn_803A8600 copies it to cls_804AF878::field_0xc
};

#endif
