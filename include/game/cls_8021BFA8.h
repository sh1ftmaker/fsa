#ifndef GAME_CLS_8021BFA8_H
#define GAME_CLS_8021BFA8_H

#include "types.h"

// Named after its first method (fn_8021BFA8). Size 0xC. Defined in
// game/tu_8021BFA8.cpp; embedded at 0xC of each horse-race table entry
// (cls_80216238).
class cls_8021BFA8 {
public:
    int fn_8021BFA8();
    void fn_8021C00C();

    /* 0x0 */ int field_0x0;
    /* 0x4 */ u32 field_0x4;
    /* 0x8 */ int field_0x8;
};

#endif
