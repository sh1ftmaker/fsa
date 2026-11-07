#ifndef GAME_TU_801213EC_H
#define GAME_TU_801213EC_H

// Exports of game/tu_801213EC.cpp (not written yet). Only what the escort
// actors call is declared.

#include "game/ActorBase.h"
#include "game/RailBase.h"
#include "game/SpriteAnim.h"

// The escort base class (vtable 0x8049E0C8), parent of EscortWoman_8049E1D0
// ('OPGL', act_escort_woman) and ZoraEscort_8049DF78 ('FAZR', act_zora_escort).
// Named after its vtable. Both children's constructors call its constructor
// (fn_801247C8) and both destructors its destructor (__dt__19EscortBase_8049E0C8Fv).
class EscortBase_8049E0C8 : public ActorBase_804A5770 {
public:
    // __ct__19EscortBase_8049E0C8FPCcll (tu_801213EC): the constructor. Both children pass their
    // SOB file name, a number and a flag (0x2E6 and 1 for the escort woman).
    EscortBase_8049E0C8(const char* sobName, s32 arg2, s32 arg3);
    /* 0x08 */ virtual ~EscortBase_8049E0C8();
    /* 0x14 */ virtual void update(); // fn_80124278; the vtable's update slot
    // Slots 0x54-0x5C: the callers test 0x54's byte result (escort woman's 0x6C-state code);
    // 0x58 is a flag, 0x5C returns a state number (3 here).
    /* 0x54 */ virtual bool vfunc_0x54();
    /* 0x58 */ virtual bool vfunc_0x58();
    /* 0x5C */ virtual s32 vfunc_0x5C();
    // Returns a byte: the escort woman's override (act_escort_woman.h) does,
    // and fn_80131D98 reads it as bool.
    /* 0x60 */ virtual u8 vfunc_0x60();
    /* 0x64 */ virtual void vfunc_0x64();
    /* 0x68 */ virtual void vfunc_0x68();
    // fn_80123E80: stores the argument at 0x33c and the old value at 0x340,
    // returns 1. The zora's and the escort woman's code call it with 0xC.
    /* 0x6C */ virtual s32 vfunc_0x6C(s32 state);
    // Slots 0x70-0x94 (the base's own entries: fn_8012140C, fn_80121410,
    // 0x78, fn_80123D30, fn_80123B94, fn_80121414-fn_80121424). Only 0x78 is
    // known; the others are declared to number the slots the escort woman
    // overrides, with her overrides' signatures.
    /* 0x70 */ virtual void vfunc_0x70();
    /* 0x74 */ virtual void vfunc_0x74();
    // 0x80123D88 (tu_801213EC): returns 1 when the escort's state word
    // (0x344) is in range and the test passes, else 0. The escort woman's
    // override calls it for states 4 and 5.
    /* 0x78 */ virtual s32 vfunc_0x78() const;
    /* 0x7C */ virtual void vfunc_0x7C();
    /* 0x80 */ virtual void vfunc_0x80();
    /* 0x84 */ virtual s32 vfunc_0x84();
    /* 0x88 */ virtual s32 vfunc_0x88();
    /* 0x8C */ virtual void vfunc_0x8C();
    /* 0x90 */ virtual void vfunc_0x90();
    /* 0x94 */ virtual void vfunc_0x94();
    // Slots 0x98-0xBC are not known yet; declared to number 0xC0 (the zora overrides it).
    // fn_80123AB4: r4 and r5 are fn_801EEC98's last two parameters, f1 the distance limit its
    // result is compared with.
    /* 0x98 */ virtual void vfunc_0x98(s32 arg1, s32 arg2, f32 arg3);
    // fn_80123394 (tu_801213EC, 0x720 bytes, not written yet; the zora forwards to it).
    // Parameters read off its register use: r4 (tested >= 0), f1, f2, f3 and f5 (f4 is not
    // read).
    /* 0x9C */ virtual void vfunc_0x9C(s32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6);
    /* 0xA0 */ virtual void vfunc_0xA0();
    /* 0xA4 */ virtual void vfunc_0xA4();
    /* 0xA8 */ virtual void vfunc_0xA8(f32* x, f32* y, s32* player); // fn_80122E34
    /* 0xAC */ virtual void vfunc_0xAC(f32* x, f32* y, s32* player); // fn_80122D38
    /* 0xB0 */ virtual void vfunc_0xB0(f32* x, f32* y, s32* player); // fn_80122CAC
    /* 0xB4 */ virtual void vfunc_0xB4(f32* x, f32* y, s32* player); // fn_80122B98
    /* 0xB8 */ virtual void vfunc_0xB8(f32* x, f32* y, s32* player); // fn_80122A9C
    /* 0xBC */ virtual bool vfunc_0xBC(); // fn_801229E4: the escort's box overlaps a player's
    // fn_80121428: returns 1 (the zora overrides it, also returning 1).
    /* 0xC0 */ virtual s32 vfunc_0xC0();
    // fn_80121430: returns 0.
    /* 0xC4 */ virtual bool vfunc_0xC4();
    /* 0xC8 */ virtual bool vfunc_0xC8(const JGeometry::TVec3f& vec, f32 arg); // fn_80121BB8
    // fn_80121DB0 (tu_801213EC): clears three bytes at 0x269-0x26B, returns 1.
    // The zora's fn_801200F8 and the escort woman's vfunc 0x74 call it.
    s32 fn_80121DB0();
    // fn_801216D0 (tu_801213EC): the index of the player (0-3) whose fn_8023ECF0 is `player`, else -1.
    s32 fn_801216D0(s32 player);
    // fn_80121438 (tu_801213EC): update's push away from a magic circle ('CIRC') the escort
    // overlaps; true with dir set (xy, length 1.6), else false and dir zero.
    bool fn_80121438(JGeometry::TVec3f* dir);
    // fn_80121764 (tu_801213EC): when field_0x332 is set, kills the life-meter actors whose serials
    // are in field_0x364 and clears the entries.
    void fn_80121764();
    // fn_80121ABC (tu_801213EC): spawns field_0x360 life-meter actors ('LIMT') in a row at the
    // escort, hidden, and stores their serials in field_0x364.
    void fn_80121ABC();
    // fn_801217EC (tu_801213EC): update's life-meter refresh (dead: mLife == 0).
    void fn_801217EC(bool dead);
    // Dead-stripped helpers inlined into fn_801217EC (its redundant field_0x332 and index tests).
    void respawnLifeMeters();
    void spawnLifeMeter(s32 index);
    // fn_80121D68 (tu_801213EC): fills mRail from the escort's position; returns 1.
    s32 fn_80121D68();
    // fn_80121DC8 (tu_801213EC): the xy distance from the player's position to the escort's.
    f32 fn_80121DC8(s32 player);
    // fn_80121E40 (tu_801213EC): eases field_0x3c toward zero on the axis field_0x98 selects, pulls
    // its z down by 0.2, moves mPos by it; true (and z = 0) once mPos.z is at or below 0.
    bool fn_80121E40();
    // fn_80121F34 (tu_801213EC): sets field_0x3c by field_0x98 (0-3: an axis, 2.5 either way).
    void fn_80121F34();
    // fn_80121FA8, fn_8012202C, fn_801220C4 (tu_801213EC): an effect (0x33A) 24 units below the
    // escort and a sound; fn_8012202C also starts animation 9.
    void fn_80121FA8();
    void fn_8012202C();
    void fn_801220C4();
    void fn_80122148();
    void fn_80122548();
    // fn_80122898 (tu_801213EC): asks the player at the escort to talk (message msg); remembers
    // the player in field_0x34c (-1 when none); returns it.
    s32 fn_80122898(u16 msg);
    // fn_801227D4 / fn_80122804 (tu_801213EC): count a frame counter up; true (and reset) at 90 / 15.
    bool fn_801227D4();
    bool fn_80122804();
    // fn_8012285C (tu_801213EC): fn_8022EA0C of the state word 0x344 when it is 0-3, else -1.
    s32 fn_8012285C() const;
    // fn_80124448 (tu_801213EC): calls ActorBase::init, then the vtable's 0x64
    // slot, then sets the state fields 0x60-0x6C. The escort woman's init calls it.
    void fn_80124448();
    // fn_80122834 (tu_801213EC): returns true when the word at 0x108 and the
    // byte at 0x333 are both zero. The escort woman's vfunc_0x90 calls it.
    bool fn_80122834();
    // The escort's sprite animation; act_escort_woman's vfunc 0x70 sets its key tables.
    /* 0x230 */ SpriteAnim_801D267C mAnim;
    /* 0x268 */ cls_800FE3EC mRail; // fn_80121D68 fills it (fn_800FE3EC); fn_80121DB0 clears its bytes 1-3
    /* 0x330 */ u8 field_0x330; // 1 after construction
    /* 0x331 */ u8 field_0x331; // 1 after construction
    /* 0x332 */ u8 field_0x332; // the constructor's third argument
    /* 0x333 */ u8 field_0x333;
    /* 0x334 */ u8 field_0x334;
    /* 0x335 */ u8 field_0x335;
    /* 0x336 */ s16 field_0x336;
    /* 0x338 */ s32 field_0x338; // the constructor's second argument
    /* 0x33C */ s32 field_0x33c; // a state word, 0 after construction; the zora's update skips update() while it is 0xF
    /* 0x340 */ s32 field_0x340; // the previous state (vfunc_0x6C)
    /* 0x344 */ s32 field_0x344; // -1 after construction
    /* 0x348 */ s32 field_0x348; // -1
    /* 0x34C */ s32 field_0x34c; // -1
    /* 0x350 */ s32 field_0x350; // a frame counter (fn_801227D4)
    /* 0x354 */ s32 field_0x354;
    /* 0x358 */ s32 field_0x358; // a frame counter (fn_80122804)
    /* 0x35C */ s32 field_0x35c; // the low byte of field_0x90, set by the escort woman's vfunc_0x64
    /* 0x360 */ s32 field_0x360; // 1 after construction
    /* 0x364 */ s32 field_0x364[5]; // -1 after construction (0x364-0x374)
    /* 0x378 */ s32 field_0x378;
    /* 0x37C */ f32 field_0x37c;
    /* 0x380 */ f32 field_0x380;
    /* 0x384 */ char mSobName[0x40]; // the constructor's first argument (strcpy)
    // Size: 0x3C4
};

// fn_80124600 (tu_801213EC): with no escort list (fn_80131F0C's first word 0) the
// actor of serial lbl_80539F30 (3 when there is none), else each 'FAZR' escort,
// is asked through its vtable's 0x5C slot with dir; returns a state number
// (0-3). fn_80231DE4 (tu_8022AD64) lets a screen change through unless it is
// 0 or 1. const is a guess (dir goes on to the virtual call).
s32 fn_80124600(const JGeometry::TVec2<s32>* dir);

#endif
