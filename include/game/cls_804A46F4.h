#ifndef GAME_CLS_804A46F4_H
#define GAME_CLS_804A46F4_H

#include "game/EnemyBase.h"

// The shared base of the Shadow Link actors and act_thief: vtable 0x804A46F4.
// Its constructor __ct__12cls_804A46F4Fv (0x801D1974, game/tu_801D11FC.cpp) calls EnemyBase's, stores
// this vtable and initialises 0x2A8-0x33C; its destructor dtor_801C5854
// stores it and then EnemyBase's (act_thief shares it). Named after its
// vtable.
class cls_804A46F4 : public EnemyBase_804A4808 {
public:
    // game/tu_801D11FC.cpp (0x801D1974): EnemyBase's constructor, this
    // vtable, the waypoint list emptied, field_0x32c zeroed, no player.
    cls_804A46F4();

    // game/tu_801D11FC.cpp (0x801D11FC): turns field_0x33c by 0x5B0 either
    // way, toward the angle of mMapHitDir (x, y).
    void fn_801D11FC();
    // game/tu_801D11FC.cpp (0x801D183C): whether player field_0x338 (0-3,
    // fn_801F320C) is within `range` (squared against; 0 = any) of mPos and
    // the way to it is blocked (fn_801CF888); 0 or 1.
    bool fn_801D183C(f32 range);
    // game/tu_801D11FC.cpp. Picks the way toward the player in field_0x338
    // (fn_801D0B44, fn_801CF888) as a list of waypoints, mPath[0] ends up in
    // field_0x32c; returns 0 or 1.
    bool fn_801D12CC();

    /* 0x2A8 */ s32 mPathCount;
    /* 0x2AC */ JGeometry::TVec2f mPath[16]; // waypoints (x, y)
    /* 0x32C */ JGeometry::TVec3f field_0x32c;
    /* 0x338 */ s32 field_0x338; // a player index, or -1
    /* 0x33C */ s16 field_0x33c; // an angle
    /* 0x33E */ u8 field_0x33e[0x340 - 0x33E];
}; // Size: at least 0x340

#endif
