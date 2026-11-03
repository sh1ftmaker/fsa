#ifndef GAME_ACT_ACT_SANDSTORM_H
#define GAME_ACT_ACT_SANDSTORM_H

// Code: game/act_sandstorm.cpp.

#include "game/ActorBase.h"
#include "game/ActorManager.h"

class JPABaseEmitter;

// The SDSM actor ("Sandstorm", docs/names/actors.tsv, high confidence): the
// spawner constructs it with fn_802005FC (allocating 0x258 bytes), which
// stores this vtable (config/G4SE01/game_units.tsv). Named after its vtable,
// 0x804AA280.
class Sandstorm_804AA280 : public ActorBase_804A5770 {
public:
    Sandstorm_804AA280() {}
    /* 0x08 */ virtual ~Sandstorm_804AA280();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw() {}

    /* 0x230 */ ActorSoundObj_804578F4* mSound; // own sound object, made in init
    /* 0x234 */ JPABaseEmitter* mEmitter;       // the sandstorm effect
    /* 0x238 */ u8 field_0x238[0x4];            // not used here
    /* 0x23C */ s32 mKind; // field_0x90 & 0xF: lowest set bit 0, 1, 2 picks the effect id
    /* 0x240 */ s32 mTimer; // 100 after the effect is switched, counts down to the stage change
    /* 0x244 */ JGeometry::TVec3f mPos244; // minus the camera's view translation, z 0
    /* 0x250 */ s32 mStarted; // set once the view reports field_0xc9
    /* 0x254 */ s32 field_0x254; // (field_0x90 >> 4) & 0xF; not read here
}; // Size: 0x258

#endif
