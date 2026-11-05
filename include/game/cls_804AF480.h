#ifndef GAME_CLS_804AF480_H
#define GAME_CLS_804AF480_H

#include "types.h"

class cls_804B1288;
class BattleStageSelect_804B1378;

// Vtable 0x804AF480 (one slot, the destructor fn_8039017C), 0x10 bytes,
// defined in game/tu_8039003C.cpp. tu_80134C04 allocates it with
// __nw__(0x10, heap, 0) (0x801352B4) and constructs it with fn_8039027C,
// which stores it in lbl_8053AD80. It holds one of two objects, picked by
// lbl_8053AAF8->field_0x7f; the member functions below only forward to it
// (this is unused; they read lbl_8053AD80's objects directly).
class cls_804AF480 {
public:
    cls_804AF480(); // fn_8039027C
    /* 0x08 */ virtual ~cls_804AF480(); // fn_8039017C

    // 0x80390088 / 0x803900C8 / 0x80390108 / 0x8039013C: called by
    // tu_80134C04 with lbl_8053AD80 in r3.
    void fn_80390088();
    void fn_803900C8();
    void fn_80390108();
    void fn_8039013C();

    /* 0x4 */ cls_804B1288* field_0x4; // when 0x7f is set
    /* 0x8 */ BattleStageSelect_804B1378* field_0x8; // when 0x7f is clear
    /* 0xC */ u32 field_0xc; // not used here
}; // Size: 0x10 (tu_80134C04 allocates 0x10)

// Defined in game/tu_8039003C.cpp (.sbss 0x8053AD80).
extern cls_804AF480* lbl_8053AD80;

// 0x8039003C (tu_8039003C.cpp): 6 without lbl_8053AD80, else the 0x668 word
// of the object at lbl_8053ADF8 (0x7f set) or the 0xE0 word of the one at
// lbl_8053AE00. Called by tu_80134C04.
s32 fn_8039003C();

#endif
