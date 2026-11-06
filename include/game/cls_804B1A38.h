#ifndef GAME_CLS_804B1A38_H
#define GAME_CLS_804B1A38_H

#include "types.h"

// Audio-data loader (vtable 0x804B1A38), defined in game/tu_80442810.cpp.
// The constructor stores `this` in lbl_8053ADD8; tu_80400DC4's fn_80401318
// reads field_0x4 through it (0x804014E4).
class cls_804B1A38 {
public:
    cls_804B1A38();
    /* 0x08 */ virtual ~cls_804B1A38();
    void fn_80442810();

    /* 0x04 */ u8 field_0x4;
}; // Size: 0x8

// Data of a unit before game/tu_80400DC4.cpp (not that unit's: the original
// pads it to 8 bytes before tu_80400DC4's .sbss at 0x8053ADE0, a file
// boundary; compiled in one file the two would be packed 4-aligned).
extern cls_804B1A38* lbl_8053ADD8;

#endif
