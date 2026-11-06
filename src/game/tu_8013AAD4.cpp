#include "game/GameAudio.h"
#include "game/GameMode.h"
#include "game/Map.h"
#include "game/Effect.h"
#include "game/Camera.h"
#include "game/Message.h"
#include "game/GameState.h"
#include "JSystem/J2D/J2DGrafContext.h"
#include "JSystem/J2D/J2DPicture.h"
#include "JSystem/J2D/J2DPrint.h"
#include "JSystem/J3D/J3DDrawBuffer.h"
#include "JSystem/J3D/J3DModel.h"
#include "JSystem/J3D/J3DSys.h"
#include "JSystem/JUtility/JUTTexture.h"
#include "JSystem/JUtility/JUTFont.h"
#include "JSystem/JUtility/JUTVideo.h"

// The two 640x448 render modes (NTSC interlaced / progressive) that
// fn_8013AAD4 / fn_8013AB00 hand to the video manager.
GXRenderModeObj lbl_8049F090 = {
    VI_TVMODE_NTSC_INT, 608, 448, 448, 27, 16, 666, 448, VI_XFBMODE_DF, 0, 0,
    {{6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}},
    {8, 8, 10, 12, 10, 8, 8},
};

GXRenderModeObj lbl_8049F0CC = {
    VI_TVMODE_NTSC_PROG, 608, 448, 448, 27, 16, 666, 448, VI_XFBMODE_SF, 0, 0,
    {{6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}},
    {0, 0, 21, 22, 21, 0, 0},
};

// .sdata2 word 0x8053CDD8: the white fn_8013B1E8 sets as TEV register 0.
static const GXColor lbl_8053CDD8 = {0xFF, 0xFF, 0xFF, 0xFF};

ScreenRect_802FF3F0* fn_8013C394();

// Not auto-inlined: GameRoot2_8049F108::fn_8013BBE0 calls fn_8013C394 out of line in the original
// (docs/INLINING.md, pragma rule 1).
#pragma auto_inline off
ScreenRect_802FF3F0* fn_8013C394() {
    return &gGameRoot2_8053AB10->field_0x24->field_0x144;
}
#pragma auto_inline reset

// The (s32) casts give the original's signed int-to-float conversion of the u16 sizes.
GameRoot2_8049F108::GameRoot2_8049F108(JKRHeap* heap) {
    heap->getTotalFreeSize();
    gGameRoot2_8053AB10 = this;
    field_0x4 = JKRSolidHeap::create(-1, heap, false);
    field_0x4->becomeCurrentHeap();
    field_0x8 = new (field_0x4, 0) J3DDrawBuffer(0x80);
    field_0xc = new (field_0x4, 0) J3DDrawBuffer(0x80);
    field_0x4->adjustSize();
    heap->becomeCurrentHeap();
    field_0x10 = new (heap, 0) J2DOrthoGraph(0.0f, 0.0f, (f32)(s32)JUTVideo::sManager->getFbWidth(),
                                             (f32)(s32)JUTVideo::sManager->getEfbHeight(), -10000.0f, 10000.0f);
    field_0x14 = new (heap, 0) J2DOrthoGraph(0.0f, 0.0f, (f32)(s32)JUTVideo::sManager->getFbWidth(),
                                             (f32)(s32)JUTVideo::sManager->getEfbHeight(), -10000.0f, 10000.0f);
    JUTFont* font = lbl_8053AD78->mFont;
    field_0x18 = new (heap, 0) J2DPrint(font, 0.0f, font->getHeight());
    field_0x34 = new (heap, 0) JUTTexture(JUTVideo::sManager->getFbWidth(),
                                          JUTVideo::sManager->getEfbHeight(), GX_TF_RGB565);
    field_0x20 = new (heap, 0) TileMap_804AA858(heap);
    field_0x24 = new (heap, 0) Camera_804AA9E4();
    field_0x28 = new (heap, 0) EffectSystem_8049F178(heap);
    field_0x2c = new (heap, 0) ShadowCircle_804A0160(heap);
    field_0x30 = new (heap, 0) cls_804A03A8(heap);
    ResTIMG* black = (ResTIMG*)fn_804030C4('TIMG', "kk_black.bti");
    field_0x1c = new (heap, 0) J2DPicture(black);
    field_0x1c->setBlackWhite(JUtility::TColor(0xFF), JUtility::TColor(0));
}

GameRoot2_8049F108::~GameRoot2_8049F108() {
    delete field_0x1c;
    delete field_0x30;
    delete field_0x2c;
    delete field_0x28;
    delete field_0x24;
    delete field_0x20;
    delete field_0x34;
    delete field_0x18;
    delete field_0x14;
    delete field_0x10;
    field_0x4->destroy();
}

void GameRoot2_8049F108::fn_8013BE04() {
    field_0x20->fn_802FF798();
    field_0x24->fn_8030E5BC();
    field_0x28->fn_8013DDEC();
    field_0x2c->fn_8015945C();
    field_0x30->fn_801601E0();
    field_0x39 = 0;
    field_0x38 = 0;
}

void GameRoot2_8049F108::fn_8013BBE0() {
    if (gGameRoot_8053AAF8->field_0x4c != 9) {
        field_0x24->fn_8030E154();
        field_0x20->fn_802FF568();
        field_0x28->fn_8013DC44();
        field_0x2c->fn_80159330();
        field_0x30->fn_8016005C();
        ScreenRect_802FF3F0* r = fn_8013C394();
        JGeometry::TBox2f box((f32)r->x0, (f32)r->y0, (f32)r->x1, (f32)r->y1);
        field_0x14->place(box);
        field_0x14->scissor(box);
        if (!gGameRoot_8053AAF8->field_0x7f || isGameMode(GAME_MODE_C)) {
            field_0x39 = 0;
        } else if (!isGameModeSet()) {
            field_0x39 = 1;
        } else {
            u8 flag = 0;
            if (field_0x38 >= 0xFF) {
                bool b = true;
                if (gGameRoot_8053AAF8->field_0x4c != GAME_MODE_B && !fn_80239D38()) {
                    b = false;
                }
                if (b) {
                    flag = 1;
                }
            }
            field_0x39 = flag;
        }
    }
}

void GameRoot2_8049F108::fn_8013BBBC() {
    field_0x28->fn_8013DC1C();
}

void GameRoot2_8049F108::fn_8013BB64() {
    field_0x24->fn_8030DFE8(false);
    j3dSys.mDrawBuffer[0] = field_0x8;
    j3dSys.mDrawBuffer[1] = field_0xc;
    PSMTXCopy(field_0x24->field_0x4, j3dSys.mViewMtx);
}

void GameRoot2_8049F108::fn_8013BB18() {
    if (!field_0x39) {
        field_0x20->fn_802FF3F0();
        field_0x24->fn_8030E150();
    }
    field_0x2c->fn_80158E88();
}

void GameRoot2_8049F108::fn_8013BAF4() {
    field_0x28->fn_8013DB2C();
}

void GameRoot2_8049F108::fn_8013B71C() {
    if (field_0x39 == 0 || gGameRoot_8053AAF8->field_0x4c == 4 || gGameRoot_8053AAF8->field_0x4c == 6) {
        gGameRoot2_8053AB10->field_0x24->fn_8030DFE8(false);
        j3dSys.drawInit();
        j3dSys.reinitGX();
        GXSetColorUpdate(GX_TRUE);
        GXSetAlphaUpdate(GX_FALSE);
        j3dSys.mDrawMode = J3DSys::SYSDRAW_Unk3;
        field_0x8->draw();
        j3dSys.reinitGX();
        if (isGameModeSet() && gGameRoot_8053AAF8->field_0x4c != GAME_MODE_C) {
            field_0x20->fn_802FF3B0();
        }
        gGameRoot2_8053AB10->field_0x24->fn_8030DFE8(false);
        j3dSys.drawInit();
        j3dSys.reinitGX();
        GXSetColorUpdate(GX_TRUE);
        GXSetAlphaUpdate(GX_FALSE);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_NOOP);
        j3dSys.mDrawMode = J3DSys::SYSDRAW_Unk4;
        field_0xc->draw();
        j3dSys.reinitGX();
        if (isGameModeSet() && gGameRoot_8053AAF8->field_0x4c != GAME_MODE_C) {
            field_0x20->fn_802FF2D8();
        }
        field_0x28->fn_8013DA10();
        j3dSys.drawInit();
        field_0x2c->fn_80158F70();
    }
    field_0x28->fn_8013D908();
    if (field_0x39 == 0 || gGameRoot_8053AAF8->field_0x4c == 4 || gGameRoot_8053AAF8->field_0x4c == 6) {
        field_0x28->fn_8013D6E4();
        if (isGameModeSet() || gGameRoot_8053AAF8->field_0x4c == GAME_MODE_C) {
            field_0x30->fn_8015F9C0();
        }
    }
}

void GameRoot2_8049F108::fn_8013B618() {
    if (isGameModeSet()) {
        field_0x20->fn_802FF0EC();
    }
    field_0x28->fn_8013D5D4();
    field_0x8->frameInit();
    field_0xc->frameInit();
}

void GameRoot2_8049F108::fn_8013B538() {
    if (isGameModeSet()) {
        field_0x20->fn_802FF0EC();
    }
}

void GameRoot2_8049F108::fn_8013B4F8() {
    field_0x28->fn_8013D5D4();
    field_0x8->frameInit();
    field_0xc->frameInit();
}

void GameRoot2_8049F108::fn_8013B4C0() {
    field_0x28->fn_8013D5B0();
    field_0x20->fn_802FF548();
}

void GameRoot2_8049F108::fn_8013B490() {
    field_0x10->setPort();
}

void GameRoot2_8049F108::fn_8013B460() {
    field_0x14->setPort();
}

void GameRoot2_8049F108::fn_8013B438() {
    field_0x24->fn_8030DFE8(false);
}

void GameRoot2_8049F108::fn_8013B414(const JGeometry::TBox2f* box, bool light) {
    field_0x24->fn_8030DD8C(box, light);
}

void GameRoot2_8049F108::fn_8013B3F0() {
    field_0x24->fn_8030DD68();
}

void GameRoot2_8049F108::fn_8013B3CC(const Mtx m) {
    field_0x24->fn_8030DD20(m);
}

void GameRoot2_8049F108::fn_8013B1E8() {
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetNumChans(1);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    GXSetNumTexGens(1);
    GXSetTevColor(GX_TEVREG0, lbl_8053CDD8);
    GXSetTevColor(GX_TEVREG1, lbl_80543F20);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_C0, GX_CC_C1);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_A1);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevDirect(GX_TEVSTAGE0);
    GXSetNumTevStages(1);
    GXSetNumIndStages(0);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetColorUpdate(GX_TRUE);
    GXSetAlphaUpdate(GX_FALSE);
    GXSetCullMode(GX_CULL_NONE);
    GXSetClipMode(GX_CLIP_DISABLE);
}

void fn_8013B004(const JGeometry::TBox2f* box, const JUtility::TColor* color) {
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetNumChans(1);
    GXSetNumTexGens(0);
    GXSetTevColor(GX_TEVREG0, *color);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevDirect(GX_TEVSTAGE0);
    GXSetNumTevStages(1);
    GXSetNumIndStages(0);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetColorUpdate(GX_TRUE);
    GXSetAlphaUpdate(GX_FALSE);
    GXSetCullMode(GX_CULL_NONE);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition2f32(box->i.x, box->i.y);
    GXPosition2f32(box->f.x, box->i.y);
    GXPosition2f32(box->f.x, box->f.y);
    GXPosition2f32(box->i.x, box->f.y);
}

// FIXME: 99.73. Regalloc only: the original keeps the position x, y, z in
// f3, f4, f5 before adding them to the matrix translation; ours rotates them.
void fn_8013AC90(f32 scale, const Vec* pos, const JUtility::TColor* color, s32 a, s32 b, const Mtx m) {
    Mtx rot;
    rot[0][0] = scale;
    rot[0][1] = 0.0f;
    rot[0][2] = 0.0f;
    rot[0][3] = 0.0f;
    rot[1][0] = 0.0f;
    rot[1][1] = 0.0f;
    rot[1][2] = scale;
    rot[1][3] = 0.0f;
    rot[2][0] = 0.0f;
    rot[2][1] = scale;
    rot[2][2] = 0.0f;
    rot[2][3] = 0.0f;
    f32 x;
    f32 y;
    f32 z;
    if (m != NULL) {
        if (pos->z > 0.0f && a < 1) {
            a++;
            b--;
        }
        f32 off = (f32)((a << 10) + (b << 7));
        Mtx tmp;
        JMath::gekko_ps_copy12(tmp, m);
        tmp[1][3] += off;
        tmp[2][3] += off;
        PSMTXConcat(tmp, rot, rot);
        x = pos->x;
        y = pos->y;
        z = pos->z;
    } else {
        z = pos->z;
        if (z > 0.0f && a < 1) {
            a++;
            b--;
        }
        f32 off = (f32)((a << 10) + (b << 7));
        x = pos->x;
        y = pos->y + off;
        z += off;
    }
    rot[1][0] *= 0.70710677f;
    rot[1][1] *= 0.70710677f;
    rot[1][2] *= 0.70710677f;
    rot[2][0] *= 0.70710677f;
    rot[2][1] *= 0.70710677f;
    rot[2][2] *= 0.70710677f;
    rot[0][3] += x;
    rot[1][3] += y;
    rot[2][3] += z;
    gGameRoot2_8053AB10->field_0x24->fn_8030DFE8(false);
    gGameRoot2_8053AB10->field_0x24->fn_8030DD20(rot);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetChanCtrl(GX_COLOR0A0, GX_TRUE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT0 | GX_LIGHT1, GX_DF_CLAMP, GX_AF_NONE);
    GXSetNumChans(1);
    GXSetNumTexGens(0);
    GXSetTevColor(GX_TEVREG0, *color);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_C0, GX_CC_RASC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevDirect(GX_TEVSTAGE0);
    GXSetNumTevStages(1);
    GXSetNumIndStages(0);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetColorUpdate(GX_TRUE);
    GXSetAlphaUpdate(GX_FALSE);
    GXSetCullMode(GX_CULL_BACK);
    GXDrawSphere1(2);
}

// FIXME: 98.87. Only the two draw-buffer loads: the original loads field_0x8
// into r0 then field_0xc into r5; ours loads field_0xc first into r6.
void fn_8013AC54(Mtx view) {
    J3DDrawBuffer* opa = gGameRoot2_8053AB10->field_0x8;
    J3DDrawBuffer* xlu = gGameRoot2_8053AB10->field_0xc;
    j3dSys.setDrawBuffer(opa, 0);
    j3dSys.setDrawBuffer(xlu, 1);
    j3dSys.setViewMtx(view);
}

void fn_8013ABCC(J3DModel* model, const Vec& scale, Mtx mtx) {
    model->setBaseScale(scale);
    PSMTXCopy(mtx, model->mPosMtx);
    model->calc();
    model->entry();
    model->viewCalc();
}

void fn_8013AB2C() {
    J3DDrawBuffer* opa = gGameRoot2_8053AB10->field_0x8;
    J3DDrawBuffer* xlu = gGameRoot2_8053AB10->field_0xc;
    j3dSys.drawInit();
    j3dSys.reinitGX();
    GXSetColorUpdate(GX_TRUE);
    GXSetAlphaUpdate(GX_FALSE);
    j3dSys.mDrawMode = J3DSys::SYSDRAW_Unk3;
    opa->draw();
    j3dSys.mDrawMode = J3DSys::SYSDRAW_Unk4;
    xlu->draw();
    opa->frameInit();
    xlu->frameInit();
}

void GameRoot2_8049F108::fn_8013AB00() {
    JUTVideo::sManager->setRenderMode(&lbl_8049F0CC);
}

void GameRoot2_8049F108::fn_8013AAD4() {
    JUTVideo::sManager->setRenderMode(&lbl_8049F090);
}
