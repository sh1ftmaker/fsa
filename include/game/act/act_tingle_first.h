#ifndef GAME_ACT_ACT_TINGLE_FIRST_H
#define GAME_ACT_ACT_TINGLE_FIRST_H

// Code: game/act_tingle_first.cpp (ACTOR_FTGL, Tingle's first appearance).

#include "game/ActorBase.h"

class cls_8031709C;

// Named after its vtable, 0x804AADD8. The slots it overrides: 0x08 the
// destructor, 0x0C init, 0x14 update, 0x20 draw.
class cls_804AADD8 : public ActorBase_804A5770 {
public:
    cls_804AADD8();
    /* 0x08 */ virtual ~cls_804AADD8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ cls_8031709C* field_0x24c; // sound/effect sequencer, owned here
    /* 0x260 */ f32 field_0x260;
    /* 0x264 */ f32 field_0x264;
    /* 0x268 */ f32 field_0x268;
    /* 0x26C */ f32 field_0x26c;
    /* 0x270 */ u8 field_0x270;
    // Size: unknown; the fields used so far end at 0x271.
};

#endif
