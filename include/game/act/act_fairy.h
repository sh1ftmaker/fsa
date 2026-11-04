#ifndef GAME_ACT_ACT_FAIRY_H
#define GAME_ACT_ACT_FAIRY_H

// Code: game/act_fairy.cpp.

#include "game/ActorBase.h"

// The FARY actor ("Fairy", docs/names/actors.tsv, high confidence): the
// spawner constructs it with fn_802039D4, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A03B8.
class Fairy_804A03B8 : public ActorBase_804A5770 {
public:
    Fairy_804A03B8() {}
    /* 0x08 */ virtual ~Fairy_804A03B8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x44 */ virtual void vfunc_0x44();
    /* 0x230 */ s32 field_0x230; // state of the spawn-in sequence (0, 4, 5)
    /* 0x234 */ s32 field_0x234; // 10000 after init; draw returns early below 0x100 with bit 2 set
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c; // 10 after init, 30 in state 3
    /* 0x240 */ s32 field_0x240; // 30 + a random 0..29 after init
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248; // selects the second sprite frame set (0x7D) when nonzero
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250; // sprite frame offset, added to 0x5E
    /* 0x254 */ s32 field_0x254; // -1 after init
    /* 0x258 */ s32 field_0x258; // state: 2 draws the second sprite, 3 sets the sprite's byte 0x23
    /* 0x25C */ s32 field_0x25c; // countdown from field_0x90's low nibble
    /* 0x260 */ s32 field_0x260; // field_0x90 bits 22-26
    /* 0x264 */ s32 field_0x264;
    /* 0x268 */ u8 field_0x268;
    /* 0x269 */ u8 field_0x269[0x3]; // not yet known
    /* 0x26C */ f32 field_0x26c;
    /* 0x270 */ f32 field_0x270; // 255.0f after init; converted to a byte for the sprite in state 3
    /* 0x274 */ u8 field_0x274[0x4]; // not yet known
    /* 0x278 */ f32 field_0x278;
    /* 0x27C */ f32 field_0x27c;
    /* 0x280 */ f32 field_0x280; // 10.0f after init, 100.0f in state 3
    /* 0x284 */ JGeometry::TVec3f field_0x284; // copy of mPos made by init
    /* 0x290 */ JPABaseEmitter* mEmitter290; // released by the destructor
    /* 0x294 */ JPABaseEmitter* mEmitter294; // released by the destructor
}; // Size: 0x298

#endif
