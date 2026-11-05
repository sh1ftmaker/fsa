#include "game/GameGlobals.h"
#include "game/GameMode.h"
#include "JSystem/JUtility/JUTTexture.h"
#include "JSystem/JUtility/JUTVideo.h"

void ShadowCircle_804A0160::fn_80158B3C() {
    field_0x4.set(0xFF, 0xFF, 0xFF, 0);
    field_0xc = 0xFF;
}

void ShadowCircle_804A0160::fn_8015945C() {
    field_0x4.set(0xFF, 0xFF, 0xFF, 0);
    field_0xc = 0xFF;
    field_0xd[0] = 0;
    field_0xd[1] = 0xFF;
}

ShadowCircle_804A0160::ShadowCircle_804A0160(JKRHeap* heap) : field_0x4() {
    field_0x8 = new (heap, 0) JUTTexture(
        static_cast<const ResTIMG*>(lbl_8053AAF8->field_0xa4->getResource('?\?\?\?', "circle.bti")), 0);
}

ShadowCircle_804A0160::~ShadowCircle_804A0160() {
    delete field_0x8;
}

GXLightObj lbl_804BFB60[20];

// FIXME: 99.48%. Only the .sdata2 float literals differ: the original's
// lbl_8053D454, lbl_8053D458 and lbl_8053D440 are @472, @473 and @475 in ours.
void ShadowCircle_804A0160::fn_80158E88() {
    GXInitLightPos(&lbl_804BFB60[0], -100000.0f, -100000.0f, 100000.0f);
    GXInitLightAttn(&lbl_804BFB60[0], 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
    GXColor white = {0xFF, 0xFF, 0xFF, 0xFF};
    GXInitLightColor(&lbl_804BFB60[0], white);
    GXLoadLightObjImm(&lbl_804BFB60[0], GX_LIGHT0);
    GXInitLightPos(&lbl_804BFB60[0], 100000.0f, -100000.0f, 100000.0f);
    GXColor grey = {0x7F, 0x7F, 0x7F, 0x7F};
    GXInitLightColor(&lbl_804BFB60[0], grey);
    GXLoadLightObjImm(&lbl_804BFB60[0], GX_LIGHT1);
    GXColor amb = {0x67, 0x67, 0x67, 0x67};
    GXSetChanAmbColor(GX_COLOR0A0, amb);
    GXColor mat = {0xFF, 0xFF, 0xFF, 0xFF};
    GXSetChanMatColor(GX_COLOR0A0, mat);
}

// FIXME: 96.5 %. The original materialises the mode-0xC test as a byte
// (subfic / cntlzw / extrwi., then bne) where ours compares (cmpwi / beq);
// a bool or u8 local and a u8 inline didn't give it. The rest is
// isGameModeSet() (game/GameMode.h).
void ShadowCircle_804A0160::fn_80159330() {
    if (lbl_8053AAF8->field_0x4c == GAME_MODE_C) {
        return;
    }
    if (!isGameModeSet() || !getGameView_802D6A9C()->fn_802DCD0C()) {
        if (lbl_8053AAF8->field_0x94 == 0 && lbl_8053AAF8->field_0x96 == 0) {
            field_0xd[0] = 0;
        }
    }
    field_0xd[1] = 0xFF;
}

// FIXME: 99.95%. Only the .sdata2 literals differ: lbl_8053D450 (-1.0f) and
// lbl_8053D43C (1.0f) are @560 and @474 in ours.
void ShadowCircle_804A0160::drawShadow_80158B5C(s32 layer, const JGeometry::TVec3f* pos, JUtility::TColor color, f32 scale) {
    if (layer >= 8) {
        return;
    }
    if (field_0xc >= 0xFF && field_0x4.toUInt32() == 0xFFFFFF00) {
        return;
    }
    gGameRoot2_8053AB10->fn_8013B438();
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetNumChans(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTexGens(1);
    if (field_0xd[1] < 0xFF) {
        s32 alpha = (color.a * field_0xd[1]) / 255;
        GXSetTevColor(GX_TEVREG0, JUtility::TColor(color.r, color.g, color.b, alpha));
    } else {
        GXSetTevColor(GX_TEVREG0, color);
    }
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_2, GX_TRUE, GX_TEVPREV);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP1);
    GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_RED, GX_CH_RED, GX_CH_RED, GX_CH_RED);
    GXSetNumTevStages(1);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_SET);
    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetColorUpdate(GX_FALSE);
    GXSetAlphaUpdate(GX_TRUE);
    GXSetCullMode(GX_CULL_NONE);
    field_0x8->load(GX_TEXMAP0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(pos->x - scale, pos->y, pos->z + scale);
    GXTexCoord2f32(-1.0f, -1.0f);
    GXPosition3f32(pos->x + scale, pos->y, pos->z + scale);
    GXTexCoord2f32(1.0f, -1.0f);
    GXPosition3f32(pos->x + scale, pos->y, pos->z - scale);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(pos->x - scale, pos->y, pos->z - scale);
    GXTexCoord2f32(-1.0f, 1.0f);
}

// FIXME: 99.92 %. The tev colour's stack slot is 0xC where the original has
// 0x10, and the 0.0f literal is @NNN (.sdata2 not split). The u16 sizes are
// converted with the signed (xoris 0x8000) form: an explicit (s32).
void ShadowCircle_804A0160::fn_80158F70() {
    if (field_0x4.toUInt32() == 0xFFFFFF00 && field_0xc >= 0xFF) {
        return;
    }
    GXRenderModeObj* renderMode = JUTVideo::sManager->getRenderMode();
    f32 fbWidth = (s32)renderMode->fbWidth;
    f32 efbHeight = (s32)renderMode->efbHeight;
    gGameRoot2_8053AB10->fn_8013B460();
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetNumChans(1);
    GXSetNumTexGens(0);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetNumTevStages(1);
    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetCullMode(GX_CULL_NONE);
    if (field_0xc < 0xFF) {
        JUtility::TColor tev = lbl_80543F28;
        tev.a = field_0xc;
        GXSetTevColor(GX_TEVREG0, tev);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_SET);
        GXSetColorUpdate(GX_FALSE);
        GXSetAlphaUpdate(GX_TRUE);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(0.0f, 0.0f, 0.0f);
        GXPosition3f32(fbWidth, 0.0f, 0.0f);
        GXPosition3f32(fbWidth, efbHeight, 0.0f);
        GXPosition3f32(0.0f, efbHeight, 0.0f);
    }
    GXSetColorUpdate(GX_TRUE);
    GXSetAlphaUpdate(GX_FALSE);
    if (field_0x4.toUInt32() != 0xFFFFFF00) {
        GXSetTevColor(GX_TEVREG0, field_0x4);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
        if (field_0xd[0] != 0) {
            GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCCLR, GX_BL_DSTALPHA, GX_LO_SET);
        } else {
            GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_DSTCLR, GX_LO_SET);
        }
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(0.0f, 0.0f, 0.0f);
        GXPosition3f32(fbWidth, 0.0f, 0.0f);
        GXPosition3f32(fbWidth, efbHeight, 0.0f);
        GXPosition3f32(0.0f, efbHeight, 0.0f);
    }
    if (field_0xc < 0xFF) {
        GXSetBlendMode(GX_BM_BLEND, GX_BL_ZERO, GX_BL_DSTALPHA, GX_LO_SET);
        GXSetColorUpdate(GX_TRUE);
        GXSetAlphaUpdate(GX_FALSE);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(0.0f, 0.0f, 0.0f);
        GXPosition3f32(fbWidth, 0.0f, 0.0f);
        GXPosition3f32(fbWidth, efbHeight, 0.0f);
        GXPosition3f32(0.0f, efbHeight, 0.0f);
    }
}
