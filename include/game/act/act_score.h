#ifndef GAME_ACT_ACT_SCORE_H
#define GAME_ACT_ACT_SCORE_H

// Code: game/act_score.cpp.

#include "game/ActorBase.h"

// The SCRE actor ("Score", docs/names/actors.tsv, med confidence): the spawner
// constructs it with fn_8020346C, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AF4C0.
class Score_804AF4C0 : public ActorBase_804A5770 {
public:
    Score_804AF4C0() {}
    /* 0x08 */ virtual ~Score_804AF4C0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    // Non-virtual members called by draw / update (not written yet).
    void fn_80391D44();
    void fn_80391F58();
    void fn_80393228();
    void fn_80393514();
    /* 0x230 */ s32 mTimer;
    /* 0x234 */ s32 mState;
    /* 0x238 */ s32 mPlayer;
    /* 0x23C */ s32 mValue; // low 16 bits of field_0x90
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ f32 field_0x248;
    /* 0x24C */ f32 field_0x24c;
}; // Size: 0x250

#endif
