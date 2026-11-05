#ifndef GAME_CLS_804A8588_H
#define GAME_CLS_804A8588_H

// Code: game/tu_802B214C.cpp. An ActorBase whose vtable is lbl_804A8588 (the
// spawner's constructor, 0x802033B8, stores it; game_units.tsv names it SKUL,
// low confidence). Named after its vtable.
#include "game/ActorBase.h"

class cls_804A8588 : public ActorBase_804A5770 {
public:
    cls_804A8588() {}
    /* 0x08 */ virtual ~cls_804A8588();
    /* 0x0C */ virtual void init();

    // Calls vfunc_0x4C(layer, pos); clears field_0x27c when it returns false.
    u8 fn_802B21AC(s32 layer, const JGeometry::TVec3f* pos);

    // Gated by field_0x25c: drawSprite_801F06F0 on "enemy_both2_sob.bin", the shadow,
    // then fn_803075AC.
    void fn_802B2728();

    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ JGeometry::TVec3f field_0x234; // copy of mPos, made by init
    /* 0x240 */ s32 field_0x240; // (field_0x90 >> 7) & 0x1F; 1 and 2 pick init's branches
    /* 0x244 */ u8 field_0x244[0x248 - 0x244]; // not yet identified
    /* 0x248 */ s32 field_0x248; // field_0x90 & 0x7F
    /* 0x24C */ s32 field_0x24c; // (field_0x90 >> 12) & 0x1F
    /* 0x250 */ s32 field_0x250; // 4 after init
    /* 0x254 */ s32 field_0x254; // 0 after init
    /* 0x258 */ s32 field_0x258; // sprite id base: 0x78 is added to it
    /* 0x25C */ s32 field_0x25c; // gate for fn_802B2728; 1 when field_0x240 != 0
    /* 0x260 */ f32 field_0x260;
    /* 0x264 */ f32 field_0x264;
    /* 0x268 */ u8 field_0x268[0x26C - 0x268]; // not yet identified
    /* 0x26C */ f32 field_0x26c;
    /* 0x270 */ JGeometry::TVec3f field_0x270; // zero after init
    /* 0x27C */ u8 field_0x27c; // flag: fn_802B21AC clears it, init sets it to 1
};

#endif
