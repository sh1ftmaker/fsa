#ifndef GAME_ACT_ACT_LIFT_H
#define GAME_ACT_ACT_LIFT_H

// Code: game/act_lift.cpp.

#include "game/ActorBase.h"

// The LIFT actor ("Lift", docs/names/actors.tsv, high confidence): the spawner
// constructs it with fn_80203144, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A6F6C.
class Lift_804A6F6C : public ActorBase_804A5770 {
public:
    Lift_804A6F6C() {}
    /* 0x08 */ virtual ~Lift_804A6F6C();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    // Steps the lift around its spawn point (fn_80256410, called by update).
    void fn_80256410();
    // Tests lift `index` (fn_802565BC); clrlwi 24 by its caller.
    bool fn_802565BC(s32 index);

    // The mode is the low 7 bits of field_0x90, compared as signed.
    s32 getMode() { return field_0x90 & 0x7F; }

    /* 0x230 */ u32 field_0x230; // step counter; limit is 0x90 bits 17-21, times 32
    /* 0x234 */ u32 field_0x234; // frames to wait; 60 after each step
    /* 0x238 */ s16 field_0x238; // angle, +0x40 per frame while mode < 6
    /* 0x23A */ s16 field_0x23a; // angle, +0x40 per frame in mode 8
    /* 0x23C */ void* field_0x23c; // resource from lbl_8053AAF8->0xA4 (init)
}; // Size: 0x240

#endif
