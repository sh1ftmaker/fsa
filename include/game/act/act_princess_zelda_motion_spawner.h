#ifndef GAME_ACT_ACT_PRINCESS_ZELDA_MOTION_SPAWNER_H
#define GAME_ACT_ACT_PRINCESS_ZELDA_MOTION_SPAWNER_H

// Code: game/act_princess_zelda_motion_spawner.cpp.

#include "game/ActorBase.h"

// The ZDCO actor ("Princess Zelda motion object spawner",
// docs/names/actors.tsv, med confidence): the spawner constructs it with
// fn_80200728, which stores this vtable (config/G4SE01/game_units.tsv). Named
// after its vtable, 0x804A375C.
class PrincessZeldaMotionObjectSpawner_804A375C : public ActorBase_804A5770 {
public:
    PrincessZeldaMotionObjectSpawner_804A375C() {}
    /* 0x08 */ virtual ~PrincessZeldaMotionObjectSpawner_804A375C();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    // mState indexes update()'s switch (0..10). init() sets it from mKind.
    /* 0x230 */ s32 mState;
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248; // result of fn_8022FD8C(mSerial)
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 mKind;       // low byte of the spawner's 5th argument
    /* 0x254 */ s32 field_0x254; // bits 22..26 of the 5th argument
    /* 0x258 */ s32 field_0x258; // bits 8..11 of the 5th argument: table index
    /* 0x25C */ s32 field_0x25c;
    /* 0x260 */ u8 field_0x260[0xC]; // not yet known
    /* 0x26C */ JGeometry::TVec3f mHomePos; // copy of mPos made by init()
}; // Size: 0x278

#endif
