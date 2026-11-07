#ifndef GAME_ACT_ACT_CLOUD_H
#define GAME_ACT_ACT_CLOUD_H

// Code: game/act_cloud.cpp.

#include "game/ActorBase.h"
#include "game/RailBase.h"
#include "game/SpriteAnim.h"

// The CLOD actor ("Cloud", docs/names/actors.tsv, med confidence): the
// spawner constructs it with its constructor (0x80105BEC, game_units.tsv).
// Named after its vtable, 0x8049CBE0.
class Cloud_8049CBE0 : public ActorBase_804A5770 {
public:
    Cloud_8049CBE0();
    /* 0x08 */ virtual ~Cloud_8049CBE0();
    // 0x801053E8: whether player `player` is in the cloud (see act_cloud.cpp).
    bool fn_801053E8(s32 player);
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    /* 0x230 */ cls_800FE3EC mRail; // the path the cloud follows
    /* 0x2F8 */ void* field_0x2f8[4]; // "SOB " resources by mSprite.mKey, drawn by draw()
    // The sprite parameters of draw() (the constructor is SpriteParams_801F5AE0's
    // inline one, run here at 0x308). Its field_0x1d is set to -1 by draw().
    /* 0x308 */ SpriteParams_801F5AE0 mParams;
    /* 0x344 */ f32 field_0x344;
    /* 0x348 */ f32 field_0x348;
    /* 0x34C */ f32 field_0x34c;
    /* 0x350 */ f32 field_0x350;
    /* 0x354 */ s32 field_0x354;
    /* 0x358 */ s32 field_0x358;
    /* 0x35C */ s32 field_0x35c; // -1 after construction
    /* 0x360 */ s32 field_0x360;
    /* 0x364 */ s32 field_0x364;
    /* 0x368 */ s32 field_0x368;
    /* 0x36C */ s32 field_0x36c; // 2 moves the draw position down by 4
    /* 0x370 */ s32 field_0x370;
    /* 0x374 */ SpriteAnim_801D267C mSprite;
}; // Size: at least 0x3AC

#endif
