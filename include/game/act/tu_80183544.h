#ifndef GAME_ACT_TU_80183544_H
#define GAME_ACT_TU_80183544_H

// Code: game/tu_80183544.cpp.

#include "game/ActorBase.h"
#include "JSystem/JUtility/TColor.h"

// Named after its vtable, 0x804A1790.
class cls_804A1790 : public ActorBase_804A5770 {
public:
    cls_804A1790() : field_0x234(-1) {}
    /* 0x08 */ virtual ~cls_804A1790();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x18 */ virtual void vfunc_0x18();
    /* 0x20 */ virtual void draw();
    /* 0x44 */ virtual void vfunc_0x44();
    /* 0x230 */ s32 field_0x230; // state: switch in update()
    /* 0x234 */ JUtility::TColor field_0x234; // colour, from fn_8018330C in init (-1 until then)
    /* 0x238 */ u8 field_0x238[3]; // red, green, blue of field_0x234
    /* 0x23B */ u8 field_0x23b;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ s32 field_0x254; // draw's frame
    /* 0x258 */ s32 field_0x258;
    /* 0x25C */ s32 field_0x25c; // init: bit 8 of field_0x90
    /* 0x260 */ u16 field_0x260; // angle
    /* 0x262 */ u8 field_0x262[2];
    /* 0x264 */ JGeometry::TVec3f field_0x264;
    /* 0x270 */ JPABaseEmitter* mEmitter270; // released by the destructor
}; // Size: 0x274

#endif
