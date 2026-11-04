#ifndef GAME_ACT_ACT_BUMPER_H
#define GAME_ACT_ACT_BUMPER_H

// Code: game/act_bumper.cpp.

#include "game/ActorBase.h"

// The BOYO actor ("Bumper", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_80203B78, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A0260.
class Bumper_804A0260 : public ActorBase_804A5770 {
public:
    Bumper_804A0260() {}
    /* 0x08 */ virtual ~Bumper_804A0260();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ f32 field_0x234; // orbit radius (30.0f or 80.0f)
    /* 0x238 */ s32 field_0x238; // field_0x90 bit 16
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c; // angle, u16 (fn_801F6874 of field_0x98)
    /* 0x250 */ s32 field_0x250; // angular speed: 100, -100 or doubled
    /* 0x254 */ u8 field_0x254[4];
    /* 0x258 */ s32 field_0x258;
    /* 0x25C */ u16 field_0x25c; // countdown, 0x100 per update
    /* 0x25E */ u8 field_0x25e[2];
    /* 0x260 */ f32 field_0x260;
    /* 0x264 */ f32 field_0x264;
    /* 0x268 */ f32 field_0x268;
    /* 0x26C */ JGeometry::TVec3f field_0x26c; // the spawn position, copied in init
}; // Size: 0x278

#endif
