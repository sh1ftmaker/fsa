#ifndef GAME_ACT_ACT_FLOORMASTER_H
#define GAME_ACT_ACT_FLOORMASTER_H

// Code: game/act_floormaster.cpp.

#include "game/EnemyBase.h"

// The FLOR actor ("Floormaster", docs/names/actors.tsv, high confidence): the
// spawner constructs it with fn_8020F510, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A6004.
class Floormaster_804A6004 : public EnemyBase_804A4808 {
public:
    Floormaster_804A6004() {}
    /* 0x08 */ virtual ~Floormaster_804A6004();
    /* 0x0C */ virtual void init();
    /* 0x20 */ virtual void draw();
    /* 0x58 */ virtual bool vfunc_0x58();
    /* 0x5C */ virtual u8 vfunc_0x5C();
    /* 0x60 */ virtual u8 vfunc_0x60();
    /* 0x88 */ virtual void vfunc_0x88();
    /* 0x90 */ virtual void vfunc_0x90(u8 mapHit);
    /* 0x94 */ virtual void vfunc_0x94();
    /* 0xA0 */ virtual void vfunc_0xA0();
    /* 0x2A8 */ u8 field_0x2a8[0x8]; // not yet known
    /* 0x2B0 */ JPABaseEmitter* mEmitter2b0; // released by the destructor
    /* 0x2B4 */ u8 field_0x2b4[0x10]; // not yet known
}; // Size: 0x2C4

#endif
