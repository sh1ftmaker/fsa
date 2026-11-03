#ifndef GAME_ACT_ACT_WARP_PILLAR_H
#define GAME_ACT_ACT_WARP_PILLAR_H

// Code: game/act_warp_pillar.cpp.

#include "game/ActorBase.h"

// The WPPR actor ("Warp pillar", docs/names/actors.tsv, medium confidence):
// the spawner constructs it with the ctor that stores this vtable, 0x804AC0A8
// (config/G4SE01/game_units.tsv). Named after its vtable.
class WarpPillar_804AC0A8 : public ActorBase_804A5770 {
public:
    WarpPillar_804AC0A8() {}
    /* 0x08 */ virtual ~WarpPillar_804AC0A8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 field_0x230; // state: update()'s switch; init zeroes it
    /* 0x234 */ s32 field_0x234; // counter: init zeroes it, update adds 1
    /* 0x238 */ s32 field_0x238; // field_0x90 bits 4-7 (init)
    /* 0x23C */ s32 field_0x23c; // field_0x90 bits 8-11 (init)
    /* 0x240 */ s32 field_0x240; // counter: init zeroes it, update sets 50
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248[4]; // one per player (init zeroes them; update's loop reads them)
    /* 0x258 */ f32 field_0x258; // 0.0f (init)
    /* 0x25C */ bool field_0x25c[4]; // one flag per player (init clears them)
    /* 0x260 */ bool field_0x260[4]; // one flag per player (init clears them)
    /* 0x264 */ JPABaseEmitter* mEmitter264; // effect created by init
    /* 0x268 */ JPABaseEmitter* mEmitter268; // effect created by init
    /* 0x26C */ JPABaseEmitter* mEmitter26c[4]; // one per player (update)
};

#endif
