#ifndef GAME_CLS_804B146C_H
#define GAME_CLS_804B146C_H

#include "types.h"

class J2DScreen;
class PaneCtrl_804AF86C;

// Named after its vtable, 0x804B146C (stored by its destructor,
// fn_80426AD4 in game/tu_8042165C.cpp; the constructor is fn_804257E4).
// The object cls_804B1920 keeps at 0x294. Only the members that
// cls_804B1920 calls are declared.
class cls_804B146C {
public:
    virtual ~cls_804B146C(); // fn_80426AD4
    // Owner: game/tu_8042165C.cpp (0x80425128). Called with only the object.
    void fn_80425128();
    // Owner: game/tu_8042165C.cpp (0x804253E0): steps player `player`'s displayed counters toward the real ones.
    void fn_804253E0(s32 player);
    // Owner: game/tu_8042165C.cpp (0x80425234). Called with the three
    // arguments cls_804B1920::fn_8043D874 received.
    void fn_80425234(s32 id, s32 arg1, u32 arg2);
    // Owner: game/tu_8042165C.cpp (0x804263A8). Called with only the object by
    // cls_804B1920::fn_8043EDF8.
    void fn_804263A8();
    // Owner: game/tu_8042165C.cpp (0x804257E4). Called with only the object by
    // cls_804B1920::fn_8043D8B4 (it stores no vtable).
    void fn_804257E4();
    // Owner: game/tu_8042165C.cpp (0x80425F6C). Called with only the object by
    // cls_804B1920::fn_8043DC98 (its draw).
    void fn_80425F6C();
    // Owner: game/tu_8042165C.cpp (0x80425F24). Called with only the object by cls_804B1920::fn_8043D39C.
    void fn_80425F24();

    /* 0x000 */ // vtable
    /* 0x004 */ PaneCtrl_804AF86C* field_0x4;
    /* 0x008 */ PaneCtrl_804AF86C* field_0x8[4];
    /* 0x018 */ J2DScreen* mScreen;
    /* 0x01C */ u8 field_0x1c[0x2C - 0x1C];
    /* 0x02C */ PaneCtrl_804AF86C* field_0x2c[4];
    /* 0x03C */ u8 field_0x3c[0x4C - 0x3C];
    /* 0x04C */ PaneCtrl_804AF86C* field_0x4c;
    /* 0x050 */ PaneCtrl_804AF86C* field_0x50[4];
    /* 0x060 */ PaneCtrl_804AF86C* field_0x60[4];
    /* 0x070 */ PaneCtrl_804AF86C* field_0x70[4];
    /* 0x080 */ PaneCtrl_804AF86C* field_0x80[4];
    /* 0x090 */ PaneCtrl_804AF86C* field_0x90[4];
    /* 0x0A0 */ PaneCtrl_804AF86C* field_0xa0[4];
    /* 0x0B0 */ PaneCtrl_804AF86C* field_0xb0[4];
    /* 0x0C0 */ PaneCtrl_804AF86C* field_0xc0[4];
    /* 0x0D0 */ PaneCtrl_804AF86C* field_0xd0[4];
    /* 0x0E0 */ PaneCtrl_804AF86C* field_0xe0[4];
    /* 0x0F0 */ PaneCtrl_804AF86C* field_0xf0;
    /* 0x0F4 */ u8 field_0xf4[0x168 - 0xF4];
    /* 0x168 */ s32 field_0x168[4];
    /* 0x178 */ s32 field_0x178[4];
    /* 0x188 */ u8 field_0x188[0x1A8 - 0x188];
    /* 0x1A8 */ s32 field_0x1a8[4];
    /* 0x1B8 */ s32 field_0x1b8[4];
    /* 0x1C8 */ s32 field_0x1c8[4];
    /* 0x1D8 */ u8 field_0x1d8[0x1E8 - 0x1D8];
    /* 0x1E8 */ s16 field_0x1e8[4];
    /* 0x1F0 */ u8 field_0x1f0[0x1F8 - 0x1F0];
    /* 0x1F8 */ s16 field_0x1f8[4];
    /* 0x200 */ s16 field_0x200[4];
    /* 0x208 */ u8 field_0x208[0x256 - 0x208];
    /* 0x256 */ u8 field_0x256; // player bit masks
    /* 0x257 */ u8 field_0x257;
};

#endif
