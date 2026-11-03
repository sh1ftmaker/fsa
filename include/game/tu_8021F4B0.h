#ifndef GAME_TU_8021F4B0_H
#define GAME_TU_8021F4B0_H

// Exports of game/tu_8021F4B0.cpp: a 0x40-byte effect sequence (three
// particle emitters that move to a point, hold, then shrink away) embedded in
// the shadow Link generator (act_shadow_link_generator, +0x3F8).

#include "types.h"
#include "JSystem/JGeometry.h"

class JPABaseEmitter;

// No vtable; named after its constructor (fn_8021FB44, called by the actor's
// constructor). The destructor (fn_8021FAB8) deletes the emitters. States:
// 0 idle, 1 move to mTarget1 (240 frames), 2 wait (60), 3 move to mTarget2
// (120 frames, then 4), 4 done (update returns true), 5 shrink away: mPos.z
// to mEndZ and mScale to 0 over 60 frames, then delete the emitters and
// back to 0 (fn_8021F914 sets the state; the actor starts 5 from outside).
class cls_8021FB44 {
public:
    cls_8021FB44();
    ~cls_8021FB44();
    // Advances the sequence one frame; returns true in states 4 and 5 and on
    // the frame state 3 ends.
    bool fn_8021F4B0();
    // Starts state `state` with a zeroed timer.
    void fn_8021F914(s32 state);
    // Deletes the three emitters.
    void fn_8021F924();
    // Starts the sequence at *pos: spawns the three emitters there.
    void fn_8021F9AC(const JGeometry::TVec3f* pos);

    /* 0x00 */ s32 mState;
    /* 0x04 */ s32 mTimer;
    /* 0x08 */ JGeometry::TVec3f mPos;
    /* 0x14 */ JGeometry::TVec3f mTarget1;
    /* 0x20 */ JGeometry::TVec3f mTarget2;
    /* 0x2C */ f32 mEndZ;
    /* 0x30 */ f32 mScale;
    /* 0x34 */ JPABaseEmitter* mEmitters[3];
}; // Size: 0x40

#endif
