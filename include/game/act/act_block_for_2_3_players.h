#ifndef GAME_ACT_ACT_BLOCK_FOR_2_3_PLAYERS_H
#define GAME_ACT_ACT_BLOCK_FOR_2_3_PLAYERS_H

// Code: game/act_block_for_2_3_players.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804AE728.
class BlockFor2Or3Players_804AE728 : public ActorBase_804A5770 {
public:
    BlockFor2Or3Players_804AE728() {}
    virtual ~BlockFor2Or3Players_804AE728();
    virtual void init();
    virtual void update();

    /* 0x230 */ u8 field_0x230[8]; // not initialized or used here
}; // Size: 0x238

#endif
