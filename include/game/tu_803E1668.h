#ifndef GAME_TU_803E1668_H
#define GAME_TU_803E1668_H

#include "types.h"

class cls_804B16A8;

// Vtable 0x804B0B00, constructor fn_803EE254 (game/tu_803E1668.cpp, owner
// unit, not written yet); 0x364 bytes (__nw__(0x364) in fn_803A8884). Only
// what game/tu_803A85E4.cpp uses is declared.
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
    // The menu update functions (state tables' rows).
    void fn_803E6A2C();
    void fn_803E9034();
    void fn_803ED604();

    /* 0x004 */ u8 field_0x4[0x5C - 0x4];
    /* 0x05C */ cls_804B16A8* field_0x5c;
    /* 0x060 */ u8 field_0x60[0x1B8 - 0x60];
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
};

#endif
