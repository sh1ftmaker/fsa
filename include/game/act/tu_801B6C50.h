#ifndef GAME_ACT_TU_801B6C50_H
#define GAME_ACT_TU_801B6C50_H

// Code: game/tu_801B6C50.cpp.

#include "game/ActorBase.h"

// Spawned for 'GLWP'; moved here from game/act/tu_801F9484.h.
class cls_804A39DC : public ActorBase_804A5770 {
public:
    cls_804A39DC() {}
    /* 0x08 */ virtual ~cls_804A39DC();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 field_0x230; // state (0-8)
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238; // countdown
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c; // bits 8-11 of field_0x90
    /* 0x250 */ s32 field_0x250; // bits 0-3
    /* 0x254 */ s32 field_0x254; // bits 4-7
    /* 0x258 */ s32 field_0x258; // bits 12-15 (0 when above 10)
    /* 0x25C */ f32 field_0x25c; // the screen fade amount
    /* 0x260 */ f32 field_0x260;
    /* 0x264 */ JGeometry::TVec3f field_0x264[4];
    /* 0x294 */ JPABaseEmitter* mEmitters[4]; // released by the destructor
}; // Size: 0x2A4

#endif
