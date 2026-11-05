#ifndef GAME_ACT_ACT_TINGLE_BUSTER_H
#define GAME_ACT_ACT_TINGLE_BUSTER_H

// Code: game/act_tingle_buster.cpp. Actor TINB ("Tingle Buster" in
// docs/names/actors.tsv, medium confidence); the spawner (spawnActor_801F9484)
// constructs it with the inline constructor at 0x801E33B8. It adds no fields
// to EnemyBase_804A4808 (size 0x2A8).

#include "game/EnemyBase.h"

// Named after its vtable (0x804A5370).
class TingleBuster_804A5370 : public EnemyBase_804A4808 {
public:
    TingleBuster_804A5370();
    virtual ~TingleBuster_804A5370();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
}; // Size: 0x2A8

// 0x801E3250, called by tu_801DE714. Whether the TINB actor on `layer`
// (fn_801F7884) has a kind (low nibble of field_0x90, or the next one when
// `highNibble`) that is 0, or 2 / 1 depending on lbl_8053AAF8->field_0x7e.
bool fn_801E3250(s32 layer, bool highNibble);

#endif
