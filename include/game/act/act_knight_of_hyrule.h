#ifndef GAME_ACT_ACT_KNIGHT_OF_HYRULE_H
#define GAME_ACT_ACT_KNIGHT_OF_HYRULE_H

// Code: game/act_knight_of_hyrule.cpp.

#include "game/ActorBase.h"

// The JWLK actor ("Knight of Hyrule", docs/names/actors.tsv, med confidence):
// the spawner constructs it with fn_8020041C, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AABA8.
class KnightOfHyrule_804AABA8 : public ActorBase_804A5770 {
public:
    KnightOfHyrule_804AABA8() {}
    /* 0x08 */ virtual ~KnightOfHyrule_804AABA8();
    /* 0x0C */ virtual void init();
    /* 0x10 */ virtual void vfunc_0x10();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    /* 0x230 */ s32 mState;
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 mTimer;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ JGeometry::TBox2f field_0x24c;
    /* 0x25C */ JGeometry::TVec3f field_0x25c;
    /* 0x268 */ JGeometry::TVec3f field_0x268;
    /* 0x274 */ u8 field_0x274[0x4];
    /* 0x278 */ s32 field_0x278;
    /* 0x27C */ u8 field_0x27c;
    /* 0x27D */ u8 field_0x27d;
    /* 0x27E */ u8 field_0x27e;
    /* 0x27F */ u8 field_0x27f;
    /* 0x280 */ u16 field_0x280;
    /* 0x282 */ u16 field_0x282;
    /* 0x284 */ f32 field_0x284;
    /* 0x288 */ s32 field_0x288;
    /* 0x28C */ u8 field_0x28c;
    /* 0x28D */ u8 field_0x28d[0x3];
}; // Size: 0x290

#endif
