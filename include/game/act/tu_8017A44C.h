#ifndef GAME_ACT_TU_8017A44C_H
#define GAME_ACT_TU_8017A44C_H

// Code: game/tu_8017A44C.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A1160.
class cls_804A1160 : public ActorBase_804A5770 {
public:
    cls_804A1160() {}
    /* 0x08 */ virtual ~cls_804A1160();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    /* 0x230 */ s32 field_0x230;      // zeroed by init (else branch)
    /* 0x234 */ u8 field_0x234;       // zeroed by init (else branch)
    /* 0x238 */ s32 field_0x238;      // zeroed by init (else branch)
    /* 0x23C */ s32 field_0x23c;      // zeroed by init; 2 in the else branch
    /* 0x240 */ s32 field_0x240;      // zeroed by init
    /* 0x244 */ s32 field_0x244;      // zeroed by init
    /* 0x248 */ s32 field_0x248;      // zeroed by init
    /* 0x24C */ s32 field_0x24c;      // zeroed by init
    /* 0x250 */ s32 field_0x250;      // zeroed by init
    /* 0x254 */ s32 field_0x254;      // zeroed by init (else branch)
    /* 0x258 */ s32 field_0x258;      // player: low nibble of the 0x90 word; the Player.h calls take it
    /* 0x25C */ s32 field_0x25c;      // bits 8-11 of the 0x90 word
}; // Size: 0x260

#endif
