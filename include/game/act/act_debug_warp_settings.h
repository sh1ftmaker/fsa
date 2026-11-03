#ifndef GAME_ACT_ACT_DEBUG_WARP_SETTINGS_H
#define GAME_ACT_ACT_DEBUG_WARP_SETTINGS_H

// Code: game/act_debug_warp_settings.cpp.

#include "game/ActorBase.h"

// Named after its vtable (0x8049C9E8).
class DebugWarpSettings_8049C9E8 : public ActorBase_804A5770 {
public:
    DebugWarpSettings_8049C9E8() {}

    /* 0x08 */ virtual ~DebugWarpSettings_8049C9E8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238; // bits 16-23 of field_0x90
    /* 0x23C */ s32 field_0x23c; // bits 8-15 of field_0x90
}; // Size: 0x240

#endif
