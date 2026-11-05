#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/cls_8016ED88.h"
#include "game/Effect.h"
#include "game/GameMode.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "JSystem/J3D/J3DModel.h"
#include "JSystem/JUtility/JUTTexture.h"
#include "JSystem/JUtility/JUTVideo.h"
#include "game/Map.h"
#include "game/Camera.h"
#include "dolphin/gx.h"
#include "JSystem/J3D/J3DModelLoader.h"
#include "JSystem/J3D/J3DAnmLoader.h"
#include "JSystem/J3D/J3DAnmTextureSRTKey.h"
#include "JSystem/J3D/J3DAnmTevRegKey.h"
#include "JSystem/J3D/J3DMaterialAnm.h"
#include "JSystem/J3D/J3DSys.h"
#include "JSystem/JKernel/JKRArchive.h"

// The colours of the backdrop GX setups (the user of each in the comment;
// none of the three users is written yet). File-scope objects: they come
// before the unit's literal pool in .sdata2, in declaration order (a function's own constants are pooled in
// first-use order), and equal colours are separate objects.
static const GXColor lbl_8053D910 = {0x00, 0x50, 0x78, 0x00}; // fn_8016CB14
static const GXColor lbl_8053D914 = {0x38, 0x60, 0xA8, 0xFF}; // fn_8016CB14
static const GXColor lbl_8053D918 = {0x00, 0x00, 0x00, 0x2F}; // fn_8016C2FC
static const GXColor lbl_8053D91C = {0xAF, 0xCF, 0xCF, 0xFF}; // fn_8016C2FC
static const GXColor lbl_8053D920 = {0xCF, 0xE7, 0xE7, 0xFF}; // fn_8016C2FC
static const GXColor lbl_8053D924 = {0x00, 0x00, 0x00, 0x2F}; // fn_8016B910
static const GXColor lbl_8053D928 = {0x36, 0x61, 0x82, 0xFF}; // fn_8016B910
static const GXColor lbl_8053D92C = {0xAF, 0xCF, 0xCF, 0xFF}; // fn_8016B910
static const GXColor lbl_8053D930 = {0x36, 0x61, 0x82, 0xFF}; // fn_8016B910
static const GXColor lbl_8053D934 = {0xCF, 0xE7, 0xE7, 0xFF}; // fn_8016B910

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

// FIXME: 99.79. regalloc only: the original keeps heap in r29 and btk1Out in r30
// (ours swaps them).
void fn_8016A3BC(JKRArchive* archive, const char* bmd, const char* btk1, const char* btk2,
                 const char* brk1, const char* brk2, u32 flags, JKRHeap* heap, J3DModel** model,
                 J3DAnmTextureSRTKey** btk1Out, J3DAnmTextureSRTKey** btk2Out,
                 J3DAnmTevRegKey** brk1Out, J3DAnmTevRegKey** brk2Out) {
    void* res = NULL;
    if (archive != NULL) {
        res = archive->getResource('J3D ', bmd);
    }
    if (res != NULL) {
        J3DModelData* data = J3DModelLoaderDataBase::load(res, flags);
        if (btk1 != NULL) {
            *btk1Out = (J3DAnmTextureSRTKey*)J3DAnmLoaderDataBase::load(archive->getResource('J3D ', btk1), J3DLOADER_UNK_FLAG0);
            (*btk1Out)->searchUpdateMaterialID(data);
        } else if (btk1Out != NULL) {
            *btk1Out = NULL;
        }
        if (btk2 != NULL) {
            *btk2Out = (J3DAnmTextureSRTKey*)J3DAnmLoaderDataBase::load(archive->getResource('J3D ', btk2), J3DLOADER_UNK_FLAG0);
            (*btk2Out)->searchUpdateMaterialID(data);
        } else if (btk2Out != NULL) {
            *btk2Out = NULL;
        }
        if (brk1 != NULL) {
            *brk1Out = (J3DAnmTevRegKey*)J3DAnmLoaderDataBase::load(archive->getResource('J3D ', brk1), J3DLOADER_UNK_FLAG0);
            (*brk1Out)->searchUpdateMaterialID(data);
        } else if (brk1Out != NULL) {
            *brk1Out = NULL;
        }
        if (brk2 != NULL) {
            *brk2Out = (J3DAnmTevRegKey*)J3DAnmLoaderDataBase::load(archive->getResource('J3D ', brk2), J3DLOADER_UNK_FLAG0);
            (*brk2Out)->searchUpdateMaterialID(data);
        } else if (brk2Out != NULL) {
            *brk2Out = NULL;
        }
        for (u16 i = 0; i < data->getMaterialNum(); i++) {
            J3DMaterialAnm* anm = new (heap, 0) J3DMaterialAnm();
            data->getMaterialNodePointer(i)->change();
            data->getMaterialNodePointer(i)->setMaterialAnm(anm);
        }
        if (btk1 != NULL) {
            j3dSys.ErrorReport(data->getMaterialTable().entryTexMtxAnimator(*btk1Out));
        }
        if (btk2 != NULL) {
            j3dSys.ErrorReport(data->getMaterialTable().entryTexMtxAnimator(*btk2Out));
        }
        if (brk1 != NULL) {
            j3dSys.ErrorReport(data->getMaterialTable().entryTevRegAnimator(*brk1Out));
        }
        if (brk2 != NULL) {
            j3dSys.ErrorReport(data->getMaterialTable().entryTevRegAnimator(*brk2Out));
        }
        *model = new (heap, 0) J3DModel(data, 0, 1);
    } else {
        if (btk1Out != NULL) {
            *btk1Out = NULL;
        }
        if (btk2Out != NULL) {
            *btk2Out = NULL;
        }
        if (brk1Out != NULL) {
            *brk1Out = NULL;
        }
        if (brk2Out != NULL) {
            *brk2Out = NULL;
        }
        *model = NULL;
    }
}

// The visible part of the map in texture coordinates (centre and half size;
// a map cell is 8 units). Dead-stripped: fn_8016ADE0, fn_8016B248, fn_8016B4D4,
// fn_8016B910, fn_8016C2FC and fn_8016CB14 inline it. Its own copy created the
// unit's first pool literals (0.125, 1, 0.5, 0, then the int-to-float double),
// which come before fn_8016A7DC's 1.5 in the original .sdata2.
void getMapTexView(JGeometry::TVec3f& center, JGeometry::TVec3f& half) {
    Camera_804AA9E4* camera = lbl_8053AB10->field_0x24;
    cls_804AA858* map = lbl_8053AB10->field_0x20;
    f32 sx = 0.125f / (f32)map->field_0x2380;
    f32 sy = 0.125f / (f32)map->field_0x2384;
    f32 inv = 1.0f / camera->field_0x858;
    JUTVideo* video = JUTVideo::getManager();
    half.x = inv * (0.5f * (sx * (s32)video->getFbWidth()));
    half.y = inv * (0.5f * (sy * (s32)video->getEfbHeight()));
    half.z = 0.0f;
    center.x = -camera->field_0x4[0][3] * sx;
    center.y = -camera->field_0x4[1][3] * sy;
    center.z = 0.0f;
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

// FIXME: 99.97. The colour's stack slots are swapped: the original builds it at
// sp+0x8 and passes the copy at sp+0xC (ours the other way round).
// Draws field_0xf4 over the whole screen, scrolled with the map view and
// field_0x1c0, at alpha field_0x1ec.
void GameRootSub24_8016A3AC::fn_8016ADE0() {
    if (field_0x1ec) {
        f32 width = (s32)JUTVideo::getManager()->getFbWidth();
        f32 height = (s32)JUTVideo::getManager()->getEfbHeight();
        f32 scroll = 0.00048828125f * (f32)(field_0x1c0 & 0x7FF);
        JGeometry::TVec3f center;
        JGeometry::TVec3f half;
        getMapTexView(center, half);
        center.x -= scroll;
        center.y -= scroll;
        lbl_8053AB10->fn_8013B460();
        lbl_8053AB10->fn_8013B1E8();
        field_0xf4->load(GX_TEXMAP0);
        GXColor color = {0, 0, 0, field_0x1ec};
        GXSetTevColor(GX_TEVREG0, color);
        GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
        GXSetZCompLoc(GX_FALSE);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(0.0f, 0.0f, 0.0f);
        GXTexCoord2f32(center.x - half.x, center.y - half.y);
        GXPosition3f32(width, 0.0f, 0.0f);
        GXTexCoord2f32(center.x + half.x, center.y - half.y);
        GXPosition3f32(width, height, 0.0f);
        GXTexCoord2f32(center.x + half.x, center.y + half.y);
        GXPosition3f32(0.0f, height, 0.0f);
        GXTexCoord2f32(center.x - half.x, center.y + half.y);
        GXEnd();
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

// FIXME: 73.96. scheduling + regalloc only (same instructions): the original
// interleaves the loads of getMapTexView differently (camera before the
// second map count) and stores the zero z components earlier.
// Draws field_0x120 over the screen, its view shifted and halved in x and y.
void GameRootSub24_8016A3AC::fn_8016B248() {
    if (field_0x120 != NULL && field_0x1cb[3]) {
        f32 width = (s32)JUTVideo::getManager()->getFbWidth();
        f32 height = (s32)JUTVideo::getManager()->getEfbHeight();
        JGeometry::TVec3f center;
        JGeometry::TVec3f half;
        getMapTexView(center, half);
        center.x -= 1.5f;
        center.y -= 0.5f;
        center.mul(JGeometry::TVec3f(0.5f, 0.5f, 1.0f));
        center.x += 0.5f;
        center.y += 0.375f;
        half.mul(JGeometry::TVec3f(1.0f, 0.75f, 1.0f));
        lbl_8053AB10->fn_8013B460();
        lbl_8053AB10->fn_8013B1E8();
        field_0x120->load(GX_TEXMAP0);
        GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(0.0f, 0.0f, 0.0f);
        GXTexCoord2f32(center.x - half.x, center.y - half.y);
        GXPosition3f32(width, 0.0f, 0.0f);
        GXTexCoord2f32(center.x + half.x, center.y - half.y);
        GXPosition3f32(width, height, 0.0f);
        GXTexCoord2f32(center.x + half.x, center.y + half.y);
        GXPosition3f32(0.0f, height, 0.0f);
        GXTexCoord2f32(center.x - half.x, center.y + half.y);
        GXEnd();
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

void GameRootSub24_8016A3AC::fn_8016DCEC() {
    if (isGameModeSet() && field_0x1cf && lbl_8053AAF8->field_0x4c != 0xB) {
        Camera_804AA9E4* camera = lbl_8053AB10->field_0x24;
        fn_8016D398(camera->field_0x4, field_0x1c0, false, !field_0x1d0, &field_0x140, &field_0x1b0,
                    &field_0x1b8, &field_0x1b4, &field_0x54, &field_0x6c, 6.0f / camera->field_0x858);
    }
}

void cls_8016ED88::fn_8016ED88() {
    for (int i = 0; i < 4; i++) {
        mEntries[i].field_0x0 = NULL;
        mEntries[i].field_0x14 = NULL;
    }
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
    field_0x140.fn_8016ED88();
    field_0x1bc = 0;
    field_0x1c0 = 0;
}

cls_8016FDE0::cls_8016FDE0() {}
