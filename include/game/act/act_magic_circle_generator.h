#ifndef GAME_ACT_ACT_MAGIC_CIRCLE_GENERATOR_H
#define GAME_ACT_ACT_MAGIC_CIRCLE_GENERATOR_H

// Code: game/act_magic_circle_generator.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A9FB0.
class cls_804A9FB0 : public ActorBase_804A5770 {
public:
    cls_804A9FB0() {}
    /* 0x08 */ virtual ~cls_804A9FB0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    // 0x802E7DB8: sets mBox to (-20, -20)-(20, 20), then takes the first KEY0
    // on the ground (z == 0, state 6) whose box overlaps it; that key's
    // position gets the circle's effects. Returns false when none is found.
    bool fn_802E7DB8();
    /* 0x230 */ JPABaseEmitter* mEmitter230; // released by the destructor
    /* 0x234 */ JPABaseEmitter* mEmitter234; // released by the destructor
    /* 0x238 */ JPABaseEmitter* mEmitter238; // released by the destructor
    /* 0x23C */ JPABaseEmitter* mEmitter23c; // released by the destructor
    /* 0x240 */ JPABaseEmitter* mEmitter240; // released by the destructor
    /* 0x244 */ s32 field_0x244; // 1 while the circle is active, 0 when the view hides it
    /* 0x248 */ s32 field_0x248; // field_0x90 & 7 (set by init)
    /* 0x24C */ s32 field_0x24c; // countdown, 0x14 when init activates the circle
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ s32 field_0x254;
    /* 0x258 */ s32 field_0x258;
    /* 0x25C */ s32 field_0x25c;
    /* 0x260 */ s32 field_0x260;
    /* 0x264 */ s32 field_0x264;
    /* 0x268 */ s32 field_0x268;
    /* 0x26C */ s32 field_0x26c;
    /* 0x270 */ s32 field_0x270;
}; // Size: 0x274

#endif
