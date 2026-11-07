#ifndef GAME_ACT_TU_803276C4_H
#define GAME_ACT_TU_803276C4_H

// Code: game/tu_803276C4.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804AB528.
class cls_804AB528 : public ActorBase_804A5770 {
public:
    cls_804AB528() {}
    /* 0x08 */ virtual ~cls_804AB528();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 field_0x230; // state word (lwz/stw); GameView_804A9950::fn_802DA630 tests 2
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240; // (field_0x90 >> 4) & 0xF
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248; // (field_0x90 >> 8) & 0xF; actors with the same value form a group
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ u8 field_0x254[4]; // per-player flags (indexed by player in update)
    /* 0x258 */ u8 field_0x258;
    /* 0x259 */ u8 field_0x259;
    /* 0x25A */ u8 field_0x25a; // (field_0x90 >> 12) & 1
    /* 0x25B */ u8 field_0x25b;
    /* 0x25C */ f32 field_0x25c;
    /* 0x260 */ JGeometry::TVec3f field_0x260;
    /* 0x26C */ JGeometry::TVec3f field_0x26c[4];
}; // Size: 0x29C

#endif
