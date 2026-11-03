#ifndef GAME_ACT_ACT_SLIME_MANAGER_H
#define GAME_ACT_ACT_SLIME_MANAGER_H

// Code: game/act_slime_manager.cpp. Named after its vtable, 0x804A5BD8
// (an EnemyBase_804A4808 descendant; docs/names/actors.tsv has SLMN, the
// slime manager, which the vtable is not yet proven to be).

#include "game/EnemyBase.h"

class cls_804A5BD8 : public EnemyBase_804A4808 {
public:
    /* 0x88 */ virtual void vfunc_0x88();

    // 0x80207E24: fn_80208244(2) while mState is 4.
    void fn_80207E24();
    // 0x80208244, the state step (r4 = state); fn_80207ECC calls it with 3.
    void fn_80208244(int state);
};

#endif
