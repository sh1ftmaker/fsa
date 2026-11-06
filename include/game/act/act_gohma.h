#ifndef GAME_ACT_ACT_GOHMA_H
#define GAME_ACT_ACT_GOHMA_H

#include "game/ActorBase.h"

class JPABaseEmitter;

class Gohma_804A307C : public ActorBase_804A5770 {
public:
    Gohma_804A307C() {}
    /* 0x08 */ virtual ~Gohma_804A307C();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x44 */ virtual void vfunc_0x44();

    void fn_801AB6A8();
    void fn_801AAF44();

    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ s32 mTimers[4];
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ s32 field_0x254;
    /* 0x258 */ s32 field_0x258;
    /* 0x25C */ s32 field_0x25c;
    /* 0x260 */ s32 field_0x260;
    /* 0x264 */ JGeometry::TVec3f field_0x264;
    /* 0x270 */ f32 field_0x270;
    /* 0x274 */ f32 field_0x274;
    /* 0x278 */ f32 field_0x278;
    /* 0x27C */ JPABaseEmitter* mEmitters[2];
}; // Size: 0x284

#endif
