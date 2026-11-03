#ifndef GAME_ACT_ACT_SHOP_ITEM_H
#define GAME_ACT_ACT_SHOP_ITEM_H

// Code: game/act_shop_item.cpp.

#include "game/ActorBase.h"
#include "game/SpriteAnim.h"

// The SITM actor ("Shop item", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_801092B0, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x8049CDC0.
class ShopItem_8049CDC0 : public ActorBase_804A5770 {
public:
    ShopItem_8049CDC0() {}
    /* 0x08 */ virtual ~ShopItem_8049CDC0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ SpriteAnim_801D267C mAnim;
    /* 0x268 */ u8 field_0x268[0x1C]; // not yet known
}; // Size: 0x284

#endif
