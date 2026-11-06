#ifndef GAME_TU_80215A94_H
#define GAME_TU_80215A94_H

// Exports of game/tu_80215A94.cpp: the 0x24-byte entry table that the
// horse-race man (game/act_horse_race_man.cpp) keeps at its 0x2B8 member and
// walks with fn_80215E48 and fn_80215E58.

#include "types.h"
#include "game/tu_801ED3DC.h"
#include "game/cls_8021C024.h"

// One entry of the table, one per player (fn_80216178 builds four with
// __construct_array, stride 0x24); named after that constructor, fn_80216238.
struct cls_80216238 {
    /* 0x00 */ s32 mId;    // the entry's index; passed to fn_8043D874
    /* 0x04 */ s32 mKey;   // -1 from the constructor; the value fn_80215E58 searches for
    /* 0x08 */ u8 mFlag8;  // read by fn_80215D38; must be set
    /* 0x09 */ u8 mFlag9;  // set by fn_80215D8C
    /* 0x0A */ u8 mFlagA;  // read by fn_80215D38; must be clear
    /* 0x0C */ u8 field_0xc[0xC]; // a cls_8021BFA8 (tu_8021BFA8.cpp), reset by fn_8021C00C
    /* 0x18 */ cls_801ED3DC mWatch;
    // 0x80215E18 (not written yet): fn_802162B8(mId, &mFlag8, mKey).
    void fn_80215E18();
}; // Size: 0x24

// The table the horse-race man keeps at 0x2B8. No vtable, ctor or dtor
// symbol; named after its lowest-address function that takes it as `this`.
struct cls_80215E48 {
    /* 0x00 */ s32 mCount; // fn_80138A30(): the number of players
    /* 0x04 */ cls_80216238 mEntries[4];
    /* 0x94 */ cls_8021C024 field_0x94; // a cls_8021C024 (tu_8021BFA8.cpp), reset by fn_8021C078
    /* 0xA0 */ u8 field_0xa0;      // zeroed by fn_80216178
    // Entry `index`'s mKey.
    s32 fn_80215E48(s32 index);
    // Once entry `index`'s stopwatch has stopped (and not at its 2 minute
    // limit), copies its seconds and centis into the save row and calls
    // fn_802D7844(1, mId) on the view.
    void fn_80215BCC(s32 index);
    // Sets entry `index`'s mFlag9 unless it or mFlag8 is already set.
    void fn_80215D8C(s32 index);
    // Index of the first entry whose mKey is `key`, or -1 when none has it.
    s32 fn_80215E58(s32 key);
    // True when every entry has mFlag8 set and mFlagA clear (true for an
    // empty table).
    bool fn_80215D38();
    // Calls fn_80215E18 on every entry, in order.
    void fn_80215DB8();
    // Number of entries whose mKey is not negative.
    s32 fn_80215E98();
    // Starts every entry's stopwatch with its mId and mKey.
    void fn_80215ECC();
    // True when some entry with a negative mKey has its stopwatch at the
    // 2 minute limit.
    bool fn_8021606C() const;
    // Zeroes every entry's stopwatch and leaves the watches running.
    void fn_8021600C();
    // Stops entry `index`'s stopwatch with `fraction` of a frame and sets its
    // mKey to `key`.
    void fn_80215FB8(s32 index, s32 key, f32 fraction);
    // Entry `index`'s mKey (0-3) picks a value from a table; when that value
    // is positive, lbl_8053AED0 gets fn_8043D874(mId, value, 0).
    void fn_80215B6C(s32 index);
    // When exactly one entry still has a negative mKey, the index of the
    // first such entry for which fn_801CD2F8(getPlayerLayer_8023DE58(i)) is
    // true; -1 otherwise.
    s32 fn_80215A94();
    // Reads entry `index`'s byte at 0x1C, its stopwatch (fn_801ED7B0) and the
    // save row's bytes at 0x78/0x79 (lbl_8050EC80); see the asm.
    bool fn_80215C6C(s32 index);
    // Unless gGameRoot's field_0x4c is 9 or 0xB, sets field_0xa0 to
    // fn_801CD664(10); then calls fn_801ED458(field_0xa0) on every entry's
    // mWatch.
    void fn_80215F30();
    // Resets every entry (mId = its index, mKey = -1, the three flag bytes
    // cleared, the 0x10 sub-object and mWatch reset), then field_0x94 and
    // field_0xa0.
    void fn_802160F4();
}; // Size: 0xA4

// tu_80215A94 (0x80216178, not written yet): the table's constructor, called by
// the horse-race man's constructor (game/act_horse_race_man.cpp) on its 0x2B8
// member. Stores the player count (fn_80138A30) as the entry count.
extern "C" {
void fn_80216178(cls_80215E48* table);
}

#endif
