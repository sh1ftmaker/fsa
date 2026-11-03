#ifndef GAME_CLS_804B1288_H
#define GAME_CLS_804B1288_H

#include "types.h"

// Vtable 0x804B1288 (one slot, the destructor), 0x66C bytes, defined in
// game/tu_80403D84.cpp (not written yet). The constructor fn_804122F4 stores
// `this` in lbl_8053ADF8; tu_8039003C allocates it with __nw__(0x66C) when
// lbl_8053AAF8->field_0x7f is set.
class cls_804B1288 {
public:
    cls_804B1288(); // fn_804122F4
    /* 0x08 */ virtual ~cls_804B1288();

    // 0x8040F480: stores 6 in field_0x668, then sets the object up.
    void fn_8040F480();
    void fn_804108B4(); // 0x804108B4
    void fn_804111C4(); // 0x804111C4
    void fn_80411230(); // 0x80411230

    /* 0x004 */ u8 field_0x4[0x668 - 0x4];
    /* 0x668 */ s32 field_0x668; // fn_8040F474 returns it
}; // Size: 0x66C

// Data of game/tu_80401848.cpp.
extern cls_804B1288* lbl_8053ADF8;

// 0x8040F474 (game/tu_80403D84.cpp): lbl_8053ADF8->field_0x668.
s32 fn_8040F474();

#endif
