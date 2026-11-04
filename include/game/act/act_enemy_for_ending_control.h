#ifndef GAME_ACT_ACT_ENEMY_FOR_ENDING_CONTROL_H
#define GAME_ACT_ACT_ENEMY_FOR_ENDING_CONTROL_H

// Code: game/act_enemy_for_ending_control.cpp.

#include "game/ActorBase.h"

// The ENPC actor ("Enemy for ending control", docs/names/actors.tsv, med
// confidence): the spawner constructs it with fn_80200F24, which stores this
// vtable (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AD48C.
class EnemyForEndingControl_804AD48C : public ActorBase_804A5770 {
public:
    EnemyForEndingControl_804AD48C() {}
    /* 0x08 */ virtual ~EnemyForEndingControl_804AD48C();
    /* 0x0C */ virtual void init();
    /* 0x10 */ virtual void vfunc_0x10();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    // Counts the steps fn_8035B81C has finished once both moves arrived.
    /* 0x230 */ s32 field_0x230;
    // 0 until fn_8035B650's first call runs the setup, then 1.
    /* 0x234 */ s32 field_0x234;
    // draw() shows the sprite only while this is nonzero.
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ u8 field_0x23C[0x4]; // not yet known
    // Moved toward lbl_805428D8 by fn_8035B81C, by the step in 0x258.
    /* 0x240 */ f32 field_0x240;
    // Step sequencer read by fn_8035C15C: a timer, the value of the current
    // step, and the index of the next step of the table it is given.
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    // Set from the second argument of fn_8035C1C4 (its sequence's start value).
    /* 0x250 */ s32 field_0x250;
    // Counted down by fn_8035BE00 while above lbl_805428E8, by lbl_805428EC
    // each call.
    /* 0x254 */ f32 field_0x254;
    // Per-call steps: field_0x240 and field_0x254 move by these (fn_8035B81C).
    /* 0x258 */ f32 field_0x258;
    /* 0x25C */ f32 field_0x25c;
    // Phase set to 0x11C by the first pass: 0 ends the effect (fn_8035B650
    // clears the emitters and mAlive), 1 spawns the ENDM actor and runs the
    // four player pillars (fn_80230498).
    /* 0x260 */ s32 field_0x260;
    /* 0x264 */ u8 field_0x264[0x14]; // not yet known
    // Serials of the actors fn_8035BE24 spawns: Princess Zelda (EZLD), six
    // maidens (EMIK) and Link (ELNK, ending). fn_8035B7A8 flags EZLD and ELNK.
    /* 0x278 */ s32 field_0x278;
    /* 0x27C */ s32 field_0x27c[6];
    /* 0x294 */ s32 field_0x294;
    // Emitters fn_8035B650 flags JPAEMIT_ForceDelete when the phase is 0,
    // then clears.
    /* 0x298 */ JPABaseEmitter* mEmitter298;
    /* 0x29C */ JPABaseEmitter* mEmitter29C;
    /* 0x2A0 */ JPABaseEmitter* mEmitter2A0;
    void fn_8035B650();
    void fn_8035B7A8();
    void fn_8035B81C();
    void fn_8035BE00();
}; // Size: 0x2A4

#endif
