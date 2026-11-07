#ifndef GAME_ACT_ACT_GANON_H
#define GAME_ACT_ACT_GANON_H

// Code: game/act_ganon.cpp.

#include "game/ActorBase.h"

class JPABaseEmitter;

// The 'GNON' actor (Ganon, docs/names/actors.tsv, high confidence; the spawner's case for
// ACTOR_GNON). Named after its vtable, 0x804A25F0.
class Ganon_804A25F0 : public ActorBase_804A5770 {
public:
    Ganon_804A25F0() {}
    /* 0x08 */ virtual ~Ganon_804A25F0();
    // fn_80192038: plays sound 0x40006 and re-issues vfunc_0x28 with 2 and (0, 0).
    /* 0x24 */ virtual void vfunc_0x24();
    // 0x8019CEBC / 0x8019CEC4 (game/tu_80197070.cpp): store a word at 0x28C / 0x2F8.
    void fn_8019CEBC(s32 value);
    void fn_8019CEC4(s32 value);

    /* 0x0C */ virtual void init();   // fn_801968C8
    /* 0x14 */ virtual void update(); // fn_80192D98
    /* 0x20 */ virtual void draw();   // fn_80192090
    void fn_80196020(s32 player);
    void fn_80196208(s32 mode);
    void fn_801964AC();
    void fn_80192BD8();

    // SHARED-EDIT: carved from field_0x230[0x28C - 0x230] .. [0x328 - 0x2FC] (game/act_ganon.cpp).
    /* 0x230 */ s32 mState;
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
    /* 0x274 */ s32 field_0x274;
    /* 0x278 */ s32 field_0x278;
    /* 0x27C */ s32 field_0x27c;
    /* 0x280 */ s32 field_0x280;
    /* 0x284 */ s32 field_0x284;
    /* 0x288 */ s32 field_0x288;
    // SHARED-EDIT: set by fn_8019CEBC (game/tu_80197070.cpp; the ZLD2 actor's update).
    /* 0x28C */ s32 field_0x28c;
    /* 0x290 */ s32 field_0x290;
    /* 0x294 */ s32 field_0x294;
    /* 0x298 */ s32 field_0x298;
    /* 0x29C */ s32 field_0x29c;
    /* 0x2A0 */ s32 field_0x2a0;
    /* 0x2A4 */ u8 field_0x2a4;
    /* 0x2A5 */ u8 field_0x2a5;
    /* 0x2A6 */ u8 field_0x2a6;
    /* 0x2A7 */ u8 field_0x2a7;
    /* 0x2A8 */ f32 field_0x2a8;
    /* 0x2AC */ f32 field_0x2ac;
    /* 0x2B0 */ f32 field_0x2b0;
    /* 0x2B4 */ s16 field_0x2b4;
    /* 0x2B6 */ s16 field_0x2b6;
    /* 0x2B8 */ JGeometry::TVec3f field_0x2b8;
    /* 0x2C4 */ u8 field_0x2c4[0x2D0 - 0x2C4]; // not yet known
    /* 0x2D0 */ JGeometry::TVec3f field_0x2d0;
    /* 0x2DC */ JGeometry::TVec3f field_0x2dc;
    /* 0x2E8 */ JGeometry::TVec3f field_0x2e8;
    /* 0x2F4 */ f32 field_0x2f4;
    // SHARED-EDIT: set by fn_8019CEC4 (game/tu_80197070.cpp).
    /* 0x2F8 */ s32 field_0x2f8;
    /* 0x2FC */ s32 field_0x2fc;
    /* 0x300 */ s32 field_0x300;
    /* 0x304 */ s16 field_0x304;
    /* 0x306 */ u8 field_0x306[2];
    /* 0x308 */ s32 field_0x308[4];
    /* 0x318 */ s32 field_0x318[4];
    /* 0x328 */ JPABaseEmitter* mEmitters[5]; // freed by the destructor with releaseEmitter_801EE434
}; // Size: 0x33C

#endif
