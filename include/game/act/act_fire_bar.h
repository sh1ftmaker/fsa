#ifndef GAME_ACT_ACT_FIRE_BAR_H
#define GAME_ACT_ACT_FIRE_BAR_H

// Code: game/act_fire_bar.cpp. Named after its vtable (0x804A3F90, the
// constructor is 0x801BD640); the spawner fn_801F9484 creates it for actor
// FBMR "Fire bar (manager)" (docs/names/actors.tsv, med confidence). It
// spawns `mCount` FBOB actors (cls_804A4038, game/tu_801BD8C0.cpp) in a line
// outward from its position and turns them about it each frame.

#include "types.h"
#include "game/EnemyBase.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"

class FireBarManager_804A3F90 : public EnemyBase_804A4808 {
public:
    FireBarManager_804A3F90();
    virtual ~FireBarManager_804A3F90();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();

    // The signed turning speed: negated while the GameView bit mFlagBit is set.
    s16 getSpeed() {
        s32 bit = mFlagBit;
        s16 speed = mSpeed;
        if (bit != 0 && fn_802D6A9C()->fn_802D800C(bit)) {
            return -speed;
        }
        return speed;
    }

    /* 0x2A8 */ s32 mCount;       // number of FBOB actors, 1-16
    /* 0x2AC */ s32 mBars[16];    // their serials
    /* 0x2EC */ u8 mFlag;         // !(field_0x90 >> 31)
    /* 0x2F0 */ s32 mFlagBit;     // GameView bit that reverses the turn
    /* 0x2F4 */ s32 mStep;        // distance of the first actor, in 16 unit steps
    /* 0x2F8 */ s16 mSpeed;       // 0x80, 0x100 or 0x40 per frame, sign from field_0x90
    /* 0x2FA */ s16 mAngle;       // 16-bit angle of the bar
    /* 0x2FC */ s16 mTangent;     // mAngle +- 0x4000, handed to the actors
}; // Size: 0x300

#endif
