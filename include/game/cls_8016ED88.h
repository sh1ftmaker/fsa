#ifndef GAME_CLS_8016ED88_H
#define GAME_CLS_8016ED88_H

#include "types.h"
#include "JSystem/JGeometry.h"

class ActorSoundObj_804578F4;
class JPABaseEmitter;

// A 0x1C-byte entry whose constructor is the empty fn_8016FDE0 (game/tu_8016A2B4.cpp);
// arrays of four are built with __construct_array by fn_8016F0BC (at its
// this + 0x140) and Title_804AF820's constructor (at 0x90). Named after its
// constructor. Only the fields the known code touches are declared.
struct cls_8016FDE0 {
    cls_8016FDE0();

    /* 0x00 */ JPABaseEmitter* field_0x0;  // cleared by cls_8016ED88::fn_8016ED88
    /* 0x04 */ JGeometry::TVec3f field_0x4;
    /* 0x10 */ u16 field_0x10;
    /* 0x12 */ u8 field_0x12[2];
    /* 0x14 */ s32 field_0x14; // cleared by cls_8016ED88::fn_8016ED88
    // Title_804AF820's constructor puts a new ActorSoundObj_804578F4(NULL,
    // NULL, 8) here; its destructor deletes it.
    /* 0x18 */ ActorSoundObj_804578F4* field_0x18;
}; // Size: 0x1C

// Four cls_8016FDE0 entries. No vtable, constructor or destructor of its own:
// named after its only known function, fn_8016ED88 (game/tu_8016A2B4.cpp),
// which Title_804AF820::fn_803A5380 calls on its field_0x90.
struct cls_8016ED88 {
    // Clears field_0x0 and field_0x14 of each entry.
    void fn_8016ED88();

    /* 0x00 */ cls_8016FDE0 mEntries[4];
}; // Size: 0x70

#endif
