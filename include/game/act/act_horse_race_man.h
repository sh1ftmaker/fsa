#ifndef GAME_ACT_ACT_HORSE_RACE_MAN_H
#define GAME_ACT_ACT_HORSE_RACE_MAN_H

// Code: game/act_horse_race_man.cpp.

#include "game/EnemyBase.h"
#include "game/tu_801ED3DC.h"
#include "game/tu_80215A94.h"

// The horse-race man (HRMN in docs/names/actors.tsv). Named after its vtable,
// 0x804A5630; its overrides are in the unit's vtable at 0x90-0xA0.
class HorseRaceMan_804A5630 : public EnemyBase_804A4808 {
public:
    // 0x801ED144. Sets the player count, clears the race state and starts the
    // countdown and the entry table.
    HorseRaceMan_804A5630();
    // 0x801ED0AC (deleting destructor). Empty in source: the base and member
    // destructors are inline and the deleting form adds the __dl__ call.
    virtual ~HorseRaceMan_804A5630();
    /* 0x94 */ virtual void vfunc_0x94();

    /* 0x2A8 */ s32 field_0x2a8;   // fn_80138A30(): the number of players taking part
    /* 0x2AC */ s32 field_0x2ac;   // zeroed by the constructor
    /* 0x2B0 */ s32 field_0x2b0;   // -1 from the constructor
    /* 0x2B4 */ u8 field_0x2b4;    // zeroed by the constructor
    /* 0x2B6 */ cls_801ED828 mCountdown;
    /* 0x2B8 */ cls_80215E48 mTable;
};

#endif
