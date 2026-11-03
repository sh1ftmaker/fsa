#ifndef GAME_CLS_804B1378_H
#define GAME_CLS_804B1378_H

#include "types.h"

// Vtable 0x804B1378 (one slot, the destructor), 0x154 bytes, defined in
// game/tu_80414F6C.cpp (not written yet). The constructor fn_8041787C stores
// `this` in lbl_8053AE00; tu_8039003C allocates it with __nw__(0x154) when
// lbl_8053AAF8->field_0x7f is clear.
class cls_804B1378 {
public:
    cls_804B1378(); // fn_8041787C
    /* 0x08 */ virtual ~cls_804B1378();

    void fn_80416BD0(); // 0x80416BD0
    void fn_804170B0(); // 0x804170B0
    void fn_804170EC(); // 0x804170EC

    /* 0x004 */ u8 field_0x4[0xE0 - 0x4];
    /* 0x0E0 */ s32 field_0xe0; // fn_80414F6C returns it
    /* 0x0E4 */ u8 field_0xe4[0x154 - 0xE4];
}; // Size: 0x154

// Data of game/tu_80401848.cpp.
extern cls_804B1378* lbl_8053AE00;

// 0x80414F6C (game/tu_80414F6C.cpp): lbl_8053AE00->field_0xe0.
s32 fn_80414F6C();

#endif
