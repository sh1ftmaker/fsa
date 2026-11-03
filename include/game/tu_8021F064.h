#ifndef GAME_TU_8021F064_H
#define GAME_TU_8021F064_H

// Exports of game/tu_8021F064.cpp: the list of navi (act_navi, unit
// 0x8021D734) actor serials kept in the guarded static at 0x804CE640.

#include "types.h"

// The guarded static at 0x804CE640 (function-local static of fn_8021F064, flag
// lbl_8053AC78; .bss, 0x120 bytes in the symbol because it covers the unit's
// global-object records too; the object itself is 0x20 bytes, the first
// global-object record sits at 0x20). A list of actor serials plus two flag
// bits. No vtable, ctor or dtor symbol (the constructor is inlined into
// fn_8021F064; fn_8021F27C is a separate clear); named after its lowest
// function.
struct cls_8021F064 {
    cls_8021F064() : mFlags(0), mCount(0) {}
    // True when the list is non-empty and fn_8021D794 holds for every listed
    // actor that still exists.
    bool fn_8021F094();
    // For every listed actor: bit 0 calls fn_8021D764, bit 1 calls fn_8021D734.
    // Then clears both bits.
    void fn_8021F130();
    // Removes the first entry equal to serial, shifting the rest down.
    void fn_8021F1F0(s32 serial);
    // Appends serial.
    void fn_8021F25C(s32 serial);
    // Clears the flags and the count.
    void fn_8021F27C();

    /* 0x00 */ u16 mFlags;
    /* 0x04 */ s32 mCount;
    /* 0x08 */ s32 mSerials[6];
}; // Size: 0x20

cls_8021F064* fn_8021F064();

#endif
