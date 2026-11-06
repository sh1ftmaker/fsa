#ifndef GAME_ACT_TU_803536C4_H
#define GAME_ACT_TU_803536C4_H

// Code: game/tu_803536C4.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804ACC68.
class cls_804ACC68 : public ActorBase_804A5770 {
public:
    cls_804ACC68() {}
    /* 0x08 */ virtual ~cls_804ACC68();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 field_0x230; // init zeroes it; nothing reads it
    /* 0x234 */ s32 field_0x234; // 9 after init; counts down, advances field_0x23c
    /* 0x238 */ s32 field_0x238; // 300 after init; counts down
    /* 0x23C */ s32 field_0x23c; // 0..3, advanced by update
    /* 0x240 */ s32 field_0x240; // 60 after init; counts down, advances field_0x244
    /* 0x244 */ s32 field_0x244; // 0..3
    /* 0x248 */ s32 field_0x248; // field_0x90 & 3
    /* 0x24C */ s32 field_0x24c; // field_0x90 >> 4 & 0xF
    /* 0x250 */ bool field_0x250; // field_0x90 >> 8 & 3 is nonzero
    /* 0x254 */ JGeometry::TVec3f field_0x254;
    /* 0x260 */ JGeometry::TVec3f field_0x260;
}; // Size: 0x26C

#endif
