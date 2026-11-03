#ifndef GAME_ACT_TU_801D8400_H
#define GAME_ACT_TU_801D8400_H

#include "types.h"
#include "game/EnemyBase.h"

// The state machine the unit at 0x801DA7C0 steps (named after its
// out-of-line constructor fn_801DAA24, called on this + 0x2A8 at 0x801DA524): word 0x4 is the current value, which
// fn_801DA904 / fn_801DA938 read. Only the written code's view is known.
struct Sequencer_801DAA24 {
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
// Size: 0x340 (li r3, 0x340 before the constructor call fn_801DA504)
class cls_804A4C58 : public EnemyBase_804A4808 {
public:
    cls_804A4C58() {}
    /* 0x20 */ virtual void draw();
    // 0x801D84E0: empty, no arguments read (vtable slot 0x24 of 0x804A4C58).
    /* 0x24 */ virtual void vfunc_0x24();

    // 0x801D8400 (first function of tu_801D8400, called by tu_801DAC6C): below
    // state 8 (mState) calls fn_801D9C58 with 8.
    void fn_801D8400();
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
    /* 0x2D0 */ u8 field_0x2d0[0x340 - 0x2D0]; // not yet known
};

#endif
