#ifndef GAME_ACTORBASE_H
#define GAME_ACTORBASE_H

// The base class of every actor the spawner creates (fn_801F9484, see
// game/ActorManager.h). Its code is game/tu_801EE24C.cpp; its vtable is at
// 0x804A5770 (__vt__18ActorBase_804A5770).
//
// Naming (docs/PLAN.md, "Placeholder names"): a provisional descriptive
// name carries the address of the class's vtable, or for a class without
// one its constructor or first method, until evidence gives the real name.
// FSA has no RTTI and no symbol map, and no string names these classes.
// Rename (with their mangled symbols) only on evidence.
//
// Vectors: prototypes and members use JGeometry::TVec3f for game data;
// dolphin's Vec only where a mangled name or a PSMTX/PSVEC call proves it.
//
// Layout: from the constructor (0x801F5AE0), the spawner's inline setters
// (0x801FF630-0x801FF738), the actor manager's loops (game/tu_801F6058.cpp)
// and the destructor (0x801F593C). sizeof == 0x230: the smallest
// __nw__(size, heap, 0) in the spawner is 0x230, for 11 actors whose inline
// constructors only call this one and store their own vtable (RAIL, STGN,
// ENDR, ...).
//
// Owner: the shared-header owner (docs/COORDINATOR.md, "Shared game
// headers"). Game-file agents propose changes as SHARED-EDITS.

#include "types.h"
#include "JSystem/JGeometry.h"
#include "dolphin/mtx.h"
#include "dolphin/gx/GXStruct.h"

class ActorSoundObj_804578F4;
class JPABaseEmitter;

// The two boxes at 0x60 and 0x70 are JGeometry::TBox2f (min i, max f) in the
// actor's top-down space (x, y - z), relative to its position: the
// spawner's whole-box copy (fn_801FF630) is the word-by-word copy MWCC emits
// for TBox2<f32> (a plain struct of four floats copies with lfs/stfs), and
// fn_801F5790 tests getWidth() > 0 and getHeight() > 0 before using one.

// The member at 0x1DC (named after its constructor, 0x801EAD68; no vtable).
// Constructor fn_801EAD68, per-frame update
// fn_801EAB0C (both in game/tu_801EAB0C.cpp), called by the actor manager's
// update loop before the actor's own update. Its purpose is unknown.
struct ActorSub1DC_801EAD68 {
    ActorSub1DC_801EAD68();
    void fn_801EAB0C();
    bool fn_801EAC20(int index);
    bool fn_801EAC54(int id);
    bool isLive(int id);
    /* 0x00 */ u8 field_0x0[4];
    /* 0x04 */ u8 field_0x4[16];
    /* 0x14 */ s32 field_0x14[16]; // -1 after construction
}; // Size: 0x54

// A colour word of SpriteParams_801F5AE0 (0x10 and 0x14), written whole
// (-1, 0) and also bytewise: fn_801EF830 reads 0x13 (alpha of 0x10),
// cls_804A4C58::draw adds to r, g and b of 0x14 (lbz/add/stb on 0x14-0x16).
union SpriteParamsColor_801F5AE0 {
    s32 mWord;
    GXColor mColor;
};

// The sprite drawing parameters (no vtable and no out-of-line constructor;
// named after the constructor it is inlined into, ActorBase's at
// 0x801F5AE0). One lives in every actor at 0x120, and draw() overrides
// build others on the stack (act_rat 0x800F3C70: the same stores in the same
// order) to pass to the "SOB " sprite functions (fn_801F06F0, fn_801F02BC,
// fn_801EFD34, fn_801EF830, fn_801EF574), which NULL-check it and read
// 0x13, 0x18, 0x1C and 0x1D (fn_801EF830, 0x801EFA64-0x801EFBB0). The
// constructor sets 0x10 and 0x14 and then again after filling the table (the
// base constructor's stores show both), hence the separate reset.
struct SpriteParams_801F5AE0 {
    SpriteParams_801F5AE0() {
        field_0x10.mWord = -1;
        field_0x14.mWord = -1;
        reset();
    }
    void reset() {
        for (u8 i = 0; i < 16; i++) {
            field_0x0[i] = i;
        }
        field_0x10.mWord = -1;
        field_0x14.mWord = 0;
        mMtx = NULL;
        field_0x1c = 0;
        field_0x1d = 0;
        field_0x1e = 0;
        field_0x1f = 1;
        field_0x20 = 1;
        field_0x21 = 0;
        field_0x22 = 0;
        field_0x23 = 0;
        field_0x24 = 0;
        field_0x25 = 0;
        field_0x26 = 1;
        field_0x28 = 3;
        field_0x2c = 1;
        field_0x2d = 0;
        field_0x2e = 0;
        field_0x30 = -1;
        field_0x34 = 0;
        field_0x38 = 2;
    }

    /* 0x00 */ u8 field_0x0[16]; // 0, 1, ..., 15
    /* 0x10 */ SpriteParamsColor_801F5AE0 field_0x10; // fn_801EF830 reads mColor.a (lbz 0x13)
    /* 0x14 */ SpriteParamsColor_801F5AE0 field_0x14;
    // When set, fn_801EF830 concatenates it with the sprite's scale matrix
    // (PSMTXConcat, 0x801EFBD0) and keeps its translation column; act_rat's
    // draw points it at a stack Mtx when its spawn parameter is nonzero.
    /* 0x18 */ MtxP mMtx;
    /* 0x1C */ s8 field_0x1c; // fn_8023CA88 (s8) in act_sign; -1 in act_blindfold (stb of a shared -1)
    /* 0x1D */ s8 field_0x1d; // -1 in tu_80333958 draw (stb of a shared -1, as 0x1C)
    /* 0x1E */ u8 field_0x1e;
    /* 0x1F */ u8 field_0x1f;
    /* 0x20 */ u8 field_0x20;
    /* 0x21 */ u8 field_0x21;
    /* 0x22 */ u8 field_0x22;
    /* 0x23 */ u8 field_0x23;
    /* 0x24 */ u8 field_0x24;
    /* 0x25 */ u8 field_0x25;
    /* 0x26 */ u8 field_0x26;
    /* 0x28 */ s32 field_0x28;
    /* 0x2C */ u8 field_0x2c;
    /* 0x2D */ u8 field_0x2d;
    /* 0x2E */ u8 field_0x2e;
    /* 0x30 */ s32 field_0x30;
    /* 0x34 */ s32 field_0x34;
    /* 0x38 */ s32 field_0x38;
}; // Size: 0x3C

class ActorBase_804A5770 {
public:
    ActorBase_804A5770();

    // Virtual table at 0x804A5770: 19 slots, settled (G1). 377 game vtables
    // use this layout: this one and its 376 descendants (288 direct children
    // with exactly 19 slots; the rest append slots after 0x50). Adding,
    // removing or reordering a slot here shifts every one of them, so the
    // signatures below were checked against every override and every virtual
    // call whose receiver is an actor (the caller's own `this`, or an actor
    // from the manager's lists). Offsets are into the vtable (MWCC: two
    // header words, the first slot at +0x8). "Overrides" counts descendant
    // classes whose slot differs from the base's; "args read" is what the
    // overrides read before writing (r4-r10, f1-f8). MWCC allocates GPR and
    // FPR arguments separately, so the order of ints against floats is not
    // visible; ints first is a guess wherever both occur. Names only where
    // the callers make the role unambiguous.
    //
    // 0x08: all 376 descendants have their own (deleting) destructor; each
    // reads r4 (extsh. of the delete flag) and returns this. The manager
    // deletes with r4 = 1 (0x801F85D0, 0x801F8984, 0x801F8C50, 0x801F8EA8).
    /* 0x08 */ virtual ~ActorBase_804A5770();
    // init: 375 overrides (370 functions), none reads an argument; the
    // spawner calls it once after the spawn fields are set (0x801FF590) and
    // ignores r3. 69 overrides call J3DFrameCtrl::init. Empty here.
    /* 0x0C */ virtual void init();
    // 24 overrides, no arguments read; one call (act_magic_circle,
    // 0x8028BDA4) ignores r3. Empty here.
    /* 0x10 */ virtual void vfunc_0x10();
    // update: 373 overrides (332 functions), no arguments read; called by
    // the manager's update loop (0x801F8910, 0x801F8B70) right after
    // savePrevPos() and by 18 actor units on themselves; r3 never read.
    // 83 overrides drive J3DFrameCtrl::update/checkPass. Empty here.
    /* 0x14 */ virtual void update();
    // fn_801F5790: box vs. player screens. 3 overrides (tu_80182914,
    // tu_80183544, act_pengator), no arguments read; the update loop calls
    // it when field_0xa4 != 0 (0x801F8B94) and ignores r3.
    /* 0x18 */ virtual void vfunc_0x18();
    // mPrevPos = mPos; never overridden; the update loop calls it right
    // before update() (0x801F88F4, 0x801F8B54).
    /* 0x1C */ virtual void savePrevPos();
    // draw: 337 overrides, no arguments read; the manager's separate draw
    // pass (fn_801F8658, 0x801F86DC) calls it while mDrawEnabled; 199 of
    // the 297 overrides seen reach GX/J3D (GXBegin, GXLoadTexObj, ...)
    // within 3 calls. Empty here.
    /* 0x20 */ virtual void draw();
    // 0x801F4F28. 43 overrides, no arguments read; 81 calls in 37 units, r3
    // never read.
    /* 0x24 */ virtual void vfunc_0x24();
    // 0x801F4E0C: the int goes to fn_8013CC50's 4th argument (all four
    // calls), the floats are added to mPos.x and mPos.y. Never overridden;
    // 8 calls in 8 units (e.g. 0x8017EF80, 0x801D95BC) set r4 (li, or extsb
    // of an s8, so the parameter is a full int), f1 and f2, and ignore r3.
    /* 0x28 */ virtual void vfunc_0x28(s32, f32, f32);
    // 0x801F4850: reads f1-f3 (mPos + (x, y) plus a random offset scaled by
    // f3); returns 0 or 1. Never overridden; all 8 calls (fn_801D8430,
    // 0x8017C4B0, 0x8017FBB4, ...) set f1-f3 and test r3 with clrlwi. 24.
    /* 0x2C */ virtual bool vfunc_0x2C(f32 x, f32 y, f32 scale);
    // 0x801F47CC: if vfunc_0x3C(), passes (view, vfunc_0x3C(), arg2, arg1)
    // to fn_802D8050, which tests arg2 with clrlwi. and stores arg1 with stb
    // (0xFF when cleared). Never overridden; 121 calls in 53 units pass
    // r4 = li 0xFF (114) or a clrlwi 24 value (6, e.g. act_switch narrowing
    // an int), r5 = li 0 or 1, and ignore r3. With (s32, s32) neither the
    // callers' clrlwi nor the missing bool conversion in 0x801F47CC would
    // come out.
    /* 0x30 */ virtual void vfunc_0x30(u8, bool);
    // 0x801F4760: vfunc_0x3C() ? fn_802D800C(view, vfunc_0x3C()) (a bit
    // test, 0 or 1) : 0. Never overridden; 196 calls in 118 units, 190 of
    // them test r3 with clrlwi 24 (the rest store it with stb).
    /* 0x34 */ virtual bool vfunc_0x34();
    // 0x801F471C: returns fn_802D7E04(view, vfunc_0x3C()) (an lbz) without
    // narrowing it. Never overridden; its 3 calls (fn_802B28E4: 0x802B2A5C,
    // 0x802B2A84, 0x802B2AA8) compare r3 with cmpwi 6 / cmpwi 3 and no
    // clrlwi, which a u8 return would need, so the result is a full int
    // (it was declared u8 before G1; the return type is not in the
    // mangled name, so the symbol is unchanged).
    /* 0x38 */ virtual s32 vfunc_0x38();
    // 0x801F4710: the top 5 bits of field_0x90 (srwi 27). Never overridden;
    // 38 calls in 22 units, 32 of them cmpwi r3, 0.
    /* 0x3C */ virtual s32 vfunc_0x3C();
    // 0x801F470C, empty. 6 overrides, all pickups (act_force_gem,
    // act_magic_jar, act_recovery_heart, act_dropped_item,
    // act_crystal_ball, tu_800F7FCC), no arguments read; tu_801A3C50 calls
    // it on itself (0x801A4BF0, 0x801A4C34, 0x801A4CC8) and ignores r3.
    /* 0x40 */ virtual void vfunc_0x40();
    // 0x801F4708, empty. 21 overrides, no arguments read; tu_801A3C50
    // calls it on itself (0x801A3CE0, 0x801A42D0, ...) and ignores r3.
    /* 0x44 */ virtual void vfunc_0x44();
    // 0x801F4704, empty. 7 overrides, all objects a player can lift or
    // throw (act_pot, act_grass, act_stone, act_sign, act_coconut,
    // act_bomb, act_bombchu), no arguments read; called on actors found by
    // serial (tu_8025EB18 0x80263A04) and by act_warp (0x8022713C) and
    // act_seesaw_lift (0x803587F0); r3 never read.
    /* 0x48 */ virtual void vfunc_0x48();
    // 0x801F15BC: the layer (cmpwi 8 in 0x801F165C) and a position go to
    // fn_802265DC(pos, layer, 0) and fn_8020D6AC; returns 0 or 1. 51
    // overrides (48 functions; 30 read r4 and r5, 9 only r5, 9 neither, all
    // return 0/1); 17 calls in 12 units pass r4 = an lwz (the layer), r5 = a
    // stack or member TVec3f, and test r3 with clrlwi 24.
    /* 0x4C */ virtual bool vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos);
    // 0x801F165C: vfunc_0x4C(layer, pos), or pos (x, y) outside the
    // view's box (0xCC, or 0xDC for layer 8 and up); returns 0 or 1. Never
    // overridden; its 8 calls (fn_801F172C) pass a layer and a stack
    // TVec3f and test r3 with clrlwi 24.
    /* 0x50 */ virtual bool vfunc_0x50(s32 layer, const JGeometry::TVec3f* pos);

    // Non-virtual members other units call: every export of game/tu_801EE24C.cpp
    // whose callers pass an actor in r3 (their own `this` in 2,409 of 2,571
    // calls; in the rest r3 holds another actor, e.g. one of the manager's slots
    // or one found by serial). Signatures come from the register use in each
    // body and at the call sites; "N units / M calls" counts the other game
    // units that call it and all its calls (tu_801EE24C's own included). The
    // names stay dtk's placeholders, and symbols.txt carries the mangled name
    // each declaration produces (fn_XXXXXXXX__18ActorBase_804A5770F...), so a
    // parameter type changed here is a symbols.txt rename too; the return type
    // is not part of the mangled name. Where a function takes ints and floats,
    // their order is not visible in the asm; ints first is a guess. Out-pointers
    // are NULL-checked by the body unless said otherwise. Exports that are not
    // members (fn_801EE3C0, fn_801EE434, fn_801EF574, fn_801EF830) are extern
    // "C" below the class.

    // 1 unit / 4 calls. Looks for a 'WARP' actor (fn_801F78F4) on this
    // layer whose (field_0x90 & 0xFF) == 8, moves mPos by the view boxes'
    // offset to the destination layer and sets field_0x4; returns 0 or 1
    // (0x801EE374, 0x801EE3A0; act_small_key tests it with clrlwi).
    bool fn_801EE24C();
    // 3 units / 13 calls. Returns lbl_8046A00C[field_0xc0] (lfsx; 0xc0 is
    // EnemyParam "efsz") in f1.
    f32 fn_801EE4F0();
    // 1 unit / 2 calls (the manager's update loop, 0x801F8B9C, with
    // mActors[i]). fn_801CB71C(&mEmitter100, &mPos, 0, -1), then the same
    // for mEmitter104 with 1.
    void fn_801EE508();
    // 10 units / 37 calls. The nearest player (player == 0: all four, else
    // only player - 1; callers pass li 0 or index + 1): its position in
    // field_0x1ac[] relative to mPos goes normalised to *outDir (stfs
    // 0/4/8), the distance to *outDist.
    void fn_801EE558(JGeometry::TVec3f* outDir, f32* outDist, s32 player);
    // 6 units / 12 calls. fn_801EE558 with extra player filters
    // (fn_80230188, fn_8022DCD8 at 0x801EE9E0, 0x801EE9F0).
    void fn_801EE8F4(JGeometry::TVec3f* outDir, f32* outDist, s32 player);
    // 58 units / 104 calls. Never reads r3, but every caller passes an actor
    // there (`this` 102 times) and its own mSerial in r4, so a member is the
    // likelier source. Looks up the actor `serial` (fn_801F666C, 0x801EECE4)
    // and reports a player relative to it, picked by `mode` (cmpwi 0-5;
    // player is used in mode 2): *outAngle (sth; callers read it back with
    // lhz), *outDir (stw), *outDist (stfs), *outPlayer (stw; -1 in both
    // when the actor is gone, 0x801EED14, 0x801EED24).
    void fn_801EEC98(s32 serial, u16* outAngle, s32* outDir, f32* outDist, s32* outPlayer,
                     s32 player, s32 mode);
    // 7 units / 10 calls. fn_801F2DB4(this, 'NULL', 'NULL', list) with a
    // stack copy of the 0x44-byte list lbl_80469FC8 (0x801EF518-0x801EF560).
    void fn_801EF510();
    // 23 units / 49 calls. fn_801F06F0 with a resource already loaded
    // (fn_801EF830's res, used when no name is given) instead of a name;
    // skipped while mSpriteParams.mMtx is set. f1-f3 are kept in f30, f31,
    // f28. `res` is only passed on; void* is still a guess.
    void fn_801EFD34(void* res, SpriteParams_801F5AE0* params, s32 flags, f32 scaleX, f32 scaleY,
                     f32 arg_f3);
    // 42 units / 59 calls. fn_801F06F0 with a scale mode (cmpwi 0 / 2 picks
    // the PSMTXScale variant, 0x801F031C-0x801F0344). `arg` is only stored
    // to the stack with stb (0x801F02E8), a byte; callers pass li 0 / li 1
    // or a clrlwi 24 value, as for a bool.
    void fn_801F02BC(const char* name, s32 frame, SpriteParams_801F5AE0* params, s32 flags,
                     s32 mode, bool arg, f32 scaleX, f32 scaleY);
    // 116 units / 193 calls. Draws frame `frame` of the "SOB " sprite bank
    // `name` at mPos on this actor's layer: fn_801EF830(field_0x4, &mPos,
    // params, flags, name, frame, NULL) (0x801F0740-0x801F0758). params may
    // be NULL (callers: a stack struct 131 times, 0 61 times); flags is -1,
    // 0 or 7 at most calls.
    // const: the asm has no store through `this`.
    void fn_801F06F0(const char* name, s32 frame, SpriteParams_801F5AE0* params, s32 flags,
                     f32 scaleX, f32 scaleY) const;
    // 53 units / 107 calls. id compared unsigned with 3 and 0x42; uses
    // field_0x4 and mPos.
    void fn_801F0D20(u32 id);
    // 222 units / 964 calls. Starts sound `soundId` on this actor's sound
    // object: fn_80456334(fn_801F6644(mgr, mSlot), &soundId, arg)
    // (0x801F1150), gated by the layer (field_0x4 against 8) and player
    // checks. fn_801F4694 calls it with the EnemyParam "voic" sound
    // (lbl_80469F38[voic - 1]) and 0x40002. The id is compared unsigned
    // (0x801F0EAC: subis/cmplwi against 0x30009). fadeTime goes to
    // fn_80456334, which tests it for 0, stores it at sound+0x30 and
    // converts it with the unsigned 2^52 constant to compute (1 - v) / n
    // into +0x28: JAISound::fadeIn(u32) -> JAISoundFader::fadeInFromOut(u32)
    // (tp JAISound.h). Every caller passes 0. Returns 0 on the early outs,
    // else fn_80456334's result (no caller reads it; the pointee is
    // unknown).
    // const: its lwz of field_0x4 is scheduled above the prologue's stw r28,
    // which MWCC does only for loads through a const `this`.
    void* fn_801F0E34(u32 soundId, u32 fadeTime) const;
    // 4 units / 5 calls. The actor's sound object: fn_801F6644(mgr, mSlot)
    // (0x801F118C-0x801F11A0), mSounds[mSlot] or the manager's shared one.
    ActorSoundObj_804578F4* fn_801F1184() const;
    // 2 units / 2 calls. If the box at 0x80 is not empty and vfunc_0x4C(
    // field_0x4, &mPos) (0x801F11FC-0x801F1210): pushes mPos back from
    // mPrevPos along field_0x98's angle, or respawns at mSpawnPos when out
    // of view; returns 0 or 1.
    bool fn_801F11B4();
    // 8 units / 8 calls, all from vfunc_0x4C overrides (act_sign 0x802D3210,
    // act_coconut 0x802DF4E4) passing their own (layer, pos) and a member
    // (this + 0x230). Gated on *state == 0 (0x801F1354); layer and pos go to
    // fn_8020D6AC and fn_8022461C/fn_80226850 as in the base vfunc_0x4C;
    // returns 0 or 1.
    bool fn_801F132C(s32 layer, const JGeometry::TVec3f* pos, s32* state);
    // 7 units / 9 calls. Collision sweep (via fn_801F172C, which calls
    // vfunc_0x50 on this) of `box` from `from` to `to` on `layer`; the
    // result position goes to *outPos (stfs 0/4/8), out2 is handed to
    // fn_801F172C, which writes a stack TVec3f copy to it (stfs 0/4/8,
    // 0x801F2344-0x801F2350).
    // `flag` reaches fn_801F172C's clrlwi. 24. Returns 0 or 1.
    bool fn_801F23BC(s32 layer, const JGeometry::TVec3f* from, const JGeometry::TVec3f* to,
                     const JGeometry::TBox2f* box, JGeometry::TVec3f* outPos,
                     JGeometry::TVec3f* out2, bool flag);
    // 82 units / 201 calls. Unless field_0x174 > 0, sweeps the box at 0x80
    // from mPrevPos to mPos (fn_801F23BC(this, field_0x4, &mPos, &mPrevPos,
    // box, ...)), moves mPos to the result and stores a vector in *out
    // (0x801F2670-0x801F2688); returns 0 or 1. Callers pass 0 or a stack
    // TVec3f for out and 0 for `flag` (197 of 201).
    bool fn_801F2618(JGeometry::TVec3f* out, bool flag);
    // 6 units / 7 calls. fn_801F7F58(mgr, this, ids, mSerial, 32.0f): the
    // serial of an actor overlapping this one whose mActorId is in `ids` (a
    // 'NULL'-ended list, walked by 4 and compared with cmplw; callers pass
    // .rodata arrays), or -1 (callers test r3 with cmpwi/blt).
    s32 fn_801F26CC(const u32* ids);
    // 30 units / 38 calls. fn_801F8108(this, actorId, mSerial, ...): the
    // serial of an actor with mActorId == actorId (4CC; cmplw) overlapping
    // this one on its layer, or -1.
    s32 fn_801F2718(u32 actorId) const;
    // 16 units / 17 calls. Bit i set for each player i (other than
    // skipPlayer; callers pass -1 in 16 of 17) whose box overlaps mBox
    // around mPos in screen space (0x801F2828-0x801F2844).
    u8 fn_801F2740(s32 skipPlayer);
    // 46 units / 65 calls. Index of a player (any when player < 0) whose box
    // overlaps mBox around mPos, or -1 (0x801F29F8). *outXY gets two floats
    // (stfs 0/4, 0x801F29EC-0x801F29F4) and *outFlag 0 or 1; `arg` is
    // forwarded to fn_8023B9CC and on to fn_802738F0, which tests it with
    // clrlwi. 24 (0x80273AF8); callers pass 1 in 54 of 65.
    s32 fn_801F285C(JGeometry::TVec2f* outXY, s32* outFlag, bool arg, s32 player);
    // 6 units / 6 calls. The mBox overlap test of fn_801F2740 against
    // fn_8022E438(player, skipPlayer) and fn_80239B64(.., arg2, arg3, arg4),
    // whose last two are clrlwi 24 bytes; returns 0 or 1.
    bool fn_801F2A10(s32 skipPlayer, s32 arg2, bool arg3, bool arg4);
    // 27 units / 42 calls. field_0x168 = arg and links the actor into the
    // manager's list (fn_801F84C4); returns the previous field_0x164
    // (0x801F2B64). Callers pass 0, 5, 6 or 2.
    u8 fn_801F2B20(s32 arg);
    // 88 units / 153 calls. Hits every player (except skipPlayer; -1 in 138
    // calls) whose box (fn_8023B190) overlaps mBox on this layer, with
    // fn_80238548(player, damage, &pos, arg); damage -100 means field_0x114
    // (EnemyParam "atcp", 0x801F2D60). Returns the bitmask of players hit.
    u8 fn_801F2B7C(s32 damage, s32 skipPlayer, s32 arg);
    // 27 units / 41 calls. Takes a hit from an overlapping actor
    // (fn_801F82CC) whose mActorId is includeId (or any, 'NULL'), is not
    // excludeId and is not in the 'NULL'-ended list excludeIds (or NULL):
    // fn_801F37AC(this, attacker->field_0x118, ...) (0x801F2ECC). Returns
    // 0 or 1.
    bool fn_801F2DB4(u32 includeId, u32 excludeId, const u32* excludeIds);
    // 20 units / 22 calls. For each player overlapping this actor
    // (fn_801F285C), fn_801F37AC(this, fn_8023B6CC(i), fn_8023E724(i), 0,
    // flag, i); returns whether any hit landed.
    bool fn_801F2FAC();
    // 22 units / 25 calls. fn_801F2FAC(), else fn_801F2DB4(this, 'NULL',
    // 'NULL', NULL) (0x801F30F0-0x801F3104).
    bool fn_801F305C();
    // 9 units / 9 calls. The centre of mBox around mPos in screen space
    // (x, y - z): two floats only (0x801F3128-0x801F3174).
    void fn_801F3128(JGeometry::TVec2f* out);
    // 9 units / 12 calls. mBox2 around mPos in screen space (x, y - z);
    // callers pass a stack box; not NULL-checked.
    void fn_801F317C(JGeometry::TBox2f* out);
    // 11 units / 21 calls. The same for mBox; the manager's actor-vs-actor
    // test (0x801F801C) uses it too.
    void fn_801F31C4(JGeometry::TBox2f* out) const;
    // 2 units / 7 calls. fn_8023DE58(player) == field_0x4, or
    // fn_801F329C(player); returns 0 or 1.
    bool fn_801F320C(s32 player) const;
    // 4 units / 5 calls. fn_8023DE58(player) < 8 && field_0x4 == 8.
    bool fn_801F329C(s32 player);
    // 1 unit / 2 calls (the manager's update loop, with mActors[i]). Fills
    // field_0x1ac[0..3] with each player's position (fn_8023E724), adjusted
    // by fn_801CD150 for players on layer 8 and up while this is on 8.
    void fn_801F32F4();
    // 1 unit / 2 calls (the manager's update loop, between savePrevPos and
    // update). Applies a warp fn_801F349C queued: mPos.xy = field_0x30.xy,
    // field_0x4 = field_0x178, field_0x17d = 1, field_0x174 = field_0x180;
    // while field_0x174 counts down, ends it once mPos is field_0x184 away
    // from field_0x30 (PSVECMag).
    void fn_801F33C4();
    // 4 units / 6 calls. Queues a warp to `pos` on `layer` unless one is
    // pending (field_0x17c) or running (field_0x174 > 0): field_0x30 =
    // *pos, field_0x178 = layer, field_0x17c = 1 (0x801F349C-0x801F34D8).
    void fn_801F349C(s32 layer, const JGeometry::TVec3f* pos);
    // 3 units / 4 calls. If field_0x16c != -1: fn_802D7618(view,
    // field_0x16c).
    void fn_801F34DC();
    // 1 unit / 1 call. mAlive = false, then fn_801F34DC's report.
    void fn_801F3518();
    // 1 unit / 2 calls. If field_0x16c != -1: fn_802D7704(view, field_0x16c,
    // 0, field_0x4).
    void fn_801F355C();
    // 9 units / 16 calls. mAlive = false, then fn_801F355C's report.
    void fn_801F35AC();
    // 13 units / 16 calls. fn_801F3668's report without clearing mAlive.
    void fn_801F3604();
    // 78 units / 153 calls. Clears mAlive, then reports the actor to the
    // view: fn_802D7704(view, field_0x16c, field_0x1a8, field_0x4) when
    // field_0x16c != -1, else fn_802D766C(view, field_0x1a8, field_0x4).
    void fn_801F3668();
    // 1 unit / 3 calls (tu_80204638, on actors from the manager's lists).
    // Returns 1 if field_0x94 == 1 or field_0xc4 is set, else 0. Defined in
    // game/act_thief.cpp.
    bool fn_80204D44();
    // 2 units / 2 calls (act_vulture, act_wallmaster vfunc_0x60s, via
    // EnemyBase_804A4808::fn_801CEA60). Whether this is a live 'BMTY' (Bombchu):
    // mActorId == 'BMTY' and EnemyBase mFlags (0x230) != 3. Defined in
    // game/tu_801CB270.cpp.
    bool fn_801CC8E4();
    // 1 unit / 2 calls (tu_8025EB18, on an actor found by serial). If
    // neither field_0xcc nor field_0xc4: field_0xcc = 1, field_0xd0 = v,
    // returns 1; else 0.
    u8 fn_801F36D4(s32 v);
    // 1 unit / 2 calls (tu_8025EB18). If field_0xc4: clears it, field_0x3c =
    // *v, returns 1; else 0.
    u8 fn_801F3708(const JGeometry::TVec3f* v);
    // 1 unit / 9 calls (tu_8025EB18; callers pass a field_0x4). If not
    // field_0xc4: field_0xc4 = 1, field_0xc8 = field_0x110 = v, field_0xcc
    // = 0, field_0xd0 = -1, returns 1; else 0.
    u8 fn_801F3744(s32 v);
    // 1 unit / 3 calls (tu_801A3C50, always 300). field_0xa4 = v and clears
    // field_0xd5, 0xd8, 0xe4, 0xe8 (and 0xe0 while mLife > 0).
    void fn_801F3780(s32 v);
    // 19 units / 35 calls. Applies one hit: attack (cmpwi 4), the attacker's
    // position (lfs 0/4/8), strength (clamped to 2), type (cmpwi 5 / 2
    // against field_0xb0) and the attacking player (-1 for none; 1 << player
    // tested against field_0xd5). Returns 0 or 1.
    bool fn_801F37AC(s32 attack, const JGeometry::TVec3f* from, s32 strength, s32 type,
                     s32 player);
    // 48 units / 63 calls. Works on field_0xd8-0x10c, field_0x3c and mPos
    // (role unproven); flag is tested with clrlwi. (0x801F3E04; callers
    // pass 1, 0 or a clrlwi 24 value). Returns 1, 0 or -1 (0x801F4630).
    // u8, not bool: EnemyBase_804A4808::update() passes a bool local with
    // clrlwi r4, r31, 24 (0x801CDE0C), which MWCC emits for a bool going to
    // an unsigned char parameter (a bool one takes it with mr).
    s32 fn_801F3D94(u8 flag);
    // 2 units / 6 calls. Plays the actor's EnemyParam "voic" sound
    // (fn_801F0E34(lbl_80469F38[voic - 1], 0)) and, if field_0xe8 == 0,
    // sound 0x40002.
    void fn_801F4694();
    // 0x8019CEAC, out of line in game/tu_80197070.cpp: field_0xb0 = v. One
    // caller (fn_80198828, 0x8019AE20) passes 0x11.
    void fn_8019CEAC(s32 v);
    // 0x8019CEB4, out of line in game/tu_80197070.cpp: returns field_0xb0.
    // Its caller (0x8019AA3C, 0x8019AC40) compares the result with 0xE.
    s32 fn_8019CEB4() const;
    // 0x8017EAF8, out of line in game/tu_8017C01C.cpp: field_0x3c = v (a whole
    // TVec3f copy, psq + stfs). Both callers in fn_8017C40C pass the actor
    // from fn_801F666C and a stack TVec3f at 0x50(r1).
    void fn_8017EAF8(const JGeometry::TVec3f& v);

    // Inline accessors. The spawner calls them out of line (it is too large
    // for the inliner), so they exist as weak functions in its unit at
    // 0x801FF630-0x801FF738; the offset each one touches is in its name
    // when the field is unnamed.
    JGeometry::TVec3f* getPos() { return &mPos; }
    void setPos(const JGeometry::TVec3f& pos) {
        mPos = pos;
        mPrevPos = pos;
    }
    void setSpawnPos(const JGeometry::TVec3f& pos) { mSpawnPos = pos; }
    // 0x80182660, out of line in game/tu_8017EF40.cpp: mPos = pos (a whole
    // TVec3f copy, psq + stfs). Called with the actor and a stack TVec3f.
    void fn_80182660(const JGeometry::TVec3f& pos);
    // 0x8017EF40, out of line in game/tu_8017EF40.cpp: starts sound 0x40038
    // (fn_801F0E34(0x40038, 0)), then vfunc_0x28(2, 0.0f, -24.0f).
    void fn_8017EF40();
    JGeometry::TBox2f* getBox() { return &mBox; }
    JGeometry::TBox2f* getBox2() { return &mBox2; }
    void setBox2(const JGeometry::TBox2f& box) { mBox2 = box; }
    void setActorId(u32 id) { mActorId = id; }
    void setSerial(s32 serial) { mSerial = serial; }
    void setSlot(s32 slot) { mSlot = slot; }
    void setLife(s32 life) { mLife = life; }
    s32 getField_0x4() const { return field_0x4; }
    // The same field (the layer), out of line in game/act_sword_soldier.cpp
    // (0x8015461C): other units call it on their own actors with a bl.
    s32 fn_8015461C() const;
    void setField_0x4(s32 v) { field_0x4 = v; }
    void setField_0x8(s32 v) { field_0x8 = v; }
    void setField_0x90(u32 v) { field_0x90 = v; }
    void setField_0xC0(s32 v) { field_0xc0 = v; }
    void setField_0x110(s32 v) { field_0x110 = v; }
    void setField_0x114(s32 v) { field_0x114 = v; }
    void setField_0x118(s32 v) { field_0x118 = v; }
    void setField_0x16C(s32 v) { field_0x16c = v; }
    void setField_0x170(s32 v) { field_0x170 = v; }
    void setField_0x18C(s32 v) { field_0x18c = v; }
    void setField_0x190(s32 v) { field_0x190 = v; }
    void setField_0x1A8(bool v) { field_0x1a8 = v; }

    // The spawner's 3rd argument; spawned children usually get their
    // parent's. fn_801F5790 compares it with fn_8023DE58(player).
    /* 0x000 vtable */
    /* 0x004 */ s32 field_0x4;
    /* 0x008 */ s32 field_0x8; // also the spawner's 3rd argument; the update
                               // loop counts actors with 0 here and
                               // field_0x1a8 == 1
    /* 0x00C */ JGeometry::TVec3f mPos;      // spawn position; the actor's
                                             // sound object follows it
    /* 0x018 */ JGeometry::TVec3f mPrevPos;  // copy of mPos before update()
    /* 0x024 */ JGeometry::TVec3f mSpawnPos; // set only by the spawner
    /* 0x030 */ JGeometry::TVec3f field_0x30;
    /* 0x03C */ JGeometry::TVec3f field_0x3c;
    /* 0x048 */ JGeometry::TVec3f field_0x48; // 1.0f
    /* 0x054 */ JGeometry::TVec3f field_0x54; // 1.0f
    /* 0x060 */ JGeometry::TBox2f mBox;  // (-8, -8)-(8, 8)
    /* 0x070 */ JGeometry::TBox2f mBox2; // zero; the spawner copies mBox here if
                                // init() left it all zero
    // A third box (min i, max f), relative to mPos like mBox: fn_801F2618
    // sweeps it against the map (it hands &field_0x80 to fn_801F23BC as the
    // box), fn_801F11B4 tests its width and height, and 182
    // actor units set it with four stfs (act_log_monitor_npc's init sets it
    // to the same values as mBox).
    /* 0x080 */ JGeometry::TBox2f field_0x80;
    // The spawner's 5th argument, a bit field each actor decodes. Unsigned:
    // vfunc_0x3C takes its top 5 bits with srwi (0x801F4710), act_rat's draw
    // tests it with cmplwi (0x800F3D7C), and the written units shift it as
    // u32.
    /* 0x090 */ u32 field_0x90;
    // u16: 311 of the 324 game loads of an actor's 0x94 are lhz (e.g.
    // fn_801CC910's cmplwi tests against 0 and 3).
    /* 0x094 */ u16 field_0x94;
    /* 0x098 */ s32 field_0x98;
    /* 0x09C */ s32 field_0x9c;
    /* 0x0A0 */ s32 field_0xa0;
    /* 0x0A4 */ s32 field_0xa4;
    /* 0x0A8 */ s32 field_0xa8;
    /* 0x0AC */ s32 field_0xac;
    /* 0x0B0 */ s32 field_0xb0;
    /* 0x0B4 */ s32 field_0xb4;
    /* 0x0B8 */ s32 field_0xb8;
    /* 0x0BC */ s32 field_0xbc;
    /* 0x0C0 */ s32 field_0xc0; // EnemyParam.csv column "efsz"
    /* 0x0C4 */ u8 field_0xc4;  // update loop: updates the actor if set
    /* 0x0C8 */ s32 field_0xc8;
    /* 0x0CC */ u8 field_0xcc;  // update loop: updates the actor if set
    /* 0x0D0 */ s32 field_0xd0;
    /* 0x0D4 */ u8 field_0xd4;
    /* 0x0D5 */ u8 field_0xd5;
    /* 0x0D8 */ s32 field_0xd8;
    /* 0x0DC */ s32 field_0xdc;
    /* 0x0E0 */ s32 field_0xe0;
    /* 0x0E4 */ s32 field_0xe4;
    /* 0x0E8 */ s32 field_0xe8;
    /* 0x0EC */ JGeometry::TVec3f field_0xec;
    /* 0x0F8 */ s32 field_0xf8;
    /* 0x0FC */ s32 field_0xfc;
    /* 0x100 */ JPABaseEmitter* mEmitter100; // released by the destructor
    /* 0x104 */ JPABaseEmitter* mEmitter104; // released by the destructor
    /* 0x108 */ s32 mLife;       // EnemyParam.csv column "life"
    /* 0x10C */ s32 field_0x10c;
    /* 0x110 */ s32 field_0x110; // spawner's 7th argument (default -1)
    /* 0x114 */ s32 field_0x114; // EnemyParam.csv column "atcp"
    /* 0x118 */ s32 field_0x118; // EnemyParam.csv column "atcn"
    // mAlive: the manager's update pass deletes the actor (virtual dtor)
    // when it is false, and its collision queries (fn_801F8108,
    // fn_801F82CC, ...) and draw pass skip it; fn_801F3668 clears it. Some
    // actors clear it in init() (TMI2), which removes them at once.
    /* 0x11C */ bool mAlive;
    /* 0x11D */ bool mDrawEnabled; // the draw loop skips the actor when false
    /* 0x11E */ u8 field_0x11e;    // update loop: updates the actor if set
    /* 0x11F */ u8 field_0x11f;    // draw/update loops: still drawn/updated
                                   // while the game is paused (fn_80134CC8)
    /* 0x120 */ SpriteParams_801F5AE0 mSpriteParams; // the actor's own
    /* 0x15C */ ActorBase_804A5770* mListPrev; // the manager's actor list at
    /* 0x160 */ ActorBase_804A5770* mListNext; // ActorManager_804A5800 0x2018 (fn_801F84C4)
    /* 0x164 */ u8 field_0x164;
    /* 0x165 */ u8 mInList;           // set by fn_801F84C4 when linked
    /* 0x168 */ s32 field_0x168;
    /* 0x16C */ s32 field_0x16c; // spawner's 6th argument (default -1)
    /* 0x170 */ s32 field_0x170; // set by the spawner from fn_802D39FC(fn_802D6A9C())
    /* 0x174 */ s32 field_0x174;
    /* 0x178 */ s32 field_0x178;
    /* 0x17C */ u8 field_0x17c;
    /* 0x17D */ u8 field_0x17d;
    /* 0x180 */ s32 field_0x180; // 20
    /* 0x184 */ f32 field_0x184; // 40.0f
    /* 0x188 */ s32 field_0x188;
    /* 0x18C */ s32 field_0x18c; // EnemyParam.csv column "dasu"
    /* 0x190 */ s32 field_0x190; // EnemyParam.csv column "voic"
    /* 0x194 */ u8 field_0x194;
    /* 0x195 */ u8 field_0x195;
    /* 0x196 */ u8 field_0x196;
    /* 0x198 */ s32 mSerial;  // the spawner's running spawn count, its return value
    /* 0x19C */ s32 mSlot;    // index into ActorManager_804A5800::mActors
    /* 0x1A0 */ u32 mActorId; // ActorId; 'NULL' until spawned
    /* 0x1A4 */ s32 field_0x1a4;
    /* 0x1A8 */ bool field_0x1a8; // EnemyParam.csv column "dstr" != 0
    /* 0x1AC */ JGeometry::TVec3f field_0x1ac[4];
    /* 0x1DC */ ActorSub1DC_801EAD68 field_0x1dc;
}; // Size: 0x230

// Exports of game/tu_801EE24C.cpp that are not ActorBase_804A5770 members
// (their callers never pass an actor in r3). extern "C" keeps the dtk names
// as link names until that unit names them.
extern "C" {

// 4 units / 8 calls. A map tile code (callers pass the lhz/clrlwi 16 result
// of fn_80226850, e.g. act_spreading_fire 0x802BC9F8) in one of the ranges
// 0x68-0x6B, 0x70-0x71, 0x74-0x75, 0xAA-0xAB or 0x4E, 0xFA, 0x146, 0x159.
bool fn_801EE3C0(u16 tile);

// 104 units / 258 calls. Releases an emitter: lets one inside the view's
// visible box (0xCC-0xD8) run out (clearStatus(Immortal),
// becomeInvalidEmitter()), deletes one outside it (forceDeleteEmitter,
// 0x801EE4D8). The destructor inlines the same code for mEmitter100/104;
// derived actors call it for their own emitters (lwz 0x230-0x23C).
void fn_801EE434(JPABaseEmitter* emitter);

// 7 units / 8 calls. Loads the "SOB " resource `sobName` (a .rodata string)
// through the archive at lbl_8053AAF8->0xA4 (vtable slot 0x18,
// 0x801EF7B4-0x801EF7F4) and hands it with pos, arg1 and params (a
// SpriteParams_801F5AE0 or NULL, then defaults are used) to fn_802F86CC
// (lbl_8053AB10->0x20).
void fn_801EF574(const char* sobName, s32 arg1, const JGeometry::TVec3f* pos,
                 SpriteParams_801F5AE0* params);

// 20 units / 34 calls. Draws frame `frame` of a "SOB " sprite at pos on
// layer `layer` (cmpwi 8; GameView 0x131DF): `name` is loaded like
// fn_801EF574 does, or when NULL `res` is used (fn_802F745C, 0x801EFC74).
// flags: -1 skips, the low 10 bits are an index, 0x400 a mode bit. Callers
// pass an actor's field_0x4 and &mPos.
void fn_801EF830(s32 layer, const JGeometry::TVec3f* pos, SpriteParams_801F5AE0* params, s32 flags,
                 const char* name, s32 frame, void* res);

} // extern "C"

// Not written yet in game/tu_801A3C50.cpp (a member of an actor class derived
// from ActorBase_804A5770, so it takes the actor in r3): extern "C" keeps the
// dtk name until that unit defines it. 1 unit (tu_802242F4). Stores 0 in
// field 0x108 and `player` in fields 0x10C and 0xA0, then calls vtable slot
// 0x44.
extern "C" {
void fn_801A3C50(ActorBase_804A5770* actor, s32 player);
} // extern "C"

// game/tu_801A3C50.cpp (not written yet; C linkage, no mangled names): map
// checks act_deadrock's update() makes with its mSerial, its box and its
// top-down position.
extern "C" {
s32 fn_801A6108(s32 serial, const JGeometry::TBox2f* box, const JGeometry::TVec2f* pos);
s32 fn_801A4D1C(s32 serial, const JGeometry::TBox2f* box, const JGeometry::TVec2f* pos);
s32 fn_801A46FC(s32 serial, const JGeometry::TBox2f* box, const JGeometry::TVec2f* pos);
s32 fn_801A4F5C(s32 serial, const JGeometry::TBox2f* box, const JGeometry::TVec2f* pos);
} // extern "C"

#endif
