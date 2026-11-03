#ifndef GAME_TU_801ED3DC_H
#define GAME_TU_801ED3DC_H

// Exports of game/tu_801ED3DC.cpp. The stopwatch is embedded 0x18 bytes into
// the 0x24-byte entries of the table game/tu_80215A94.cpp walks (fn_801ED3DC,
// fn_801ED458, fn_801ED5C8, fn_801ED798, fn_801ED7B0, fn_801ED7F0 and
// fn_801ED80C are called with &entry->field_0x18); the countdown is embedded
// at 0x2B6 of the horse-race man actor (game/act_horse_race_man.cpp: fn_801ED828,
// fn_801ED8F0, fn_801ED968 and fn_801ED974).

#include "types.h"

// A stopwatch that runs at 60 frames a second and stops at 2 minutes (7200
// frames); no vtable, ctor or dtor symbol, named after its lowest-address
// function.
struct cls_801ED3DC {
    /* 0x0 */ u8 mStopped;
    /* 0x1 */ u8 pad_0x1[3];
    /* 0x4 */ s32 mFrames;
    /* 0x8 */ u8 mSeconds; // at most 119
    /* 0x9 */ u8 mCentis;  // at most 99
    // Records the displayed time in lbl_8053AEF8's slot `slot`, and `rank`
    // (0-3) when it is in range.
    void fn_801ED3DC(s32 slot, s32 rank);
    // One frame: counts mFrames up while running; `jitter` (the callers pass
    // a random number) nudges the last digit of mCentis.
    void fn_801ED458(u8 jitter);
    // Stops the watch, adding `fraction` (0-1) of a frame to the displayed time.
    void fn_801ED5C8(f32 fraction);
    // Zeroes everything and leaves the watch running (0).
    void fn_801ED798();
    // True when the watch has stopped at the 2 minute limit (1, 7200, 119, 99).
    bool fn_801ED7B0();
    // Both stop the watch and zero the time.
    void fn_801ED7F0();
    void fn_801ED80C();
}; // Size: 0xA (the entries embedding it are 0x24 bytes)

// A frame countdown (the horse-race man's start count). Named after its
// lowest-address function.
struct cls_801ED828 {
    /* 0x0 */ u16 mFrames;
    // Draws the digit 1-3 for the current second at the screen centre.
    void fn_801ED828();
    // Counts down; plays a sound every full second.
    void fn_801ED8F0();
    // Starts the countdown at 180 frames.
    void fn_801ED968();
    // Clears the countdown.
    void fn_801ED974();
}; // Size: 0x2

#endif
