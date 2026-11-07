#ifndef GAME_CLS_804B146C_H
#define GAME_CLS_804B146C_H

#include "types.h"
#include "JSystem/JUtility/TColor.h"

class J2DScreen;
class J2DOrthoGraph;
class PaneCtrl_804AF86C;
class cls_804B19E0;

// Named after its vtable, 0x804B146C (stored by its destructor,
// fn_80426AD4 in game/tu_8042165C.cpp; the constructor is fn_804257E4).
// The object cls_804B1920 keeps at 0x294. Only the members that
// cls_804B1920 calls are declared.
class cls_804B146C {
public:
    cls_804B146C();
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

    void fn_80424148(s32 player, bool force);
    void fn_80423DF0(s32 player);
    void fn_80423804(s32 player);
    void fn_80423248(s32 player);
    void fn_80422824();
    void fn_8042215C(s32 player);
    void fn_80421D78(s32 player);
    void fn_80421A34(s32 player, bool force);
    void fn_8042165C();

    /* 0x000 */ // vtable
    /* 0x004 */ J2DScreen* field_0x4;
    /* 0x008 */ J2DScreen* field_0x8[4];
    /* 0x018 */ J2DScreen* mScreen;
    /* 0x01C */ u8 field_0x1c[0x2C - 0x1C];
    /* 0x02C */ J2DScreen* field_0x2c[4];
    /* 0x03C */ u8 field_0x3c[0x4C - 0x3C];
    /* 0x04C */ J2DOrthoGraph* field_0x4c;
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
    /* 0x0F0 */ cls_804B19E0* field_0xf0;
    /* 0x0F4 */ f32 field_0xf4;
    /* 0x0F8 */ f32 field_0xf8[4];
    /* 0x108 */ f32 field_0x108[4];
    /* 0x118 */ f32 field_0x118[4];
    /* 0x128 */ f32 field_0x128[4];
    /* 0x138 */ f32 field_0x138[4];
    /* 0x148 */ s32 field_0x148[4];
    /* 0x158 */ s32 field_0x158[4];
    /* 0x168 */ s32 field_0x168[4];
    /* 0x178 */ s32 field_0x178[4];
    /* 0x188 */ s32 field_0x188[4];
    /* 0x198 */ s32 field_0x198[4];
    /* 0x1A8 */ s32 field_0x1a8[4];
    /* 0x1B8 */ s32 field_0x1b8[4];
    /* 0x1C8 */ s32 field_0x1c8[4];
    /* 0x1D8 */ s32 field_0x1d8[4];
    /* 0x1E8 */ s16 field_0x1e8[4];
    /* 0x1F0 */ u16 field_0x1f0[4];
    /* 0x1F8 */ s16 field_0x1f8[4];
    /* 0x200 */ s16 field_0x200[4];
    /* 0x208 */ s16 field_0x208[4];
    /* 0x210 */ JUtility::TColor field_0x210;
    /* 0x214 */ JUtility::TColor field_0x214;
    /* 0x218 */ u8 field_0x218;
    /* 0x219 */ u8 field_0x219[3];
    /* 0x21C */ u8 field_0x21c;
    /* 0x21D */ u8 field_0x21d;
    /* 0x21E */ u8 field_0x21e;
    /* 0x21F */ u8 field_0x21f;
    /* 0x220 */ u8 field_0x220[3];
    /* 0x223 */ u8 field_0x223[3];
    /* 0x226 */ u8 field_0x226;
    /* 0x227 */ u8 field_0x227;
    /* 0x228 */ u8 field_0x228;
    /* 0x229 */ u8 field_0x229;
    /* 0x22A */ u8 field_0x22a;
    /* 0x22B */ u8 field_0x22b;
    /* 0x22C */ u8 field_0x22c[6];
    /* 0x232 */ u8 field_0x232;
    /* 0x233 */ u8 field_0x233;
    /* 0x234 */ u8 field_0x234;
    /* 0x235 */ u8 field_0x235;
    /* 0x236 */ u8 field_0x236;
    /* 0x237 */ u8 field_0x237;
    /* 0x238 */ u8 field_0x238;
    /* 0x239 */ u8 field_0x239;
    /* 0x23A */ u8 field_0x23a;
    /* 0x23B */ u8 field_0x23b;
    /* 0x23C */ u8 field_0x23c[4];
    /* 0x240 */ u8 field_0x240[7];
    /* 0x247 */ u8 field_0x247[6];
    /* 0x24D */ u8 field_0x24d[4];
    /* 0x251 */ u8 field_0x251[4];
    /* 0x255 */ u8 field_0x255;
    /* 0x256 */ u8 field_0x256; // player bit masks
    /* 0x257 */ u8 field_0x257;
    /* 0x258 */ u8 field_0x258;
    /* 0x259 */ u8 field_0x259;
    /* 0x25A */ u8 field_0x25a;
    /* 0x25B */ u8 field_0x25b;
    /* 0x25C */ u8 field_0x25c;
    /* 0x25D */ u8 field_0x25d;
    /* 0x25E */ u8 field_0x25e[4];
};

#endif
