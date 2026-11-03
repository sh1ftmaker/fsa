#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/RotMatrix.h"
#include "game/act/tu_801A908C.h"

void cls_804A2FB0::draw() {
    switch (field_0x258) {
    case 0:
    case 5:
        fn_801A90D4();
        break;
    case 1:
    case 2:
    case 3:
    case 4:
        fn_801A9374();
        break;
    }
}

// FIXME: 99.2 %. regalloc-only: the original keeps the angle (and then
// cos y) in f30 and cos x in f31, ours the other way round; cos z and the
// sx * sy product swap f3 / f2.
void cls_804A2FB0::fn_801A90D4() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    params.field_0x1d = 1;
    JGeometry::TVec3f rot(0.0f, field_0x2b4 / -10430.228f, 0.0f);
    setRotScaleMtx(mtx, rot, 1.0f);
    params.mMtx = mtx;
    void* res = lbl_8053AAF8->field_0xa4->getResource('TIMG', lbl_804A2F2C[32]);
    fn_801EFD34(res, &params, -1, 0.0f, 0.0f, 0.0f);
}
