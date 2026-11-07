#ifndef GAME_ACT_TU_8034C950_H
#define GAME_ACT_TU_8034C950_H

// Code: game/tu_8034C950.cpp.

#include "game/ActorBase.h"
#include "game/act/tu_801F9484_elems.h"

// Named after its vtable, 0x804AC950.
class cls_804AC950 : public ActorBase_804A5770 {
public:
    cls_804AC950() {}
    /* 0x08 */ virtual ~cls_804AC950();
    /* 0x0C */ virtual void init();
    /* 0x10 */ virtual void vfunc_0x10();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x24 */ virtual void vfunc_0x24();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ s32 field_0x234[4]; // -1 after init
    /* 0x244 */ s32 field_0x244[4];
    /* 0x254 */ s32 field_0x254[2]; // -1 after init
    /* 0x25C */ u8 field_0x25c[4];
    /* 0x260 */ Elem_80200A84 field_0x260[8];
    /* 0x320 */ Elem_80200A80 field_0x320[4];
    /* 0x370 */ u8 field_0x370;
    /* 0x371 */ u8 field_0x371[3];
    /* 0x374 */ s32 field_0x374;
    /* 0x378 */ s32 field_0x378; // timer for vfunc_0x4C (fn_801F132C)
    /* 0x37C */ s32 field_0x37c;
    /* 0x380 */ u8 field_0x380[6];
    /* 0x386 */ u8 field_0x386[2];
    /* 0x388 */ s32 field_0x388; // -1 after init
}; // Size: 0x38C

#endif
