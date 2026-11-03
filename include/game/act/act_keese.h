#ifndef GAME_ACT_ACT_KEESE_H
#define GAME_ACT_ACT_KEESE_H

// Code: game/act_keese.cpp.

#include "game/ActorBase.h"

// The KETH actor ("Keese", docs/names/actors.tsv, high confidence): the
// spawner constructs it with fn_80203B3C, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A02B8.
class Keese_804A02B8 : public ActorBase_804A5770 {
public:
    Keese_804A02B8() {}
    /* 0x08 */ virtual ~Keese_804A02B8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    /* 0x230 */ u8 field_0x230[0x5C]; // not yet known
}; // Size: 0x28C

#endif
