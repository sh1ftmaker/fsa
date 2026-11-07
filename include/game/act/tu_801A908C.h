#ifndef GAME_ACT_TU_801A908C_H
#define GAME_ACT_TU_801A908C_H

// Code: game/tu_801A908C.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A2FB0 (still lbl_804A2FB0 in symbols.txt;
// its constructor is inlined in game/tu_801F9484.cpp at 0x80200D18).
class JPABaseEmitter;

class cls_804A2FB0 : public ActorBase_804A5770 {
public:
    // 0x801AA464 (deleting form): frees the emitter at 0x2B8; in state 0 and when the mode
    // word of GameRoot is not 9 calls GameRoot::fn_80138B7C; in state 5 counts lbl_8053ABD0 down.
    /* 0x08 */ virtual ~cls_804A2FB0();
    /* 0x14 */ virtual void update(); // 0x801A961C
    /* 0x20 */ virtual void draw(); // 0x801A908C
    // The two sprite draws of states 0 and 1-4: the 'TIMG' picture
    // lbl_804A2F2C[32] / [field_0x24c] rotated by field_0x2b4 about y.
    void fn_801A90D4();
    void fn_801A9374();
    // 0x801A96B4 / 0x801A9CF4: the per-state steps update() calls (plain symbols, not written).
    void fn_801A96B4();
    void fn_801A9CF4();
    // 0x801AA8F4 (a plain `fn_801AA8F4` symbol): steps the frame field_0x240 by the mode field_0x260.
    void fn_801AA8F4();

    /* 0x230 */ u8 field_0x230[0x234 - 0x230]; // not yet known
    /* 0x234 */ s32 field_0x234; // countdown (update)
    /* 0x238 */ s32 field_0x238; // countdown (update)
    /* 0x23C */ s32 field_0x23c; // countdown (update)
    /* 0x240 */ s32 field_0x240; // fn_801AA8F4's frame step (0..3)
    /* 0x244 */ u8 field_0x244[0x24C - 0x244]; // not yet known
    /* 0x24C */ s32 field_0x24c; // index into lbl_804A2F2C
    /* 0x250 */ u8 field_0x250[0x258 - 0x250]; // not yet known
    /* 0x258 */ s32 field_0x258; // the state draw switches on
    /* 0x25C */ s32 field_0x25c; // base of field_0x24c
    /* 0x260 */ s32 field_0x260; // fn_801AA8F4's mode (0-4)
    /* 0x264 */ u8 field_0x264[0x280 - 0x264];
    /* 0x280 */ JGeometry::TVec3f field_0x280[4];
    /* 0x2B0 */ u8 field_0x2b0; // set by fn_801AA8F4 when the step runs out
    /* 0x2B1 */ u8 field_0x2b1[0x3];
    /* 0x2B4 */ f32 field_0x2b4; // an angle in s16 units (draw divides it by -10430.228f)
    /* 0x2B8 */ JPABaseEmitter* field_0x2b8;
}; // Size: 0x2BC

// The 33 'TIMG' picture names, "e_tst_paku_green_0.bti" first (.data).
extern const char* lbl_804A2F2C[33];

#endif
