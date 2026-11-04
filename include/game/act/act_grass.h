#ifndef GAME_ACT_ACT_GRASS_H
#define GAME_ACT_ACT_GRASS_H

// Code: game/act_grass.cpp.

#include "game/ActorBase.h"

// The GRAS actor ("Grass", docs/names/actors.tsv, high confidence): the
// spawner constructs it with fn_802041A8, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A92D0.
class Grass_804A92D0 : public ActorBase_804A5770 {
public:
    Grass_804A92D0() {}
    /* 0x08 */ virtual ~Grass_804A92D0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x24 */ virtual void vfunc_0x24();
    /* 0x48 */ virtual void vfunc_0x48();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    // Not virtual. Called from update(); true when a BLZ2 actor overlaps this
    // one's box, after which it plays the effect and sound itself.
    bool fn_802CF598();
    // Not virtual. The spawn state machine vfunc_0x24 runs after a hit; it
    // clears mAlive at its end, as vfunc_0x24 does.
    void fn_802CEC10();
    /* 0x230 */ s32 field_0x230; // state word, tested by fn_802CEC10
    /* 0x234 */ s32 field_0x234; // state word, tested first by fn_802CEC10
    /* 0x238 */ s32 field_0x238; // state word, tested by fn_802CEC10
    /* 0x23C */ s32 field_0x23c; // state word handed to ActorBase::fn_801F132C by vfunc_0x4C
    /* 0x240 */ s32 field_0x240; // countdown, 30 when init sets up the second branch
    /* 0x244 */ u8 field_0x244; // 0xFF from init
    /* 0x245 */ u8 field_0x245; // 0 from init
    /* 0x246 */ u8 field_0x246[0x2]; // not yet known
}; // Size: 0x248

#endif
