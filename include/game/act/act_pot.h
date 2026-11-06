#ifndef GAME_ACT_ACT_POT_H
#define GAME_ACT_ACT_POT_H

// Code: game/act_pot.cpp.

#include "game/ActorBase.h"

class JPABaseEmitter;

// The TUBO actor ("Pot", docs/names/actors.tsv, high confidence): the spawner
// constructs it with fn_80204040, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A9428.
class Pot_804A9428 : public ActorBase_804A5770 {
public:
    Pot_804A9428() {}
    /* 0x08 */ virtual ~Pot_804A9428();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x24 */ virtual void vfunc_0x24();
    /* 0x48 */ virtual void vfunc_0x48();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);

    /* 0x230 */ JPABaseEmitter* mEmitter;
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ u8 field_0x254;
    /* 0x255 */ u8 field_0x255[3];
}; // Size: 0x258

#endif
