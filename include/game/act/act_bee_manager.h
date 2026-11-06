#ifndef GAME_ACT_ACT_BEE_MANAGER_H
#define GAME_ACT_ACT_BEE_MANAGER_H

#include "game/ActorBase.h"

// Spawned for 'BEMN'.
class BeeManager_804AC810 : public ActorBase_804A5770 {
public:
    BeeManager_804AC810() {}
    /* 0x08 */ virtual ~BeeManager_804AC810();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    void fn_80349914(JGeometry::TVec3f* outPos);

    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240[14];
    /* 0x278 */ s32 field_0x278;
    /* 0x27C */ s32 field_0x27c;
    /* 0x280 */ s16 field_0x280;
    /* 0x282 */ u8 field_0x282;
}; // Size: 0x284

#endif
