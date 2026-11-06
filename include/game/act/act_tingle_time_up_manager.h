#ifndef GAME_ACT_ACT_TINGLE_TIME_UP_MANAGER_H
#define GAME_ACT_ACT_TINGLE_TIME_UP_MANAGER_H

// Code: game/act_tingle_time_up_manager.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804ACBF0.
class TingleTimeUpManager_804ACBF0 : public ActorBase_804A5770 {
public:
    TingleTimeUpManager_804ACBF0() {}
    /* 0x08 */ virtual ~TingleTimeUpManager_804ACBF0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ s32 mState;
    /* 0x234 */ s32 mTimer;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ u32 field_0x23c; // RGBA colour word, from fn_8018330C
    /* 0x240 */ u32 field_0x240;
    /* 0x244 */ f32 field_0x244; // colour channels as floats
    /* 0x248 */ f32 field_0x248;
    /* 0x24C */ f32 field_0x24c;
    /* 0x250 */ f32 field_0x250;
    /* 0x254 */ f32 field_0x254; // per-frame colour steps
    /* 0x258 */ f32 field_0x258;
    /* 0x25C */ f32 field_0x25c;
    /* 0x260 */ f32 field_0x260;
    /* 0x264 */ f32 field_0x264;
    /* 0x268 */ f32 field_0x268;
    /* 0x26C */ JGeometry::TBox2f mViewBox; // copy of the view's visible box
    /* 0x27C */ JGeometry::TVec3f field_0x27c;
    /* 0x288 */ JGeometry::TVec3f field_0x288;
}; // Size: 0x294

#endif
