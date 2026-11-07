#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/cls_8049D83C.h"

static const s32 lbl_80464830[15] = {
    0x199, 0x1E7, 0x17F, 0x201, 0x1CD, 0x1B3, 0x21B, 0x21B,
    0x14C, 0x173, 0x20E, 0x21A, 0x29B, 0x2B1, 0x2CD,
};

// 0x80114C1C.
void cls_8049D83C::draw() {
    cls_8049D140::draw();
    fn_80113790();
    switch (field_0x23c) {
    case 0x10:
        fn_80113B54(0x1D6);
        break;
    default:
        if (field_0x232 != 0) {
            fn_80113B54(lbl_80464830[field_0x238]);
        }
        break;
    }
}

// 0x80113E98.
s32 cls_8049D83C::vfunc_0x5C() {
    switch (field_0x23c) {
    case 0xA:
    case 0x11:
        return 1;
    default:
        return 0;
    }
}

// Not auto-inlined: draw calls fn_80113790 out of line in the original (docs/INLINING.md, pragma rule 1).
#pragma auto_inline off
// Darkens the screen: draws the image field_0x260 scaled by 96 with an alpha of field_0x248.
// 0x80113790.
// FIXME(shotgun): 95.13 %, different + reloc-name
void cls_8049D83C::fn_80113790() {
    if (field_0x248 > 0) {
        SpriteParams_801F5AE0 params;
        Mtx mtx;
        PSMTXTrans(mtx, 0.0f, 0.0f, 0.0f);
        mtx[0][0] = 96.0f;
        mtx[0][1] = 0.0f;
        mtx[0][2] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = 1.0f;
        mtx[1][2] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = 96.0f;
        params.field_0x10.r = 0;
        params.field_0x10.g = 0;
        params.field_0x10.b = 0;
        params.field_0x10.a = field_0x248;
        params.mMtx = mtx;
        params.field_0x1d = -1;
        params.field_0x2d = 1;
        JGeometry::TVec3f pos = mPos;
        pos.z = 0.0f;
        if (field_0x4 < 8) {
            gGameRoot2_8053AB10->field_0x20->fn_802F745C(&pos, field_0x260, &params);
        }
    }
}
#pragma auto_inline reset

// 0x80113788.
s32 cls_8049D83C::vfunc_0x58() {
    return field_0x23c;
}
