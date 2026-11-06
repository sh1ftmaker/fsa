#include "game/GameAudio.h"
#include "game/GameGlobals.h"

// Named after its first function (no vtable): a small timed state machine
// that re-rolls a random value `cur` (0-3) every few frames.
class cls_802153A8 {
public:
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
};

void cls_802153A8::fn_80215578() {
    active = 0;
    cur = 0;
    target = 0;
    phase = 0;
    timer = 0;
}

void cls_802153A8::fn_80215558(u8 value) {
    active = 1;
    cur = value;
    target = value;
    phase = 0;
    timer = 0;
}

 // Size: 0x6

bool cls_802153A8::fn_802153A8() {
    bool done = false;
    if (active) {
        timer++;
        switch (phase) {
        case 0:
            if (timer % 5 == 0) {
                cur = fn_801CD580(cur);
            }
            if (timer >= 100) {
                phase++;
                timer = 0;
            }
            break;
        case 1:
            if (timer % 15 == 0) {
                cur = fn_801CD580(cur);
            }
            if (timer >= 75) {
                if (isTarget()) {
                    phase++;
                    timer = 0;
                } else {
                    done = true;
                }
            }
            break;
        case 2:
            if (timer % 15 == 0) {
                cur = fn_801CD398(cur, target);
                done = true;
            }
            break;
        }
    }
    if (done) {
        timer--;
        active = 0;
    }
    return done;
}
