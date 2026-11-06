#ifndef GAME_ACT_ACT_KEESE_H
#define GAME_ACT_ACT_KEESE_H

// Code: game/act_keese.cpp.

#include "game/ActorBase.h"

// The KETH actor ("Keese", docs/names/actors.tsv, high confidence): the
// spawner constructs it with fn_80203B3C, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A02B8.
class Keese_804A02B8 : public ActorBase_804A5770 {
public:
    Keese_804A02B8() {}
    /* 0x08 */ virtual ~Keese_804A02B8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);

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
    /* 0x25C */ u16 field_0x25c;
    /* 0x25E */ u8 field_0x25e;
    /* 0x260 */ s32 field_0x260;
    /* 0x264 */ s32 field_0x264;
    /* 0x268 */ u16 field_0x268;
    /* 0x26A */ u16 field_0x26a;
    /* 0x26C */ f32 field_0x26c;
    /* 0x270 */ f32 field_0x270;
    /* 0x274 */ f32 field_0x274;
    /* 0x278 */ f32 field_0x278;
    /* 0x27C */ f32 field_0x27c;
    /* 0x280 */ JGeometry::TVec3f field_0x280;
}; // Size: 0x28C

#endif
