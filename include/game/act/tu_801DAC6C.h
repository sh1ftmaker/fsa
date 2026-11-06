#ifndef GAME_ACT_TU_801DAC6C_H
#define GAME_ACT_TU_801DAC6C_H

#include "types.h"
#include "JSystem/JGeometry.h"

// A list of up to 30 actor serials plus two "watched" serials, the guarded
// static in .bss at 0x804C92D0 (constructed on first use by fn_801DAC6C,
// guard lbl_8053AC28). No vtable, constructor or destructor symbol: named
// after its lowest-address function, the getter fn_801DAC6C (provisional).
// Size: 0x88
struct Singleton_801DAC6C {
    Singleton_801DAC6C() : mSerial0(-1), mSerial4(-1), mCount(0), field_0x84(0) {}

    // 0x801DACAC: zeroes *out, then copies the position of the actor with
    // serial mSerial0 into it. Returns false only when mSerial0 < 0.
    bool fn_801DACAC(JGeometry::TVec3f* out) const;
    // 0x801DAD2C: fn_801DC264 of the actor with serial mSerial4, or 0.
    u8 fn_801DAD2C() const;
    // 0x801DAD88: calls fn_801D8400 on the actor with serial mSerial0.
    void fn_801DAD88();
    // 0x801DADD0: sends sound 0x40005 (startSound_801F0E34) to the actor with serial mSerial0.
    void fn_801DADD0();
    // 0x801DAE24: for each listed actor sets field_0x10c to -1 and calls vfunc_0x24.
    void fn_801DAE24() const;
    // 0x801DAEAC: removes serial from the list, closing the gap.
    void fn_801DAEAC(s32 serial);
    // 0x801DAF14: appends serial (the list holds 30).
    void fn_801DAF14(s32 serial);
    // 0x801DAF3C / 0x801DAF44: set the two serials.
    void fn_801DAF3C(s32 serial);
    void fn_801DAF44(s32 serial);
    // 0x801DAF4C: resets to the constructed state.
    void fn_801DAF4C();

    /* 0x00 */ s32 mSerial0;
    /* 0x04 */ s32 mSerial4;
    /* 0x08 */ s32 mList[30];
    /* 0x80 */ s32 mCount;
    /* 0x84 */ u16 field_0x84; // bit 0: set by the tail on its tenth hit (act_frostare_tail lhz/ori/sth)
};

// Owner game/tu_801DAC6C.cpp: a byte counter (0-4) other units step with lbz/stb;
// GameView_804A9950::fn_802D8328 clears it.
extern u8 lbl_8053AC30;

// Owner game/tu_801DAC6C.cpp. Returns the guarded static above.
Singleton_801DAC6C* fn_801DAC6C();

#endif
