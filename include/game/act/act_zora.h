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
    // once mState is set, the camera collects mPos (addFocusPoint_8030C210) so that it
    // stays in view.
    /* 0xA0 */ virtual void vfunc_0xA0();
    // Vtable slot 0x4C (fn_801BA88C): tile code 0xD (fn_8022461C) blocks
    // the position outright; anything else falls to ActorBase's test.
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    // Slots 0xA4-0xB0, the four this class appends. ZoraEnemy's own are
    // empty (0xAC returns false); the lotus Zora (cls_804AC3E0,
    // act_zora_lotus.cpp) overrides all four.
    /* 0xA4 */ virtual void vfunc_0xA4();
    /* 0xA8 */ virtual void vfunc_0xA8();
    /* 0xAC */ virtual bool vfunc_0xAC();
    /* 0xB0 */ virtual void vfunc_0xB0();

    // An effect emitter: stored from a spawn (0x801BAF6C), read and cleared
    // by the destructor (0x801BB30C), which passes it to fn_801EE434.
    /* 0x2A8 */ JPABaseEmitter* field_0x2a8;
}; // Size: 0x2AC (the spawner's __nw__; the lotus Zora adds its flags after it)

#endif
