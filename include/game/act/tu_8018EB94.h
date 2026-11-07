#ifndef GAME_ACT_TU_8018EB94_H
#define GAME_ACT_TU_8018EB94_H

// Code: game/tu_8018EB94.cpp.

#include "game/ActorBase.h"

// Spawned for 'DGT2'; moved here from game/act/tu_801F9484.h.
class cls_804A1FA0 : public ActorBase_804A5770 {
public:
    cls_804A1FA0() {}
    /* 0x08 */ virtual ~cls_804A1FA0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    // Slot 0x44: stops the three emitters and forgets them.
    /* 0x44 */ virtual void vfunc_0x44();
    /* 0x230 */ u8 field_0x230[0x58];
    /* 0x288 */ JGeometry::TVec3f field_0x288[6];
    /* 0x2D0 */ JGeometry::TVec3f field_0x2d0[20];
    /* 0x3C0 */ JPABaseEmitter* mEmitters[3]; // released by the destructor
}; // Size: 0x3CC

#endif
