#ifndef GAME_CLS_804B1A38_H
#define GAME_CLS_804B1A38_H

#include "types.h"

// Audio-data loader (vtable 0x804B1A38), defined in game/tu_80442810.cpp.
// The constructor stores `this` in lbl_8053ADD8; tu_80400DC4 (not written
// yet) reads field_0x4 through it (0x804014E4).
class cls_804B1A38 {
public:
    cls_804B1A38();
    /* 0x08 */ virtual ~cls_804B1A38();
    void fn_80442810();

    /* 0x04 */ u8 field_0x4;
}; // Size: 0x8

// Data of game/tu_80400DC4.cpp.
extern cls_804B1A38* lbl_8053ADD8;

#endif
