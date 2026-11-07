#ifndef GAME_CLS_804B13D4_H
#define GAME_CLS_804B13D4_H

// Vtable 0x804B13D4 (0, 0, the destructor), defined in game/tu_80418B64.cpp.
// Its constructor is not identified yet; fn_8041E3B4 (tu_80418B64) is a
// member taking a long and reading the byte at 0x28F, and fn_8041FC2C uses
// the object's tables. The element and member types are not identified: they
// are polymorphic objects with only a destructor, typed as PaneCtrl_804AF86C
// (game/GameState.h, vtable 0x804AF86C, same shape) until their own classes
// are known.

#include "types.h"

class PaneCtrl_804AF86C;
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
    // Owner: game/tu_80418B64.cpp (0x8041E3B4): updates the player's bit in the masks at 0x28F / 0x290 / 0x293.
    void fn_8041E3B4(s32 player);
    // Owner: game/tu_80418B64.cpp (0x80418B64): 0..4 from the bits of lbl_8053AED0's byte 0xDC and whether
    // player 0 is off the upper or lower third of the screen (field_0x218).
    s32 fn_80418B64();
    // Owner: game/tu_80418B64.cpp (0x8041FC2C). Called with only the object by
    // cls_804B1920::fn_8043EDF8.
    void fn_8041FC2C();
    // Owner: game/tu_80418B64.cpp (0x8041EDB0). Called with only the object by
    // cls_804B1920::fn_8043D8B4.
    void fn_8041EDB0();
    // Owner: game/tu_80418B64.cpp (0x8041F720). Called with only the object by
    // cls_804B1920::fn_8043DC98 (its draw).
    void fn_8041F720();
    // Owner: game/tu_80418B64.cpp (0x8041F5FC): draws the HUD screen (field_0x18) unless the
    // game mode excludes it.
    void fn_8041F5FC();

    /* 0x00 */ // vtable
    /* 0x04 */ PaneCtrl_804AF86C* field_0x4;
    /* 0x08 */ PaneCtrl_804AF86C* field_0x8[4];
    /* 0x18 */ PaneCtrl_804AF86C* field_0x18;
    /* 0x1C */ u8 field_0x1c[0x2C - 0x1C];
    /* 0x2C */ J2DPane* field_0x2c[4]; // search(u64) at vtable 0x3C (fn_8041918C)
    /* 0x3C */ u8 field_0x3c[0x4C - 0x3C];
    /* 0x4C */ PaneCtrl_804AF86C* field_0x4c;
    /* 0x50 */ PaneCtrl_804AF86C* field_0x50;
    /* 0x54 */ PaneCtrl_804AF86C* field_0x54;
    /* 0x58 */ PaneCtrl_804AF86C* field_0x58;
    /* 0x5C */ PaneCtrl_804AF86C* field_0x5c;
    /* 0x60 */ u8 field_0x60[0x6C - 0x60];
    /* 0x6C */ PaneCtrl_804AF86C* field_0x6c;
    /* 0x70 */ PaneCtrl_804AF86C* field_0x70;
    /* 0x74 */ PaneCtrl_804AF86C* field_0x74[4];
    /* 0x84 */ PaneCtrl_804AF86C* field_0x84[4];
    /* 0x94 */ PaneCtrl_804AF86C* field_0x94[4];
    /* 0xA4 */ PaneCtrl_804AF86C* field_0xa4[4];
    /* 0xB4 */ PaneCtrl_804AF86C* field_0xb4[4];
    /* 0xC4 */ PaneCtrl_804AF86C* field_0xc4[4];
    /* 0xD4 */ PaneCtrl_804AF86C* field_0xd4[4];
    /* 0xE4 */ PaneCtrl_804AF86C* field_0xe4[4];
    /* 0xF4 */ u8 field_0xf4[0x198 - 0xF4];
    /* 0x198 */ s32 field_0x198[4]; // fn_8041E58C clears them
    /* 0x1A8 */ s32 field_0x1a8[4];
    /* 0x1B8 */ s32 field_0x1b8[4];
    /* 0x1C8 */ u8 field_0x1c8[0x218 - 0x1C8];
    /* 0x218 */ u8 field_0x218; // fn_80418B64: player 0 is in the lower part of the screen
    /* 0x219 */ u8 field_0x219[0x224 - 0x219];
    /* 0x224 */ s16 field_0x224[4]; // fn_8041E708: the change per 100
    /* 0x22C */ s16 field_0x22c[4];
    /* 0x234 */ s16 field_0x234[4]; // countdown per player, stepped down by fn_8041918C
    /* 0x23C */ u8 field_0x23c[0x289 - 0x23C];
    /* 0x289 */ u8 field_0x289; // player bit masks fn_8041E58C sets
    /* 0x28A */ u8 field_0x28a;
    /* 0x28B */ u8 field_0x28b;
    /* 0x28C */ u8 field_0x28c[0x28F - 0x28C];
    /* 0x28F */ u8 field_0x28f; // player bit masks fn_8041E3B4 maintains
    /* 0x290 */ u8 field_0x290;
    /* 0x291 */ u8 field_0x291[0x293 - 0x291];
    /* 0x293 */ u8 field_0x293;
    /* 0x294 */ u8 field_0x294[4];  // level per player, set by fn_8041918C
}; // Size: 0x298 or more (fn_8041E3B4 reads the byte at 0x28F)

#endif
