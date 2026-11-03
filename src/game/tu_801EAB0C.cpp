#include "game/GameAudio.h"
#include "game/ActorBase.h"

// Per-frame update: counts every non-zero timer down; a slot in field_0x4
// that reaches zero releases its id.
void ActorSub1DC_801EAD68::fn_801EAB0C() {
    for (int i = 0; i < 4; i++) {
        if (field_0x0[i] != 0) {
            field_0x0[i]--;
        }
    }
    for (int i = 0; i < 16; i++) {
        if (field_0x4[i] != 0) {
            field_0x4[i]--;
            if (field_0x4[i] == 0) {
                field_0x14[i] = -1;
            }
        }
    }
}

// Starts the 24-frame timer of slot `index` (0-3) if it is idle.
bool ActorSub1DC_801EAD68::fn_801EAC20(int index) {
    bool started = false;
    if (index >= 0 && index < 4 && field_0x0[index] == 0) {
        field_0x0[index] = 24;
        started = true;
    }
    return started;
}

// Whether a live slot already holds `id`.
inline bool ActorSub1DC_801EAD68::isLive(int id) {
    bool found = false;
    if (id >= 0) {
        for (int i = 0; i < 16; i++) {
            if (field_0x4[i] != 0 && field_0x14[i] == id) {
                found = true;
                break;
            }
        }
    }
    return found;
}

// Claims a free slot for `id` unless a live slot already holds it.
bool ActorSub1DC_801EAD68::fn_801EAC54(int id) {
    bool claimed = false;
    if (id >= 0 && !isLive(id)) {
        for (int i = 0; i < 16; i++) {
            if (field_0x4[i] == 0) {
                field_0x4[i] = 24;
                field_0x14[i] = id;
                claimed = true;
                break;
            }
        }
    }
    return claimed;
}

ActorSub1DC_801EAD68::ActorSub1DC_801EAD68() {
    for (int i = 0; i < 4; i++) {
        field_0x0[i] = 0;
    }
    for (int i = 0; i < 16; i++) {
        field_0x4[i] = 0;
        field_0x14[i] = -1;
    }
}
