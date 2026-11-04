#ifndef GAME_ACT_ACT_SHADOW_LINK_DEMO_H
#define GAME_ACT_ACT_SHADOW_LINK_DEMO_H

// Code: game/act_shadow_link_demo.cpp. Named after its vtable, 0x8049DB90
// (24 slots; init and draw are not written yet). Only the
// field vfunc_0x58 returns is known, and vfunc_0x5C tests it.

#include "game/act/tu_8010DF44.h"

class cls_8049DB90 : public cls_8049D140 {
public:
    /* 0x08 */ virtual ~cls_8049DB90();
    /* 0x58 */ virtual s32 vfunc_0x58();
    /* 0x5C */ virtual s32 vfunc_0x5C(); // 1 or 0, as cls_8049D140::vfunc_0x5C

    /* 0x230 */ u8 field_0x230[0x238 - 0x230]; // not yet known
    /* 0x238 */ s32 field_0x238; // returned by vfunc_0x58
}; // Size: at least 0x23C

#endif
