#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Unk_8045A080.h"
#include "JSystem/JUtility/JUTTexture.h"
#include "JSystem/JUtility/JUTVideo.h"
#include "JSystem/J2D/J2DGrafContext.h"

// 0x8015F454: one textured quad (position rect, texture rect in texels, colour, optional
// second colour, optional mask texture, mirror flag for the mask, blend flag).
void fn_8015F454(JUTTexture* tex, const f32* pos, const f32* uv, GXColor color, const GXColorS10* color2,
                 JUTTexture* mask, u8 mirror, u8 blend);

// 0x801602E8
cls_804A03A8::cls_804A03A8(JKRHeap* heap) {
    heap->getTotalFreeSize();
    field_0x290 = new (heap, 0) JUTTexture(
        static_cast<const ResTIMG*>(gGameRoot_8053AAF8->field_0xa4->getResource('?\?\?\?', "circle.bti")), 0);
    field_0x294 = new (heap, 0) JUTTexture(
        static_cast<const ResTIMG*>(gGameRoot_8053AAF8->field_0xa4->getResource('?\?\?\?', "circle2.bti")), 0);
}

// 0x80160278
cls_804A03A8::~cls_804A03A8() {
    delete field_0x294;
    delete field_0x290;
}

// FIXME(shotgun): 99.9 %, reloc-name (the 32 zeros are stored straight-line, as in the original)
// 0x801601E0
void cls_804A03A8::fn_801601E0() {
    field_0x204[0] = 0;
    field_0x204[1] = 0;
    field_0x204[2] = 0;
    field_0x204[3] = 0;
    field_0x204[4] = 0;
    field_0x204[5] = 0;
    field_0x204[6] = 0;
    field_0x204[7] = 0;
    field_0x204[8] = 0;
    field_0x204[9] = 0;
    field_0x204[10] = 0;
    field_0x204[11] = 0;
    field_0x204[12] = 0;
    field_0x204[13] = 0;
    field_0x204[14] = 0;
    field_0x204[15] = 0;
    field_0x204[16] = 0;
    field_0x204[17] = 0;
    field_0x204[18] = 0;
    field_0x204[19] = 0;
    field_0x204[20] = 0;
    field_0x204[21] = 0;
    field_0x204[22] = 0;
    field_0x204[23] = 0;
    field_0x204[24] = 0;
    field_0x204[25] = 0;
    field_0x204[26] = 0;
    field_0x204[27] = 0;
    field_0x204[28] = 0;
    field_0x204[29] = 0;
    field_0x204[30] = 0;
    field_0x204[31] = 0;
    field_0x284 = 0;
    field_0x288 = 0.0f;
    field_0x28c = 0.0f;
}

// FIXME(shotgun): 98.6 %, missing/extra + reloc-name
// 0x8016005C
void cls_804A03A8::fn_8016005C() {
    for (s32 i = 0; i < 0x20; i++) {
        if (field_0x204[i] > 0) {
            field_0x204[i]--;
        }
    }
    f32 old = field_0x288;
    if (field_0x284 != 0) {
        f32 value = old + field_0x28c;
        field_0x288 = value >= 1.0f ? 1.0f : value;
    } else {
        f32 value = old - field_0x28c;
        field_0x288 = value >= 0.0f ? value : 0.0f;
    }
    field_0x284 = 0;
    if (field_0x288 > 0.0f && old == 0.0f) {
        gAudioMgr_8053AF80->fn_80457E04(true);
    } else if (field_0x288 == 0.0f && old > 0.0f) {
        gAudioMgr_8053AF80->fn_80457E04(false);
    }
}

// FIXME(shotgun): 92.5 %, different (stack slots of the two arrays and the colour copy are ordered differently)
// 0x8015F9C0
void cls_804A03A8::fn_8015F9C0() {
    u8 active = 0;
    for (s32 i = 0; i < 0x20; i++) {
        if (field_0x204[i] > 0) {
            active = 1;
        }
    }
    if (active) {
        f32 width = (f32)(s32)JUTVideo::sManager->getFbWidth();
        f32 height = (f32)(s32)JUTVideo::sManager->getEfbHeight();
        for (s32 i = 0; i < 0x20; i++) {
            if (field_0x204[i] > 0) {
                switch (field_0x4[i]) {
                case 0: {
                    f32 t = 0.033333335f * field_0x204[i];
                    t = t * t;
                    f32 alpha = 2.0f * t;
                    if (alpha > 1.0f) {
                        alpha = 2.0f - alpha;
                    }
                    alpha *= 159.0f;
                    Vec proj;
                    gGameRoot2_8053AB10->field_0x24->fn_8030C5F0(&field_0x84[i], false, false, &proj);
                    f32 uv[4];
                    uv[0] = t * (proj.x - 20.0f);
                    uv[1] = t * (proj.y - 15.0f);
                    uv[2] = t * (20.0f + proj.x) + (1.0f - t) * width;
                    uv[3] = t * (15.0f + proj.y) + (1.0f - t) * height;
                    f32 pos[4];
                    pos[0] = 0.0f;
                    pos[1] = 0.0f;
                    pos[2] = width;
                    pos[3] = height;
                    GXColor color;
                    color.r = 0xFF;
                    color.g = 0xFF;
                    color.b = 0xFF;
                    color.a = (u8)alpha;
                    fn_8015F454(gGameRoot2_8053AB10->field_0x34, pos, uv, color, NULL, NULL, 0, 0);
                    break;
                }
                case 1: {
                    f32 t = 1.0f - 0.033333335f * field_0x204[i];
                    t = t * t;
                    f32 alpha = 2.0f * t;
                    if (alpha > 1.0f) {
                        alpha = 2.0f - alpha;
                    }
                    alpha *= 159.0f;
                    Vec proj;
                    gGameRoot2_8053AB10->field_0x24->fn_8030C5F0(&field_0x84[i], false, false, &proj);
                    f32 uv[4];
                    uv[0] = t * (proj.x - 20.0f);
                    uv[1] = t * (proj.y - 15.0f);
                    uv[2] = t * (20.0f + proj.x) + (1.0f - t) * width;
                    uv[3] = t * (15.0f + proj.y) + (1.0f - t) * height;
                    f32 pos[4];
                    pos[0] = 0.0f;
                    pos[1] = 0.0f;
                    pos[2] = width;
                    pos[3] = height;
                    GXColor color;
                    color.r = 0xFF;
                    color.g = 0xFF;
                    color.b = 0xFF;
                    color.a = (u8)alpha;
                    fn_8015F454(gGameRoot2_8053AB10->field_0x34, pos, uv, color, NULL, NULL, 0, 0);
                    break;
                }
                case 5: {
                    JGeometry::TVec3f base(field_0x84[i].x, field_0x84[i].y, field_0x84[i].z + 6.0f);
                    Vec p;
                    gGameRoot2_8053AB10->field_0x24->fn_8030C5F0(&base, true, false, &p);
                    JGeometry::TVec3f off(20.0f + field_0x84[i].x, 20.0f + field_0x84[i].y, 6.0f + field_0x84[i].z);
                    Vec q;
                    gGameRoot2_8053AB10->field_0x24->fn_8030C5F0(&off, true, false, &q);
                    f32 uv[4];
                    uv[0] = -256.0f;
                    uv[1] = -256.0f;
                    uv[2] = 256.0f;
                    uv[3] = 256.0f;
                    f32 pos[4];
                    pos[0] = p.x - 80.0f - 4.0f;
                    pos[1] = p.y - 80.0f - 4.0f;
                    pos[2] = 4.0f + (80.0f + p.x);
                    pos[3] = 4.0f + (80.0f + p.y);
                    q.x -= p.x;
                    q.y -= p.y;
                    q.z -= p.z;
                    GXColor white = {0xFF, 0xFF, 0xFF, 0xFF};
                    fn_8015F454(field_0x294, pos, uv, white, NULL, NULL, 0, 0);
                    f32 uv2[4];
                    uv2[0] = p.x - q.x;
                    uv2[1] = p.y - q.y;
                    uv2[2] = p.x + q.x;
                    uv2[3] = p.y + q.y;
                    f32 pos2[4];
                    pos2[0] = p.x - 80.0f;
                    pos2[1] = p.y - 80.0f;
                    pos2[2] = 80.0f + p.x;
                    pos2[3] = 80.0f + p.y;
                    GXColor white2 = {0xFF, 0xFF, 0xFF, 0xFF};
                    fn_8015F454(gGameRoot2_8053AB10->field_0x34, pos2, uv2, white2, NULL, field_0x294, 1, 0);
                    break;
                }
                }
            }
        }
    }
    if (field_0x288 > 0.0f) {
        f32 sq = field_0x288 * field_0x288;
        JUtility::TColor color(0, 0, 0, (u8)(255.0f * sq));
        gGameRoot2_8053AB10->fn_8013B460();
        f32 bar = 48.0f * sq;
        J2DFillBox(0.0f, 0.0f, (f32)(s32)JUTVideo::sManager->getFbWidth(), bar, color);
        J2DFillBox(0.0f, (f32)(s32)JUTVideo::sManager->getEfbHeight() - bar, (f32)(s32)JUTVideo::sManager->getFbWidth(),
                   (f32)(s32)JUTVideo::sManager->getEfbHeight(), color);
    }
}

// 0x8015F8FC
u8 cls_804A03A8::fn_8015F8FC(s32 layer, const JGeometry::TVec3f* pos, s32 mode) {
    s32 placed = 0;
    if (layer < 8) {
        for (s32 i = 0; i < 0x20; i++) {
            if (field_0x204[i] == 0) {
                field_0x4[i] = mode;
                field_0x84[i].x = pos->x;
                field_0x84[i].y = pos->y;
                field_0x84[i].z = pos->z;
                placed = 1;
                switch (mode) {
                case 0:
                    field_0x204[i] = 0x1E;
                    break;
                case 1:
                    field_0x204[i] = 0x1E;
                    break;
                case 4:
                    field_0x204[i] = 0x73;
                    break;
                default:
                    field_0x204[i] = 1;
                    break;
                }
                break;
            }
        }
    }
    return placed;
}

// FIXME(shotgun): 99.9 %, reloc-name
// 0x8015F454
void fn_8015F454(JUTTexture* tex, const f32* pos, const f32* uv, GXColor color, const GXColorS10* color2,
                 JUTTexture* mask, u8 mirror, u8 blend) {
    f32 u0 = uv[0];
    f32 v0 = uv[1];
    f32 u1 = uv[2];
    f32 v1 = uv[3];
    gGameRoot2_8053AB10->fn_8013B490();
    gGameRoot2_8053AB10->fn_8013B1E8();
    tex->load(GX_TEXMAP0);
    if (mask != NULL) {
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX1, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_TEX_ST, GX_F32, 0);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
        GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
        GXSetNumTexGens(2);
        mask->load(GX_TEXMAP1);
        GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
        GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
        GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_TEXA, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        if (mask->getFormat() == 0xE) {
            GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP1);
            GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_ALPHA, GX_CH_ALPHA, GX_CH_ALPHA, GX_CH_RED);
        }
        GXSetNumTevStages(2);
    }
    GXSetTevColor(GX_TEVREG0, color);
    if (color2 != NULL) {
        GXSetTevColorS10(GX_TEVREG1, *color2);
    }
    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    if (blend) {
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_SET);
    }
    u0 /= tex->getWidth();
    v0 /= tex->getHeight();
    u1 /= tex->getWidth();
    v1 /= tex->getHeight();
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    if (mask != NULL) {
        if (mirror) {
            GXPosition3f32(pos[0], pos[1], 0.0f);
            GXTexCoord2f32(u0, v0);
            GXTexCoord2f32(-1.0f, -1.0f);
            GXPosition3f32(pos[2], pos[1], 0.0f);
            GXTexCoord2f32(u1, v0);
            GXTexCoord2f32(1.0f, -1.0f);
            GXPosition3f32(pos[2], pos[3], 0.0f);
            GXTexCoord2f32(u1, v1);
            GXTexCoord2f32(1.0f, 1.0f);
            GXPosition3f32(pos[0], pos[3], 0.0f);
            GXTexCoord2f32(u0, v1);
            GXTexCoord2f32(-1.0f, 1.0f);
        } else {
            GXPosition3f32(pos[0], pos[1], 0.0f);
            GXTexCoord2f32(u0, v0);
            GXTexCoord2f32(0.0f, 0.0f);
            GXPosition3f32(pos[2], pos[1], 0.0f);
            GXTexCoord2f32(u1, v0);
            GXTexCoord2f32(1.0f, 0.0f);
            GXPosition3f32(pos[2], pos[3], 0.0f);
            GXTexCoord2f32(u1, v1);
            GXTexCoord2f32(1.0f, 1.0f);
            GXPosition3f32(pos[0], pos[3], 0.0f);
            GXTexCoord2f32(u0, v1);
            GXTexCoord2f32(0.0f, 1.0f);
        }
    } else {
        GXPosition3f32(pos[0], pos[1], 0.0f);
        GXTexCoord2f32(u0, v0);
        GXPosition3f32(pos[2], pos[1], 0.0f);
        GXTexCoord2f32(u1, v0);
        GXPosition3f32(pos[2], pos[3], 0.0f);
        GXTexCoord2f32(u1, v1);
        GXPosition3f32(pos[0], pos[3], 0.0f);
        GXTexCoord2f32(u0, v1);
    }
}
