#ifndef GAME_ACT_ACT_TALKING_SIGN_H
#define GAME_ACT_ACT_TALKING_SIGN_H

// Code: game/act_talking_sign.cpp.

#include "game/ActorBase.h"

// The KNBN actor ("Talking sign", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_80203610, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AF428.
class TalkingSign_804AF428 : public ActorBase_804A5770 {
public:
    TalkingSign_804AF428() {}
    /* 0x08 */ virtual ~TalkingSign_804AF428();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x8]; // not yet known
}; // Size: 0x238

#endif
