#ifndef GAME_TU_8012F3E4_H
#define GAME_TU_8012F3E4_H

// Exports of game/tu_8012F3E4.cpp. Called by tu_8012F8AC,
// act_ice_floor_check_npc and tu_802D39B8.

#include "game/ActorBase.h"

// The guarded static at 0x804BDDFC (function-local static of fn_8012F554): the
// serials of the 'IFCK' actors, an s32 count and 64 slots, all -1 after
// construction. No vtable; named after its destructor fn_8012F64C, which is
// passed to __register_global_object (0x8012F628). The constructor
// is inlined into fn_8012F554 and calls fn_8012F494.
struct cls_8012F64C {
    cls_8012F64C() : mCount(0) { fn_8012F494(); }
    ~cls_8012F64C() {}
    // Forgets every serial: count 0, all slots -1.
    void fn_8012F494();
    // True once every recorded actor is an 'IFCK' actor whose floor tile changed.
    bool fn_8012F3E4() const;
    // Appends the actor's serial (mSerial).
    void fn_8012F530(const ActorBase_804A5770* actor);

    /* 0x0 */ s32 mCount;
    /* 0x4 */ s32 mSerials[64];
}; // Size: 0x104

// Getter of the guarded static at 0x804BDDFC.
cls_8012F64C* fn_8012F554();

#endif
