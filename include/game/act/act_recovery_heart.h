#ifndef GAME_ACT_ACT_RECOVERY_HEART_H
#define GAME_ACT_ACT_RECOVERY_HEART_H

// Code: game/act_recovery_heart.cpp.

#include "game/ActorBase.h"

// The HART actor ("Recovery Heart", docs/names/actors.tsv, med confidence):
// the spawner constructs it with fn_80203ED8, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x8049F4F0.
class RecoveryHeart_8049F4F0 : public ActorBase_804A5770 {
public:
    RecoveryHeart_8049F4F0() {}
    /* 0x08 */ virtual ~RecoveryHeart_8049F4F0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x40 */ virtual void vfunc_0x40();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    /* 0x230 */ s32 field_0x230; // state word; vfunc_0x40 sets 4
    /* 0x234 */ s32 field_0x234; // init: 0x3FF
    /* 0x238 */ s32 field_0x238; // init: 0
    /* 0x23C */ s32 field_0x23c; // sway angle (sincosTable_.sinShort index); += or -= 0x700
    /* 0x240 */ s32 field_0x240; // init: field_0x90 & 3; 1 skips the setup
}; // Size: 0x244

#endif
