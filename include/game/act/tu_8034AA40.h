#ifndef GAME_ACT_TU_8034AA40_H
#define GAME_ACT_TU_8034AA40_H

// Code: game/tu_8034AA40.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804AC8C0.
class cls_804AC8C0 : public ActorBase_804A5770 {
public:
    cls_804AC8C0() {}
    /* 0x08 */ virtual ~cls_804AC8C0();
    /* 0x0C */ virtual void init();
    /* 0x10 */ virtual void vfunc_0x10();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    // 0x8034AAA0: marks the cuccos ('NIWA') inside the area and counts them; true progress rewards.
    s32 fn_8034AAA0();
    // 0x8034ACF4: per player, the facing they left by and the push into the area.
    void fn_8034ACF4();
    /* 0x230 */ s32 field_0x230; // state
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c; // facing (0-3)
    /* 0x240 */ u8 field_0x240[4]; // not yet known
    /* 0x244 */ s32 field_0x244; // kind (low nibble of field_0x90)
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c; // bits 4-11 of field_0x90
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ s32 field_0x254;
    /* 0x258 */ s32 field_0x258;
    /* 0x25C */ s32 field_0x25c;
    /* 0x260 */ s32 field_0x260[4];
    /* 0x270 */ s32 field_0x270;
    /* 0x274 */ s32 field_0x274[4];
    /* 0x284 */ u8 field_0x284[4];
    /* 0x288 */ u8 field_0x288[4];
    /* 0x28C */ u8 field_0x28c[3];
    /* 0x28F */ u8 field_0x28f;
    /* 0x290 */ JGeometry::TVec3f field_0x290[4];
    /* 0x2C0 */ s32 field_0x2c0;
}; // Size: 0x2C4

#endif
