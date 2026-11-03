#ifndef GAME_ACT_ACT_WIND_ENVIRONMENT_H
#define GAME_ACT_ACT_WIND_ENVIRONMENT_H

// Code: game/act_wind_environment.cpp.

#include "game/ActorBase.h"
#include "game/ActorManager.h"

class JPABaseEmitter;

// The WIND actor ("Wind environment" in docs/names/actors.tsv, med
// confidence; stages 8-1 and 8-2): the spawner (fn_801F9484) allocates 0x248
// bytes and calls the out-of-line constructor 0x801070B4, which stores this
// vtable (0x8049CC98). Named after that vtable.
class WindEnvironment_8049CC98 : public ActorBase_804A5770 {
public:
    WindEnvironment_8049CC98();
    /* 0x08 */ virtual ~WindEnvironment_8049CC98();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();

    // 0x80106ADC: pushes every player on this actor's layer in the wind's
    // direction (mKind) unless a player state test or the tile (0x3EE,
    // 0x3EF) forbids it; `state` 1 is the base speed, 2 doubles it, others
    // push nothing.
    void fn_80106ADC(s32 state);
    // True when the wind should be blowing: update starts the effect (state 0 to 1) when it
    // holds and stops it (1 to 0) when it does not (inlined twice).
    bool shouldBlow();

    /* 0x230 */ s32 mState; // 0 or 1: whether the effect emits (update toggles it)
    /* 0x234 */ s32 mKind; // field_0x90 & 0xF: wind direction, 0 right, 1 left, 2 up, 3 down
    /* 0x238 */ s32 mCondition; // (field_0x90 >> 4) & 0xF: which test toggles the effect
    /* 0x23C */ s32 mTimer; // counts frames; not read here
    /* 0x240 */ JPABaseEmitter* mEmitter; // the wind effect, made in init
    /* 0x244 */ ActorSoundObj_804578F4* mSound; // own sound object, made in the constructor
}; // Size: 0x248

#endif
