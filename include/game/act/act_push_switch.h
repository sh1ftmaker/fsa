#ifndef GAME_ACT_ACT_PUSH_SWITCH_H
#define GAME_ACT_ACT_PUSH_SWITCH_H

// Code: game/act_push_switch.cpp.

#include "game/ActorBase.h"

// The PSSW actor ("Push switch", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_80201980, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A7B38.
class PushSwitch_804A7B38 : public ActorBase_804A5770 {
public:
    PushSwitch_804A7B38() {}
    /* 0x08 */ virtual ~PushSwitch_804A7B38();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    // How many tiles long the switch is (param bits 12-16; 0 means 1).
    s32 getLength() {
        s32 length = 1;
        if ((field_0x90 >> 12) & 0x1F) {
            length = (field_0x90 >> 12) & 0x1F;
        }
        return length;
    }
}; // Size: 0x230

#endif
