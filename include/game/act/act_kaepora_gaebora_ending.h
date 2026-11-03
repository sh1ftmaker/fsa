#ifndef GAME_ACT_ACT_KAEPORA_GAEBORA_ENDING_H
#define GAME_ACT_ACT_KAEPORA_GAEBORA_ENDING_H

// Code: game/act_kaepora_gaebora_ending.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804ADAC8.
class cls_804ADAC8 : public ActorBase_804A5770 {
public:
    cls_804ADAC8() {}
    /* 0x08 */ virtual ~cls_804ADAC8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x8C]; // not yet known
}; // Size: 0x2BC

#endif
