#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/act/tu_801F9484.h"
#include "game/Map.h"
#include "game/RotMatrix.h"
#include "JSystem/JUtility/JUTTexture.h"
#include "dolphin/gx.h"

// Offsets of the four ghost positions from mPos (x, y pairs), .data at 0x804A0410.
static const f32 lbl_804A0410[8] = {-80.0f, -40.0f, 80.0f, -40.0f, -80.0f, 40.0f, 80.0f, 40.0f};

// FIXME(shotgun): table not written out (gate.py literal tripwire); 8 colour sets of three
// RGBA colours (96 bytes), then the three texture names; extern-declared, size from symbols.txt.
extern const JUtility::TColor lbl_804661E0[24];

// A flag of the ghost's attacks, .sbss at 0x8053AB58.
s32 lbl_8053AB58;

// 0x80164118.
cls_804A0440::~cls_804A0440() {
    releaseEmitter_801EE434(field_0x2e8);
    if (field_0x268 == 2) {
        getGameView_802D6A9C()->fn_802D7440(true);
        fn_8022ADCC();
    }
}

// 0x80163E80.
// FIXME(shotgun): 72.84 %, missing/extra + reloc-name
void cls_804A0440::init() {
    field_0x94 = 0;
    mBox.set(-60.0f, -60.0f, 60.0f, 60.0f);
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x258 = 0;
    field_0x25c = 5;
    field_0x27c = 1.0f;
    field_0x280 = 1.0f;
    field_0x284 = 0.4f;
    field_0x274 = 0.0f;
    field_0x278 = 0.05f;
    field_0x254 = 0;
    field_0x294[8] = 0.0f;
    field_0x294[7] = 0.0f;
    field_0x294[6] = 0.0f;
    field_0x294[0] = mPos.x;
    field_0x294[1] = mPos.y;
    field_0x294[2] = mPos.z;
    field_0x294[3] = mPos.x;
    field_0x294[4] = mPos.y;
    field_0x294[5] = mPos.z;
    field_0x260 = 0;
    field_0x288 = 0.0f;
    field_0x28c = 0.0f;
    lbl_8053AB58 = 0;
    field_0x2e8 = NULL;
    field_0x2b8[0] = mPos;
    field_0x2b8[0].x += lbl_804A0410[0];
    field_0x2b8[0].y += lbl_804A0410[1];
    field_0x2b8[1] = mPos;
    field_0x2b8[1].x += lbl_804A0410[2];
    field_0x2b8[1].y += lbl_804A0410[3];
    field_0x2b8[2] = mPos;
    field_0x2b8[2].x += lbl_804A0410[4];
    field_0x2b8[2].y += lbl_804A0410[5];
    field_0x2b8[3] = mPos;
    field_0x2b8[3].x += lbl_804A0410[6];
    field_0x2b8[3].y += lbl_804A0410[7];
    field_0x2ec = -1;
    field_0x2f0 = -1;
    field_0x2f4[0].set(0);
    field_0x2f4[1].set(0);
    field_0x2f4[2].set(0);
    field_0x26c = -1;
    field_0x290 = 0;
    field_0x270 = (field_0x90 >> 2) & 0x1F;
    field_0x196 = 1;
    field_0x268 = field_0x90 & 3;
    mLife = 10000;
    switch (field_0x268) {
    case 0:
        getGameView_802D6A9C()->fn_802D7440(false);
        field_0x230 = 0;
        break;
    case 1:
        field_0x290 = 0x8000;
        mBox.set(-12.0f, -12.0f, 12.0f, 12.0f);
        field_0x288 = 1.0f;
        field_0x230 = 12;
        break;
    case 2:
        field_0x230 = 14;
        break;
    }
}

// 0x80162208.
// FIXME(shotgun): 96.91 %, missing/extra + reloc-name
void cls_804A0440::draw() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    if (field_0x268 == 1) {
        return;
    }
    if (field_0x268 == 2) {
        params.field_0x1d = 3;
        params.field_0x1c = 1;
        fn_801F02BC("enemy_both2_sob.bin", field_0x258 + 0x1B1, &params, 0, 0, false, 1.0f, 1.0f);
        return;
    }
    params.field_0x1c = 1;
    if (field_0x258 >= 3) {
        params.field_0x1d = 2;
    } else {
        params.field_0x1d = -1;
    }
    if (field_0x188 == 0 && field_0xd8 > 0) {
        s32 start = field_0xdc - 25;
        if (field_0xd8 > start || field_0xdc < 25) {
            s32 v;
            if (field_0xdc < 25) {
                v = field_0xd8 * 255 / field_0xdc;
            } else {
                v = (field_0xd8 - start) * 255 / 25;
            }
            if (field_0xf8 == 2) {
                v = (s32)(v - 6.0f * mPos.z);
                if (v < 0) {
                    v = 0;
                }
            }
            if (field_0xfc == 5) {
                params.field_0x14.r += v;
                params.field_0x14.g += v >> 1;
            } else {
                params.field_0x14.r += v;
                params.field_0x14.g += v;
                params.field_0x14.b += v;
            }
        }
    }
    if (field_0x188 > 0) {
        params.field_0x14.r = 200;
        params.field_0x14.g = 0x41;
    }
    f32 angle = (u16)field_0x290 / -10430.228f;
    JGeometry::TVec3f rot(0.0f, angle, 0.0f);
    setRotScaleMtx(mtx, rot, field_0x27c);
    params.field_0x2d = 1;
    params.mMtx = mtx;
    fn_801618D0(&mPos, &params);
}

// 0x80161F98.
// FIXME(shotgun): 62.92 %, missing/extra
void cls_804A0440::fn_80161F98(s32 set, s32 frames) {
    if (field_0x2ec != set) {
        if (field_0x2ec < 0) {
            const JUtility::TColor* row = lbl_804661E0 + set * 3;
            field_0x2f4[0] = row[0];
            field_0x2f4[1] = row[1];
            field_0x2f4[2] = row[2];
        }
        field_0x2ec = set;
        if (field_0x2f0 < 0) {
            field_0x2f0 = 0;
        } else {
            field_0x2f0 = frames;
        }
    } else if (field_0x2f0 > 0) {
        f32 weight = 1.0f / field_0x2f0;
        for (s32 i = 0; i < 3; i++) {
            JUtility::TColor& c = field_0x2f4[i];
            const JUtility::TColor& target = lbl_804661E0[set * 3 + i];
            c.r = (u8)((f32)c.r * (1.0f - weight) + weight * (f32)target.r);
            c.g = (u8)((f32)c.g * (1.0f - weight) + weight * (f32)target.g);
            c.b = (u8)((f32)c.b * (1.0f - weight) + weight * (f32)target.b);
            c.a = (u8)((f32)c.a * (1.0f - weight) + weight * (f32)target.a);
        }
        field_0x2f0--;
    }
}

// 0x80161F20.
void cls_804A0440::vfunc_0x24() {
    startSound_801F0E34(0x40015, 0);
    vfunc_0x28(7, 0.0f, 16.0f);
    s32 bit = field_0x270;
    if (bit != 0) {
        getGameView_802D6A9C()->fn_802D8050(bit, true, 0xFF);
    }
}

// 0x801618D0.
// FIXME(shotgun): 99.96 %, operands (the texture names sit after the extern colour table: +0x20 here, +0x60 in the original)
void cls_804A0440::fn_801618D0(const JGeometry::TVec3f* pos, SpriteParams_801F5AE0* params) {
    JUTTexture tex0(static_cast<const ResTIMG*>(fn_804032E8('BPOH', "obake_sirokuro.bti")), 0);
    JUTTexture tex1(static_cast<const ResTIMG*>(fn_804032E8('BPOH', "obake_ci.bti")), 0);
    JUTTexture tex2(static_cast<const ResTIMG*>(fn_804032E8('BPOH', "obake_eye.bti")), 0);
    f32 w = tex0.getWidth();
    f32 h = tex0.getHeight();
    JGeometry::TVec3f out;
    fn_802F5040(pos, params, &out);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumChans(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTexGens(1);
    tex0.load(GX_TEXMAP0);
    tex1.load(GX_TEXMAP1);
    tex2.load(GX_TEXMAP2);

    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_KONST, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_TEXA, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevKColorSel(GX_TEVSTAGE0, GX_TEV_KCSEL_K0);
    GXSetTevKAlphaSel(GX_TEVSTAGE0, GX_TEV_KASEL_K0_A);

    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_CPREV, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_KONST, GX_CA_APREV, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevKColorSel(GX_TEVSTAGE1, GX_TEV_KCSEL_K0);
    GXSetTevKAlphaSel(GX_TEVSTAGE1, GX_TEV_KASEL_K0_A);

    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD0, GX_TEXMAP2, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_C1, GX_CC_TEXC, GX_CC_CPREV);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_A1, GX_CA_TEXA, GX_CA_APREV);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevSwapMode(GX_TEVSTAGE2, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevKColorSel(GX_TEVSTAGE2, GX_TEV_KCSEL_K0);
    GXSetTevKAlphaSel(GX_TEVSTAGE2, GX_TEV_KASEL_K0_A);

    GXSetTevOrder(GX_TEVSTAGE3, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE3, GX_CC_ZERO, GX_CC_CPREV, GX_CC_C2, GX_CC_KONST);
    GXSetTevAlphaIn(GX_TEVSTAGE3, GX_CA_ZERO, GX_CA_APREV, GX_CA_A2, GX_CA_KONST);
    GXSetTevColorOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevSwapMode(GX_TEVSTAGE3, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevKColorSel(GX_TEVSTAGE3, GX_TEV_KCSEL_K1);
    GXSetTevKAlphaSel(GX_TEVSTAGE3, GX_TEV_KASEL_K1_A);
    GXSetTevDirect(GX_TEVSTAGE3);
    GXSetNumTevStages(4);

    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetColorUpdate(GX_TRUE);
    GXSetCullMode(GX_CULL_NONE);
    GXSetTevColor(GX_TEVREG0, field_0x2f4[0]);
    GXSetTevKColor(GX_KCOLOR0, field_0x2f4[1]);
    GXSetTevColor(GX_TEVREG1, field_0x2f4[2]);
    if (params != NULL) {
        GXSetTevColor(GX_TEVREG2, params->field_0x10);
        GXSetTevKColor(GX_KCOLOR1, params->field_0x14);
    } else {
        static const GXColor white = {0xFF, 0xFF, 0xFF, 0xFF};
        GXSetTevColor(GX_TEVREG2, white);
        GXSetTevKColor(GX_KCOLOR1, lbl_80543F30);
    }
    out.x -= 0.5f * w;
    out.z -= 0.5f * h;
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(out.x, out.y, out.z + h);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(out.x + w, out.y, out.z + h);
    GXTexCoord2f32(1.0f, 0.0f);
    GXPosition3f32(out.x + w, out.y, out.z);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(out.x, out.y, out.z);
    GXTexCoord2f32(0.0f, 1.0f);
}
