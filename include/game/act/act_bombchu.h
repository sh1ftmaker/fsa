#ifndef GAME_ACT_ACT_BOMBCHU_H
#define GAME_ACT_ACT_BOMBCHU_H

// Code: game/act_bombchu.cpp.

#include "game/ActorBase.h"

// The BMTY actor ("Bombchu", docs/names/actors.tsv, high confidence): its
// constructor stores this vtable (config/G4SE01/game_units.tsv). Named after
// its vtable, 0x804AB464. Its vtable is ActorBase's 0x54 bytes plus two
// slots, and its destructor calls ~ActorBase directly, so the base is
// ActorBase, not EnemyBase.
class Bombchu_804AB464 : public ActorBase_804A5770 {
public:
    // 0x8032500C: r4 = the delete flag; > 0 frees the object (__dl__FPv).
    /* 0x08 */ virtual ~Bombchu_804AB464();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x48 */ virtual void vfunc_0x48();
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    /* 0x54 */ virtual void vfunc_0x54();

    s32 kind() const;  // inline, act_bombchu.cpp
    s32 hitPlayers();  // inline, act_bombchu.cpp
    void faceMoveDirection();
    void fn_8032506C();

    /* 0x230 */ s32 field_0x230; // state; vfunc_0x48 sets 3; fn_801CC8E4 tests != 3 (cmpwi)
    /* 0x234 */ f32 field_0x234; // the speed along the second axis (+-1.25 / +-2.5)
    /* 0x238 */ s32 field_0x238; // countdowns, each stepped by update while > 0
    /* 0x23C */ s32 field_0x23c; // fuse (300)
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250; // sprite frame offset; index into lbl_804AB440
    /* 0x254 */ s32 field_0x254;
    /* 0x258 */ s32 field_0x258;
    /* 0x25C */ s32 field_0x25c;
    /* 0x260 */ s32 field_0x260;
    /* 0x264 */ s32 field_0x264;
    /* 0x268 */ s32 field_0x268;
    /* 0x26C */ s32 field_0x26c; // player (bits 16-17 of the spawn field_0x90)
    /* 0x270 */ s32 field_0x270; // bits 8-11 of the spawn field_0x90
    /* 0x274 */ u8 field_0x274;
    /* 0x275 */ u8 field_0x275;
    /* 0x276 */ u8 field_0x276; // 1: moving along x first
    /* 0x277 */ u8 field_0x277;
    /* 0x278 */ u8 field_0x278; // bit 4 of the spawn field_0x90
    /* 0x27C */ f32 field_0x27c; // facing angle (radians), drawn as a y rotation
    /* 0x280 */ f32 field_0x280;
}; // Size: 0x284 (the spawner's __nw__)

#endif
