#ifndef GAME_ACT_ACT_TUTORIAL_OLD_MAN_H
#define GAME_ACT_ACT_TUTORIAL_OLD_MAN_H

// Code: game/act_tutorial_old_man.cpp.

#include "game/ActorBase.h"

// Named after its vtable (0x804AE508).
class TutorialOldMan_804AE508 : public ActorBase_804A5770 {
public:
    TutorialOldMan_804AE508() {}
    /* 0x08 */ virtual ~TutorialOldMan_804AE508();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
}; // Size: 0x230

#endif
