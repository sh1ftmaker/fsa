#ifndef GAME_ACT_ACT_VAATI_BOSS_H
#define GAME_ACT_ACT_VAATI_BOSS_H

// Owner: game/act_vaati_boss.cpp (not written yet).

#include "types.h"
#include "game/SpriteAnim.h"

// The member at 0x568 of the Vaati boss actor (code in game/tu_801E667C.cpp,
// called from the actor's functions at 0x801D5B58, 0x801D6398 and the
// constructor 0x801D6904); named after its constructor, 0x801E6AB8 (no
// vtable). A short timed sequence that plays an animation on the players in
// mActive and ends after state 4. mState steps 0 -> 1 -> 2 -> 3 -> 4.
// Size: 0x48 (the actor's next member is at 0x5B0).
class cls_801E6AB8 {
public:
    cls_801E6AB8();
    // 0x801E667C: steps the timer and the state; true once state 4 is reached.
    bool fn_801E667C();
    // 0x801E67C4: enters state `state`.
    void fn_801E67C4(s32 state);
    // 0x801E6A84: starts the sequence (state 1) if it is idle.
    void fn_801E6A84(s32 arg);

    /* 0x00 */ s32 mState;
    /* 0x04 */ s32 mTimer;
    /* 0x08 */ s32 field_0x8; // set by fn_801E6A84; state 4 acts only when 0
    /* 0x0C */ u8 mActive[4]; // per player
    /* 0x10 */ SpriteAnim_801D267C mAnim;
}; // Size: 0x48

#endif
