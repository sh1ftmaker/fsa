#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/BgRewrite.h"
#include "game/Effect.h"
#include "game/EmitterDelete.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/act/tu_801BEDBC.h"

static inline bool contains(const JGeometry::TBox2f& b, const JGeometry::TVec3f& p) {
    return p.x >= b.i.x && p.y >= b.i.y && p.x < b.f.x && p.y < b.f.y;
}

// The two burst effects (0x1E, 0x1F) and their sound.
inline void cls_804A4188::spawnBurst() {
    fn_8013CC50(field_0x4, &mPos, 0x1E, 0, -1, NULL, NULL, true, 1.0f);
    fn_8013CC50(field_0x4, &mPos, 0x1F, 0, 0, NULL, NULL, true, 1.0f);
    fn_801F0E34(0x30061, 0);
    mFlags |= FLAG_BURST;
}

void cls_804A4188::fn_801BEDBC() {
    if (mAlive && !(mFlags & FLAG_BURST)) {
        spawnBurst();
    }
}

// The effect follows the actor; its angle is the direction of field_0x3c.
inline void cls_804A4188::moveEmitter() {
    if (mEmitter2a8 != NULL) {
        s32 depth = 1;
        if (mStateTimer < 2) {
            depth = field_0x94;
        }
        fn_8013CB44(mEmitter2a8, &mPos, 0, depth, true);
        s16 angle = JMath::atanTable_.atan2_(field_0x3c.x, field_0x3c.y) *
                    (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180());
        fn_8013CAA4(mEmitter2a8, 0.017453294f * (0.005493164f * angle));
    }
}

// Whether the point (dx, dy) from the actor is blocked.
inline bool cls_804A4188::blockedAt(f32 dx, f32 dy) {
    JGeometry::TVec3f p(mPos.x + dx, mPos.y + dy, mPos.z);
    return vfunc_0x4C(field_0x4, &p);
}

// Burns the tiles under the box (a 2x2 grid of points over it).
inline void cls_804A4188::burnTiles() {
    JGeometry::TVec3f p(mPos);
    p.x = mPos.x + field_0x80.i.x;
    for (s32 i = 0; i < 2; i++) {
        p.y = mPos.y + field_0x80.i.y;
        for (s32 j = 0; j < 2; j++) {
            if (fn_801CCC78(&p, field_0x4)) {
                lbl_8053AC98->fn_802A20F0(&p, -1, field_0x4, false, false);
            }
            p.y += field_0x80.f.y - field_0x80.i.y;
        }
        p.x += field_0x80.f.x - field_0x80.i.x;
    }
}

// FIXME: 91.0 %. Same instructions as ActorBase_804A5770::vfunc_0x4C (tu_801EE24C); only the saved
// registers differ: the target keeps this in r31, the result in r30, layer in r29 and pos in r28
// (this code: result r31, this r30, pos r29, layer r28), so the prologue stores and the argument
// moves are ordered differently.
bool cls_804A4188::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    switch (fn_8022461C(pos, layer)) {
    case 3:
    case 0xD:
    case 0xE:
        return false;
    }
    bool blocked = true;
    if (!fn_802265DC(pos, layer, false)) {
        s32 serial = field_0x90;
        CollisionBoxes_8020D58C* boxes = fn_8020D58C();
        if (!boxes->mLists[boxes->field_0x1].fn_8020D6AC(layer, pos, serial, NULL)) {
            blocked = false;
        }
    }
    return blocked;
}

// FIXME: 98.3 %. Scheduling only: the target loads mFlags again for the ENEMY_FLAG_STARTED test
// (lwz r0; this code reuses the first load in r3), keeps layer in r29 (this code r30), mPos.x of
// the view test in f2 (f3 here) and loads the probe operands in the order y, dy, x, dx.
void cls_804A4188::update() {
    if (mFlags & FLAG_BURST) {
        fn_801EE434(mEmitter2a8);
        mEmitter2a8 = NULL;
        mAlive = false;
        return;
    }
    mStateTimer++;
    if (mStateTimer < 0) {
        mStateTimer = 0;
    }
    if (!(mFlags & ENEMY_FLAG_STARTED)) {
        mFlags |= ENEMY_FLAG_STARTED;
        vfunc_0x54();
        return;
    }
    mPos += field_0x3c;
    moveEmitter();
    s32 layer = field_0x4;
    GameView_804A9950* view = fn_802D6A9C();
    const JGeometry::TBox2f& box = layer < 8 ? view->mVisibleBox : view->field_0xdc;
    if (!contains(box, mPos)) {
        mFlags |= FLAG_BURST;
        return;
    }
    if (blockedAt(field_0x80.i.x, field_0x80.i.y) || blockedAt(field_0x80.f.x, field_0x80.i.y) ||
        blockedAt(field_0x80.i.x, field_0x80.f.y) || blockedAt(field_0x80.f.x, field_0x80.f.y)) {
        burnTiles();
        spawnBurst();
    } else {
        fn_801F2B7C(field_0x114, -1, 1);
    }
}

void cls_804A4188::vfunc_0x54() {
    mEmitter2a8 = fn_8013CC50(field_0x4, &mPos, 0x1B, 0, field_0x94, NULL, NULL, true, 1.0f);
    moveEmitter();
}

void cls_804A4188::init() {
    EnemyBase_804A4808::init();
    mBox.set(-2.0f, -2.0f, 2.0f, 2.0f);
    mBox2.set(1.0f, 1.0f, -1.0f, -1.0f);
    field_0x80.set(-1.0f, -1.0f, 1.0f, 1.0f);
    field_0xb0 = 0;
    field_0x98 = 2;
    mFlags |= ENEMY_FLAG_NO_HURT | ENEMY_FLAG_KEEP_LIFE;
    mLife = 1000;
    mLifeBackup = mLife;
    field_0x194 = 1;
    field_0x114 = 2;
    mStateTimer = 0;
}

cls_804A4188::~cls_804A4188() {
    fn_801EE434(mEmitter2a8);
}

cls_804A4188::cls_804A4188() : mEmitter2a8(NULL) {}
