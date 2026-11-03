#ifndef GAME_ACT_TU_8012F8AC_H
#define GAME_ACT_TU_8012F8AC_H

// Code: game/tu_8012F8AC.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x8049E9A8.
class cls_8049E9A8 : public ActorBase_804A5770 {
public:
    cls_8049E9A8() {}
    /* 0x08 */ virtual ~cls_8049E9A8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x230 */ s32 mState; // 0: waiting for the check below, 1: done
}; // Size: 0x234

#endif
