#ifndef GAME_ACT_ACT_GBA_APPEAR_H
#define GAME_ACT_ACT_GBA_APPEAR_H

// Code: game/act_gba_appear.cpp.

#include "game/ActorBase.h"

class JPABaseEmitter;

// The GBAO actor ("Appears from GBA", docs/names/actors.tsv, med
// confidence): the spawner constructs it with fn_80201B78, which stores this
// vtable (config/G4SE01/game_units.tsv). Named after its vtable, 0x804B16E0.
class GBAAppear_804B16E0 : public ActorBase_804A5770 {
public:
    GBAAppear_804B16E0() {}
    /* 0x08 */ virtual ~GBAAppear_804B16E0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ JPABaseEmitter* mEmitters[4]; // per player; update creates, the destructor deletes
    /* 0x240 */ u8 mState[4]; // per player; 0xFF unset, 0 / 1 picks the _01 / _02 image
}; // Size: 0x244

#endif
