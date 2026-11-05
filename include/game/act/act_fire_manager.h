#ifndef GAME_ACT_ACT_FIRE_MANAGER_H
#define GAME_ACT_ACT_FIRE_MANAGER_H

// Code: game/act_fire_manager.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804AB2B0.
class FireManager_804AB2B0 : public ActorBase_804A5770 {
public:
    FireManager_804AB2B0() {}
    /* 0x08 */ virtual ~FireManager_804AB2B0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x230 */ JGeometry::TVec3f mBasePos; // copy of mPos made by init()
    /* 0x23C */ f32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ u8 field_0x244;
    /* 0x245 */ u8 field_0x245;
    /* 0x248 */ u32 field_0x248;
    /* 0x24C */ u32 field_0x24c;
    /* 0x250 */ u32 field_0x250;
    /* 0x254 */ u8 field_0x254;
    /* 0x255 */ u8 field_0x255;
}; // Size: 0x258

#endif
