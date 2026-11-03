#ifndef GAME_ACT_TU_801E801C_H
#define GAME_ACT_TU_801E801C_H

// Code: game/tu_801E801C.cpp.

#include "game/EnemyBase.h"

// Named after its vtable, 0x804A5568.
class cls_804A5568 : public EnemyBase_804A4808 {
public:
    cls_804A5568() {}
    /* 0x08 */ virtual ~cls_804A5568();
    /* 0x0C */ virtual void init();
    /* 0x5C */ virtual bool vfunc_0x5C();
    /* 0x60 */ virtual bool vfunc_0x60();
    /* 0x68 */ virtual u8 vfunc_0x68();
    /* 0x90 */ virtual void vfunc_0x90(u8 mapHit);
    /* 0xA0 */ virtual void vfunc_0xA0();
    /* 0x2A8 */ u8 field_0x2a8[0x118]; // not yet known
    /* 0x3C0 */ JPABaseEmitter* mEmitter3c0; // released by the destructor
}; // Size: 0x3C4

#endif
