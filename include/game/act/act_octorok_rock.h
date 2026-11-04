#ifndef GAME_ACT_ACT_OCTOROK_ROCK_H
#define GAME_ACT_ACT_OCTOROK_ROCK_H

// Code: game/act_octorok_rock.cpp.

#include "game/ActorBase.h"

// The OKTM actor ("Octorok rock", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_80203E9C, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x8049F5D8.
class OctorokRock_8049F5D8 : public ActorBase_804A5770 {
public:
    OctorokRock_8049F5D8() {}
    /* 0x08 */ virtual ~OctorokRock_8049F5D8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c; // act_shadow_link_bow: distance over speed (fn_80222730)
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ f32 field_0x254;
    /* 0x258 */ s32 field_0x258;
    /* 0x25C */ u16 field_0x25c; // angle, subtracted from the target's facing (lhz)
    /* 0x25E */ u8 field_0x25e[2]; // not yet known
}; // Size: 0x260

#endif
