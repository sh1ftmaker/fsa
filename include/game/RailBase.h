#ifndef GAME_RAILBASE_H
#define GAME_RAILBASE_H

// Types whose code is in game/RailBase.cpp (the file name comes from the
// "RailBase.cpp" OSPanic string; the panic message is
// "RailBase: Buffsize Error").
//
// The same file also holds the RAIL actor (ActorId ACTOR_RAIL): vtable
// 0x8049C770, destructor __dt__12cls_8049C770Fv (it chains to ~ActorBase_804A5770), and an inline
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
// destructor (slot 0x08, __dt__12cls_8049C770Fv) and the other slots are not yet.
class cls_8049C770 : public ActorBase_804A5770 {
public:
    /* 0x08 */ virtual ~cls_8049C770();
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
    // RailBase.cpp (0x800FE2E4, not written yet): the entry index of the
    // point nearest to *pos on `layer`, as fn_800FE294 takes it; game/
    // tu_801C5B08.cpp (fn_801C5B08) calls it on the current buffer.
    u8 fn_800FE2E4(const JGeometry::TVec3f* pos, s32 layer);
    void fn_800FDE38();
    // RailBase.cpp (0x800FDEA0, not written yet): r3 is the rail buffer; game/
    // act_princess_zelda.cpp passes the actor position, 1.0f, an out vector
    // (the step to add to the position), a layer, an out angle and 0x200.
    // Returns 3 when the walk is over.
    s32 fn_800FDEA0(JGeometry::TVec3f* pos, f32 speed, JGeometry::TVec3f* outStep, s32* layer, u16* outAngle, s32 arg);
    bool fn_800FD548(u8 index);
    bool fn_800FD594(u8 index);
    // RailBase.cpp (not written yet): steps the rail in direction dir (1 or -1);
    // called from fn_801C79A0 (tu_801C71BC.cpp) with the result of fn_80214244.
    void fn_800FD818(s32 dir);
    bool fn_800FD4C0(u8 a, u8 b);
    s32 fn_800FE25C(u8 index);
    u8 fn_800FE21C(u8 index);

    void clear() {
        field_0x2 = 0;
        field_0x3 = 0;
        field_0x1 = 0;
    }

    /* 0x00 */ u8 field_0x0; // fn_800FE3EC's first argument
    /* 0x01 */ u8 field_0x1;
    /* 0x02 */ u8 field_0x2;
    /* 0x03 */ u8 field_0x3;
    /* 0x04 */ ActorBase_804A5770* field_0x4[0x31]; // the rail points (fn_801F71E8 fills them)
}; // Size: 0xC8

extern "C" {
// game/RailBase.cpp: *out = the position of entry
// `index` (0xFF: the current one, entry field_0x3) of the buffer, read from
// the entry's +0xC. r3 out, r4 buffer, r5 index (clrlwi 24).
void fn_800FE294(JGeometry::TVec3f* out, cls_800FE3EC* buffer, u8 index);
// game/RailBase.cpp (0x800FDC70, not written yet): r3 out vector, r4 rail buffer, r5 position
// (by value), r6 the constant 1, r7 entry index (clrlwi 24); game/act_zora_escort_goal.cpp state 11
// passes the last entry (count - 1). What it computes is not yet known.
void fn_800FDC70(JGeometry::TVec3f* out, cls_800FE3EC* rail, JGeometry::TVec3f pos, s32 arg4, u8 index);

}

#endif
