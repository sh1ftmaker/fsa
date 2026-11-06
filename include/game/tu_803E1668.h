#ifndef GAME_TU_803E1668_H
#define GAME_TU_803E1668_H

#include "types.h"

class J2DScreen;
class J2DAnmTransform;
class J2DPicture;
class PaneCtrl_804AF86C;
class cls_804B16A8;
class cls_804B20F8;

// Vtable 0x804B0B00, constructor fn_803EE254, destructor fn_803EDF94
// (game/tu_803E1668.cpp); 0x364 bytes (__nw__(0x364) in fn_803A8884). The
// sister screen of ConnectScreen_804B0F10 (game/tu_803F0A54.h): seven J2DScreens and a
// 15-state machine run through three pointer-to-member tables (enter,
// update, draw).
class cls_804B0B00 {
public:
    typedef void (cls_804B0B00::*StateFunc)();

    cls_804B0B00();
    /* 0x08 */ virtual ~cls_804B0B00();
    // Draws the four player sprites over field_0xc's player panes.
    void fn_803E1668();
    void fn_803ED83C();
    void fn_803EDA2C();
    // `mode` is tested after clrlwi 24 (switch on 0, 1, 2).
    void fn_803EDBF0(u8 mode);

    // The state setters (game/tu_803E1668.cpp): each saves field_0x1d4 in
    // field_0x1d8 and stores the new state; the 15 of them in the table at
    // 0x804B06A0 are indexed by the state.
    void fn_803E29C8();
    void fn_803E2A0C();
    void fn_803E3548();
    void fn_803E3C00();
    void fn_803E3C80();
    void fn_803E3C94();
    void fn_803E4308();
    void fn_803E4330();
    void fn_803E45FC();
    void fn_803E4610();
    void fn_803E462C();
    void fn_803E6A28();
    void fn_803ED5B4();
    void fn_803E3320();
    void drawFade();
    void enterState15();

    // Enter functions, lbl_804B0754[state].
    void fn_803E2A20(); // 12
    void fn_803E3564(); // 9
    void fn_803E3CB0(); // 5
    void fn_803E4344(); // 2
    // Draw functions, lbl_804B0A24[state].
    void fn_803E4650(); // 12, 13
    void fn_803E4940(); // 0, 9-11
    void fn_803E51C4(); // 5-8
    void fn_803E64A0(); // 2-4
    // Update functions, lbl_804B08BC[state].
    void fn_803E6A2C(); // 14
    void fn_803E6AB0(); // 13
    void fn_803E8210(); // 12
    void fn_803E831C(); // 11
    void fn_803E9034(); // 10
    void fn_803E926C(); // 9
    void fn_803E9A20(); // 8
    void fn_803E9EE8(); // 7
    void fn_803EA3F8(); // 6
    void fn_803EAEC8(); // 5
    void fn_803EBAA8(); // 4
    void fn_803EC804(); // 3
    void fn_803ECC64(); // 2
    void fn_803ED604(); // 0

    /* 0x000 */ // vtable
    /* 0x004 */ J2DScreen* mScreen;
    /* 0x008 */ J2DScreen* field_0x8;
    /* 0x00C */ J2DScreen* field_0xc;
    /* 0x010 */ J2DScreen* field_0x10;
    /* 0x014 */ J2DScreen* field_0x14;
    /* 0x018 */ J2DScreen* field_0x18[4];
    /* 0x028 */ J2DScreen* field_0x28[4];
    /* 0x038 */ J2DAnmTransform* field_0x38[4];
    /* 0x048 */ J2DAnmTransform* field_0x48[4];
    /* 0x058 */ u8 field_0x58[0x5C - 0x58];
    /* 0x05C */ cls_804B16A8* field_0x5c;
    /* 0x060 */ J2DPicture* field_0x60;
    /* 0x064 */ PaneCtrl_804AF86C* field_0x64;
    /* 0x068 */ PaneCtrl_804AF86C* field_0x68[4];
    /* 0x078 */ PaneCtrl_804AF86C* field_0x78;
    /* 0x07C */ cls_804B20F8* field_0x7c[4];
    // The effect position fn_803E9A20 / fn_803E9EE8 compute from a player
    // pane ('Pplli01'-'04') at its 36th frame.
    /* 0x08C */ f32 field_0x8c;
    /* 0x090 */ f32 field_0x90;
    /* 0x094 */ f32 field_0x94;
    // Pane positions read in the constructor (getBounds()->i), with the
    // offsets the state functions add to them (cleared there).
    /* 0x098 */ f32 field_0x98;  // field_0x8 'Ndata03' x
    /* 0x09C */ f32 field_0x9c;  // field_0x8 'Ndata03' y
    /* 0x0A0 */ f32 field_0xa0;
    /* 0x0A4 */ f32 field_0xa4;  // field_0x8 'Ndata05' x
    /* 0x0A8 */ f32 field_0xa8;  // field_0x8 'Ndata05' y
    /* 0x0AC */ f32 field_0xac;
    /* 0x0B0 */ f32 field_0xb0;  // field_0x8 'Ndata06' x
    /* 0x0B4 */ f32 field_0xb4;  // field_0x8 'Ndata06' y
    /* 0x0B8 */ f32 field_0xb8;
    /* 0x0BC */ f32 field_0xbc;
    /* 0x0C0 */ f32 field_0xc0[4]; // field_0x8 'Ppllbk02' / 12 / 22 / 32 x
    /* 0x0D0 */ f32 field_0xd0[4]; // the same panes' y
    /* 0x0E0 */ f32 field_0xe0[4];
    /* 0x0F0 */ f32 field_0xf0;  // mScreen 'Pmadbb07' x
    /* 0x0F4 */ f32 field_0xf4;  // mScreen 'Pmadbb07' y
    /* 0x0F8 */ f32 field_0xf8;
    /* 0x0FC */ f32 field_0xfc;
    /* 0x100 */ f32 field_0x100[4]; // mScreen 'Pnopb04' / 08 / 12 / 16 x
    /* 0x110 */ f32 field_0x110[4]; // the same panes' y
    /* 0x120 */ f32 field_0x120[4];
    /* 0x130 */ f32 field_0x130; // mScreen 'Ndata02' x
    /* 0x134 */ f32 field_0x134; // mScreen 'Ndata02' y
    /* 0x138 */ f32 field_0x138;
    /* 0x13C */ f32 field_0x13c; // mScreen 'Ndata05' x
    /* 0x140 */ f32 field_0x140; // mScreen 'Ndata05' y
    /* 0x144 */ f32 field_0x144; // mScreen 'Ndata07' x
    /* 0x148 */ f32 field_0x148; // mScreen 'Ndata07' y
    /* 0x14C */ f32 field_0x14c;
    /* 0x150 */ f32 field_0x150[3]; // field_0xc 'Pstab04' / 'Phanb04' / 'Prub04' y
    /* 0x15C */ f32 field_0x15c; // field_0xc 'Pmadbb07' x
    /* 0x160 */ f32 field_0x160; // field_0xc 'Pmadbb07' y
    /* 0x164 */ f32 field_0x164;
    /* 0x168 */ f32 field_0x168;
    /* 0x16C */ f32 field_0x16c[3]; // field_0xc 'Pstab04' / 'Phanb04' / 'Prub04' x
    /* 0x178 */ f32 field_0x178[3]; // the same panes' y
    /* 0x184 */ f32 field_0x184[3];
    /* 0x190 */ f32 field_0x190; // field_0xc 'Plibkk03' x
    /* 0x194 */ f32 field_0x194; // field_0xc 'Plibkk03' y
    /* 0x198 */ f32 field_0x198;
    /* 0x19C */ f32 field_0x19c; // mScreen 'Pscuro02' x
    /* 0x1A0 */ f32 field_0x1a0; // mScreen 'Pscuro02' y
    /* 0x1A4 */ f32 field_0x1a4; // field_0xc 'Pscuro02' x
    /* 0x1A8 */ f32 field_0x1a8; // field_0xc 'Pscuro02' y
    /* 0x1AC */ f32 field_0x1ac; // field_0x10 'Paccur02' x
    /* 0x1B0 */ f32 field_0x1b0; // field_0x10 'Paccur02' y
    /* 0x1B4 */ s32 field_0x1b4;
    /* 0x1B8 */ s32 field_0x1b8;
    /* 0x1BC */ s32 field_0x1bc; // lbl_80529DEC.field_0x23c
    /* 0x1C0 */ s32 field_0x1c0[4];
    /* 0x1D0 */ s32 field_0x1d0;
    /* 0x1D4 */ s32 field_0x1d4; // state
    /* 0x1D8 */ s32 field_0x1d8; // previous state
    /* 0x1DC */ s32 field_0x1dc;
    /* 0x1E0 */ s32 field_0x1e0[4];
    /* 0x1F0 */ s32 field_0x1f0[4]; // fn_80403414(player)
    /* 0x200 */ s32 field_0x200[4]; // fn_804033D4(player)
    /* 0x210 */ s32 field_0x210[4]; // fn_8040336C(player)
    /* 0x220 */ s32 field_0x220[4];
    /* 0x230 */ s32 field_0x230[4];
    /* 0x240 */ s32 field_0x240[4];
    /* 0x250 */ s16 field_0x250;
    /* 0x252 */ s16 field_0x252;
    /* 0x254 */ s16 field_0x254[4];
    /* 0x25C */ s16 field_0x25c;
    /* 0x25E */ s16 field_0x25e;
    /* 0x260 */ s16 field_0x260;
    /* 0x262 */ s16 field_0x262;
    /* 0x264 */ u8 field_0x264;
    /* 0x265 */ u8 field_0x265; // initial alpha of field_0x8 'Pcgbak01'
    /* 0x266 */ u8 field_0x266; // initial alpha of field_0x8 'Pcgba01'
    /* 0x267 */ u8 field_0x267; // initial alpha of field_0x8 'Pcgp01'
    /* 0x268 */ u8 field_0x268;
    /* 0x269 */ u8 field_0x269;
    /* 0x26A */ u8 field_0x26a;
    /* 0x26B */ u8 field_0x26b;
    /* 0x26C */ u8 field_0x26c;
    /* 0x26D */ u8 field_0x26d;
    /* 0x26E */ u8 field_0x26e;
    // Initial alphas of the panes of the tag tables (game/tu_803E1668.cpp).
    /* 0x26F */ u8 field_0x26f[17];   // mScreen
    /* 0x280 */ u8 field_0x280[4][6]; // mScreen
    /* 0x298 */ u8 field_0x298[29];   // mScreen
    /* 0x2B5 */ u8 field_0x2b5[22];   // mScreen
    /* 0x2CB */ u8 field_0x2cb[11];   // field_0x8
    /* 0x2D6 */ u8 field_0x2d6[6];    // field_0x8
    /* 0x2DC */ u8 field_0x2dc[34];   // field_0x8
    /* 0x2FE */ u8 field_0x2fe[4][9]; // field_0x8
    /* 0x322 */ u8 field_0x322[17];   // field_0xc
    /* 0x333 */ u8 field_0x333[3][5]; // field_0xc
    /* 0x342 */ u8 field_0x342[26];   // field_0xc
    /* 0x35C */ u8 field_0x35c[0x360 - 0x35C];
    /* 0x360 */ s32 field_0x360; // fn_803A8600 copies it to cls_804AF878::field_0xc
}; // Size: 0x364

#endif
