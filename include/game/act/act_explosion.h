#ifndef GAME_ACT_ACT_EXPLOSION_H
#define GAME_ACT_ACT_EXPLOSION_H

// Code: game/act_explosion.cpp.

#include "game/ActorBase.h"

// Named after its vtable (0x804A7FB8).
class Explosion_804A7FB8 : public ActorBase_804A5770 {
public:
    Explosion_804A7FB8() {}
    virtual ~Explosion_804A7FB8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ s32 field_0x234; // 20 after init
    /* 0x238 */ JPABaseEmitter* mEmitter238; // released by the destructor
}; // Size: 0x23C

#endif
