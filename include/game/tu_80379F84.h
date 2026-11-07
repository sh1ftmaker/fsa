#ifndef GAME_TU_80379F84_H
#define GAME_TU_80379F84_H

#include "types.h"
#include "game/tu_803A8DFC.h"
#include "JSystem/JGeometry.h"

class JKRArchive;
class J2DScreen;
class J2DPicture;
class cls_804B20F8;
class ActorSoundObj_804578F4;
class JPABaseEmitter;
class PaneCtrl_804AF86C;

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
    // game/tu_803B7DCC.cpp; no other arguments.
    void fn_803B7DCC(); // draws the screen with its fade (unless stage 8 / sub 3)
    void fn_803B7E64(); // the same, first telling lbl_8053ADC8 to draw in modes 3 and 4
    void fn_803B8188(); // (re)starts the screen: clears the state and takes the stage from lbl_80529DEC
    void fn_803B7F6C(); // steps the state machine of field_0x294
    cls_804AFAB0(); // fn_803B8414

    /* 0x004 */ J2DPicture* field_0x4; // the screen's picture
    /* 0x008 */ u8 field_0x8[0x1C - 0x8];
    /* 0x01C */ cls_804B20F8* field_0x1c; // sound object
    /* 0x020 */ ActorSoundObj_804578F4* field_0x20;
    /* 0x024 */ JPABaseEmitter* field_0x24[50];
    /* 0x0EC */ JGeometry::TVec3f field_0xec; // the sound object's position
    /* 0x0F8 */ JGeometry::TVec3f field_0xf8[10];
    /* 0x170 */ s32 field_0x170[10]; // -1 after construction
    /* 0x198 */ s32 field_0x198;
    /* 0x19C */ s32 field_0x19c;
    /* 0x1A0 */ s32 field_0x1a0;
    /* 0x1A4 */ s32 field_0x1a4;
    /* 0x1A8 */ s32 field_0x1a8;
    /* 0x1AC */ s32 field_0x1ac;
    /* 0x1B0 */ u8 field_0x1b0[0x1B8 - 0x1B0];
    /* 0x1B8 */ s32 field_0x1b8;
    /* 0x1BC */ s32 field_0x1bc;
    /* 0x1C0 */ s32 field_0x1c0;
    /* 0x1C4 */ s32 field_0x1c4;
    /* 0x1C8 */ s32 field_0x1c8;
    /* 0x1CC */ s32 field_0x1cc;
    /* 0x1D0 */ s32 field_0x1d0;
    /* 0x1D4 */ s32 field_0x1d4;
    /* 0x1D8 */ s32 field_0x1d8;
    /* 0x1DC */ s16 field_0x1dc;
    /* 0x1DE */ u8 field_0x1de[0x280 - 0x1DE];
    /* 0x280 */ s32 field_0x280; // frame counter
    /* 0x284 */ s32 field_0x284; // stage (0-7)
    /* 0x288 */ s32 field_0x288; // sub-stage (0-3)
    /* 0x28C */ s32 field_0x28c; // -1 after construction
    /* 0x290 */ s32 field_0x290; // -1 after construction
    /* 0x294 */ u8 field_0x294; // mode, read by fn_8037A120
    /* 0x295 */ u8 field_0x295; // step within a mode
    /* 0x296 */ u8 field_0x296; // alpha of the screen
    /* 0x297 */ u8 field_0x297;
    /* 0x298 */ u8 field_0x298;
    /* 0x299 */ u8 field_0x299[0x29B - 0x299];
    /* 0x29B */ u8 field_0x29b;
    /* 0x29C */ u8 field_0x29c;
    /* 0x29D */ u8 field_0x29d;
    /* 0x29E */ u8 field_0x29e;
    /* 0x29F */ u8 field_0x29f;
}; // Size: at least 0x2A0

// Vtable 0x804B03D8; constructor fn_803D901C, destructor fn_803D8D74
// (game/tu_803D7098.cpp). The constructor stores the object in lbl_8053ADC0.
class cls_804B03D8 {
public:
    cls_804B03D8(); // fn_803D901C
    /* 0x08 */ virtual ~cls_804B03D8();
    // game/tu_803D7098.cpp, not written yet; no other arguments.
    void fn_803D82EC();
    void fn_803D8364();
    void fn_803D8670();
    void fn_803D87C0();
    // 0x803D8718: steps the fade of the screen: mode 6 fades the picture in (field_0x77 += 10), 5 out
    // (then mode 2); mode 2 runs fn_803D75B0.
    void fn_803D8718();
    void fn_803D75B0();

    /* 0x04 */ J2DScreen* field_0x4;
    /* 0x08 */ J2DScreen* field_0x8;
    /* 0x0C */ u8 field_0xc[0x1C - 0xC];
    /* 0x1C */ J2DPicture* field_0x1c; // the fade picture
    /* 0x20 */ J2DPicture* field_0x20; // the dark picture (alpha field_0x81)
    /* 0x24 */ PaneCtrl_804AF86C* field_0x24;
    /* 0x28 */ PaneCtrl_804AF86C* field_0x28;
    /* 0x2C */ PaneCtrl_804AF86C* field_0x2c;
    /* 0x30 */ u8 field_0x30[0x6C - 0x30];
    /* 0x6C */ s16 field_0x6c; // frame counter (0-59)
    /* 0x6E */ s16 field_0x6e; // frame of the bounce (0-10), 0 when idle
    /* 0x70 */ u8 field_0x70[0x76 - 0x70];
    /* 0x76 */ u8 field_0x76; // mode, read by fn_8037A120
    /* 0x77 */ u8 field_0x77; // the fade picture's alpha
    /* 0x78 */ u8 field_0x78;
    /* 0x79 */ u8 field_0x79;
    /* 0x7A */ u8 field_0x7a[0x7F - 0x7A];
    /* 0x7F */ u8 field_0x7f;
    /* 0x80 */ u8 field_0x80;
    /* 0x81 */ u8 field_0x81;
}; // Size: at least 0x82

// Vtable 0x804B0288; constructor fn_803D58E0, destructor fn_803D5710
// (game/tu_803D2C34.cpp). The constructor stores the object in lbl_8053ADB8.
class cls_804B0288 {
public:
    cls_804B0288(JKRArchive* archive, bool flag); // fn_803D58E0
    /* 0x08 */ virtual ~cls_804B0288();
    // game/tu_803D2C34.cpp, not written yet; no other arguments.
    void fn_803D5090();
    void fn_803D521C();
    void fn_803D5554();
    // The state handlers of game/tu_803D2C34.cpp (no other arguments), indexed by field_0x216c / 0x216d.
    void fn_803D2C34();
    void fn_803D2FFC();
    void fn_803D3060();
    void fn_803D3108();
    void fn_803D3180();
    void fn_803D335C();
    void fn_803D3460();
    void fn_803D35C4();
    void fn_803D3898();
    void fn_803D3928();
    void fn_803D35F0();
    void fn_803D373C();
    void fn_803D39C8();
    void fn_803D3B38();
    void fn_803D3C90();
    void fn_803D3D50();
    void fn_803D49A4();
    void fn_803D4DB0();

    /* 0x0004 */ J2DScreen* mScreen;
    /* 0x0008 */ J2DPicture* field_0x8; // the dark overlay (alpha 0x2173)
    /* 0x000C */ u8 field_0xc[0x14 - 0xC];
    /* 0x0014 */ PaneCtrl_804AF86C* field_0x14;
    /* 0x0018 */ PaneCtrl_804AF86C* field_0x18;
    /* 0x001C */ PaneCtrl_804AF86C* field_0x1c[2];
    /* 0x0024 */ PaneCtrl_804AF86C* field_0x24;
    /* 0x0028 */ PaneCtrl_804AF86C* field_0x28;
    /* 0x002C */ PaneCtrl_804AF86C* field_0x2c[8];
    /* 0x004C */ u8 field_0x4c[0x50 - 0x4C];
    /* 0x0050 */ u32 field_0x50;
    /* 0x0054 */ s32 field_0x54; // the message the screen shows
    /* 0x0058 */ u8 field_0x58[0x78 - 0x58];
    /* 0x0078 */ s32 field_0x78;
    /* 0x007C */ s32 field_0x7c; // countdown
    /* 0x0080 */ s16 field_0x80;
    /* 0x0082 */ s16 field_0x82;
    /* 0x0084 */ s16 field_0x84;
    /* 0x0086 */ s16 field_0x86;
    /* 0x0088 */ u8 field_0x88;
    /* 0x0089 */ u8 field_0x89[0x1B4 - 0x89];
    /* 0x01B4 */ u8 field_0x1b4;
    /* 0x01B5 */ u8 field_0x1b5[0x1C0 - 0x1B5];
    /* 0x01C0 */ u8 field_0x1c0[0x258]; // the three save files read from / written to the card
    /* 0x0418 */ u8 field_0x418[0x420 - 0x418];
    /* 0x0420 */ u8 field_0x420[0x1D4C]; // their bodies
    /* 0x216C */ u8 field_0x216c; // state, read by fn_8037AA28
    /* 0x216D */ u8 field_0x216d; // step within the state
    /* 0x216E */ u8 field_0x216e;
    /* 0x216F */ u8 field_0x216f;
    /* 0x2170 */ u8 field_0x2170; // the selected answer
    /* 0x2171 */ u8 field_0x2171;
    /* 0x2172 */ u8 field_0x2172;
    /* 0x2173 */ u8 field_0x2173;
    /* 0x2174 */ u8 field_0x2174;
    /* 0x2175 */ u8 field_0x2175;
    /* 0x2176 */ u8 field_0x2176; // the button-icon overlay (lbl_8053AED8) is drawn when set
    /* 0x2177 */ u8 field_0x2177;
}; // Size: at least 0x2178

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
    // 0x8037A174: the per-frame update: steps the active screen (the result screen's mode 5 asks fn_8037AA28
    // for the next stage: 0x1A moves on, 0x19 goes back).
    void fn_8037A174();
    void fn_8037A580();
    void fn_8037A664();
    void fn_8037A708();

    /* 0x04 */ ResultScreen_804AF9A0* field_0x4;
    /* 0x08 */ cls_804AFAB0* field_0x8;
    /* 0x0C */ cls_804B03D8* field_0xc;
    /* 0x10 */ cls_804B0288* field_0x10; // the fourth screen (always made)
    /* 0x14 */ JKRArchive* field_0x14; // lbl_80529DEC's result archive; its resources are dropped by the destructor
    /* 0x18 */ JKRArchive* field_0x18; // the result screen's .blo/.btk archive (ResultScreen_804AF9A0's constructor)
    /* 0x1C */ u8 field_0x1c; // cleared by fn_8037A708
    /* 0x1D */ u8 field_0x1d; // set by ResultScreen_804AF9A0::fn_803A9830 when its fade-out ends
    /* 0x1E */ u8 field_0x1e;
}; // Size: at least 0x1F

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
