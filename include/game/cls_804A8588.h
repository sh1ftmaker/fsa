#ifndef GAME_CLS_804A8588_H
#define GAME_CLS_804A8588_H

// Code: game/tu_802B214C.cpp. An ActorBase whose vtable is lbl_804A8588 (the
// spawner's constructor, 0x802033B8, stores it; game_units.tsv names it SKUL,
// low confidence). Named after its vtable.
#include "game/ActorBase.h"

class RotatingSkull_804A8588 : public ActorBase_804A5770 {
public:
    RotatingSkull_804A8588() {}
    /* 0x08 */ virtual ~RotatingSkull_804A8588();
    /* 0x0C */ virtual void init();

    // Calls vfunc_0x4C(layer, pos); clears field_0x27c when it returns false.
    u8 fn_802B21AC(s32 layer, const JGeometry::TVec3f* pos);

    /* 0x14 */ virtual void update();

    // Gated by field_0x25c: drawSprite_801F06F0 on "enemy_both2_sob.bin", the shadow,
    // then fn_803075AC.
    /* 0x20 */ virtual void fn_802B2728();

    // 0x802B21EC (a plain `fn_802B21EC` symbol): moves by field_0x3c and field_0x270, bouncing
    // off walls (field_0x27c = 1 while field_0x270 is in use).
    void fn_802B21EC();

    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ JGeometry::TVec3f field_0x234; // copy of mPos, made by init
    /* 0x240 */ s32 field_0x240; // (field_0x90 >> 7) & 0x1F; 1 and 2 pick init's branches
    /* 0x244 */ s32 field_0x244; // update: the angle around field_0x234 (kind 0)
    /* 0x248 */ s32 field_0x248; // field_0x90 & 0x7F
    /* 0x24C */ s32 field_0x24c; // (field_0x90 >> 12) & 0x1F
    /* 0x250 */ s32 field_0x250; // 4 after init
    /* 0x254 */ s32 field_0x254; // 0 after init
    /* 0x258 */ s32 field_0x258; // sprite id base: 0x78 is added to it
    /* 0x25C */ s32 field_0x25c; // gate for fn_802B2728; 1 when field_0x240 != 0
    /* 0x260 */ JGeometry::TVec3f field_0x260; // update: the velocity field_0x3c eases toward (kind 1)
    /* 0x26C */ f32 field_0x26c;
    /* 0x270 */ JGeometry::TVec3f field_0x270; // zero after init; the second velocity of fn_802B21EC
    /* 0x27C */ u8 field_0x27c; // flag: fn_802B21AC clears it, init sets it to 1
};

#endif
