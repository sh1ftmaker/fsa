#ifndef GAME_ACT_TU_80343A70_H
#define GAME_ACT_TU_80343A70_H

// Code: game/tu_80343A70.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804AC600.
class cls_804AC600 : public ActorBase_804A5770 {
public:
    cls_804AC600() {}
    /* 0x08 */ virtual ~cls_804AC600();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x24 */ virtual void vfunc_0x24();
    /* 0x230 */ s32 field_0x230; // state
    /* 0x234 */ u8 field_0x234;
    /* 0x235 */ u8 field_0x235[3]; // not yet known
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ s32 field_0x254[7];
    /* 0x270 */ s32 field_0x270; // bit 0 of field_0x90
    /* 0x274 */ s32 field_0x274; // kind (1, 2 pick what vfunc_0x24 drops)
    /* 0x278 */ f32 field_0x278[7];
    /* 0x294 */ u16 field_0x294;
    /* 0x296 */ u8 field_0x296[0xC]; // not yet known
    /* 0x2A2 */ s16 field_0x2a2;
    /* 0x2A4 */ JGeometry::TVec3f field_0x2a4[7];
    /* 0x2F8 */ JGeometry::TVec3f field_0x2f8[7];
    /* 0x34C */ JGeometry::TVec3f field_0x34c[7];
}; // Size: 0x3A0

#endif
