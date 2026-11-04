#ifndef GAME_ACT_ACT_GREAT_FAIRY_SPFR_H
#define GAME_ACT_ACT_GREAT_FAIRY_SPFR_H

// Code: game/act_great_fairy_spfr.cpp. The class is named after its vtable,
// 0x8049EB8C (lbl_8049EB8C); only the slots the unit defines are declared.

#include "game/ActorBase.h"

class cls_8049EB8C : public ActorBase_804A5770 {
public:
    /* 0x58 */ virtual s32 vfunc_0x58();

    /* 0x230 */ u8 field_0x230[0x234 - 0x230]; // not yet known
    /* 0x234 */ s32 field_0x234; // state: fn_801321E4 switches on it
};

#endif
