#ifndef GAME_ACT_ACT_BEAMOS_H
#define GAME_ACT_ACT_BEAMOS_H

// Code: game/act_beamos.cpp.

#include "game/EnemyBase.h"

// The BMST actor ("Beamos", docs/names/actors.tsv, med confidence): the
// spawner constructs it with __ct__15Beamos_804A40E0Fv (0x801BEB44), which
// calls EnemyBase's constructor, stores this vtable
// (config/G4SE01/game_units.tsv) and zeroes 0x2A8-0x2B8. Named after its
// vtable, 0x804A40E0.
class Beamos_804A40E0 : public EnemyBase_804A4808 {
public:
    Beamos_804A40E0();
    /* 0x08 */ virtual ~Beamos_804A40E0();
    /* 0x0C */ virtual void init();
    /* 0x20 */ virtual void draw();
    /* 0x24 */ virtual void vfunc_0x24();
    /* 0x5C */ virtual u8 vfunc_0x5C();
    /* 0x60 */ virtual u8 vfunc_0x60();
    /* 0x90 */ virtual void vfunc_0x90(u8 mapHit);
    // Sets the state (mState) and runs its entry block: 0 and 3 set field_0x250;
    // 2 fires two effects and a sound and sets field_0x2aa; 3 also calls fn_801BE86C.
    void fn_801BE6C4(s32 state);
    void fn_801BE86C();
    /* 0x98 */ virtual void vfunc_0x98();
    /* 0x9C */ virtual void vfunc_0x9C(u8 mapHit);
    /* 0x2A8 */ s16 field_0x2a8; // zeroed by the constructor; a sincos angle (init: random)
    /* 0x2AA */ u16 field_0x2aa; // nonzero: vfunc_0x60 ignores mActorId
    /* 0x2AC */ u8 field_0x2ac;  // zeroed by the constructor
    /* 0x2B0 */ s32 field_0x2b0; // zeroed by the constructor
    /* 0x2B4 */ s32 field_0x2b4; // zeroed by the constructor
}; // Size: 0x2B8

#endif
