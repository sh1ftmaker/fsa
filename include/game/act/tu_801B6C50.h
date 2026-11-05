#ifndef GAME_ACT_TU_801B6C50_H
#define GAME_ACT_TU_801B6C50_H

// Code: game/tu_801B6C50.cpp.

#include "game/ActorBase.h"

// Spawned for 'GLWP'; moved here from game/act/tu_801F9484.h.
class cls_804A39DC : public ActorBase_804A5770 {
public:
    cls_804A39DC() {}
    /* 0x08 */ virtual ~cls_804A39DC();
    /* 0x230 */ u8 field_0x230[0x34];
    /* 0x264 */ JGeometry::TVec3f field_0x264[4];
    /* 0x294 */ JPABaseEmitter* mEmitters[4]; // released by the destructor
}; // Size: 0x2A4

#endif
