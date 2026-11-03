#ifndef GAME_CLS_8021C2B4_H
#define GAME_CLS_8021C2B4_H

#include "types.h"
#include "JSystem/JGeometry.h"
#include "game/cls_8021CBA0.h"

// One flying piece aimed at a player (shadow Link's thrown object), defined
// in game/tu_8021C2B4.cpp. No vtable; named after its constructor
// (fn_8021C5B0, the element constructor act_shadow_link_generator passes to
// __construct_array).
class cls_8021C5B0 {
public:
    cls_8021C5B0();

    /* 0x00 */ u8 mActive;
    /* 0x01 */ u8 mDone;
    /* 0x04 */ s32 mPlayer;
    /* 0x08 */ JGeometry::TVec2f mStart;
    /* 0x10 */ JGeometry::TVec2f mTarget;
    /* 0x18 */ JGeometry::TVec3f mPos;
    /* 0x24 */ JGeometry::TVec3f mVel;
    /* 0x30 */ cls_8021CBA0 mAnim;
}; // Size: 0x48

// The four pieces, embedded in act_shadow_link_generator (+0x2D8); named
// after its first method.
class cls_8021C2B4 {
public:
    // Moves each active piece under gravity, steps its animation and draws it
    // at its position for its player (fn_80230498).
    void fn_8021C2B4();
    // Starts a piece for every present player with a target
    // (fn_801C0FEC); the others are marked done.
    void fn_8021C468();

    /* 0x00 */ cls_8021C5B0 mPieces[4];
}; // Size: 0x120

#endif
