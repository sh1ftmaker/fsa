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
    /* 0x230 */ JPABaseEmitter* mEmitter230; // released by the destructor
    /* 0x234 */ JPABaseEmitter* mEmitter234; // released by the destructor
    /* 0x238 */ JPABaseEmitter* mEmitter238; // released by the destructor
    /* 0x23C */ JPABaseEmitter* mEmitter23c; // released by the destructor
    /* 0x240 */ JPABaseEmitter* mEmitter240; // released by the destructor
    /* 0x244 */ u8 field_0x244[0x30]; // not yet known
}; // Size: 0x274

#endif
