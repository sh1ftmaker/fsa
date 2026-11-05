#include "game/GameAudio.h"
#include "game/act/tu_802B7C78.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Player.h"

// Read as data by fn_802B7D1C (lbl_804A8858 in the original).
s32 lbl_804A8858[5] = {0, 1, 2, 3, 3};

// Frame offsets into the "SOB " bank, picked by field_0x24c.
const s32 lbl_8046CB60[4] = {0, 1, 0, 2};

static inline void normalize(JGeometry::TVec2f& v) {
    f32 sq = v.x * v.x + v.y * v.y;
    if (sq <= 32.0f * FLT_EPSILON) {
        return;
    }
    f32 norm = JGeometry::TUtil<f32>::inv_sqrt(sq);
    v.x *= norm;
    v.y *= norm;
}

cls_804A886C::~cls_804A886C() {}

void fn_802B7CD8(ActorBase_804A5770* actor) {
    getGameView_802D6A9C()->fn_802D9FBC();
    actor->vfunc_0x24();
}

// FIXME: 88.43 %. The original copies arg to r31 first and keeps idx in r28,
// and its frame is 0x40 with the playerPos copy at 0x10 (ours: arg unsaved,
// idx in r29, playerPos at 0x8). The direction temp's dead stack stores are
// not reproduced.
void cls_804A886C::fn_802B7D1C(s32 idx, s32 arg) {
    JGeometry::TVec3f playerPos(*getPlayerPos_8023E724(field_0x298));
    s32 dist = (s32)FABS(mPos.x - playerPos.x);
    if (dist < 40) {
        field_0x250 = 1;
    } else if (mPos.x < playerPos.x) {
        field_0x250 = 2;
    } else {
        field_0x250 = 4;
    }

    startSound_801F0E34(0x40040, 0);
    s32 layer = field_0x4;
    if (getPlayerLayer_8023DE58(field_0x298) == layer) {
        JGeometry::TVec3f* p = getPlayerPos_8023E724(field_0x298);
        JGeometry::TVec2f dir(p->x, p->y);
        dir.x -= mPos.x;
        dir.y -= mPos.y;
        field_0x284 = dir;
        normalize(field_0x284);

        field_0x254 = field_0x284.x * field_0x268[idx];
        field_0x258 = field_0x284.y * field_0x268[idx];
        addCalc2_801F71A4(&field_0x3c.x, field_0x254, 0.3f, field_0x260[idx]);
        addCalc2_801F71A4(&field_0x3c.y, field_0x258, 0.3f, field_0x260[idx]);
        addCalc2_801F71A4(&field_0x260[idx], 1.0f, 0.1f, 0.1f);
        mPos += field_0x3c;

        s32 damage = field_0x114;
        s32 arg3 = 0x12;
        if (lbl_8053AAF8->field_0x7f == 0) {
            s32 n = fn_80237774(field_0x298);
            damage = n > 1 ? n - 1 : 0;
            arg3 = 4;
        }
        if (fn_8023F264(field_0x298) == true && !fn_80236B50(field_0x298)) {
            u8 hit = fn_801F2B7C(damage, -1, arg3);
            if (hit) {
                startSound_801F0E34(0x40041, 0);
                field_0x244 = 140;
                field_0x234 = mPos;
                if (field_0x298 == lbl_804A8858[hit >> 1]) {
                    field_0x230 = arg;
                    field_0x250 = 0;
                }
                field_0x280 = 100;
                if (lbl_8053AAF8->field_0x7e == 0) {
                    mAlive = false;
                    startSound_801F0E34(0x40041, 0);
                }
            }
        } else {
            field_0x244 = 180;
            field_0x230 = arg;
            field_0x250 = 0;
        }
    } else {
        field_0x244 = 180;
        field_0x234 = mPos;
        field_0x230 = 3;
        field_0x250 = 0;
    }
}

void cls_804A886C::vfunc_0x24() {
    spawnEmitter_8013CC50(field_0x4, &mPos, 0x173, 1, 0, NULL, NULL, true, 1.0f);
    mAlive = false;
    startSound_801F0E34(0x40041, 0);
}

void cls_804A886C::draw() {
    if (field_0x230 == 0) {
        return;
    }
    SpriteParams_801F5AE0 params;
    params.field_0x1c = 1;
    params.field_0x1d = 1;
    params.field_0x10.mColor.a = field_0x270;
    params.field_0x30 = 8;
    drawSprite_801F06F0("enemy_gc_sob.bin", lbl_8046CB60[field_0x24c] + 0x466, &params, 7, 1.0f, 1.0f);
}

// FIXME: 99.64 %. Only reloc-names: the -8.0f, 8.0f and 2.0f literals are
// @603-@605 here, lbl_80540ADC/AE0/AE4 in the original; they sit at the right
// offsets once fn_802B8078 and update() (which use 16.0f and 48.0f) are written.
void cls_804A886C::init() {
    mBox.i.x = -8.0f;
    mBox.i.y = -8.0f;
    mBox.f.x = 8.0f;
    mBox.f.y = 8.0f;
    field_0x244 = 180;
    field_0x248 = 20;
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x260[0] = 1.0f;
    field_0x268[0] = 2.0f;
    field_0x260[1] = 0.75f;
    field_0x268[1] = 1.5f;
    field_0x234 = mPos;
    field_0x270 = 0;
    field_0x274 = 0;
    field_0x298 = 4;
    field_0x280 = 0;
    for (s32 i = 0; i < 4; i++) {
        field_0x2a0[i] = getActorManager_801F856C()->fn_801F6058(i);
    }
    field_0x27c = 0;
    field_0x196 = 1;
    field_0x230 = 0;
    startSound_801F0E34(0x4003F, 0);
    lbl_8053AED0->fn_8043D67C(0x197, 0, 0);
}
