#ifndef GAME_ACT_ACT_ARROW_H
#define GAME_ACT_ACT_ARROW_H

// Code: game/act_arrow.cpp.

#include "game/ActorBase.h"

// Spawned for 'ARRW' (Arrow (player), docs/names/actors.tsv, high confidence);
// moved here from game/act/tu_801F9484.h. Named after its vtable, 0x804A05E0.
class ArrowPlayer_804A05E0 : public ActorBase_804A5770 {
public:
    ArrowPlayer_804A05E0() {}
    /* 0x08 */ virtual ~ArrowPlayer_804A05E0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    /* 0x230 */ s32 field_0x230; // lwz / stw in act_arrow (tested 0, 4); 0 lets the player's state 0x43 go on (tu_80256C74 fn_8025AB00)
    /* 0x234 */ u8 field_0x234[0x38];
    // SHARED-EDIT: tested by the ZLD2 actor's vfunc_0x44 (game/tu_80197070.cpp).
    /* 0x26C */ s32 field_0x26c;
    /* 0x270 */ u8 field_0x270[8];
    /* 0x278 */ JPABaseEmitter* mEmitters[2]; // released by the destructor
}; // Size: 0x280

#endif
