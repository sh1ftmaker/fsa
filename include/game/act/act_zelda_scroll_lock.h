#ifndef GAME_ACT_ACT_ZELDA_SCROLL_LOCK_H
#define GAME_ACT_ACT_ZELDA_SCROLL_LOCK_H

// Code: game/act_zelda_scroll_lock.cpp.

#include "game/ActorBase.h"

// Named after its vtable (0x804AC708).
class ZeldaScrollLock_804AC708 : public ActorBase_804A5770 {
public:
    ZeldaScrollLock_804AC708() {}
    virtual ~ZeldaScrollLock_804AC708();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    /* 0x230 */ s32 mState;
}; // Size: 0x234

#endif
