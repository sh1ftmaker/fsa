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
    /* 0x230 */ u8 field_0x230[0x14]; // not yet known
    // Step sequencer read by fn_8035C15C: a timer, the value of the current
    // step, and the index of the next step of the table it is given.
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ u8 field_0x250[0x4]; // not yet known
    // Counted down by fn_8035BE00 while above lbl_805428E8, by lbl_805428EC
    // each call.
    /* 0x254 */ f32 field_0x254;
    /* 0x258 */ u8 field_0x258[0x4C]; // not yet known
    void fn_8035BE00();
    bool fn_8035C15C(s32 seq);
}; // Size: 0x2A4

#endif
