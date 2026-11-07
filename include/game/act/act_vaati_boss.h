#ifndef GAME_ACT_ACT_VAATI_BOSS_H
#define GAME_ACT_ACT_VAATI_BOSS_H

// Owner: game/act_vaati_boss.cpp (not written yet). The draw helpers are in
// game/tu_801EDBA4.cpp, the member at 0x568 in game/tu_801E667C.cpp.

#include "types.h"
#include "game/EnemyBase.h"
#include "game/SpriteAnim.h"
#include "game/cls_8021BCBC.h"
#include "game/cls_801EB8CC.h" // the member at 0x464
#include "game/DampedSine.h"

// An element of the 4-entry array at 0x2A8 of Vaati, built by
// __construct_array with the constructor 0x801D6A88 (game/act_vaati_boss.cpp).
// No vtable: named after that constructor. fn_801D789C (game/tu_801D6CEC.cpp)
// is called on the element after its cls_8021BCBC has been set up.
struct cls_801D6A88 {
    cls_801D6A88();
    void fn_801D789C();

    /* 0x00 */ u8 field_0x0; // cleared by fn_801D789C
    /* 0x01 */ u8 field_0x1[0x4 - 0x1]; // not yet known
    /* 0x04 */ s32 field_0x4; // cleared by fn_801D789C
    /* 0x08 */ s32 field_0x8; // cleared by fn_801D789C
    /* 0x0C */ cls_8021BCBC field_0xc;
}; // Size: 0x18

// An element of the 16-entry array at 0x308 of Vaati, built by
// __construct_array in the actor's constructor (0x801D686C) with the
// constructor 0x801D6A58 (game/act_vaati_boss.cpp). No vtable: named after
// that constructor. fn_801D7874 (game/tu_801D6CEC.cpp) sets the fields below.
struct cls_801D6A58 {
    cls_801D6A58();
    // 0x801D7874: sets field_0x0 to -1, the two bytes and the three floats to 0.
    void fn_801D7874();
    // 0x801D7830: damps the two floats (0.97) and lets field_0x10 fall by 0.2 per call, to -4.
    void fn_801D7830();

    /* 0x00 */ s32 field_0x0;
    /* 0x04 */ u8 field_0x4;
    /* 0x05 */ u8 field_0x5;
    /* 0x08 */ f32 field_0x8;
    /* 0x0C */ f32 field_0xc;
    /* 0x10 */ f32 field_0x10;
}; // Size: 0x14

// The shake offset at 0x52c of Vaati: three DampedSine_801E4A18
// (game/tu_801E4A18.cpp, 0x14 bytes each, `value` first) for x, y and z.
// The actor's constructor (0x801D686C) calls fn_801E4B0C on 0x52c, 0x540 and
// 0x554 and fn_801D4ED4 steps them with fn_801E4A18. No vtable, ctor or dtor:
// named after its lowest-address function, 0x801D6CEC (game/tu_801D6CEC.cpp,
// not written yet; fn_801D6D08 there starts the z sine at 0x28).
struct cls_801D6CEC {
    cls_801D6CEC() {
        fn_801E4B0C(&field_0x0);
        fn_801E4B0C(&field_0x14);
        fn_801E4B0C(&field_0x28);
    }
    // Writes (x value, y value, 0.0f) to *out.
    void fn_801D6CEC(JGeometry::TVec3f* out);
    // 0x801D6D08: starts the z sine. 0x801D6D38: starts the x and y sines along dir (length 20).
    void fn_801D6D08();
    void fn_801D6D38(const JGeometry::TVec2f* dir);
    /* 0x00 */ DampedSine_801E4A18 field_0x0;
    /* 0x14 */ DampedSine_801E4A18 field_0x14;
    /* 0x28 */ DampedSine_801E4A18 field_0x28;
}; // Size: 0x3C

// The member at 0x568 of the Vaati boss actor (code in game/tu_801E667C.cpp,
// called from the actor's functions at 0x801D5B58 and 0x801D6398 and from the
// call site 0x801D6904 in its constructor (0x801D686C); named after its constructor, 0x801E6AB8 (no
// vtable). A short timed sequence that plays an animation on the players in
// mActive and ends after state 4. mState steps 0 -> 1 -> 2 -> 3 -> 4.
class cls_801E6AB8 {
public:
    cls_801E6AB8();
    // 0x801D69FC: destroys mAnim (its J3DFrameCtrl's vtable is stored at
    // 0x34); the member with the delete flag is the implicit destructor.
    ~cls_801E6AB8();
    // 0x801E667C: steps the timer and the state; true once state 4 is reached.
    bool fn_801E667C();
    // 0x801E67C4: enters state `state`.
    void fn_801E67C4(s32 state);
    // 0x801E6A84: starts the sequence (state 1) if it is idle.
    void fn_801E6A84(s32 arg);

    /* 0x00 */ s32 mState;
    /* 0x04 */ s32 mTimer;
    /* 0x08 */ s32 field_0x8; // set by fn_801E6A84; state 4 acts only when 0
    /* 0x0C */ u8 mActive[4]; // per player
    /* 0x10 */ SpriteAnim_801D267C mAnim;
}; // Size: 0x48

// The member at 0x448 of Vaati, defined in game/tu_8021775C.cpp. Six particle
// emitters from spawnEmitter_8013CC50, freed with releaseEmitter_801EE434, and the owner (Vaati) at
// 0x18. The constructor (0x80218020) clears all seven words; named after the
// destructor 0x80217F80, as there is no vtable.
class VaatiBoss_804A4958;
class JPABaseEmitter;

class cls_80217F80 {
public:
    // 0x80218020: clears the emitters and the owner.
    cls_80217F80();
    // 0x80217F80: frees the six emitters (releaseEmitter_801EE434 on 0x0..0x14).
    ~cls_80217F80();
    // 0x802179EC: frees and clears the emitters at 0x10 and 0x14.
    void fn_802179EC();
    // 0x80217A30: frees and clears the emitters at 0x8 and 0xC.
    void fn_80217A30();
    // 0x80217A74: frees and clears the emitters at 0x0 and 0x4.
    void fn_80217A74();
    // 0x8021798C: moves the emitters at 0x0 and 0x4 to the owner's position.
    void fn_8021798C();
    // 0x8021775C: moves the emitters at 0x10 and 0x14 to a position between
    // the owner's 0x600 and 0x60C vectors, by the factor t.
    void fn_8021775C(f32 t);
    // 0x80217874: moves the emitters at 0x8 and 0xC to a position between the
    // owner's 0x60C vector and its mPos, by the factor t.
    void fn_80217874(f32 t);
    // 0x80217AB8: spawns effects 0x224 and 0x225 at the owner's 0x60C vector in
    // the empty slots 0x10 and 0x14.
    void fn_80217AB8();
    // 0x80217BC8: spawns effects 0x22F and 0x230 at the owner's mPos in the
    // empty slots 0x8 and 0xC.
    void fn_80217BC8();
    // 0x80217CD8: spawns effects 0x228 and 0x229 at the owner's mPos in the
    // empty slots 0x0 and 0x4.
    void fn_80217CD8();
    // 0x80217DE8: spawns effects 0x231 and 0x232 at the owner's mPos; not kept.
    void fn_80217DE8();
    // 0x80217ED4: spawns the six effects of lbl_8046A5A0 at the owner's mPos;
    // not kept.
    void fn_80217ED4();

    /* 0x00 */ JPABaseEmitter* mEmitters[6];
    /* 0x18 */ VaatiBoss_804A4958* mOwner;
}; // Size: 0x1C

// The member at 0x4C4 of Vaati: its destructor (dtor_801D7730, tu_801D6CEC,
// not written yet) clears the 16 actor serials in field_0x0 and the
// flags of the actors they name. No vtable; named after that destructor.
class cls_801D7730 {
public:
    // 0x801D76B4: sets the 16 serials to -1 and the rest to 0 (0.0f for the floats).
    cls_801D7730();
    ~cls_801D7730();
    // 0x801D7638: clears mAlive of the actors the serials name.
    void fn_801D7638();
    // Shotgun pass: the ring's per-frame steps (names by address).
    void fn_801D77B4();                                          // reset
    void fn_801D74E0(const JGeometry::TVec3f* pos, s32 layer);   // restart and spawn
    void fn_801D6E20(const JGeometry::TVec3f* pos);              // follow
    void fn_801D70BC(const JGeometry::TVec3f* pos, s32 player);  // fling one
    void fn_801D72C0(const JGeometry::TVec3f* pos);              // fling all
    /* 0x00 */ s32 field_0x0[0x10];
    /* 0x40 */ s32 field_0x40; // cleared by the constructor
    /* 0x44 */ s32 field_0x44; // cleared by the constructor
    /* 0x48 */ f32 field_0x48; // cleared by the constructor
    /* 0x4C */ f32 field_0x4c; // cleared by the constructor
    /* 0x50 */ f32 field_0x50; // cleared by the constructor
    /* 0x54 */ s16 field_0x54; // cleared by the constructor
    /* 0x56 */ s16 field_0x56; // cleared by the constructor
    /* 0x58 */ u8 field_0x58; // cleared by the constructor
    /* 0x5C */ f32 field_0x5c; // cleared by the constructor
    /* 0x60 */ f32 field_0x60; // cleared by the constructor
    /* 0x64 */ s32 field_0x64; // cleared by the constructor
}; // Size: 0x68

// The boss Vaati (actor 'BOGF', EnemyBase_804A4808 child), named after its
// vtable at 0x804A4958 (its draw is slot 0x20, fn_801EDBA4). Only the members
// the written code uses are known.
class VaatiBoss_804A4958 : public EnemyBase_804A4808 {
public:
    VaatiBoss_804A4958();
    // 0x801D46FC: the deleting destructor (slot 0x08). Destroys the members
    // from 0x568 down, then the base; deletes unless the flag is <= 0.
    virtual ~VaatiBoss_804A4958();
    /* 0x0C */ virtual void init();
    /* 0x20 */ virtual void draw();
    /* 0x24 */ virtual void vfunc_0x24(); // empty override
    // 0x801D652C: gAudioMgr_8053AF80->fn_80458F9C(60).
    /* 0x54 */ virtual void vfunc_0x54();
    /* 0x74 */ virtual void vfunc_0x74();
    /* 0x88 */ virtual void vfunc_0x88();
    /* 0x90 */ virtual void vfunc_0x90(u8 mapHit);
    // 0x801D5338: advances the sprite animation (mAnim.fn_801D1C18) unless
    // mState is 6, 7, 0xB or 0x17..0x1A (no call in those states).
    /* 0x94 */ virtual void vfunc_0x94();
    /* 0x98 */ virtual void vfunc_0x98();
    /* 0x9C */ virtual void vfunc_0x9C(u8 mapHit);
    /* 0xA0 */ virtual void vfunc_0xA0();

    void fn_801D63D0();
    void fn_801D4C70();
    void fn_801E4790(bool arg);
    void fn_801E4758();

    // 0x801EDC0C (draw's body): adds the offset of field_0x52c to mPos,
    // shakes mPos.x by 1.0f every other frame in state 0xB once the timer is
    // 0x3C or more, then draws "enemy_both_sob.bin" frame `frame`
    // (fn_801F02BC) on the lower layers, else fn_801EDD0C.
    void fn_801EDC0C(s32 frame);
    // 0x801EDD0C: draws the 'TIMG' resource e_gufu_<mAnim.mCel + 1>.bti with
    // a matrix scaled in z by 1.0f + the z shake (at least 0.001f); in
    // state 5 it draws it once more first and scales the matrix by a
    // height-dependent factor (fn_802F5FF4 of mPos.z).
    void fn_801EDD0C();
    // 0x801D5D84: the state setter (a jump table on `state`, up to 0x1A; it
    // uses the emitters at 0x448).
    void fn_801D5D84(s32 state);
    // 0x801E4348: runs the attack-state checks for one player in range and
    // returns whether it acted (game/tu_801E3618.cpp).
    bool fn_801E4348();
    // 0x801E4D54 (game/tu_801E4D54.cpp): clears mFlags' 0x4000 bit, zeroes
    // field_0x5ce and sets the float at 0x4BC (field_0x464's field_0x58) to 0.
    void fn_801E4D54();
    // 0x801E4D74: the same, with 0x32 at 0x5CE and 255.0f at 0x4BC.
    void fn_801E4D74();
    // 0x801E5BA8 (game/tu_801E4D54.cpp): the first element of field_0x308
    // whose serial is actor's mSerial gets a direction (x, y) and a z. flag 0:
    // dir normalised and doubled, z 1.5. Otherwise a player within 160 units
    // of mPos whose field_0x2a8 entry has field_0x0 clear gives the sin and
    // cos of its angle, else dir is normalised; both are quadrupled, z 3.
    void fn_801E5BA8(const ActorBase_804A5770* actor, const JGeometry::TVec2f* dir, bool flag);
    // 0x801E5754 (game/tu_801E4D54.cpp): steps the four player records in
    // field_0x2a8 (countdowns, the 0x2AC timers) against this boss's layer and
    // the positions of the players.
    void fn_801E5754();

    /* 0x2A8 */ cls_801D6A88 field_0x2a8[0x4];
    /* 0x308 */ cls_801D6A58 field_0x308[0x10];
    /* 0x448 */ cls_80217F80 field_0x448;
    /* 0x464 */ cls_801EB8CC field_0x464;
    /* 0x4C4 */ cls_801D7730 field_0x4c4;
    /* 0x52C */ cls_801D6CEC field_0x52c;
    /* 0x568 */ cls_801E6AB8 field_0x568;
    /* 0x5B0 */ s32 field_0x5b0;
    /* 0x5B4 */ s32 field_0x5b4;
    /* 0x5B8 */ s32 field_0x5b8;
    /* 0x5BC */ f32 field_0x5bc;
    /* 0x5C0 */ f32 field_0x5c0;
    /* 0x5C4 */ f32 field_0x5c4;
    /* 0x5C8 */ f32 field_0x5c8;
    /* 0x5CC */ u8 field_0x5cc;
    /* 0x5CD */ u8 field_0x5cd;
    /* 0x5CE */ u8 field_0x5ce; // 0 or 0x32, set by fn_801E4D54 / fn_801E4D74
    /* 0x5CF */ u8 field_0x5cf;
    /* 0x5D0 */ u8 field_0x5d0;
    /* 0x5D1 */ u8 field_0x5d1;
    /* 0x5D2 */ s16 field_0x5d2;
    /* 0x5D4 */ s16 field_0x5d4;
    /* 0x5D8 */ s32 field_0x5d8;
    /* 0x5DC */ s32 field_0x5dc;
    /* 0x5E0 */ f32 field_0x5e0;
    /* 0x5E4 */ f32 field_0x5e4;
    /* 0x5E8 */ u8 field_0x5e8;
    /* 0x5E9 */ u8 field_0x5e9;
    /* 0x5EA */ u8 field_0x5ea[0x5EC - 0x5EA];
    /* 0x5EC */ f32 field_0x5ec;
    /* 0x5F0 */ f32 field_0x5f0;
    /* 0x5F4 */ f32 field_0x5f4;
    /* 0x5F8 */ f32 field_0x5f8;
    /* 0x5FC */ s32 field_0x5fc; // effect layer (spawnEmitter_8013CC50's first argument)
    /* 0x600 */ JGeometry::TVec3f field_0x600;
    /* 0x60C */ JGeometry::TVec3f field_0x60c;
}; // Size: 0x618 (the spawner's __nw__, li r3 0x618 before the 0x801FC280 ctor call)

#endif
