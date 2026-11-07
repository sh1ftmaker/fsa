#ifndef GAME_ACT_ACT_MAGIC_CIRCLE_H
#define GAME_ACT_ACT_MAGIC_CIRCLE_H

// Code: game/act_magic_circle.cpp.

#include "game/ActorBase.h"

// The CIRC actor ("Magic circle", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_802023CC, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A7960.
class MagicCircle_804A7960 : public ActorBase_804A5770 {
public:
    MagicCircle_804A7960() {}
    /* 0x08 */ virtual ~MagicCircle_804A7960();
    /* 0x0C */ virtual void init();
    /* 0x10 */ virtual void vfunc_0x10();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    bool fn_8028B850();

    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ bool field_0x234;
    /* 0x235 */ u8 field_0x235[3];
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ f32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ u8 field_0x250;
    /* 0x251 */ u8 field_0x251;
    /* 0x252 */ u8 field_0x252[4];
    /* 0x256 */ u8 field_0x256[2];
    /* 0x258 */ s32 field_0x258;
    /* 0x25C */ s32 field_0x25c;
    /* 0x260 */ u8 field_0x260;
    /* 0x261 */ u8 field_0x261;
    /* 0x262 */ u8 field_0x262;
    /* 0x263 */ u8 field_0x263;
    /* 0x264 */ u8 field_0x264;
    /* 0x265 */ u8 field_0x265[3];
    /* 0x268 */ JGeometry::TVec3f field_0x268;
    /* 0x274 */ JPABaseEmitter* mEmitter274; // released by the destructor
    /* 0x278 */ JPABaseEmitter* mEmitter278; // released by the destructor
}; // Size: 0x27C

#endif
