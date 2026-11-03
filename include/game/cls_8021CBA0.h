#ifndef GAME_CLS_8021CBA0_H
#define GAME_CLS_8021CBA0_H

#include "types.h"

// A frame-count sequencer over three tables in lbl_8046A688 (types 0, 1, 2
// pick the table), defined in game/tu_8021C848.cpp. No vtable; named after
// its constructor (fn_8021CBA0 zeroes every member). Embedded in
// cls_8021C5B0 (+0x30).
class cls_8021CBA0 {
public:
    cls_8021CBA0();
    // Advances the timer; when the current step's frame count is used up,
    // moves to the next step and finishes (mDone) after the last one.
    // Reloads mFrames and returns mDone.
    bool fn_8021C848();
    // Restarts the sequence of the current type with table variant `variant`.
    void fn_8021C9C4(int variant);
    // Restarts the sequence with type `type` (the variant is kept).
    void fn_8021CA70(int type);
    // The total frame count of the current type's table.
    s32 fn_8021CB18();

    /* 0x00 */ s32 mType;
    /* 0x04 */ s32 mFrames; // frame count of the current step
    /* 0x08 */ s32 mStep;
    /* 0x0C */ s32 mTimer;
    /* 0x10 */ s32 mVariant;
    /* 0x14 */ u8 mDone;
}; // Size: 0x18

#endif
