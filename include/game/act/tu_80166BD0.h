#ifndef GAME_ACT_TU_80166BD0_H
#define GAME_ACT_TU_80166BD0_H

// Code: game/tu_80166BD0.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A0668.
class cls_804A0668 : public ActorBase_804A5770 {
public:
    cls_804A0668() {}
    /* 0x08 */ virtual ~cls_804A0668();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x44 */ virtual void vfunc_0x44();
    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ f32 field_0x254;
    /* 0x258 */ f32 field_0x258;
    /* 0x25C */ f32 field_0x25c;
    /* 0x260 */ f32 field_0x260;
    /* 0x264 */ f32 field_0x264;
    /* 0x268 */ f32 field_0x268;
    /* 0x26C */ JPABaseEmitter* mEmitter26c; // released by the destructor
}; // Size: 0x270

#endif
