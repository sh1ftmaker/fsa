#ifndef GAME_ACT_ACT_FLAME_H
#define GAME_ACT_ACT_FLAME_H

// Code: game/act_flame.cpp.

#include "game/ActorBase.h"

// Spawned for 'FLME'; moved here from game/act/tu_801F9484.h.
class cls_804A0330 : public ActorBase_804A5770 {
public:
    cls_804A0330() {}
    /* 0x08 */ virtual ~cls_804A0330();
    /* 0x230 */ u8 field_0x230[0x34];
    /* 0x264 */ JPABaseEmitter* mEmitters[4]; // released by the destructor
}; // Size: 0x274

#endif
