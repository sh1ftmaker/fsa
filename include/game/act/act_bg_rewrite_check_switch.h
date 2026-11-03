#ifndef GAME_ACT_ACT_BG_REWRITE_CHECK_SWITCH_H
#define GAME_ACT_ACT_BG_REWRITE_CHECK_SWITCH_H

// Code: game/act_bg_rewrite_check_switch.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A8998.
class BgRewriteCheckSwitch_804A8998 : public ActorBase_804A5770 {
public:
    BgRewriteCheckSwitch_804A8998() {}
    virtual ~BgRewriteCheckSwitch_804A8998();
    virtual void init();
    virtual void update();
    virtual void draw();

    /* 0x230 */ u32 field_0x230; // not initialized or used here
}; // Size: 0x234

#endif
