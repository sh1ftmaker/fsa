#ifndef GAME_ACT_ACT_FOUR_SWORD_DEMO_H
#define GAME_ACT_ACT_FOUR_SWORD_DEMO_H

// Code: game/act_four_sword_demo.cpp.

#include "game/act/tu_8010DF44.h"

// Named after its vtable, 0x8049DA0C, which derives from cls_8049D140. Its
// constructor (0x801180D0, not written yet) clears field_0x230, field_0x231
// and field_0x234.
class cls_8049DA0C : public cls_8049D140 {
public:
    /* 0x08 */ virtual ~cls_8049DA0C();

    // Slot 0x58, overriding cls_8049D140::vfunc_0x58 (game/tu_8010DF44.cpp).
    // Returns field_0x234.
    /* 0x58 */ virtual s32 vfunc_0x58();

    /* 0x230 */ u8 field_0x230;
    /* 0x231 */ u8 field_0x231;
    /* 0x234 */ s32 field_0x234;
}; // Size: at least 0x238

#endif
