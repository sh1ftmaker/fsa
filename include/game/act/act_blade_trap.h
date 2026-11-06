#ifndef GAME_ACT_ACT_BLADE_TRAP_H
#define GAME_ACT_ACT_BLADE_TRAP_H

// Code: game/act_blade_trap.cpp.

#include "game/ActorBase.h"

// The TRAP actor ("Blade Trap", docs/names/actors.tsv, high confidence): the
// spawner constructs it with fn_8020382C, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A04D8.
class BladeTrap_804A04D8 : public ActorBase_804A5770 {
public:
    BladeTrap_804A04D8() {}
    /* 0x08 */ virtual ~BladeTrap_804A04D8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);

    // Both update() states that set the trap moving inline this.
    inline void setMoveSpeed();

    // State: 0 waiting at home, 1 sliding out, 2 sliding back home, 3
    // shuttling between walls (spawn parameter bits 12-15 set; init).
    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ s32 field_0x234; // wait after reaching home (10), counted down by update
    /* 0x238 */ s32 field_0x238; // animation frame timer (10), counted down by update
    /* 0x23C */ s32 field_0x23c; // wall spark cooldown (4), counted down by update
    /* 0x240 */ s32 field_0x240; // 0 in init
    /* 0x244 */ s32 field_0x244; // 0 in init
    /* 0x248 */ s32 field_0x248; // 0 in init
    /* 0x24C */ s32 field_0x24c; // vfunc_0x4C cooldown (120 after a 0x10-0x13 tile)
    /* 0x250 */ u8 field_0x250[4]; // not yet known
    /* 0x254 */ s32 field_0x254; // spawn parameter bit 4; draw's scale mode
    /* 0x258 */ s32 field_0x258; // spawn parameter bits 8-11; nonzero: "enemy_gc_sob.bin" animated
    /* 0x25C */ s32 field_0x25c; // spawn parameter bits 12-15; nonzero: starts in state 3
    /* 0x260 */ s32 field_0x260; // animation frame 0-3 (draw adds 0x17A)
    /* 0x264 */ u8 field_0x264;  // spawn parameter bit 2: turns towards the player
    /* 0x268 */ JGeometry::TVec3f field_0x268; // target velocity (field_0x3c eases to it)
    /* 0x274 */ JGeometry::TVec3f field_0x274; // home position (mPos in init)
}; // Size: 0x280

#endif
