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
    // Vtable slot 0x4C (fn_801BA88C): tile code 0xD (fn_8022461C) blocks
    // the position outright; anything else falls to ActorBase's test.
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    // Vtable slot 0x68 (0x80340650, in act_zora_lotus.cpp): returns 0, so
    // no player or actor hit mask is reported (EnemyBase's vfunc_0x68 is
    // the bitmask hook).
    /* 0x68 */ virtual u8 vfunc_0x68();
    // Vtable slot 0x5C (0x80340660, in act_zora_lotus.cpp): returns false,
    // so no player hit is reported (EnemyBase's vfunc_0x5C is that check).
    /* 0x5C */ virtual bool vfunc_0x5C();
    // Vtable slot 0x60 (0x80340658, in act_zora_lotus.cpp): returns false,
    // so no actor hit is reported (EnemyBase's vfunc_0x60 is the actor-hit
    // check that ENEMY_FLAG_HIT_ACTOR records).
    /* 0x60 */ virtual bool vfunc_0x60();
    // Vtable slot 0xA4, the first slot this class adds (0x80340BC8, in
    // act_zora_lotus.cpp): clears the flags below and puts mPos.y back at
    // mSpawnPos.y.
    /* 0xA4 */ virtual void vfunc_0xA4();

    // An effect emitter: stored from a spawn (0x801BAF6C), read and cleared
    // by the destructor (0x801BB30C), which passes it to fn_801EE434.
    /* 0x2A8 */ JPABaseEmitter* field_0x2a8;
    /* 0x2AC */ u8 field_0x2ac;
    /* 0x2AD */ u8 field_0x2ad;
    /* 0x2AE */ u8 field_0x2ae;
    /* 0x2AF */ u8 field_0x2af;
    /* 0x2B0 */ u8 field_0x2b0;
    /* 0x2B2 */ u16 field_0x2b2;
};

#endif
