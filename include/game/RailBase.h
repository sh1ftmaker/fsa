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
#include "game/ActorBase.h"

// The RAIL actor's class, named after its vtable 0x8049C770 (no name evidence
// yet). Only the init override (slot 0x0C, init__12cls_8049C770Fv, moves mPos by
// 8 in x and y), the update override (slot 0x14, fn_800FD49C, an empty `blr`) and
// the draw override (slot 0x20, fn_800FD498, an empty `blr`) are declared; the
// destructor fn_800FD438 and the other slots are not yet.
class cls_8049C770 : public ActorBase_804A5770 {
public:
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
};

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
    u8 fn_800FD894(s32 layer, u8* out, s32 max);
    bool fn_800FD5E8(u8 index);
    bool fn_800FD4C0(u8 a, u8 b);
    s32 fn_800FE25C(u8 index);

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

extern "C" {
// game/RailBase.cpp (not written yet): *out = the position of entry
// `index` (0xFF: the current one, entry field_0x3) of the buffer, read from
// the entry's +0xC. r3 out, r4 buffer, r5 index (clrlwi 24).
void fn_800FE294(JGeometry::TVec3f* out, cls_800FE3EC* buffer, u8 index);

}

#endif
