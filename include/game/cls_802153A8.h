#ifndef GAME_CLS_802153A8_H
#define GAME_CLS_802153A8_H

#include "types.h"

// Named after its first function (no vtable): a small timed state machine
// that re-rolls a random value `cur` (0-3) every few frames. Defined in
// game/tu_802153A8.cpp; embedded in the shadow Link actor (+0x548).
class cls_802153A8 {
public:
    cls_802153A8() { fn_80215578(); }
    bool fn_802153A8();
    void fn_80215558(u8 value);
    void fn_80215578();

    bool isTarget() const {
        bool result = false;
        if (cur == target) {
            result = true;
        }
        return result;
    }

    /* 0x0 */ u8 active;
    /* 0x1 */ u8 cur;
    /* 0x2 */ u8 target;
    /* 0x3 */ u8 phase;
    /* 0x4 */ u16 timer;
}; // Size: 0x6

#endif
