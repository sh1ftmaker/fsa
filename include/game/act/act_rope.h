#ifndef GAME_ACT_ACT_ROPE_H
#define GAME_ACT_ACT_ROPE_H

// Code: game/act_rope.cpp.

#include "game/EnemyBase.h"

// The ROPE actor ("Rope", docs/names/actors.tsv, high confidence): the spawner
// constructs it with the out-of-line constructor 0x801B7DE0, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A3A68.
class Rope_804A3A68 : public EnemyBase_804A4808 {
public:
    Rope_804A3A68(); // fn_801B7DE0 (0x801B7DE0), out of line
    /* 0x08 */ virtual ~Rope_804A3A68();
    /* 0x0C */ virtual void init();
    /* 0x20 */ virtual void draw();
    /* 0x90 */ virtual void vfunc_0x90(u8 mapHit);
    /* 0x94 */ virtual void vfunc_0x94();
}; // Size: 0x2A8

#endif
