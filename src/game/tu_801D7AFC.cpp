#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "JSystem/JGeometry.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/act/tu_801D7AFC.h"

// Animation 0 of mAnim: three cels of 2 frames, ended by a negative cel.
static const s16 lbl_804694D8[] = {0, 2, 1, 2, 2, 2, -1, -1};
static const s16* lbl_8053A140[] = {lbl_804694D8, NULL};

static inline bool isTiny(const JGeometry::TVec3f& v) {
    return v.squared() <= 32.0f * FLT_EPSILON;
}

void cls_804A4BB0::draw() {
    if (isTiny(field_0x54) == false) {
        if (fn_801CD2F8(field_0x4)) {
            fn_803075AC(mSerial, field_0x4, &mPos, mAnim.mCel + 0x1C7, false, 2, -1, 1, 0, 0);
        }
    }
}

static inline bool contains(const JGeometry::TBox2f& b, const JGeometry::TVec3f& p) {
    return p.x >= b.i.x && p.y >= b.i.y && p.x < b.f.x && p.y < b.f.y;
}

// FIXME: 99.47%. Registers only: the target keeps the layer and the player
// mode (1 or 3) in r27 and the loop counter in r28; here the counter gets
// r27, the mode r28 and the layer r29.
void cls_804A4BB0::update() {
    if (isTiny(field_0x3c)) {
        fn_801F2B7C(field_0x114, -1, 1);
    } else {
        s32 layer = field_0x4;
        GameView_804A9950* view = getGameView_802D6A9C();
        const JGeometry::TBox2f& box = layer < 8 ? view->mVisibleBox : view->field_0xdc;
        if (!contains(box, mPos)) {
            mAlive = false;
        } else {
            s32 mode = 1;
            if (!fn_801CD2F8(field_0x4)) {
                mode = 3;
            }
            u8 hits = fn_801F2740(-1);
            if (hits) {
                for (s32 i = 0; i < 4; i++) {
                    if (hits & (1 << i)) {
                        JGeometry::TVec2f p(fn_8023E724(i)->x - field_0x3c.x,
                                            fn_8023E724(i)->y - fn_8023E724(i)->z - field_0x3c.y);
                        fn_80238548(i, field_0x114, &p, mode);
                    }
                }
            }
            if (hits) {
                mAlive = false;
            } else {
                if (fn_801CD2F8(field_0x4)) {
                    mAnim.fn_801D1C18();
                }
                mPos += field_0x3c;
            }
        }
    }
    setEmitterPos_8013CB44(mEmitters[0], &mPos, 1, 2, true);
    setEmitterPos_8013CB44(mEmitters[1], &mPos, 1, 2, true);
}

void cls_804A4BB0::fn_801D7DB4(f32 scale) {
    field_0x54.set(scale, scale, scale);
    mBox.set(-3.0f * scale, -3.0f * scale, 3.0f * scale, 3.0f * scale);
    mBox2.set(-6.0f * scale, -6.0f * scale, 6.0f * scale, 6.0f * scale);
    if (mEmitters[0]) {
        mEmitters[0]->setGlobalScale(field_0x54);
    }
    if (mEmitters[1]) {
        mEmitters[1]->setGlobalScale(field_0x54);
    }
}

void cls_804A4BB0::init() {
    EnemyBase_804A4808::init();
    mBox.set(1.0f, 1.0f, -1.0f, -1.0f);
    mBox2.set(mBox);
    field_0x80.set(1.0f, 1.0f, -1.0f, -1.0f);
    field_0xb0 = 4;
    field_0x98 = 2;
    mFlags |= 0x300;
    field_0x114 = 2;
    field_0x194 = 1;
    if (fn_801CD2F8(field_0x4)) {
        mAnim.fn_801D2608(lbl_8053A140, 1);
        mAnim.startAnim_801D22DC(0, 2, false);
    } else {
        mEmitters[0] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x226, 1, 2, NULL, NULL, true, 1.0f);
        mEmitters[1] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x227, 1, 2, NULL, NULL, true, 1.0f);
    }
    if (fn_801CD2F8(field_0x4)) {
        fn_801D7DB4(1.0f);
    } else {
        fn_801D7DB4(0.0f);
    }
}

cls_804A4BB0::~cls_804A4BB0() {
    fn_801EE434(mEmitters[0]);
    fn_801EE434(mEmitters[1]);
}

cls_804A4BB0::cls_804A4BB0() {
    mEmitters[0] = NULL;
    mEmitters[1] = NULL;
}
