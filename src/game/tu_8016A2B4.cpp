#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Effect.h"
#include "game/GameMode.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "JSystem/J3D/J3DModel.h"

void GameRootSub24_8016A3AC::fn_8016A2B4() {
    if (field_0x124 != NULL) {
        JPAEmitterManager* manager = fn_8013C824()->mEmitterManager;
        manager->forceDeleteEmitter(field_0x124);
    }
    if (field_0x128 != NULL) {
        JPAEmitterManager* manager = fn_8013C824()->mEmitterManager;
        manager->forceDeleteEmitter(field_0x128);
    }
    if (field_0x12c != NULL) {
        JPAEmitterManager* manager = fn_8013C824()->mEmitterManager;
        manager->forceDeleteEmitter(field_0x12c);
    }
}

void GameRootSub24_8016A3AC::fn_8016A330() {
    field_0x1ec = 0;
    field_0x1ed = 0;
    field_0x1ee = 0;
    field_0x1ef = 0;
    field_0x1f0 = 0;
    field_0x1f1 = 0;
    field_0x1f2 = 0;
    field_0x1f3 = 0;
    field_0x1f4 = 0;
    field_0x1f5 = 0;
    field_0x1c4 = 0;
    field_0x1c5 = 0;
    field_0x1c6 = 0;
    field_0x1c7 = 0;
    field_0x1c8 = 0;
    field_0x1c9 = 0;
    field_0x1ca = 0;
    field_0x1cb[0] = 0;
    field_0x1cb[1] = 0;
    field_0x1cb[2] = 0;
    field_0x1cb[3] = 0;
    field_0x1cf = 0;
    field_0x1d1 = 0;
    field_0x1d2 = 0;
    field_0x1d3 = 0;
    field_0x1d8 = -1;
    field_0x1dc = 0;
    field_0x1dd = 0;
}

// Returns lbl_8053AAF8->field_0x4->field_0x24 (the object act_environment_control_2
// writes flags into).
GameRootSub24_8016A3AC* fn_8016A3AC() {
    return lbl_8053AAF8->field_0x4->field_0x24;
}

// FIXME: 98.3. regalloc only: the original keeps the camera in r4 and the 0x4330
// conversion constant in r3 (ours swaps them) and the float temporaries in f3/f5.
void GameRootSub24_8016A3AC::fn_8016A7DC(J3DModel* model, const JGeometry::TVec3f& pos, s32 dir, bool followX, bool followY, f32 parallax, f32 scale) {
    if (dir == 1) {
        bool skip = lbl_8053AAF8->field_0x4c == 11 ? true : lbl_8053AAF8->field_0x88 == 2;
        if (skip) {
            return;
        }
    }
    Mtx mtx;
    JGeometry::TVec3f off;
    f32 z = 1.5f * (f32)(dir << 10);
    Camera_804AA9E4* camera = lbl_8053AB10->field_0x24;
    if (followX) {
        f32 c = -camera->field_0x4[0][3];
        f32 d = c * parallax;
        off.x = (c - d) + (f32)((s32)(0.001953125f * d) << 9);
    } else {
        off.x = 0.0f;
    }
    if (followY) {
        f32 c = -camera->field_0x4[1][3];
        f32 d = c * parallax;
        off.y = (c - d) + (f32)((s32)(0.001953125f * d) << 9);
    } else {
        off.y = 0.0f;
    }
    off.y += z;
    off.z = z;
    off += pos;
    mtx[0][0] = 1.0f;
    mtx[0][1] = 0.0f;
    mtx[0][2] = 0.0f;
    mtx[0][3] = off.x;
    mtx[1][0] = 0.0f;
    mtx[1][1] = 0.0f;
    mtx[1][2] = 1.0f;
    mtx[1][3] = off.y;
    mtx[2][0] = 0.0f;
    mtx[2][1] = 1.0f;
    mtx[2][2] = 0.0f;
    mtx[2][3] = off.z;
    model->setBaseScale(scale, 1.0f, scale);
    PSMTXCopy(mtx, model->mPosMtx);
    model->calc();
    model->entry();
    model->viewCalc();
}

void GameRootSub24_8016A3AC::fn_8016A9EC() {
    if (field_0x1dd) {
        JGeometry::TVec3f pos(256.0f, 256.0f, 0.0f);
        fn_8016A7DC(field_0xec, pos, -1, true, true, 0.5f, 10.0f);
    }
}

void GameRootSub24_8016A3AC::fn_8016AA48() {
    if (field_0x1f5) {
        JGeometry::TVec3f pos(256.0f, 256.0f, 0.0f);
        fn_8016A7DC(field_0xe0, pos, 1, true, true, 1.5f, 10.24f);
    }
}

void GameRootSub24_8016A3AC::fn_8016AAA4() {
    if (field_0x1dc) {
        JGeometry::TVec3f pos(256.0f, 256.0f, 0.0f);
        fn_8016A7DC(field_0xcc, pos, 1, true, true, 1.5f, 1.0f);
    }
}

void GameRootSub24_8016A3AC::fn_8016AB00() {
    if (field_0x1f4) {
        JGeometry::TVec3f pos(256.0f, 256.0f, 0.0f);
        fn_8016A7DC(field_0xd4, pos, 1, true, true, 1.0f, 10.0f);
    }
}

void GameRootSub24_8016A3AC::fn_8016AB5C() {
    if (field_0x1d8 > 0) {
        JGeometry::TVec3f pos(256.0f, 256.0f, 0.0f);
        fn_8016A7DC(field_0xc0, pos, 1, true, true, 1.0f, 10.0f);
    }
}

void GameRootSub24_8016A3AC::fn_8016ABB8() {
    if (field_0x1f3) {
        JGeometry::TVec3f pos(256.0f, 256.0f, 0.0f);
        fn_8016A7DC(field_0xb4, pos, 1, true, true, 1.0f, 10.0f);
    }
}

void GameRootSub24_8016A3AC::fn_8016AC14() {
    if (field_0x1f2) {
        JGeometry::TVec3f pos(256.0f, 256.0f, 0.0f);
        fn_8016A7DC(field_0x9c, pos, 1, true, true, 1.0f, 10.0f);
    }
}

void GameRootSub24_8016A3AC::fn_8016AC70() {
    if (field_0x1f1) {
        JGeometry::TVec3f pos(256.0f, 256.0f, 0.0f);
        fn_8016A7DC(field_0x90, pos, 1, true, true, 1.5f, 10.0f);
    }
}

void GameRootSub24_8016A3AC::fn_8016ACCC() {
    if (field_0x1f0) {
        JGeometry::TVec3f pos(256.0f, 256.0f, 0.0f);
        fn_8016A7DC(field_0x84, pos, 1, true, true, 1.0f, 10.0f);
    }
}

void GameRootSub24_8016A3AC::fn_8016AD28() {
    if (field_0x1ef) {
        JGeometry::TVec3f pos(256.0f, 256.0f, 0.0f);
        fn_8016A7DC(field_0x28, pos, 1, true, true, 1.0f, 10.0f);
    }
}

void GameRootSub24_8016A3AC::fn_8016AD84() {
    if (field_0x1ed) {
        JGeometry::TVec3f pos(256.0f, 256.0f, 0.0f);
        fn_8016A7DC(field_0x1c, pos, 1, true, true, 1.5f, 10.24f);
    }
}

void GameRootSub24_8016A3AC::fn_8016B054() {
    if (field_0x1d3) {
        JGeometry::TVec3f pos(256.0f, 256.0f, 0.0f);
        fn_8016A7DC(field_0xa8, pos, -1, true, true, 1.0f, 10.0f);
    }
}

void GameRootSub24_8016A3AC::fn_8016B0B0() {
    if (field_0x1cf) {
        Camera_804AA9E4* camera = lbl_8053AB10->field_0x24;
        f32 scale = 6.0f / camera->field_0x858;
        for (s32 i = 0; i < 6; i++) {
            if (field_0x3c[i] == NULL) {
                continue;
            }
            if (i == 3 && field_0x1d0 == 0) {
                continue;
            }
            if ((i == 1 || i == 2) && field_0x1d0 != 0) {
                continue;
            }
            Mtx mtx;
            f32 x = -camera->field_0x4[0][3];
            f32 z = -545.0f - 96.0f * (f32)i;
            f32 y = 32.0f + (z - camera->field_0x4[1][3]);
            mtx[0][0] = 1.0f;
            mtx[0][1] = 0.0f;
            mtx[0][2] = 0.0f;
            mtx[0][3] = x;
            mtx[1][0] = 0.0f;
            mtx[1][1] = 0.0f;
            mtx[1][2] = 1.0f;
            mtx[1][3] = y;
            mtx[2][0] = 0.0f;
            mtx[2][1] = 1.0f;
            mtx[2][2] = 0.0f;
            mtx[2][3] = z;
            field_0x3c[i]->setBaseScale(scale, scale, scale);
            PSMTXCopy(mtx, field_0x3c[i]->mPosMtx);
            field_0x3c[i]->calc();
            field_0x3c[i]->entry();
            field_0x3c[i]->viewCalc();
        }
    }
}

void GameRootSub24_8016A3AC::fn_8016B8AC() {
    if (field_0x1cb[0]) {
        JGeometry::TVec3f pos(256.0f, 256.0f + field_0x1e4, 0.0f);
        fn_8016A7DC(field_0x34, pos, 1, true, false, 1.0f, 102.4f);
    }
}

void GameRootSub24_8016A3AC::fn_8016D284() {
    if (field_0x1c6) {
        JGeometry::TVec3f pos(256.0f, 256.0f, 0.0f);
        fn_8016A7DC(field_0x14, pos, -1, true, true, 0.5f, 10.24f);
    }
}

void GameRootSub24_8016A3AC::fn_8016D2E0() {
    if (field_0x1c5) {
        JGeometry::TVec3f pos(256.0f, 256.0f, 0.0f);
        fn_8016A7DC(field_0xc, pos, -1, true, true, 0.5f, 10.24f);
    }
}

void GameRootSub24_8016A3AC::fn_8016D33C() {
    if (field_0x1c4) {
        JGeometry::TVec3f pos(256.0f, 256.0f, 0.0f);
        fn_8016A7DC(field_0x4, pos, -1, true, true, 0.5f, 10.24f);
    }
}

// FIXME: 75.2 %. The original calls fn_8016ADE0 and fn_8016AD84 (bl); here MWCC inlines
// fn_8016AD84 (defined earlier in the unit), so the call is a copy of its body.
// Defining fn_8016AD84 after this function gives 100 %, but breaks address
// order, so the original's call likely survives for a reason not yet known.
void GameRootSub24_8016A3AC::fn_8016DA6C() {
    if (isGameModeSet()) {
        fn_8016ADE0();
        fn_8016AD84();
    }
}

void GameRootSub24_8016A3AC::fn_8016ED88() {
    field_0x0 = NULL;
    field_0x14 = NULL;
    field_0x1c = NULL;
    field_0x30 = NULL;
    field_0x38 = NULL;
    field_0x3c[4] = NULL;
    field_0x54 = NULL;
    field_0x68 = NULL;
}

void GameRootSub24_8016A3AC::fn_8016EDB0() {
    fn_8016A330();
    field_0x130 = 0;
    field_0x134 = 0;
    field_0x124 = NULL;
    field_0x128 = NULL;
    field_0x12c = NULL;
    field_0x13c = 0;
    field_0x138 = 0;
    field_0x1b0 = 0x78;
    field_0x1b8 = 0x3c;
    field_0x1b4 = 0;
    field_0x140 = 0;
    field_0x154 = 0;
    field_0x15c = 0;
    field_0x170 = 0;
    field_0x178 = 0;
    field_0x18c = 0;
    field_0x194 = 0;
    field_0x1a8 = 0;
    field_0x1bc = 0;
    field_0x1c0 = 0;
}

void fn_8016FDE0() {}
