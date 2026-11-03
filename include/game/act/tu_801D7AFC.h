#ifndef GAME_ACT_TU_801D7AFC_H
#define GAME_ACT_TU_801D7AFC_H

// Code: game/tu_801D7AFC.cpp.

#include "game/EnemyBase.h"

// Named after its vtable, 0x804A4BB0.
class cls_804A4BB0 : public EnemyBase_804A4808 {
public:
    cls_804A4BB0() {}
    /* 0x08 */ virtual ~cls_804A4BB0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x2A8 */ JPABaseEmitter* mEmitter2a8; // released by the destructor
    /* 0x2AC */ JPABaseEmitter* mEmitter2ac; // released by the destructor
}; // Size: 0x2B0

#endif
