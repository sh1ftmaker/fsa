#ifndef GAME_ACT_ACT_ICICLE_H
#define GAME_ACT_ACT_ICICLE_H

// Code: game/act_icicle.cpp.

#include "game/ActorBase.h"
#include "game/SpriteAnim.h"

// The ICCL actor ("Icicle", docs/names/actors.tsv, med confidence): the
// spawner constructs it with its constructor (0x8010C00C). Named after its
// vtable, 0x8049CFC8.
class Icicle_8049CFC8 : public ActorBase_804A5770 {
public:
    Icicle_8049CFC8();
    /* 0x08 */ virtual ~Icicle_8049CFC8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    // 0x8010B284: looks at the first player (0-3) that lines up with the
    // icicle (within 40 units in x, not above it in y) and probes the map
    // tiles above the icicle every 16 units, up to 24 steps (fn_80226850),
    // until the player's height is reached. Returns true when no blocking
    // tile was found; false when no player lines up.
    bool fn_8010B284();
    // 0x8010B40C: the icicle's state machine (state 1-5; init() calls it with
    // 5). Stores the state in field_0x230 and always returns true.
    bool fn_8010B40C(s32 state);

    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ f32 field_0x23c; // fall time: += 0.1f a frame in state 1 (update)
    /* 0x240 */ f32 field_0x240; // 1.0f after construction
    /* 0x244 */ SpriteAnim_801D267C mSprite;
    // The emitter the destructor releases with fn_801EE434.
    /* 0x27C */ JPABaseEmitter* field_0x27c;
}; // Size: 0x280

#endif
