#ifndef GAME_ACT_TU_8012FBCC_H
#define GAME_ACT_TU_8012FBCC_H

// Code: game/tu_8012FBCC.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x8049EA00.
class cls_8049EA00 : public ActorBase_804A5770 {
public:
    cls_8049EA00() : mFlags(0) {}
    /* 0x08 */ virtual ~cls_8049EA00();
    /* 0x0C */ virtual void init();
    // True when there is no escorted woman ('OPGL') actor, or the 2-int
    // vector at v points the wrong way for a flag in mFlags (fn_80130030
    // calls it with an actor of this class).
    bool fn_8012FC2C(const s32* v);
    /* 0x230 */ u32 mFlags; // field_0x90 & 0xF; bit 0/1: v[0] must be
                            // positive/negative, bit 2/3: v[1] must be
                            // positive/negative (else fn_8012FC2C is true)
}; // Size: 0x234

#endif
