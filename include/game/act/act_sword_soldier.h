#ifndef GAME_ACT_ACT_SWORD_SOLDIER_H
#define GAME_ACT_ACT_SWORD_SOLDIER_H

// Code: game/act_sword_soldier.cpp.

#include "game/ActorBase.h"

// The SLR1 actor (Sword Soldier, docs/names/actors.tsv, high confidence).
// Named after its vtable, lbl_8049FCA0 (.data 0x270, size 0x58). Field names
// come from the asm only (shotgun pass).
class SwordSoldier_8049FCA0 : public ActorBase_804A5770 {
public:
    /* 0x08 */ virtual ~SwordSoldier_8049FCA0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x44 */ virtual void vfunc_0x44();
    // 0x54, a slot of its own: true when the soldier fell into a hole or water.
    /* 0x54 */ virtual bool vfunc_0x54();

    /* 0x230 */ s32 mState;
    /* 0x234 */ u8 field_0x234;
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
    /* 0x274 */ s32 field_0x274; // sprite frame base
    /* 0x278 */ s32 field_0x278; // index among the living soldiers
    /* 0x27C */ s32 field_0x27c;
    /* 0x280 */ s32 field_0x280;
    /* 0x284 */ s32 field_0x284;
    /* 0x288 */ s32 field_0x288; // kind (spawn parameter bits 20-23)
    /* 0x28C */ f32 field_0x28c;
    /* 0x290 */ f32 field_0x290;
    /* 0x294 */ f32 field_0x294;
    /* 0x298 */ f32 field_0x298;
    /* 0x29C */ s16 field_0x29c;
    /* 0x29E */ s16 field_0x29e;
    /* 0x2A0 */ s16 field_0x2a0;
    /* 0x2A4 */ s32 field_0x2a4; // serial of the soldier it follows
    /* 0x2A8 */ s32 field_0x2a8;
    /* 0x2AC */ JGeometry::TVec3f field_0x2ac;
    /* 0x2B8 */ JGeometry::TVec3f field_0x2b8;
    /* 0x2C4 */ u8 field_0x2c4[0x2C8 - 0x2C4]; // not yet known (size: the spawner's __nw__)
};

#endif
