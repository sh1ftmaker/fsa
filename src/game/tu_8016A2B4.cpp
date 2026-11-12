#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/cls_8016ED88.h"
#include "game/ActorManager.h"
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

// Forward declarations.
void getMapTexView(JGeometry::TVec3f& center, JGeometry::TVec3f& half);

cls_8016FDE0::cls_8016FDE0() {}

// FIXME(shotgun): 80.8 %, missing/extra (the original reloads each emitter field for the call; 0xF4-0x100 are deleted without a null check)
GameRootSub24_8016A3AC::~GameRootSub24_8016A3AC() {
    for (int i = 0; i < 4; i++) {
        if (field_0x140.mEntries[i].field_0x0 != NULL) {
            getEffectSystem_8013C824()->mEmitterManager->forceDeleteEmitter(field_0x140.mEntries[i].field_0x0);
        }
        if (field_0x140.mEntries[i].field_0x18 != NULL) {
            delete field_0x140.mEntries[i].field_0x18;
        }
    }
    if (field_0x138 != NULL) {
        getEffectSystem_8013C824()->mEmitterManager->forceDeleteEmitter(field_0x138);
    }
    if (field_0x13c != NULL) {
        getEffectSystem_8013C824()->mEmitterManager->forceDeleteEmitter(field_0x13c);
    }
    if (field_0x130 != NULL) {
        getEffectSystem_8013C824()->mEmitterManager->forceDeleteEmitter(field_0x130);
    }
    if (field_0x134 != NULL) {
        getEffectSystem_8013C824()->mEmitterManager->forceDeleteEmitter(field_0x134);
    }
    if (field_0x124 != NULL) {
        getEffectSystem_8013C824()->mEmitterManager->forceDeleteEmitter(field_0x124);
    }
    if (field_0x128 != NULL) {
        getEffectSystem_8013C824()->mEmitterManager->forceDeleteEmitter(field_0x128);
    }
    if (field_0x12c != NULL) {
        getEffectSystem_8013C824()->mEmitterManager->forceDeleteEmitter(field_0x12c);
    }
    delete field_0x120;
    delete field_0x11c;
    delete field_0x118;
    delete field_0x114;
    delete field_0x110;
    delete field_0x10c;
    delete field_0x108;
    delete field_0x104;
    delete field_0x100;
    delete field_0xfc;
    delete field_0xf8;
    delete field_0xf4;
    field_0x0->destroy();
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

void cls_8016ED88::fn_8016ED88() {
    for (int i = 0; i < 4; i++) {
        mEntries[i].field_0x0 = NULL;
        mEntries[i].field_0x14 = 0;
    }
}

#ifdef NONMATCHING
static const s16 lbl_8050DD28[4] = {0x048F, 0x0490, 0x0491, 0x0492};

// FIXME(shotgun): missing/extra
void fn_8016D398(Mtx view, s32 frame, bool arg2, bool arg3, cls_8016ED88* entries, s32* timer1,
                 s32* timer2, u8* flag, J3DAnmTextureSRTKey** btk, J3DAnmTevRegKey** brk, f32 scale) {
    s32 maxTimer = -1;
    if (*timer2 > 0) {
        (*timer2)--;
    } else if (*flag != 0) {
        *flag = 0;
        for (s32 i = 0; i < 4; i++) {
            cls_8016FDE0* entry = &entries->mEntries[i];
            if (entry->field_0x0 == NULL) {
                entry->field_0x4.set(576.0f * (gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 0.5f),
                                     384.0f * (gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 0.5f), 0.0f);
                entry->field_0x10 = (s16)((s32)(4.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) & 3);
                u32 groupId = arg2 ? 0 : 12;
                entry->field_0x0 = spawnEmitter_8013CC50(groupId, &entry->field_0x4, lbl_8050DD28[entry->field_0x10],
                                                         0, 0, NULL, NULL, true, 1.0f);
                entry->field_0x0->setStatus(JPAEMIT_Immortal);
                entry->field_0x14 = 60;
                if (entry->field_0x18 != NULL) {
                    entry->field_0x18->startSound(0x30078);
                }
                break;
            }
        }
    } else if (*timer1 > 0) {
        (*timer1)--;
    } else {
        for (s32 i = 0; i < 4; i++) {
            cls_8016FDE0* entry = &entries->mEntries[i];
            if (entry->field_0x0 == NULL) {
                entry->field_0x4.set(576.0f * (gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 0.5f),
                                     384.0f * (gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 0.5f), 0.0f);
                entry->field_0x10 = (s16)((s32)(4.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) & 3);
                u32 groupId = arg2 ? 0 : 12;
                entry->field_0x0 = spawnEmitter_8013CC50(groupId, &entry->field_0x4, lbl_8050DD28[entry->field_0x10],
                                                         0, 0, NULL, NULL, true, 1.0f);
                entry->field_0x0->setStatus(JPAEMIT_Immortal);
                entry->field_0x14 = 60;
                if (entry->field_0x18 != NULL) {
                    entry->field_0x18->startSound(0x30031);
                }
                break;
            }
        }
        *timer1 = 60 + (s32)(150.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
        if ((s32)(100.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) < 50) {
            *flag = 1;
            *timer2 = 5 + (s32)(5.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
        }
    }

    for (s32 i = 0; i < 4; i++) {
        cls_8016FDE0* entry = &entries->mEntries[i];
        if (entry->field_0x18 != NULL) {
            entry->field_0x18->process();
        }
        if (entry->field_0x0 != NULL) {
            if (entry->field_0x0->isEnableDeleteEmitter()) {
                getEffectSystem_8013C824()->mEmitterManager->forceDeleteEmitter(entry->field_0x0);
                entry->field_0x0 = NULL;
                entry->field_0x14 = 0;
            } else {
                JGeometry::TVec3f s(scale, scale, scale);
                entry->field_0x0->setGlobalScale(s);
                if (!arg2) {
                    f32 z = -640.0f;
                    JGeometry::TVec3f trans(-view[0][3] + entry->field_0x4.x, 32.0f + (z - view[1][3]),
                                            entry->field_0x4.y + z);
                    entry->field_0x0->setGlobalTranslation(trans);
                } else {
                    JGeometry::TVec3f trans(300.0f + entry->field_0x4.x, 220.0f + entry->field_0x4.y, -513.0f);
                    entry->field_0x0->setGlobalTranslation(trans);
                }
                if (entry->field_0x14 > 0) {
                    entry->field_0x14--;
                    if (entry->field_0x14 > maxTimer) {
                        maxTimer = entry->field_0x14;
                    }
                }
            }
        }
    }

    if (maxTimer < 0) {
        maxTimer = 0;
    }
    for (s32 i = 0; i < 6; i++) {
        if (i == 3 && !arg3) {
            continue;
        }
        if ((i == 1 || i == 2) && arg3) {
            continue;
        }
        if (btk[i] != NULL) {
            btk[i]->setFrame((f32)(frame % btk[i]->getTotalFrameCount()));
        }
        if (brk[i] != NULL) {
            brk[i]->setFrame((f32)((brk[i]->getTotalFrameCount() * maxTimer) / 60));
        }
    }
}
#endif

void GameRootSub24_8016A3AC::fn_8016DCEC() {
    if (isGameModeSet() && field_0x1cf && gGameRoot_8053AAF8->field_0x4c != 0xB) {
        Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
        fn_8016D398(camera->field_0x4, field_0x1c0, false, !field_0x1d0, &field_0x140, &field_0x1b0,
                    &field_0x1b8, &field_0x1b4, &field_0x54, &field_0x6c, 6.0f / camera->field_0x858);
    }
}

// FIXME(shotgun): 100 %
void GameRootSub24_8016A3AC::fn_8016DB60() {
    if (isGameModeSet()) {
        fn_8016D33C();
        fn_8016D2E0();
        fn_8016D284();
        fn_8016CB14();
        fn_8016C2FC();
        fn_8016B910();
        fn_8016B8AC();
        fn_8016B4D4();
        fn_8016B248();
        fn_8016B0B0();
        fn_8016ACCC();
        fn_8016AD28();
        fn_8016AC70();
        fn_8016AC14();
        fn_8016B054();
        fn_8016ABB8();
        fn_8016AB5C();
        fn_8016AAA4();
        fn_8016A9EC();
        fn_8016AB00();
        fn_8016AA48();
    }
}

// 100 % since the draw passes below are kept out of line (pragma region at fn_8016D33C).
void GameRootSub24_8016A3AC::fn_8016DA6C() {
    if (isGameModeSet()) {
        fn_8016ADE0();
        fn_8016AD84();
    }
}

// Not auto-inlined: fn_8016DB60 calls fn_8016D33C, fn_8016D2E0, fn_8016D284 and fn_8016B8AC out of
// line in the original (docs/INLINING.md, pragma rule 1).
#pragma auto_inline off
void GameRootSub24_8016A3AC::fn_8016D33C() {
    if (field_0x1c4) {
        JGeometry::TVec3f pos(256.0f, 256.0f, 0.0f);
        fn_8016A7DC(field_0x4, pos, -1, true, true, 0.5f, 10.24f);
    }
}

void GameRootSub24_8016A3AC::fn_8016D2E0() {
    if (field_0x1c5) {
        JGeometry::TVec3f pos(256.0f, 256.0f, 0.0f);
        fn_8016A7DC(field_0xc, pos, -1, true, true, 0.5f, 10.24f);
    }
}

void GameRootSub24_8016A3AC::fn_8016D284() {
    if (field_0x1c6) {
        JGeometry::TVec3f pos(256.0f, 256.0f, 0.0f);
        fn_8016A7DC(field_0x14, pos, -1, true, true, 0.5f, 10.24f);
    }
}

#ifdef NONMATCHING
// FIXME(shotgun): missing/extra
void GameRootSub24_8016A3AC::fn_8016CB14() {
    if (field_0x1c7 && field_0xf8 != NULL && field_0xfc != NULL) {
        f32 width = (s32)JUTVideo::getManager()->getFbWidth();
        f32 height = (s32)JUTVideo::getManager()->getEfbHeight();
        f32 scroll = 0.00048828125f * (f32)(field_0x1c0 & 0x7FF);
        JGeometry::TVec3f center;
        JGeometry::TVec3f half;
        getMapTexView(center, half);
        gGameRoot2_8053AB10->fn_8013B460();
        gGameRoot2_8053AB10->fn_8013B1E8();
        field_0xf8->load(GX_TEXMAP0);
        field_0xfc->load(GX_TEXMAP1);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX1, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_TEX_ST, GX_F32, 0);
        GXSetNumChans(0);
        GXSetNumTexGens(2);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
        GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
        GXSetNumTevStages(2);
        GXColor color1 = lbl_8053D910;
        GXSetTevColor(GX_TEVREG0, color1);
        GXColor color2 = lbl_8053D914;
        GXSetTevColor(GX_TEVREG1, color2);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
        GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_C0, GX_CC_C1, GX_CC_APREV, GX_CC_ZERO);
        GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_TEXA, GX_CA_APREV, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
        GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);
        GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
        GXSetZCompLoc(GX_FALSE);
        GXSetCullMode(GX_CULL_NONE);
        JGeometry::TVec3f center2(center);
        JGeometry::TVec3f half2(half);
        center.x -= scroll;
        center.y -= scroll;
        center2.x += scroll;
        center2.y += scroll;
        center2.mul(JGeometry::TVec3f(2.0f, 2.0f, 1.0f));
        half2.mul(JGeometry::TVec3f(2.0f, 2.0f, 1.0f));
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(0.0f, 0.0f, 0.0f);
        GXTexCoord2f32(center.x - half.x, center.y - half.y);
        GXTexCoord2f32(center2.x - half2.x, center2.y - half2.y);
        GXPosition3f32(width, 0.0f, 0.0f);
        GXTexCoord2f32(center.x + half.x, center.y - half.y);
        GXTexCoord2f32(center2.x + half2.x, center2.y - half2.y);
        GXPosition3f32(width, height, 0.0f);
        GXTexCoord2f32(center.x + half.x, center.y + half.y);
        GXTexCoord2f32(center2.x + half2.x, center2.y + half2.y);
        GXPosition3f32(0.0f, height, 0.0f);
        GXTexCoord2f32(center.x - half.x, center.y + half.y);
        GXTexCoord2f32(center2.x - half2.x, center2.y + half2.y);
        GXEnd();
    }
}

// FIXME(shotgun): missing/extra
void GameRootSub24_8016A3AC::fn_8016C2FC() {
    if (field_0x1c9 && field_0x100 != NULL && field_0x104 != NULL) {
        f32 width = (s32)JUTVideo::getManager()->getFbWidth();
        f32 height = (s32)JUTVideo::getManager()->getEfbHeight();
        f32 scroll = 0.00048828125f * (f32)(field_0x1c0 & 0x7FF);
        JGeometry::TVec3f center;
        JGeometry::TVec3f half;
        getMapTexView(center, half);
        gGameRoot2_8053AB10->fn_8013B460();
        gGameRoot2_8053AB10->fn_8013B1E8();
        field_0x100->load(GX_TEXMAP0);
        field_0x104->load(GX_TEXMAP1);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX1, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_TEX_ST, GX_F32, 0);
        GXSetNumChans(0);
        GXSetNumTexGens(2);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
        GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
        GXSetNumTevStages(2);
        GXColor color1 = lbl_8053D918;
        GXSetTevColor(GX_TEVREG0, color1);
        GXColor color2 = lbl_8053D91C;
        GXSetTevColor(GX_TEVREG1, color2);
        GXColor color3 = lbl_8053D920;
        GXSetTevColor(GX_TEVREG2, color3);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_C1, GX_CC_TEXA, GX_CC_ZERO);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A0, GX_CA_A1, GX_CA_TEXA, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
        GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_C2, GX_CC_TEXA, GX_CC_ZERO);
        GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_APREV, GX_CA_A2, GX_CA_TEXA, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
        GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);
        GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
        GXSetZCompLoc(GX_FALSE);
        GXSetCullMode(GX_CULL_NONE);
        JGeometry::TVec3f center2(center);
        JGeometry::TVec3f half2(half);
        center.x -= scroll;
        center.y -= scroll;
        center.mul(JGeometry::TVec3f(2.0f, 2.0f, 1.0f));
        half.mul(JGeometry::TVec3f(2.0f, 2.0f, 1.0f));
        center2.x -= scroll;
        center2.y -= scroll;
        center2.mul(JGeometry::TVec3f(2.0f, 2.0f, 1.0f));
        half2.mul(JGeometry::TVec3f(2.0f, 2.0f, 1.0f));
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(0.0f, 0.0f, 0.0f);
        GXTexCoord2f32(center.x - half.x, center.y - half.y);
        GXTexCoord2f32(center2.x - half2.x, center2.y - half2.y);
        GXPosition3f32(width, 0.0f, 0.0f);
        GXTexCoord2f32(center.x + half.x, center.y - half.y);
        GXTexCoord2f32(center2.x + half2.x, center2.y - half2.y);
        GXPosition3f32(width, height, 0.0f);
        GXTexCoord2f32(center.x + half.x, center.y + half.y);
        GXTexCoord2f32(center2.x + half2.x, center2.y + half2.y);
        GXPosition3f32(0.0f, height, 0.0f);
        GXTexCoord2f32(center.x - half.x, center.y + half.y);
        GXTexCoord2f32(center2.x - half2.x, center2.y + half2.y);
        GXEnd();
    }
}

// FIXME(shotgun): missing/extra
void GameRootSub24_8016A3AC::fn_8016B910() {
    if (field_0x1ca && field_0x10c != NULL && field_0x108 != NULL && field_0x100 != NULL && field_0x104 != NULL) {
        f32 width = (s32)JUTVideo::getManager()->getFbWidth();
        f32 height = (s32)JUTVideo::getManager()->getEfbHeight();
        f32 scroll = 0.00048828125f * (f32)(field_0x1c0 & 0x7FF);
        JGeometry::TVec3f center;
        JGeometry::TVec3f half;
        getMapTexView(center, half);
        gGameRoot2_8053AB10->fn_8013B460();
        gGameRoot2_8053AB10->fn_8013B1E8();
        field_0x100->load(GX_TEXMAP0);
        field_0x104->load(GX_TEXMAP1);
        field_0x108->load(GX_TEXMAP2);
        field_0x10c->load(GX_TEXMAP3);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX1, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX2, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX3, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_TEX_ST, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX2, GX_TEX_ST, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX3, GX_TEX_ST, GX_F32, 0);
        GXSetNumChans(0);
        GXSetNumTexGens(4);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
        GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
        GXSetTexCoordGen2(GX_TEXCOORD2, GX_TG_MTX2x4, GX_TG_TEX2, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
        GXSetTexCoordGen2(GX_TEXCOORD3, GX_TG_MTX2x4, GX_TG_TEX3, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
        GXSetNumTevStages(3);
        GXColor color1 = lbl_8053D924;
        GXSetTevColor(GX_TEVREG0, color1);
        GXColor color2 = lbl_8053D928;
        GXColor color3 = lbl_8053D92C;
        GXSetTevColor(GX_TEVREG1, color3);
        GXColor color4 = lbl_8053D930;
        GXColor color5 = lbl_8053D934;
        GXSetTevColor(GX_TEVREG2, color5);
        GXSetTevKColor(GX_KCOLOR0, color2);
        GXSetTevKColor(GX_KCOLOR1, color4);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD2, GX_TEXMAP2, GX_COLOR_NULL);
        GXSetTevKColorSel(GX_TEVSTAGE0, GX_TEV_KCSEL_K0);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_KONST, GX_CC_TEXA, GX_CC_ZERO);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_C1, GX_CC_TEXA, GX_CC_ZERO);
        GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_APREV, GX_CA_A1, GX_CA_TEXA, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
        GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_CPREV, GX_CC_C2, GX_CC_TEXA, GX_CC_ZERO);
        GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_APREV, GX_CA_A2, GX_CA_TEXA, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
        GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);
        GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
        GXSetZCompLoc(GX_FALSE);
        GXSetCullMode(GX_CULL_NONE);
        JGeometry::TVec3f center2(center);
        JGeometry::TVec3f half2(half);
        JGeometry::TVec3f center3(center);
        JGeometry::TVec3f half3(half);
        JGeometry::TVec3f center4(center);
        JGeometry::TVec3f half4(half);
        center.x -= scroll;
        center.y -= scroll;
        center.mul(JGeometry::TVec3f(2.0f, 2.0f, 1.0f));
        half.mul(JGeometry::TVec3f(2.0f, 2.0f, 1.0f));
        center2.x -= scroll;
        center2.y -= scroll;
        center2.mul(JGeometry::TVec3f(2.0f, 2.0f, 1.0f));
        half2.mul(JGeometry::TVec3f(2.0f, 2.0f, 1.0f));
        center3.x -= 0.7f * scroll;
        center3.y -= 0.7f * scroll;
        center3.mul(JGeometry::TVec3f(2.0f, 2.0f, 1.0f));
        half3.mul(JGeometry::TVec3f(2.0f, 2.0f, 1.0f));
        center4.x -= 0.7f * scroll;
        center4.y -= 0.7f * scroll;
        center4.mul(JGeometry::TVec3f(2.0f, 2.0f, 1.0f));
        half4.mul(JGeometry::TVec3f(2.0f, 2.0f, 1.0f));
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(0.0f, 0.0f, 0.0f);
        GXTexCoord2f32(center.x - half.x, center.y - half.y);
        GXTexCoord2f32(center2.x - half2.x, center2.y - half2.y);
        GXTexCoord2f32(center3.x - half3.x, center3.y - half3.y);
        GXTexCoord2f32(center4.x - half4.x, center4.y - half4.y);
        GXPosition3f32(width, 0.0f, 0.0f);
        GXTexCoord2f32(center.x + half.x, center.y - half.y);
        GXTexCoord2f32(center2.x + half2.x, center2.y - half2.y);
        GXTexCoord2f32(center3.x + half3.x, center3.y - half3.y);
        GXTexCoord2f32(center4.x + half4.x, center4.y - half4.y);
        GXPosition3f32(width, height, 0.0f);
        GXTexCoord2f32(center.x + half.x, center.y + half.y);
        GXTexCoord2f32(center2.x + half2.x, center2.y + half2.y);
        GXTexCoord2f32(center3.x + half3.x, center3.y + half3.y);
        GXTexCoord2f32(center4.x + half4.x, center4.y + half4.y);
        GXPosition3f32(0.0f, height, 0.0f);
        GXTexCoord2f32(center.x - half.x, center.y + half.y);
        GXTexCoord2f32(center2.x - half2.x, center2.y + half2.y);
        GXTexCoord2f32(center3.x - half3.x, center3.y + half3.y);
        GXTexCoord2f32(center4.x - half4.x, center4.y + half4.y);
        GXEnd();
    }
}
#endif

void GameRootSub24_8016A3AC::fn_8016B8AC() {
    if (field_0x1cb[0]) {
        JGeometry::TVec3f pos(256.0f, 256.0f + field_0x1e4, 0.0f);
        fn_8016A7DC(field_0x34, pos, 1, true, false, 1.0f, 102.4f);
    }
}

void GameRootSub24_8016A3AC::fn_8016B4D4() {
    if ((field_0x1cb[1] && field_0x110 != NULL) || (field_0x1cb[2] && field_0x118 != NULL)) {
        f32 width = (s32)JUTVideo::getManager()->getFbWidth();
        f32 height = (s32)JUTVideo::getManager()->getEfbHeight();
        f32 scroll = 0.000244140625f * (f32)(field_0x1c0 & 0xFFF);
        JGeometry::TVec3f center1;
        JGeometry::TVec3f half1;
        getMapTexView(center1, half1);
        JGeometry::TVec3f center2 = center1;
        JGeometry::TVec3f half2 = half1;
        center1.mul(JGeometry::TVec3f(1.0f, 3.0f, 1.0f));
        center1.x -= scroll;
        half1.mul(JGeometry::TVec3f(1.0f, 3.0f, 1.0f));
        center2.mul(JGeometry::TVec3f(1.0f, 0.75f, 1.0f));
        half2.mul(JGeometry::TVec3f(1.0f, 1.5f, 1.0f));
        gGameRoot2_8053AB10->fn_8013B460();
        gGameRoot2_8053AB10->fn_8013B1E8();
        GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
        if (field_0x1cb[1]) {
            field_0x114->load(GX_TEXMAP0);
        } else {
            field_0x11c->load(GX_TEXMAP0);
        }
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(0.0f, 0.0f, 0.0f);
        GXTexCoord2f32(center1.x - half1.x, center1.y - half1.y);
        GXPosition3f32(width, 0.0f, 0.0f);
        GXTexCoord2f32(center1.x + half1.x, center1.y - half1.y);
        GXPosition3f32(width, height, 0.0f);
        GXTexCoord2f32(center1.x + half1.x, center1.y + half1.y);
        GXPosition3f32(0.0f, height, 0.0f);
        GXTexCoord2f32(center1.x - half1.x, center1.y + half1.y);
        GXEnd();
        if (field_0x1cb[1]) {
            field_0x110->load(GX_TEXMAP0);
        } else {
            field_0x118->load(GX_TEXMAP0);
        }
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(0.0f, 0.0f, 0.0f);
        GXTexCoord2f32(center2.x - half2.x, center2.y - half2.y);
        GXPosition3f32(width, 0.0f, 0.0f);
        GXTexCoord2f32(center2.x + half2.x, center2.y - half2.y);
        GXPosition3f32(width, height, 0.0f);
        GXTexCoord2f32(center2.x + half2.x, center2.y + half2.y);
        GXPosition3f32(0.0f, height, 0.0f);
        GXTexCoord2f32(center2.x - half2.x, center2.y + half2.y);
        GXEnd();
    }
}

#pragma auto_inline reset

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
        gGameRoot2_8053AB10->fn_8013B460();
        gGameRoot2_8053AB10->fn_8013B1E8();
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

void GameRootSub24_8016A3AC::fn_8016B0B0() {
    if (field_0x1cf) {
        Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
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

// Not auto-inlined: fn_8016DB60 calls fn_8016B054 out of line in the original (docs/INLINING.md,
// pragma rule 1).
#pragma auto_inline off
void GameRootSub24_8016A3AC::fn_8016B054() {
    if (field_0x1d3) {
        JGeometry::TVec3f pos(256.0f, 256.0f, 0.0f);
        fn_8016A7DC(field_0xa8, pos, -1, true, true, 1.0f, 10.0f);
    }
}
#pragma auto_inline reset

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
        gGameRoot2_8053AB10->fn_8013B460();
        gGameRoot2_8053AB10->fn_8013B1E8();
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

// Not auto-inlined: fn_8016DA6C calls fn_8016AD84, and fn_8016DB60 calls fn_8016AD28 down to fn_8016A9EC,
// out of line in the original (docs/INLINING.md, pragma rule 1).
#pragma auto_inline off
void GameRootSub24_8016A3AC::fn_8016AD84() {
    if (field_0x1ed) {
        JGeometry::TVec3f pos(256.0f, 256.0f, 0.0f);
        fn_8016A7DC(field_0x1c, pos, 1, true, true, 1.5f, 10.24f);
    }
}

void GameRootSub24_8016A3AC::fn_8016AD28() {
    if (field_0x1ef) {
        JGeometry::TVec3f pos(256.0f, 256.0f, 0.0f);
        fn_8016A7DC(field_0x28, pos, 1, true, true, 1.0f, 10.0f);
    }
}

void GameRootSub24_8016A3AC::fn_8016ACCC() {
    if (field_0x1f0) {
        JGeometry::TVec3f pos(256.0f, 256.0f, 0.0f);
        fn_8016A7DC(field_0x84, pos, 1, true, true, 1.0f, 10.0f);
    }
}

void GameRootSub24_8016A3AC::fn_8016AC70() {
    if (field_0x1f1) {
        JGeometry::TVec3f pos(256.0f, 256.0f, 0.0f);
        fn_8016A7DC(field_0x90, pos, 1, true, true, 1.5f, 10.0f);
    }
}

void GameRootSub24_8016A3AC::fn_8016AC14() {
    if (field_0x1f2) {
        JGeometry::TVec3f pos(256.0f, 256.0f, 0.0f);
        fn_8016A7DC(field_0x9c, pos, 1, true, true, 1.0f, 10.0f);
    }
}

void GameRootSub24_8016A3AC::fn_8016ABB8() {
    if (field_0x1f3) {
        JGeometry::TVec3f pos(256.0f, 256.0f, 0.0f);
        fn_8016A7DC(field_0xb4, pos, 1, true, true, 1.0f, 10.0f);
    }
}

void GameRootSub24_8016A3AC::fn_8016AB5C() {
    if (field_0x1d8 > 0) {
        JGeometry::TVec3f pos(256.0f, 256.0f, 0.0f);
        fn_8016A7DC(field_0xc0, pos, 1, true, true, 1.0f, 10.0f);
    }
}

void GameRootSub24_8016A3AC::fn_8016AB00() {
    if (field_0x1f4) {
        JGeometry::TVec3f pos(256.0f, 256.0f, 0.0f);
        fn_8016A7DC(field_0xd4, pos, 1, true, true, 1.0f, 10.0f);
    }
}

void GameRootSub24_8016A3AC::fn_8016AAA4() {
    if (field_0x1dc) {
        JGeometry::TVec3f pos(256.0f, 256.0f, 0.0f);
        fn_8016A7DC(field_0xcc, pos, 1, true, true, 1.5f, 1.0f);
    }
}

void GameRootSub24_8016A3AC::fn_8016AA48() {
    if (field_0x1f5) {
        JGeometry::TVec3f pos(256.0f, 256.0f, 0.0f);
        fn_8016A7DC(field_0xe0, pos, 1, true, true, 1.5f, 10.24f);
    }
}

void GameRootSub24_8016A3AC::fn_8016A9EC() {
    if (field_0x1dd) {
        JGeometry::TVec3f pos(256.0f, 256.0f, 0.0f);
        fn_8016A7DC(field_0xec, pos, -1, true, true, 0.5f, 10.0f);
    }
}
#pragma auto_inline reset

// FIXME: 98.3. regalloc only: the original keeps the camera in r4 and the 0x4330
// conversion constant in r3 (ours swaps them) and the float temporaries in f3/f5.
void GameRootSub24_8016A3AC::fn_8016A7DC(J3DModel* model, const JGeometry::TVec3f& pos, s32 dir, bool followX, bool followY, f32 parallax, f32 scale) {
    if (dir == 1) {
        bool skip = gGameRoot_8053AAF8->field_0x4c == 11 ? true : gGameRoot_8053AAF8->field_0x88 == 2;
        if (skip) {
            return;
        }
    }
    Mtx mtx;
    JGeometry::TVec3f off;
    f32 z = 1.5f * (f32)(dir << 10);
    Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
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

// The visible part of the map in texture coordinates (centre and half size;
// a map cell is 8 units). Dead-stripped: fn_8016ADE0, fn_8016B248, fn_8016B4D4,
// fn_8016B910, fn_8016C2FC and fn_8016CB14 inline it. Its own copy created the
// unit's first pool literals (0.125, 1, 0.5, 0, then the int-to-float double),
// which come before fn_8016A7DC's 1.5 in the original .sdata2.
void getMapTexView(JGeometry::TVec3f& center, JGeometry::TVec3f& half) {
    Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
    TileMap_804AA858* map = gGameRoot2_8053AB10->field_0x20;
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

// Returns gGameRoot_8053AAF8->field_0x4->field_0x24 (the object act_environment_control_2
// writes flags into).
GameRootSub24_8016A3AC* fn_8016A3AC() {
    return gGameRoot_8053AAF8->field_0x4->field_0x24;
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

void GameRootSub24_8016A3AC::fn_8016A2B4() {
    if (field_0x124 != NULL) {
        JPAEmitterManager* manager = getEffectSystem_8013C824()->mEmitterManager;
        manager->forceDeleteEmitter(field_0x124);
    }
    if (field_0x128 != NULL) {
        JPAEmitterManager* manager = getEffectSystem_8013C824()->mEmitterManager;
        manager->forceDeleteEmitter(field_0x128);
    }
    if (field_0x12c != NULL) {
        JPAEmitterManager* manager = getEffectSystem_8013C824()->mEmitterManager;
        manager->forceDeleteEmitter(field_0x12c);
    }
}
