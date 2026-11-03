#ifndef GAME_ACT_ACT_GAMBLING_CHEST_H
#define GAME_ACT_ACT_GAMBLING_CHEST_H

// Code: game/act_gambling_chest.cpp.

#include "game/ActorBase.h"

// The GMTB actor ("Gambling shop chest", docs/names/actors.tsv, med
// confidence): the spawner constructs it with fn_80200904, which stores this
// vtable (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AAAE8.
class GamblingShopChest_804AAAE8 : public ActorBase_804A5770 {
public:
    GamblingShopChest_804AAAE8() {}
    /* 0x08 */ virtual ~GamblingShopChest_804AAAE8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x28]; // not yet known
}; // Size: 0x258

#endif
