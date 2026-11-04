#ifndef GAME_ACT_ACT_CANNON_H
#define GAME_ACT_ACT_CANNON_H

// Code: game/act_cannon.cpp.

#include "game/ActorBase.h"
#include "game/SpriteAnim.h"

// The CNON actor ("Cannon", docs/names/actors.tsv, high confidence): the
// spawner constructs it with __ct__15Cannon_8049CCF0Fv (0x80108168), which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x8049CCF0.
class Cannon_8049CCF0 : public ActorBase_804A5770 {
public:
    // __ct__15Cannon_8049CCF0Fv (0x80108168): the base constructor, the
    // vtable, then mAnim's constructor (SpriteAnim_801D267C), then the
    // fields below.
    Cannon_8049CCF0();
    /* 0x08 */ virtual ~Cannon_8049CCF0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    f32 fn_801080B8() const;
    bool fn_80107360();
    /* 0x230 */ SpriteAnim_801D267C mAnim;
    /* 0x268 */ u8 field_0x268; // 1 after construction
    /* 0x269 */ u8 field_0x269; // set by init() when vfunc_0x3C() and vfunc_0x34() hold
    /* 0x26A */ u8 field_0x26a; // bit 16 of field_0x90 (init)
    /* 0x26C */ s32 field_0x26c; // -1 after construction; passed to fn_80230FD0
    /* 0x270 */ s32 field_0x270;
    /* 0x274 */ s32 field_0x274; // state: 0 swing, 1 (init() sets it for some spawns), 2 swing + timer, 3 animation
    /* 0x278 */ s32 field_0x278; // the previous state (fn_80107360)
    /* 0x27C */ s32 field_0x27c; // bits 4-7 of field_0x90 (init)
    /* 0x280 */ f32 field_0x280; // bits 8-12 of field_0x90 as a float (init)
    /* 0x284 */ f32 field_0x284; // 0.0f; fn_80107360 clears it
    /* 0x288 */ JGeometry::TVec3f mHomePos; // mPos as init() saw it; fn_80107360 moves mPos.y back to mHomePos.y
}; // Size: 0x294

#endif
