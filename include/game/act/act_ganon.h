#ifndef GAME_ACT_ACT_GANON_H
#define GAME_ACT_ACT_GANON_H

// Code: game/act_ganon.cpp.

#include "game/ActorBase.h"

class JPABaseEmitter;

// The 'GNON' actor (Ganon, docs/names/actors.tsv, high confidence; the spawner's case for
// ACTOR_GNON). Named after its vtable, 0x804A25F0.
class cls_804A25F0 : public ActorBase_804A5770 {
public:
    cls_804A25F0() {}
    /* 0x08 */ virtual ~cls_804A25F0();
    // fn_80192038: plays sound 0x40006 and re-issues vfunc_0x28 with 2 and (0, 0).
    /* 0x24 */ virtual void vfunc_0x24();

    /* 0x230 */ u8 field_0x230[0x328 - 0x230]; // not yet known
    /* 0x328 */ JPABaseEmitter* mEmitters[5]; // freed by the destructor with fn_801EE434
}; // Size: 0x33C

#endif
