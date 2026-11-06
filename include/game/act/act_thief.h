#ifndef GAME_ACT_ACT_THIEF_H
#define GAME_ACT_ACT_THIEF_H

// Code: game/act_thief.cpp (not written yet; the members below are in
// game/tu_8020F798.cpp). Actor TZOK ("Thief" in docs/names/actors.tsv): an
// EnemyBase_804A4808 whose spawner (ctor 0x80206810) allocates 0x388 bytes.
// Only the fields tu_8020F798 uses are named; the rest of the class is
// padding until act_thief is written.

#include "game/cls_804A46F4.h"
#include "game/cls_80217374.h"
#include "game/cls_8021B904.h"
#include "game/tu_801D2918.h"

class JPABaseEmitter;



// Named after its vtable, 0x804A5A30.
class Thief_804A5A30 : public cls_804A46F4 {
public:
    // One entry of the type table lbl_8046A238 (0x14 bytes each, 4 entries,
    // .rodata of act_thief; indexed by mType). Only two fields are known.
    struct TypeParams {
        /* 0x00 */ f32 field_0x0;
        /* 0x04 */ f32 field_0x4;
        /* 0x08 */ s32 field_0x8;
        /* 0x0C */ s32 mSpriteFrame; // added to the animation cel (mAnim.mCel)
        /* 0x10 */ f32 mHeight;      // subtracted from mPos.z for the lit sprite
    };

    // 0x80206810 (act_thief): stores this vtable and the initial state of
    // every field below, and resets the two timers at 0x368 and 0x374.
    Thief_804A5A30();
    virtual ~Thief_804A5A30();
    /* 0x20 */ virtual void draw();
    // Overrides in game/tu_80206B30.cpp (0x80206B30-0x80207004; slot order).
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    /* 0x58 */ virtual bool vfunc_0x58();
    /* 0x5C */ virtual u8 vfunc_0x5C();
    /* 0x60 */ virtual u8 vfunc_0x60();
    /* 0x64 */ virtual u8 vfunc_0x64();
    /* 0x68 */ virtual u8 vfunc_0x68();
    // 0x80204D6C (act_thief): the last slot of the vtable. Tells the camera
    // to keep mPos in view, unless the thief is in state 8 or 9.
    /* 0xA0 */ virtual void vfunc_0xA0();
    // 0x80204DAC (act_thief): the Thief's slot 0x98 (EnemyBase's step). In
    // state 3 with field_0xc4 set it takes the player in field_0xc8's
    // direction, angle and position (and, on a layer change, its position
    // in field_0x32c); in other states it runs fn_801CE32C when no hit bit
    // is set.
    /* 0x98 */ virtual void vfunc_0x98();

    // 0x802072E4 (tu_80206B30): mBox = (-8, -16)-(8, 8), copied to mBox2.
    void fn_802072E4();
    // 0x80205E0C (act_thief, not written yet): switches to state `state`.
    void fn_80205E0C(s32 state);
    // 0x8020609C and 0x802064E0 (act_thief, not written yet): state tests
    // the hit overrides make; roles unknown.
    bool fn_8020609C() const;
    bool fn_802064E0();
    // 0x80206168 (act_thief, not written yet): called by a player lifting the
    // thief ('TZOK', Player_804A6F38::fn_80263860 at frame 11 of state 0x15);
    // the caller ignores r3.
    void fn_80206168();
    // 0x802063FC (act_thief): sorts the actors on this layer
    // by distance from mPos, then returns the serial of the first (of up to
    // 20) whose way from mPos is clear, or -1.
    s32 fn_802063FC();

    // 0x8020F80C: draws the shadow and the sprite at mPos on the layer for
    // sprite frame `frame` (the draw() argument).
    void fn_8020F80C(s32 frame);
    // 0x8020FADC: called from act_thief's update.
    void fn_8020FADC();

    /* 0x340 */ s32 field_0x340; // -1 at construction
    /* 0x344 */ s32 field_0x344;
    /* 0x348 */ u8 field_0x348;
    /* 0x349 */ u8 field_0x349;
    /* 0x34A */ u8 field_0x34a;
    /* 0x34B */ u8 field_0x34b;
    /* 0x34C */ u8 field_0x34c;
    /* 0x350 */ s32 mType;            // index into lbl_8046A238
    /* 0x354 */ s32 field_0x354;
    /* 0x358 */ JPABaseEmitter* mEmitter358;
    /* 0x35C */ Damped2D_801D2918 field_0x35c; // a hit player pushes it (vfunc_0x68)
    /* 0x368 */ cls_80217374 field_0x368;      // wandering direction timer
    /* 0x374 */ cls_8021B904 field_0x374;      // steering timer
}; // Size: 0x388

extern const Thief_804A5A30::TypeParams lbl_8046A238[];

#endif
