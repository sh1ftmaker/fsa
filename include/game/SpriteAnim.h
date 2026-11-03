#ifndef GAME_SPRITEANIM_H
#define GAME_SPRITEANIM_H

// A sprite-cel animation player: a J3DFrameCtrl runs over one of several
// key tables, and the key the frame falls in gives the current cel.
// Code: game/tu_801D1C18.cpp. No vtable; named after its constructor
// (0x801D267C). "SpriteAnim" is descriptive: 34 sites in enemy units load
// mCel (this + 0x270 + 0x10) and pass it as the frame of a "SOB " sprite
// draw (ActorBase_804A5770::fn_801F06F0 9 times, fn_801F02BC 5 times).
// Members: EnemyBase_804A4808 at 0x270 (game/EnemyBase.h); act_escort_woman
// at 0x230, act_princess_zelda_demo at 0x2F4 and 0x32C, and about 20 other
// actor units.
//
// Owner: the shared-header owner (docs/COORDINATOR.md).

#include "types.h"
#include "JSystem/J3D/J3DFrameCtrl.h" // mkdd's, with tww's accessors

// The constructor stores the J3DFrameCtrl vtable at 0x24 and calls
// J3DFrameCtrl::init(0) after its own stores (0x801D26D4), so mFrameCtrl is
// constructed after the other members, as the last one declared.
class SpriteAnim_801D267C {
public:
    // 23 units / 28 calls (4 in the spawner's inline constructors).
    SpriteAnim_801D267C();

    // 11 units / 11 calls (enemy init()s). mFrameCtrl.update(), then mCel and
    // mKey for the new frame; skipped once after setAnim(.., false) and while
    // no animation is set (mAnim outside [0, mTableCount)).
    void fn_801D1C18();
    // 11 units / 11 calls. fn_801D1F14 at a random frame in [0, mLength]
    // (fn_801CD664(mLength + 1)).
    void fn_801D1D60();
    // 25 units / 102 calls (callers pass lfs values). Sets the frame,
    // clamped to [0, mLength] and kept below mLength (0 for a looping
    // animation, mode 2), and updates mCel and mKey.
    void fn_801D1F14(f32 frame);
    // 4 units / 9 calls; callers test r3 with clrlwi 24. fn_801D21F0 for
    // mode 0, fn_801D217C for mode 2, else 0.
    bool fn_801D208C();
    // 5 units / 8 calls (clrlwi 24). A looping animation passed its last
    // frame (J3DFrameCtrl::checkPass(mLength - 0.001f)).
    bool fn_801D217C();
    // 15 units / 53 calls (clrlwi 24). Stopped (J3DFrameCtrl state bit 1),
    // or a mode 0 animation with rate 0 within 0.002 of its end.
    bool fn_801D21F0();
    // 3 units / 4 calls and 12 units / 22 calls. setAnim(anim, mode, true /
    // false) if anim is not the current one; returns whether it was set.
    // Callers pass lwz or addi values for anim and li 0 or 2 for mode.
    bool fn_801D2264(s32 anim, s32 mode);
    bool fn_801D22A0(s32 anim, s32 mode);
    // 54 units / 262 calls. Starts animation `anim` (an index into mTables)
    // in J3DFrameCtrl mode `mode` (stb 0x28; callers pass li 0 or 2): mLength
    // = the sum of its key durations (minus 1 unless mode 2). With keepFrame
    // (clrlwi. r30 at 0x801D2370) the previous frame is kept as fn_801D1F14
    // would set it; otherwise the frame starts at 0 and the next update is
    // skipped. mode is compared as an int (cmpwi 2), so it is no u8.
    void fn_801D22DC(s32 anim, s32 mode, bool keepFrame);
    // 48 units / 52 calls. Sets the key tables and computes mMinCel/mMaxCel
    // over all their keys. Callers pass a .data table of pointers (addi
    // @l) and its count (li). The constness of the tables is a guess.
    void fn_801D2608(s16** tables, s32 count);

    // Inline helpers: their code is repeated in fn_801D1C18, fn_801D1D60,
    // fn_801D1F14 and fn_801D22DC (fn_801D208C repeats fn_801D217C and
    // fn_801D21F0 the same way). Not called out of line anywhere.
    void calcCel();
    void setFrameInline(f32 frame);
    bool isLoopEnd();
    bool isStopped();

    /* 0x00 */ s16** mTables; // per animation: (cel, duration) s16 pairs, ended by a cel < 0
    /* 0x04 */ s32 mTableCount;
    /* 0x08 */ s32 mAnim;    // -1 until fn_801D22DC
    /* 0x0C */ s32 mLength;  // in frames
    /* 0x10 */ s32 mCel;     // the current key's cel, clamped to [mMinCel, mMaxCel]
    /* 0x14 */ s32 mMinCel;  // 0x7FFFFFFF
    /* 0x18 */ s32 mMaxCel;  // 0x80000000
    /* 0x1C */ s32 mKey;     // index of the current key
    /* 0x20 */ u8 mHold;     // skip the next fn_801D1C18
    /* 0x24 */ J3DFrameCtrl mFrameCtrl;
}; // Size: 0x38

#endif
