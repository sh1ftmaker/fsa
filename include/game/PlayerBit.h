#ifndef GAME_PLAYERBIT_H
#define GAME_PLAYERBIT_H

#include "game/GameGlobals.h"

// Whether bit `player` of the game root's mPlayerMask is set: the body of
// GameRoot_8049EF20::fn_8023F98C (tu_8022AD64), which the per-player code
// (tu_8022AD64, tu_8023FBF0) inlines. It returns a byte (u8), not bool: tu_802684A4
// fn_8026C350 keeps the value (neg / or / srwi, then cmplwi 1) for `== true` where a bool
// return folds it into and.; every other user compiles the same either way.
inline u8 isPlayerBitSet(GameRoot_8049EF20* root, s32 player) {
    u8 bit = 1 << player;
    return (root->mPlayerMask & bit) != 0;
}

#endif
