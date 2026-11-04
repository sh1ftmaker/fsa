#ifndef GAME_ACT_ACT_INEXTINGUISHABLE_FLAME_H
#define GAME_ACT_ACT_INEXTINGUISHABLE_FLAME_H

// Code: game/act_inextinguishable_flame.cpp.

#include "game/ActorBase.h"

// Actor BLZ2 ("Inextinguishable flame", docs/names/actors.tsv): the spawner
// stores this vtable, 0x804A9D28. Named after that vtable. The spawn field
// field_0x90 gives a grid of flames: X and Y counts in bits 22-26 and 17-21
// (each +1), and the flags below.
class InextinguishableFlame_804A9D28 : public ActorBase_804A5770 {
public:
    InextinguishableFlame_804A9D28() {}
    /* 0x08 */ virtual ~InextinguishableFlame_804A9D28();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x24 */ virtual void vfunc_0x24();
    /* 0x54 */ virtual void vfunc_0x54();
    /* 0x58 */ virtual bool vfunc_0x58(JGeometry::TVec3f* pos);

    // Three rows of emitters, one row per flame kind, each as big as the grid.
    /* 0x230 */ JPABaseEmitter* mEmitters[3][16];
    /* 0x2F0 */ s32 mState; // 0: off, 1, 2 (drawn), 6
    /* 0x2F4 */ s32 mCountX; // grid width: (field_0x90 >> 22 & 31) + 1
    /* 0x2F8 */ s32 mCountY; // grid height: (field_0x90 >> 17 & 31) + 1
    /* 0x2FC */ s32 mKind;   // field_0x90 & 0xF
    /* 0x300 */ s32 mFlag300; // field_0x90 bit 4
    /* 0x304 */ s32 mTimer;     // counts down in states 4 and 5 (60 at 2, 40 at 1)
    /* 0x308 */ s32 mViewBit;   // field_0x90 >> 12 & 31, a GameView bit
}; // Size: at least 0x30C

#endif
