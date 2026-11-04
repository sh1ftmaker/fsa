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
    // __dt__12cls_802150B8Fv (game/tu_802144F0.cpp): deletes the emitters at
    // 0x0 and 0x18, unlinks the actor at 0x30, runs mOwner's fn_801CDC8C(0x4000E),
    // then destroys the two callbacks; deletes itself when asked to.
    ~cls_802150B8();

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

    // game/tu_802144F0.cpp: from mOwner's position (y - 16): while
    // fn_801CD2F8(mOwner->field_0x4) holds, the actor at serial 0x30 (if >= 0
    // and live) takes that position; otherwise, with an emitter at 0x18, the
    // callback at 0x1C gets the position fn_80214E50 computes and that emitter
    // is moved with fn_8013CB44 (height and depth from z, adjust on).
    void fn_80214994();

    // game/tu_802144F0.cpp: called by act_shadow_link_2 with this+0x4EC, layer,
    // this+0xC and flag 0. Unless fn_801C0D70's object has bit 31 of its word
    // 0x50 set, spawns effects 0x1D6-0x1D9 at pos with depth (bit 0x400 of the
    // owner's field_0x524); otherwise three effects from the table for flag
    // (depth bit - 1, then the bit), and mOwner's fn_801CDCD0 with sound 0x40048
    // or 0x4004E. The first branch ends with mOwner's sound 0x400D6.
    void fn_80214740(s32 layer, const JGeometry::TVec3f* pos, bool flag);

    // game/tu_802144F0.cpp: (re)starts the position's owner: unlinks the actor
    // at 0x30 and deletes the emitter at 0x18, runs mOwner's fn_801CDC8C(0x4000E),
    // then spawns actor 'SLEF' (serial to 0x30) while fn_801CD2F8(mOwner->field_0x4)
    // holds, else effect 0x565 at the position with the callback at 0x1C (scale
    // 0.5, emitter to 0x18); ends with mOwner's sound 0x4000E.
    void fn_80214AF8();

    // game/tu_802144F0.cpp: while an emitter is held at 0x0, moves it with
    // fn_8013CB44 to the position fn_80214E50 computes (the callback at 0x4
    // takes the same point), then deletes it when it is flagged deletable with
    // no particles left.
    void fn_80214CE4();

    // game/tu_802144F0.cpp: unless an emitter is held at 0x0, spawns effect
    // 0x33B on mOwner's layer at its position (y - 24), lifted by 1024 when
    // the z is above 0 and by 128 otherwise (fn_8013CB44's height and depth),
    // with the particle callback at 0x4 whose position it sets to the same
    // point; the emitter is kept at 0x0 and made immortal.
    void fn_80214E50();

    // game/tu_802144F0.cpp: spawns effect 0x416 at mOwner's position with
    // height flag - 1 and effect 0x417 with height flag, flag being the
    // owner's field_0x524 bit 0x400 (act_shadow_link_2).
    void fn_802146A4() const;

    /* 0x00 */ JPABaseEmitter* field_0x0;
    /* 0x04 */ cls_804A64A8 field_0x4;
    /* 0x18 */ JPABaseEmitter* field_0x18;
    /* 0x1C */ cls_804A64A8 field_0x1c;
    /* 0x30 */ s32 field_0x30;
    /* 0x34 */ ActorBase_804A5770* mOwner; // the actor it is embedded in
                                           // (fn_801C573C stores it at +0x520)
}; // Size: 0x38

#endif
