#ifndef GAME_CLS_804A8638_H
#define GAME_CLS_804A8638_H

// Code: game/tu_802B3674.cpp. An ActorBase whose vtable is lbl_804A8638 (its
// constructor, fn_802B51C0, stores it). No actor id is known for it, so it is
// named after the vtable.
#include "game/ActorBase.h"

class JPABaseEmitter;

class cls_804A8638 : public ActorBase_804A5770 {
public:
    /* 0x10 */ virtual void vfunc_0x10();
    /* 0x20 */ virtual void draw();
    /* 0x54 */ virtual void vfunc_0x54(s32 a, s32 b, s32 c, s32 d, s32 e, s32 state);

    /* 0x230 */ u8 field_0x230[0x234 - 0x230]; // not yet identified
    /* 0x234 */ JPABaseEmitter* field_0x234; // emitter, deleted by vfunc_0x10
    /* 0x238 */ JPABaseEmitter* field_0x238; // emitter, deleted by vfunc_0x10
    /* 0x23C */ JPABaseEmitter* mEmitter;
    /* 0x240 */ s32 field_0x240; // state; set to 0xC when the 0x310 index is below 0x314
    /* 0x244 */ u8 field_0x244[0x288 - 0x244]; // not yet identified
    /* 0x288 */ s32 field_0x288; // state; vfunc_0x10 acts on 4 and resets it to 0
    /* 0x28C */ u8 field_0x28c[0x290 - 0x28C]; // not yet identified
    /* 0x290 */ s32 field_0x290;
    /* 0x294 */ u8 field_0x294[0x2E0 - 0x294]; // not yet identified
    /* 0x2E0 */ s32 field_0x2e0;
    /* 0x2E4 */ s32 field_0x2e4;
    /* 0x2E8 */ s32 field_0x2e8;
    /* 0x2EC */ s32 field_0x2ec;
    /* 0x2F0 */ s32 field_0x2f0;
    /* 0x2F4 */ s32 field_0x2f4;
    /* 0x2F8 */ s32 field_0x2f8;
    /* 0x2FC */ u8 field_0x2fc[0x300 - 0x2FC]; // not yet identified
    /* 0x300 */ s32 field_0x300; // copied to 0x2E0..0x2E8 by vfunc_0x10
    /* 0x304 */ s32 field_0x304;
    /* 0x308 */ s32 field_0x308;
    /* 0x30C */ u8 field_0x30c[0x310 - 0x30C]; // not yet identified
    /* 0x310 */ s32 field_0x310;
    /* 0x314 */ s32 field_0x314;
    /* 0x318 */ s32 field_0x318;
    /* 0x31C */ s32 field_0x31c;
    /* 0x320 */ s32 field_0x320;
};

#endif
