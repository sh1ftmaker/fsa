#ifndef GAME_ACT_ACT_BOMBCHU_H
#define GAME_ACT_ACT_BOMBCHU_H

// Code: game/act_bombchu.cpp.

#include "game/ActorBase.h"

// The BMTY actor ("Bombchu", docs/names/actors.tsv, high confidence): its
// constructor stores this vtable (config/G4SE01/game_units.tsv). Named after
// its vtable, 0x804AB464. Its vtable is ActorBase's 0x54 bytes plus two
// slots, and its destructor calls ~ActorBase directly, so the base is
// ActorBase, not EnemyBase.
class Bombchu_804AB464 : public ActorBase_804A5770 {
public:
    // 0x8032500C: r4 = the delete flag; > 0 frees the object (__dl__FPv).
    /* 0x08 */ virtual ~Bombchu_804AB464();

    /* 0x48 */ virtual void vfunc_0x48();
    /* 0x54 */ virtual void vfunc_0x54();

    /* 0x230 */ s32 field_0x230; // vfunc_0x48 sets it to 3; fn_801CC8E4 tests != 3 (cmpwi)
};

#endif
