#ifndef GAME_ACT_ACT_SHADOW_LINK_DEMO_H
#define GAME_ACT_ACT_SHADOW_LINK_DEMO_H

// Code: game/act_shadow_link_demo.cpp. Named after its vtable, 0x8049DB90
// (24 slots; the destructor, init and draw are not written yet). Only the
// field vfunc_0x58 returns is known.

#include "game/ActorBase.h"

class cls_8049DB90 : public ActorBase_804A5770 {
public:
    /* 0x58 */ virtual s32 vfunc_0x58();

    /* 0x230 */ u8 field_0x230[0x238 - 0x230]; // not yet known
    /* 0x238 */ s32 field_0x238; // returned by vfunc_0x58
}; // Size: at least 0x23C

#endif
