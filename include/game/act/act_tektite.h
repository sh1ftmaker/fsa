#ifndef GAME_ACT_ACT_TEKTITE_H
#define GAME_ACT_ACT_TEKTITE_H

// Code: game/act_tektite.cpp.

#include "game/ActorBase.h"

// The TKTA actor ("Tektite", docs/names/actors.tsv, high confidence): the
// spawner constructs it with fn_80203C2C, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x8049FD50.
class Tektite_8049FD50 : public ActorBase_804A5770 {
public:
    Tektite_8049FD50() {}
    /* 0x08 */ virtual ~Tektite_8049FD50();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x24 */ virtual void vfunc_0x24();
    /* 0x230 */ s32 field_0x230; // 0 or 1 after init; update branches on it
    /* 0x234 */ s32 field_0x234; // countdown (update)
    /* 0x238 */ s32 field_0x238; // countdown (update); init sets 0x14 + random(0..19), or 0
    /* 0x23C */ s32 field_0x23c; // countdown (update)
    /* 0x240 */ s32 field_0x240; // zeroed by init, not yet read
    /* 0x244 */ s32 field_0x244; // zeroed by init, not yet read
    /* 0x248 */ s32 field_0x248; // zeroed by init, not yet read
    /* 0x24C */ u8 field_0x24c[4]; // not yet known
    /* 0x250 */ s32 field_0x250; // toggles 0 / 1 in update; draw adds it to 0x49 + 3 * field_0x90
    /* 0x254 */ u16 field_0x254; // tile code of mPos (fn_80226998), 0xFFFF until first looked up
    /* 0x258 */ JGeometry::TVec3f field_0x258; // (x, y) = 2 * (sin, cos) of the angle in init and update
}; // Size: 0x264

#endif
