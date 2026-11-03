#ifndef GAME_ACT_TU_801BEDBC_H
#define GAME_ACT_TU_801BEDBC_H

// Code: game/tu_801BEDBC.cpp.

#include "game/EnemyBase.h"

// Named after its vtable, 0x804A4188.
class cls_804A4188 : public EnemyBase_804A4808 {
public:
    cls_804A4188() {}
    /* 0x08 */ virtual ~cls_804A4188();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    /* 0x54 */ virtual void vfunc_0x54();
    /* 0x2A8 */ JPABaseEmitter* mEmitter2a8; // released by the destructor
}; // Size: 0x2AC

#endif
