// The player manager (include/game/Player.h) and the per-player queries
// other units call. Written so far: the manager's flag accessors and one
// query of each shape (a getter call, a setter call with an argument, a
// field, a field's address), the templates for the rest.

#include "game/GameAudio.h"
#include "game/Player.h"

// What the position queries return when there is no such player. The
// unit's __sinit (0x8023F9B8) stores 0.0f to its three floats before the
// JAudio pools; dtk's lbl_804CF460 (0x108) also covers the pools'
// destructor chain after it.
JGeometry::TVec3f lbl_804CF460(0.0f, 0.0f, 0.0f);

// fn_8022ADFC inlines the flag getters below, which come after it.

static inline bool getFlag_0x6c() {
    if (lbl_8053AC90 == NULL) {
        return false;
    }
    return lbl_8053AC90->field_0x6c;
}

static inline bool getFlag_0x6d() {
    if (lbl_8053AC90 == NULL) {
        return false;
    }
    return lbl_8053AC90->field_0x6d;
}

static inline bool getFlag_0x6e() {
    if (lbl_8053AC90 == NULL) {
        return false;
    }
    return lbl_8053AC90->field_0x6e;
}

static inline bool getFlag_0x6f() {
    if (lbl_8053AC90 == NULL) {
        return false;
    }
    return lbl_8053AC90->field_0x6f;
}

static inline bool getFlag_0x70() {
    if (lbl_8053AC90 == NULL) {
        return false;
    }
    return lbl_8053AC90->field_0x70;
}

static inline bool getFlag_0x71() {
    if (lbl_8053AC90 == NULL) {
        return false;
    }
    return lbl_8053AC90->field_0x71;
}

static inline bool getFlag_0x72() {
    if (lbl_8053AC90 == NULL) {
        return false;
    }
    return lbl_8053AC90->field_0x72;
}

bool fn_8022ADB0() {
    if (lbl_8053AC90 == NULL) {
        return false;
    }
    return lbl_8053AC90->field_0x73;
}

void fn_8022ADCC() {
    if (lbl_8053AC90 != NULL) {
        lbl_8053AC90->field_0x73 = false;
    }
}

void fn_8022ADE4() {
    if (lbl_8053AC90 != NULL) {
        lbl_8053AC90->field_0x73 = true;
    }
}

// Whether any of the flags 0x6C-0x72 is set.
bool fn_8022ADFC() {
    if (lbl_8053AC90 == NULL) {
        return false;
    }
    if (getFlag_0x6c()) {
        return true;
    }
    if (getFlag_0x6d()) {
        return true;
    }
    if (getFlag_0x6e()) {
        return true;
    }
    if (getFlag_0x6f()) {
        return true;
    }
    if (getFlag_0x70()) {
        return true;
    }
    if (getFlag_0x71()) {
        return true;
    }
    if (getFlag_0x72()) {
        return true;
    }
    return false;
}

bool fn_8022AF10() {
    return getFlag_0x72();
}

void fn_8022AF2C() {
    if (lbl_8053AC90 != NULL) {
        lbl_8053AC90->field_0x72 = false;
    }
}

void fn_8022AF44() {
    if (lbl_8053AC90 != NULL) {
        lbl_8053AC90->field_0x72 = true;
    }
}

bool fn_8022AF5C() {
    return getFlag_0x71();
}

void fn_8022B130() {
    if (lbl_8053AC90 != NULL) {
        lbl_8053AC90->field_0x70 = false;
    }
}

void fn_8022B148() {
    if (lbl_8053AC90 != NULL) {
        lbl_8053AC90->field_0x70 = true;
    }
}

bool fn_8022B160() {
    return getFlag_0x6f();
}

void fn_8022B17C() {
    if (lbl_8053AC90 != NULL) {
        lbl_8053AC90->field_0x6f = false;
    }
}

void fn_8022B194() {
    if (lbl_8053AC90 != NULL) {
        lbl_8053AC90->field_0x6f = true;
    }
}

bool fn_8022B1AC() {
    return getFlag_0x6e();
}

void fn_8022B1C8() {
    if (lbl_8053AC90 != NULL) {
        lbl_8053AC90->field_0x6e = false;
    }
}

void fn_8022B1E0() {
    if (lbl_8053AC90 != NULL) {
        lbl_8053AC90->field_0x6e = true;
    }
}

bool fn_8022B1F8() {
    return getFlag_0x6d();
}

void fn_8022B214() {
    if (lbl_8053AC90 != NULL) {
        lbl_8053AC90->field_0x6d = false;
    }
}

void fn_8022B22C() {
    if (lbl_8053AC90 != NULL) {
        lbl_8053AC90->field_0x6d = true;
    }
}

bool fn_8022B244() {
    return getFlag_0x6c();
}

void fn_8022B260() {
    if (lbl_8053AC90 != NULL) {
        lbl_8053AC90->field_0x6c = false;
    }
}

bool fn_8022B7C4(s32 player) {
    if (lbl_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return lbl_8053AC90->mPlayers[player]->fn_8023FC08();
    }
    return false;
}

void fn_8022B860(s32 player, s32 value) {
    if (lbl_8053AC90 == NULL) {
        return;
    }
    if (isPlayerActive(player)) {
        lbl_8053AC90->mPlayers[player]->fn_8023FC10(value);
    }
}

s32 fn_8023DE58(s32 player) {
    if (lbl_8053AC90 == NULL) {
        return 0;
    }
    if (isPlayerActive(player)) {
        return lbl_8053AC90->mPlayers[player]->field_0x3b4;
    }
    return 0;
}

JGeometry::TVec3f* fn_8023E724(s32 player) {
    if (lbl_8053AC90 == NULL) {
        return &lbl_804CF460;
    }
    if (isPlayerActive(player)) {
        return &lbl_8053AC90->mPlayers[player]->mPos;
    }
    return &lbl_804CF460;
}

bool fn_8023F264(s32 player) {
    return isPlayerActive(player);
}
