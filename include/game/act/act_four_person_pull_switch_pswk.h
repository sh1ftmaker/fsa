#ifndef GAME_ACT_ACT_FOUR_PERSON_PULL_SWITCH_PSWK_H
#define GAME_ACT_ACT_FOUR_PERSON_PULL_SWITCH_PSWK_H

// Code: game/act_four_person_pull_switch_pswk.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804AE5C8.
class FourPersonPullSwitchChainVersion_804AE5C8 : public ActorBase_804A5770 {
public:
    FourPersonPullSwitchChainVersion_804AE5C8() {}
    /* 0x08 */ virtual ~FourPersonPullSwitchChainVersion_804AE5C8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x28]; // not yet known
}; // Size: 0x258

#endif
