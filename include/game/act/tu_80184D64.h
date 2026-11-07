#ifndef GAME_ACT_TU_80184D64_H
#define GAME_ACT_TU_80184D64_H

// Code: game/tu_80184D64.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A18E0.
class cls_804A18E0 : public ActorBase_804A5770 {
public:
    cls_804A18E0() {}
    /* 0x08 */ virtual ~cls_804A18E0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 field_0x230; // state
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ s32 field_0x254;
    /* 0x258 */ s32 field_0x258;
    /* 0x25C */ s32 field_0x25c;
    /* 0x260 */ s32 field_0x260; // slot of the lead trail point (-1: none)
    /* 0x264 */ s32 field_0x264; // sprite frame offset
    /* 0x268 */ s32 field_0x268;
    /* 0x26C */ f32 field_0x26c;
    /* 0x270 */ s16 field_0x270;
    /* 0x272 */ u16 field_0x272;
    /* 0x274 */ s16 field_0x274[10];
    /* 0x288 */ f32 field_0x288[3];
    /* 0x294 */ JGeometry::TVec3f field_0x294[10];
    /* 0x30C */ JGeometry::TVec3f field_0x30c[10];
    /* 0x384 */ f32 field_0x384[3]; // copy of mPos made by init
}; // Size: 0x390

#endif
