#ifndef GAME_ACT_ACT_FAIRY_INVINCIBLE_H
#define GAME_ACT_ACT_FAIRY_INVINCIBLE_H

// Code: game/act_fairy_invincible.cpp.

#include "game/ActorBase.h"

// The INFR actor ("Invincibility fairy", docs/names/actors.tsv, med
// confidence): the spawner constructs it with fn_8020258C, which stores this
// vtable (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AB4C0.
class InvincibilityFairy_804AB4C0 : public ActorBase_804A5770 {
public:
    InvincibilityFairy_804AB4C0() {}
    /* 0x08 */ virtual ~InvincibilityFairy_804AB4C0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x44 */ virtual void vfunc_0x44();
    /* 0x230 */ u8 field_0x230[0x34]; // not yet known
    /* 0x264 */ JPABaseEmitter* mEmitter264; // released by the destructor
    /* 0x268 */ JPABaseEmitter* mEmitter268; // released by the destructor
}; // Size: 0x26C

#endif
