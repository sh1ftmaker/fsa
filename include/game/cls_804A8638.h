#ifndef GAME_CLS_804A8638_H
#define GAME_CLS_804A8638_H

// Code: game/tu_802B3674.cpp. An ActorBase whose vtable is lbl_804A8638 (its
// constructor, fn_802B51C0, stores it). No actor id is known for it, so it is
// named after the vtable.
#include "game/ActorBase.h"

class JPABaseEmitter;

class cls_804A8638 : public ActorBase_804A5770 {
public:
    /* 0x20 */ virtual void draw();

    /* 0x230 */ u8 field_0x230[0x23C - 0x230]; // not yet identified
    /* 0x23C */ JPABaseEmitter* mEmitter;
    /* 0x240 */ u8 field_0x240[0x290 - 0x240]; // not yet identified
    /* 0x290 */ s32 field_0x290;
};

#endif
