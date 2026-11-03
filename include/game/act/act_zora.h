#ifndef GAME_ACT_ACT_ZORA_H
#define GAME_ACT_ACT_ZORA_H

// Code: game/act_zora.cpp.

#include "game/EnemyBase.h"

// The Zora enemy ('ZORA', docs/names/actors.tsv, high confidence). Named
// after its vtable, 0x804A3D7C (a sub-base of EnemyBase_804A4808 that
// appends 4 slots after 0xA0). Its constructor (0x801BB3B0) calls
// EnemyBase's, stores this vtable and clears field_0x2a8, the first field
// past EnemyBase's 0x2A8 bytes.
class ZoraEnemy_804A3D7C : public EnemyBase_804A4808 {
public:
    ZoraEnemy_804A3D7C();
    virtual ~ZoraEnemy_804A3D7C();
    // Vtable slot 0xA0 (fn_801BA854, overriding EnemyBase's vfunc_0xA0):
    // once mState is set, the camera collects mPos (fn_8030C210) so that it
    // stays in view.
    /* 0xA0 */ virtual void vfunc_0xA0();

    // Read and written by other Zora functions too (0x801BAF6C, 0x801BB338,
    // 0x801BB348); the type is not yet known.
    /* 0x2A8 */ s32 field_0x2a8;
};

#endif
