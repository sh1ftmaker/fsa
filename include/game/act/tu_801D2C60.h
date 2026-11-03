#ifndef GAME_ACT_TU_801D2C60_H
#define GAME_ACT_TU_801D2C60_H

// Code: game/tu_801D2C60.cpp (owner unit, not written yet).
// A sub-base of EnemyBase_804A4808, named after its vtable (0x804A48B0). Its
// own functions (0x801D2C60-0x801D30FC) override the destructor
// (dtor_801D2C60), init (0x801D2FEC), update (0x801D2E70) and draw
// (0x801D2D38); the constructor is 0x801D30B8. Derived classes
// (tu_802201D8's 0x804A6828, two more in tu_801F9484's spawner) call these
// qualified and inline its destructor. 

#include "game/EnemyBase.h"

class cls_804A48B0 : public EnemyBase_804A4808 {
public:
    cls_804A48B0();
    virtual ~cls_804A48B0() {}
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    /* 0x2A8 */ u8 field_0x2a8[4]; // unknown
}; // Size: 0x2AC

#endif
