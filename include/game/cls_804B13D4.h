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
#include "JSystem/JUtility/TColor.h"

class PaneCtrl_804AF86C;
class J2DPane;
class J2DScreen;
class J2DOrthoGraph;
class cls_804B19E0;

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
    // 0x80418D08: steps the alpha scales field_0x128 / field_0x168 of the player and, when one
    // changed or `force` is set, applies them to the player's panes. Called by fn_8041EDB0, fn_80420574.
    void fn_80418D08(s32 player, bool force);
    // 0x80419570: shows the player's hearts (eight of four quarters) for field_0x1d8 / field_0x1f8.
    // Called by fn_8041EDB0.
    void fn_80419570(s32 player);
    // 0x8041E8B4: counts the shown life (lbl_80529DEC.field_0xd8) and Force Gems (field_0xe8)
    // of the player towards the real values. Called by fn_8041EDB0.
    void fn_8041E8B4(s32 player);
    // 0x80419C38 (not written yet): called per present player with 1 by fn_8041C67C.
    void fn_80419C38(s32 player, bool arg);
    // 0x8041C67C: sets up the HUD panes of the present players and the rank pictures.
    // Called by fn_80420574.
    void fn_8041C67C();
    // 0x8041CC04: shows player 0's Force Gem count (lbl_80529DEC.field_0xe8[0], -5000..9999)
    // in the digit pictures 'Ph1ps01'-'Ph1ps05' / 'Ph1psk01'-'Ph1psk05' when bit 0 of field_0x28b is set.
    // Called by fn_8041EDB0.
    void fn_8041CC04();
    // 0x8041C0C0: fn_8041CC04 for any player (its bit in field_0x28b, its tags). Called by fn_8041EDB0.
    void fn_8041C0C0(s32 player);
    // 0x8041DD24: fn_80419570 for player 0 on the main screen field_0x4, when bit 0 of field_0x28a
    // is set. Called by fn_8041EDB0.
    void fn_8041DD24();
    // 0x8041A868 (not written yet): called by fn_8041FC2C with 1.
    void fn_8041A868(bool arg);

    /* 0x00 */ // vtable
    /* 0x04 */ J2DScreen* field_0x4; // drawn by fn_8041F720
    /* 0x08 */ J2DScreen* field_0x8[4]; // per-player screens: search(u64) at vtable 0x3C, drawn by fn_8041F720
    /* 0x18 */ J2DScreen* field_0x18; // the HUD screen
    /* 0x1C */ u8 field_0x1c[0x2C - 0x1C];
    /* 0x2C */ J2DScreen* field_0x2c[4]; // search(u64) at vtable 0x3C (fn_8041918C), drawn by fn_8041F720
    /* 0x3C */ u8 field_0x3c[0x4C - 0x3C];
    /* 0x4C */ J2DOrthoGraph* field_0x4c; // setOrtho + setPort in fn_8041F720
    /* 0x50 */ cls_804B19E0* field_0x50; // the window picker (game/act/tu_80441318.h)
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
    /* 0xF4 */ u8 field_0xf4[0xF8 - 0xF4];
    // fn_8041FC2C clears the eight float arrays 0xF8-0x174.
    /* 0xF8 */ f32 field_0xf8[4];
    /* 0x108 */ f32 field_0x108[4];
    /* 0x118 */ f32 field_0x118[4];
    /* 0x128 */ f32 field_0x128[4]; // fn_80418D08: alpha scale of field_0x8[player], 0..1 in 0.1 steps
    /* 0x138 */ f32 field_0x138[4];
    /* 0x148 */ f32 field_0x148[4];
    /* 0x158 */ f32 field_0x158[4];
    /* 0x168 */ f32 field_0x168[4]; // fn_80418D08: alpha scale of field_0x2c[player]
    /* 0x178 */ s32 field_0x178[4]; // fn_8041FC2C sets -2
    /* 0x188 */ s32 field_0x188[4]; // fn_8041FC2C sets 0x180
    /* 0x198 */ s32 field_0x198[4]; // fn_8041E58C clears them
    /* 0x1A8 */ s32 field_0x1a8[4];
    /* 0x1B8 */ s32 field_0x1b8[4];
    /* 0x1C8 */ s32 field_0x1c8[4];
    /* 0x1D8 */ s32 field_0x1d8[4]; // fn_80419570: the life shown (fn_80237774), in quarter hearts
    /* 0x1E8 */ s32 field_0x1e8[4]; // fn_8041E8B4: the previous field_0x1d8
    /* 0x1F8 */ s32 field_0x1f8[4]; // fn_80419570: the maximum shown (fn_80237810)
    /* 0x208 */ s32 field_0x208[4];
    /* 0x218 */ u8 field_0x218; // fn_80418B64: player 0 is in the lower part of the screen
    /* 0x219 */ u8 field_0x219;
    /* 0x21A */ s16 field_0x21a[4]; // fn_80419570: countdown; at 0 it re-reads the life
    /* 0x222 */ s16 field_0x222; // fn_8041FC2C sets -1
    /* 0x224 */ s16 field_0x224[4]; // fn_8041E708: the change per 100
    /* 0x22C */ s16 field_0x22c[4];
    /* 0x234 */ s16 field_0x234[4]; // countdown per player, stepped down by fn_8041918C
    /* 0x23C */ s16 field_0x23c[4];
    // fn_8041CC04: the black / white of the Force Gem digits while the count is not negative.
    /* 0x244 */ JUtility::TColor field_0x244;
    /* 0x248 */ JUtility::TColor field_0x248;
    /* 0x24C */ u8 field_0x24c[0x250 - 0x24C];
    /* 0x250 */ u8 field_0x250[4]; // fn_80418D08: base alphas of the four pane groups of field_0x8
    /* 0x254 */ u8 field_0x254[0x27B - 0x254];
    /* 0x27B */ u8 field_0x27b[6]; // fn_80418D08: base alphas of field_0x2c's panes lbl_8048DDD0
    /* 0x281 */ u8 field_0x281[4]; // fn_8041FC2C sets 0xFF
    /* 0x285 */ u8 field_0x285[4]; // fn_8041FC2C sets the player index
    /* 0x289 */ u8 field_0x289; // player bit masks fn_8041E58C sets
    /* 0x28A */ u8 field_0x28a;
    /* 0x28B */ u8 field_0x28b;
    /* 0x28C */ u8 field_0x28c;
    /* 0x28D */ u8 field_0x28d;
    /* 0x28E */ u8 field_0x28e;
    /* 0x28F */ u8 field_0x28f; // player bit masks fn_8041E3B4 maintains
    /* 0x290 */ u8 field_0x290;
    /* 0x291 */ u8 field_0x291; // fn_8041E8B4: refresh the hearts while the life counts
    /* 0x292 */ u8 field_0x292;
    /* 0x293 */ u8 field_0x293;
    /* 0x294 */ u8 field_0x294[4];  // level per player, set by fn_8041918C
}; // Size: 0x298 or more (fn_8041E3B4 reads the byte at 0x28F)

#endif
