#ifndef GAME_TU_80127E20_H
#define GAME_TU_80127E20_H

// Exports of game/tu_80127E20.cpp. Called by the Zora escort units
// (act_zora_escort, act_zora_escort_goal).

#include "game/ActorBase.h"

// The guarded static at 0x804BD36C (function-local static of fn_801283E8): the
// serials of the escort's 'FZRA' (ZoraEscortGoal) actors. No vtable; named
// after its destructor fn_80128460, which is passed to __register_global_object
// (the constructor is inlined into fn_801283E8). Twin of cls_80131F6C.
struct cls_80128460 {
    cls_80128460() : mCount(0) {
        for (s32 i = 0; i < 8; i++) {
            mSerials[i] = 0;
        }
    }
    ~cls_80128460() {}

    // Move every 'FZRA' actor whose field_0x258 is 0 (fn_80127E20) / nonzero
    // (fn_80127ED8) to *pos.
    void fn_80127E20(const JGeometry::TVec3f* pos);
    void fn_80127ED8(const JGeometry::TVec3f* pos);
    // The midpoint (x, y, 0) of the first two actors' positions, or the zero
    // vector with fewer than two. The result is a stack local.
    const JGeometry::TVec3f* fn_80127F90();
    // Call fn_80126A4C(0x10) / fn_80126A4C(0xA) on every 'FZRA' actor.
    void fn_80128074();
    void fn_801281A4();
    // True when an 'FZRA' actor's state word is 2, 3 or 9.
    bool fn_801280F8();
    // fn_80230534(false); the object is not used.
    void fn_80128228();
    // fn_8023077C(), then fn_80126A4C(5) on each actor; false when one is
    // not an 'FZRA' actor.
    bool fn_8012824C();
    // True when there is at least one actor and every one is a live 'FZRA'
    // actor with fn_80126EF8() set.
    bool fn_801282E0();
    // Appends the actor's serial.
    void fn_80128398(const ActorBase_804A5770* actor);
    // Forgets every serial.
    void fn_801283BC();

    /* 0x00 */ s32 mCount;
    /* 0x04 */ s32 mSerials[8];
}; // Size: 0x24

cls_80128460* fn_801283E8();

#endif
