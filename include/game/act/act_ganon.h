#ifndef GAME_ACT_ACT_GANON_H
#define GAME_ACT_ACT_GANON_H

// Code: game/act_ganon.cpp.

#include "game/ActorBase.h"

class JPABaseEmitter;

// The 'GNON' actor (Ganon, docs/names/actors.tsv, high confidence; the spawner's case for
// ACTOR_GNON). Named after its vtable, 0x804A25F0.
class Ganon_804A25F0 : public ActorBase_804A5770 {
public:
    Ganon_804A25F0() {}
    /* 0x08 */ virtual ~Ganon_804A25F0();
    // fn_80192038: plays sound 0x40006 and re-issues vfunc_0x28 with 2 and (0, 0).
    /* 0x24 */ virtual void vfunc_0x24();
    // 0x8019CEBC / 0x8019CEC4 (game/tu_80197070.cpp): store a word at 0x28C / 0x2F8.
    void fn_8019CEBC(s32 value);
    void fn_8019CEC4(s32 value);

    /* 0x230 */ u8 field_0x230[0x28C - 0x230]; // not yet known
    // SHARED-EDIT: set by fn_8019CEBC (game/tu_80197070.cpp; the ZLD2 actor's update).
    /* 0x28C */ s32 field_0x28c;
    /* 0x290 */ u8 field_0x290[0x2F8 - 0x290]; // not yet known
    // SHARED-EDIT: set by fn_8019CEC4 (game/tu_80197070.cpp).
    /* 0x2F8 */ s32 field_0x2f8;
    /* 0x2FC */ u8 field_0x2fc[0x328 - 0x2FC]; // not yet known
    /* 0x328 */ JPABaseEmitter* mEmitters[5]; // freed by the destructor with releaseEmitter_801EE434
}; // Size: 0x33C

#endif
