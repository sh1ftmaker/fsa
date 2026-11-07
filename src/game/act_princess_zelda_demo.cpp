#include "game/GameAudio.h"
#include "game/act/act_princess_zelda_demo.h"
#include "game/ActorId.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/cls_8049D740.h"

// FIXME(shotgun): 63.6 %, missing/extra
PrincessZeldaDemo_8049D208::PrincessZeldaDemo_8049D208() {
    field_0x230 = 0;
    field_0x231 = 0;
    field_0x234 = 4;
    field_0x238 = 1;
    field_0x23c = -1;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x254 = 0;
    field_0x258 = 0;
    field_0x25c = -1;
    field_0x260[0] = 0;
    field_0x260[1] = 0;
    field_0x268[0] = 0.0f;
    field_0x268[1] = 0.0f;
    field_0x270 = 0.37f;
    field_0x274 = 96.0f;
    field_0x278 = 0.0f;
    field_0x364 = NULL;
    field_0x368 = NULL;
    field_0x36c = NULL;
    for (s32 i = 0; i < 4; i++) {
        field_0x370[i] = NULL;
        if (i < 2) {
            field_0x380[i] = NULL;
        }
    }
}

PrincessZeldaDemo_8049D208::~PrincessZeldaDemo_8049D208() {
    releaseEmitter_801EE434(field_0x364);
    field_0x364 = NULL;
    releaseEmitter_801EE434(field_0x368);
    field_0x368 = NULL;
    releaseEmitter_801EE434(field_0x36c);
    field_0x36c = NULL;
    for (s32 i = 0; i < 4; i++) {
        releaseEmitter_801EE434(field_0x370[i]);
        field_0x370[i] = NULL;
        if (i < 2) {
            releaseEmitter_801EE434(field_0x380[i]);
            field_0x380[i] = NULL;
        }
    }
}

// The animation key tables (lbl_8049D1A0, 26 of them; lbl_80539EF8, 2) are in this unit's data.
extern const s16* lbl_8049D1A0[26];
extern const s16* lbl_80539EF8[2];

// FIXME(shotgun): 98.6 %, missing/extra
void PrincessZeldaDemo_8049D208::init() {
    cls_8049D140::init();
    field_0x234 = field_0x90 & 0xFF;
    field_0x244 = (field_0x90 >> 8) & 0xF;
    field_0x248 = (field_0x90 >> 12) & 0xF;
    field_0x254 = (field_0x90 >> 17) & 0x1F;
    field_0x250 = (field_0x90 >> 22) & 0x1F;
    mAnim0.fn_801D2608(lbl_8049D1A0, 26);
    mAnim0.startAnim_801D22DC(8, 2, false);
    mAnim0.fn_801D1F14(0.0f);
    mAnim1.fn_801D2608(lbl_80539EF8, 2);
    mAnim1.startAnim_801D22DC(0, 2, false);
    mAnim1.fn_801D1F14(0.0f);
    if (field_0x234 == 2) {
        if (getGameView_802D6A9C()->fn_802D800C(field_0x250)) {
            fn_8010F478(0x3d);
        } else {
            mDrawEnabled = false;
        }
    }
}

void PrincessZeldaDemo_8049D208::vfunc_0x10() {
    cls_801192AC* list = fn_8011921C();
    ActorBase_804A5770** goal = list->fn_8011871C(list->mBegin, ACTOR_DMGL);
    if (goal != fn_8011921C()->mEnd) {
        field_0x27c = (*goal)->mPos;
    }

    ActorBase_804A5770** node = fn_8011921C()->mBegin;
    do {
        node = fn_8011921C()->fn_8011871C(node, ACTOR_DMMD);
        if (node == fn_8011921C()->mEnd) {
            break;
        }
        cls_8049D740* maiden = static_cast<cls_8049D740*>(*node);
        if (maiden->mMode < 6) {
            field_0x288[maiden->mMode] = maiden->mPos;
            node++;
        }
    } while (node != fn_8011921C()->mEnd);

    field_0x2d0 = mPos;
}

void PrincessZeldaDemo_8049D208::update() {
    cls_8049D140::update();
    switch (field_0x234) {
    case 0:
    case 1:
        fn_80110E9C();
        break;
    case 2:
        fn_8011093C();
        break;
    case 3: // nothing sets this state
        break;
    }
    mAnim0.fn_801D1C18();
    mAnim1.fn_801D1C18();
    if (field_0x230 != 0) {
        fn_8010F4DC();
    }
}

void PrincessZeldaDemo_8049D208::draw() {
    cls_8049D140::draw();
    if (field_0x230 == 0) {
        if (mAnim0.mAnim >= 0x13) {
            fn_8010F7EC(0x21b);
        } else if (mAnim0.mAnim >= 0xf) {
            fn_8010F7EC(0x274);
        } else {
            fn_8010F7EC(0x9b);
        }
    }
}

s32 PrincessZeldaDemo_8049D208::vfunc_0x58() {
    return field_0x238;
}

bool PrincessZeldaDemo_8049D208::fn_8010FA54() const {
    switch (field_0x238) {
    case 0x2f:
    case 0x3d:
    case 0x3e:
        return true;
    }
    return false;
}

void PrincessZeldaDemo_8049D208::fn_8010F9F0() {
    switch (field_0x234) {
    case 0:
    case 1:
        fn_8010E7E8(0x31);
        break;
    case 2:
        fn_8010E2D4(0x31);
        break;
    case 3: // nothing sets this state
        break;
    }
    field_0x238 = 0x31;
}

// FIXME(shotgun): 94.4 %, missing/extra
void PrincessZeldaDemo_8049D208::fn_8010F478(s32 arg) {
    switch (field_0x234) {
    case 0:
    case 1:
        fn_8010E7E8(arg);
        break;
    case 2:
        fn_8010E2D4(arg);
        break;
    }
    field_0x238 = arg;
}
