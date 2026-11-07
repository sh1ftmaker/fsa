#ifndef GAME_ACT_ACT_CUCCO_H
#define GAME_ACT_ACT_CUCCO_H

// Code: game/act_cucco.cpp.

#include "game/ActorBase.h"

// The NIWA actor ("Cucco", docs/names/actors.tsv, high confidence): the
// spawner constructs it with fn_80202FA0, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AB8C0.
class Cucco_804AB8C0 : public ActorBase_804A5770 {
public:
    Cucco_804AB8C0() {}
    /* 0x08 */ virtual ~Cucco_804AB8C0();
    /* 0x0C */ virtual void init();
    /* 0x10 */ virtual void vfunc_0x10();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    // 0x8032F148: a one-player version of ActorBase::fn_801EEC98 (angle, direction, distance of an actor seen from `player`).
    void fn_8032F148(s32 serial, u16* outAngle, s32* outDir, f32* outDist, s32* outPlayer, s32 player);
    u8 fn_80332378();
    void fn_80331D38();
    void fn_80331C24();
    void fn_80331B40();
    void fn_803319B8();
    /* 0x230 */ s32 field_0x230; // state
    /* 0x234 */ u8 field_0x234[4]; // not yet known
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ u8 field_0x244[0x10]; // not yet known
    /* 0x254 */ s32 field_0x254;
    /* 0x258 */ s32 field_0x258;
    /* 0x25C */ s32 field_0x25c;
    /* 0x260 */ u8 field_0x260[4]; // not yet known
    /* 0x264 */ s32 field_0x264;
    /* 0x268 */ s32 field_0x268;
    /* 0x26C */ s32 field_0x26c;
    /* 0x270 */ s32 field_0x270;
    /* 0x274 */ s32 field_0x274;
    /* 0x278 */ s32 field_0x278;
    // SHARED-EDIT: carved from field_0x27c[0x14] (game/act_cucco.cpp draw).
    /* 0x27C */ u8 field_0x27c[0xC]; // not yet known
    /* 0x288 */ u8 field_0x288; // opacity
    /* 0x289 */ u8 field_0x289[7]; // not yet known
    /* 0x290 */ s32 field_0x290;
    /* 0x294 */ u8 field_0x294[0xd]; // not yet known
    // Tested == 1 by the player's cucco glide (Player_804A6F38::fn_80263078, tu_8025EB18).
    /* 0x2A1 */ u8 field_0x2a1;
    /* 0x2A2 */ u8 field_0x2a2; // not yet known
    /* 0x2A3 */ u8 field_0x2a3;
    /* 0x2A4 */ u8 field_0x2a4[3]; // not yet known
    /* 0x2A7 */ u8 field_0x2a7;
    /* 0x2A8 */ u8 field_0x2a8;
    /* 0x2A9 */ u8 field_0x2a9;
    /* 0x2AA */ u8 field_0x2aa[10]; // not yet known
    /* 0x2B4 */ f32 field_0x2b4;
    /* 0x2B8 */ f32 field_0x2b8[3];
    /* 0x2C4 */ u8 field_0x2c4[0x30]; // not yet known
    /* 0x2F4 */ JPABaseEmitter* mEmitter2f4; // released by the destructor
    /* 0x2F8 */ JPABaseEmitter* mEmitter2f8; // released by the destructor
    /* 0x2FC */ JPABaseEmitter* mEmitter2fc; // released by the destructor
    /* 0x300 */ JPABaseEmitter* mEmitter300; // released by the destructor
}; // Size: 0x304

#endif
