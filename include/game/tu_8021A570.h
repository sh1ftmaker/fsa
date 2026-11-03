#ifndef GAME_TU_8021A570_H
#define GAME_TU_8021A570_H

// Exports of game/tu_8021A570.cpp: a 0xE-byte swinging-head animation embedded
// in Kaepora Gaebora (act_kaepora_gaebora +0x2C8, act_kaepora_gaebora_ending
// +0x2A0). It sways mAngle back and forth between two limits, pausing at the
// ends and at the centre, with random pauses.

#include "types.h"

// No vtable; named after its constructor (fn_8021ACE0, zeroes every field,
// called by the actors' constructors).
class cls_8021ACE0 {
public:
    cls_8021ACE0();
    // Advances one frame. `flag` selects the actor's "settle at the centre"
    // mode. Returns true on the frame a sway stops at an end and on the
    // frame a centre pause starts.
    bool fn_8021A570(bool flag);
    // Picks the next random state (sway or pause) and its length.
    void fn_8021ABC0();

    /* 0x00 */ s32 mState;  // 0 pause, 1 sway
    /* 0x04 */ s16 mAngle;
    /* 0x06 */ u16 mTimer;  // frames until the current sway/pause ends
    /* 0x08 */ u16 mTimer2; // frames until the next random pick
    /* 0x0A */ s16 mSpeed;  // angle step per frame while swaying
    /* 0x0C */ u8 field_0xc;
    /* 0x0D */ u8 mFlipped; // swings about 0x8000 instead of 0
}; // Size: 0xE

#endif
