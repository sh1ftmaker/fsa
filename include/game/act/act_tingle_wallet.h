#ifndef GAME_ACT_ACT_TINGLE_WALLET_H
#define GAME_ACT_ACT_TINGLE_WALLET_H

// Code: game/act_tingle_wallet.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A3948.
class cls_804A3948 : public ActorBase_804A5770 {
public:
    cls_804A3948() {}
    /* 0x08 */ virtual ~cls_804A3948();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ u8 field_0x230[0x40]; // not yet known
}; // Size: 0x270

#endif
