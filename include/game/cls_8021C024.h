#ifndef GAME_CLS_8021C024_H
#define GAME_CLS_8021C024_H

#include "types.h"

// Named after its first method (fn_8021C024). Size 0xC. Defined in
// game/tu_8021BFA8.cpp; embedded at 0x94 of the horse-race table (cls_80215E48).
class cls_8021C024 {
public:
    int fn_8021C024();
    void fn_8021C078();

    /* 0x0 */ int field_0x0;
    /* 0x4 */ u32 field_0x4;
    /* 0x8 */ int field_0x8;
};

#endif
