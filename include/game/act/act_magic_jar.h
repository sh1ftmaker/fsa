#ifndef GAME_ACT_ACT_MAGIC_JAR_H
#define GAME_ACT_ACT_MAGIC_JAR_H

// Code: game/act_magic_jar.cpp.

#include "game/ActorBase.h"

// The MTBO actor ("Magic jar", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_80203A88, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AF018.
class MagicJar_804AF018 : public ActorBase_804A5770 {
public:
    MagicJar_804AF018() {}
    /* 0x08 */ virtual ~MagicJar_804AF018();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x40 */ virtual void vfunc_0x40();
    void collect(s32 player);
    /* 0x230 */ s32 field_0x230; // state: 0 airborne, 1 bouncing, 2 resting (vfunc_0x40 sets it)
    /* 0x234 */ s32 field_0x234; // frames left before it disappears; draw blinks below 0x100
    /* 0x238 */ s32 field_0x238; // bounces left
}; // Size: 0x23C

#endif
