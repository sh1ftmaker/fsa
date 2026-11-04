#ifndef GAME_CLS_804B1D58_H
#define GAME_CLS_804B1D58_H

#include "types.h"

class J2DScreen;

// 0x10-byte element of field_0x164, named after its constructor
// __ct__12cls_80453830Fv (fn_80453830, game/tu_8044C7F0.cpp), which
// __construct_array runs on the two elements.
struct cls_80453830 {
    cls_80453830(); // out-of-line (game/tu_8044C7F0.cpp): every word -1

    /* 0x0 */ s32 field_0x0;
    /* 0x4 */ s32 field_0x4;
    /* 0x8 */ s32 field_0x8;
    /* 0xC */ s32 field_0xc;
}; // Size: 0x10

// Vtable 0x804B1D58 (one slot, the destructor), 0x188 bytes, defined in
// game/tu_8044C7F0.cpp (not written yet). The constructor fn_80452A80 stores
// `this` in lbl_8053AF40; tu_8037ACC4 allocates it with __nw__(0x188) when
// lbl_8053AAF8->field_0x7f is set.
class cls_804B1D58 {
public:
    cls_804B1D58(); // fn_80452A80
    /* 0x08 */ virtual ~cls_804B1D58();

    u8 fn_8044CB00(); // 0x8044CB00: field_0x140
    void fn_8044CB08(int arg); // 0x8044CB08
    u8 fn_8044D290(); // 0x8044D290: field_0x140
    void fn_8044D298(int arg); // 0x8044D298
    void fn_8044D894(); // 0x8044D894
    s32 fn_8044F680(); // 0x8044F680
    void fn_804522C8(); // 0x804522C8
    void fn_80452308(); // 0x80452308
    void fn_80452350(); // 0x80452350

    /* 0x004 */ J2DScreen* mScreen; // drawn by fn_80452308
    /* 0x008 */ u8 field_0x8[0xFA - 0x8];
    // fn_804522C8 tests it against 7, 8 and 9 (lha).
    /* 0x0FA */ s16 field_0xfa;
    /* 0x0FC */ u8 field_0xfc[0x13F - 0xFC];
    /* 0x13F */ u8 field_0x13f; // fn_8037ADD8 / fn_8037AE00 / fn_8037AE30
    /* 0x140 */ u8 field_0x140; // fn_8044CB00 returns it
    /* 0x141 */ u8 field_0x141[0x164 - 0x141];
    /* 0x164 */ cls_80453830 field_0x164[2]; // constructed by __construct_array (0x10 x 2)
    /* 0x184 */ s32 field_0x184; // fn_8044F664 returns it
}; // Size: 0x188

// Data of game/tu_8044C7F0.cpp.
extern cls_804B1D58* lbl_8053AF40;

// 0x8044F664 (game/tu_8044C7F0.cpp): 9 without lbl_8053AF40, else its
// field_0x184.
s32 fn_8044F664();

#endif
