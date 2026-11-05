#ifndef GAME_ACT_TU_80221B44_H
#define GAME_ACT_TU_80221B44_H

#include "game/cls_804A6A00.h"
#include "game/act/tu_800F7FCC.h"

// A minor Shadow Link actor (sub-base ShadowLinkBase_804A6A00) that, after its hop
// animation, puts a shockwave ring (cls_800F802C) and effects at its feet;
// code in game/tu_80221B44.cpp. Not tied to a spawner ID. Named after its
// vtable (0x804A6AA8). The destructor is fn_80221B44, the constructor
// fn_80222190.
class cls_804A6AA8 : public ShadowLinkBase_804A6A00 {
public:
    cls_804A6AA8();
    virtual ~cls_804A6AA8();
    /* 0x0C */ virtual void init();
    /* 0x88 */ virtual void vfunc_0x88();
    /* 0x90 */ virtual void vfunc_0x90(u8 mapHit);
    /* 0x9C */ virtual void vfunc_0x9C(u8 mapHit);

    // Starts the shockwave ring at pos (taken by value, as the original does).
    void startRing(s32 layer, const JGeometry::TVec3f& pos) {
        mRing.fn_800F8210(layer, pos, 2);
    }

    // Landings counted in state 7 (the third one goes to state 8).
    /* 0x2B8 */ s32 mLandings;
    /* 0x2BC */ cls_800F802C mRing;
}; // Size: 0x2D4

#endif
