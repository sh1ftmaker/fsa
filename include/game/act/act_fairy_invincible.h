#ifndef GAME_ACT_ACT_FAIRY_INVINCIBLE_H
#define GAME_ACT_ACT_FAIRY_INVINCIBLE_H

// Code: game/act_fairy_invincible.cpp.

#include "game/ActorBase.h"

// The INFR actor ("Invincibility fairy", docs/names/actors.tsv, med
// confidence): the spawner constructs it with fn_8020258C, which stores this
// vtable (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AB4C0.
class InvincibilityFairy_804AB4C0 : public ActorBase_804A5770 {
public:
    InvincibilityFairy_804AB4C0() {}
    // The helpers below are inline in act_fairy_invincible.cpp.
    void clearWork();
    void clearTracking();
    void followEmitters();
    void releaseEmitters();
    /* 0x08 */ virtual ~InvincibilityFairy_804AB4C0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x44 */ virtual void vfunc_0x44();
    /* 0x230 */ s32 field_0x230; // state, 0 at init
    /* 0x234 */ s32 mCounters[2][2]; // pairs, stepped by 8 bytes; each counts down to 0 in update()
    //   [0][0] 0x234: 0x400 at init, 0x1F4 when tracking starts, 1000 in state 1
    //   [0][1] 0x238: 0 at init; 4 when it reaches 0, with the sprite frame (0x250)
    //   [1][0] 0x23C: 10 at init; 5 after each re-aim in state 1
    //   [1][1] 0x240: timer, 30 + a random 0..29 at init
    /* 0x244 */ s32 field_0x244; // bob angle (+0x300 a frame) in state 0; the tracked player in state 1
    /* 0x248 */ s32 field_0x248; // random 0..32767 at init; counts up in state 1
    /* 0x24C */ s32 field_0x24c; // 0 at init; flag for the state 0 timer
    /* 0x250 */ s32 field_0x250; // sprite frame 0..2, 0 at init; the draw adds 0x5E to it
    /* 0x254 */ s16 field_0x254; // aim angle, 0 at init; set in state 1
    /* 0x256 */ u8 field_0x256[2];
    /* 0x258 */ f32 field_0x258; // target for field_0x3c.x; 0.0f at init
    /* 0x25C */ f32 field_0x25c; // target for field_0x3c.y; 0.0f at init
    /* 0x260 */ f32 field_0x260; // target for mPos.z; 10.0f at init, 7 + 2 sin in state 0
    /* 0x264 */ JPABaseEmitter* mEmitter264; // released by the destructor
    /* 0x268 */ JPABaseEmitter* mEmitter268; // released by the destructor
}; // Size: 0x26C

#endif
