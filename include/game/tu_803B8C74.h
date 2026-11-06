#ifndef GAME_TU_803B8C74_H
#define GAME_TU_803B8C74_H

#include "types.h"

// The save data at lbl_8050EC80 (game/tu_803B8C74.cpp), seen as its real
// layout. Other units still read it through the SaveRow_8050EC80[] view of
// game/tu_80379F84.h (row `i` there is file `i` here shifted by 0x20: file
// offset N is row offset N + 0x20).

// One save file (0xC8 bytes); three of them follow the 0x20-byte header.
// Named after fn_803B950C, the first function that fills one.
struct SaveFile_803B950C {
    // Sets the defaults every cleared file gets: fn_803B950C, and the copies
    // fn_803B933C / fn_803B93E8 make on the stack, inline it after their memsets.
    void setDefaults() {
        field_0x5c = 1;
        field_0x59 = 50;
        for (int stage = 0; stage < 7; stage++) {
            mFlags |= 1 << (stage * 4 + 2);
        }
        mFlags |= 1 << (7 * 4 + 1);
        field_0x68 = 1;
        field_0x69 = 1;
    }

    /* 0x00 */ s64 field_0x0; // OSGetTime() when chosen last (tu_803BC6F0)
    // Bit stage * 4 + sub (stage 0-7, sub 0-3), tested by fn_803B8CAC.
    /* 0x08 */ u32 mFlags;
    /* 0x0C */ u8 field_0xc[0x59 - 0xC];
    /* 0x59 */ u8 field_0x59;
    /* 0x5A */ u8 field_0x5a[0x5C - 0x5A];
    /* 0x5C */ u8 field_0x5c;
    /* 0x5D */ u8 field_0x5d[0x66 - 0x5D];
    /* 0x66 */ u8 field_0x66; // fn_803B8C74 / fn_803B8C90
    /* 0x67 */ u8 field_0x67;
    /* 0x68 */ u8 field_0x68;
    /* 0x69 */ u8 field_0x69;
    /* 0x6A */ u8 field_0x6a[0xC8 - 0x6A];
}; // Size: 0xC8

// The 0x9C4-byte block that goes with each save file (cleared only).
struct SaveFileBody_803B950C {
    /* 0x000 */ u8 field_0x0[0x9C4];
}; // Size: 0x9C4

// Stores `value` in the current save file's byte 0x66 (callers pass
// lbl_80529DEC's byte 0x24F, which fn_803B8C90 reads back).
void fn_803B8C74(u8 value);

// fn_803B950C: clears a file and its block and sets the file's defaults.
void fn_803B950C(SaveFile_803B950C* file, SaveFileBody_803B950C* body);

// The save data object lbl_8050EC80 (vtable 0x804AFB30, destructor fn_803B94C4).
class cls_804AFB30 {
public:
    /* 0x08 */ virtual ~cls_804AFB30();
    s32 fn_803B933C(SaveFile_803B950C* files, s32 index);
    s32 fn_803B93E8(SaveFile_803B950C* files, SaveFileBody_803B950C* bodies, s32 index);

    /* 0x00 vtable */
    /* 0x04 */ u8 field_0x4[0x8 - 0x4];
    /* 0x08 */ u64 field_0x8; // memory card serial number (tu_803BC6F0)
    /* 0x10 */ s32 mCurrentFile;
    /* 0x14 */ u8 field_0x14[0x20 - 0x14];
    /* 0x20 */ SaveFile_803B950C mFiles[3];
    /* 0x278 */ u8 field_0x278[0x280 - 0x278];
    /* 0x280 */ SaveFileBody_803B950C mBodies[3];
    /* 0x1FCC */ u8 field_0x1fcc[0x1FE0 - 0x1FCC];
}; // Size: 0x1FE0

extern cls_804AFB30 lbl_8050EC80;

// Half of cls_804AFB3C (0x45 bytes); the object holds two, 0x90 apart, and its
// constructor clears both in the same loops.
struct cls_804AFB3C_Half {
    /* 0x00 */ u8 field_0x0[16];
    /* 0x10 */ u8 field_0x10[0x20 - 0x10];
    /* 0x20 */ u8 field_0x20[8];
    /* 0x28 */ u8 field_0x28;
    /* 0x29 */ u8 field_0x29[8];
    /* 0x31 */ u8 field_0x31[8];
    /* 0x39 */ u8 field_0x39[8];
    /* 0x41 */ u8 field_0x41;
    /* 0x42 */ u8 field_0x42;
    /* 0x43 */ u8 field_0x43;
    /* 0x44 */ u8 field_0x44;
}; // Size: 0x45

// The object at lbl_8050EC80 + 0x1FEC (vtable 0x804AFB3C; constructor
// fn_803B9160, destructor fn_803B95D4), constructed by this unit's __sinit.
class cls_804AFB3C {
public:
    cls_804AFB3C();
    /* 0x08 */ virtual ~cls_804AFB3C();

    /* 0x00 vtable */
    /* 0x04 */ cls_804AFB3C_Half field_0x4;
    /* 0x4C */ s32 field_0x4c[8];
    /* 0x6C */ s32 field_0x6c[8];
    /* 0x8C */ u32 field_0x8c;
    /* 0x90 */ u32 field_0x90;
    /* 0x94 */ cls_804AFB3C_Half field_0x94;
}; // Size: 0xDC

#endif
