#ifndef GAME_ACT_TU_803D9944_H
#define GAME_ACT_TU_803D9944_H

// Code: game/tu_803D9944.cpp.

#include "game/ActorBase.h"

// Spawned for 'TNGL'; moved here from game/act/tu_801F9484.h.
class cls_804B0478 : public ActorBase_804A5770 {
public:
    cls_804B0478() {}
    /* 0x08 */ virtual ~cls_804B0478();
    /* 0x230 */ u8 field_0x230[0x21];
    /* 0x251 */ u8 field_0x251; // stb in tu_803D9944; the player's state 0x34 waits while it is clear
    /* 0x252 */ u8 field_0x252[0xE];
    /* 0x260 */ JGeometry::TVec3f field_0x260[4];
    /* 0x290 */ JPABaseEmitter* mEmitters[4]; // released by the destructor
}; // Size: 0x2A0

#endif
