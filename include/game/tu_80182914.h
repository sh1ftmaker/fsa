#ifndef GAME_TU_80182914_H
#define GAME_TU_80182914_H

// Exports of game/tu_80182914.cpp.

#include "JSystem/JUtility/TColor.h"

// 0x8018330C: the colour of the cls_804A0160 at lbl_8053AB10->0x2C.
// game/tu_802EF76C.h declares it as (GXColor* out, s32 index); the body
// (lwz/stw through r3, no other argument) only takes the return pointer.
extern "C" {
JUtility::TColor fn_8018330C();
}

#endif
