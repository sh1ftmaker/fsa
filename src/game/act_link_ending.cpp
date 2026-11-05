#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/Player.h"
#include "game/act/act_link_ending.h"

LinkEnding_804AD858::~LinkEnding_804AD858() {}

// The state table: update() calls the function for mState.
void (LinkEnding_804AD858::*lbl_804AD7F8[8])() = {
    &LinkEnding_804AD858::fn_8035ED60,
    &LinkEnding_804AD858::fn_8035EC98,
    &LinkEnding_804AD858::fn_8035EB90,
    &LinkEnding_804AD858::fn_8035EB18,
    &LinkEnding_804AD858::fn_8035EAA4,
    &LinkEnding_804AD858::fn_8035EA48,
    &LinkEnding_804AD858::fn_8035E9A0,
    &LinkEnding_804AD858::fn_8035E93C,
};

// Sprite ids of fn_8035EC98's effects.
static const u16 lbl_80472820[20] = {
    0x038A, 0x038B, 0x038C, 0x038D, 0x038E, 0x038F, 0x0390, 0x0391, 0x0392, 0x0393,
    0x0394, 0x0395, 0x002C, 0x002D, 0x002E, 0x002F, 0x0030, 0x0000, 0x0000, 0x0000,
};

// State 7: walks mPos.y up to 1966 at 0.23 a step, cycling the animation step;
// past 1966 the actor is no longer alive.
void LinkEnding_804AD858::fn_8035E93C() {
    if (mPos.y < 1966.0f) {
        if (field_0x244 == 0) {
            field_0x244 = 6;
            field_0x248++;
            if (field_0x248 >= 8) {
                field_0x248 = 0;
            }
        }
        mPos.y += 0.23f;
    } else {
        mAlive = false;
    }
}

// State 6: follows the partner's state and height: while the partner is at or
// above 1810 it picks the animation base (0x3D, or 0x3F while the partner is in
// state 4 with a countdown under 0xF0), and on the partner's go flag it moves
// to state 7 with base frame 0x10.
void LinkEnding_804AD858::fn_8035E9A0() {
    cls_804A5958* partner =
        static_cast<cls_804A5958*>(fn_801F856C()->findActorBySerial_801F666C(field_0x90));
    if (partner != NULL && partner->mPos.y >= 1810.0f) {
        field_0x248 = 0;
        field_0x250 = 0x3d;
        if (partner->mState == 4 && partner->field_0x234 < 0xF0) {
            field_0x250 = 0x3f;
        }
    }
    if (field_0x238 != 0) {
        field_0x248 = 0;
        field_0x250 = 0x10;
        mState++;
    }
}

// State 5: once the countdown runs out, sets the partner's state to 3.
void LinkEnding_804AD858::fn_8035EA48() {
    if (field_0x234 == 0) {
        mState++;
        cls_804A5958* partner =
            static_cast<cls_804A5958*>(fn_801F856C()->findActorBySerial_801F666C(field_0x23c));
        if (partner != NULL) {
            partner->mState = 3;
        }
    }
}

// State 4: plays a sound, then tells the partner to start (its field_0x238 =
// 1) and waits 0x78 frames.
void LinkEnding_804AD858::fn_8035EAA4() {
    if (field_0x234 == 0) {
        startSound_801F0E34(0x30085, 0);
        cls_804A5958* partner =
            static_cast<cls_804A5958*>(fn_801F856C()->findActorBySerial_801F666C(field_0x23c));
        if (partner != NULL) {
            partner->field_0x238 = 1;
        }
        field_0x234 = 0x78;
        mState++;
    }
}

// State 3: moves mPos 48 right and 400 up, then a 0x10E countdown; on the
// countdown's 10th frame it plays a sound.
void LinkEnding_804AD858::fn_8035EB18() {
    if (field_0x234 == 0) {
        mPos.x += 48.0f;
        mPos.y -= 400.0f;
        field_0x234 = 0x10e;
        mState++;
    } else if (field_0x234 == 10) {
        startSound_801F0E34(0x30088, 0);
    }
}

// State 2: drops mPos.y by 400, sends the three side sprites (fn_80230498 with
// the field_0x254 positions), then a 0x17C countdown; field_0x264 goes to -1
// so update() takes the player-index path from here on.
void LinkEnding_804AD858::fn_8035EB90() {
    if (field_0x234 == 0) {
        mPos.y += 400.0f;
        fn_80230498(0, &mPos, field_0x250 + field_0x248, 0xff, -0x80, -0x80, true);
        field_0x260 = mPos.y;
        for (s32 i = 0; i < 3; i++) {
            JGeometry::TVec3f pos(field_0x254[i], field_0x260, 0.0f);
            fn_80230498(i + 1, &pos, field_0x264 + field_0x268, 0xff, -0x80, -0x80, true);
        }
        field_0x264 = -1;
        field_0x234 = 0x17c;
        mState++;
    }
}

// State 1: spawnEmitter_8013CC50 at the fixed point (1280, 1728) for each of the 17 ids
// of lbl_80472820, then a sound and a 0x14 countdown.
// FIXME: 99.20 %. Regalloc only: the original keeps the id table's base in
// r30 and the loop counter in r31; here they are swapped.
void LinkEnding_804AD858::fn_8035EC98() {
    if (field_0x234 == 0) {
        JGeometry::TVec3f pos;
        pos.x = 1280.0f;
        pos.y = 1728.0f;
        pos.z = 0.0f;
        for (s32 i = 0; i < 0x11; i++) {
            spawnEmitter_8013CC50(field_0x4, &pos, lbl_80472820[i], 1, 1, NULL, NULL, true, 1.0f);
        }
        startSound_801F0E34(0x30087, 0);
        field_0x234 = 0x14;
        mState++;
    }
}

// State 0: copies mPos.y, walks it down to 1810 (0.4 a step) and then hands
// the partner over to state 2 with a 0x8F countdown.
// FIXME: 99.90 %. The original's branch on the height is a bne to the partner
// block (cror + bne); this one is a beq, the only opcode that differs.
void LinkEnding_804AD858::fn_8035ED60() {
    field_0x260 = mPos.y;
    if (!(mPos.y >= 1810.0f)) {
        mPos.y -= 0.4f;
        if (field_0x244 == 0) {
            field_0x244 = 7;
            field_0x248++;
            if (field_0x248 >= 8) {
                field_0x248 = 0;
            }
        }
        field_0x268 = field_0x248;
    } else {
        field_0x268 = 0;
        field_0x248 = 0;
        field_0x244 = 0;
        cls_804A5958* partner =
            static_cast<cls_804A5958*>(fn_801F856C()->findActorBySerial_801F666C(field_0x23c));
        if (partner != NULL) {
            partner->mState = 2;
        }
        field_0x234 = 0x8f;
        mState++;
    }
}

void LinkEnding_804AD858::draw() {}

void LinkEnding_804AD858::update() {
    if (field_0x234 != 0) {
        field_0x234--;
    }
    if (field_0x244 != 0) {
        field_0x244--;
    }
    (this->*lbl_804AD7F8[mState])();
    if (field_0x264 >= 0) {
        fn_80230498(0, &mPos, field_0x250 + field_0x248, 0xff, -0x80, -0x80, true);
        for (s32 i = 0; i < 3; i++) {
            JGeometry::TVec3f pos(field_0x254[i], field_0x260, 0.0f);
            fn_80230498(i + 1, &pos, field_0x264 + field_0x268, 0xff, -0x80, -0x80, true);
        }
    } else {
        s32 value = lbl_80529DEC.field_0x1a4;
        s32 player = value;
        if (value < 0) {
            player = 0;
        }
        if (fn_8022F514(player)) {
            player = fn_8022F3DC(player);
        }
        fn_80230498(player, &mPos, field_0x250 + field_0x248, 0xff, -0x80, -0x80, true);
    }
}

void LinkEnding_804AD858::init() {
    mState = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x24c = 0;
    field_0x250 = 0x18;
    fn_80234810();
    fn_802382D0(0, 0);
    fn_8023077C();
    for (s32 i = 0; i < 3; i++) {
        field_0x254[i] = 32.0f + (mPos.x + i * 32.0f);
    }
    field_0x260 = mPos.y;
    field_0x264 = 0x18;
    field_0x268 = 0;
    field_0x26c = 0;
}
