#ifndef GAME_TU_80131D98_H
#define GAME_TU_80131D98_H

// Exports of game/tu_80131D98.cpp. Called by the escort units (act_zora_escort
// and others) and by game/tu_80285E58.cpp.

#include "game/ActorBase.h"

// The guarded static at 0x804BE514 (function-local static of fn_80131F0C):
// the serials of the escort actors. No vtable; named after its destructor
// fn_80131F6C, which is passed to __register_global_object. The constructor
// is inlined into fn_80131F0C.
struct cls_80131F6C {
    cls_80131F6C() : mCount(0) {
        mSerials[0] = 0;
        mSerials[1] = 0;
    }
    ~cls_80131F6C() { fn_80131EF8(); }
    // The actor at index idx when it is a 'FAZR' or 'OPGL' actor, else NULL.
    ActorBase_804A5770* fn_80131E30(s32 idx);
    // Appends the actor's serial.
    void fn_80131EC0(const ActorBase_804A5770* actor);
    // Both forget every serial (identical code).
    void fn_80131EE4();
    void fn_80131EF8();

    /* 0x0 */ s32 mCount;
    /* 0x4 */ s32 mSerials[2];
}; // Size: 0xC

// True when the 'OPGL' actor (escorted woman) exists and its vfunc_0x60 says so.
bool fn_80131D98();
cls_80131F6C* fn_80131F0C();

#endif
