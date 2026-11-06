#ifndef GAME_ACT_ACT_CUCCO_H
#define GAME_ACT_ACT_CUCCO_H

// Code: game/act_cucco.cpp.

#include "game/ActorBase.h"

// The NIWA actor ("Cucco", docs/names/actors.tsv, high confidence): the
// spawner constructs it with fn_80202FA0, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AB8C0.
class Cucco_804AB8C0 : public ActorBase_804A5770 {
public:
    Cucco_804AB8C0() {}
    /* 0x08 */ virtual ~Cucco_804AB8C0();
    /* 0x0C */ virtual void init();
    /* 0x10 */ virtual void vfunc_0x10();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    /* 0x230 */ u8 field_0x230[0x71]; // not yet known
    // Tested == 1 by the player's cucco glide (Player_804A6F38::fn_80263078, tu_8025EB18).
    /* 0x2A1 */ u8 field_0x2a1;
    /* 0x2A2 */ u8 field_0x2a2[0x52]; // not yet known
    /* 0x2F4 */ JPABaseEmitter* mEmitter2f4; // released by the destructor
    /* 0x2F8 */ JPABaseEmitter* mEmitter2f8; // released by the destructor
    /* 0x2FC */ JPABaseEmitter* mEmitter2fc; // released by the destructor
    /* 0x300 */ JPABaseEmitter* mEmitter300; // released by the destructor
}; // Size: 0x304

#endif
