#ifndef GAME_ACT_ACT_SLIME_MANAGER_H
#define GAME_ACT_ACT_SLIME_MANAGER_H

// Code: game/act_slime_manager.cpp. Named after its vtable, 0x804A5BD8
// (docs/names/actors.tsv has SLMN, the slime manager, which the vtable is
// not yet proven to be). Its constructor, __ct__12cls_804A5BD8Fv (0x80208BE0),
// calls __ct__12cls_804A5B28Fv, so the base is cls_804A5B28 (0x340 bytes).

#include "game/act/tu_80207548.h"

class cls_804A5BD8 : public cls_804A5B28 {
public:
    // 0x80208BE0: the base constructor, then this vtable and zeroed fields.
    cls_804A5BD8();

    /* 0x88 */ virtual void vfunc_0x88();

    // 0x80207E24: fn_80208244(2) while mState is 4.
    void fn_80207E24();
    // 0x80207E58: true in state 6; in state 3 it moves to 6 (true) once
    // field_0x34a reaches 10, else to state 2 (false).
    bool fn_80207E58();
    // 0x80207ECC: fn_80208244(3) while mState is 1.
    void fn_80207ECC();
    // 0x80208244, the state step (r4 = state); fn_80207ECC calls it with 3.
    void fn_80208244(int state);

    // 0x802088B4: fn_80208244(5) unless already in state 5, then
    // fn_80209D44 on each slot's actor (fn_801F666C of the slot's serial).
    void fn_802088B4();

    // 0x80208924: the tile codes under mPos on this layer, from
    // fn_8022461C and fn_80226850.
    void fn_80208924();

    /* 0x340 */ s32 field_0x340;
    /* 0x344 */ u8 field_0x344;
    /* 0x345 */ u8 field_0x345;
    /* 0x346 */ u16 field_0x346; // fn_8022461C(&mPos, field_0x4)
    /* 0x348 */ u16 field_0x348; // fn_80226850(&mPos, field_0x4)
    /* 0x34A */ u8 field_0x34a;
};

#endif
