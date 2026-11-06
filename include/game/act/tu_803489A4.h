#ifndef GAME_ACT_TU_803489A4_H
#define GAME_ACT_TU_803489A4_H

// Code: game/tu_803489A4.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804AC7B8.
class BeeFormation_804AC7B8 : public ActorBase_804A5770 {
public:
    BeeFormation_804AC7B8() {}
    /* 0x08 */ virtual ~BeeFormation_804AC7B8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ s32 field_0x254;
    /* 0x258 */ f32 field_0x258;
    /* 0x25C */ f32 field_0x25c;
    /* 0x260 */ s16 field_0x260;
    /* 0x262 */ u8 field_0x262;
    /* 0x263 */ u8 field_0x263;
    /* 0x264 */ u8 field_0x264;
    /* 0x265 */ u8 field_0x265;
    /* 0x268 */ JGeometry::TVec3f field_0x268;
    /* 0x274 */ JGeometry::TVec3f field_0x274;
    /* 0x280 */ JGeometry::TVec3f field_0x280;
    /* 0x28C */ JGeometry::TVec3f field_0x28c;
    /* 0x298 */ JGeometry::TVec3f field_0x298;
}; // Size: 0x2A4

#endif
