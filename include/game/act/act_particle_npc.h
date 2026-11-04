#ifndef GAME_ACT_ACT_PARTICLE_NPC_H
#define GAME_ACT_ACT_PARTICLE_NPC_H

// Code: game/act_particle_npc.cpp.

#include "game/ActorBase.h"
#include "game/ActorManager.h"

// The PRTC actor ("particle NPC", docs/names/actors.tsv): the spawner
// (fn_801F9484) constructs it with fn_80201E80, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804A9AC8.
//
// init() unpacks field_0x90: bits 0-7 are the mode (field_0x250), bits 12-15
// go to field_0x258, bits 16-19 to field_0x254 (which init() then sets to 1)
// and bits 20-23 to field_0x25c. The emitters are spawned in init() when
// field_0x25c is 0, else in update() once vfunc_0x34() holds.
class ParticleNpc_804A9AC8 : public ActorBase_804A5770 {
public:
    ParticleNpc_804A9AC8() {}
    /* 0x08 */ virtual ~ParticleNpc_804A9AC8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    /* 0x230 */ ActorSoundObj_804578F4* mSound; // made at the end of init()
    /* 0x234 */ JPABaseEmitter* field_0x234;
    /* 0x238 */ JPABaseEmitter* field_0x238;
    /* 0x23C */ JPABaseEmitter* field_0x23c;
    /* 0x240 */ s32 field_0x240; // 0 until the emitters are spawned, then 1
    /* 0x244 */ f32 field_0x244; // offset added to mPos.x for every emitter
    /* 0x248 */ f32 field_0x248; // offset added to mPos.y for every emitter
    /* 0x24C */ f32 field_0x24c; // zeroed by init(), never read
    /* 0x250 */ s32 field_0x250; // mode: the switch index in init() and update()
    /* 0x254 */ s32 field_0x254; // emitter height: 1 from init(), 0 for mode 9 when fn_802DEBA0()
    /* 0x258 */ s32 field_0x258; // field_0x90 bits 12-15, stored by init() only
    /* 0x25C */ s32 field_0x25c; // field_0x90 bits 20-23; nonzero skips the spawn in init()
    /* 0x260 */ bool field_0x260; // field_0x90 bit 26, stored by init() only
}; // Size: unknown (>= 0x261)

#endif
