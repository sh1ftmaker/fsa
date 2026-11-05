// Actor EXPL ("Explosion" in docs/names/actors.tsv): the spawner
// (spawnActor_801F9484) allocates 0x23C bytes for it (0x801FB3B0) and calls its
// constructor fn_80203C68, which stores the vtable 0x804A7FB8.

#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/act/act_explosion.h"
#include "game/Effect.h"
#include "game/GameView.h"
#include "JSystem/JParticle/JPAEmitter.h"

void Explosion_804A7FB8::draw() {}

// States in field_0x230: count down, wait for vfunc_0x34(), spawn effect
// 0x173 offset by (16, 24), then for 20 frames widen the view's entry
// field_0x4 (fn_802D79E8) and hit players, then remove itself.
void Explosion_804A7FB8::update() {
    GameView_804A9950* view = getGameView_802D6A9C(); // read only by the missing code below
    switch (field_0x230) {
    case 0:
        field_0x234--;
        if (field_0x234 == 0) {
            field_0x230 = 1;
            // FIXME: 92.4 %. Here the original, when (field_0x90 & 0xFF) == 1,
            // converts mPos.y - view->mVisibleBox.i.y to an int (fctiwz, stfd
            // into a 0x30-byte frame) and never reads it; whatever used the
            // value is gone, and an unused local compiles to nothing, so the
            // test, the conversion and 0x10 bytes of frame are missing.
        }
        break;
    case 1:
        if (vfunc_0x34()) {
            field_0x230 = 2;
            field_0x234 = 1;
        }
        break;
    case 2:
        if (field_0x234 == 0) {
            if (mEmitter238 == NULL) {
                JGeometry::TVec3f pos(16.0f + mPos.x, 24.0f + mPos.y, mPos.z);
                mEmitter238 = spawnEmitter_8013CC50(field_0x4, &pos, 0x173, 1, 0, NULL, NULL, true, 1.0f);
                field_0x230 = 3;
                mPos.x += 16.0f;
                mPos.y += 24.0f;
                startSound_801F0E34(0x30001, 0);
                field_0x234 = 20;
            }
        } else {
            field_0x234--;
        }
        break;
    case 3:
        getGameView_802D6A9C()->fn_802D79E8(field_0x4, 4.0f, 0.0f, false);
        fn_801F2B7C(field_0x114, -1, 6);
        field_0x234--;
        if (field_0x234 == 0) {
            field_0x230 = 4;
        }
        break;
    case 4:
    default:
        if (mEmitter238 != NULL) {
            mEmitter238->becomeInvalidEmitter();
            mEmitter238 = NULL;
        }
        fn_801F3668();
        break;
    }
}

Explosion_804A7FB8::~Explosion_804A7FB8() {
    if (mEmitter238 != NULL) {
        releaseEmitter_801EE434(mEmitter238);
    }
}

void Explosion_804A7FB8::init() {
    mBox.set(-48.0f, -48.0f, 48.0f, 48.0f);
    mEmitter238 = NULL;
    field_0x230 = 0;
    field_0x234 = 20;
}
