#ifndef GAME_ACT_TU_80333958_H
#define GAME_ACT_TU_80333958_H

// Code: game/tu_80333958.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804AB968.
class cls_804AB968 : public ActorBase_804A5770 {
public:
    cls_804AB968() {}
    /* 0x08 */ virtual ~cls_804AB968();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    /* 0x230 */ s32 field_0x230; // init zeroes it; nothing reads it
    /* 0x234 */ s32 field_0x234; // 30 after init; update counts it down by the speed
    /* 0x238 */ s32 field_0x238; // 0 or 1 (1 while field_0x234 <= 15); picks the sprite frame
    /* 0x23C */ s32 field_0x23c; // field_0x90 & 0xF is nonzero
    /* 0x240 */ s32 field_0x240; // direction 0..3, field_0x90 >> 4 & 0xF
    /* 0x244 */ s32 field_0x244; // frames alive
}; // Size: 0x248

#endif
