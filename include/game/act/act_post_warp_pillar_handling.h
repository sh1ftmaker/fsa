#ifndef GAME_ACT_ACT_POST_WARP_PILLAR_HANDLING_H
#define GAME_ACT_ACT_POST_WARP_PILLAR_HANDLING_H

// Code: game/act_post_warp_pillar_handling.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804AC1F8.
class PostWarpPillarHandling_804AC1F8 : public ActorBase_804A5770 {
public:
    PostWarpPillarHandling_804AC1F8() {}
    /* 0x08 */ virtual ~PostWarpPillarHandling_804AC1F8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 field_0x230; // counter: init 0, update counts up to 40 (scales the shadow alpha)
}; // Size: 0x234

#endif
