#ifndef GAME_GAMEMODE_H
#define GAME_GAMEMODE_H

#include "game/GameGlobals.h"

// The mode test on gGameRoot_8053AAF8 (GameRoot_8049EF20) fields 0x7F and 0x4C,
// inlined in 52 functions of 8 units (tu_8013AAD4, tu_8013C5C8, tu_8016A2B4,
// tu_801902C0, tu_802F5040, tu_8037DF68, tu_80390818, tu_80134C04). Each set
// is compared value by value into a byte (li 0, cmpwi/beq in this order,
// li 1), turned into a bool, and the caller tests that bool again (clrlwi.).
// What the modes mean is not known yet; the names only give the values.
//
// The values are named constants: MWCC folds `mode == 7 || mode == 8 || ...`
// with literals (or enumerators) into range checks (subi / cmplwi), which
// the original never has; with `const s32` names it keeps one cmpwi per
// value in source order, as the original does.
const s32 GAME_MODE_7 = 7;
const s32 GAME_MODE_8 = 8;
const s32 GAME_MODE_9 = 9;
const s32 GAME_MODE_A = 0xA;
const s32 GAME_MODE_B = 0xB;
const s32 GAME_MODE_C = 0xC;
const s32 GAME_MODE_16 = 0x16;

// The set used while field_0x7f is set.
inline u8 isGameModeSetA(s32 mode) {
    u8 result = false;
    if (mode == GAME_MODE_7 || mode == GAME_MODE_8 || mode == GAME_MODE_9 ||
        mode == GAME_MODE_16 || mode == GAME_MODE_A || mode == GAME_MODE_B) {
        result = true;
    }
    return result;
}

// The same set plus mode 0xC: used while field_0x7f is clear.
inline u8 isGameModeSetB(s32 mode) {
    u8 result = false;
    if (mode == GAME_MODE_7 || mode == GAME_MODE_8 || mode == GAME_MODE_9 ||
        mode == GAME_MODE_16 || mode == GAME_MODE_A || mode == GAME_MODE_B ||
        mode == GAME_MODE_C) {
        result = true;
    }
    return result;
}

// The current mode == mode, as a u8: the callers test the materialised byte (subfic / cntlzw /
// extrwi.); a plain `x == N` or a bool return folds into cmpwi / bne (tu_80285E58 fn_80286688,
// tu_8026FC20 fn_8027057C with mode 7).
inline u8 isGameMode(s32 mode) {
    return gGameRoot_8053AAF8->field_0x4c == mode ? true : false;
}

inline bool isGameModeSet() {
    GameRoot_8049EF20* root = gGameRoot_8053AAF8;
    if (root->field_0x7f) {
        return isGameModeSetA(root->field_0x4c);
    }
    return isGameModeSetB(root->field_0x4c);
}

#endif
