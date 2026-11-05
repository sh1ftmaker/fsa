#ifndef GAME_ACT_ACT_EYEBALL_SWITCH_H
#define GAME_ACT_ACT_EYEBALL_SWITCH_H

// Code: game/act_eyeball_switch.cpp.

#include "game/ActorBase.h"

class JPABaseEmitter;

// The MEDM actor ("Eyeball switch", docs/names/actors.tsv, med confidence):
// the spawner's constructor (0x802005C0) stores this vtable. Named after its
// vtable, 0x804AE678.
class EyeballSwitch_804AE678 : public ActorBase_804A5770 {
public:
    EyeballSwitch_804AE678() {}
    /* 0x08 */ virtual ~EyeballSwitch_804AE678();
    /* 0x0C */ virtual void init();
    /* 0x10 */ virtual void vfunc_0x10(); // looks for the switch monitor of its group
    /* 0x14 */ virtual void update();
    /* 0x24 */ virtual void vfunc_0x24(); // deletes the emitter and dies

    /* 0x230 */ JPABaseEmitter* mEmitter;
    /* 0x234 */ s32 field_0x234; // state: 0 idle, 1 closing, 2 just switched
    /* 0x238 */ s32 field_0x238; // countdown
    /* 0x23C */ s32 field_0x23c; // kind, field_0x90 bits 12-14
    /* 0x240 */ s32 field_0x240; // player-count condition, field_0x90 bits 25-26
    /* 0x244 */ bool field_0x244; // switched on
    /* 0x248 */ s32 field_0x248; // switch index, field_0x90 >> 27
    /* 0x24C */ s32 field_0x24c; // delay, field_0x90 & 0xFFF
    /* 0x250 */ s32 field_0x250; // serial of the arrow that hit it, -1 for none
    /* 0x254 */ s32 field_0x254; // view bit of the monitor's switch
}; // Size: 0x258

#endif
