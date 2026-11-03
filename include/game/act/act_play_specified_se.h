#ifndef GAME_ACT_ACT_PLAY_SPECIFIED_SE_H
#define GAME_ACT_ACT_PLAY_SPECIFIED_SE_H

// Code: game/act_play_specified_se.cpp.

#include "game/ActorBase.h"

// Named after its vtable (0x804A9F00).
class PlaySpecifiedSe_804A9F00 : public ActorBase_804A5770 {
public:
    PlaySpecifiedSe_804A9F00() {}
    virtual ~PlaySpecifiedSe_804A9F00();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    /* 0x230 */ s32 field_0x230; // bit 26 of the spawn parameter (0x90)
    /* 0x234 */ u32 field_0x234; // low 8 bits: index into lbl_80541250
    /* 0x238 */ s32 field_0x238; // bits 8-19: frames to wait
}; // Size: 0x23C

#endif
