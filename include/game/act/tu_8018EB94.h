#ifndef GAME_ACT_TU_8018EB94_H
#define GAME_ACT_TU_8018EB94_H

// Code: game/tu_8018EB94.cpp.

#include "game/ActorBase.h"

// Spawned for 'DGT2'; moved here from game/act/tu_801F9484.h.
class cls_804A1FA0 : public ActorBase_804A5770 {
public:
    cls_804A1FA0() {}
    /* 0x08 */ virtual ~cls_804A1FA0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    // Slot 0x44: stops the three emitters and forgets them.
    /* 0x44 */ virtual void vfunc_0x44();
    // SHARED-EDIT: carved from u8 field_0x230[0x58] (game/tu_8018EB94.cpp init).
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
    /* 0x258 */ u8 field_0x258[4];
    /* 0x25C */ s32 field_0x25c;
    /* 0x260 */ s32 field_0x260;
    /* 0x264 */ s32 field_0x264;
    /* 0x268 */ f32 field_0x268;
    /* 0x26C */ f32 field_0x26c;
    /* 0x270 */ f32 field_0x270;
    /* 0x274 */ f32 field_0x274;
    /* 0x278 */ f32 field_0x278;
    /* 0x27C */ f32 field_0x27c;
    /* 0x280 */ f32 field_0x280;
    /* 0x284 */ f32 field_0x284;
    /* 0x288 */ JGeometry::TVec3f field_0x288[6];
    /* 0x2D0 */ JGeometry::TVec3f field_0x2d0[20];
    /* 0x3C0 */ JPABaseEmitter* mEmitters[3]; // released by the destructor
}; // Size: 0x3CC

#endif
