#ifndef GAME_ACT_ACT_GAMBLING_SHOP_H
#define GAME_ACT_ACT_GAMBLING_SHOP_H

// Code: game/act_gambling_shop.cpp.

#include "game/ActorBase.h"

// The GMBL actor ("Gambling shop", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_80200940, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AAA70.
class GamblingShop_804AAA70 : public ActorBase_804A5770 {
public:
    GamblingShop_804AAA70() {}
    /* 0x08 */ virtual ~GamblingShop_804AAA70();
    /* 0x0C */ virtual void init();
    /* 0x10 */ virtual void vfunc_0x10();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    // act_gambling_chest's update (0x80311364) calls it on the shop that
    // answers its serial: field_0x25c = value, field_0x234 = 0x3C, state 6
    // (stw 0x25c / 0x234 / 0x230, 0x8030FDC0).
    void fn_8030FDC0(s32 value);
    /* 0x230 */ u8 field_0x230[0x5C]; // not yet known
}; // Size: 0x28C

#endif
