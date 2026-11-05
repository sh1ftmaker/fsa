#ifndef GAME_ACT_TU_80362634_H
#define GAME_ACT_TU_80362634_H

// Code: game/tu_80362634.cpp.

#include "game/ActorBase.h"

// Spawned for 'EETC' (the ending's other characters, docs/names/actors.tsv; its
// "fune.bti" image is a boat). Named after its vtable, 0x804ADF28. A state
// machine: field_0x234 is the state, indexing tables of member function pointers.
class cls_804ADF28 : public ActorBase_804A5770 {
public:
    cls_804ADF28() {}
    /* 0x08 */ virtual ~cls_804ADF28();

    // 0x80362634: deletes the emitter.
    void fn_80362634();
    // 0x80362B14: draws the "fune.bti" image at mPos.
    void fn_80362B14();
    // 0x8036335C: empty.
    void fn_8036335C();
    // 0x80363F7C: steps the pattern index when its counter ran out, then moves by field_0x3c.
    void fn_80363F7C();
    // 0x80364F00: like fn_80363F7C with the pattern read from the byte tables at 0x270 / 0x274.
    void fn_80364F00();
    // 0x80365220: bobs mPos.y up and down every 0x40 frames.
    void fn_80365220();

    /* 0x230 */ s8 field_0x230;
    /* 0x231 */ u8 pad_0x231[3];
    /* 0x234 */ s32 field_0x234; // state, the index of the member function tables
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ u8 field_0x23c[0xC]; // not yet known
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ u8 field_0x24c[0x10]; // not yet known
    /* 0x25C */ s32 field_0x25c;
    /* 0x260 */ u8 field_0x260[4]; // not yet known
    /* 0x264 */ s32 field_0x264;
    /* 0x268 */ s32 field_0x268;
    /* 0x26C */ u8 field_0x26c[4]; // not yet known
    /* 0x270 */ s8* field_0x270;
    /* 0x274 */ s8* field_0x274;
    /* 0x278 */ u8 field_0x278[4]; // not yet known
    /* 0x27C */ JGeometry::TVec3f field_0x27c[3];
    /* 0x2A0 */ JGeometry::TVec3f field_0x2a0[3];
    /* 0x2C4 */ JGeometry::TVec3f field_0x2c4[3];
    /* 0x2E8 */ u8 field_0x2e8[0x18]; // not yet known
    /* 0x300 */ JPABaseEmitter* field_0x300;
    /* 0x304 */ u8 field_0x304[0x18]; // not yet known
}; // Size: 0x31C

#endif
