#ifndef GAME_CLS_804AE9B0_H
#define GAME_CLS_804AE9B0_H

#include "types.h"

class cls_804B1D58;
class cls_804B1DA8;

// Vtable 0x804AE9B0 (one slot, the destructor fn_8037B000), 0x10 bytes,
// defined in game/tu_8037ACC4.cpp. tu_80134C04 allocates it with
// __nw__(0x10) and constructs it with fn_8037B0B8, which stores it in
// lbl_8053AD70. It holds one of two objects, picked by
// gGameRoot_8053AAF8->field_0x7f; the member functions below forward to it
// (this is unused except by fn_8037AEAC and fn_8037AFC0).
class cls_804AE9B0 {
public:
    cls_804AE9B0(); // fn_8037B0B8
    /* 0x08 */ virtual ~cls_804AE9B0(); // fn_8037B000

    // 0x8037ACC4..0x8037AFC0: called with lbl_8053AD70 in r3.
    u8 fn_8037ACC4();
    void fn_8037AD00(int arg);
    u8 fn_8037AD34();
    void fn_8037AD70(int arg);
    void fn_8037ADA4();
    u8 fn_8037ADD8();
    void fn_8037AE00();
    void fn_8037AE30();
    s32 fn_8037AEAC();
    void fn_8037AF4C();
    void fn_8037AF80();
    void fn_8037AFC0();

    /* 0x4 */ cls_804B1D58* field_0x4; // when 0x7f is set
    /* 0x8 */ cls_804B1DA8* field_0x8; // when 0x7f is clear
    /* 0xC */ u8 field_0xc;
}; // Size: 0x10

// Defined in game/tu_8037ACC4.cpp (.sbss 0x8053AD70).
extern cls_804AE9B0* lbl_8053AD70;

// 0x8037AE60 (tu_8037ACC4.cpp): 9 without lbl_8053AD70, else the 0x184 word
// of the object at lbl_8053AF40 (0x7f set) or the 0x58 word of the one at
// lbl_8053AF48. Called by tu_80134C04.
s32 fn_8037AE60();

#endif
