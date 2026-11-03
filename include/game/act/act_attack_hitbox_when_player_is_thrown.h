#ifndef GAME_ACT_ACT_ATTACK_HITBOX_WHEN_PLAYER_IS_THROWN_H
#define GAME_ACT_ACT_ATTACK_HITBOX_WHEN_PLAYER_IS_THROWN_H

// Code: game/act_attack_hitbox_when_player_is_thrown.cpp.

#include "game/ActorBase.h"

// The THCL actor ("Attack hitbox when player is thrown",
// docs/names/actors.tsv, med confidence): the spawner constructs it with
// fn_80200158, which stores this vtable (config/G4SE01/game_units.tsv). Named
// after its vtable, 0x804A7C40.
class AttackHitboxWhenPlayerIsThrown_804A7C40 : public ActorBase_804A5770 {
public:
    AttackHitboxWhenPlayerIsThrown_804A7C40() {}
    /* 0x08 */ virtual ~AttackHitboxWhenPlayerIsThrown_804A7C40();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x24 */ virtual void vfunc_0x24();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    /* 0x230 */ u8 field_0x230[0x4]; // not yet known
}; // Size: 0x234

#endif
