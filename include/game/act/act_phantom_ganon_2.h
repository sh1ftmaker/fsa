#ifndef GAME_ACT_ACT_PHANTOM_GANON_2_H
#define GAME_ACT_ACT_PHANTOM_GANON_2_H

// Code: game/act_phantom_ganon_2.cpp.

#include "game/ActorBase.h"

class JPABaseEmitter;

// The FGN2 actor (Phantom Ganon 2, docs/names/actors.tsv, high confidence;
// the spawner's case for ACTOR_FGN2). Named after its vtable, 0x804A1A98.
class PhantomGanon2_804A1A98 : public ActorBase_804A5770 {
public:
    PhantomGanon2_804A1A98() {}
    /* 0x08 */ virtual ~PhantomGanon2_804A1A98();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x24 */ virtual void vfunc_0x24();

    // Picks the player this actor goes for (a random starting player, the next
    // free one after it), stores it in field_0x94 and returns it (0x80189DEC).
    u16 fn_80189DEC();

    // SHARED-EDIT: carved from the unknown ranges (game/act_phantom_ganon_2.cpp).
    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ s32 field_0x254;
    /* 0x258 */ s32 field_0x258;
    /* 0x25C */ s32 field_0x25c;
    /* 0x260 */ s32 field_0x260;
    /* 0x264 */ s32 field_0x264;
    /* 0x268 */ s32 field_0x268;
    /* 0x26C */ s32 field_0x26c;
    /* 0x270 */ s32 field_0x270;
    /* 0x274 */ s32 field_0x274; // serial of the other Phantom Ganon (-1 for none)
    /* 0x278 */ s32 field_0x278;
    /* 0x27C */ s32 field_0x27c;
    /* 0x280 */ f32 field_0x280;
    /* 0x284 */ f32 field_0x284;
    /* 0x288 */ f32 field_0x288;
    /* 0x28C */ f32 field_0x28c;
    /* 0x290 */ f32 field_0x290;
    /* 0x294 */ f32 field_0x294;
    /* 0x298 */ f32 field_0x298;
    /* 0x29C */ f32 field_0x29c;
    /* 0x2A0 */ JGeometry::TVec3f field_0x2a0;
    /* 0x2AC */ f32 field_0x2ac;
    /* 0x2B0 */ JGeometry::TVec3f field_0x2b0;
    /* 0x2BC */ JGeometry::TVec3f field_0x2bc;
    /* 0x2C8 */ JGeometry::TVec3f field_0x2c8;
    /* 0x2D4 */ JGeometry::TVec3f field_0x2d4;
    /* 0x2E0 */ JGeometry::TVec3f field_0x2e0;
    /* 0x2EC */ s16 field_0x2ec;
    /* 0x2EE */ s8 field_0x2ee;
    /* 0x2EF */ u8 field_0x2ef;
    /* 0x2F0 */ u8 field_0x2f0;
    /* 0x2F1 */ u8 field_0x2f1[3];
    /* 0x2F4 */ JPABaseEmitter* mEmitters2f4[4];
    /* 0x304 */ JPABaseEmitter* mEmitters304[4];
    /* 0x314 */ JPABaseEmitter* mEmitter314;
}; // Size: 0x318 (the spawner's __nw__)

#endif
