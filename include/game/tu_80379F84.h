#ifndef GAME_TU_80379F84_H
#define GAME_TU_80379F84_H

#include "types.h"
#include "game/tu_803A8DFC.h"

class JKRArchive;

// The four screen objects whose constructors store them in .sbss globals
// (lbl_8053ADA0, lbl_8053ADA8, lbl_8053ADC0, lbl_8053ADB8). Each is named after
// its vtable (slot 0x08 is the destructor); their units are not written yet,
// so only what game/tu_80379F84.cpp uses is declared here.

// ResultScreen_804AF9A0 (vtable 0x804AF9A0) is declared in game/tu_803A8DFC.h.

// Vtable 0x804AFAB0; constructor fn_803B8414, destructor fn_803B82FC
// (game/tu_803B7DCC.cpp). The constructor stores the object in lbl_8053ADA8.
class cls_804AFAB0 {
public:
    /* 0x08 */ virtual ~cls_804AFAB0();
    // game/tu_803B7DCC.cpp, not written yet; no other arguments.
    void fn_803B7DCC();
    void fn_803B7E64();
    void fn_803B8188();

    /* 0x004 */ u8 field_0x4[0x294 - 0x4];
    /* 0x294 */ u8 field_0x294; // mode, read by fn_8037A120
}; // Size: at least 0x295

// Vtable 0x804B03D8; constructor fn_803D901C, destructor fn_803D8D74
// (game/tu_803D7098.cpp). The constructor stores the object in lbl_8053ADC0.
class cls_804B03D8 {
public:
    /* 0x08 */ virtual ~cls_804B03D8();
    // game/tu_803D7098.cpp, not written yet; no other arguments.
    void fn_803D82EC();
    void fn_803D8364();
    void fn_803D8670();
    void fn_803D87C0();

    /* 0x04 */ u8 field_0x4[0x76 - 0x4];
    /* 0x76 */ u8 field_0x76; // mode, read by fn_8037A120
}; // Size: at least 0x77

// Vtable 0x804B0288; constructor fn_803D58E0, destructor fn_803D5710
// (game/tu_803D2C34.cpp). The constructor stores the object in lbl_8053ADB8.
class cls_804B0288 {
public:
    /* 0x08 */ virtual ~cls_804B0288();
    // game/tu_803D2C34.cpp, not written yet; no other arguments.
    void fn_803D5090();
    void fn_803D521C();
    void fn_803D5554();

    /* 0x0004 */ u8 field_0x4[0x216C - 0x4];
    /* 0x216C */ u8 field_0x216c; // read by fn_8037AA28
}; // Size: at least 0x216D

// The object lbl_8053AD68 holds (declared in GameGlobals.h, which forward
// declares it). Named after its vtable 0x804AE9A0 (.data, this unit):
// fn_8037A8D4 is its constructor and stores the object in lbl_8053AD68;
// fn_8037A768 is its destructor. Its three pointers are the screens above
// (fn_8037A708 calls their members with them; fn_80379F84, fn_8037A580 and
// fn_8037A664 test them and use the screens' globals).
struct cls_804AE9A0 {
    // fn_8037A8D4; GameRoot's fn_80135020 (tu_80134C04) makes it with __nw__(0x20).
    cls_804AE9A0();
    /* 0x08 */ virtual ~cls_804AE9A0();
    bool fn_80379F84();
    void fn_8037A580();
    void fn_8037A664();
    void fn_8037A708();

    /* 0x04 */ ResultScreen_804AF9A0* field_0x4;
    /* 0x08 */ cls_804AFAB0* field_0x8;
    /* 0x0C */ cls_804B03D8* field_0xc;
    /* 0x10 */ u8 field_0x10[0x18 - 0x10];
    /* 0x18 */ JKRArchive* field_0x18; // the result screen's .blo/.btk archive (ResultScreen_804AF9A0's constructor)
    /* 0x1C */ u8 field_0x1c; // cleared by fn_8037A708
    /* 0x1D */ u8 field_0x1d; // set by ResultScreen_804AF9A0::fn_803A9830 when its fade-out ends
}; // Size: at least 0x1E

// Owner: game/tu_80197070.cpp (0x8019CDE4 / 0x8019CE40). Set / test bit
// stage * 4 + sub of the current save file's mFlags (game/tu_803B8C74.h; stage 0-7, sub 0-3;
// anything else is ignored / false).
void fn_8019CDE4(s32 stage, s32 sub);
BOOL fn_8019CE40(s32 stage, s32 sub);

// .sbss 0x8053ADB8, 0x8053ADA0, 0x8053ADA8, 0x8053ADC0 (auto split, 8 bytes
// each; stored by the constructors above, in units not written yet).
extern cls_804B0288* lbl_8053ADB8;
extern cls_804AFAB0* lbl_8053ADA8;
extern cls_804B03D8* lbl_8053ADC0;

extern "C" {
// Owner: game/tu_80379F84.cpp.
u8 fn_8037AA28(void);
void fn_8037AA34(void);
void fn_8037AA58(void);
void fn_8037AA7C(void);
u8 fn_8037A120(void);
// Steps lbl_80529DEC's stage (0x118) and sub-stage (0x11C) and sets the flag bit
// of the current save file.
void fn_80379FF4(void);
}

#endif
