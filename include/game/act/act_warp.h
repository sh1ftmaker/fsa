#ifndef GAME_ACT_ACT_WARP_H
#define GAME_ACT_ACT_WARP_H

// Code: game/act_warp.cpp.

#include "game/ActorBase.h"

// The WARP actor ("Warp", docs/names/actors.tsv, med confidence): the spawner
// constructs it with fn_802040B8, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A6DC8.
class Warp_804A6DC8 : public ActorBase_804A5770 {
public:
    Warp_804A6DC8() {}
    /* 0x08 */ virtual ~Warp_804A6DC8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x4]; // not yet known
    /* 0x234 */ u8 field_0x234[4]; // per player: 1 while the player stands on the warp
    /* 0x238 */ JGeometry::TVec3f field_0x238; // Bomb's fn_8014D868 spawns a bomb here (0x8014D9C4)
    /* 0x244 */ s32 field_0x244; // compared with GameView field_0x14 by cls_804A7AE0::vfunc_0x10
    /* 0x248 */ s32 field_0x248; // compared with GameView field_0x18
    /* 0x24C */ JPABaseEmitter* mEmitter24c; // released by the destructor
    /* 0x250 */ JPABaseEmitter* mEmitter250; // released by the destructor
    // Code: game/act_warp.cpp.
    void fn_80226FC8(s32 player);
    s32 fn_80227290(f32 maxDz, const u32* ids, s32 excludeSerial);
    void fn_80227530(JGeometry::TBox2f* box);
    void fn_802278A8(JGeometry::TBox2f* box);
    /* 0x254 */ u8 field_0x254;
    /* 0x255 */ u8 field_0x255; // the warp effect has been spawned
    /* 0x256 */ u8 field_0x256; // draw: the sand shifts while it is 1
    /* 0x257 */ u8 field_0x257[1]; // not yet known
    /* 0x258 */ f32 field_0x258[4]; // per player: fn_8022C76C, written by update
    /* 0x268 */ f32 field_0x268[4]; // copy of field_0x258
    /* 0x278 */ s32 field_0x278; // index into the shake table, -1 when idle
    /* 0x27C */ ActorBase_804A5770* field_0x27c[4]; // per player: the actor that is being carried in
}; // Size: 0x28C

#endif
