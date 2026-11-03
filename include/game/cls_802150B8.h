#ifndef GAME_CLS_802150B8_H
#define GAME_CLS_802150B8_H

#include "types.h"
#include "game/cls_804A64A8.h"

class ActorBase_804A5770;

// A struct with no vtable, named after its constructor (fn_802150B8,
// game/tu_802144F0.cpp). It holds two particle callbacks (cls_804A64A8) at
// 0x04 and 0x1C and is embedded in act_shadow_link_2 at +0x4EC (the
// constructor is called from fn_801C573C).
// Size: 0x38.
class cls_802150B8 {
public:
    cls_802150B8();

    // game/tu_802144F0.cpp: starts sound 0x400C3 on mOwner, then spawns effect
    // 0x3CF at mOwner's position raised by 6.
    void fn_802144F0();

    /* 0x00 */ u32 field_0x0;
    /* 0x04 */ cls_804A64A8 field_0x4;
    /* 0x18 */ u32 field_0x18;
    /* 0x1C */ cls_804A64A8 field_0x1c;
    /* 0x30 */ s32 field_0x30;
    /* 0x34 */ ActorBase_804A5770* mOwner; // the actor it is embedded in
                                           // (fn_801C573C stores it at +0x520)
}; // Size: 0x38

#endif
