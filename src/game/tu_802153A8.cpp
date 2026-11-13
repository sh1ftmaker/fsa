#include "game/GameAudio.h"
#include "game/GameGlobals.h"

#include "game/cls_802153A8.h"

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
