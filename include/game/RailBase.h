#ifndef GAME_RAILBASE_H
#define GAME_RAILBASE_H

// Types whose code is in game/RailBase.cpp (the file name comes from the
// "RailBase.cpp" OSPanic string; the panic message is
// "RailBase: Buffsize Error").
//
// The same file also holds the RAIL actor (ActorId ACTOR_RAIL): vtable
// 0x8049C770, destructor fn_800FD438 (it chains to ~ActorBase_804A5770), and an inline
// constructor in the spawner's unit (fn_80202A0C, __nw__ 0x230, so no fields
// beyond ActorBase_804A5770). Its class may be "RailBase" itself, but nothing proves
// that yet, so it is not declared here. The unit's worker declares it here
// as a SHARED-EDIT.
//
// Owner: the shared-header owner (docs/COORDINATOR.md).

#include "types.h"
#include "JSystem/JGeometry.h"

// The element class of a 0xC8-byte buffer (0x31 pointer entries). Its method
// fn_800FE3EC (RailBase.cpp) starts by clearing bytes 2, 3 and 1, as the
// inline clear() does, and then fills the entries. Users: the pair container
// in game/tu_80214244.cpp, also reached from tu_801D3320 and tu_8021CDE4
// (progress/game__tu_80214244.md). Named after its method until evidence
// names it.
class cls_800FE3EC {
public:
    // Mangled fn_800FE3EC__12cls_800FE3ECFUcQ29JGeometry8TVec3<f>i; the vector
    // is passed by value.
    void fn_800FE3EC(u8, JGeometry::TVec3f, int);

    void clear() {
        field_0x2 = 0;
        field_0x3 = 0;
        field_0x1 = 0;
    }

    /* 0x00 */ u8 field_0x0; // fn_800FE3EC's first argument
    /* 0x01 */ u8 field_0x1;
    /* 0x02 */ u8 field_0x2;
    /* 0x03 */ u8 field_0x3;
    /* 0x04 */ void* field_0x4[0x31];
}; // Size: 0xC8

#endif
