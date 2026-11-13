#ifndef GAME_CLS_80214244_H
#define GAME_CLS_80214244_H

#include "types.h"
#include "JSystem/JGeometry.h"
#include "game/RailBase.h"

// A small container, defined in game/tu_80214244.cpp. No vtable; named
// after its first method. Two 0xC8-byte elements plus a current-index byte,
// embedded in the Shadow Link actor (act_shadow_link_2, +0x358).
class cls_80214244 {
public:
    cls_80214244() { fn_802142A8(); }
    // The current element.
    cls_800FE3EC* fn_80214244();
    void fn_80214254(u8);
    void fn_8021425C(u8, u8, const JGeometry::TVec3f&, int);
    void fn_802142A8();

    /* 0x000 */ cls_800FE3EC field_0x0[2];
    /* 0x190 */ u8 field_0x190;
}; // Size: 0x194

#endif
