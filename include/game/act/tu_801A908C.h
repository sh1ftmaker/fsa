#ifndef GAME_ACT_TU_801A908C_H
#define GAME_ACT_TU_801A908C_H

// Code: game/tu_801A908C.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A2FB0 (still lbl_804A2FB0 in symbols.txt;
// its constructor is inlined in game/tu_801F9484.cpp at 0x80200D18).
class cls_804A2FB0 : public ActorBase_804A5770 {
public:
    /* 0x20 */ virtual void draw(); // 0x801A908C
    // The two sprite draws of states 0 and 1-4: the 'TIMG' picture
    // lbl_804A2F2C[32] / [field_0x24c] rotated by field_0x2b4 about y.
    void fn_801A90D4();
    void fn_801A9374();

    /* 0x230 */ u8 field_0x230[0x24C - 0x230]; // not yet known
    /* 0x24C */ s32 field_0x24c; // index into lbl_804A2F2C
    /* 0x250 */ u8 field_0x250[0x258 - 0x250]; // not yet known
    /* 0x258 */ s32 field_0x258; // the state draw switches on
    /* 0x25C */ u8 field_0x25c[0x2B4 - 0x25C]; // not yet known
    /* 0x2B4 */ f32 field_0x2b4; // an angle in s16 units (draw divides it by -10430.228f)
}; // Size: not yet known (at least 0x2BC: the destructor reads 0x2B8)

// The 33 'TIMG' picture names, "e_tst_paku_green_0.bti" first (.data).
extern const char* lbl_804A2F2C[33];

#endif
