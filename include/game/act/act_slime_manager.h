#ifndef GAME_ACT_ACT_SLIME_MANAGER_H
#define GAME_ACT_ACT_SLIME_MANAGER_H

// Code: game/act_slime_manager.cpp. Named after its vtable, 0x804A5BD8
// (an EnemyBase_804A4808 descendant; docs/names/actors.tsv has SLMN, the
// slime manager, which the vtable is not yet proven to be).

#include "game/EnemyBase.h"

class cls_804A5BD8 : public EnemyBase_804A4808 {
public:
    /* 0x88 */ virtual void vfunc_0x88();
};

#endif
