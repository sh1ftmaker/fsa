#ifndef GAME_ACT_ACT_SLINGSHOT_SEED_H
#define GAME_ACT_ACT_SLINGSHOT_SEED_H

// Code: game/act_slingshot_seed.cpp.

#include "game/ActorBase.h"

// The PCDM actor ("Slingshot seed", docs/names/actors.tsv, med confidence):
// the spawner constructs it with fn_80202688, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AB3A8.
class SlingshotSeed_804AB3A8 : public ActorBase_804A5770 {
public:
    SlingshotSeed_804AB3A8() {}
    /* 0x08 */ virtual ~SlingshotSeed_804AB3A8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    /* 0x230 */ s32 field_0x230; // state
    /* 0x234 */ f32 field_0x234; // 255.0f after init: the alpha
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240; // life counter (500 after init)
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ s32 field_0x254;
    /* 0x258 */ u8 field_0x258[0x25C - 0x258]; // not yet known
    /* 0x25C */ s32 field_0x25c;
    /* 0x260 */ s32 field_0x260;
    /* 0x264 */ s32 field_0x264;
    /* 0x268 */ u8 field_0x268;
    /* 0x269 */ u8 field_0x269;
    /* 0x26A */ u8 field_0x26a;
    /* 0x26B */ u8 field_0x26b; // not yet known
    /* 0x26C */ JPABaseEmitter* mEmitter26c; // released by the destructor
    /* 0x270 */ f32 field_0x270;
    /* 0x274 */ f32 field_0x274;
    /* 0x278 */ u8 field_0x278; // the layer's flag, copied by init
    /* 0x279 */ u8 field_0x279[3]; // not yet known
}; // Size: 0x27C

#endif
