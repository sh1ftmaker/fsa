#ifndef GAME_CLS_80218A8C_H
#define GAME_CLS_80218A8C_H

#include "types.h"

class ShadowLink2_804A43E8;

// An 8-byte helper embedded in the Shadow Link 2 actor (act_shadow_link_2,
// +0x540; its constructor fn_801C573C calls this class's constructor
// fn_80218A8C), defined in game/tu_802188B0.cpp. No vtable; named after its
// constructor. Its owner is the actor itself.
class cls_80218A8C {
public:
    cls_80218A8C();
    // Per-frame (fn_802188B0): while the timer runs down, nothing; at the
    // end, if the owner is on the ground and fast enough, no player is near
    // and it is in state 0x11 or 0x1b, spawns a bomb (HBMB) and restarts
    // the timer.
    void fn_802188B0();
    // fn_802189AC: whether a live bomb (HBMB) on the owner's layer is within
    // 32 units (squared distance in x, y <= 1024) of the owner.
    bool fn_802189AC();

    /* 0x0 */ ShadowLink2_804A43E8* mOwner;
    /* 0x4 */ u16 mTimer;
}; // Size: 0x8

#endif
