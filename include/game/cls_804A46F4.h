#ifndef GAME_CLS_804A46F4_H
#define GAME_CLS_804A46F4_H

#include "game/EnemyBase.h"

// The shared base of the Shadow Link actors and act_thief: vtable 0x804A46F4.
// Its constructor fn_801D1974 (game/tu_801D11FC.cpp) calls EnemyBase's, stores
// this vtable and initialises 0x2A8-0x33C; its destructor dtor_801C5854
// stores it and then EnemyBase's (act_thief shares it). Named after its
// vtable.
class cls_804A46F4 : public EnemyBase_804A4808 {
public:
    /* 0x2A8 */ u8 field_0x2a8[0x32C - 0x2A8];
    /* 0x32C */ JGeometry::TVec3f field_0x32c;
    /* 0x338 */ u8 field_0x338[0x340 - 0x338];
}; // Size: at least 0x340

#endif
