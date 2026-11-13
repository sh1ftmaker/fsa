#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/EnemyBase.h"
#include "game/Message.h"
#include "game/Player.h"
#include "game/GameView.h"
#include "game/act/tu_801286C0.h"
#include "game/act/tu_8044C028.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "JSystem/JUtility/JUTVideo.h"

// Auto-inlining is off for the whole file (docs/INLINING.md, pragma rule 2): the
// original inlines no same-unit plain function. Checked: built with inlining off,
// every written function's calls to this unit's functions equal the original's
// `bl`s. The callers that need it: draw calls fn_80128C58 (4 instructions) out of line.
#pragma auto_inline off

const char* lbl_80539F38 = "kk_black.bti";

// Thresholds that the value at fn_8011921C()->field_0x8 is matched against.
const s32 lbl_80464FC0[11] = {0, 60, 60, 60, 150, 280, 340, 610, 670, 820, 880};
void cls_8049E500::init() {
    cls_8049D140::init();
    // '????' (escaped: "??'" would be a trigraph).
    field_0x23c = gGameRoot_8053AAF8->field_0xa4->getResource('?\?\?\?', lbl_80539F38);
    getGameView_802D6A9C()->fn_802D7440(false);
    fn_80128728(0);
}

// FIXME: 84.86%. try.py class: scheduling+regalloc with reloc-name. The
// original loads the 96.0f constant before the camera's byte at 0x179 is
// stored (ours after), and the float pool (lbl_8053CAF8..CB18) and the state
// jump table are @NNN in ours. Not yet pinned down; the .sdata2 order
// probably depends on the unwritten fn_80128728, which uses the 0.5f, 0.0f,
// 1.0f and 40.0f constants at lower addresses.
void cls_8049E500::update() {
    cls_8049D140::update();
    gGameRoot2_8053AB10->field_0x24->field_0x179 = 1;
    Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
    if (camera->field_0x840 == 0) {
        camera->field_0x840 = 1;
        camera->field_0x844.set(mPos.x, mPos.y - 96.0f, mPos.x, 96.0f + mPos.y);
        camera->field_0x854 = 1.0f;
        camera->field_0x1d4 = -1.0f;
        camera->field_0x1d8 = -1.0f;
    }
    if (field_0x230 <= 9) {
        cls_804A03A8* sub = gGameRoot2_8053AB10->field_0x30;
        if (sub->field_0x284 == 0) {
            sub->field_0x284 = 1;
            sub->field_0x28c = 0.05f;
        }
    }
    if (field_0x230 >= 1 && field_0x230 <= 3) {
        lbl_8053AF38->fn_8044C028(field_0x238);
    }
    switch (field_0x230) {
    case 0:
        if (fn_8011921C()->field_0x0 == 0) {
            gAudioMgr_8053AF80->fn_80458F9C(60);
            fn_8011921C()->fn_801186B0(12);
        }
        break;
    case 1:
        fn_801CD950(&field_0x238, 300.0f, 0.4f);
        if (fn_80119DD0()->field_0x0 == 0) {
            fn_80128728(2);
        }
        break;
    case 2:
        if (fn_801CD950(&field_0x238, 1340.0f, 0.4f) == 0) {
            fn_80128728(3);
        }
        break;
    case 3:
        fn_801CD950(&field_0x238, 1640.0f, 0.4f);
        if (fn_80119DD0()->field_0x4 >= 0xB3) {
            fn_8011921C()->fn_801183BC();
            fn_80119DD0()->field_0x1 = 1;
        }
        break;
    case 4:
    case 8:
    case 9:
    case 10:
        break;
    case 5:
        fn_8022FE80(0, field_0x4, mSerial, 1, 0);
        if (fn_803865B8(0)) {
            fn_80388EEC(0);
            fn_8011921C()->fn_801183BC();
        }
        break;
    case 6:
        fn_801CD950(&field_0x234, 255.0f, -2.125f);
        break;
    case 7:
        fn_8022FE80(0, field_0x4, mSerial, 1, 0);
        if (fn_803865B8(0)) {
            fn_80388EEC(0);
            fn_8011921C()->fn_801183BC();
        }
        break;
    }
}

void cls_8049E500::draw() {
    fn_80128C58((s32)field_0x234);
    if (field_0x230 >= 1 && field_0x230 <= 3) {
        lbl_8053AF38->fn_8044C028(field_0x238);
    }
    cls_8049D140::draw();
}

// Stores the value in the byte at 0xC of gGameRoot2_8053AB10->0x2C (ShadowCircle_804A0160).
void fn_80128C58(s32 value) {
    gGameRoot2_8053AB10->field_0x2c->field_0xc = value;
}

void cls_8049E500::vfunc_0x54() {
    if (fn_8011921C()->field_0x4 != 0xC) {
        return;
    }
    s32 value = fn_8011921C()->field_0x8;
    if (value == lbl_80464FC0[1]) {
        fn_80128728(1);
    } else if (value == lbl_80464FC0[4]) {
        fn_80128728(4);
    } else if (value == lbl_80464FC0[5]) {
        fn_80128728(5);
    } else if (value == lbl_80464FC0[6]) {
        fn_80128728(6);
    } else if (value == lbl_80464FC0[7]) {
        fn_80128728(7);
    } else if (value == lbl_80464FC0[8]) {
        fn_80128728(8);
    } else if (value == lbl_80464FC0[9]) {
        fn_80128728(9);
    } else if (value == lbl_80464FC0[10]) {
        fn_80128728(10);
    }
}

s32 cls_8049E500::vfunc_0x58() {
    return field_0x230;
}

s32 cls_8049E500::vfunc_0x5C() {
    return field_0x230 == 10;
}

// FIXME(shotgun): 63.1 %, missing/extra: the camera-box centre in case 4 and the screen-centre vector of cases 5 / 7 are built differently; jump table is @NNN
void cls_8049E500::fn_80128728(int state) {
    switch (state) {
    case 0:
        fn_80119DD0()->fn_80119998(0xB4, 0, 0, 0, 0, 0, 0);
        fn_80119DD0()->field_0x1 = 1;
        break;
    case 1:
        gAudioMgr_8053AF80->fn_80458FF0(0x100002C, false, 0);
        fn_8011921C()->fn_80118454();
        fn_80119DD0()->field_0x1 = 0;
        break;
    case 3:
        fn_80119DD0()->fn_80119998(0xB4, 0, 1, 0, 0, 0, 0);
        fn_80119DD0()->field_0x1 = 0;
        break;
    case 4: {
        gAudioMgr_8053AF80->fn_80458F9C(0x3C);
        fn_80119DD0()->field_0x1 = 0;
        GameView_804A9950* view = getGameView_802D6A9C();
        JGeometry::TBox2f box;
        box = view->mVisibleBox;
        JGeometry::TVec3f pos(0.5f * (box.f.x - box.i.x) + box.i.x, 0.5f * (box.f.y - box.i.y) + box.i.y, 0.0f);
        spawnEmitter_8013CC50(field_0x4, &pos, 0x33C, 1, 2, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &pos, 0x33D, 1, 2, NULL, NULL, true, 1.0f);
        break;
    }
    case 5: {
        GXRenderModeObj* mode = JUTVideo::getManager()->getRenderMode();
        JGeometry::TVec3f pos(0.5f * (s32)mode->fbWidth, 0.5f * (s32)mode->efbHeight - 40.0f, 0.0f);
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        if (fn_8038BD08(0, 0x231, field_0x4, pos, 0x62)) {
            fn_8011921C()->fn_80118454();
        }
        break;
    }
    case 7: {
        GXRenderModeObj* mode = JUTVideo::getManager()->getRenderMode();
        JGeometry::TVec3f pos(0.5f * (s32)mode->fbWidth, 0.5f * (s32)mode->efbHeight - 40.0f, 0.0f);
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        if (fn_8038BD08(0, 0x23C, field_0x4, pos, 0x62)) {
            fn_8011921C()->fn_80118454();
        }
        break;
    }
    case 6:
        gAudioMgr_8053AF80->fn_80458FF0(0x100001D, false, 0);
        getActorManager_801F856C()->spawnActor_801F9484('SEV2', field_0x4, &mPos, 0x2000, -1, -1);
        break;
    case 8:
        fn_80119DD0()->fn_80119998(0x10E, 0, 2, 0, 0, 0x3C, 0);
        gAudioMgr_8053AF80->fn_80458F9C(0x96);
        break;
    case 9: {
        s32 serial = getActorManager_801F856C()->fn_801F7A08('SEV2');
        if (serial >= 0) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (actor != NULL) {
                actor->fn_801F3668();
            }
        }
        break;
    }
    case 10:
        fn_8011921C()->fn_801184F0(false);
        getGameView_802D6A9C()->fn_802D9FF4(2, 4);
        getGameView_802D6A9C()->fn_802DA8DC();
        break;
    }
    field_0x230 = state;
}

cls_8049E500::~cls_8049E500() {
    field_0x23c = 0;
}
