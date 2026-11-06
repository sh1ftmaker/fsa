#ifndef GAME_TU_803B8C74_H
#define GAME_TU_803B8C74_H

#include "types.h"

// The save data at lbl_8050EC80 (game/tu_803B8C74.cpp): a 0x20-byte header
// (card serial, current file), three save files and their bodies.

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

    /* 0x00 */ s64 field_0x0; // OSGetTime() when chosen last; 0 for an empty file (tu_803BC6F0)
    // Bit stage * 4 + sub (stage 0-7, sub 0-3): set by fn_80379FF4 and
    // fn_8019CDE4, tested by isStageFlagSet (game/StageFlag.h) and fn_803B8CAC.
    /* 0x08 */ s32 mFlags;
    // Added to by tu_801DE714 (__dt__12cls_804A51E4Fv, the TINK actor's
    // destructor) and clamped at 0 from below.
    /* 0x0C */ s32 field_0xc;
    /* 0x10 */ u8 field_0x10[0x14 - 0x10];
    /* 0x14 */ u16 field_0x14; // fn_803A3034 (game/tu_803944C8.cpp) returns it
    /* 0x16 */ u16 field_0x16; // fn_803A3018 returns it
    /* 0x18 */ u8 field_0x18[0x58 - 0x18];
    /* 0x58 */ u8 field_0x58; // written by fn_80215BCC (the stopwatch's centis)
    /* 0x59 */ u8 field_0x59; // written by fn_80215BCC (the stopwatch's seconds)
    /* 0x5A */ u8 field_0x5a[0x5C - 0x5A]; // byte 0: bit tests in cls_804B1288::fn_804045F8
    // GameView_804A9950::fn_802D7440 (game/tu_802D39B8.cpp): field_0x5c while
    // GameRoot's field_0x7f is set, else field_0x5d; 1 lets the time limit run.
    /* 0x5C */ u8 field_0x5c;
    /* 0x5D */ u8 field_0x5d;
    // Read by Player::fn_802710C8 (tu_8026FC20): 0x5E while GameRoot's field_0x7f is set,
    // else 0x5F; a value of 1 there calls fn_8024E500 on a hit of 4 or more.
    /* 0x5E */ u8 field_0x5e;
    /* 0x5F */ u8 field_0x5f;
    /* 0x60 */ u8 field_0x60;
    /* 0x61 */ u8 field_0x61; // 1 when the battle stage chosen last is 5 or more (tu_80414F6C fn_80415028)
    /* 0x62 */ u8 field_0x62[0x65 - 0x62];
    /* 0x65 */ u8 field_0x65; // bit 0 is tested (tu_803E1668 enterState15)
    /* 0x66 */ u8 field_0x66; // fn_803B8C74 / fn_803B8C90
    /* 0x67 */ u8 field_0x67; // bit 0: act_fairy's init has shown message 0x339
    // Tested by act_rank_display's update before it shows a rank change:
    // field_0x68 when GameRoot's field_0x7f is 1, field_0x69 when it is 0.
    /* 0x68 */ u8 field_0x68;
    /* 0x69 */ u8 field_0x69;
    /* 0x6A */ u8 field_0x6a[0xC0 - 0x6A];
    // Checksum of bytes 0x00-0xBF: their sum, then the sum of their
    // complements (fn_803BADAC compares both).
    /* 0xC0 */ u32 field_0xc0;
    /* 0xC4 */ u32 field_0xc4;
}; // Size: 0xC8

// The 0x9C4-byte block that goes with each save file (cleared only).
struct SaveFileBody_803B950C {
    /* 0x000 */ u8 field_0x0[0x9C4];
}; // Size: 0x9C4

// Stores `value` in the current save file's byte 0x66 (callers pass
// lbl_80529DEC's byte 0x24F, which fn_803B8C90 reads back).
void fn_803B8C74(u8 value);

// Owner: game/tu_803BACF8.cpp (0x803BADAC). True when `file`'s checksum words
// (0xC0 / 0xC4) match its first 0xC0 bytes (cntlzw / srwi).
bool fn_803BADAC(SaveFile_803B950C* file);
// Owner: game/tu_803BACF8.cpp (0x803BACF8). Writes files[file]'s checksum
// words: 0xC0 the byte sum of its first 0xC0 bytes, 0xC4 the sum of their
// complements (the index is clrlwi'd: u8).
void fn_803BACF8(SaveFile_803B950C* files, u8 file);

// fn_803B950C: clears a file and its block and sets the file's defaults.
void fn_803B950C(SaveFile_803B950C* file, SaveFileBody_803B950C* body);

// FAKEMATCH(crack-pass): docs/CRACK_PASS.md "Save-data layout". Save file i seen
// 0x20 bytes early, as a 0xC8-byte row at lbl_8050EC80 + i * 0xC8 (row offset N is
// SaveFile_803B950C offset N - 0x20). Only tu_801DE714's destructor and act_fairy's
// init use it: they keep that row address in a register across a read and a write of
// one field, which no access through mFiles reproduces (an extra `addi 0x20`).
struct SaveFileRow_CrackPass {
    /* 0x00 */ u8 field_0x0[0x2C];
    /* 0x2C */ s32 field_0x2c; // SaveFile_803B950C::field_0xc
    /* 0x30 */ u8 field_0x30[0x87 - 0x30];
    /* 0x87 */ u8 field_0x87; // SaveFile_803B950C::field_0x67
    /* 0x88 */ u8 field_0x88[0xC8 - 0x88];
}; // Size: 0xC8

// The save data object lbl_8050EC80 (vtable 0x804AFB30, destructor fn_803B94C4).
class cls_804AFB30 {
public:
    // Inlined into this unit's __sinit (0x803B961C): clears the header words and
    // gives each of the three files and bodies fn_803B950C's defaults.
    cls_804AFB30() {
        mCurrentFile = 0;
        field_0x8 = 0;
        for (int i = 0; i < 3; i++) {
            fn_803B950C(&mFiles[i], &mBodies[i]);
        }
    }
    /* 0x08 */ virtual ~cls_804AFB30();
    s32 fn_803B933C(SaveFile_803B950C* files, s32 index);
    s32 fn_803B93E8(SaveFile_803B950C* files, SaveFileBody_803B950C* bodies, s32 index);
    // Every reader goes through these: with them the save-data tests of
    // fn_803B8CAC / fn_803B8EBC / fn_803B90E4 (here) and fn_8019CE40
    // (tu_80197070) match; indexing mFiles directly swaps the add's operands.
    SaveFile_803B950C& getFile(s32 file) { return mFiles[file]; }
    SaveFile_803B950C& getCurrentFile() { return mFiles[mCurrentFile]; }
    // FAKEMATCH(crack-pass): the current file as SaveFileRow_CrackPass (above).
    SaveFileRow_CrackPass& getCurrentRow_CrackPass() {
        return reinterpret_cast<SaveFileRow_CrackPass*>(this)[mCurrentFile];
    }

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

// The object lbl_80510C6C (vtable 0x804AFB3C; constructor
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
