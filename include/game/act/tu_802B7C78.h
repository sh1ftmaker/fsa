#ifndef GAME_ACT_TU_802B7C78_H
#define GAME_ACT_TU_802B7C78_H

// Code: game/tu_802B7C78.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A886C.
class cls_804A886C : public ActorBase_804A5770 {
public:
    cls_804A886C() {}
    /* 0x08 */ virtual ~cls_804A886C();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x24 */ virtual void vfunc_0x24();

    // Game/tu_802B7C78.cpp. idx (0 or 1) selects field_0x260 / field_0x268;
    // arg is stored to field_0x230 on the paths that set it.
    void fn_802B7D1C(s32 idx, s32 arg);

    /* 0x230 */ s32 field_0x230; // draw() shows the sprite only when nonzero
    /* 0x234 */ JGeometry::TVec3f field_0x234; // copy of mPos
    /* 0x240 */ u8 field_0x240[0x4]; // not yet known
    /* 0x244 */ s32 field_0x244; // init: 180
    /* 0x248 */ s32 field_0x248; // init: 20
    /* 0x24C */ s32 field_0x24c; // index into lbl_8046CB60 (draw)
    /* 0x250 */ s32 field_0x250; // init: 0
    /* 0x254 */ f32 field_0x254;
    /* 0x258 */ f32 field_0x258;
    /* 0x25C */ u8 field_0x25c[0x4]; // not yet known
    /* 0x260 */ f32 field_0x260[2]; // init: 1.0f, 0.75f; scaled by addCalc2_801F71A4
    /* 0x268 */ f32 field_0x268[2]; // init: 2.0f, 1.5f; multipliers of the direction
    /* 0x270 */ s32 field_0x270; // draw(): low byte is the sprite's alpha
    /* 0x274 */ s32 field_0x274; // init: 0
    /* 0x278 */ u8 field_0x278[0x4]; // not yet known
    /* 0x27C */ s32 field_0x27c; // init: 0
    /* 0x280 */ s32 field_0x280; // init: 0
    /* 0x284 */ JGeometry::TVec2f field_0x284; // direction to the player, normalized
    /* 0x28C */ u8 field_0x28c[0xC]; // not yet known
    /* 0x298 */ s32 field_0x298; // a player index (init: 4)
    /* 0x29C */ u8 field_0x29c[0x4]; // not yet known
    /* 0x2A0 */ s32 field_0x2a0[4]; // init: fn_801F6058(i) for each i
}; // Size: 0x2B0

#endif
