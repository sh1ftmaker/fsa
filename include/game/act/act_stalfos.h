#ifndef GAME_ACT_ACT_STALFOS_H
#define GAME_ACT_ACT_STALFOS_H

// Code: game/act_stalfos.cpp.

#include "game/ActorBase.h"

// The STAL actor ("Stalfos", docs/names/actors.tsv, high confidence): the
// spawner constructs it with fn_80202C8C, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A0CD8.
class Stalfos_804A0CD8 : public ActorBase_804A5770 {
public:
    Stalfos_804A0CD8() {}
    /* 0x08 */ virtual ~Stalfos_804A0CD8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ u8 field_0x234[0x4];
    /* 0x238 */ s32 mTimers[5];
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ s32 field_0x254;
    /* 0x258 */ s32 field_0x258;
    /* 0x25C */ s32 field_0x25c;
    /* 0x260 */ s32 field_0x260;
    /* 0x264 */ s32 field_0x264;
    /* 0x268 */ s16 field_0x268;
    /* 0x26A */ u8 field_0x26a[0x2];
    /* 0x26C */ JGeometry::TVec3f field_0x26c;
    /* 0x278 */ JGeometry::TVec3f field_0x278;
}; // Size: 0x284

#endif
