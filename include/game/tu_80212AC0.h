#ifndef GAME_TU_80212AC0_H
#define GAME_TU_80212AC0_H

// Exports of game/tu_80212AC0.cpp. Called by the Pengator actors
// (act_pengator, tu_80182914, tu_80183544: fn_80212AF0 on their mPos at 0xC),
// tu_801DE714 and tu_802D39B8 (which clears the count).

#include "types.h"
#include "JSystem/JGeometry.h"

// The guarded static at 0x804CCDB8 (function-local static of fn_80212AC0, flag
// lbl_8053AC70; .bss, 0x180 bytes in the symbol because it covers the unit's
// global-object records too). A list of up to 0x20 actor serials. No vtable,
// ctor or dtor symbol (the constructor is inlined into fn_80212AC0); named
// after its first function.
struct cls_80212AC0 {
    cls_80212AC0() : mCount(0) {}
    // Looks the tracked actors up in order; for the first one whose area
    // contains *pos, pushes *pos out by the vector fn_80210E90 returns.
    void fn_80212AF0(JGeometry::TVec3f* pos);
    // Removes the first entry equal to serial, shifting the rest down.
    void fn_80212BA0(s32 serial);
    // Appends serial.
    void fn_80212BFC(s32 serial);

    /* 0x00 */ s32 mSerials[0x20];
    /* 0x80 */ s32 mCount;
}; // Size: 0x84

cls_80212AC0* fn_80212AC0();

#endif
