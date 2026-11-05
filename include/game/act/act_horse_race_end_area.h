#ifndef GAME_ACT_ACT_HORSE_RACE_END_AREA_H
#define GAME_ACT_ACT_HORSE_RACE_END_AREA_H

// Code: game/act_horse_race_end_area.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804AE830.
class HorseRaceEndArea_804AE830 : public ActorBase_804A5770 {
public:
    HorseRaceEndArea_804AE830() {}
    /* 0x08 */ virtual ~HorseRaceEndArea_804AE830();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
}; // Size: 0x230

#endif
