#ifndef GAME_ACT_ACT_CHIEF_SOLDIER_H
#define GAME_ACT_ACT_CHIEF_SOLDIER_H

// Code: game/act_chief_soldier.cpp.

#include "game/ActorBase.h"

// The Chief Soldier actor ("SLR4", docs/names/actors.tsv): the spawner
// constructs it with fn_80203090, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A0B90.
class ChiefSoldier_804A0B90 : public ActorBase_804A5770 {
public:
    ChiefSoldier_804A0B90() {}
    // Destructor (game/act_chief_soldier.cpp): clears mAlive on the actor
    // whose serial is field_0x238, unless the state or the game root says
    // not to, then runs the base destructor and frees the object if the
    // delete flag is set.
    /* 0x08 */ virtual ~ChiefSoldier_804A0B90();
    // 0x80172900 (game/act_chief_soldier.cpp): the draw override. Draws the
    // "enemy_gc_sob.bin" sprite at mPos, through the visible box and a
    // SpriteParams, unless field_0x90 is 7 or 8 (fn_80172BC8) or lbl_8053AB60 is 1.
    /* 0x20 */ virtual void draw();
    /* 0x24 */ virtual void vfunc_0x24();
    // 0x801744E4 (game/act_chief_soldier.cpp): vtable slot 0x44, where
    // ActorBase has vfunc_0x44. Runs the spawned state (field_0x90 2 or 3)
    // and, once mLife is 0 or less, clears the actor at field_0x238.
    /* 0x44 */ virtual void vfunc_0x44();
    // 0x80172BC8 (game/act_chief_soldier.cpp): draws the "enemy_gc_sob.bin"
    // sprite at mPos with a 3x3 scale matrix (2.0, or 1.0 while field_0x23c
    // is 0), unless field_0x90 is 7 or the layer (field_0x4) is 8 or more.
    void fn_80172BC8();
    // 0x80154490-0x801544C0 (game/act_sword_soldier.cpp, the first unit to need
    // them): the sword soldier (SLR1) calls them on the SLR4 actor it found.
    void fn_80154490();
    void fn_8015449C();
    s32 fn_801544AC() const;
    void fn_801544B4();
    s32 fn_801544C0();
    // The spawn type, the low byte of field_0x90 (compared signed: cmpwi).
    s32 getType() const { return field_0x90 & 0xFF; }

    /* 0x0C */ virtual void init();   // fn_80174B44
    /* 0x14 */ virtual void update(); // fn_80172DC4, not written yet
    // 0x801746DC: steps the animation selected by field_0x29c.
    void fn_801746DC();
    // 0x80174220: places the hit box for the heading and damages / grabs the players in it.
    void fn_80174220();
    // 0x80175024: keeps the partner actor (the 'SLR2' at field_0x238) in step.
    void fn_80175024();
    // 0x801752D8: spawns the four keese circling the chief and counts them.
    void fn_801752D8();

    // SHARED-EDIT: carved from the unknown ranges (game/act_chief_soldier.cpp).
    /* 0x230 */ s32 field_0x230; // set to 2 by vfunc_0x44
    /* 0x234 */ u8 field_0x234; // draw() passes it as the sprite params' byte 1
    /* 0x235 */ u8 field_0x235;
    /* 0x236 */ u16 field_0x236;
    /* 0x238 */ s32 field_0x238; // serial of the actor the destructor clears
    /* 0x23C */ s32 field_0x23c; // vfunc_0x44 tests it against 0
    /* 0x240 */ s32 field_0x240; // cleared by vfunc_0x44
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250; // set to 0x14 by vfunc_0x44
    /* 0x254 */ s32 field_0x254;
    /* 0x258 */ s32 field_0x258;
    /* 0x25C */ s32 field_0x25c;
    /* 0x260 */ s32 field_0x260;
    /* 0x264 */ s32 field_0x264; // cleared by vfunc_0x44
    /* 0x268 */ s32 field_0x268;
    /* 0x26C */ s32 field_0x26c;
    /* 0x270 */ s32 field_0x270;
    /* 0x274 */ s32 field_0x274;
    /* 0x278 */ s32 field_0x278;
    /* 0x27C */ s32 field_0x27c;
    /* 0x280 */ s32 field_0x280; // frame base: fn_80172BC8 draws frame field_0x280 + 0x3FD
    /* 0x284 */ s32 field_0x284;
    /* 0x288 */ s32 field_0x288; // set to 2 by vfunc_0x44
    /* 0x28C */ s32 field_0x28c; // fn_801544AC (out of line in game/act_sword_soldier.cpp)
    /* 0x290 */ s32 field_0x290; // counter: fn_8015449C adds 1, fn_801544B4 sets -1
    /* 0x294 */ s32 field_0x294;
    /* 0x298 */ s32 field_0x298;
    /* 0x29C */ s32 field_0x29c;
    /* 0x2A0 */ s32 field_0x2a0;
    /* 0x2A4 */ u8 field_0x2a4;
    /* 0x2A5 */ u8 field_0x2a5; // vfunc_0x44 tests it against 1; fn_80154490 sets it to 1
    /* 0x2A6 */ u8 field_0x2a6[2];
    /* 0x2A8 */ f32 field_0x2a8;
    /* 0x2AC */ f32 field_0x2ac;
    /* 0x2B0 */ f32 field_0x2b0;
    /* 0x2B4 */ JGeometry::TVec3f field_0x2b4;
    /* 0x2C0 */ s32 field_0x2c0[4]; // serials of the four keese
    /* 0x2D0 */ // size: the spawner's __nw__
};

#endif
