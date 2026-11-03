#ifndef GAME_TU_801213EC_H
#define GAME_TU_801213EC_H

// Exports of game/tu_801213EC.cpp (not written yet). Only what the escort
// actors call is declared.

#include "game/ActorBase.h"

// The escort base class (vtable 0x8049E0C8), parent of EscortWoman_8049E1D0
// ('OPGL', act_escort_woman) and ZoraEscort_8049DF78 ('FAZR', act_zora_escort).
// Named after its vtable. Both children's constructors call its constructor
// (fn_801247C8) and both destructors its destructor (fn_80124740).
class EscortBase_8049E0C8 : public ActorBase_804A5770 {
public:
    /* 0x08 */ virtual ~EscortBase_8049E0C8();
    /* 0x14 */ virtual void update(); // fn_80124278; the vtable's update slot
    // Slots 0x54-0x68 of the vtable (fn_801213EC, fn_80123E94, ...) are not
    // known yet; they are declared only to number the slot below.
    /* 0x54 */ virtual void vfunc_0x54();
    /* 0x58 */ virtual void vfunc_0x58();
    /* 0x5C */ virtual void vfunc_0x5C();
    // Returns a byte: the escort woman's override (act_escort_woman.h) does,
    // and fn_80131D98 reads it as bool.
    /* 0x60 */ virtual u8 vfunc_0x60();
    /* 0x64 */ virtual void vfunc_0x64();
    /* 0x68 */ virtual void vfunc_0x68();
    // fn_80123E80: stores the argument at 0x33c and the old value at 0x340,
    // returns 1. The zora's and the escort woman's code call it with 0xC.
    /* 0x6C */ virtual s32 vfunc_0x6C(s32 state);
    // fn_80121DB0 (tu_801213EC): clears three bytes at 0x269-0x26B, returns 1.
    // The zora's fn_801200F8 and the escort woman's vfunc 0x74 call it.
    s32 fn_80121DB0();
    // fn_80124448 (tu_801213EC): calls ActorBase::init, then the vtable's 0x64
    // slot, then sets the state fields 0x60-0x6C. The escort woman's init calls it.
    void fn_80124448();
    /* 0x230 */ u8 field_0x230[0x33C - 0x230]; // not declared yet
    /* 0x33C */ s32 field_0x33c; // a state word, 0 after construction; the zora's update skips update() while it is 0xF
    // Size: unknown (only the fields above are declared)
};

#endif
