#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/act/act_vaati_boss.h"

// Auto-inlining is off for the whole file (docs/INLINING.md, pragma rule 2): the
// original inlines no same-unit plain function. Checked: built with inlining off,
// every written function's calls to this unit's functions equal the original's
// `bl`s. The callers that need it: draw calls fn_801EDC0C out of line.
#pragma auto_inline off

static const char* lbl_804A5760[] = {"e_gufu_1.bti", "e_gufu_2.bti", "e_gufu_3.bti",
                                     "e_gufu_4.bti"};

// FIXME: 32.8%. The target picks max(1.0f + field_0x52c.field_0x28, 0.001f) with one fsel
// (0.001f - x >= 0); ours compiles the ?: as fcmpo / cror / branch (the same
// gap as EnemyBase_804A4808::fn_801CE010; tried a - b >= 0 ? a : b, a > b ? a : b,
// a local and an inline). The rest of the body (stack layout, the stores, the
// two calls) is the target's up to scheduling: the target also keeps the
// twelve scaled matrix entries in f4-f13/f30/f31 and stores them after
// all the loads, ours interleaves loads and stores; the .sdata2 literal order
// follows the fsel.
void VaatiBoss_804A4958::fn_801EDD0C() {
    void* res = fn_804032E8('TIMG', lbl_804A5760[mAnim.mCel]);
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    params.field_0x1c = 1;
    params.field_0x1d = 1;
    params.field_0x1f = 0;
    s32 flags = 7;
    if (field_0x5d1 != 0) {
        flags = 0x407;
        params.field_0x10.a = 0;
    }
    f32 scale = 1.0f + field_0x52c.field_0x28;
    scale = 0.001f - scale >= 0.0f ? 0.001f : scale;
    mtx[0][0] = 1.0f;
    mtx[0][1] = 0.0f;
    mtx[0][2] = 0.0f;
    mtx[0][3] = 0.0f;
    mtx[1][0] = 0.0f;
    mtx[1][1] = 1.0f;
    mtx[1][2] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][0] = 0.0f;
    mtx[2][1] = 0.0f;
    mtx[2][2] = scale;
    mtx[2][3] = 0.0f;
    params.mMtx = mtx;
    if (mState == 5) {
        if (mSpriteParams.mMtx == NULL) {
            fn_801EF830(field_0x4, &mPos, &params, flags, NULL, 0, res);
        }
        f32 k = 1.0f / fn_802F5FF4(lbl_8053F0B8);
        k = fn_802F5FF4(mPos.z) * k;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 4; j++) {
                mtx[i][j] *= k;
            }
        }
        params.mMtx = mtx;
        fn_801EFD34(res, &params, -1, 0.0f, -12.0f * lbl_8053F0BC, 0.0f);
    } else {
        fn_801EFD34(res, &params, flags, 0.0f, -12.0f * lbl_8053F0BC, 0.0f);
    }
}

void VaatiBoss_804A4958::fn_801EDC0C(s32 frame) {
    JGeometry::TVec3f offset;
    field_0x52c.fn_801D6CEC(&offset);
    mPos += offset;
    if (mState == 0xb && mStateTimer >= 0x3c) {
        if (mStateTimer % 4 < 2) {
            mPos.x += 1.0f;
        } else {
            mPos.x -= 1.0f;
        }
    }
    if (fn_801CD2F8(field_0x4)) {
        fn_801F02BC("enemy_both_sob.bin", frame, NULL, -1, 0, false, 1.0f, 1.0f);
    } else {
        fn_801EDD0C();
    }
}

void VaatiBoss_804A4958::draw() {
    if (mState != 0x17) {
        JGeometry::TVec3f savedPos = mPos;
        fn_801EDC0C(mAnim.mCel + 0x10c);
        mPos.set(savedPos.x, savedPos.y, savedPos.z);
    }
}
