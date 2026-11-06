#ifndef GAME_ACT_ACT_FOUR_PERSON_PULL_SWITCH_H
#define GAME_ACT_ACT_FOUR_PERSON_PULL_SWITCH_H

// Code: game/act_four_person_pull_switch.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804AE230.
class FourPersonPullSwitch_804AE230 : public ActorBase_804A5770 {
public:
    FourPersonPullSwitch_804AE230() {}
    /* 0x08 */ virtual ~FourPersonPullSwitch_804AE230();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    void fn_8036CBBC();

    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ s32 field_0x234[3];
    /* 0x240 */ JGeometry::TVec3f field_0x240;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ u8 field_0x250;
    /* 0x254 */ f32 field_0x254;
    /* 0x258 */ s32 field_0x258;
    /* 0x25C */ s32 field_0x25c;
    /* 0x260 */ s32 field_0x260;
}; // Size: 0x264

#endif
