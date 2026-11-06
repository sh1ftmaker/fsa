#ifndef GAME_ACT_ACT_SNOWBALL_H
#define GAME_ACT_ACT_SNOWBALL_H

// Code: game/act_snowball.cpp.

#include "game/ActorBase.h"
#include "game/RailBase.h"
#include "game/SpriteAnim.h"

// The SNBL actor ("Snowball", docs/names/actors.tsv, med confidence): the
// spawner constructs it with its constructor (0x8010D7A0). Named after its
// vtable, 0x8049D020.
class cls_8049D020 : public ActorBase_804A5770 {
public:
    cls_8049D020();
    /* 0x08 */ virtual ~cls_8049D020();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    // 0x8010C294: state machine dispatcher.
    bool fn_8010C294(s32 state);

    /* 0x230 */ s16 field_0x230;
    /* 0x232 */ u8 field_0x232[2];
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ s32 field_0x254;
    /* 0x258 */ s32 field_0x258;
    /* 0x25C */ s32 field_0x25c;
    /* 0x260 */ f32 field_0x260;
    /* 0x264 */ f32 field_0x264;
    /* 0x268 */ f32 field_0x268;
    /* 0x26C */ f32 field_0x26c;
    /* 0x270 */ f32 field_0x270;
    /* 0x274 */ SpriteAnim_801D267C mSprite;
    /* 0x2AC */ cls_800FE3EC mRail;
    /* 0x374 */ JPABaseEmitter* field_0x374;
    /* 0x378 */ JPABaseEmitter* field_0x378;
    /* 0x37C */ JPABaseEmitter* field_0x37c;
}; // Size: 0x380

#endif
