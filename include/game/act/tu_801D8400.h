#ifndef GAME_ACT_TU_801D8400_H
#define GAME_ACT_TU_801D8400_H

#include "types.h"
#include "game/EnemyBase.h"

// The state machine the unit at 0x801DA7C0 steps (named after its
// out-of-line constructor fn_801DAA24, called on this + 0x2A8 at 0x801DA524): word 0x4 is the current value, which
// fn_801DA904 / fn_801DA938 read. Only the written code's view is known.
struct Sequencer_801DAA24 {
    // Constructor __ct__18Sequencer_801DAA24Fv (0x801DAA24, tu_801DA7C0.cpp,
    // not written yet): zeroes every word but field_0x10, which it sets to 2.
    Sequencer_801DAA24();
    // 0x801DA938 / 0x801DA904 (tu_801DA7C0.cpp, not written yet). Both read
    // field_0x4 clamped to 0-47: fn_801DA938 returns that entry of the string
    // table lbl_804A4DA8; fn_801DA904 whether it is 28 or more.
    const char* fn_801DA938();
    bool fn_801DA904();
    // 0x801DA7C0 / 0x801DA894 / 0x801DA96C / 0x801DA9E8 (tu_801DA7C0.cpp, not written yet).
    void fn_801DA7C0();
    bool fn_801DA894();
    s32 fn_801DA96C();
    void fn_801DA9E8(s32 a, s32 b);
    // 0x801DA994 (tu_801DA7C0.cpp, not written yet): like fn_801DA9E8 with a second mode.
    bool fn_801DA994(s32 a, s32 b);

    /* 0x00 */ s32 field_0x0;
    /* 0x04 */ s32 field_0x4; // current value, 0-47
    /* 0x08 */ s32 field_0x8;
    /* 0x0C */ s32 field_0xc;
    /* 0x10 */ s32 field_0x10;
    /* 0x14 */ s32 field_0x14;
}; // Size: 0x18

// An EnemyBase_804A4808 (vtable 0x804A4C58, code in game/tu_801D8400.cpp
// and game/tu_801DD1AC.cpp, unwritten); named after its vtable. Its draw()
// and effect helpers are in game/tu_801E5FC8.cpp: it draws the resource
// named by its sequencer ('ICEB' in the archive at GameState field_0x10),
// mirrored when mFlags has 0x2000 and the sequencer is at 28 or more, and
// keeps two pairs of effects (ids 0x25E/0x25F in mEmitter2c0/2c4, 0x25C/0x25D
// in mEmitter2c8/2cc).
// Size: 0x340 (li r3, 0x340 before the constructor, 0x801DA504 here).
class cls_804A4C58 : public EnemyBase_804A4808 {
public:
    // 0x801DA504 (__ct__12cls_804A4C58Fv, game/tu_801D8400.cpp): EnemyBase's
    // constructor, the vtable, the sequencer, the ceiling and the arrays.
    cls_804A4C58();
    /* 0x08 */ virtual ~cls_804A4C58();
    /* 0x0C */ virtual void init();   // fn_801DA124
    /* 0x14 */ virtual void update(); // fn_801D8430
    /* 0x20 */ virtual void draw();
    // 0x801D84E0: empty, no arguments read (vtable slot 0x24 of 0x804A4C58).
    /* 0x24 */ virtual void vfunc_0x24();
    // 0x801DA0FC: gAudioMgr_8053AF80->fn_80458F9C(60).
    /* 0x54 */ virtual void vfunc_0x54();
    // 0x801D8980: a mPos.y above field_0x2d0 is clamped to it (and
    // field_0x3c.y zeroed), then EnemyBase's floor check runs.
    // 0x801D85B0: after a hit, in state 0xB-0xE picks the next target column and facing.
    /* 0x74 */ virtual void vfunc_0x74();
    /* 0x80 */ virtual void vfunc_0x80();
    // 0x801D882C: in state 0xB-0xE steers toward the column at field_0x334.
    /* 0x88 */ virtual void vfunc_0x88();
    // 0x801D8B28: the per-state behaviour, not written yet.
    /* 0x90 */ virtual void vfunc_0x90(u8 mapHit);
    // 0x801D8A30: steps the sequencer and plays sound 0x40058 on its landing frames.
    /* 0x94 */ virtual void vfunc_0x94();
    // 0x801D89BC: with a hit bit in mFlags (& 7), vfunc_0x94 once mState is
    // 0xF; with none, fn_801CE2BC(mapHit). Then field_0x336 counts down to
    // 0 and fn_801E61C4 moves the pair at 0x2C8.
    /* 0x9C */ virtual void vfunc_0x9C(u8 mapHit);
    // 0x801D84E4: the last slot of the vtable (0xA0). Below state 4 (and not
    // 0) a state 1-3 sets the flag at GameRoot2's 0x30 once and, when the
    // camera's 0x7F5 is clear, starts a scripted move to mPos; other states
    // queue mPos with the camera and, unless 0x179 is set, set 0x178.
    /* 0xA0 */ virtual void vfunc_0xA0();

    // 0x801D8400 (first function of tu_801D8400, called by tu_801DAC6C): below
    // state 8 (mState) calls fn_801D9C58 with 8.
    void fn_801D8400();
    // 0x801D9C58 (tu_801D8400 and tu_801DD1AC call it on this with a state
    // number in r4, e.g. 8 from fn_801D8400); its body is not written yet.
    void fn_801D9C58(int state);
    // 0x801D95F4: throws a 'BOIB' from the mouth; true when it was made.
    bool fn_801D95F4();
    // 0x801D9960: counts the throw timers down and starts the next state.
    void fn_801D9960();
    // 0x801DE12C (game/tu_801DD1AC.cpp, not written yet).
    void fn_801DE12C();
    // 0x801DE018 (game/tu_801DD1AC.cpp): in states 4-7, if a player is inside the box around mPos
    // (-40..40, -16..50), switches to state 5; true when it did.
    bool fn_801DE018();
    // 0x801DD1AC (game/tu_801DD1AC.cpp): keeps mPos inside the arena (field_0x2d4 columns, field_0x2d0
    // ceiling) and sets mMapHitDir; true when it clamped.
    bool fn_801DD1AC();
    // 0x801DD28C (game/tu_801DD1AC.cpp): tests the players against a box (state 9 uses a wide
    // one, mirrored when the sequencer flips) with fn_801F2B7C; the hit mask.
    u8 fn_801DD28C();
    // 0x801E61C4 (tu_801D8400's code calls it): moves the pair at 0x2C8 to
    // 40 units below mPos (in y).
    void fn_801E61C4();
    // 0x801E6234: starts the pair at 0x2C8 (ids 0x25C, 0x25D) there.
    void fn_801E6234();
    // 0x801E62D4: starts effect 0x259 at mPos (the emitter is not kept).
    void fn_801E62D4() const;
    // 0x801E631C: starts effects 0x25A and 0x25B at mPos (not kept).
    void fn_801E631C() const;
    // 0x801E6394: deletes the pair at 0x2C0 and clears it.
    void fn_801E6394();
    // 0x801E63D8: starts the pair at 0x2C0 (ids 0x25E, 0x25F) at mPos.
    void fn_801E63D8();

    /* 0x2A8 */ Sequencer_801DAA24 mSeq2a8;
    /* 0x2C0 */ JPABaseEmitter* mEmitter2c0;
    /* 0x2C4 */ JPABaseEmitter* mEmitter2c4;
    /* 0x2C8 */ JPABaseEmitter* mEmitter2c8;
    /* 0x2CC */ JPABaseEmitter* mEmitter2cc;
    /* 0x2D0 */ f32 field_0x2d0; // vfunc_0x80's ceiling for mPos.y
    /* 0x2D4 */ JGeometry::TVec2f field_0x2d4[12]; // constructed by __construct_array (8-byte TVec2f)
    /* 0x334 */ u8 field_0x334; // zeroed by the constructor
    /* 0x335 */ u8 field_0x335; // not written by the constructor
    /* 0x336 */ u16 field_0x336; // countdown, stepped down by vfunc_0x9C
    /* 0x338 */ u16 field_0x338; // zeroed by the constructor
    /* 0x33A */ u16 field_0x33a; // zeroed by the constructor
    /* 0x33C */ u16 field_0x33c; // zeroed by the constructor
    /* 0x33E */ u8 field_0x33e; // zeroed by the constructor
    /* 0x33F */ u8 field_0x33f; // zeroed by the constructor
};

#endif
