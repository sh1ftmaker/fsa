#ifndef GAME_ACT_ACT_ADVANCE_NOTICE_SE_H
#define GAME_ACT_ACT_ADVANCE_NOTICE_SE_H

// Code: game/act_advance_notice_se.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A9C78.
class cls_804A9C78 : public ActorBase_804A5770 {
public:
    cls_804A9C78() {}
    /* 0x08 */ virtual ~cls_804A9C78();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x10]; // not yet known
}; // Size: 0x240

#endif
