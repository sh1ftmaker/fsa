#ifndef GAME_ACT_ACT_ESCORT_WOMAN_H
#define GAME_ACT_ACT_ESCORT_WOMAN_H

// Code: game/act_escort_woman.cpp (not written yet). Only the slots other
// units call are declared.

#include "game/ActorBase.h"

// The escorted woman ('OPGL', actors.tsv). "EscortWoman_8049E1D0" is a
// provisional name carrying its vtable address; the constructor is
// fn_801258D8. It first calls fn_801247C8 (tu_801213EC), so the real parent
// is an unwritten tu_801213EC class.
class EscortWoman_8049E1D0 : public ActorBase_804A5770 {
public:
    /* 0x08 */ virtual ~EscortWoman_8049E1D0();
    /* 0x54 */ virtual void vfunc_0x54();
    /* 0x58 */ virtual void vfunc_0x58();
    /* 0x5C */ virtual void vfunc_0x5C();
    // fn_80125648 in the vtable; fn_80131D98 (game/tu_80131D98.cpp) calls it
    // and converts the byte result to bool.
    /* 0x60 */ virtual u8 vfunc_0x60();
};

#endif
