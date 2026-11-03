#include "game/GameAudio.h"
#include "game/cls_8049D740.h"

#include "game/ActorId.h"
#include "game/ActorManager.h"

// The Maiden's animation keys: (cel, frames) s16 pairs, ended by a cel of -1.
static const s16 lbl_8053C600[4] = {6, 20, -1, -1};
static const s16 lbl_8053C608[4] = {7, 20, -1, -1};
static const s16 lbl_8053C610[4] = {8, 20, -1, -1};
static const s16 lbl_8053C618[4] = {9, 20, -1, -1};
static const s16 lbl_8053C620[4] = {8, 9, -1, -1};
static const s16 lbl_80464678[14] = {0, 9, 1, 9, 2, 9, 3, 9, 4, 9, 5, 9, -1, -1};
static const s16 lbl_80464694[10] = {8, 5, 0x15, 6, 0x16, 6, 0x17, 60, -1, -1};
static const s16 lbl_804646A8[8] = {8, 9, 0xC, 9, 0xD, 40, -1, -1};
static const s16 lbl_804646B8[14] = {0xE, 9, 0xF, 9, 0x10, 9, 0x11, 9, 0x12, 9, 0x13, 9, -1, -1};
static const s16 lbl_804646D4[8] = {8, 6, 0x14, 9, 0xE, 9, -1, -1};
static const s16 lbl_804646E4[8] = {8, 1, 0xA, 30, 0xB, 10, -1, -1};
static const s16 lbl_804646F4[8] = {0x13, 9, 0x14, 9, 8, 40, -1, -1};

// The key tables of mAnim, in address order.
static const s16* lbl_8049D710[12] = {
    lbl_80464678, lbl_8053C600, lbl_8053C608, lbl_8053C610, lbl_8053C618, lbl_8053C620,
    lbl_80464694, lbl_804646A8, lbl_804646B8, lbl_804646D4, lbl_804646E4, lbl_804646F4,
};

void cls_8049D740::fn_801133C0() {
    cls_8049D140::init();
    mMode = field_0x90 & 0x1F;
    mAnim.fn_801D2608(lbl_8049D710, 12);
    mAnim.fn_801D22DC(5, 2, false);
    mAnim.fn_801D1F14(0.0f);
}

cls_8049D740::~cls_8049D740() {
    fn_801EE434(mEmitter298);
    mEmitter298 = NULL;
    fn_801EE434(mEmitter29c);
    mEmitter29c = NULL;
    fn_801EE434(mEmitter2a0);
    mEmitter2a0 = NULL;
}

void cls_8049D740::vfunc_0x10() {
    s32 serial = fn_801F856C()->fn_801F7A08(ACTOR_DMGL);
    ActorBase_804A5770* goal = fn_801F856C()->fn_801F666C(serial);
    if (goal != NULL) {
        mGoalPos = goal->mPos;
    }
}

s32 cls_8049D740::vfunc_0x58() {
    return field_0x238;
}
