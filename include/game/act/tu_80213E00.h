#ifndef GAME_ACT_TU_80213E00_H
#define GAME_ACT_TU_80213E00_H

#include "types.h"
#include "game/EnemyBase.h"

// An EnemyBase_804A4808 that draws one sprite cel (animation table
// lbl_8046A578, four cels of 3 frames) and never moves; code in
// game/tu_80213E00.cpp. Named after its vtable (0x804A6400), whose overrides
// are the destructor, init, draw, vfunc_0x88 and vfunc_0x90. It adds no
// fields.
// Size: 0x2A8
class cls_804A6400 : public EnemyBase_804A4808 {
public:
    cls_804A6400();
    virtual ~cls_804A6400();
    /* 0x0C */ virtual void init();
    /* 0x20 */ virtual void draw();
    // Zeroes field_0x3c (the base turns mSpeed into a velocity here).
    /* 0x88 */ virtual void vfunc_0x88();
    // Empty (the base's runs the per-state behaviour).
    /* 0x90 */ virtual void vfunc_0x90(u8 mapHit);
};

#endif
