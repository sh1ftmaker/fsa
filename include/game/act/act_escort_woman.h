#ifndef GAME_ACT_ACT_ESCORT_WOMAN_H
#define GAME_ACT_ACT_ESCORT_WOMAN_H

// Code: game/act_escort_woman.cpp (not written yet). Only the slots other
// units call are declared.

#include "game/tu_801213EC.h"

// The escorted woman ('OPGL', actors.tsv). "EscortWoman_8049E1D0" is a
// provisional name carrying its vtable address; the constructor is
// __ct__20EscortWoman_8049E1D0Fv. It calls the base constructor (tu_801213EC), so its parent is
// EscortBase_8049E0C8.
class EscortWoman_8049E1D0 : public EscortBase_8049E0C8 {
public:
    // __ct__20EscortWoman_8049E1D0Fv: passes "enemy_gc_sob.bin", 0x2E6 and 1 to the base constructor,
    // then clears field_0x3c4, field_0x3c8 and field_0x3cc.
    EscortWoman_8049E1D0();
    /* 0x08 */ virtual ~EscortWoman_8049E1D0();
    // fn_80125858: the vtable's init slot (ActorBase's 0x0C); calls the base's fn_80124448.
    /* 0x0C */ virtual void init();
    /* 0x54 */ virtual bool vfunc_0x54();
    /* 0x58 */ virtual bool vfunc_0x58();
    /* 0x5C */ virtual s32 vfunc_0x5C();
    // fn_80125648 in the vtable; fn_80131D98 (game/tu_80131D98.cpp) calls it
    // and converts the byte result to bool.
    /* 0x60 */ virtual u8 vfunc_0x60();
    // fn_8012562C: copies the low byte of field_0x90 to field_0x35c and bits
    // 5-9 (from the top) of field_0x90 to field_0x3cc.
    /* 0x64 */ virtual void vfunc_0x64();
    /* 0x68 */ virtual void vfunc_0x68();
    /* 0x6C */ virtual s32 vfunc_0x6C(s32 state);
    // 0x80124C4C: sets the key tables of mAnim (14 of them, lbl_8049E198),
    // plays animation 7 in mode 2 without keeping the frame, and sets frame 0.
    /* 0x70 */ virtual void vfunc_0x70();
    // 0x80124C2C: returns the result of the base's fn_80121DB0.
    /* 0x74 */ virtual void vfunc_0x74();
    // 0x80124BF0: the base's vfunc_0x78 when field_0x33c is 4 or 5, else 0.
    /* 0x78 */ virtual s32 vfunc_0x78() const;
    // 0x80124BC0 / 0x80124B90 / 0x80124B60: vfunc_0x6C(7), (5) and (8).
    /* 0x84 */ virtual s32 vfunc_0x84();
    /* 0x88 */ virtual s32 vfunc_0x88();
    /* 0x8C */ virtual void vfunc_0x8C();
    // 0x80124B08: when the base's fn_80122834 is true and field_0x33c is
    // below 10, calls vfunc_0x6C(10).
    /* 0x90 */ virtual void vfunc_0x90();
    // 0x80124AD8: vfunc_0x6C(9), as the zora's 0x88 does.
    /* 0x94 */ virtual void vfunc_0x94();

    /* 0x3C4 */ s32 field_0x3c4; // cleared by the constructor
    /* 0x3C8 */ s32 field_0x3c8; // cleared by the constructor
    /* 0x3CC */ s32 field_0x3cc; // field_0x90 bits 5-9 (from the top), set by vfunc_0x64
    /* 0x3D0 */ s32 field_0x3d0;
}; // Size: 0x3D4

#endif
