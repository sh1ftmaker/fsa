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
    /* 0x230 */ u8 field_0x230[0x48];
    /* 0x278 */ JPABaseEmitter* mEmitters[2]; // released by the destructor
}; // Size: 0x280

#endif
