#ifndef GAME_CLS_804B1378_H
#define GAME_CLS_804B1378_H

#include "types.h"

class J2DScreen;
class cls_804AF86C;

// Vtable 0x804B1378 (one slot, the destructor), 0x154 bytes, defined in
// game/tu_80414F6C.cpp. The battle stage select screen ("bat_stage_select.blo"):
// the constructor stores `this` in lbl_8053AE00; tu_8039003C allocates it
// with __nw__(0x154) when lbl_8053AAF8->field_0x7f is clear. fn_804170B0 runs
// the handler of state field_0x109 through a pointer-to-member table.
class cls_804B1378 {
public:
    typedef void (cls_804B1378::*StateFunc)();

    cls_804B1378(); // fn_8041787C
    /* 0x08 */ virtual ~cls_804B1378(); // fn_8041769C

    // State 2 (0x80414F78): steps field_0x10a: waits for lbl_8053AEC4's
    // fn_8042EC7C, then for button 0x1300, then fn_8042EBEC; then state 1.
    void fn_80414F78();
    void fn_80415028(); // state 1 (0x80415028)
    void fn_80416038(); // state 0 (0x80416038): the panes slide in
    // 0x80416BD0: resets the states, the panes' alphas and positions.
    void fn_80416BD0();
    // 0x804170B0: runs the handler of state field_0x109.
    void fn_804170B0();
    // 0x804170EC: blinks the cursors and draws the screen.
    void fn_804170EC();

    // Selects stage field_0xc * 5 + field_0x10: its name (message
    // lbl_8048D2B0[stage]) in both text panes and the 'Ndata04' cursor on its
    // 'Ndata' pane; past the 10 stages the random message 0x109. No
    // out-of-line copy: inlined into the constructor and fn_80415028.
    inline void setStageText();
    // Fade the panes of lbl_8048CD50, lbl_8048CDA8, lbl_8048CE00, lbl_8048CE48
    // and lbl_8048CE78 to rate (clamped to [0, 1]) times their initial
    // alphas. No out-of-line copies.
    inline void fadeCD50(f32 rate);
    inline void fadeCDA8(f32 rate);
    inline void fadeCE00(f32 rate);
    inline void fadeCE48(f32 rate);
    inline void fadeStage(int stage, f32 rate); // lbl_8048CE78[stage]

    /* 0x000 */ // vtable
    /* 0x004 */ f32 field_0x4; // 'Ndata05' bounds y
    /* 0x008 */ f32 field_0x8; // 'Pocur02' bounds y
    /* 0x00C */ s32 field_0xc;  // stage row
    /* 0x010 */ s32 field_0x10; // stage column
    /* 0x014 */ s32 field_0x14; // blink counter
    /* 0x018 */ s32 field_0x18;
    /* 0x01C */ s32 field_0x1c;
    /* 0x020 */ s32 field_0x20;
    /* 0x024 */ J2DScreen* mScreen;
    /* 0x028 */ cls_804AF86C* field_0x28[3]; // 'Pbsbk02', 'Pbsbk01', 'Pbsb09'
    /* 0x034 */ cls_804AF86C* field_0x34;    // 'Ndata03'
    /* 0x038 */ cls_804AF86C* field_0x38;    // 'Ndata02'
    /* 0x03C */ cls_804AF86C* field_0x3c[10]; // lbl_8048D198
    /* 0x064 */ u8 field_0x64[0x6C - 0x64];
    /* 0x06C */ cls_804AF86C* field_0x6c[10]; // lbl_8048D1E8
    /* 0x094 */ u8 field_0x94[0x9C - 0x94];
    /* 0x09C */ cls_804AF86C* field_0x9c;     // 'Ndata05'
    /* 0x0A0 */ cls_804AF86C* field_0xa0;     // lbl_8053AED8->field_0x8
    /* 0x0A4 */ cls_804AF86C* field_0xa4[15]; // lbl_8048D238
    /* 0x0E0 */ s32 field_0xe0; // fn_80414F6C returns it
    /* 0x0E4 */ char field_0xe4[0x102 - 0xE4]; // text buffer, length not known
    /* 0x102 */ s16 field_0x102; // frame counter of state 0
    /* 0x104 */ s16 field_0x104; // number of open stages
    /* 0x106 */ u16 field_0x106; // bit i: stage i is open
    /* 0x108 */ u8 field_0x108;
    /* 0x109 */ u8 field_0x109; // state
    /* 0x10A */ u8 field_0x10a; // sub-state of state 2
    /* 0x10B */ u8 field_0x10b[10]; // the open stages
    /* 0x115 */ u8 field_0x115[0x117 - 0x115];
    /* 0x117 */ u8 field_0x117[10]; // the open stages
    /* 0x121 */ u8 field_0x121[0x123 - 0x121];
    /* 0x123 */ u8 field_0x123[11]; // initial alphas of lbl_8048CD50's panes
    /* 0x12E */ u8 field_0x12e[11]; // ... of lbl_8048CDA8
    /* 0x139 */ u8 field_0x139[9];  // ... of lbl_8048CE00
    /* 0x142 */ u8 field_0x142[6];  // ... of lbl_8048CE48
    /* 0x148 */ u8 field_0x148[10]; // ... of lbl_8048CE78[0]
    /* 0x152 */ u8 field_0x152[0x154 - 0x152];
}; // Size: 0x154

// Data of game/tu_80401848.cpp.
extern cls_804B1378* lbl_8053AE00;

// 0x80414F6C (game/tu_80414F6C.cpp): lbl_8053AE00->field_0xe0.
s32 fn_80414F6C();

#endif
