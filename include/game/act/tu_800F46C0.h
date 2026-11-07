#ifndef GAME_ACT_TU_800F46C0_H
#define GAME_ACT_TU_800F46C0_H

// Code: game/tu_800F46C0.cpp.

#include "game/ActorBase.h"

// The IWAG actor ("Boulder spawner", docs/names/actors.tsv, med confidence):
// the spawner constructs it with fn_80202E74, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x8049C46C.
class BoulderSpawner_8049C46C : public ActorBase_804A5770 {
public:
    BoulderSpawner_8049C46C() {}
    /* 0x08 */ virtual ~BoulderSpawner_8049C46C();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230; // field_0x90's low byte: 2, 4 or other (spawn timer range)
    /* 0x231 */ u8 field_0x231[3];
    /* 0x234 */ s32 field_0x234; // a timer
}; // Size: 0x238

// Named after its vtable, 0x8049C4C0.
class Boulder_8049C4C0 : public ActorBase_804A5770 {
public:
    Boulder_8049C4C0() {}
    /* 0x08 */ virtual ~Boulder_8049C4C0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    /* 0x230 */ u8 field_0x230; // the state update() switches on (0-5)
    // SHARED-EDIT: carved from field_0x231[3], field_0x234[8], field_0x24c[0x14] (game/tu_800F46C0.cpp init).
    /* 0x231 */ u8 field_0x231;
    /* 0x232 */ s8 field_0x232;
    /* 0x233 */ s8 field_0x233;
    /* 0x234 */ u8 field_0x234;
    /* 0x235 */ u8 field_0x235[3]; // not yet known
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ JGeometry::TVec3f field_0x254;
    // game/tu_800F46C0.cpp (not written yet): update()'s state 3 and states 0.
    void fn_800F555C();
    void fn_800F5E08();
}; // Size: 0x260

#endif
