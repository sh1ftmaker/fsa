#ifndef GAME_PLAYERFLAGS_H
#define GAME_PLAYERFLAGS_H

#include "game/Player.h"

// The byte chain of Player_804A6F38::fn_802413F0 (0x802413F0): false unless
// 0xC80 is set, 0xCEE and 0x1215 are clear, 0xCA0 is set; then 0xCE9.
// Inlined 33 times in tu_8023FBF0 (e.g. fn_8023FC5C, before the
// out-of-line copy, so it is a header inline), tu_8025EB18 and tu_802684A4:
// each test loads one byte, and the inlined result is materialised
// (li 0 / li 1) and tested again (clrlwi.). What it means is not known yet;
// the name keeps the address of its out-of-line copy. That copy converts
// 0xCE9 with neg / or / srwi (`!= 0`, 100 %); the inlined copies in
// tu_8023FBF0 test it with cmplwi / bne, which is playerFlagsGate below.
inline bool playerFlags_802413F0(const Player_804A6F38* player) {
    if (!player->field_0xc80) {
        return false;
    }
    if (player->field_0xcee) {
        return false;
    }
    if (player->field_0x1215) {
        return false;
    }
    if (!player->field_0xca0) {
        return false;
    }
    return player->field_0xce9 != 0;
}


// The same five tests as the gate in front of the player's pad queries and
// fn_8023FC5C (inlined about 40 times in tu_8023FBF0): these copies branch on
// the last byte (cmplwi / bne, li 1 / li 0), which fn_802413F0's body above
// cannot give (forcing one form on both drops fn_802413F0 to 84.6 %), so the
// inlined gate is a separate source construct with the if form.
inline bool playerFlagsGate(const Player_804A6F38* player) {
    if (!player->field_0xc80) {
        return false;
    }
    if (player->field_0xcee) {
        return false;
    }
    if (player->field_0x1215) {
        return false;
    }
    if (!player->field_0xca0) {
        return false;
    }
    if (!player->field_0xce9) {
        return false;
    }
    return true;
}

#endif
