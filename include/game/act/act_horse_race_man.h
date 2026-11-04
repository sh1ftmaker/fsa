#ifndef GAME_ACT_ACT_HORSE_RACE_MAN_H
#define GAME_ACT_ACT_HORSE_RACE_MAN_H

// Code: game/act_horse_race_man.cpp.

#include "game/EnemyBase.h"

// The horse-race man (HRMN in docs/names/actors.tsv). Named after its vtable,
// 0x804A5630; its overrides are in the unit's vtable at 0x90-0xA0.
class HorseRaceMan_804A5630 : public EnemyBase_804A4808 {
public:
    /* 0x94 */ virtual void vfunc_0x94();
};

#endif
