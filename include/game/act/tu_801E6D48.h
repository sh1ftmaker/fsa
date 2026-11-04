#ifndef GAME_ACT_TU_801E6D48_H
#define GAME_ACT_TU_801E6D48_H

// Code: game/tu_801E6D48.cpp. An actor named after its vtable (0x804A5418),
// derived from cls_804A61C8 (game/act/tu_80210630.h). Its constructor
// (0x801E78F0) calls the base's, stores this vtable and sets the three fields
// below; its update() is slot 0x14 and runs the base update() unless mFlags
// has the 0x8000 bit set.

#include "game/act/tu_80210630.h"

class cls_804A5418 : public cls_804A61C8 {
public:
    // 0x801E78F0
    cls_804A5418();
    /* 0x14 */ virtual void update();

    /* 0x2AC */ s32 field_0x2ac;
    /* 0x2B0 */ s16 field_0x2b0;
    /* 0x2B4 */ s32 field_0x2b4;
}; // Size: 0x2B8

#endif
