#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/cls_8049D83C.h"
#include "game/GameRootSub20.h"
#include "game/RotMatrix.h"
#include "game/SpriteParams.h"

static const s32 lbl_80464830[15] = {
    0x199, 0x1E7, 0x17F, 0x201, 0x1CD, 0x1B3, 0x21B, 0x21B,
    0x14C, 0x173, 0x20E, 0x21A, 0x29B, 0x2B1, 0x2CD,
};

// Frame tables of fn_8011395C (indexed by field_0x240): where the counter restarts, where the
// stage ends, and the crystal slot of each field_0x238.
static const s32 lbl_804648B4[3] = {0, 0xC9, 0x191};
static const s32 lbl_804648C0[3] = {0xC8, 0x190, 0x258};
static const s32 lbl_804648CC[7] = {1, 4, 0, 5, 3, 2, 6};

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
// FIXME(shotgun): 74.0 %, missing/extra + reloc-name
void cls_8049D83C::fn_80113B54(s32 arg) {
    SpriteParams_801F5AE0 params;
    SpriteParams_801F5AE0 params2;
    JGeometry::TVec3f pos(mPos.x, mSpawnPos.y, mPos.z);
    s32 frame = arg + field_0x274;
    const char* name;
    if (arg == 0x1D6) {
        name = "enemy_both2_sob.bin";
    } else {
        name = "enemy_gc_sob.bin";
    }
    fn_801EF830(field_0x4, &pos, &params2, 7, name, frame, NULL);
    if (field_0x4 < 8) {
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', name);
        gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, &params, 1.0f, 1.0f);
    }
}

// FIXME(shotgun): 84.8 %, missing/extra + reloc-name
void cls_8049D83C::fn_8011395C() {
    Mtx mtx;
    PSMTXTrans(mtx, mPos.x, mPos.y, mPos.z);
    JGeometry::TVec3f axisX(1.0f, 0.0f, 0.0f);
    fn_8010F668(mtx, &axisX, 0.08726646f);
    Mtx rot;
    for (s32 i = 0; i < 3; i++) {
        for (s32 j = 0; j < 4; j++) {
            rot[i][j] = 0.0f;
        }
    }
    JGeometry::TVec3f axisZ(0.0f, 0.0f, 1.0f);
    fn_8010F668(rot, &axisZ, field_0x258);
    PSMTXConcat(mtx, rot, mtx);
    if (field_0x234 != 0) {
        field_0x24c++;
    }
    switch (field_0x240) {
    case 0:
    case 2:
        if (field_0x24c >= lbl_804648C0[field_0x240]) {
            field_0x24c = lbl_804648B4[field_0x240];
        }
        break;
    case 1:
        if (field_0x24c >= lbl_804648C0[field_0x240]) {
            field_0x240 = 2;
        }
        break;
    }
    s32 frame = field_0x24c;
    GameRootSub20_801902C0::Crystal& crystal = fn_801902C0()->mCrystals[lbl_804648CC[field_0x238]];
    if (crystal.mModel != NULL && crystal.mActive == 0) {
        crystal.mActive = 1;
        crystal.mPos.x = 0.5f;
        crystal.mPos.y = 0.5f;
        crystal.mPos.z = 0.5f;
        PSMTXCopy(mtx, crystal.mMtx);
        crystal.field_0x48 = -1.0f;
        crystal.field_0x4c = frame;
        crystal.field_0x54 = 0;
        crystal.field_0x58 = 2;
    }
}

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
