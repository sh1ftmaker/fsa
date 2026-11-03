#include "game/GameAudio.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/act/act_leaf_effect_npc.h"

cls_804AE2E0::~cls_804AE2E0() {}

void cls_804AE2E0::draw() {}

// The leaf emitter ids per kind.
static const u16 lbl_80542D38[4] = {0xFB, 0xFC, 0xFD, 0xFE};

inline void cls_804AE2E0::spawnLeaves() {
    fn_8013CC50(field_0x4, &mPos, lbl_80542D38[field_0x234], 0, 2, NULL, NULL, true, 1.0f);
    if (field_0x23c != 0xFFFFFFFF) {
        fn_801F0E34(field_0x23c, 0);
    }
}

// The tile code under pos on the actor's layer.
static inline u16 tileCodeAt(cls_804AE2E0* self, const JGeometry::TVec2f* pos) {
    return lbl_8053AB10->field_0x20->fn_802F9908(self->field_0x4, fn_80226BC4(pos, self->field_0x4)) &
           0x3FF;
}

// FIXME: 85%. try.py class: missing/extra. In the original the kind == 1 branch
// jumps into the loop's spawn block (the one for a player that is not in state
// 1), so only three copies of the spawn exist; ours inlines a fourth copy
// there.
void cls_804AE2E0::update() {
    // x and y lead the TVec3f; the function reads only those.
    u16 code = tileCodeAt(this, (const JGeometry::TVec2f*)&field_0x244);
    if (code != field_0x240) {
        field_0x250 = 1;
        return;
    }
    if (field_0x234 == 1 && field_0x250 == 1) {
        field_0x250 = 0;
        spawnLeaves();
    } else {
        field_0x250 = 0;
        for (s32 i = 0; i < 4; i++) {
            if (!fn_8023A51C(i)) {
                continue;
            }
            if (fn_802373EC(i) != 1) {
                spawnLeaves();
                break;
            }
            JGeometry::TVec3f pos = *fn_8023E724(i);
            if (PSVECDistance(&mPos, &pos) <= 57.0f && __fabsf(mPos.x - pos.x) <= 42.0f) {
                spawnLeaves();
                break;
            }
        }
    }
    if (field_0x230 == 1) {
        field_0x230 = 0;
        spawnLeaves();
    }
}

void cls_804AE2E0::init() {
    u32 type = field_0x90 & 0x3FF;
    if (type == 0x1E1) {
        field_0x244 = mPos;
        mPos.x += 16.0f;
        mPos.y += 25.0f;
        field_0x240 = type;
    } else if (type == 0x1E2) {
        JGeometry::TVec2f p(mPos.x - 16.0f, mPos.y);
        u16 code = tileCodeAt(this, &p);
        if (code == 0x1E1) {
            mAlive = false;
            return;
        }
        field_0x244 = mPos;
        mPos.y += 25.0f;
        field_0x240 = code;
    } else if (type == 0x1F1) {
        JGeometry::TVec2f p(mPos.x, mPos.y - 16.0f);
        u16 code = tileCodeAt(this, &p);
        if (code == 0x1E1) {
            mAlive = false;
            return;
        }
        field_0x244 = mPos;
        mPos.x += 16.0f;
        mPos.y += 9.0f;
        field_0x240 = code;
    } else {
        JGeometry::TVec2f p(mPos.x - 16.0f, mPos.y);
        u16 code = tileCodeAt(this, &p);
        if (code == 0x1F1) {
            mAlive = false;
            return;
        }
        JGeometry::TVec2f q(mPos.x - 16.0f, mPos.y - 16.0f);
        code = tileCodeAt(this, &q);
        if (code == 0x1E2) {
            mAlive = false;
            return;
        }
        field_0x244 = mPos;
        mPos.y += 9.0f;
        field_0x240 = code;
    }
    field_0x238 = fn_802D6A9C()->fn_802D9C4C();
    field_0x23c = -1;
    char c = field_0x238[0];
    if (c == 'r' || c == 's') {
        field_0x234 = 1;
        field_0x23c = 0x50046;
    } else if (c == 'l' || c == 'x' || c == 'p' || c == 't') {
        field_0x234 = 2;
        field_0x23c = 0x50047;
    } else if (c == 'u') {
        field_0x234 = 3;
        field_0x23c = 0x50070;
    } else if (c != '1' || field_0x238[1] != 'b') {
        field_0x234 = 0;
        field_0x23c = 0x50046;
    } else {
        mAlive = false;
    }
    field_0x250 = 0;
    field_0x230 = 0;
}
