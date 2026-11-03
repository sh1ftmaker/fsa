#ifndef GAME_ACT_ACT_SPECIAL_BG_DRAW_OBJECT_H
#define GAME_ACT_ACT_SPECIAL_BG_DRAW_OBJECT_H

// Code: game/act_special_bg_draw_object.cpp.

#include "game/ActorBase.h"

// The SPBG actor ("Special BG draw object", docs/names/actors.tsv, med
// confidence): the spawner constructs it with fn_80201348, which stores this
// vtable (config/G4SE01/game_units.tsv). Named after its vtable, 0x804AC760.
class SpecialBGDrawObject_804AC760 : public ActorBase_804A5770 {
public:
    SpecialBGDrawObject_804AC760() {}
    /* 0x08 */ virtual ~SpecialBGDrawObject_804AC760();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 mState;
    /* 0x234 */ s32 mKind; // field_0x90 & 0xF
    /* 0x238 */ s32 mBit1; // GameView bit (field_0x90 bits 22-26)
    /* 0x23C */ s32 mBit2; // GameView bit (field_0x90 bits 17-21)
    /* 0x240 */ s32 mTimer;
    /* 0x244 */ s32 mSoundState;
    /* 0x248 */ u8 mTriggered;
    /* 0x249 */ u8 field_0x249[3]; // padding
}; // Size: 0x24C

#endif
