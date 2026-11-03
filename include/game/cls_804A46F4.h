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
    // game/tu_801D11FC.cpp (not written yet; act_thief calls it too). Picks
    // the way toward the player in field_0x338 (fn_801D0B44, fn_801CF888
    // on field_0x32c); returns 0 or 1.
    bool fn_801D12CC();

    /* 0x2A8 */ u8 field_0x2a8[0x32C - 0x2A8];
    /* 0x32C */ JGeometry::TVec3f field_0x32c;
    /* 0x338 */ s32 field_0x338; // a player index, or -1
    /* 0x33C */ s16 field_0x33c; // an angle
    /* 0x33E */ u8 field_0x33e[0x340 - 0x33E];
}; // Size: at least 0x340

#endif
