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
class J2DPane;

class cls_804B13D4 {
public:
    /* 0x08 */ virtual ~cls_804B13D4();

    // tu_80418B64 (0x8041918C). Takes the player index; sets the byte at
    // 0x294 + player to a level (fn_802366CC / 60 + 1, clamped to 0..99) and
    // shows the pictures of the player's pane tags for it.
    void fn_8041918C(s32 player);

    // Owner: game/tu_80418B64.cpp (0x8041E58C). Called with only the object by
    // cls_804B1920::fn_8043D834.
    void fn_8041E58C();
    // Owner: game/tu_80418B64.cpp (0x8041E708). Called with the three
    // arguments cls_804B1920::fn_8043D874 received.
    void fn_8041E708(s32 id, s32 arg1, u32 arg2);
    // Owner: game/tu_80418B64.cpp (0x8041FC2C). Called with only the object by
    // cls_804B1920::fn_8043EDF8.
    void fn_8041FC2C();

    /* 0x00 */ // vtable
    /* 0x04 */ cls_804AF86C* field_0x4;
    /* 0x08 */ cls_804AF86C* field_0x8[4];
    /* 0x18 */ cls_804AF86C* field_0x18;
    /* 0x1C */ u8 field_0x1c[0x2C - 0x1C];
    /* 0x2C */ J2DPane* field_0x2c[4]; // search(u64) at vtable 0x3C (fn_8041918C)
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
    /* 0xF4 */ u8 field_0xf4[0x234 - 0xF4];
    /* 0x234 */ s16 field_0x234[4]; // countdown per player, stepped down by fn_8041918C
    /* 0x23C */ u8 field_0x23c[0x294 - 0x23C];
    /* 0x294 */ u8 field_0x294[4];  // level per player, set by fn_8041918C
}; // Size: 0x298 or more (fn_8041E3B4 reads the byte at 0x28F)

#endif
