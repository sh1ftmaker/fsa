#ifndef GAME_ACT_ACT_ESCORT_WOMAN_H
#define GAME_ACT_ACT_ESCORT_WOMAN_H

// Code: game/act_escort_woman.cpp (not written yet). Only the slots other
// units call are declared.

#include "game/tu_801213EC.h"

// The escorted woman ('OPGL', actors.tsv). "EscortWoman_8049E1D0" is a
// provisional name carrying its vtable address; the constructor is
// fn_801258D8. It calls fn_801247C8 (tu_801213EC), so its parent is
// EscortBase_8049E0C8.
class EscortWoman_8049E1D0 : public EscortBase_8049E0C8 {
public:
    /* 0x08 */ virtual ~EscortWoman_8049E1D0();
    // fn_80125858: the vtable's init slot (ActorBase's 0x0C); calls the base's fn_80124448.
    /* 0x0C */ virtual void init();
    /* 0x54 */ virtual void vfunc_0x54();
    /* 0x58 */ virtual void vfunc_0x58();
    /* 0x5C */ virtual void vfunc_0x5C();
    // fn_80125648 in the vtable; fn_80131D98 (game/tu_80131D98.cpp) calls it
    // and converts the byte result to bool.
    /* 0x60 */ virtual u8 vfunc_0x60();
    // fn_8012562C: copies the low byte of field_0x90 to field_0x35c and bits
    // 5-9 (from the top) of field_0x90 to field_0x3cc.
    /* 0x64 */ virtual void vfunc_0x64();
    // fn_80124C2C: returns the result of the base's fn_80121DB0.
    /* 0x74 */ virtual s32 vfunc_0x74();
    // fn_80124BF0: returns the base's fn_80123D88 when field_0x33c is 4 or 5, else 0.
    /* 0x78 */ virtual s32 vfunc_0x78();
    // fn_80124B90: returns the base's vfunc_0x6C(5).
    // fn_80124BC0: returns the base's vfunc_0x6C(7).
    /* 0x84 */ virtual s32 vfunc_0x84();
    /* 0x88 */ virtual s32 vfunc_0x88();
    // fn_80124B60: returns the base's vfunc_0x6C(8).
    /* 0x8C */ virtual s32 vfunc_0x8C();
    // fn_80124AD8: returns the base's vfunc_0x6C(9), as the zora's 0x88 does.
    /* 0x94 */ virtual s32 vfunc_0x94();

    // fn_80124B08: when the base's fn_80122834 is true and field_0x33c is
    // below 10, calls the vtable's 0x6C slot with 10. Not known whether it
    // is a vtable slot itself.
    void fn_80124B08();

    /* 0x340 */ u8 field_0x340[0x35C - 0x340]; // not declared yet
    /* 0x35C */ s32 field_0x35c; // the low byte of field_0x90, set by vfunc_0x64
    /* 0x360 */ u8 field_0x360[0x3CC - 0x360]; // not declared yet
    /* 0x3CC */ s32 field_0x3cc; // field_0x90 bits 5-9 (from the top), set by vfunc_0x64
};

#endif
