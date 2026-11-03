#ifndef GAME_ACT_ACT_FOUR_PERSON_PULL_SWITCH_PSWB_H
#define GAME_ACT_ACT_FOUR_PERSON_PULL_SWITCH_PSWB_H

// Code: game/act_four_person_pull_switch_pswb.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804AE574.
class cls_804AE574 : public ActorBase_804A5770 {
public:
    cls_804AE574() {}
    /* 0x08 */ virtual ~cls_804AE574();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x28]; // not yet known
}; // Size: 0x258

#endif
