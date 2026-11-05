#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/act/act_thwomp.h"

Thwomp_804AB5D8::~Thwomp_804AB5D8() {}

void Thwomp_804AB5D8::draw() {
    if (field_0x4 < 8) {
        void* res = lbl_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
        lbl_8053AB10->field_0x20->fn_802F86CC(&mPos, res, 0x17A, NULL, 1.0f, 1.0f);
    }
    fn_803075AC(mSerial, field_0x4, &mPos, 0x17A, false, 2, -1, 0, 0, 0);
}

// FIXME: 98.55 %. Scheduling and register allocation only: the li of `hit`
// is placed two instructions earlier than in the original, which shifts the
// r28-r30 assignment. hitMap is an s32 because the original keeps
// fn_801F2618's result zero-extended at the call (clrlwi r30, r3, 24) and
// tests it with cmpwi; a bool gives mr + clrlwi.
void Thwomp_804AB5D8::update() {
    bool hit = false;
    JGeometry::TVec2f pos(mPos.x, mPos.y);
    JGeometry::TBox2f box(mBox);
    box.addPos(pos);
    for (s32 i = 0; i < 4; i++) {
        field_0x238[i] = 0;
    }
    mPos += field_0x3c;
    s32 hitMap = fn_801F2618(NULL, false);
    for (s32 i = 0; i < 4; i++) {
        if (fn_8023F264(i) && !fn_8023EA28(i)) {
            if (fn_8023E724(i)->y > mPos.y && mPos.x - 15.0f < fn_8023E724(i)->x &&
                15.0f + mPos.x > fn_8023E724(i)->x && field_0x230 == 1 && !hitMap) {
                if (fn_8023B190(i, &box, field_0x4)) {
                    JGeometry::TVec2f p(box.f.x + 0.5f * (box.i.x - box.f.x),
                                        box.f.y + 0.5f * (box.i.y - box.f.y));
                    fn_80238548(i, field_0x114, &p, 20);
                    if (!hit) {
                        hit = true;
                    }
                }
            }
            fn_802399D8(i, &box, field_0x4, mSerial);
            field_0x238[i] = fn_80239718(i, mSerial);
        }
        if (field_0x230 == 0) {
            if (mPos.x - 40.0f < fn_8023E724(i)->x && 40.0f + mPos.x > fn_8023E724(i)->x) {
                field_0x230 = 1;
            }
        }
    }
    switch (field_0x230) {
    case 0: // the original has a branch for state 0 that does nothing
        break;
    case 1:
        if (hitMap || hit == true) {
            getGameView_802D6A9C()->fn_802D79E8(field_0x4, 4.0f, 0.0f, false);
            startSound_801F0E34(0x3000F, 0);
            field_0x230 = 2;
            field_0x234 = 100;
        }
        break;
    case 2:
        field_0x234--;
        if (field_0x234 <= 0) {
            field_0x230 = 3;
        }
        break;
    case 3:
        if (mPos.y <= field_0x23c.y) {
            mPos.y = field_0x23c.y;
            field_0x230 = 0;
        }
        break;
    }
    if (field_0x230 == 1) {
        if (field_0x3c.y < 5.0f) {
            field_0x3c.y += 0.1f;
        }
    } else if (field_0x230 == 3) {
        field_0x3c.y = -0.5f;
    } else {
        field_0x3c.y = 0.0f;
    }
}

void Thwomp_804AB5D8::init() {
    mLife = 10000;
    field_0x23c = mPos;
    field_0x230 = 0;
    field_0x234 = 0;
    field_0x238[0] = 0;
    field_0x238[1] = 0;
    field_0x238[2] = 0;
    field_0x238[3] = 0;
    field_0x114 = 4;
    field_0x80.set(-15.0f, -23.0f, 15.0f, 7.0f);
    mBox.set(-15.0f, -23.0f, 15.0f, 15.0f);
}
