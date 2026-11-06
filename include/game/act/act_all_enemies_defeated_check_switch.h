#ifndef GAME_ACT_ACT_ALL_ENEMIES_DEFEATED_CHECK_SWITCH_H
#define GAME_ACT_ACT_ALL_ENEMIES_DEFEATED_CHECK_SWITCH_H

// Code: game/act_all_enemies_defeated_check_switch.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A8AC8.
class AllEnemiesDefeatedCheckSwitch_804A8AC8 : public ActorBase_804A5770 {
public:
    AllEnemiesDefeatedCheckSwitch_804A8AC8() {}
    /* 0x08 */ virtual ~AllEnemiesDefeatedCheckSwitch_804A8AC8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    void fn_802BDCC0();

    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ JGeometry::TBox2f field_0x23c;
    /* 0x24C */ u32 field_0x24c;
    /* 0x250 */ u8 field_0x250;
    /* 0x254 */ s32 field_0x254;
    /* 0x258 */ s32 field_0x258;
    /* 0x25C */ s32 field_0x25c;
}; // Size: 0x260

#endif
