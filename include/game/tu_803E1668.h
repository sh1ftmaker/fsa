#ifndef GAME_TU_803E1668_H
#define GAME_TU_803E1668_H

#include "types.h"

class J2DScreen;
class J2DAnmBase;
class J2DPicture;
class cls_804AF86C;
class cls_804B16A8;
class cls_804B20F8;

// Vtable 0x804B0B00, constructor fn_803EE254, destructor fn_803EDF94
// (game/tu_803E1668.cpp); 0x364 bytes (__nw__(0x364) in fn_803A8884). The
// sister screen of cls_804B0F10 (game/tu_803F0A54.h): seven J2DScreens and a
// 15-state machine run through three pointer-to-member tables (enter,
// update, draw).
class cls_804B0B00 {
public:
    typedef void (cls_804B0B00::*StateFunc)();

    cls_804B0B00();
    /* 0x08 */ virtual ~cls_804B0B00();
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
    // Not written yet: fn_803E3320 (called by the two menu update functions
    // when the current choice is below 3).
    void fn_803E3320();

    // Enter functions not written yet, lbl_804B0754[state].
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
    /* 0x038 */ J2DAnmBase* field_0x38[4];
    /* 0x048 */ J2DAnmBase* field_0x48[4];
    /* 0x058 */ u8 field_0x58[0x5C - 0x58];
    /* 0x05C */ cls_804B16A8* field_0x5c;
    /* 0x060 */ J2DPicture* field_0x60;
    /* 0x064 */ cls_804AF86C* field_0x64;
    /* 0x068 */ cls_804AF86C* field_0x68[4];
    /* 0x078 */ cls_804AF86C* field_0x78;
    /* 0x07C */ cls_804B20F8* field_0x7c[4];
    /* 0x08C */ u8 field_0x8c[0x1B8 - 0x8C];
    /* 0x1B8 */ s32 field_0x1b8;
    /* 0x1BC */ s32 field_0x1bc;
    /* 0x1C0 */ u8 field_0x1c0[0x1D0 - 0x1C0];
    /* 0x1D0 */ s32 field_0x1d0;
    /* 0x1D4 */ s32 field_0x1d4; // state
    /* 0x1D8 */ s32 field_0x1d8; // previous state
    /* 0x1DC */ s32 field_0x1dc;
    /* 0x1E0 */ s32 field_0x1e0;
    /* 0x1E4 */ u8 field_0x1e4[0x220 - 0x1E4];
    /* 0x220 */ s32 field_0x220;
    /* 0x224 */ u8 field_0x224[0x254 - 0x224];
    /* 0x254 */ s16 field_0x254;
    /* 0x256 */ u8 field_0x256[0x26C - 0x256];
    /* 0x26C */ u8 field_0x26c;
    /* 0x26D */ u8 field_0x26d;
    /* 0x26E */ u8 field_0x26e;
    /* 0x26F */ u8 field_0x26f[0x360 - 0x26F];
    /* 0x360 */ s32 field_0x360; // fn_803A8600 copies it to cls_804AF878::field_0xc
}; // Size: 0x364

#endif
