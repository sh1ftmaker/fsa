#ifndef GAME_ACT_TU_802957CC_H
#define GAME_ACT_TU_802957CC_H

// Code: game/tu_802957CC.cpp. The spawner (game/tu_801F9484.cpp, fn_802019BC)
// constructs it and stores this vtable, 0x804A7AE0.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A7AE0 (__vt__12cls_804A7AE0). Its size is
// 0x3F8: the spawner allocates 0x3F8 bytes for it.
class cls_804A7AE0 : public ActorBase_804A5770 {
public:
    /* 0x08 */ virtual ~cls_804A7AE0();

    /* 0x230 */ u8 field_0x230[0x2FC - 0x230]; // not yet known
    /* 0x2FC */ JPABaseEmitter* mEmitters[2][2]; // released by the destructor
    /* 0x30C */ u8 field_0x30c[0x310 - 0x30C]; // not yet known
    /* 0x310 */ JPABaseEmitter* mEmitters2[4]; // released by the destructor
    /* 0x320 */ u8 field_0x320[0x3F8 - 0x320]; // not yet known
}; // Size: 0x3F8

#endif
