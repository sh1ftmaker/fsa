#ifndef GAME_ACT_ACT_FAIRY_REVIVAL_H
#define GAME_ACT_ACT_FAIRY_REVIVAL_H

// Code: game/act_fairy_revival.cpp.

#include "game/ActorBase.h"

// The RVFR actor ("Revival fairy", docs/names/actors.tsv, med confidence): the
// spawner constructs it with fn_80201D74, which stores this vtable
// (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AC250.
class RevivalFairy_804AC250 : public ActorBase_804A5770 {
public:
    RevivalFairy_804AC250() {}
    /* 0x08 */ virtual ~RevivalFairy_804AC250();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 mState;               // update only handles 0
    /* 0x234 */ s32 mTimer;               // counts up every update
    /* 0x238 */ s32 mLifeTime;            // counts down from 180; the actor ends at 0
    /* 0x23C */ s32 mFrame;               // sprite frame offset 0..2 (0x5e + mFrame)
    /* 0x240 */ s32 mPlayer;              // field_0x90 & 3: the player it flies to
    /* 0x244 */ s16 mAngle;               // from the player to the fairy
    /* 0x248 */ JGeometry::TVec3f mTargetVel; // field_0x3c approaches x and y; mPos.z approaches z
    /* 0x254 */ JPABaseEmitter* mEmitter254; // effect 0x183
    /* 0x258 */ JPABaseEmitter* mEmitter258; // effect 0x184
}; // Size: 0x25C

#endif
