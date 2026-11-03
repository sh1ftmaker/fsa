#ifndef GAME_TU_80379F84_H
#define GAME_TU_80379F84_H

#include "types.h"

// The object lbl_8053ADB8 points to (.sbss, in dtk's auto split). Only the byte
// at 0x216C is read in this unit (fn_8037AA28). Named after that getter, the
// first function seen reading it; its real layout is unknown.
struct Unk_8037AA28 {
    /* 0x0000 */ u8 field_0x0[0x216C];
    /* 0x216C */ u8 field_0x216c;
}; // Size: at least 0x216D

// The object lbl_8053AD68 holds (declared in GameGlobals.h, which forward
// declares it). Its pointers at 0x4, 0x8 and 0xC are tested against NULL
// (fn_80379F84, fn_8037A120); only the test is seen, so they stay void*.
// Named after its vtable 0x804AE9A0 (.data, this unit): fn_8037A8D4 is its
// constructor and stores the object in lbl_8053AD68; fn_8037A768 is its
// destructor. Only the fields the functions here touch are named.
struct cls_804AE9A0 {
    /* 0x00 */ u8 field_0x0[0x4];
    /* 0x04 */ void* field_0x4;
    /* 0x08 */ void* field_0x8;
    /* 0x0C */ void* field_0xc;
    /* 0x10 */ u8 field_0x10[0x1C - 0x10];
    /* 0x1C */ u8 field_0x1c; // cleared by fn_8037A708
}; // Size: at least 0x1D

// The object lbl_8053ADA0 points to (.sbss, auto split). fn_80379F84 reads
// the byte at 0x4C0. Named after the first function reading it (fn_80379F84),
// with its global as suffix, since fn_80379F84 is also the first function to
// read cls_804AE9A0 and the plain name would clash with it.
struct Unk_80379F84_ADA0 {
    /* 0x0000 */ u8 field_0x0[0x4C0];
    /* 0x04C0 */ u8 field_0x4c0;
}; // Size: at least 0x4C1

// The object lbl_8053ADA8 points to (.sbss, auto split). fn_8037A120 reads the
// byte at 0x294; first reader fn_8037A120, the suffix as above.
struct Unk_8037A120_ADA8 {
    /* 0x000 */ u8 field_0x0[0x294];
    /* 0x294 */ u8 field_0x294;
}; // Size: at least 0x295

// The object lbl_8053ADC0 points to (.sbss, auto split). fn_8037A120 reads the
// byte at 0x76; first reader fn_8037A120, the suffix as above.
struct Unk_8037A120_ADC0 {
    /* 0x00 */ u8 field_0x0[0x76];
    /* 0x76 */ u8 field_0x76;
}; // Size: at least 0x77

// One row of the save table lbl_8050EC80 (.bss, 0x21E0 bytes; GameState.h and
// GameView.h describe it; fn_80403514 in game/tu_803BC6F0.cpp picks rows). The
// word at 0x10 of row 0 is the index of the current row.
struct SaveRow_8050EC80 {
    /* 0x00 */ u8 field_0x0[0x10];
    /* 0x10 */ s32 field_0x10;
    /* 0x14 */ u8 field_0x14[0x28 - 0x14];
    // Bit stage * 4 + sub set by fn_80379FF4.
    /* 0x28 */ s32 field_0x28;
    /* 0x2C */ u8 field_0x2c[0xC8 - 0x2C];
}; // Size: 0xC8

extern SaveRow_8050EC80 lbl_8050EC80[];

// .sbss 0x8053ADB8, 0x8053ADA0, 0x8053ADA8, 0x8053ADC0 (auto split, 8 bytes
// each; the globals are stored by units not written yet).
extern Unk_8037AA28* lbl_8053ADB8;
extern Unk_80379F84_ADA0* lbl_8053ADA0;
extern Unk_8037A120_ADA8* lbl_8053ADA8;
extern Unk_8037A120_ADC0* lbl_8053ADC0;

extern "C" {
// Owner: game/tu_80379F84.cpp.
u8 fn_8037AA28(void);
void fn_8037AA34(void);
void fn_8037AA58(void);
void fn_8037AA7C(void);
u8 fn_8037A120(void);
// Takes the object lbl_8053AD68 holds (tu_80301CD0 passes the global).
bool fn_80379F84(cls_804AE9A0* obj);
// Steps lbl_80529DEC's stage (0x118) and sub-stage (0x11C) and sets the flag bit
// of the current save row.
void fn_80379FF4(void);
// Takes the object lbl_8053AD68 holds (game/tu_80134C04.cpp passes the global).
void fn_8037A580(cls_804AE9A0* obj);
void fn_8037A664(cls_804AE9A0* obj);
// fn_8037A708 (GameGlobals.h) takes the same object. The three below are not
// written yet (their unit is unknown); each takes one pointer field of
// lbl_8053AD68's object in r3, as fn_8037A708 passes it.
void fn_803B6768(void* obj);
void fn_803B8188(void* obj);
void fn_803D87C0(void* obj);
// fn_8037A580 / fn_8037A664: the object each one is called with (lbl_8053ADA0,
// lbl_8053ADA8 or lbl_8053ADC0 in r3, as loaded for the test before the call).
void fn_803A8F5C(Unk_80379F84_ADA0* obj);
void fn_803B31AC(Unk_80379F84_ADA0* obj);
void fn_803A97A0(Unk_80379F84_ADA0* obj);
void fn_803B7DCC(Unk_8037A120_ADA8* obj);
void fn_803B7E64(Unk_8037A120_ADA8* obj);
void fn_803D82EC(Unk_8037A120_ADC0* obj);
void fn_803D8364(Unk_8037A120_ADC0* obj);
void fn_803D8670(Unk_8037A120_ADC0* obj);

// Not written yet (their unit is unknown); each takes the object lbl_8053ADB8
// holds, in r3, and returns nothing this unit reads.
void fn_803D5090(Unk_8037AA28* obj);
void fn_803D5554(Unk_8037AA28* obj);
void fn_803D521C(Unk_8037AA28* obj);
}

#endif
