#ifndef GAME_ACT_ACT_PHANTOM_GANON_2_H
#define GAME_ACT_ACT_PHANTOM_GANON_2_H

// Code: game/act_phantom_ganon_2.cpp.

#include "game/ActorBase.h"

// The FGN2 actor (Phantom Ganon 2, docs/names/actors.tsv, high confidence;
// the spawner's case for ACTOR_FGN2). Named after its vtable, 0x804A1A98.
class cls_804A1A98 : public ActorBase_804A5770 {
public:
    cls_804A1A98() {}
    /* 0x08 */ virtual ~cls_804A1A98();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x24 */ virtual void vfunc_0x24();

    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ u8 field_0x238[0x2ee - 0x238]; // not yet known
    /* 0x2EE */ s8 field_0x2ee;
    /* 0x2EF */ u8 field_0x2ef[0x318 - 0x2EF]; // not yet known
}; // Size: 0x318 (the spawner's __nw__)

#endif
