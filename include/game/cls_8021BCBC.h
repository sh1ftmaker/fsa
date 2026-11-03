#ifndef GAME_CLS_8021BCBC_H
#define GAME_CLS_8021BCBC_H

#include "types.h"

// Defined in game/tu_8021BCBC.cpp (same layout there). 12 bytes.
class cls_8021BCBC {
public:
    void fn_8021BCBC();
    void fn_8021BD18(int);

    /* 0x0 */ int field_0x0;
    /* 0x4 */ u32 field_0x4;
    /* 0x8 */ int field_0x8;
};

#endif
