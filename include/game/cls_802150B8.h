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

    // game/tu_802144F0.cpp: a copy of mOwner's position, nudged by field_0x98
    // (0: x -24, 1: x +24, 2: x +2 y -26, 3: x -4 y +18), then effects 0xCB
    // and 0xCC are spawned there on mOwner's layer, height 0, depth 1.
    void fn_80214578();

    // game/tu_802144F0.cpp: takes the actor serial at 0x30 (if >= 0) and
    // marks that actor not alive, sets the serial to -1, deletes the emitter
    // at 0x18 and calls mOwner's fn_801CDC8C(0x4000E).
    void fn_80214900();

    // game/tu_802144F0.cpp: spawns effect 0x416 at mOwner's position with
    // height flag - 1 and effect 0x417 with height flag, flag being the
    // owner's field_0x524 bit 0x400 (act_shadow_link_2).
    void fn_802146A4() const;

    /* 0x00 */ u32 field_0x0;
    /* 0x04 */ cls_804A64A8 field_0x4;
    /* 0x18 */ JPABaseEmitter* field_0x18;
    /* 0x1C */ cls_804A64A8 field_0x1c;
    /* 0x30 */ s32 field_0x30;
    /* 0x34 */ ActorBase_804A5770* mOwner; // the actor it is embedded in
                                           // (fn_801C573C stores it at +0x520)
}; // Size: 0x38

#endif
