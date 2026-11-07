#ifndef GAME_ACT_ACT_OCTOROK_H
#define GAME_ACT_ACT_OCTOROK_H

// Code: game/act_octorok.cpp.

#include "game/ActorBase.h"

// The OKTA actor ("Octorok", docs/names/actors.tsv, high confidence): the
// spawner constructs it with fn_8020425C, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x8049F208.
class Octorok_8049F208 : public ActorBase_804A5770 {
public:
    Octorok_8049F208() {}
    /* 0x08 */ virtual ~Octorok_8049F208();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ s32 field_0x254;
    /* 0x258 */ s32 field_0x258;
    /* 0x25C */ f32 field_0x25c;
    /* 0x260 */ f32 field_0x260;
    /* 0x264 */ f32 field_0x264;
    /* 0x268 */ f32 field_0x268;
    /* 0x26C */ f32 field_0x26c;
    /* 0x270 */ s32 field_0x270;
    /* 0x274 */ s32 field_0x274;
    /* 0x278 */ u8 field_0x278;
    /* 0x27C */ JGeometry::TVec3f field_0x27c;
}; // Size: 0x288

#endif
