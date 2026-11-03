#ifndef GAME_CLS_8049D740_H
#define GAME_CLS_8049D740_H

// Code: game/act_maiden_demo.cpp (not written yet). The Maiden (DMMD) actor,
// named after its vtable, 0x8049D740, which its constructor (0x8011342C)
// stores after cls_8049D140's init. Only the field PrincessZeldaDemo reads is
// known.

#include "game/act/tu_8010DF44.h"

class cls_8049D740 : public cls_8049D140 {
public:
    /* 0x10 */ virtual void vfunc_0x10(); // copies the Goal (DMGL) actor's position
    /* 0x58 */ virtual s32 vfunc_0x58();

    /* 0x230 */ s32 field_0x230; // not yet known
    /* 0x234 */ s32 mMode; // field_0x90 & 0x1F, set by init (0x801133C0)
    /* 0x238 */ s32 field_0x238; // returned by vfunc_0x58
    /* 0x23C */ u8 field_0x23c[0x18]; // not yet known
    // The position of the Goal (DMGL) actor, copied by vfunc_0x10.
    /* 0x254 */ JGeometry::TVec3f mGoalPos;
}; // Size: at least 0x260

#endif
