#ifndef GAME_TU_803F0A54_H
#define GAME_TU_803F0A54_H

#include "types.h"

class J2DScreen;
class J2DAnmBase;
class J2DPicture;
class PaneCtrl_804AF86C;
class cls_804B16A8;
class cls_804B20F8;

// The multiplayer (GameCube + GBA) connect screen: vtable 0x804B0F10,
// constructor fn_803FCBC4 (game/tu_803F0A54.cpp); 0x354 bytes
// (__nw__(0x354) in cls_804AF878's constructor fn_803A8884, which builds it
// as its field_0x8 while gGameRoot_8053AAF8->field_0x7f is clear, i.e. when the
// game is not in single-player mode). It loads seven J2DScreens from the
// archive fn_804031C4 reads (field_0x28 of lbl_80529DEC), counts the
// connected controllers (fn_8030A308 on each player's input slot) into
// gGameRoot_8053AAF8->mPlayerMask and runs a 13-state machine through three
// pointer-to-member tables (enter, update, draw); state 13 runs the
// previous state's entries. The fade picture field_0x5c covers the screen
// while field_0x25e is set.
class ConnectScreen_804B0F10 {
public:
    typedef void (ConnectScreen_804B0F10::*StateFunc)();

    ConnectScreen_804B0F10();
    /* 0x08 */ virtual ~ConnectScreen_804B0F10();

    // Draws the four player sprites ("player_sob.bin") over field_0xc's
    // player panes, sliding them between the 2/3-player and 4-player
    // layouts as field_0x210[0] counts 100-112 and 212-224.
    void fn_803F0A54();

    // Enter functions, lbl_804B0BDC[state] (set field_0x1c8 to the current
    // state and field_0x1c4 to the new one).
    void fn_803F1C18(); // 12
    void fn_803F1C5C(); // 11
    void fn_803F1C70(); // 10
    void fn_803F2580(); // 9
    void fn_803F2660(); // 8
    void fn_803F267C(); // 7 (also called by fn_803FC5E4)
    void fn_803F2AEC(); // 6
    void fn_803F2B48(); // 5
    void fn_803F2B5C(); // 4
    void fn_803F2B78(); // 3
    void fn_803F32E8(); // 2
    void fn_803F3310(); // 1
    void fn_803F3324(); // 0
    // Draw functions, lbl_804B0E4C[state].
    void fn_803F358C(); // 12
    void fn_803F35B0(); // 10, 11
    void fn_803F38A4(); // 7-9
    void fn_803F4120(); // 3-6
    void fn_803F5410(); // 0-2
    // Update functions, lbl_804B0D14[state].
    void fn_803F59A0(); // 12
    void fn_803F5A0C(); // 11
    void fn_803F715C(); // 10
    void fn_803F726C(); // 9
    void fn_803F8050(); // 8
    void fn_803F8354(); // 7
    void fn_803F8FBC(); // 6
    void fn_803F8AE4(); // 5
    void fn_803F943C(); // 4
    void fn_803F9EA8(); // 3
    void fn_803FAA88(); // 2
    void fn_803FB6F4(); // 1
    void fn_803FB9E4(); // 0

    // cls_804AF878::fn_803A8600 each frame: counts the connected
    // controllers, then runs the state's update.
    void fn_803FC304();
    // cls_804AF878::fn_803A8664: draws field_0x14, the state, then the fade.
    void fn_803FC46C();
    // cls_804AF878::fn_803A86A4: `flag` set goes straight to state 7.
    void fn_803FC5E4(u8 flag);

    /* 0x000 */ // vtable
    /* 0x004 */ J2DScreen* mScreen;
    /* 0x008 */ J2DScreen* field_0x8;
    /* 0x00C */ J2DScreen* field_0xc;
    /* 0x010 */ J2DScreen* field_0x10;
    /* 0x014 */ J2DScreen* field_0x14;
    /* 0x018 */ J2DScreen* field_0x18[4];
    /* 0x028 */ J2DScreen* field_0x28[4];
    /* 0x038 */ J2DAnmBase* field_0x38[4];
    /* 0x048 */ J2DAnmBase* field_0x48[4];
    /* 0x058 */ cls_804B16A8* field_0x58;
    /* 0x05C */ J2DPicture* field_0x5c; // full-screen fade
    /* 0x060 */ PaneCtrl_804AF86C* field_0x60;
    /* 0x064 */ PaneCtrl_804AF86C* field_0x64[4];
    /* 0x074 */ PaneCtrl_804AF86C* field_0x74; // lbl_8053AED8->field_0x8
    /* 0x078 */ cls_804B20F8* field_0x78[4];
    /* 0x088 */ f32 field_0x88;
    /* 0x08C */ f32 field_0x8c;
    /* 0x090 */ f32 field_0x90;
    /* 0x094 */ u8 field_0x94[0xF0 - 0x94];
    /* 0x0F0 */ f32 field_0xf0;    // mScreen 'Pscuro02' y
    /* 0x0F4 */ u8 field_0xf4[0xF8 - 0xF4];
    /* 0x0F8 */ f32 field_0xf8;    // field_0xc 'Pscuro02' y
    /* 0x0FC */ f32 field_0xfc[3]; // field_0xc 'Pstab04' / 'Phanb04' / 'Prub04' y
    /* 0x108 */ f32 field_0x108; // 'Pmbabb07' x
    /* 0x10C */ f32 field_0x10c; // 'Pmbabb07' y
    /* 0x110 */ f32 field_0x110; // 'Pmbabb07' y offset
    /* 0x114 */ f32 field_0x114; // field_0x74 y offset
    /* 0x118 */ f32 field_0x118[3]; // 'Pnopb08' / 12 / 16 x (2-4 players)
    /* 0x124 */ f32 field_0x124[3]; // the same panes' y
    /* 0x130 */ f32 field_0x130[3]; // the same panes' x offsets
    /* 0x13C */ f32 field_0x13c; // 'Ndata02' x
    /* 0x140 */ f32 field_0x140; // 'Ndata02' y
    /* 0x144 */ f32 field_0x144; // 'Ndata02' x offset
    /* 0x148 */ f32 field_0x148; // 'Ndata05' x
    /* 0x14C */ f32 field_0x14c; // 'Ndata05' y
    /* 0x150 */ f32 field_0x150; // 'Ndata07' x
    /* 0x154 */ f32 field_0x154; // 'Ndata07' y
    /* 0x158 */ f32 field_0x158; // 'Ndata05' / 'Ndata07' x offset
    /* 0x15C */ u8 field_0x15c[0x1A0 - 0x15C];
    /* 0x1A0 */ f32 field_0x1a0; // field_0x10 'Paccur02' y
    /* 0x1A4 */ s32 field_0x1a4;
    /* 0x1A8 */ s32 field_0x1a8;
    /* 0x1AC */ s32 field_0x1ac; // connected controllers beyond the first two
    /* 0x1B0 */ s32 field_0x1b0[4];
    /* 0x1C0 */ s32 field_0x1c0; // connected controllers this frame
    /* 0x1C4 */ s32 field_0x1c4; // state (13: run field_0x1c8's entries)
    /* 0x1C8 */ s32 field_0x1c8; // previous state
    /* 0x1CC */ s32 field_0x1cc; // state entered when the fade-out ends
    /* 0x1D0 */ s32 field_0x1d0[4];
    /* 0x1E0 */ s32 field_0x1e0[4]; // fn_80403414(player)
    /* 0x1F0 */ s32 field_0x1f0[4]; // fn_804033D4(player)
    /* 0x200 */ s32 field_0x200[4]; // fn_8040336C(player)
    /* 0x210 */ s32 field_0x210[4];
    /* 0x220 */ s32 field_0x220[4];
    /* 0x230 */ s32 field_0x230[4];
    /* 0x240 */ s16 field_0x240[4];
    /* 0x248 */ s16 field_0x248;
    /* 0x24A */ s16 field_0x24a;
    /* 0x24C */ s16 field_0x24c;
    /* 0x24E */ s16 field_0x24e;
    /* 0x250 */ s16 field_0x250;
    /* 0x252 */ u8 field_0x252;
    /* 0x253 */ u8 field_0x253;
    /* 0x254 */ u8 field_0x254;
    /* 0x255 */ u8 field_0x255;
    /* 0x256 */ u8 field_0x256[0x258 - 0x256];
    /* 0x258 */ u8 field_0x258;
    /* 0x259 */ u8 field_0x259;
    /* 0x25A */ u8 field_0x25a;
    /* 0x25B */ u8 field_0x25b;
    /* 0x25C */ u8 field_0x25c;
    /* 0x25D */ u8 field_0x25d; // fade alpha
    /* 0x25E */ u8 field_0x25e; // fade: 0 off, 1 out (then enter field_0x1cc), 2 in
    /* 0x25F */ u8 field_0x25f[20];    // initial alpha of 20 mScreen panes
    /* 0x273 */ u8 field_0x273[3][6];  // initial alpha of 3 x 6 mScreen panes
    /* 0x285 */ u8 field_0x285[24];    // initial alpha of 24 mScreen panes
    /* 0x29D */ u8 field_0x29d[18];    // initial alpha of 18 mScreen panes
    /* 0x2AF */ u8 field_0x2af[0x335 - 0x2AF];
    /* 0x335 */ u8 field_0x335; // initial alpha of field_0xc's 'Pcon01' (fn_803F0A54 divides by it)
    /* 0x336 */ u8 field_0x336[0x350 - 0x336];
    /* 0x350 */ s32 field_0x350; // fn_803A8600 copies it to cls_804AF878::field_0xc
}; // Size: 0x354

#endif
