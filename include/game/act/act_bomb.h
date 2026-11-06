#ifndef GAME_ACT_ACT_BOMB_H
#define GAME_ACT_ACT_BOMB_H

// Code: game/act_bomb.cpp (update() is not written yet). Only what other units
// read, and what the unit's functions touch, is declared.

#include "game/ActorBase.h"

// The HBMB actor ("Bomb", docs/names/actors.tsv, high confidence): the
// spawner constructs it with fn_80203D1C, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x8049F784.
class Bomb_8049F784 : public ActorBase_804A5770 {
public:
    Bomb_8049F784() {}
    /* 0x08 */ virtual ~Bomb_8049F784();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x44 */ virtual void vfunc_0x44();
    /* 0x48 */ virtual void vfunc_0x48();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    /* 0x54 */ virtual void vfunc_0x54();

    void fn_8014D868(const JGeometry::TVec3f* pos);
    bool fn_8014D6DC();

    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238;               // act_shadow_link_2's helper ignores bombs with >= 5
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ s32 field_0x254;
    /* 0x258 */ s32 field_0x258;
    /* 0x25C */ s32 field_0x25c;
    /* 0x260 */ s32 field_0x260;
    /* 0x264 */ s32 field_0x264;
    /* 0x268 */ s32 field_0x268;
    /* 0x26C */ s32 field_0x26c;
    /* 0x270 */ s32 field_0x270;
    /* 0x274 */ f32 field_0x274;
    /* 0x278 */ f32 field_0x278;
    /* 0x27C */ f32 field_0x27c;
    /* 0x280 */ u8 field_0x280;
    /* 0x281 */ u8 field_0x281[0x284 - 0x281];
    /* 0x284 */ JGeometry::TVec3f field_0x284;
    /* 0x290 */ JPABaseEmitter* mEmitters[3];  // released by the destructor
}; // Size: 0x29C

#endif
