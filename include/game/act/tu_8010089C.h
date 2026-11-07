#ifndef GAME_ACT_TU_8010089C_H
#define GAME_ACT_TU_8010089C_H

// Code: game/tu_8010089C.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x8049C8EC.
class WhackAMoleShop_8049C8EC : public ActorBase_804A5770 {
public:
    WhackAMoleShop_8049C8EC() {}
    /* 0x08 */ virtual ~WhackAMoleShop_8049C8EC();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x44 */ virtual void vfunc_0x44();
    // 0x80100A90 (the sign's init), 0x80100D28 (the shop's per-frame work).
    void fn_80100A90();
    void fn_80100D28();

    /* 0x230 */ s32 field_0x230; // state
    /* 0x234 */ s32 field_0x234; // frame counter
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c; // the counter fn_80100A90 reads from the shop it points to (field_0x290)
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ u8 field_0x244; // the number of players (GameRoot::fn_80138A30)
    /* 0x245 */ u8 field_0x245;
    /* 0x246 */ u8 field_0x246;
    /* 0x247 */ u8 field_0x247;
    /* 0x248 */ u8 field_0x248; // bits 22..26 of field_0x90
    /* 0x249 */ u8 field_0x249[3]; // not yet known
    /* 0x24C */ f32 field_0x24c; // the position after init's offset
    /* 0x250 */ f32 field_0x250;
    /* 0x254 */ f32 field_0x254;
    /* 0x258 */ f32 field_0x258;
    /* 0x25C */ u8 field_0x25c[4];
    /* 0x260 */ s16 field_0x260[4];
    /* 0x268 */ s16 field_0x268[4];
    /* 0x270 */ s32 field_0x270[4]; // fn_802373EC of each player (fn_80100D28 state 1)
    /* 0x280 */ s32 field_0x280[4]; // fn_8023725C of each player
    /* 0x290 */ WhackAMoleShop_8049C8EC* field_0x290; // the shop whose scores are counted (vfunc_0x44)
}; // Size: 0x294

#endif
