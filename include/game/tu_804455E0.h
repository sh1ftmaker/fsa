#ifndef GAME_TU_804455E0_H
#define GAME_TU_804455E0_H

// Exports of game/tu_804455E0.cpp (not written yet).

#include "types.h"

// The 0x118-byte object whose constructor (game/tu_804455E0.cpp, 0x804461D0)
// stores the vtable 0x804B1BB0 and the object itself in lbl_8053AEF8. Named
// after that vtable. Only the best-time records fn_801ED3DC (game/tu_801ED3DC.cpp)
// writes are known.
class cls_804B1BB0 {
public:
    /* 0x000 */ u8 pad_0x0[0x38]; // vtable pointer and unknown fields
    /* 0x038 */ u8 field_0x38[4]; // per slot: hundredths of a second
    /* 0x03C */ u8 field_0x3c[4]; // per slot: seconds
    /* 0x040 */ u8 field_0x40[4]; // per slot: rank + 1
    /* 0x044 */ u8 field_0x44;    // bit n: slot n has a time; bit n + 4: slot n has a rank
    /* 0x045 */ u8 pad_0x45[0x118 - 0x45];
}; // Size: 0x118
extern cls_804B1BB0* lbl_8053AEF8;

#endif
