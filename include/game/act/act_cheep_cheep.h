#ifndef GAME_ACT_ACT_CHEEP_CHEEP_H
#define GAME_ACT_ACT_CHEEP_CHEEP_H

// Code: game/act_cheep_cheep.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A11B8.
class CheepCheep_804A11B8 : public ActorBase_804A5770 {
public:
    CheepCheep_804A11B8() {}
    /* 0x08 */ virtual ~CheepCheep_804A11B8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    /* 0x230 */ u8 field_0x230[0x4C]; // not yet known
}; // Size: 0x27C

#endif
