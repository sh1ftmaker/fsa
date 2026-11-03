#ifndef GAME_CLS_804B13D4_H
#define GAME_CLS_804B13D4_H

// Vtable 0x804B13D4 (0, 0, the destructor), defined in game/tu_80418B64.cpp.
// Its constructor is not identified yet; fn_8041E3B4 (tu_80418B64) is a
// member taking a long and reading the byte at 0x28F, and fn_8041FC2C uses
// the object's tables. The element and member types are not identified: they
// are polymorphic objects with only a destructor, typed as cls_804AF86C
// (game/GameState.h, vtable 0x804AF86C, same shape) until their own classes
// are known.

#include "types.h"

class cls_804AF86C;

class cls_804B13D4 {
public:
    /* 0x08 */ virtual ~cls_804B13D4();

    /* 0x00 */ // vtable
    /* 0x04 */ cls_804AF86C* field_0x4;
    /* 0x08 */ cls_804AF86C* field_0x8[4];
    /* 0x18 */ cls_804AF86C* field_0x18;
    /* 0x1C */ u8 field_0x1c[0x2C - 0x1C];
    /* 0x2C */ cls_804AF86C* field_0x2c[4];
    /* 0x3C */ u8 field_0x3c[0x4C - 0x3C];
    /* 0x4C */ cls_804AF86C* field_0x4c;
    /* 0x50 */ cls_804AF86C* field_0x50;
    /* 0x54 */ cls_804AF86C* field_0x54;
    /* 0x58 */ cls_804AF86C* field_0x58;
    /* 0x5C */ cls_804AF86C* field_0x5c;
    /* 0x60 */ u8 field_0x60[0x6C - 0x60];
    /* 0x6C */ cls_804AF86C* field_0x6c;
    /* 0x70 */ cls_804AF86C* field_0x70;
    /* 0x74 */ cls_804AF86C* field_0x74[4];
    /* 0x84 */ cls_804AF86C* field_0x84[4];
    /* 0x94 */ cls_804AF86C* field_0x94[4];
    /* 0xA4 */ cls_804AF86C* field_0xa4[4];
    /* 0xB4 */ cls_804AF86C* field_0xb4[4];
    /* 0xC4 */ cls_804AF86C* field_0xc4[4];
    /* 0xD4 */ cls_804AF86C* field_0xd4[4];
    /* 0xE4 */ cls_804AF86C* field_0xe4[4];
    /* 0xF4 */ u8 field_0xf4[0x290 - 0xF4];
}; // Size: 0x290 or more (fn_8041E3B4 reads the byte at 0x28F)

#endif
