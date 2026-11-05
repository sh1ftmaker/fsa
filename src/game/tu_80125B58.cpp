#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/cls_8049E350.h"

// Animations of mAnim244 and mAnim27c: (cel, duration) pairs ended by a cel < 0.
static const s16 lbl_80464EA0[6] = {0, 20, 1, 15, -1, -1};
static const s16 lbl_80464EAC[6] = {2, 20, 3, 15, -1, -1};
static const s16 lbl_80464EB8[6] = {4, 20, 5, 15, -1, -1};
static const s16 lbl_80464EC4[6] = {6, 20, 7, 15, -1, -1};
static const s16 lbl_80464ED0[6] = {0, 4, 2, 10, -1, -1};
static const s16 lbl_80464EDC[6] = {0, 4, 3, 10, -1, -1};

static const s16* lbl_8049E330[4] = {lbl_80464EA0, lbl_80464EAC, lbl_80464EB8, lbl_80464EC4};
static const s16* lbl_8049E340[4] = {lbl_80464ED0, lbl_80464EDC, lbl_80464ED0, lbl_80464EDC};

cls_8049E350::~cls_8049E350() {}

void cls_8049E350::init() {
    ActorBase_804A5770::init();
    field_0x238 = field_0x90 & 1;
    field_0x240 = (field_0x90 >> 22) & 0x1F;
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    mBox2.set(mBox);
    field_0x80.set(mBox);
    field_0x98 = 2;
    mAnim244.fn_801D2608(lbl_8049E330, 4);
    mAnim244.startAnim_801D22DC(2, 2, false);
    mAnim244.fn_801D1F14(0.0f);
    mAnim27c.fn_801D2608(lbl_8049E340, 4);
    mAnim27c.startAnim_801D22DC(0, 0, false);
    mAnim27c.fn_801D1F14(0.0f);
    mDrawEnabled = false;
    if (vfunc_0x3C() != 0 && !vfunc_0x34()) {
        return;
    }
    field_0x234 = 1;
    switch (field_0x234) {
    case 0:
        field_0x230 = 0;
        break;
    case 1:
        vfunc_0x30(0xFF, true);
        field_0x230 = 1;
        mDrawEnabled = true;
        field_0x23c = -1;
        break;
    case 2: {
        s32 bit = field_0x240;
        if (bit > 0) {
            getGameView_802D6A9C()->fn_802D8050(bit, true, 0xFF);
        }
        break;
    }
    }
}

void cls_8049E350::draw() {
    ActorBase_804A5770::draw();
    SpriteParams_801F5AE0 params;
    // FIXME: 82.62 %. The original puts the name's lis/addi and li 0x2E6
    // between the stores of params (0xB4-0xC4 of the diff), so its
    // registers differ from ours from the first reset store on.
    fn_801F02BC("enemy_gc_sob.bin", mAnim244.mCel + (field_0x238 == 1 ? 0x2D2 : 0x2E6),
                &params, 0x407, 0, false, 1.0f, 1.0f);
    if (field_0x231) {
        JGeometry::TVec3f pos(mPos);
        pos.y += -20.0f;
        if (field_0x238 == 1) {
            pos.x += 10.0f;
        } else {
            pos.x += -10.0f;
        }
        if (field_0x4 < 8) {
            void* res = lbl_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
            lbl_8053AB10->field_0x20->fn_802F86CC(&pos, res, mAnim27c.mCel + 0x2A3, NULL, 1.0f, 1.0f);
        }
    }
}

void cls_8049E350::fn_80125BF4() {
    JGeometry::TVec3f pos(mPos);
    pos.y += -24.0f;
    spawnEmitter_8013CC50(field_0x4, &pos, 0x33A, 0, 0, NULL, NULL, true, 1.0f);
}

void cls_8049E350::fn_80125C58(bool flag) {
    field_0x231 = flag;
    if (field_0x231) {
        // FIXME: 96.82 %. The original compares field_0x238 with 1 as
        // beq / bge / b (an extra bge and b to the default block); ours
        // has only beq and b.
        switch (field_0x238) {
        case 1:
            mAnim27c.startAnim_801D22DC(0, 0, false);
            break;
        default:
            mAnim27c.startAnim_801D22DC(1, 0, false);
            break;
        }
        mAnim27c.fn_801D1F14(0.0f);
    }
}
