#ifndef GAME_ACT_ACT_GIANT_BOMB_H
#define GAME_ACT_ACT_GIANT_BOMB_H

#include "game/ActorBase.h"
#include "game/SpriteParams.h"

class JPABaseEmitter;

// The BBMB actor ("Giant Bomb", docs/names/actors.tsv, med confidence):
// spawner constructs it with fn_80201AC4, storing this vtable 0x804AC588.
class GiantBomb_804AC588 : public ActorBase_804A5770 {
public:
    GiantBomb_804AC588() {}
    /* 0x08 */ virtual ~GiantBomb_804AC588();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    // Not written yet (0x80341D08): drawn with a loaded resource at mPos + (offsetX, offsetY, offsetZ).
    /* 0x54 */ virtual void vfunc_0x54(void* res, SpriteParams_801F5AE0* params, s32 flags, f32 offsetX,
                                       f32 offsetY, f32 offsetZ);

    void fn_8034335C(s32 layer, const JGeometry::TVec3f* pos);

    /* 0x230 */ s32 mState;
    /* 0x234 */ s32 mTimer;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ JGeometry::TVec3f field_0x254;
    /* 0x260 */ u8 field_0x260;
    /* 0x261 */ u8 field_0x261;
    /* 0x262 */ u8 field_0x262[0x264 - 0x262];
    /* 0x264 */ JPABaseEmitter* mEmitter264;
    /* 0x268 */ JPABaseEmitter* mEmitter268;
}; // Size: 0x26C

#endif
