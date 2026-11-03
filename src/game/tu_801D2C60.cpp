// cls_804A48B0 (game/act/tu_801D2C60.h): the sub-base of EnemyBase_804A4808
// whose draw shows the "enemy_gc_sob.bin" cels 0x10-0x13 (a projectile that
// moves by field_0x3c, hits a player and vanishes after two more frames).

#include "game/GameAudio.h"

#include "game/ActorBase.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/act/tu_801D2C60.h"

// Animation 0 of mAnim: four cels of 3 frames each, ended by a negative cel.
static const s16 lbl_80469460[] = {0x10, 3, 0x11, 3, 0x12, 3, 0x13, 3, -1, -1};
static const s16* lbl_8053A130[] = {lbl_80469460, NULL};

void cls_804A48B0::fn_801D2CF0(const JGeometry::TVec2f* v) {
    field_0x3c.x = v->x;
    field_0x3c.y = v->y;
    field_0x3c.z = 0.0f;
    field_0x98 = fn_801CF0F4();
}

void cls_804A48B0::draw() {
    SpriteParams_801F5AE0 params;
    params.field_0x30 = 12;
    params.field_0x1d = 1;
    fn_801F06F0("enemy_gc_sob.bin", mAnim.mCel + 0xD5, &params, -1, 1.0f, 1.0f);
}

static inline bool contains(const JGeometry::TBox2f& b, const JGeometry::TVec3f& p) {
    return p.x >= b.i.x && p.y >= b.i.y && p.x < b.f.x && p.y < b.f.y;
}

void cls_804A48B0::update() {
    if (mTimer != 0) {
        mTimer--;
        if (mTimer == 0) {
            fn_801F0E34(0x400D1, 0);
            fn_801CB538(field_0x4, &mPos, 0, 0);
            mAlive = false;
        }
    } else {
        s32 layer = field_0x4;
        GameView_804A9950* view = fn_802D6A9C();
        const JGeometry::TBox2f& box = layer < 8 ? view->mVisibleBox : view->field_0xdc;
        if (!contains(box, mPos)) {
            mAlive = false;
        } else if (fn_801F2A10(-1, field_0x98, false, false)) {
            mTimer = 2;
        } else if (fn_801F2B7C(field_0x114, -1, 1)) {
            mAlive = false;
        }
    }
    if (mAlive) {
        mPos += field_0x3c;
    }
    EnemyBase_804A4808::vfunc_0x94();
}

void cls_804A48B0::init() {
    EnemyBase_804A4808::init();
    mBox.set(-2.0f, -2.0f, 2.0f, 2.0f);
    mBox2.set(1.0f, 1.0f, -1.0f, -1.0f);
    field_0x80.set(1.0f, 1.0f, -1.0f, -1.0f);
    field_0xb0 = 0;
    mFlags |= 0x280;
    mLife = 1000;
    mLifeBackup = mLife;
    field_0x194 = 1;
    field_0x114 = 2;
    mAnim.fn_801D2608(lbl_8053A130, 1);
    mAnim.fn_801D22DC(0, 2, false);
    mAnim.fn_801D1D60();
}

cls_804A48B0::cls_804A48B0() {
    mTimer = 0;
}
