#ifndef GAME_ACT_ACT_WAVE_SE_H
#define GAME_ACT_ACT_WAVE_SE_H

// Code: game/act_wave_se.cpp.

#include "game/ActorBase.h"
#include "game/ActorManager.h"

// Actor WAVE ("Wave SE" in docs/names/actors.tsv): the spawner
// (spawnActor_801F9484) constructs it with fn_802012D0, which stores the vtable
// 0x804A9EA8. Named after that vtable.
class WaveSe_804A9EA8 : public ActorBase_804A5770 {
public:
    WaveSe_804A9EA8() {}
    /* 0x08 */ virtual ~WaveSe_804A9EA8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    /* 0x230 */ ActorSoundObj_804578F4* mSound; // own sound object, made in init
    /* 0x234 */ s32 field_0x234; // guess: frames until the next 0x3003D (set to 60, never counted down here)
}; // Size: unknown (>= 0x238)

#endif
