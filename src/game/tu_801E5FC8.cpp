#include "game/GameAudio.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/act/tu_801D8400.h"
#include "game/act/tu_801DAC6C.h"

void cls_804A4C58::fn_801E63D8() {
    mEmitter2c0 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x25e, 0, 0, NULL, NULL, true, 1.0f);
    mEmitter2c4 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x25f, 0, 0, NULL, NULL, true, 1.0f);
}

void cls_804A4C58::fn_801E6394() {
    releaseEmitter_801EE434(mEmitter2c0);
    releaseEmitter_801EE434(mEmitter2c4);
    mEmitter2c0 = NULL;
    mEmitter2c4 = NULL;
}

void cls_804A4C58::fn_801E631C() const {
    spawnEmitter_8013CC50(field_0x4, &mPos, 0x25a, 0, 0, NULL, NULL, true, 1.0f);
    spawnEmitter_8013CC50(field_0x4, &mPos, 0x25b, 0, 0, NULL, NULL, true, 1.0f);
}

void cls_804A4C58::fn_801E62D4() const {
    spawnEmitter_8013CC50(field_0x4, &mPos, 0x259, 0, 0, NULL, NULL, true, 1.0f);
}

void cls_804A4C58::fn_801E6234() {
    JGeometry::TVec3f pos(mPos.x, -40.0f + mPos.y, mPos.z);
    mEmitter2c8 = spawnEmitter_8013CC50(field_0x4, &pos, 0x25c, 0, 0, NULL, NULL, true, 1.0f);
    mEmitter2cc = spawnEmitter_8013CC50(field_0x4, &pos, 0x25d, 0, 0, NULL, NULL, true, 1.0f);
}

void cls_804A4C58::fn_801E61C4() {
    JGeometry::TVec3f pos(mPos.x, -40.0f + mPos.y, mPos.z);
    fn_801CB71C(&mEmitter2c8, &pos, 0, 0);
    fn_801CB71C(&mEmitter2cc, &pos, 0, 0);
}

// FIXME: 98.39%. Only register allocation of the three colour adds differs: the
// target loads byte 0x4c into r7 and adds in place (add r7, r7, r3); this
// loads it into r0 and adds into r7. (Declaring mtx before params changes the stack.)
void cls_804A4C58::draw() {
    void* res = fn_804032E8('ICEB', mSeq2a8.fn_801DA938());
    if (res != NULL) {
        SpriteParams_801F5AE0 params;
        Mtx mtx;
        if ((mFlags & 0x2000) != 0 && mSeq2a8.fn_801DA904()) {
            mtx[0][3] = 0.0f;
            mtx[1][3] = 0.0f;
            mtx[2][3] = 0.0f;
            mtx[0][0] = -1.0f;
            mtx[0][1] = 0.0f;
            mtx[0][2] = 0.0f;
            mtx[1][0] = 0.0f;
            mtx[1][1] = 1.0f;
            mtx[1][2] = 0.0f;
            mtx[2][0] = 0.0f;
            mtx[2][1] = 0.0f;
            mtx[2][2] = 1.0f;
            params.mMtx = mtx;
        }
        s32 d = fn_801DAC6C()->fn_801DAD2C();
        params.field_0x14.r += d;
        params.field_0x14.g += d;
        params.field_0x14.b += d;
        fn_801EFD34(res, &params, -1, 0.0f, mState < 8 ? 0.0f : -40.0f, 0.0f);
    }
}
