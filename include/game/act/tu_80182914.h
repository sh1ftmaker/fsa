#ifndef GAME_ACT_TU_80182914_H
#define GAME_ACT_TU_80182914_H

// Code: game/tu_80182914.cpp.

#include "game/ActorBase.h"
#include "JSystem/JUtility/TColor.h"

// 0x8018330C, in this unit: the colour of lbl_8053AB10->0x2C (cls_804A0160).
extern "C" JUtility::TColor fn_8018330C();

// Named after its vtable, 0x804A1718.
class cls_804A1718 : public ActorBase_804A5770 {
public:
    cls_804A1718() {}
    /* 0x08 */ virtual ~cls_804A1718();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x18 */ virtual void vfunc_0x18();
    /* 0x20 */ virtual void draw();
    /* 0x44 */ virtual void vfunc_0x44();
    /* 0x230 */ s32 field_0x230;   // state: switch in update(); 0 from init, vfunc_0x44 returns early in 5
    /* 0x234 */ JUtility::TColor field_0x234; // colour, from fn_8018330C in init
    /* 0x238 */ u8 field_0x238[3];  // red, green, blue of field_0x234 (init)
    /* 0x23C */ s32 field_0x23c;   // draw skips the sprite in states 0 and 1 while 0
    /* 0x240 */ s32 field_0x240;   // zeroed by init
    /* 0x244 */ s32 field_0x244;   // zeroed by init
    /* 0x248 */ s32 field_0x248;   // zeroed by init
    /* 0x24C */ s32 field_0x24c;   // zeroed by init
    /* 0x250 */ s32 field_0x250;   // zeroed by init
    /* 0x254 */ s32 field_0x254;   // 6 once vfunc_0x44 has run; draw's frame
    /* 0x258 */ s32 field_0x258;   // vfunc_0x44 does nothing while this is set
    /* 0x25C */ s32 field_0x25c;   // init: bit 0 of field_0x90; draw's frame flag
    /* 0x260 */ s32 field_0x260;   // init: bit 8 of field_0x90
    /* 0x264 */ u16 field_0x264;   // zeroed by init; angle, stepped by fn_801F714C in update
    /* 0x266 */ u8 field_0x266[2]; // not yet known
    /* 0x268 */ JGeometry::TVec3f field_0x268; // zeroed by init
    /* 0x274 */ JPABaseEmitter* mEmitter274; // released by the destructor
}; // Size: 0x278

#endif
