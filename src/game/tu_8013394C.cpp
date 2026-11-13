#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/Message.h"
#include "game/Player.h"
#include "game/act/tu_801F9484.h"

static const s16 lbl_80465288[14] = {0, 1, 1, 1, 2, 1, 3, 1, 4, 1, 5, 1, -1, -1};
static const s16* lbl_80539F50[2] = {lbl_80465288, NULL};

static const u32 lbl_804652A4[7] = {0x33B, 0x33C, 0x33D, 0x33E, 0x33F, 0x340, 0x340};
static const s32 lbl_804652C0[6] = {0x1D6, 0x1E8, 0x1D0, 0x1EE, 0x1E2, 0x1DC};

static inline void setState(cls_8049ED30* self, s32 state) {
    self->field_0x230 = state;
    switch (self->field_0x230) {
    case 0:
        self->field_0x238 = -1;
        break;
    case 2:
        self->field_0x23c = 0.0f;
        break;
    case 3:
        for (s32 i = 0; i < 2; i++) {
            releaseEmitter_801EE434(self->mEmitters[i]);
            self->mEmitters[i] = NULL;
        }
        self->fn_801F3668();
        break;
    }
}

static inline s32 checkTalk(cls_8049ED30* self, u32 msg) {
    JGeometry::TBox2f box(self->mBox);
    box.addPos(self->mPos.x, self->mPos.y);
    fn_8022FEB8(&box, self->field_0x4, self->mSerial, true, false);
    s32 p = fn_8022FD8C(self->mSerial);
    if (p < 0 || p >= 4) {
        return -1;
    }
    if (!fn_802300EC(p)) {
        return -1;
    }
    if (!fn_8038AD24(p, msg, self->mSerial, self->field_0x4, self->mPos, 0x28)) {
        return -1;
    }
    return p;
}

// 0x8013416C.
// FIXME(shotgun): 99.80 %, reloc-name; behaviour clean.
void cls_8049ED30::init() {
    ActorBase_804A5770::init();
    field_0x234 = field_0x90 & 0x1F;
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    mBox2.i.x = mBox.i.x;
    mBox2.i.y = mBox.i.y;
    mBox2.f.x = mBox.f.x;
    mBox2.f.y = mBox.f.y;
    field_0x80.i.x = mBox.i.x;
    field_0x80.i.y = mBox.i.y;
    field_0x80.f.x = mBox.f.x;
    field_0x80.f.y = mBox.f.y;
    field_0x240.fn_801D2608(lbl_80539F50, 1);
    field_0x240.startAnim_801D22DC(0, 2, false);
    field_0x240.fn_801D1F14(0.0f);
    if (mEmitters[0] == NULL) {
        mEmitters[0] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x32C, 0, 0, NULL, NULL, true, 1.0f);
    }
    if (mEmitters[1] == NULL) {
        mEmitters[1] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x183, 0, 0, NULL, NULL, true, 1.0f);
    }
}

// 0x80133DD0.
// FIXME(shotgun): 87.65 %, different + reloc-name; behaviour drift.
void cls_8049ED30::update() {
    ActorBase_804A5770::update();
    JGeometry::TBox2f box(mBox);
    box.addPos(mPos.x, mPos.y);
    switch (field_0x230) {
    case 0:
        fn_801339EC();
        field_0x238 = checkTalk(this, lbl_804652A4[field_0x234]);
        if (field_0x238 >= 0) {
            setState(this, 1);
        }
        break;
    case 1:
        fn_8022FF8C(field_0x238, &box, field_0x4, mSerial, true, false);
        if (fn_80388CAC(field_0x238, mSerial)) {
            setState(this, 0);
        }
        break;
    case 2:
        fn_801CD950(&field_0x23c, 2.0f, 0.02f);
        fn_801CD950(&mPos.z, 500.0f, field_0x23c);
        fn_801339EC();
        if (mPos.z >= 150.0f) {
            setState(this, 3);
        }
        break;
    }
    fn_80239914(&box, field_0x4, mSerial);
    field_0x240.fn_801D1C18();
}

// 0x80133C6C.
// FIXME(shotgun): 99.55 %, regalloc-only + reloc-name; behaviour clean.
void cls_8049ED30::draw() {
    ActorBase_804A5770::draw();
    SpriteParams_801F5AE0 params;
    fn_801F02BC("enemy_both2_sob.bin", lbl_804652C0[field_0x234] + field_0x240.mCel, &params, 7, 0, false, 1.0f, 1.0f);
}

// 0x80133B58.
// FIXME(shotgun): 98.70 %, regalloc-only + reloc-name; behaviour clean.
bool fn_80133B58() {
    bool res = true;
    s32 serial = getActorManager_801F856C()->fn_801F7A08('CRNV');
    while (serial != -1) {
        cls_8049ED30* actor = (cls_8049ED30*)getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor == NULL) {
            break;
        }
        bool ok;
        if (actor->field_0x230 == 1) {
            ok = false;
        } else {
            setState(actor, 2);
            ok = true;
        }
        if (!ok) {
            res = false;
        }
        serial = getActorManager_801F856C()->fn_801F78F4('CRNV', serial);
    }
    return res;
}

// 0x801339EC.
// Not auto-inlined: update() calls fn_801339EC out of line in the original (docs/INLINING.md, pragma rule 1).
#pragma auto_inline off
// FIXME(shotgun): 86.15 %, missing/extra; behaviour drift.
// (3-byte emitter color copy at emitter+0xBC..0xBE not written; JPABaseEmitter header layout differs).
void cls_8049ED30::fn_801339EC() {
    static JUtility::TColor colors[6] = {
        JUtility::TColor(0x2D, 0x78, 0xFF, 0xFF),
        JUtility::TColor(0xC8, 0xA0, 0x28, 0xFF),
        JUtility::TColor(0x4B, 0xFF, 0x2D, 0xFF),
        JUtility::TColor(0x1E, 0xA0, 0xBE, 0xFF),
        JUtility::TColor(0xFF, 0x41, 0x37, 0xFF),
        JUtility::TColor(0xDC, 0x1E, 0xDC, 0xFF),
    };
    for (s32 i = 0; i < 2; i++) {
        if (mEmitters[i] != NULL) {
            setEmitterPos_8013CB44(mEmitters[i], &mPos, 0, 0, true);
            // FIXME(shotgun): emitter color copy from colors[field_0x234] not written.
        }
    }
}
#pragma auto_inline reset

// 0x8013394C.
// FIXME(shotgun): 89.62 %, different; behaviour clean.
cls_8049ED30::~cls_8049ED30() {
    for (s32 i = 0; i < 2; i++) {
        releaseEmitter_801EE434(mEmitters[i]);
        mEmitters[i] = NULL;
    }
}
