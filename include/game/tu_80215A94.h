#ifndef GAME_TU_80215A94_H
#define GAME_TU_80215A94_H

// Exports of game/tu_80215A94.cpp: the 0x24-byte entry table that the
// horse-race man (game/act_horse_race_man.cpp) keeps at its 0x2B8 member and
// walks with fn_80215E48 and fn_80215E58.

#include "types.h"

// One 0x24-byte entry of that table. The table's entry count is the first
// word of entry 0, which fn_80215E58 reads as its loop bound. No vtable, ctor
// or dtor symbol; named after its lowest-address function that takes the
// table as its first argument.
struct cls_80215E48 {
    /* 0x0 */ s32 mCount; // entry 0 only: the number of entries in the table
    /* 0x4 */ s32 mId;    // the id fn_80215B6C passes to fn_8043D874
    /* 0x8 */ s32 mKey;   // the value fn_80215E58 searches for
    /* 0xC */ u8 mFlagC;  // read by fn_80215D38; must be set
    /* 0xD */ u8 pad_0xD[0x1];
    /* 0xE */ u8 mFlagE;  // read by fn_80215D38; must be clear
    /* 0xF */ u8 pad_0xF[0x1C - 0xF];
    /* 0x1C */ u8 pad_0x1C[0x24 - 0x1C]; // the stopwatch (cls_801ED3DC) starts here
    // Index of the first entry whose mKey is `key`, or -1 when none has it.
    s32 fn_80215E58(s32 key);
    // True when every entry has mFlagC set and mFlagE clear (true for an
    // empty table).
    bool fn_80215D38();
    // Calls fn_80215E18 on every entry's mId, in order.
    void fn_80215DB8();
    // Zeroes every entry's stopwatch and leaves the watches running.
    void fn_8021600C();
    // Stops entry `index`'s stopwatch with `fraction` of a frame and sets its
    // mKey to `key`.
    void fn_80215FB8(s32 index, s32 key, f32 fraction);
    // Entry `index`'s mKey (0-3) picks a value from a table; when that value
    // is positive, lbl_8053AED0 gets fn_8043D874(mId, value, 0).
    void fn_80215B6C(s32 index);
}; // Size: 0x24

// tu_80215A94 (0x80215E18, not written yet): called by
// cls_80215E48::fn_80215DB8 with each entry's &mId.
extern "C" {
void fn_80215E18(s32* p);
}

#endif
