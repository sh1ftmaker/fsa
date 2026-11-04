#ifndef GAME_ACT_TU_801DB18C_H
#define GAME_ACT_TU_801DB18C_H

#include "types.h"
#include "game/EnemyBase.h"

// Code: game/tu_801DB18C.cpp. An EnemyBase_804A4808 named after its vtable,
// 0x804A4EE8 (config/G4SE01/tu_map.tsv: enemy_both_sob.bin unit).
// Its vfunc_0x9C runs EnemyBase's, then moves the emitter at 0x2E4 to mPos
// with fn_8013CB44 (height 1, depth 0, adjusted).
class cls_804A4EE8 : public EnemyBase_804A4808 {
public:
    /* 0x9C */ virtual void vfunc_0x9C(u8 mapHit);
    /* 0x2A8 */ u8 field_0x2a8[0x3C]; // not yet known
    /* 0x2E4 */ JPABaseEmitter* field_0x2e4; // emitter moved with mPos
}; // Size: at least 0x2E8

#endif
