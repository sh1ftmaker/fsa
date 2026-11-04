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
    // 0x801E6D48: the destructor; the vtable stores and base destructors are
    // all inline (cls_804A61C8, EnemyBase), then ActorBase's runs.
    /* 0x08 */ virtual ~cls_804A5418();
    /* 0x10 */ virtual void vfunc_0x10();
    /* 0x14 */ virtual void update();
    // 0x801E6DE8: unless mFlags has 0x8000, or mState is not 0 or 2, puts
    // mPos's (x, y) offset by the direction field_0x98 into a TVec2f and
    // calls fn_801EA108 with it, field_0x2b0 and a static scale of 128.
    void fn_801E6DE8();
    // 0x801E6EAC: with mFlags bit 0x8000, reports field_0x2b4 (if set) to the
    // view and calls fn_801F3668(); else, unless mState is 1, sets state 1.
    void fn_801E6EAC();
    // 0x801E728C: state setter (this in r3); its callers pass 1 or 2.
    void fn_801E728C(int state);
    // 0x801E6F28: unless mFlags has 0x8000, draws through cls_804A61C8's
    // fn_80210630 with bit 0x2000 of mFlags, and field_0x2a8 in state 2.
    /* 0x20 */ virtual void draw();

    /* 0x2AC */ s32 field_0x2ac;
    /* 0x2B0 */ s16 field_0x2b0;
    /* 0x2B4 */ s32 field_0x2b4;
}; // Size: 0x2B8

#endif
