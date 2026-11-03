#ifndef GAME_ACT_ACT_ZORA_ESCORT_H
#define GAME_ACT_ACT_ZORA_ESCORT_H

// Code: game/act_zora_escort.cpp.

#include "game/tu_801213EC.h"

// The escorted Zora ('FAZR', docs/names/actors.tsv, med confidence). Named
// after its vtable, 0x8049DF78. Its constructor (fn_80121164) and destructor
// (fn_80121104) call the escort base's constructor and destructor.
class ZoraEscort_8049DF78 : public EscortBase_8049E0C8 {
public:
    ZoraEscort_8049DF78() {}
    /* 0x14 */ virtual void update();
    // Vtable slot 0x88, fn_80120098: forwards 9 to the state slot 0x6C.
    /* 0x88 */ virtual s32 vfunc_0x88();
    // Vtable slot 0x8C, fn_80120068: forwards 0xC to the state slot 0x6C.
    /* 0x8C */ virtual s32 vfunc_0x8C();
};

#endif
