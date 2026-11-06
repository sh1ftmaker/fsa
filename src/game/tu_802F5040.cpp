#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "dolphin/gx.h"
#include "JSystem/JUtility/JUTTexture.h"
#include "JSystem/JUtility/TColor.h"
#include "JSystem/JKernel/JKRHeap.h"
#include <stdio.h>
#include "JSystem/JKernel/JKRArchive.h"
#include "JSystem/JKernel/JKRDecomp.h"
#include "dolphin/os.h"
#include "game/Camera.h"
#include "game/GameMode.h"
#include "game/Player.h"
#include "JSystem/JUtility/JUTVideo.h"
#include "game/tu_8030AE84.h"

// The Dolphin SDK's indexed vertex writes (GXVert.h's GXPosition1x16 /
// GXColor1x16 / GXTexCoord1x16), which libs/sdk's GXVert.h lacks: added there
// they renumber the @NNN objects of the generator's sinit units, so they stay
// here, the only game user (fn_802FC268).
static inline void GXPosition1x16(const u16 x) { GXWGFifo.u16 = x; }
static inline void GXColor1x16(const u16 x) { GXWGFifo.u16 = x; }
static inline void GXTexCoord1x16(const u16 x) { GXWGFifo.u16 = x; }

// The palettes of fn_802FF798's 16 colour-index textures per layer.
static const GXTlut lbl_8046F940[16] = {
    GX_TLUT0, GX_TLUT1, GX_TLUT2,  GX_TLUT3,  GX_TLUT4,  GX_TLUT5,  GX_TLUT6,  GX_TLUT7,
    GX_TLUT8, GX_TLUT9, GX_TLUT10, GX_TLUT11, GX_TLUT12, GX_TLUT13, GX_TLUT14, GX_TLUT15,
};

// The TEV register colours of the sprite and tile GX setups (the user of each
// in the comment). File-scope objects: they come before the unit's literal
// pool in .sdata2 (a function's own constants are pooled in first-use order,
// after the floats of fn_802F5040), and the two white ones are separate.
static const GXColor lbl_805414E8 = {0xFF, 0xFF, 0xFF, 0xFF}; // fn_802FC80C, TEVREG2
static const GXColor lbl_805414EC = {0x00, 0x00, 0x00, 0xFF}; // fn_802FB69C, TEVREG0
static const GXColor lbl_805414F0 = {0x00, 0x00, 0x00, 0x7E}; // fn_802FAC10, TEVREG0
static const GXColor lbl_805414F4 = {0xFF, 0xFF, 0xFF, 0xFF}; // fn_802F8F0C, TEVREG0

// The draw offset of a sprite with params at pos: field_0x1c / field_0x1d
// in 0x400 / 0x80 units, one step up (0x1c + 1, 0x1d - 1) for a raised sprite
// (the map's field_0xe461, params' field_0x20, above the ground) below step 1.
// fn_802F5040 and fn_802F86CC inline it; both read the map through
// gGameRoot2_8053AB10, also in the member.
static inline f32 getDrawOffset(const SpriteParams_801F5AE0* params, const JGeometry::TVec3f* pos) {
    u8 raised = gGameRoot2_8053AB10->field_0x20->field_0xe461;
    if (raised) {
        raised = params->field_0x20;
    }
    if (raised) {
        raised = pos->z > 0.0f;
    }
    s32 a = params->field_0x1c;
    s32 b = params->field_0x1d;
    if (raised && a < 1) {
        a++;
        b--;
    }
    return (a << 10) + (b << 7);
}

// One GBA OAM entry (attributes 0-2 and the affine halfword) as fn_802F86CC
// unpacks the 6-byte little-endian entries of a "SOB " frame; the bit fields
// in the GBA's order (big-endian bit fields, so the first is the high bits).
struct SpriteObj_802F86CC {
    /* 0x0 */ u16 mY : 8;
    u16 mAffine : 2;
    u16 mMode : 2;
    u16 mMosaic : 1;
    u16 mColor256 : 1;
    u16 mShape : 2;
    /* 0x2 */ u16 mX : 9;
    u16 mAffineIndex : 3;
    u16 mHFlip : 1;
    u16 mVFlip : 1;
    u16 mSize : 2;
    /* 0x4 */ u16 mTile : 10;
    u16 mPriority : 2;
    u16 mPalette : 4;
    /* 0x6 */ u16 mAffineParam;
}; // Size: 0x8

// A 6-byte entry of a "SOB " frame: the GBA OAM attributes 0-2 as stored
// (little-endian halfwords, so each byte's fields from the high bits down).
struct SobObj_802F86CC {
    /* 0x0 */ u8 mY;
    /* 0x1 */ u8 mShape : 2;
    u8 mColor256 : 1;
    u8 mMosaic : 1;
    u8 mMode : 2;
    u8 mAffine : 2;
    /* 0x2 */ u8 mX;
    /* 0x3 */ u8 mSize : 2;
    u8 mVFlip : 1;
    u8 mHFlip : 1;
    u8 mAffineIndex : 3;
    u8 : 1; // bit 8 of x: copied from mX's bit 7 instead (sign extension)
    /* 0x4 */ u8 mTileLow;
    /* 0x5 */ u8 mPalette : 4;
    u8 mPriority : 2;
    u8 mTileHigh : 2;
}; // Size: 0x6

// One 5-bit colour channel times scale plus offset, clamped to 0-31
// (fn_802F93F4's channel adjustment, inlined for r, g and b).
// scale / offset are parameters (callers pass 1.0f, 0.0f): the original keeps the
// fmadds, which constants in the body would fold away.
static inline u8 adjustChannel(s32 value, f32 scale, f32 offset) {
    s32 result = value * scale + offset;
    if (result < 0) {
        result = 0;
    } else if (result > 31) {
        result = 31;
    }
    return result;
}

// One shade of fn_802FA3FC's stage tables (0x28 bytes): drawn on every tile
// whose code is mTile, at the tile's corner + (mX, mY): kind 0 the
// parallelogram of 96 * mU and 96 * mV from there, kind 1 the quad of
// 24 * mV either side stretched by 64 * mU, kind 2 that plus one stretched by
// -4 * mU. mMore: the next entry is for the same tile.
struct TileShade_802FA3FC {
    /* 0x00 */ u16 mTile;
    /* 0x02 */ u16 field_0x2[4]; // two more tile codes in some entries (0x36A / 0x37B, 0x36C / 0x37D); not read here
    /* 0x0A */ u8 mMore;
    /* 0x0B */ u8 mAlpha;
    /* 0x0C */ f32 mX;
    /* 0x10 */ f32 mY;
    /* 0x14 */ s32 mKind;
    /* 0x18 */ f32 mU[2];
    /* 0x20 */ f32 mV[2];
}; // Size: 0x28

// Forward declarations.
void fn_802F5040(const JGeometry::TVec3f* pos, SpriteParams_801F5AE0* params, JGeometry::TVec3f* out);

cls_803004C0::cls_803004C0() {}

cls_803003AC::cls_803003AC() {}

cls_803003A8::cls_803003A8() {}

cls_8030029C::cls_8030029C() {}

// FIXME: 99.9. Instructions identical; the two file names are @NNN strings, not
// lbl_804711D4 / lbl_804711E4, until the unit's .rodata is emitted in order.
TileMap_804AA858::TileMap_804AA858(JKRHeap* heap) {
    heap->getTotalFreeSize();
    for (s32 i = 0; i < 2; i++) {
        field_0x4[i] = new (heap, 0x20) u8[0x8000];
    }
    for (s32 i = 0; i < 3; i++) {
        field_0xc[i] = new (heap, 0x20) u8[0x200];
        field_0x18[i] = new (heap, 0x20) u8[0x200];
    }
    field_0x24 = new (heap, 0x20) u16[0x1000];
    for (s32 i = 0; i < 4; i++) {
        mLayerGrid[i] = new (heap, 0x20) u16[0x1000];
        mLayerGrid2[i] = new (heap, 0x20) u16[0x1000];
    }
    for (s32 i = 0; i < 16; i++) {
        field_0x48[i] = new (heap, 0x20) u16[0x1000];
    }
    for (s32 i = 0; i < 4; i++) {
        field_0x88[i] = new (heap, 0x20) u8[0x8000];
    }
    for (s32 i = 0; i < 12; i++) {
        field_0x98[i] = new (heap, 0x20) u8[0x200];
        field_0xc8[i] = new (heap, 0x20) u8[0x200];
    }
    field_0xf8 = new (heap, 0x20) u8[0x8000];
    for (s32 i = 0; i < 2; i++) {
        field_0x100[i] = new (heap, 0) JGeometry::TVec3f[65][65];
        field_0x108[i] = new (heap, 0) JUtility::TColor[65 * 65];
    }
    field_0x110 = new (heap, 0) JGeometry::TVec2f[0x1000];
    for (s32 i = 0; i < 4; i++) {
        field_0x11c[i] = new (heap, 0) cls_804AA858Sub11C;
    }
    for (s32 i = 0; i < 10; i++) {
        char name[32];
        sprintf(name, "filter%d.bti", i == 0 ? 1 : i);
        field_0xe428[i] = new (heap, 0) JUTTexture(
            (ResTIMG*)gGameRoot_8053AAF8->field_0xa4->getResource('????', name), 0);
    }
    field_0xe454 = new (heap, 0) JUTTexture(
        (ResTIMG*)gGameRoot_8053AAF8->field_0xa4->getResource('????', "circle2.bti"), 0);
    field_0xfc = new (heap, 0) cls_804B1668();
    field_0x238c = new (heap, 0x20) u8[0x100];
}

// Frees what the constructor allocated, in reverse order. Only the first four
// field_0x98 / field_0xc8 buffers are freed (in the loop of field_0x88).
TileMap_804AA858::~TileMap_804AA858() {
    delete field_0xe454;
    for (s32 i = 9; i >= 0; i--) {
        delete field_0xe428[i];
    }
    for (s32 i = 3; i >= 0; i--) {
        delete field_0x11c[i];
    }
    delete[] field_0x110;
    for (s32 i = 1; i >= 0; i--) {
        delete[] field_0x108[i];
        delete[] field_0x100[i];
    }
    delete[] field_0xf8;
    for (s32 i = 3; i >= 0; i--) {
        delete[] field_0xc8[i];
        delete[] field_0x98[i];
        delete[] field_0x88[i];
    }
    for (s32 i = 15; i >= 0; i--) {
        delete[] field_0x48[i];
    }
    for (s32 i = 3; i >= 0; i--) {
        delete[] mLayerGrid2[i];
        delete[] mLayerGrid[i];
    }
    delete[] field_0x24;
    for (s32 i = 2; i >= 0; i--) {
        delete[] field_0x18[i];
        delete[] field_0xc[i];
    }
    for (s32 i = 1; i >= 0; i--) {
        delete[] field_0x4[i];
    }
    delete field_0xfc;
    field_0xfc = NULL;
    delete field_0x238c;
}

// FIXME: 99.4. Register allocation in the per-player loop (r4-r8 one off); the
// grid's .sdata2 literals (lbl_80541528 ... lbl_80541614) are @NNN because the
// unwritten functions between fn_802F9DA8 and it use earlier pool entries first.
void TileMap_804AA858::fn_802FF798() {
    for (s32 i = 0; i < 4; i++) {
        field_0x235c[i] = 0;
        field_0x2360[i] = 0;
    }
    field_0x2364 = 0;
    field_0x2365 = 1;
    field_0x2368.x = field_0x2368.y = field_0x2368.z = 0.0f;
    field_0x2374.x = field_0x2374.y = field_0x2374.z = 0.0f;
    field_0x2380 = 0x40;
    field_0x2384 = 0x40;
    for (int i = 0; i < 0x100; i++) {
        field_0x2398[i].mTimer = 0;
    }
    field_0xbf98 = 0;
    field_0x2388 = 0;
    for (int i = 0; i < 16; i++) {
        field_0xbf9c[i].mKind = 0;
    }
    for (s32 y = 0; y < 0x40; y++) {
        f32 v = 0.001953125f * (8.0f * y + 0.501f);
        for (s32 x = 0; x < 0x10; x++) {
            s32 index = y * 16 + x;
            field_0x110[index].x = 0.0078125f * (8.0f * x + 0.501f);
            field_0x110[index].y = v;
            field_0x110[index + 0x400].x = 0.05453125f + field_0x110[index].x;
            field_0x110[index + 0x400].y = field_0x110[index].y;
            field_0x110[index + 0x800].x = field_0x110[index].x;
            field_0x110[index + 0x800].y = 0.013632813f + field_0x110[index].y;
            field_0x110[index + 0xC00].x = 0.05453125f + field_0x110[index].x;
            field_0x110[index + 0xC00].y = 0.013632813f + field_0x110[index].y;
        }
    }
    field_0xc21c = 0;
    field_0xd320 = 0;
    field_0xe424 = 0;
    fn_802FDDD4();
    for (s32 i = 0; i < 4; i++) {
        field_0x12c[i] = 0;
        field_0x138[i] = 0;
        field_0x148[i] = 0;
        field_0x14c[i] = 0;
        field_0x210[i] = 0;
        s32 y = (i & 2) ? JUTVideo::getManager()->getEfbHeight() - 0x90 : 0x48;
        s32 x = (i & 1) ? JUTVideo::getManager()->getFbWidth() - 0x60 : 0x18;
        field_0x1d0[i].field_0x0 = x;
        field_0x1d0[i].field_0x4 = y;
        field_0x1d0[i].field_0x8 = 0x48;
        field_0x1d0[i].field_0xc = 0x48;
        field_0x217[i] = 0;
    }
    field_0x216 = 0;
    field_0x214 = 0;
    field_0x215 = 0;
    field_0x22c = -1;
    field_0x230 = -1;
    field_0x234 = 0;
    field_0x248 = 0;
    field_0xe450 = 1;
    field_0xe451 = 1;
    field_0xe458 = 1.8f;
    field_0xe45c = -0.25f;
    field_0x2390 = 0;
    field_0x2394 = 0;
    field_0xe461 = 1;
    for (s32 i = 0; i < 3; i++) {
        for (s32 j = 0; j < 4; j++) {
            for (s32 k = 0; k < 16; k++) {
                GXInitTlutObj(&field_0x25c[i][j][k], field_0xc8[i * 4 + j] + k * 0x20, GX_TL_RGB5A3, 16);
                GXInitTexObjCI(&field_0xb5c[i][j][k], field_0x88[j], 0x100, 0x100, GX_TF_C4, GX_CLAMP,
                               GX_CLAMP, GX_FALSE, lbl_8046F940[k]);
            }
        }
    }
}

void TileMap_804AA858::fn_802FF568() {
    fn_802FDDD4();
    if (isGameModeSet() == true) {
        fn_802FCE74();
    }
    for (s32 i = 0; i < 0x100; i++) {
        if (field_0x2398[i].mTimer > 0) {
            field_0x2398[i].mTimer--;
        }
    }
    field_0xbf98 = 1;
    field_0xe461 = 1;
    if (gGameRoot_8053AAF8->field_0x7f == 0) {
        for (s32 i = 0; i < 4; i++) {
            if (fn_8023F264(i)) {
                // tu_80441A1C reads these 2-bit per-player fields the same way.
                u32 mode = (gGameRoot_8053AAF8->field_0x83 >> (i * 2)) & 3;
                if (mode == 2) {
                    field_0xe461 = 0;
                    break;
                }
            }
        }
    }
}

void TileMap_804AA858::fn_802FF548() {
    fn_802FF798();
}

// FIXME: 88.7. The original copies the rectangles two words at a time (lwz, lwz,
// stw, stw) and builds the full-screen one on the stack while storing it; ours copies
// word by word and drops the stack copy. The loop registers swap as in fn_802FF2D8.
void TileMap_804AA858::fn_802FF3F0() {
    if (field_0x248 && field_0x24c.x1 != 0) {
        gGameRoot2_8053AB10->field_0x24->field_0x144 = field_0x24c;
    } else {
        ScreenRect_802FF3F0 rect;
        rect.x0 = 0;
        rect.y0 = 0;
        rect.x1 = JUTVideo::getManager()->getFbWidth();
        rect.y1 = JUTVideo::getManager()->getEfbHeight();
        gGameRoot2_8053AB10->field_0x24->field_0x144 = rect;
    }
    fn_802FC80C();
    for (s32 i = 0; i < field_0xd320; i++) {
        cls_8030029C* sprite = &field_0xc220[i];
        if (sprite->mRes != NULL) {
            fn_802F86CC(&sprite->mPos, sprite->mRes, sprite->mFrame, &sprite->mParams, 1.0f, 1.0f);
        } else if (sprite->mImage != NULL) {
            fn_802F745C(&sprite->mPos, sprite->mImage, &sprite->mParams);
        } else if (sprite->field_0x18 != NULL) {
            fn_802F6CF4(&sprite->mPos, sprite->field_0x18, &sprite->mParams);
        }
    }
    field_0xd320 = 0;
}

void TileMap_804AA858::fn_802FF3B0() {
    if (field_0xe451 != 0 && gGameRoot_8053AAF8->field_0x4 != NULL) {
        fn_802FA3FC();
    }
}

// FIXME: 95.6. The loop index and the element pointer swap r29/r30 (and the
// stw r29 save moves up one slot). Literals match.
void TileMap_804AA858::fn_802FF2D8() {
    for (s32 i = 0; i < field_0xe424; i++) {
        cls_8030029C* sprite = &field_0xd324[i];
        if (sprite->mRes != NULL) {
            fn_802F86CC(&sprite->mPos, sprite->mRes, sprite->mFrame, &sprite->mParams, 1.0f, 1.0f);
        } else if (sprite->mImage != NULL) {
            fn_802F745C(&sprite->mPos, sprite->mImage, &sprite->mParams);
        } else if (sprite->field_0x18 != NULL) {
            fn_802F6CF4(&sprite->mPos, sprite->field_0x18, &sprite->mParams);
        }
    }
    field_0xe424 = 0;
    fn_802FEE68();
}

// FIXME: 92.1. Scheduling and registers only: the original keeps i, the flag,
// the lbl_8053ACF0 pointer and the three per-player pointers in r22-r29 (ours
// r24-r31, one pointer induction fewer) and loads field_0x210[i] before
// field_0x11c[i] for the fn_802FB69C call.
// Draws the panels of the players whose field_0x148 byte is set (not in
// screen mode 12), each after field_0xfc's window when it belongs to that
// player (field_0x230, -1 for all); a player in state 1 of fn_80301D30
// passes a count, one in state 2 fades in by its lbl_8053ACF0 counter.
void TileMap_804AA858::fn_802FF0EC() {
    bool window = false;
    for (s32 i = 0; i < 4; i++) {
        if (field_0x148[i] == 0 || gGameRoot_8053AAF8->field_0x4c == 12) {
            continue;
        }
        if (field_0x214) {
            s32 owner = field_0x230;
            if (i == owner || owner == -1) {
                field_0xfc->fn_80432750(owner == -1, field_0x21c, field_0x220, field_0x224, field_0x228);
                window = true;
            }
        }
        s32 count = 0;
        s32 alpha = 0xFF;
        if (fn_80301D30(i) == 1) {
            count = lbl_8053ACF0[i] / 2;
        } else if (fn_80301D30(i) == 2) {
            if (lbl_8053AD30->fn_8030A364(getPlayerInputSlot_8030AAF4(i)) || lbl_80506D88[i] == lbl_80506D98[i]) {
                alpha = (0x10 - lbl_8053ACF0[i]) << 4;
                if (alpha > 0xFF) {
                    alpha = 0xFF;
                }
            }
        }
        fn_802FB69C(i, field_0x11c[i], &field_0x150[i], &field_0x190[i], field_0x210[i], count, alpha);
    }
    if (window) {
        field_0xfc->fn_80432638();
    }
    field_0xbf98 = 0;
}

// Draws the field_0x2398 effects, last first: params' alphas fade with the
// time left, and an effect with a scale shrinks from it to 1 (its matrix is
// a scaled copy for the draw).
void TileMap_804AA858::fn_802FEE68() {
    for (int i = 0xFF; i >= 0; i--) {
        cls_803003AC* effect = &field_0x2398[i];
        if (effect->mTimer > 0) {
            f32 t = (f32)effect->mTimer / (f32)effect->mDuration;
            effect->mParams.field_0x10.r = effect->mColor.r;
            effect->mParams.field_0x10.g = effect->mColor.g;
            effect->mParams.field_0x10.b = effect->mColor.b;
            effect->mParams.field_0x10.a = effect->mColor.a * t;
            effect->mParams.field_0x14.r = effect->mAddColor.r;
            effect->mParams.field_0x14.g = effect->mAddColor.g;
            effect->mParams.field_0x14.b = effect->mAddColor.b;
            effect->mParams.field_0x14.a = effect->mAddColor.a * t;
            MtxP mtx;
            Mtx m;
            if (effect->mScale != 1.0f) {
                JMath::gekko_ps_copy12(m, effect->mParams.mMtx);
                mtx = effect->mParams.mMtx;
                f32 scale = effect->mScale - (effect->mScale - 1.0f) * t;
                m[0][0] *= scale;
                m[1][0] *= scale;
                m[2][0] *= scale;
                m[0][1] *= scale;
                m[1][1] *= scale;
                m[2][1] *= scale;
                m[0][2] *= scale;
                m[1][2] *= scale;
                m[2][2] *= scale;
                m[0][3] *= scale;
                m[1][3] *= scale;
                m[2][3] *= scale;
                effect->mParams.mMtx = m;
            }
            if (effect->mRes != NULL) {
                fn_802F86CC(&effect->mPos, effect->mRes, effect->mFrame, &effect->mParams, 1.0f, 1.0f);
            } else if (effect->mImage != NULL) {
                fn_802F745C(&effect->mPos, effect->mImage, &effect->mParams);
            } else if (effect->field_0x24 != NULL) {
                fn_802F6CF4(&effect->mPos, effect->field_0x24, &effect->mParams);
            }
            if (effect->mScale != 1.0f) {
                effect->mParams.mMtx = mtx;
            }
        }
    }
}

// Draws the tile layers: indexed f32 positions / RGBA8 colours of the
// field_0x2388 height grid and the texture coordinates of field_0x110, the
// tile texture times the vertex colour; with field_0xe450 set, three more TEV
// stages put the player's field_0xe428 texture over it. Then each layer whose
// field_0x235c / field_0x2360 byte is set (layers 0-3 at field_0x2368, 4-7 at
// field_0x2374), and with field_0x2365 layer 0 a second time as pass 8.
void TileMap_804AA858::fn_802FC80C() {
    gGameRoot2_8053AB10->fn_8013B1E8();
    gGameRoot2_8053AB10->fn_8013B438();
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
    GXSetVtxDesc(GX_VA_CLR0, GX_INDEX16);
    GXSetVtxDesc(GX_VA_TEX0, GX_INDEX16);
    GXSetVtxDesc(GX_VA_TEX1, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGB, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_TEX_ST, GX_F32, 0);
    GXSetArray(GX_VA_POS, field_0x100[field_0x2388], sizeof(JGeometry::TVec3f));
    GXSetArray(GX_VA_CLR0, field_0x108[field_0x2388], sizeof(JUtility::TColor));
    GXSetArray(GX_VA_TEX0, field_0x110, sizeof(JGeometry::TVec2f));
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumChans(1);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTexGens(2);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ONE, GX_CC_RASC, GX_CC_TEXC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_KONST, GX_CA_RASA, GX_CA_TEXA);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_SUBHALF, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_SUBHALF, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevDirect(GX_TEVSTAGE0);
    if (field_0xe450) {
        s32 player;
        if (isGameModeSet() && gGameRoot_8053AAF8->field_0x4 != NULL) {
            player = getGameView_802D6A9C()->field_0x50;
        } else {
            player = 0;
        }
        field_0xe428[player]->load(GX_TEXMAP1);
        GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
        GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_CPREV, GX_CC_TEXC, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
        GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG0);
        GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP0);
        GXSetTevDirect(GX_TEVSTAGE1);
        GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
        GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_ONE, GX_CC_C0, GX_CC_CPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
        GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevSwapMode(GX_TEVSTAGE2, GX_TEV_SWAP0, GX_TEV_SWAP0);
        GXSetTevDirect(GX_TEVSTAGE2);
        GXSetTevColor(GX_TEVREG1, lbl_805414E8);
        GXSetTevOrder(GX_TEVSTAGE3, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
        GXSetTevColorIn(GX_TEVSTAGE3, GX_CC_A1, GX_CC_APREV, GX_CC_CPREV, GX_CC_C0);
        GXSetTevAlphaIn(GX_TEVSTAGE3, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
        GXSetTevColorOp(GX_TEVSTAGE3, GX_TEV_COMP_R8_GT, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevSwapMode(GX_TEVSTAGE3, GX_TEV_SWAP0, GX_TEV_SWAP0);
        GXSetTevDirect(GX_TEVSTAGE3);
        GXSetNumTevStages(4);
    } else {
        GXSetNumTevStages(1);
    }
    GXSetNumIndStages(0);
    GXInvalidateVtxCache();
    GXSetCullMode(GX_CULL_NONE);
    for (s32 i = 0; i <= 8; i++) {
        if (i < 4 && field_0x235c[i]) {
            if (i == 0 && field_0x2365) {
                GXSetAlphaCompare(GX_EQUAL, 0xFF, GX_AOP_OR, GX_NEVER, 0);
                GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
            } else {
                GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_AND, GX_ALWAYS, 0);
                GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
            }
            fn_802FC268(&field_0x2368, i, mLayerGrid[i]);
        } else if (i >= 4 && i < 8 && field_0x2360[i - 4]) {
            GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_AND, GX_ALWAYS, 0);
            GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
            fn_802FC268(&field_0x2374, i - 4, mLayerGrid2[i - 4]);
        } else if (field_0x2365 && i == 8 && field_0x235c[0]) {
            GXSetAlphaCompare(GX_LESS, 0xFF, GX_AOP_AND, GX_GREATER, 0);
            GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
            fn_802FC268(&field_0x2368, 0, mLayerGrid[0]);
        }
    }
}

// FIXME: 87.0. Structurally close (2 instructions more): the original builds
// the four vertex indices as a chain of u16 truncations (clrlwi after +1, +65,
// -1; ours folds them into offsets from one clrlwi), keeps the draw loop's
// block counters in different stack slots / registers (one more spilled), and
// orders the count loop's four field_0x24 loads differently. Literals and the
// .rodata table (lbl_8046F9F0$NNN) are reloc-name.
// Draws the tile grid `grid` of `layer` (2 x 2 blocks of 16 x 16 tiles, up to
// the field_0x2380 x field_0x2384 cells of the height grid) at origin, raised
// by (layer - 0.5) * 1024 in y and z, one palette at a time (field_0x18[0]):
// each tile's 2 x 2 entries of field_0x24 are quads on the 65-wide height grid
// vertices (position and colour indices), with field_0x110's corner texture
// coordinates (the entry's tile and flips) and the screen position / 128.
// fn_802FC80C sets up the vertex format.
void TileMap_804AA858::fn_802FC268(const JGeometry::TVec3f* origin, s32 layer, u16* grid) {
    static const GXTlut lbl_8046F9F0[16] = {
        GX_TLUT0, GX_TLUT1, GX_TLUT2,  GX_TLUT3,  GX_TLUT4,  GX_TLUT5,  GX_TLUT6,  GX_TLUT7,
        GX_TLUT8, GX_TLUT9, GX_TLUT10, GX_TLUT11, GX_TLUT12, GX_TLUT13, GX_TLUT14, GX_TLUT15,
    };
    JGeometry::TVec3f pos(*origin);
    f32 offset = 1024.0f * (layer - 0.5f);
    pos.y += offset;
    pos.z += offset;
    Mtx m;
    PSMTXTrans(m, pos.x, pos.y, pos.z);
    gGameRoot2_8053AB10->fn_8013B3CC(m);
    GXTlutObj tluts[16];
    for (int i = 0; i < 16; i++) {
        GXInitTlutObj(&tluts[i], field_0x18[0] + i * 0x20, GX_TL_RGB5A3, 16);
        GXLoadTlut(&tluts[i], lbl_8046F9F0[i]);
    }
    s32 counts[16];
    for (int i = 0; i < 16; i++) {
        counts[i] = 0;
    }
    for (int by = 0; by < 2; by++) {
        for (int bx = 0; bx < 2; bx++) {
            s32 block = by * 2 + bx;
            for (int y = 0; y < 16 && by * 16 + y < field_0x2384 / 2; y++) {
                for (int x = 0; x < 16 && bx * 16 + x < field_0x2380 / 2; x++) {
                    u16 tile = grid[(block * 16 + y) * 16 + x];
                    if (tile != 0) {
                        for (int k = 0; k < 4; k++) {
                            counts[(field_0x24[tile * 4 + k] >> 12) & 0xF]++;
                        }
                    }
                }
            }
        }
    }
    GXTexObj tex;
    for (int palette = 0; palette < 16; palette++) {
        if (counts[palette] <= 0) {
            continue;
        }
        GXInitTexObjCI(&tex, field_0x4[0], 0x80, 0x200, GX_TF_C4, GX_CLAMP, GX_CLAMP, GX_FALSE,
                       lbl_8046F9F0[palette]);
        GXLoadTexObj(&tex, GX_TEXMAP0);
        GXBegin(GX_QUADS, GX_VTXFMT0, counts[palette] * 4);
        for (int by = 0; by < 2; by++) {
            for (int bx = 0; bx < 2; bx++) {
                s32 block = by * 2 + bx;
                for (int y = 0; y < 16 && by * 16 + y < field_0x2384 / 2; y++) {
                    for (int x = 0; x < 16 && bx * 16 + x < field_0x2380 / 2; x++) {
                        u16 tile = grid[(block * 16 + y) * 16 + x];
                        if (tile == 0) {
                            continue;
                        }
                        for (int j = 0; j < 2; j++) {
                            for (int i = 0; i < 2; i++) {
                                u16 entry = field_0x24[tile * 4 + j * 2 + i];
                                if (((entry >> 12) & 0xF) != palette) {
                                    continue;
                                }
                                s32 cx = bx * 32 + x * 2 + i;
                                s32 cy = by * 32 + y * 2 + j;
                                u16 corner = entry & 0xFFF;
                                f32 s = (origin->x + cx * 8) * 0.0078125f;
                                f32 t = (origin->y + cy * 8) * 0.0078125f;
                                u16 vertex = cx + cy * 65;
                                GXPosition1x16(vertex);
                                GXColor1x16(vertex);
                                GXTexCoord1x16(corner);
                                GXTexCoord2f32(s, t);
                                vertex++;
                                GXPosition1x16(vertex);
                                GXColor1x16(vertex);
                                GXTexCoord1x16(corner ^ 0x400);
                                GXTexCoord2f32(0.0625f + s, t);
                                vertex += 65;
                                GXPosition1x16(vertex);
                                GXColor1x16(vertex);
                                GXTexCoord1x16(corner ^ 0xC00);
                                GXTexCoord2f32(0.0625f + s, 0.0625f + t);
                                vertex--;
                                GXPosition1x16(vertex);
                                GXColor1x16(vertex);
                                GXTexCoord1x16(corner ^ 0x800);
                                GXTexCoord2f32(s, 0.0625f + t);
                            }
                        }
                    }
                }
            }
        }
        GXEnd();
    }
}

// FIXME: 89.1. Structurally close (6 instructions fewer): in the mosaic the
// original loads rect2's four fields once before the loops (and multiplies per
// pixel), ours reloads them per pixel (as locals they get strength-reduced and
// flag / alpha spill); both mode-11 tests are a byte in the original (subfic /
// cntlzw / extrwi., the unsolved shape of tu_80285E58's fn_80286688); the first
// tint stores its colour bytes before its box; mode goes through r0; stack
// slots differ; the literals are reloc-name.
// One player's panel (fn_802FF0EC): the ortho box rect and viewport / scissor
// rect2; with flag the record's tile layer mValid - 1 (on its palette set's
// first colour, field_0x114) and, when the overlay flag (mEntries[3].mScrollBit) is set and the pause screen
// (lbl_8053AEC8's alpha) is not fully up, layer mValid + 7 too, tinted by
// field_0x130[player] (1 half grey, 2 black at 0xC0) and with the player's
// line shifts (cls_804AA8D8::fn_8030B1E8); without flag a black quad of
// field_0xe454. While paused a black box at the pause screen's alpha; count:
// a mosaic of the panel (points of count + 1 pixels from the EFB); alpha: a
// black box at 0xFF - alpha.
void TileMap_804AA858::fn_802FB69C(s32 player, cls_804AA858Sub11C* record, cls_803004C0* rect,
                                   cls_803004C0* rect2, u8 flag, s32 count, u8 alpha) {
    JGeometry::TBox2f box(rect->field_0x0, rect->field_0x4, rect->field_0x0 + rect->field_0x8,
                          rect->field_0x4 + rect->field_0xc);
    gGameRoot2_8053AB10->fn_8013B414(&box, false);
    GXSetViewport(rect2->field_0x0, rect2->field_0x4, rect2->field_0x8, rect2->field_0xc, 0.0f, 1.0f);
    GXSetScissor(rect2->field_0x0, rect2->field_0x4, rect2->field_0x8, rect2->field_0xc);
    s32 scrollX = record->mEntries[0].mScroll << 5 | record->mEntries[0].mScrollBit << 4 | record->mEntries[1].mScroll;
    s32 scrollY = record->mEntries[2].mScroll << 5 | record->mEntries[2].mScrollBit << 4 | record->mEntries[3].mScroll;
    s32* offsets = NULL;
    cls_804AA8D8* shifts = fn_8022BCD4(player);
    if (flag) {
        bool first = ((record->mValid - 1) & 7) == 0 || field_0x134[player] != 0;
        u16 c = field_0x114[first ? 1 : 2];
        gGameRoot2_8053AB10->fn_8013B3F0();
        JGeometry::TBox2f fill(rect->field_0x0, rect->field_0x4, rect->field_0x0 + rect->field_0x8,
                               rect->field_0x4 + rect->field_0xc);
        JUtility::TColor color((c & 0x7C00) >> 7 | (c & 0x7C00) >> 12, (c & 0x3E0) >> 2 | (c & 0x3E0) >> 7,
                               (c & 0x1F) << 3 | (c & 0x1F) >> 2, 0xFF);
        fn_8013B004(&fill, &color);
        if (record->mEntries[3].mScrollBit) {
            u8 paused = gGameRoot_8053AAF8->field_0x4c == 11;
            if (!(paused || gGameRoot_8053AAF8->field_0x88 != 0) ||
                (u8)(lbl_8053AEC8 != NULL ? lbl_8053AEC8->field_0x5c : 0) < 0xFF) {
                u8 mode = 6;
                if (record->mValid <= 1 || field_0x134[player] != 0) {
                    mode = 5;
                } else if (record->mMode == 3) {
                    mode = record->mOffsetMode;
                }
                if (shifts != NULL) {
                    offsets = shifts->fn_8030B1E8(record->mValid, mode, scrollY);
                }
                fn_802FAC10(record->mValid - 1, scrollX, scrollY, field_0x134[player], 0xFF, 0xFF, offsets);
            }
        }
    } else {
        gGameRoot2_8053AB10->fn_8013B1E8();
        field_0xe454->load(GX_TEXMAP0);
        GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
        GXSetColorUpdate(GX_FALSE);
        GXSetAlphaUpdate(GX_TRUE);
        GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ZERO, GX_LO_NOOP);
        GXSetTevColor(GX_TEVREG0, lbl_805414EC);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        s32 left = rect->field_0x0;
        s32 top = rect->field_0x4;
        s32 right = left + rect->field_0x8;
        s32 bottom = top + rect->field_0xc;
        GXPosition3f32(left, top, 0.0f);
        GXTexCoord2f32(-1.0f, -1.0f);
        GXPosition3f32(right, top, 0.0f);
        GXTexCoord2f32(1.0f, -1.0f);
        GXPosition3f32(right, bottom, 0.0f);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3f32(left, bottom, 0.0f);
        GXTexCoord2f32(-1.0f, 1.0f);
        GXEnd();
        GXSetColorUpdate(GX_TRUE);
        GXSetAlphaUpdate(GX_FALSE);
        GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_AND, GX_ALWAYS, 0);
    }
    fn_8030275C(record, player, flag, field_0x134[player] != 0);
    if (flag) {
        gGameRoot2_8053AB10->fn_8013B3F0();
        if (record->mEntries[3].mScrollBit) {
            u8 paused = gGameRoot_8053AAF8->field_0x4c == 11;
            if (!(paused || gGameRoot_8053AAF8->field_0x88 != 0) ||
                (u8)(lbl_8053AEC8 != NULL ? lbl_8053AEC8->field_0x5c : 0) < 0xFF) {
                if (field_0x130[player] == 1) {
                    fn_802FAC10(record->mValid + 7, scrollX, scrollY, field_0x134[player], 0x80, 0xFF, offsets);
                } else if (field_0x130[player] == 2) {
                    fn_802FAC10(record->mValid + 7, scrollX, scrollY, field_0x134[player], 0, 0xC0, offsets);
                } else {
                    fn_802FAC10(record->mValid + 7, scrollX, scrollY, field_0x134[player], 0xFF, 0xFF, offsets);
                }
            }
        }
        bool paused = gGameRoot_8053AAF8->field_0x4c == 11 ? true : gGameRoot_8053AAF8->field_0x88 == 2;
        if (paused) {
            gGameRoot2_8053AB10->fn_8013B3F0();
            JGeometry::TBox2f fill(rect->field_0x0, rect->field_0x4, rect->field_0x0 + rect->field_0x8,
                                   rect->field_0x4 + rect->field_0xc);
            JUtility::TColor color(0, 0, 0, lbl_8053AEC8 != NULL ? lbl_8053AEC8->field_0x5c : 0);
            fn_8013B004(&fill, &color);
        }
    }
    fn_803022C4(record, player, flag, field_0x134[player] != 0);
    if (count != 0) {
        s32 step = count + 1;
        gGameRoot2_8053AB10->fn_8013B3F0();
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGB, GX_RGBA8, 0);
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
        GXSetNumChans(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
        GXSetNumTevStages(1);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ZERO, GX_LO_NOOP);
        GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
        GXSetZCompLoc(GX_FALSE);
        GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
        GXSetColorUpdate(GX_TRUE);
        GXSetAlphaUpdate(GX_FALSE);
        GXSetCullMode(GX_CULL_NONE);
        GXSetPointSize(rect2->field_0x8 * step * 6 / 240.0f, GX_TO_ZERO);
        GXDrawDone();
        GXBegin(GX_POINTS, GX_VTXFMT0, (s32)(0.9999f + 160.0f / step) * (s32)(0.9999f + 240.0f / step));
        s32 half = step >> 1;
        for (s32 py = half; py < half + 0xA0; py += step) {
            u16 sy = 0.00625f * (rect2->field_0xc * py) + rect2->field_0x4;
            for (s32 px = half; px < half + 0xF0; px += step) {
                u16 sx = 0.004166667f * (rect2->field_0x8 * px) + rect2->field_0x0;
                u8 argb[4];
                GXPeekARGB(sx, sy, (u32*)argb);
                GXPosition3f32(px, py, 0.0f);
                GXColor4u8(argb[1], argb[2], argb[3], argb[0]);
            }
        }
        GXEnd();
    }
    if (flag && alpha < 0xFF) {
        gGameRoot2_8053AB10->fn_8013B3F0();
        JGeometry::TBox2f fill(rect->field_0x0, rect->field_0x4, rect->field_0x0 + rect->field_0x8,
                               rect->field_0x4 + rect->field_0xc);
        JUtility::TColor color(0, 0, 0, 0xFF - alpha);
        fn_8013B004(&fill, &color);
    }
    gGameRoot2_8053AB10->field_0x24->fn_8030DFE8(true);
}

// FIXME: 99.2. One induction variable differs (try.py "different", same
// length): the shift loop's line index starts from py (mr r6, r0; mr r4, r6 per
// row) in the original, ours from 0 with py added per row (li, add); the
// literals and the GXTlut table are reloc-name.
// Draws the panel tile grid field_0x48[layer] (2 x 2 blocks of 16 x 16 tiles,
// each four 8 x 8 entries of field_0x24) scrolled by (x, y) (wrapped to
// -0x80..0x180), one palette at a time: direct f32 positions at z 0 and
// field_0x110's corner texture coordinates (the entry's character and flips).
// Rows outside the 160 screen lines are skipped, and without offsets columns
// outside the 240 screen pixels too; with offsets each 8-line row is shifted by
// the negated average of its lines' offsets (the tile wrapped by 512 when all
// three of its rows' shifts put it off one side). grey / alpha other than 0xFF
// tint the texture (second TEV stage, blended onto the screen); else the
// texture's alpha is compared with 0x7E.
void TileMap_804AA858::fn_802FAC10(s32 layer, s32 x, s32 y, s32 flag, u8 grey, u8 alpha, const s32* offsets) {
    static const GXTlut lbl_8046FA30[16] = {
        GX_TLUT0, GX_TLUT1, GX_TLUT2,  GX_TLUT3,  GX_TLUT4,  GX_TLUT5,  GX_TLUT6,  GX_TLUT7,
        GX_TLUT8, GX_TLUT9, GX_TLUT10, GX_TLUT11, GX_TLUT12, GX_TLUT13, GX_TLUT14, GX_TLUT15,
    };
    if (x > 0x180) {
        x -= 0x200;
    }
    if (y > 0x180) {
        y -= 0x200;
    }
    f32 originX = -x;
    f32 originY = -y;
    gGameRoot2_8053AB10->fn_8013B1E8();
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_INDEX16);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetArray(GX_VA_TEX0, field_0x110, sizeof(JGeometry::TVec2f));
    if (grey == 0xFF && alpha == 0xFF) {
        GXSetTevColor(GX_TEVREG0, lbl_805414F0);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_TEXA, GX_CA_A0, GX_CA_KONST, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_COMP_A8_GT, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    } else {
        GXColor color = {grey, grey, grey, alpha};
        GXSetTevColor(GX_TEVREG0, color);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_TEXA, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
        GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
        GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_CPREV, GX_CC_C0, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
        GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP0);
        GXSetTevDirect(GX_TEVSTAGE1);
        GXSetNumTevStages(2);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_SRCALPHA, GX_LO_NOOP);
    }
    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    GXSetCullMode(GX_CULL_NONE);
    u16* grid = field_0x48[layer];
    s32 set;
    if ((layer & 7) == 0 || flag != 0) {
        set = 1;
    } else {
        set = 2;
    }
    GXTlutObj tluts[16];
    for (int i = 0; i < 16; i++) {
        GXInitTlutObj(&tluts[i], field_0x18[set] + i * 0x20, GX_TL_RGB5A3, 16);
        GXLoadTlut(&tluts[i], lbl_8046FA30[i]);
    }
    s32 counts[16];
    for (int i = 0; i < 16; i++) {
        counts[i] = 0;
    }
    for (int by = 0; by < 2; by++) {
        for (int bx = 0; bx < 2; bx++) {
            s32 block = by * 2 + bx;
            for (int cy = 0; cy < 16; cy++) {
                s32 py = (by * 32 + cy * 2) * 8 - y;
                if (py <= -16 || py >= 0xA0) {
                    continue;
                }
                for (int cx = 0; cx < 16; cx++) {
                    s32 px = (bx * 32 + cx * 2) * 8 - x;
                    if (offsets == NULL && (px <= -16 || px >= 0xF0)) {
                        continue;
                    }
                    u16 tile = grid[(block * 16 + cy) * 16 + cx];
                    if (tile != 0) {
                        for (int j = 0; j < 2; j++) {
                            for (int i = 0; i < 2; i++) {
                                counts[(field_0x24[tile * 4 + j * 2 + i] >> 12) & 0xF]++;
                            }
                        }
                    }
                }
            }
        }
    }
    GXTexObj tex;
    for (int palette = 0; palette < 16; palette++) {
        if (counts[palette] <= 0) {
            continue;
        }
        GXInitTexObjCI(&tex, field_0x4[1], 0x80, 0x200, GX_TF_C4, GX_CLAMP, GX_CLAMP, GX_FALSE,
                       lbl_8046FA30[palette]);
        GXLoadTexObj(&tex, GX_TEXMAP0);
        GXBegin(GX_QUADS, GX_VTXFMT0, counts[palette] * 4);
        for (int by = 0; by < 2; by++) {
            for (int bx = 0; bx < 2; bx++) {
                s32 block = by * 2 + bx;
                for (int cy = 0; cy < 16; cy++) {
                    s32 py = (by * 32 + cy * 2) * 8 - y;
                    if (py <= -16 || py >= 0xA0) {
                        continue;
                    }
                    if (offsets != NULL) {
                        // The shift of the row's two halves' top and bottom edges.
                        f32 shift[3];
                        for (int k = 0; k < 3; k++) {
                            shift[k] = 0.0f;
                            for (int m = 0; m < 8; m++) {
                                s32 line = py + k * 8 + m;
                                s32 row;
                                if (line < 0) {
                                    row = 0;
                                } else if (line > 0x9F) {
                                    row = 0x9F;
                                } else {
                                    row = line;
                                }
                                shift[k] -= offsets[row];
                            }
                            shift[k] *= 0.125f;
                        }
                        for (int cx = 0; cx < 16; cx++) {
                            u16 tile = grid[(block * 16 + cy) * 16 + cx];
                            if (tile == 0) {
                                continue;
                            }
                            f32 left = originX + (bx * 32 + cx * 2) * 8;
                            if (left + shift[0] < -16.0f && left + shift[1] < -16.0f && left + shift[2] < -16.0f) {
                                left += 512.0f;
                            } else if (left + shift[0] > 240.0f && left + shift[1] > 240.0f &&
                                       left + shift[2] > 240.0f) {
                                left -= 512.0f;
                            }
                            for (int j = 0; j < 2; j++) {
                                for (int i = 0; i < 2; i++) {
                                    u16 entry = field_0x24[tile * 4 + j * 2 + i];
                                    if (((entry >> 12) & 0xF) != palette) {
                                        continue;
                                    }
                                    f32 x0 = left + i * 8;
                                    f32 top = originY + (by * 32 + cy * 2 + j) * 8;
                                    u16 corner = entry & 0xFFF;
                                    GXPosition3f32(x0 + shift[j], top, 0.0f);
                                    GXTexCoord1x16(corner);
                                    GXPosition3f32(8.0f + x0 + shift[j], top, 0.0f);
                                    GXTexCoord1x16(corner ^ 0x400);
                                    GXPosition3f32(8.0f + x0 + shift[j + 1], 8.0f + top, 0.0f);
                                    GXTexCoord1x16(corner ^ 0xC00);
                                    GXPosition3f32(x0 + shift[j + 1], 8.0f + top, 0.0f);
                                    GXTexCoord1x16(corner ^ 0x800);
                                }
                            }
                        }
                    } else {
                        for (int cx = 0; cx < 16; cx++) {
                            s32 px = (bx * 32 + cx * 2) * 8 - x;
                            if (px <= -16 || px >= 0xF0) {
                                continue;
                            }
                            u16 tile = grid[(block * 16 + cy) * 16 + cx];
                            if (tile == 0) {
                                continue;
                            }
                            for (int j = 0; j < 2; j++) {
                                for (int i = 0; i < 2; i++) {
                                    u16 entry = field_0x24[tile * 4 + j * 2 + i];
                                    if (((entry >> 12) & 0xF) != palette) {
                                        continue;
                                    }
                                    f32 x0 = originX + (bx * 32 + cx * 2 + i) * 8;
                                    f32 top = originY + (by * 32 + cy * 2 + j) * 8;
                                    u16 corner = entry & 0xFFF;
                                    GXPosition3f32(x0, top, 0.0f);
                                    GXTexCoord1x16(corner);
                                    GXPosition3f32(8.0f + x0, top, 0.0f);
                                    GXTexCoord1x16(corner ^ 0x400);
                                    GXPosition3f32(8.0f + x0, 8.0f + top, 0.0f);
                                    GXTexCoord1x16(corner ^ 0xC00);
                                    GXPosition3f32(x0, 8.0f + top, 0.0f);
                                    GXTexCoord1x16(corner ^ 0x800);
                                }
                            }
                        }
                    }
                }
            }
        }
        GXEnd();
    }
}

// FIXME: 98.2. Register allocation and data offsets only (try.py "operands +
// reloc-name"): the tables and "circle.bti" sit 0xB0 / 0x218 too early in our
// .rodata (fn_802FCE74's and fn_802FAC10's tables and fn_802F6800's five are
// not written), and the three GXColor templates are .sbss2 0x80543F54-5C,
// outside this unit's split (0x80543F60). Registers r16-r29 / f20-f31 rotate.
// Draws the stage's tile shades: for layer 0 at field_0x2368 (when
// field_0x235c[0] is set) and layer 4 at field_0x2374 (field_0x2360[1]), every
// tile of the 2 x 2 blocks of 16 x 16 (up to half the height grid's cells) whose
// code has entries in the stage's table (by the first letter of the stage name)
// gets those shapes of "circle.bti" in black with the entry's alpha (TEV
// swap: the texture's alpha from its red), at z 614.4.
void TileMap_804AA858::fn_802FA3FC() {
    // stage 'f' (alpha * 0.85) and every stage without its own table.
    static const TileShade_802FA3FC lbl_8046FA70[34] = {
        {0x36C, {0}, 0, 95, 11.0f, 8.0f, 1, {1.0f, 0.0f}, {0.0f, 1.0f}},
        {0x36A, {0}, 0, 95, 8.0f, 11.0f, 1, {0.0f, 1.0f}, {1.0f, 0.0f}},
        {0x362, {0x36A, 0x36C}, 0, 31, 11.0f, 11.0f, 0, {1.0f, 0.0f}, {0.0f, 1.0f}},
        {0x363, {0x36A, 0x37D}, 0, 31, 5.0f, 11.0f, 0, {-1.0f, 0.0f}, {0.0f, 1.0f}},
        {0x372, {0x37B, 0x36C}, 0, 31, 11.0f, 5.0f, 0, {1.0f, 0.0f}, {0.0f, -1.0f}},
        {0x373, {0x37B, 0x37D}, 0, 31, 5.0f, 5.0f, 0, {-1.0f, 0.0f}, {0.0f, -1.0f}},
        {0x34D, {0}, 0, 95, 5.0f, 5.0f, 1, {1.0f, 1.0f}, {1.0f, -1.0f}},
        {0x36E, {0}, 0, 95, 11.0f, 5.0f, 1, {0.0f, 1.0f}, {1.0f, 1.0f}},
        {0x35D, {0}, 0, 95, 5.0f, 11.0f, 1, {1.0f, 0.0f}, {1.0f, 1.0f}},
        {0x121, {0}, 0, 95, 8.0f, -1.0f, 2, {0.0f, -1.0f}, {1.0f, 0.0f}},
        {0x11B, {0}, 0, 95, 17.0f, 8.0f, 2, {1.0f, 0.0f}, {0.0f, 1.0f}},
        {0x120, {0}, 0, 79, 5.0f, 5.0f, 2, {-1.0f, -1.0f}, {-1.0f, 1.0f}},
        {0x105, {0}, 0, 79, 13.0f, 13.0f, 2, {1.0f, 1.0f}, {1.0f, -1.0f}},
        {0x122, {0}, 0, 119, 13.0f, 5.0f, 2, {1.0f, -1.0f}, {-1.0f, -1.0f}},
        {0x169, {0}, 0, 95, 8.0f, -1.0f, 2, {0.0f, -1.0f}, {1.0f, 0.0f}},
        {0x16B, {0}, 0, 95, 17.0f, 8.0f, 2, {1.0f, 0.0f}, {0.0f, 1.0f}},
        {0x160, {0}, 0, 79, 5.0f, 5.0f, 2, {-1.0f, -1.0f}, {-1.0f, 1.0f}},
        {0x167, {0}, 0, 79, 13.0f, 13.0f, 2, {1.0f, 1.0f}, {1.0f, -1.0f}},
        {0x163, {0}, 0, 119, 13.0f, 5.0f, 2, {1.0f, -1.0f}, {-1.0f, -1.0f}},
        {0x14A, {0}, 0, 95, 8.0f, -1.0f, 2, {0.0f, -1.0f}, {1.0f, 0.0f}},
        {0x15C, {0}, 0, 95, 17.0f, 8.0f, 2, {1.0f, 0.0f}, {0.0f, 1.0f}},
        {0x140, {0}, 0, 79, 5.0f, 5.0f, 2, {-1.0f, -1.0f}, {-1.0f, 1.0f}},
        {0x155, {0}, 0, 79, 13.0f, 13.0f, 2, {1.0f, 1.0f}, {1.0f, -1.0f}},
        {0x145, {0}, 0, 119, 13.0f, 5.0f, 2, {1.0f, -1.0f}, {-1.0f, -1.0f}},
        {0x19B, {0}, 0, 135, 16.0f, 24.0f, 1, {0.7f, 0.0f}, {0.0f, 1.0f}},
        {0x189, {0}, 0, 135, 16.0f, 24.0f, 1, {0.7f, 0.0f}, {0.0f, 1.0f}},
        {0x194, {0}, 0, 135, 16.0f, 24.0f, 1, {0.7f, 0.0f}, {0.0f, 1.0f}},
        {0x367, {0}, 1, 119, 5.0f, 11.0f, 0, {-0.4f, 0.4f}, {0.0f, 1.0f}},
        {0x367, {0}, 0, 119, 5.0f, 11.0f, 0, {0.4f, 0.0f}, {0.0f, 1.0f}},
        {0x376, {0}, 1, 95, 11.0f, 5.0f, 0, {1.0f, 0.0f}, {0.4f, -0.4f}},
        {0x376, {0}, 0, 95, 11.0f, 5.0f, 0, {1.0f, 0.0f}, {0.0f, 0.4f}},
        {0x366, {0}, 1, 135, 11.0f, 11.0f, 0, {1.0f, 0.0f}, {0.0f, 1.0f}},
        {0x366, {0}, 1, 135, 11.0f, 11.0f, 0, {-0.4f, 0.0f}, {0.0f, 1.0f}},
        {0x366, {0}, 0, 135, 11.0f, 11.0f, 0, {1.0f, 0.0f}, {0.0f, -0.4f}},
    };
    // stage 'b' (moved 3 towards the tile centre).
    static const TileShade_802FA3FC lbl_8046FFC0[33] = {
        {0x36C, {0}, 1, 95, 18.0f, 8.0f, 1, {1.0f, 0.0f}, {0.0f, 1.0f}},
        {0x36C, {0}, 0, 95, 18.0f, 8.0f, 1, {-0.2f, 0.0f}, {0.0f, 1.0f}},
        {0x36A, {0}, 1, 95, 8.0f, 18.0f, 1, {0.0f, 1.0f}, {1.0f, 0.0f}},
        {0x36A, {0}, 0, 95, 8.0f, 18.0f, 1, {0.0f, -0.2f}, {1.0f, 0.0f}},
        {0x362, {0x36A, 0x36C}, 1, 31, 2e+01f, 2e+01f, 0, {1.0f, 0.0f}, {0.0f, 1.0f}},
        {0x362, {0x36A, 0x36C}, 1, 31, 2e+01f, 2e+01f, 0, {-0.2f, 0.0f}, {0.0f, 1.0f}},
        {0x362, {0x36A, 0x36C}, 1, 31, 2e+01f, 2e+01f, 0, {1.0f, 0.0f}, {0.0f, -0.2f}},
        {0x362, {0x36A, 0x36C}, 0, 31, 2e+01f, 2e+01f, 0, {-0.2f, 0.0f}, {0.0f, -0.2f}},
        {0x363, {0x36A, 0x37D}, 1, 31, -4.0f, 2e+01f, 0, {-1.0f, 0.0f}, {0.0f, 1.0f}},
        {0x363, {0x36A, 0x37D}, 1, 31, -4.0f, 2e+01f, 0, {0.2f, 0.0f}, {0.0f, 1.0f}},
        {0x363, {0x36A, 0x37D}, 1, 31, -4.0f, 2e+01f, 0, {-1.0f, 0.0f}, {0.0f, -0.2f}},
        {0x363, {0x36A, 0x37D}, 0, 31, -4.0f, 2e+01f, 0, {0.2f, 0.0f}, {0.0f, -0.2f}},
        {0x372, {0x37B, 0x36C}, 1, 31, 2e+01f, -4.0f, 0, {1.0f, 0.0f}, {0.0f, -1.0f}},
        {0x372, {0x37B, 0x36C}, 1, 31, 2e+01f, -4.0f, 0, {-0.2f, 0.0f}, {0.0f, -1.0f}},
        {0x372, {0x37B, 0x36C}, 1, 31, 2e+01f, -4.0f, 0, {1.0f, 0.0f}, {0.0f, 0.2f}},
        {0x372, {0x37B, 0x36C}, 0, 31, 2e+01f, -4.0f, 0, {-0.2f, 0.0f}, {0.0f, 0.2f}},
        {0x373, {0x37B, 0x37D}, 1, 31, -4.0f, -4.0f, 0, {-1.0f, 0.0f}, {0.0f, -1.0f}},
        {0x373, {0x37B, 0x37D}, 1, 31, -4.0f, -4.0f, 0, {0.2f, 0.0f}, {0.0f, -1.0f}},
        {0x373, {0x37B, 0x37D}, 1, 31, -4.0f, -4.0f, 0, {-1.0f, 0.0f}, {0.0f, 0.2f}},
        {0x373, {0x37B, 0x37D}, 0, 31, -4.0f, -4.0f, 0, {0.2f, 0.0f}, {0.0f, 0.2f}},
        {0x367, {0}, 1, 119, 5.0f, 18.0f, 0, {-0.4f, 0.4f}, {0.0f, 1.0f}},
        {0x367, {0}, 1, 119, 5.0f, 18.0f, 0, {-0.4f, 0.4f}, {0.0f, -0.2f}},
        {0x367, {0}, 1, 119, 5.0f, 18.0f, 0, {0.4f, 0.0f}, {0.0f, 1.0f}},
        {0x367, {0}, 0, 119, 5.0f, 18.0f, 0, {0.4f, 0.0f}, {0.0f, -0.2f}},
        {0x376, {0}, 1, 95, 18.0f, 5.0f, 0, {1.0f, 0.0f}, {0.4f, -0.4f}},
        {0x376, {0}, 1, 95, 18.0f, 5.0f, 0, {-0.2f, 0.0f}, {0.4f, -0.4f}},
        {0x376, {0}, 1, 95, 18.0f, 5.0f, 0, {1.0f, 0.0f}, {0.0f, 0.4f}},
        {0x376, {0}, 0, 95, 18.0f, 5.0f, 0, {-0.2f, 0.0f}, {0.0f, 0.4f}},
        {0x366, {0}, 1, 135, 18.0f, 18.0f, 0, {1.0f, 0.0f}, {0.0f, 1.0f}},
        {0x366, {0}, 1, 135, 18.0f, 18.0f, 0, {-0.4f, 0.0f}, {0.0f, 1.0f}},
        {0x366, {0}, 1, 135, 18.0f, 18.0f, 0, {-0.4f, 0.0f}, {0.0f, -0.2f}},
        {0x366, {0}, 1, 135, 18.0f, 18.0f, 0, {1.0f, 0.0f}, {0.0f, -0.4f}},
        {0x366, {0}, 0, 135, 18.0f, 18.0f, 0, {-0.2f, 0.0f}, {0.0f, -0.4f}},
    };
    // stage 'c'.
    static const TileShade_802FA3FC lbl_804704E8[6] = {
        {0x25B, {0}, 0, 31, 1.0f, -4.0f, 0, {1.0f, 0.0f}, {0.0f, 1.0f}},
        {0x254, {0}, 0, 31, 15.0f, -4.0f, 0, {-1.0f, 0.0f}, {0.0f, 1.0f}},
        {0x26C, {0}, 0, 31, 16.0f, 3.0f, 0, {1.0f, 0.0f}, {0.0f, -1.0f}},
        {0x263, {0}, 0, 31, 0.0f, 3.0f, 0, {-1.0f, 0.0f}, {0.0f, -1.0f}},
        {0x25C, {0}, 0, 47, 15.0f, 8.0f, 1, {1.0f, 0.0f}, {0.0f, 1.0f}},
        {0x2A8, {0}, 0, 55, 8.0f, -6.0f, 1, {0.0f, 3.0f}, {1.0f, 0.0f}},
    };
    // stage 'e'.
    static const TileShade_802FA3FC lbl_804705D8[37] = {
        {0x362, {0x36A, 0x36C}, 0, 31, 11.0f, 11.0f, 0, {1.0f, 0.0f}, {0.0f, 1.0f}},
        {0x363, {0x36A, 0x37D}, 0, 31, 5.0f, 11.0f, 0, {-1.0f, 0.0f}, {0.0f, 1.0f}},
        {0x372, {0x37B, 0x36C}, 0, 31, 11.0f, 5.0f, 0, {1.0f, 0.0f}, {0.0f, -1.0f}},
        {0x373, {0x37B, 0x37D}, 0, 31, 5.0f, 5.0f, 0, {-1.0f, 0.0f}, {0.0f, -1.0f}},
        {0x36C, {0}, 0, 95, 11.0f, 8.0f, 1, {1.0f, 0.0f}, {0.0f, 1.0f}},
        {0x36A, {0}, 0, 95, 8.0f, 11.0f, 1, {0.0f, 1.0f}, {1.0f, 0.0f}},
        {0x34D, {0}, 0, 95, 5.0f, 5.0f, 1, {1.0f, 1.0f}, {1.0f, -1.0f}},
        {0x36E, {0}, 0, 95, 11.0f, 5.0f, 1, {0.0f, 1.0f}, {1.0f, 1.0f}},
        {0x35D, {0}, 0, 95, 5.0f, 11.0f, 1, {1.0f, 0.0f}, {1.0f, 1.0f}},
        {0x121, {0}, 0, 95, 8.0f, -1.0f, 2, {0.0f, -1.0f}, {1.0f, 0.0f}},
        {0x11B, {0}, 0, 95, 17.0f, 8.0f, 2, {1.0f, 0.0f}, {0.0f, 1.0f}},
        {0x120, {0}, 0, 79, 5.0f, 5.0f, 2, {-1.0f, -1.0f}, {-1.0f, 1.0f}},
        {0x105, {0}, 0, 79, 13.0f, 13.0f, 2, {1.0f, 1.0f}, {1.0f, -1.0f}},
        {0x122, {0}, 0, 119, 13.0f, 5.0f, 2, {1.0f, -1.0f}, {-1.0f, -1.0f}},
        {0x169, {0}, 0, 95, 8.0f, -1.0f, 2, {0.0f, -1.0f}, {1.0f, 0.0f}},
        {0x16B, {0}, 0, 95, 17.0f, 8.0f, 2, {1.0f, 0.0f}, {0.0f, 1.0f}},
        {0x160, {0}, 0, 79, 5.0f, 5.0f, 2, {-1.0f, -1.0f}, {-1.0f, 1.0f}},
        {0x167, {0}, 0, 79, 13.0f, 13.0f, 2, {1.0f, 1.0f}, {1.0f, -1.0f}},
        {0x163, {0}, 0, 119, 13.0f, 5.0f, 2, {1.0f, -1.0f}, {-1.0f, -1.0f}},
        {0x14A, {0}, 0, 95, 8.0f, -1.0f, 2, {0.0f, -1.0f}, {1.0f, 0.0f}},
        {0x15C, {0}, 0, 95, 17.0f, 8.0f, 2, {1.0f, 0.0f}, {0.0f, 1.0f}},
        {0x140, {0}, 0, 79, 5.0f, 5.0f, 2, {-1.0f, -1.0f}, {-1.0f, 1.0f}},
        {0x155, {0}, 0, 79, 13.0f, 13.0f, 2, {1.0f, 1.0f}, {1.0f, -1.0f}},
        {0x145, {0}, 0, 119, 13.0f, 5.0f, 2, {1.0f, -1.0f}, {-1.0f, -1.0f}},
        {0x367, {0}, 1, 119, 5.0f, 11.0f, 0, {-0.4f, 0.4f}, {0.0f, 1.0f}},
        {0x367, {0}, 0, 119, 5.0f, 11.0f, 0, {0.4f, 0.0f}, {0.0f, 1.0f}},
        {0x376, {0}, 1, 95, 11.0f, 5.0f, 0, {1.0f, 0.0f}, {0.4f, -0.4f}},
        {0x376, {0}, 0, 95, 11.0f, 5.0f, 0, {1.0f, 0.0f}, {0.0f, 0.4f}},
        {0x366, {0}, 1, 135, 11.0f, 11.0f, 0, {1.0f, 0.0f}, {0.0f, 1.0f}},
        {0x366, {0}, 1, 135, 11.0f, 11.0f, 0, {-0.4f, 0.0f}, {0.0f, 1.0f}},
        {0x366, {0}, 0, 135, 11.0f, 11.0f, 0, {1.0f, 0.0f}, {0.0f, -0.4f}},
        {0x27D, {0}, 0, 31, 1.0f, -4.0f, 0, {1.0f, 0.0f}, {0.0f, 1.0f}},
        {0x272, {0}, 0, 31, 15.0f, -4.0f, 0, {-1.0f, 0.0f}, {0.0f, 1.0f}},
        {0x25D, {0}, 0, 31, 16.0f, 3.0f, 0, {1.0f, 0.0f}, {0.0f, -1.0f}},
        {0x252, {0}, 0, 31, 0.0f, 3.0f, 0, {-1.0f, 0.0f}, {0.0f, -1.0f}},
        {0x25B, {0}, 0, 47, 15.0f, 8.0f, 1, {1.0f, 0.0f}, {0.0f, 1.0f}},
        {0x2A8, {0}, 0, 55, 8.0f, -6.0f, 1, {0.0f, 3.0f}, {1.0f, 0.0f}},
    };
    // stage 'i'.
    static const TileShade_802FA3FC lbl_80470BA0[24] = {
        {0x36C, {0}, 1, 95, 15.0f, 8.0f, 1, {1.0f, 0.0f}, {0.0f, 1.0f}},
        {0x36C, {0}, 0, 95, 15.0f, 8.0f, 1, {-0.2f, 0.0f}, {0.0f, 1.0f}},
        {0x36A, {0}, 1, 95, 8.0f, 15.0f, 1, {0.0f, 1.0f}, {1.0f, 0.0f}},
        {0x36A, {0}, 0, 95, 8.0f, 15.0f, 1, {0.0f, -0.2f}, {1.0f, 0.0f}},
        {0x362, {0x36A, 0x36C}, 0, 31, 11.0f, 11.0f, 0, {1.0f, 0.0f}, {0.0f, 1.0f}},
        {0x363, {0x36A, 0x37D}, 0, 31, 5.0f, 11.0f, 0, {-1.0f, 0.0f}, {0.0f, 1.0f}},
        {0x372, {0x37B, 0x36C}, 0, 31, 11.0f, 5.0f, 0, {1.0f, 0.0f}, {0.0f, -1.0f}},
        {0x373, {0x37B, 0x37D}, 0, 31, 5.0f, 5.0f, 0, {-1.0f, 0.0f}, {0.0f, -1.0f}},
        {0x34D, {0}, 1, 95, 9.0f, 9.0f, 1, {1.0f, 1.0f}, {1.0f, -1.0f}},
        {0x34D, {0}, 0, 95, 9.0f, 9.0f, 1, {-0.2f, -0.2f}, {1.0f, -1.0f}},
        {0x36E, {0}, 1, 95, 7.0f, 9.0f, 1, {0.0f, 1.0f}, {1.0f, 1.0f}},
        {0x36E, {0}, 0, 95, 7.0f, 9.0f, 1, {0.0f, -0.2f}, {1.0f, 1.0f}},
        {0x35D, {0}, 1, 95, 9.0f, 7.0f, 1, {1.0f, 0.0f}, {1.0f, 1.0f}},
        {0x35D, {0}, 0, 95, 9.0f, 7.0f, 1, {-0.2f, 0.0f}, {1.0f, 1.0f}},
        {0x367, {0}, 1, 119, 5.0f, 11.0f, 0, {-0.4f, 0.4f}, {0.0f, 1.0f}},
        {0x367, {0}, 1, 119, 5.0f, 15.0f, 0, {0.4f, 0.0f}, {0.0f, 1.0f}},
        {0x367, {0}, 0, 119, 5.0f, 15.0f, 0, {0.4f, 0.0f}, {0.0f, -0.2f}},
        {0x376, {0}, 1, 95, 11.0f, 5.0f, 0, {1.0f, 0.0f}, {0.4f, -0.4f}},
        {0x376, {0}, 1, 95, 15.0f, 5.0f, 0, {1.0f, 0.0f}, {0.0f, 0.4f}},
        {0x376, {0}, 0, 95, 15.0f, 5.0f, 0, {-0.2f, 0.0f}, {0.0f, 0.4f}},
        {0x366, {0}, 1, 135, 15.0f, 15.0f, 0, {1.0f, 0.0f}, {0.0f, 1.0f}},
        {0x366, {0}, 1, 135, 15.0f, 15.0f, 0, {-0.4f, 0.0f}, {0.0f, -0.4f}},
        {0x366, {0}, 1, 135, 15.0f, 15.0f, 0, {-0.4f, 0.0f}, {0.0f, 1.0f}},
        {0x366, {0}, 0, 135, 15.0f, 15.0f, 0, {1.0f, 0.0f}, {0.0f, -0.4f}},
    };

    f32 alphaScale = 1.0f;
    s8 stage = getGameView_802D6A9C()->fn_802D9C4C()[0];
    JUTTexture tex((const ResTIMG*)gGameRoot_8053AAF8->field_0xa4->getResource('TIMG', "circle.bti"), 0);
    const TileShade_802FA3FC* table;
    s32 count;
    switch (stage) {
    case 'c':
        table = lbl_804704E8;
        count = ARRAY_SIZE(lbl_804704E8);
        break;
    case 'b':
        table = lbl_8046FFC0;
        count = ARRAY_SIZE(lbl_8046FFC0);
        break;
    case 'e':
        table = lbl_804705D8;
        count = ARRAY_SIZE(lbl_804705D8);
        break;
    case 'f':
        alphaScale = 0.85f;
        table = lbl_8046FA70;
        count = ARRAY_SIZE(lbl_8046FA70);
        break;
    case 'i':
        table = lbl_80470BA0;
        count = ARRAY_SIZE(lbl_80470BA0);
        break;
    default:
        table = lbl_8046FA70;
        count = ARRAY_SIZE(lbl_8046FA70);
        break;
    }
    gGameRoot2_8053AB10->fn_8013B438();
    gGameRoot2_8053AB10->fn_8013B1E8();
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP1);
    GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_ALPHA, GX_CH_ALPHA, GX_CH_ALPHA, GX_CH_RED);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
    tex.load(GX_TEXMAP0);
    for (s32 pass = 0; pass < 2; pass++) {
        f32 originX;
        f32 originY;
        u16* grid;
        if (pass == 0) {
            originX = field_0x2368.x;
            originY = field_0x2368.y;
            grid = mLayerGrid[0];
        } else {
            originX = field_0x2374.x;
            originY = field_0x2374.y;
            grid = mLayerGrid2[0];
        }
        if ((pass != 0 || !field_0x235c[0]) && (pass != 1 || !field_0x2360[1])) {
            continue;
        }
        for (s32 by = 0; by < 2; by++) {
            for (s32 bx = 0; bx < 2; bx++) {
                for (s32 y = 0; y < 16 && by * 16 + y < field_0x2384 / 2; y++) {
                    f32 tileY = 614.4f + (16.0f * (by * 16 + y) + originY);
                    for (s32 x = 0; x < 16 && bx * 16 + x < field_0x2380 / 2; x++) {
                        u16 tile = grid[((by * 2 + bx) * 16 + y) * 16 + x];
                        f32 tileX = 16.0f * (bx * 16 + x) + originX;
                        const TileShade_802FA3FC* shade = table;
                        for (s32 i = 0; i < count; shade++, i++) {
                            if (shade->mTile != tile) {
                                continue;
                            }
                            f32 px = tileX + shade->mX;
                            f32 py = tileY + shade->mY;
                            s32 alpha = shade->mAlpha * alphaScale;
                            if (stage == 'b') {
                                if (shade->mX < 8.0f) {
                                    px += 3.0f;
                                } else {
                                    px -= 3.0f;
                                }
                                if (shade->mY < 8.0f) {
                                    py += 3.0f;
                                } else {
                                    py -= 3.0f;
                                }
                            }
                            switch (shade->mKind) {
                            case 0: {
                                GXColor color = {0, 0, 0, alpha};
                                f32 ux = shade->mU[0];
                                f32 uy = shade->mU[1];
                                f32 vx = shade->mV[0];
                                f32 vy = shade->mV[1];
                                ux *= 96.0f;
                                uy *= 96.0f;
                                vx *= 96.0f;
                                vy *= 96.0f;
                                GXSetTevColor(GX_TEVREG0, color);
                                GXBegin(GX_QUADS, GX_VTXFMT0, 4);
                                GXPosition3f32(px, py, 614.4f);
                                GXTexCoord2f32(0.0f, 0.0f);
                                GXPosition3f32(px + ux, py + uy, 614.4f);
                                GXTexCoord2f32(1.0f, 0.0f);
                                GXPosition3f32(vx + (px + ux), vy + (py + uy), 614.4f);
                                GXTexCoord2f32(1.0f, 1.0f);
                                GXPosition3f32(px + vx, py + vy, 614.4f);
                                GXTexCoord2f32(0.0f, 1.0f);
                                GXEnd();
                                break;
                            }
                            case 1: {
                                GXColor color = {0, 0, 0, alpha};
                                f32 ux = shade->mU[0];
                                f32 uy = shade->mU[1];
                                f32 vx = shade->mV[0];
                                f32 vy = shade->mV[1];
                                ux *= 64.0f;
                                uy *= 64.0f;
                                vx *= 24.0f;
                                vy *= 24.0f;
                                GXSetTevColor(GX_TEVREG0, color);
                                GXBegin(GX_QUADS, GX_VTXFMT0, 4);
                                f32 x0 = px - vx;
                                f32 y0 = py - vy;
                                f32 x1 = px + vx;
                                f32 y1 = py + vy;
                                GXPosition3f32(x0, y0, 614.4f);
                                GXTexCoord2f32(0.0f, -1.0f);
                                GXPosition3f32(ux + x0, uy + y0, 614.4f);
                                GXTexCoord2f32(1.0f, -1.0f);
                                GXPosition3f32(ux + x1, uy + y1, 614.4f);
                                GXTexCoord2f32(1.0f, 1.0f);
                                GXPosition3f32(x1, y1, 614.4f);
                                GXTexCoord2f32(0.0f, 1.0f);
                                GXEnd();
                                break;
                            }
                            case 2: {
                                GXColor color = {0, 0, 0, alpha};
                                f32 ux = shade->mU[0];
                                f32 uy = shade->mU[1];
                                f32 backX = ux;
                                f32 backY = uy;
                                f32 vx = shade->mV[0];
                                f32 vy = shade->mV[1];
                                ux *= 64.0f;
                                uy *= 64.0f;
                                backX *= -4.0f;
                                backY *= -4.0f;
                                vx *= 24.0f;
                                vy *= 24.0f;
                                GXSetTevColor(GX_TEVREG0, color);
                                GXBegin(GX_QUADS, GX_VTXFMT0, 8);
                                f32 x0 = px - vx;
                                f32 y0 = py - vy;
                                f32 x1 = px + vx;
                                f32 y1 = py + vy;
                                GXPosition3f32(x0, y0, 614.4f);
                                GXTexCoord2f32(0.0f, -1.0f);
                                GXPosition3f32(ux + x0, uy + y0, 614.4f);
                                GXTexCoord2f32(1.0f, -1.0f);
                                GXPosition3f32(ux + x1, uy + y1, 614.4f);
                                GXTexCoord2f32(1.0f, 1.0f);
                                GXPosition3f32(x1, y1, 614.4f);
                                GXTexCoord2f32(0.0f, 1.0f);
                                GXPosition3f32(x0, y0, 614.4f);
                                GXTexCoord2f32(0.0f, -1.0f);
                                GXPosition3f32(backX + x0, backY + y0, 614.4f);
                                GXTexCoord2f32(1.0f, -1.0f);
                                GXPosition3f32(backX + x1, backY + y1, 614.4f);
                                GXTexCoord2f32(1.0f, 1.0f);
                                GXPosition3f32(x1, y1, 614.4f);
                                GXTexCoord2f32(0.0f, 1.0f);
                                GXEnd();
                                break;
                            }
                            }
                            if (!shade->mMore) {
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}

// FIXME: 88.1. As in fn_802F9714: the original loads the next source bytes of the
// unrolled nibble-swap loop before storing the previous ones; registers differ.
// fn_802F9714 for the 'SCH ' tile images of field_0x4[slot]; for slot 1 it
// also keeps bytes 0x200-0x2FF of that buffer in field_0x238c.
void TileMap_804AA858::fn_802FA160(void* res, u32 srcOffset, s32 slot, u32 dstOffset, u32 size) {
    u32 len;
    if (size == 0) {
        s32 resSize;
        if (JKRDecomp::checkCompressed((u8*)res)) {
            resSize = JKRDecompExpandSize((u8*)res);
        } else {
            resSize = gGameRoot_8053AAF8->field_0xa4->getResSize((u8*)res);
        }
        if (resSize < 0) {
            len = 0x8000;
        } else {
            len = resSize;
        }
    } else {
        len = size;
    }
    u8* dst = field_0x4[slot] + dstOffset;
    u8* src;
    if (JKRDecomp::checkCompressed((u8*)res)) {
        JKRDecomp::decode((u8*)res, field_0xf8, 0x8000, 0);
        src = field_0xf8;
    } else {
        src = (u8*)res;
    }
    src += srcOffset;
    for (u32 i = 0; i < len; i++) {
        dst[i] = ((src[i] >> 4) & 0xF) | ((src[i] & 0xF) << 4);
    }
    DCFlushRange(dst, len);
    if (slot == 1) {
        u8* from = field_0x4[1] + 0x200;
        u8* to = field_0x238c;
        for (int i = 0; i < 0x100; i++) {
            *to++ = *from++;
        }
    }
}

// FIXME: 93.7. One instruction short: the original stores the high byte of colour
// 0xD8 as clrlwi + srawi 8 (extrwi for 0xD9 / 0xDA; ours extrwi for all three).
// Also moved: the original adds src + srcOffset (add r9, r26, r7) after copying
// len to r4 (mr r4, r24) before the first loop; ours adds it right after the
// decode and copies len after the loops (as in fn_802F93F4). Registers differ.
// The 'SCL ' palettes of the sprite layer `slot` (0-2): fn_802F93F4 into
// field_0xc[slot] (keeping the first colour, made transparent, in
// field_0x114[slot]), colours 0xD8-0xDA made RGB4A3 with alpha 4 / 7, and the
// adjusted copy in field_0x18[slot].
void TileMap_804AA858::fn_802F9DA8(void* res, u32 srcOffset, s32 slot, u32 dstOffset, u32 size, bool flag) {
    u32 len;
    if (size == 0) {
        s32 resSize;
        if (JKRDecomp::checkCompressed((u8*)res)) {
            resSize = JKRDecompExpandSize((u8*)res);
        } else {
            resSize = gGameRoot_8053AAF8->field_0xa4->getResSize((u8*)res);
        }
        if (resSize < 0) {
            len = 0x200;
        } else {
            len = resSize;
        }
    } else {
        len = size;
    }
    u8* src;
    if (JKRDecomp::checkCompressed((u8*)res)) {
        JKRDecomp::decode((u8*)res, field_0xf8, 0x8000, 0);
        src = field_0xf8;
    } else {
        src = (u8*)res;
    }
    src += srcOffset;
    u8* dst = field_0xc[slot] + dstOffset;
    u16 first;
    for (s32 i = 0; i < len; i += 2) {
        u16 color;
        if (i != 0 && (i & 0x1F) == 0) {
            color = 0;
        } else {
            u16 bgr = src[0] | (src[1] << 8);
            color = ((bgr & 0x1F) << 10) | (bgr & 0x3E0) | ((bgr >> 10) & 0x1F) | 0x8000;
            if (i == 0) {
                first = color;
                color = 0;
            }
        }
        dst[i] = color >> 8;
        dst[i + 1] = color;
        src += 2;
    }
    field_0x114[slot] = first;
    for (int j = 0; j < 3; j++) {
        u8* entry = field_0xc[slot] + 0x1B0 + j * 2;
        u16 color = (entry[0] << 8) | entry[1];
        color = 0x4000 | ((color >> 3) & 0xF00) | ((color >> 2) & 0xF0) | ((color >> 1) & 0xF);
        entry[0] = color >> 8;
        entry[1] = color;
    }
    u8* out = field_0x18[slot] + dstOffset;
    u8* in = field_0xc[slot] + dstOffset;
    for (s32 i = 0; i < len; i += 2) {
        u16 color = (in[i] << 8) | in[i + 1];
        if (color & 0x8000) {
            u8 r = adjustChannel((color >> 10) & 0x1F, 1.0f, 0.0f);
            u8 g = adjustChannel((color >> 5) & 0x1F, 1.0f, 0.0f);
            u8 b = adjustChannel(color & 0x1F, 1.0f, 0.0f);
            if (flag) {
                color = 0x8000 | (b << 10) | (r << 5) | g;
            } else {
                color = 0x8000 | (r << 10) | (g << 5) | b;
            }
        }
        out[i] = color >> 8;
        out[i + 1] = color;
    }
    DCFlushRange(out, len);
}

// FIXME: 90.7. As in fn_802F99DC: one mr fewer in the 4x-unrolled copy loop (the
// original loads the next low byte before the store); registers differ.
// Loads the u16 grid field_0x24 from res like fn_802F99DC (little-endian,
// `size` bytes, else the resource's size), from byte `start` on.
void TileMap_804AA858::fn_802F9BE8(void* res, u32 srcOffset, u32 start, u32 size) {
    u32 len;
    if (size == 0) {
        s32 resSize;
        if (JKRDecomp::checkCompressed((u8*)res)) {
            resSize = JKRDecompExpandSize((u8*)res);
        } else {
            resSize = gGameRoot_8053AAF8->field_0xa4->getResSize((u8*)res);
        }
        if (resSize < 0) {
            len = 0x2000;
        } else {
            len = resSize;
        }
    } else {
        len = size;
    }
    u8* src;
    if (JKRDecomp::checkCompressed((u8*)res)) {
        JKRDecomp::decode((u8*)res, field_0xf8, 0x8000, 0);
        src = field_0xf8;
    } else {
        src = (u8*)res;
    }
    src += srcOffset;
    u32 index = start / 2;
    for (u32 i = start; i < len; i += 2) {
        field_0x24[index++] = (src[i + 1] << 8) | src[i];
    }
}

// FIXME: 89.4. In the 4x-unrolled copy loop the original loads the second
// element's low byte before storing the first (one mr more); registers differ.
// Loads the 'SCN ' tile grid of `layer` from res (decoded first when
// compressed; `size` bytes from srcOffset, else the resource's size):
// little-endian u16 tile codes, stored at byte offset dstOffset of the grid.
void TileMap_804AA858::fn_802F99DC(void* res, u32 srcOffset, s32 layer, u32 dstOffset, u32 size) {
    u32 len;
    if (size == 0) {
        s32 resSize;
        if (JKRDecomp::checkCompressed((u8*)res)) {
            resSize = JKRDecompExpandSize((u8*)res);
        } else {
            resSize = gGameRoot_8053AAF8->field_0xa4->getResSize((u8*)res);
        }
        if (resSize < 0) {
            len = 0x2000;
        } else {
            len = resSize;
        }
    } else {
        len = size;
    }
    u8* src;
    if (JKRDecomp::checkCompressed((u8*)res)) {
        JKRDecomp::decode((u8*)res, field_0xf8, 0x8000, 0);
        src = field_0xf8;
    } else {
        src = (u8*)res;
    }
    src += srcOffset;
    u16* dst;
    if (layer >= 0 && layer < 4) {
        dst = &mLayerGrid[layer][dstOffset / 2];
    } else if (layer >= 4 && layer < 8) {
        dst = &mLayerGrid2[layer - 4][dstOffset / 2];
    } else {
        dst = &mLayerGrid[layer][dstOffset / 2];
    }
    for (u32 i = 0; i < len; i += 2) {
        dst[i / 2] = (src[i + 1] << 8) | src[i];
    }
}

void TileMap_804AA858::fn_802F9974(s32 layer, s32 index, u16 value) {
    if (layer >= 0 && layer < 4) {
        mLayerGrid[layer][index] = value;
    } else if (layer >= 4 && layer < 8) {
        mLayerGrid2[layer - 4][index] = value;
    } else {
        mLayerGrid[layer][index] = value;
    }
}

// Layers 0-3 and 4-7 have their own grid arrays; any other layer indexes
// the first array as is.
u16 TileMap_804AA858::fn_802F9908(s32 layer, s32 index) {
    s32 tile;
    if (layer >= 0 && layer < 4) {
        tile = mLayerGrid[layer][index];
    } else if (layer >= 4 && layer < 8) {
        tile = mLayerGrid2[layer - 4][index];
    } else {
        tile = mLayerGrid[layer][index];
    }
    return tile;
}

// FIXME: 84.4. In the unrolled nibble-swap loop the original loads the next
// source byte before storing the previous one (ours stores, then loads), and the
// computed length lives in r26 (ours r31; the parameters move up one register).
void TileMap_804AA858::fn_802F9714(void* res, u32 srcOffset, s32 slot, u32 dstOffset, u32 size) {
    u32 len;
    if (size == 0) {
        s32 resSize;
        if (JKRDecomp::checkCompressed((u8*)res)) {
            resSize = JKRDecompExpandSize((u8*)res);
        } else {
            resSize = gGameRoot_8053AAF8->field_0xa4->getResSize((u8*)res);
        }
        if (resSize < 0) {
            len = 0x8000;
        } else {
            len = resSize;
        }
    } else {
        len = size;
    }
    u8* dst = field_0x88[slot] + dstOffset;
    u8* src;
    if (JKRDecomp::checkCompressed((u8*)res)) {
        JKRDecomp::decode((u8*)res, field_0xf8, 0x8000, 0);
        src = field_0xf8;
    } else {
        src = (u8*)res;
    }
    src += srcOffset;
    for (u32 i = 0; i < len; i++) {
        dst[i] = ((src[i] >> 4) & 0xF) | ((src[i] & 0xF) << 4);
    }
    DCFlushRange(dst, len);
}

// FIXME: 94.9. Register allocation (res / srcOffset / dstOffset / len one register
// off) and the original copies len to r4 before the first loop for the second
// loop and DCFlushRange (ours after it); its src + srcOffset is added later.
// Converts the 'SCL ' palette resource res (little-endian BGR555, decoded
// first when compressed; `size` bytes from srcOffset, else the resource's
// size) into the GX RGB5A3 palettes field_0x98[slot] + dstOffset, the first
// colour of every 16 transparent; then copies them to field_0xc8[slot] with
// each opaque colour's channels adjusted (rotated to b, r, g with `flag`) and
// flushes that copy.
void TileMap_804AA858::fn_802F93F4(void* res, u32 srcOffset, s32 slot, u32 dstOffset, u32 size, bool flag) {
    u32 len;
    if (size == 0) {
        s32 resSize;
        if (JKRDecomp::checkCompressed((u8*)res)) {
            resSize = JKRDecompExpandSize((u8*)res);
        } else {
            resSize = gGameRoot_8053AAF8->field_0xa4->getResSize((u8*)res);
        }
        if (resSize < 0) {
            len = 0x200;
        } else {
            len = resSize;
        }
    } else {
        len = size;
    }
    u8* dst = field_0x98[slot] + dstOffset;
    u8* src;
    if (JKRDecomp::checkCompressed((u8*)res)) {
        JKRDecomp::decode((u8*)res, field_0xf8, 0x8000, 0);
        src = field_0xf8;
    } else {
        src = (u8*)res;
    }
    src += srcOffset;
    for (s32 i = 0; i < len; i += 2) {
        u16 color;
        if (i != 0 && (i & 0x1F) == 0) {
            color = 0;
        } else {
            u16 bgr = src[i] | (src[i + 1] << 8);
            color = ((bgr & 0x1F) << 10) | (bgr & 0x3E0) | ((bgr >> 10) & 0x1F) | 0x8000;
            if (i == 0) {
                color = 0;
            }
        }
        dst[i] = color >> 8;
        dst[i + 1] = color;
    }
    u8* out = field_0xc8[slot] + dstOffset;
    u8* in = field_0x98[slot] + dstOffset;
    for (s32 i = 0; i < len; i += 2) {
        u16 color = (in[i] << 8) | in[i + 1];
        if (color & 0x8000) {
            u8 r = adjustChannel((color >> 10) & 0x1F, 1.0f, 0.0f);
            u8 g = adjustChannel((color >> 5) & 0x1F, 1.0f, 0.0f);
            u8 b = adjustChannel(color & 0x1F, 1.0f, 0.0f);
            if (flag) {
                color = 0x8000 | (b << 10) | (r << 5) | g;
            } else {
                color = 0x8000 | (r << 10) | (g << 5) | b;
            }
        }
        out[i] = color >> 8;
        out[i + 1] = color;
    }
    DCFlushRange(out, len);
}

// The view matrix of a sprite at pos (raised by offset): params' matrix (a
// zero diagonal made 1) or the identity, scaled for the height like
// fn_802F5FF4 and moved to pos, when params has a matrix or pos is off the
// ground; *loaded says whether it was loaded (else the plain camera matrix).
void TileMap_804AA858::fn_802F91AC(const JGeometry::TVec3f* pos, SpriteParams_801F5AE0* params, bool* loaded,
                               f32 offset) {
    *loaded = false;
    if ((params != NULL && params->mMtx != NULL) ||
        (0.0f != pos->z && (params == NULL || params->field_0x1f)))
    {
        Mtx m;
        if (params != NULL && params->mMtx != NULL) {
            JMath::gekko_ps_copy12(m, params->mMtx);
            if (0.0f == m[0][0] && 0.0f == m[1][1] && 0.0f == m[2][2]) {
                m[0][0] = 1.0f;
                m[1][1] = 1.0f;
                m[2][2] = 1.0f;
            }
        } else {
            PSMTXIdentity(m);
        }
        if (0.0f != pos->z && (params == NULL || params->field_0x1f)) {
            f32 scale = fn_802F5FF4(pos->z);
            m[0][0] *= scale;
            m[1][0] *= scale;
            m[2][0] *= scale;
            m[0][1] *= scale;
            m[1][1] *= scale;
            m[2][1] *= scale;
            m[0][2] *= scale;
            m[1][2] *= scale;
            m[2][2] *= scale;
            m[0][3] *= scale;
            m[1][3] *= scale;
            m[2][3] *= scale;
        }
        m[0][3] += pos->x;
        m[1][3] += pos->y + offset;
        m[2][3] += pos->z + offset;
        gGameRoot2_8053AB10->fn_8013B3CC(m);
        *loaded = true;
    } else {
        gGameRoot2_8053AB10->fn_8013B3F0();
    }
}

// The GX state of a textured sprite quad (direct f32 position and texture
// coordinates, one TEV stage of the texture times the register colours): the
// TEV colours, blend mode, z mode and alpha update of params, else white /
// zero, the plain blend and the default z mode. fn_802F86CC calls it after
// fn_802F91AC.
void TileMap_804AA858::fn_802F8F0C(SpriteParams_801F5AE0* params) {
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumChans(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTexGens(1);
    if (params != NULL) {
        GXSetTevColor(GX_TEVREG0, params->field_0x10);
        GXSetTevColor(GX_TEVREG1, params->field_0x14);
    } else {
        GXSetTevColor(GX_TEVREG0, lbl_805414F4);
        GXColor zero = {0, 0, 0, 0};
        GXSetTevColor(GX_TEVREG1, zero);
    }
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_C0, GX_CC_C1);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_A1);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetNumTevStages(1);
    GXSetNumIndStages(0);
    if (params != NULL && params->field_0x34 == 1) {
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_NOOP);
    } else if (params != NULL && params->field_0x34 == 2) {
        GXSetBlendMode(GX_BM_BLEND, GX_BL_DSTALPHA, GX_BL_INVDSTALPHA, GX_LO_NOOP);
    } else {
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    }
    if (params != NULL) {
        GXSetZMode(params->field_0x26, (GXCompare)params->field_0x28, params->field_0x2c);
    } else {
        GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    }
    GXSetZCompLoc(GX_FALSE);
    GXSetColorUpdate(GX_TRUE);
    if (params != NULL) {
        GXSetAlphaUpdate(params->field_0x2d);
        GXSetAlphaCompare(GX_GREATER, params->field_0x2e, GX_AOP_AND, GX_ALWAYS, 0);
    } else {
        GXSetAlphaUpdate(GX_FALSE);
        GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_AND, GX_ALWAYS, 0);
    }
    GXSetCullMode(GX_CULL_NONE);
}

// FIXME: 88.9. The original keeps obj in memory as if its address were taken:
// every bit-field store is kept (byte 0x11 stored after each field, the
// unused mAffineParam = 0 too) and the draw re-reads the fields; ours keeps
// them in registers and drops the dead stores. The three tables are at
// .rodata + 0x40 / 0x60 / 0xA0 here, + 0x1620 / 0x1640 / 0x1680 in the original
// (the local tables of the later fn_802FA3FC ... fn_802FCE74 come first and
// are not written yet). tmp's constructor stores are scheduled later.
// Draws frame `frame` of the "SOB " resource res at pos: res + 2 holds the
// little-endian start offsets of the frames, each frame 6-byte GBA OAM
// entries, drawn last to first as quads of the 4-bit tile images (field_0xb5c,
// palettes field_0x25c; the set is params' field_0x1e) scaled by (scaleX,
// scaleY). params' bytes 0x21 / 0x22 queue the sprite instead (fn_802F654C /
// fn_802F6298); field_0x30 adds the effect of fn_802F6800.
void TileMap_804AA858::fn_802F86CC(const JGeometry::TVec3f* pos, void* res, s32 frame,
                                   SpriteParams_801F5AE0* params, f32 scaleX, f32 scaleY) {
    // The GX texture maps of the (palette & 1, priority) images.
    static const GXTexMapID lbl_80470F60[8] = {
        GX_TEXMAP4, GX_TEXMAP5, GX_TEXMAP6, GX_TEXMAP7, GX_TEXMAP0, GX_TEXMAP1, GX_TEXMAP2, GX_TEXMAP3,
    };
    static const GXTlut lbl_80470F80[16] = {
        GX_TLUT0, GX_TLUT1, GX_TLUT2,  GX_TLUT3,  GX_TLUT4,  GX_TLUT5,  GX_TLUT6,  GX_TLUT7,
        GX_TLUT8, GX_TLUT9, GX_TLUT10, GX_TLUT11, GX_TLUT12, GX_TLUT13, GX_TLUT14, GX_TLUT15,
    };
    // The GBA sprite sizes in 8-pixel tiles (width, height) by shape and size.
    static const s32 lbl_80470FC0[3][4][2] = {
        {{1, 1}, {2, 2}, {4, 4}, {8, 8}},
        {{2, 1}, {4, 1}, {4, 2}, {8, 4}},
        {{1, 2}, {1, 4}, {2, 4}, {4, 8}},
    };

    if (!field_0x2364) {
        return;
    }
    if (params != NULL && params->field_0x21 && fn_802F654C(pos, res, frame, NULL, NULL, params)) {
        return;
    }
    if (params != NULL && params->field_0x22 && fn_802F6298(pos, res, frame, NULL, NULL, params)) {
        return;
    }
    u8* data = (u8*)res;
    s32 start = data[frame * 2 + 2] | data[frame * 2 + 3] << 8;
    s32 end = data[frame * 2 + 4] | data[frame * 2 + 5] << 8;
    s32 count = (end - start) / 6;
    Mtx mtx;
    SpriteParams_801F5AE0 tmp;
    JGeometry::TVec3f out;
    SpriteObj_802F86CC obj;
    SpriteParams_801F5AE0* outParams;
    bool loaded;
    s32 lastPriority = -1;
    s32 lastPalette = -1;
    fn_802F5570(pos, params, &out, &outParams, mtx, &tmp);
    f32 offset;
    if (outParams != NULL) {
        offset = getDrawOffset(outParams, &out);
    } else {
        offset = 0.0f;
    }
    fn_802F91AC(&out, outParams, &loaded, offset);
    fn_802F8F0C(outParams);
    s32 dy = 4.0f * (1.0f - scaleY);
    s32 dx = 4.0f * (1.0f - scaleX);
    for (s32 i = count - 1; i >= 0; i--) {
        SobObj_802F86CC* entry = (SobObj_802F86CC*)(data + start) + i;
        obj.mY = entry->mY;
        obj.mAffine = entry->mAffine;
        obj.mMode = entry->mMode;
        obj.mMosaic = entry->mMosaic;
        obj.mColor256 = entry->mColor256;
        obj.mShape = entry->mShape;
        obj.mX = entry->mX | (entry->mX & 0x80) << 1;
        if (i != 0) {
            obj.mY += dy;
            obj.mX += dx;
        }
        obj.mAffineIndex = entry->mAffineIndex;
        obj.mHFlip = entry->mHFlip;
        obj.mVFlip = entry->mVFlip;
        obj.mSize = entry->mSize;
        obj.mTile = entry->mTileLow | entry->mTileHigh << 8;
        obj.mPriority = entry->mPriority;
        if (outParams != NULL) {
            obj.mPalette = outParams->field_0x0[entry->mPalette];
        } else {
            obj.mPalette = entry->mPalette;
        }
        obj.mAffineParam = 0;
        s32 priority = obj.mPriority;
        if (lastPriority != priority || lastPalette != obj.mPalette) {
            lastPriority = priority;
            lastPalette = obj.mPalette;
            s32 set = 0;
            if (outParams != NULL) {
                set = outParams->field_0x1e;
            }
            GXLoadTlut(&field_0x25c[set][priority][lastPalette], lbl_80470F80[lastPalette]);
            GXLoadTexObj(&field_0xb5c[set][priority][lastPalette], lbl_80470F60[(lastPalette & 1) << 2 | priority]);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, lbl_80470F60[(lastPalette & 1) << 2 | priority], GX_COLOR_NULL);
        }
        f32 w = lbl_80470FC0[obj.mShape][obj.mSize][0] * 8;
        f32 h = lbl_80470FC0[obj.mShape][obj.mSize][1] * 8;
        f32 x;
        if (obj.mX & 0x100) {
            x = (s16)(obj.mX | 0xFE00);
        } else {
            x = (u8)obj.mX;
        }
        f32 y = 0.0f;
        f32 z = -(f32)(s8)obj.mY;
        if (!loaded) {
            x += out.x;
            y += out.y + offset;
            z += out.z + offset;
        }
        f32 u0 = (8.0f * (obj.mTile & 0x1F) + 0.5f) / 256.0f;
        f32 v0 = (8.0f * (obj.mTile & 0x3E0) / 32.0f + 0.5f) / 256.0f;
        f32 u1;
        if (obj.mHFlip) {
            u1 = u0;
            u0 = (w - 1.0f) / 256.0f + u0;
        } else {
            u1 = (w - 1.0f) / 256.0f + u0;
        }
        f32 v1;
        if (obj.mVFlip) {
            v1 = v0;
            v0 = (h - 1.0f) / 256.0f + v0;
        } else {
            v1 = (h - 1.0f) / 256.0f + v0;
        }
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(x, y, z);
        GXTexCoord2f32(u0, v0);
        GXPosition3f32(x + w * scaleX, y, z);
        GXTexCoord2f32(u1, v0);
        GXPosition3f32(x + w * scaleX, y, z - h * scaleY);
        GXTexCoord2f32(u1, v1);
        GXPosition3f32(x, y, z - h * scaleY);
        GXTexCoord2f32(u0, v1);
        GXEnd();
    }
    if (params != NULL && params->field_0x30 != -1) {
        fn_802F6800(pos, res, frame, NULL, NULL, params);
    }
}

// FIXME: 89.8. The quad loop: the original converts the tile's pixel x / y
// ((x * 2 + i) * 8, (y * 2 + j) * 8) to float twice each inside the innermost
// loop and computes the second texture coordinates as px * (1 / 128) + 0.0f
// (fmadds with the pool's 0.0f); ours hoists the y conversion out of the i loop,
// converts once and folds the + 0.0f (fmuls). The count loop's registers differ.
// Draws the 16 x 16 block map res (little-endian u16 block numbers, 0 empty)
// at pos: block n is the 2 x 2 tiles field_0x24[n * 4 ...] (GBA BG entries:
// tile column / row of field_0x4[0], flips, palette), drawn one palette at a
// time (field_0x18[0]) as 8 x 8 quads; with field_0xe450 a second TEV stage
// multiplies in the player's field_0xe428 texture. params' bytes 0x21 / 0x22
// queue it instead; field_0x30 adds the effect of fn_802F6800.
void TileMap_804AA858::fn_802F6CF4(const JGeometry::TVec3f* pos, void* res, SpriteParams_801F5AE0* params) {
    static const GXTlut lbl_80471020[16] = {
        GX_TLUT0, GX_TLUT1, GX_TLUT2,  GX_TLUT3,  GX_TLUT4,  GX_TLUT5,  GX_TLUT6,  GX_TLUT7,
        GX_TLUT8, GX_TLUT9, GX_TLUT10, GX_TLUT11, GX_TLUT12, GX_TLUT13, GX_TLUT14, GX_TLUT15,
    };
    GXTlutObj tluts[16];
    s32 counts[16];
    GXTexObj tex;
    JGeometry::TVec3f out;

    if (!field_0x2364) {
        return;
    }
    if (params != NULL && params->field_0x21 && fn_802F654C(pos, NULL, 0, NULL, res, params)) {
        return;
    }
    if (params != NULL && params->field_0x22 && fn_802F6298(pos, NULL, 0, NULL, res, params)) {
        return;
    }
    gGameRoot2_8053AB10->fn_8013B1E8();
    fn_802F5040(pos, params, &out);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX1, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_TEX_ST, GX_F32, 0);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTexGens(2);
    if (field_0xe450) {
        s32 player;
        if (isGameModeSet() && gGameRoot_8053AAF8->field_0x4 != NULL) {
            player = getGameView_802D6A9C()->field_0x50;
        } else {
            player = 0;
        }
        field_0xe428[player]->load(GX_TEXMAP1);
        GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
        GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_CPREV, GX_CC_TEXC, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
        GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP0);
        GXSetTevDirect(GX_TEVSTAGE1);
        GXSetNumTevStages(2);
    }
    for (int i = 0; i < 16; i++) {
        GXInitTlutObj(&tluts[i], field_0x18[0] + i * 0x20, GX_TL_RGB5A3, 16);
        GXLoadTlut(&tluts[i], lbl_80471020[i]);
    }
    for (int i = 0; i < 16; i++) {
        counts[i] = 0;
    }
    u8* data = (u8*)res;
    for (int y = 0; y < 16; y++) {
        for (int x = 0; x < 16; x++) {
            s32 index = y * 16 + x;
            u16 block = data[index * 2] | data[index * 2 + 1] << 8;
            if (block != 0) {
                for (int k = 0; k < 4; k++) {
                    counts[(field_0x24[block * 4 + k] >> 12) & 0xF]++;
                }
            }
        }
    }
    for (int palette = 0; palette < 16; palette++) {
        if (counts[palette] > 0) {
            GXInitTexObjCI(&tex, field_0x4[0], 0x80, 0x200, GX_TF_C4, GX_CLAMP, GX_CLAMP, GX_FALSE,
                           lbl_80471020[palette]);
            GXLoadTexObj(&tex, GX_TEXMAP0);
            GXBegin(GX_QUADS, GX_VTXFMT0, counts[palette] * 4);
            for (int y = 0; y < 16; y++) {
                for (int x = 0; x < 16; x++) {
                    s32 index = y * 16 + x;
                    u16 block = data[index * 2] | data[index * 2 + 1] << 8;
                    if (block == 0) {
                        continue;
                    }
                    for (int j = 0; j < 2; j++) {
                        for (int i = 0; i < 2; i++) {
                            u16 tile = field_0x24[block * 4 + j * 2 + i];
                            if (((tile >> 12) & 0xF) != palette) {
                                continue;
                            }
                            f32 u0 = (8.0f * (tile & 0xF) + 0.501f) / 128.0f;
                            f32 v0 = (8.0f * ((tile >> 4) & 0x3F) + 0.501f) / 512.0f;
                            f32 s0 = ((x * 2 + i) * 8) / 128.0f + 0.0f;
                            f32 t0 = ((y * 2 + j) * 8) / 128.0f + 0.0f;
                            f32 s1 = 0.0625f + s0;
                            f32 t1 = 0.0625f + t0;
                            f32 x0 = (x * 2 + i) * 8 + out.x;
                            f32 y0 = (y * 2 + j) * 8 + out.y;
                            f32 u1;
                            if (tile & 0x400) {
                                u1 = u0;
                                u0 = 0.05453125f + u0;
                            } else {
                                u1 = 0.05453125f + u0;
                            }
                            f32 v1;
                            if (tile & 0x800) {
                                v1 = v0;
                                v0 = 0.013632813f + v0;
                            } else {
                                v1 = 0.013632813f + v0;
                            }
                            GXPosition3f32(x0, y0, out.z);
                            GXTexCoord2f32(u0, v0);
                            GXTexCoord2f32(s0, t0);
                            GXPosition3f32(8.0f + x0, y0, out.z);
                            GXTexCoord2f32(u1, v0);
                            GXTexCoord2f32(s1, t0);
                            GXPosition3f32(8.0f + x0, 8.0f + y0, out.z);
                            GXTexCoord2f32(u1, v1);
                            GXTexCoord2f32(s1, t1);
                            GXPosition3f32(x0, 8.0f + y0, out.z);
                            GXTexCoord2f32(u0, v1);
                            GXTexCoord2f32(s0, t1);
                        }
                    }
                }
            }
            GXEnd();
        }
    }
    if (params != NULL && params->field_0x30 != -1) {
        fn_802F6800(pos, NULL, 0, NULL, res, params);
    }
}

// Copies the tiles of layer src to layer dst.
void TileMap_804AA858::fn_802F6BBC(s32 src, s32 dst) {
    u16* from;
    if (src >= 0 && src < 4) {
        from = mLayerGrid[src];
    } else if (src >= 4 && src < 8) {
        from = mLayerGrid2[src - 4];
    } else {
        from = mLayerGrid[src];
    }
    u16* to;
    if (dst >= 0 && dst < 4) {
        to = mLayerGrid[dst];
    } else if (dst >= 4 && dst < 8) {
        to = mLayerGrid2[dst - 4];
    } else {
        to = mLayerGrid[dst];
    }
    for (int i = 0; i < 0x1000; i++) {
        to[i] = from[i];
    }
}

// The same for field_0xc220 (drawn by fn_802FF3F0), clearing byte 0x21.
// The u8 return with a bool local gives the original's final clrlwi (both bool and both u8 measured as different).
u8 TileMap_804AA858::fn_802F654C(const JGeometry::TVec3f* pos, void* res, s32 frame, void* image,
                                   void* res3, SpriteParams_801F5AE0* params) {
    bool result = false;
    if (field_0xd320 < 0x20) {
        cls_8030029C* sprite = &field_0xc220[field_0xd320];
        sprite->mPos.set(*pos);
        sprite->mRes = res;
        sprite->mFrame = frame;
        sprite->mImage = image;
        sprite->field_0x18 = res3;
        if (params != NULL) {
            sprite->mParams = *params;
            sprite->mParams.field_0x21 = 0;
            if (params->mMtx != NULL) {
                JMath::gekko_ps_copy12(sprite->mMtx, params->mMtx);
                sprite->mParams.mMtx = sprite->mMtx;
            }
        } else {
            sprite->mParams.reset();
        }
        field_0xd320++;
        result = true;
    }
    return result;
}

// Queues a sprite on field_0xd324 (drawn by fn_802FF2D8) with a copy of params
// (its byte 0x22 cleared, its matrix copied into the entry), else the default
// parameters; false when the queue is full.
// The u8 return with a bool local gives the original's final clrlwi (both bool and both u8 measured as different).
u8 TileMap_804AA858::fn_802F6298(const JGeometry::TVec3f* pos, void* res, s32 frame, void* image,
                                   void* res3, SpriteParams_801F5AE0* params) {
    bool result = false;
    if (field_0xe424 < 0x20) {
        cls_8030029C* sprite = &field_0xd324[field_0xe424];
        sprite->mPos.set(*pos);
        sprite->mRes = res;
        sprite->mFrame = frame;
        sprite->mImage = image;
        sprite->field_0x18 = res3;
        if (params != NULL) {
            sprite->mParams = *params;
            sprite->mParams.field_0x22 = 0;
            if (params->mMtx != NULL) {
                JMath::gekko_ps_copy12(sprite->mMtx, params->mMtx);
                sprite->mParams.mMtx = sprite->mMtx;
            }
        } else {
            sprite->mParams.reset();
        }
        field_0xe424++;
        result = true;
    }
    return result;
}

// Starts an effect of `kind` at pos in a free slot of field_0xbf9c, else in
// the one with the smallest timer; kind 3 also keeps the normalized dir.
void TileMap_804AA858::fn_802F601C(const JGeometry::TVec3f* pos, s32 kind, const JGeometry::TVec3f* dir,
                               f32 scaleX, f32 scaleY) {
    s32 minTimer = -1;
    s32 minIndex = 0;
    s32 index = -1;
    for (s32 i = 0; i < 16; i++) {
        cls_803003A8* slot = &field_0xbf9c[i];
        if (slot->mKind == 0) {
            index = i;
            break;
        }
        if (minTimer < 0 || minTimer > slot->mTimer) {
            minTimer = slot->mTimer;
            minIndex = i;
        }
    }
    if (index < 0) {
        index = minIndex;
    }
    cls_803003A8* slot = &field_0xbf9c[index];
    slot->mKind = kind;
    slot->mPos.set(*pos);
    slot->field_0x14 = scaleX;
    slot->field_0x18 = scaleY;
    switch (kind) {
    case 2:
        slot->mTimer = 60;
        break;
    case 1:
        slot->mTimer = 5;
        break;
    case 3:
        slot->mTimer = 18;
        slot->field_0x1c.set(dir->x, dir->y, dir->z);
        slot->field_0x1c.normalize();
        break;
    case 4:
        slot->mKind = 5;
        slot->field_0x14 = -slot->field_0x14;
        slot->mTimer = 1;
        break;
    case 5:
        slot->mTimer = 1;
        break;
    case 6:
        slot->mTimer = 300;
        break;
    }
}

// The scale of a sprite at height z (callers pass mPos.z; fn_802F5040 inlines it).
f32 fn_802F5FF4(f32 z) {
    f32 h;
    if (z >= 96.0f) {
        h = 96.0f;
    } else {
        h = z;
    }
    return 192.0f / (192.0f - h);
}

// FIXME: 98.7. Register allocation only: the 8.0f literal and field_0x2368.y swap
// f5/f6 (and 0.125f is in f6, not f7); the ix * 0xC / iy * 0x30C products swap r0/r3.
f32 TileMap_804AA858::fn_802F5E9C(const JGeometry::TVec3f* pos) {
    f32 z = pos->z;
    if (field_0xc21c > 0 && pos->x >= field_0x2368.x &&
        pos->x < field_0x2368.x + 8.0f * field_0x2380 && pos->y >= field_0x2368.y &&
        pos->y < field_0x2368.y + 8.0f * field_0x2384)
    {
        f32 dx = pos->x - field_0x2368.x;
        f32 dy = pos->y - field_0x2368.y;
        s32 ix = 0.125f * dx;
        s32 iy = 0.125f * dy;
        JGeometry::TVec3f* p = &field_0x100[field_0x2388][iy][ix];
        f32 fx = 0.125f * (dx - 8.0f * ix);
        f32 fy = 0.125f * (dy - 8.0f * iy);
        f32 top = p[0].z * (1.0f - fx) + p[1].z * fx;
        f32 bottom = (1.0f - fx) * p[65].z + fx * p[66].z;
        z += (1.0f - fy) * top + fy * bottom;
    }
    return z;
}

// FIXME: 88.2. Register allocation (the grid test uses f7-f11 for the original's
// f3-f9, the result flag r5 for r6) and the first interpolation step: the original
// scales v before loading p[1] and stores top's z before its x/y. Literals @NNN.
bool TileMap_804AA858::fn_802F58A8(const JGeometry::TVec3f* pos, Mtx mtx) {
    bool result = false;
    if (field_0xc21c > 0 && pos->x >= field_0x2368.x &&
        pos->x < field_0x2368.x + 8.0f * field_0x2380 && pos->y >= field_0x2368.y &&
        pos->y < field_0x2368.y + 8.0f * field_0x2384)
    {
        f32 dx = pos->x - field_0x2368.x;
        f32 dy = pos->y - field_0x2368.y;
        s32 ix = 0.125f * dx;
        s32 iy = 0.125f * dy;
        JGeometry::TVec3f* p = &field_0x100[field_0x2388][iy][ix];
        f32 fx = 0.125f * (dx - 8.0f * ix);
        f32 fy = 0.125f * (dy - 8.0f * iy);
        JGeometry::TVec3f v;
        v.set(p[0]);
        v.scale(1.0f - fx);
        JGeometry::TVec3f top;
        top.set(p[1]);
        top.scale(fx);
        top.add(v);
        top.scale(1.0f - fy);
        v.set(p[65]);
        v.scale(1.0f - fx);
        JGeometry::TVec3f point;
        point.set(p[66]);
        point.scale(fx);
        point.add(v);
        point.scale(fy);
        point.add(top);
        v.sub(p[1], p[0]);
        JGeometry::TVec3f side;
        side.cross(v, JGeometry::TVec3f(0.0f, 1.0f, 0.0f));
        side.normalize();
        side.scale(1.0f - fy);
        v.sub(p[66], p[65]);
        JGeometry::TVec3f front;
        front.cross(v, JGeometry::TVec3f(0.0f, 1.0f, 0.0f));
        front.normalize();
        front.scale(fy);
        front.add(side);
        front.y = 0.0f;
        front.normalize();
        side.cross(JGeometry::TVec3f(0.0f, 1.0f, 0.0f), front);
        mtx[0][0] = side.x;
        mtx[0][1] = 0.0f;
        mtx[0][2] = front.x;
        mtx[0][3] = point.x - dx;
        mtx[1][0] = side.y;
        mtx[1][1] = 1.0f;
        mtx[1][2] = front.y;
        mtx[1][3] = point.y - dy;
        mtx[2][0] = side.z;
        mtx[2][1] = 0.0f;
        mtx[2][2] = front.z;
        mtx[2][3] = point.z;
        result = true;
    }
    return result;
}

// FIXME: 99.2. Register allocation only, as in fn_802F5E9C (inlined here):
// 8.0f and pos->x swap f5/f6.
// The position (out) and parameters (*outParams) a sprite at pos is drawn
// with: on the height grid's tilt (fn_802F58A8, params' matrix concatenated
// into tmp) for field_0x38 2 or no params, raised by the height (fn_802F5E9C)
// for 1, else unchanged.
void TileMap_804AA858::fn_802F5570(const JGeometry::TVec3f* pos, SpriteParams_801F5AE0* params,
                                    JGeometry::TVec3f* out, SpriteParams_801F5AE0** outParams, Mtx mtx,
                                    SpriteParams_801F5AE0* tmp) {
    if (params == NULL || params->field_0x38 == 2) {
        if (fn_802F58A8(pos, mtx)) {
            if (params != NULL) {
                *tmp = *params;
                if (tmp->mMtx != NULL) {
                    PSMTXConcat(tmp->mMtx, mtx, mtx);
                }
            }
            tmp->mMtx = mtx;
            *outParams = tmp;
        } else {
            *outParams = params;
        }
        out->set(*pos);
    } else if (params->field_0x38 == 1) {
        out->set(pos->x, pos->y, pos->z + fn_802F5E9C(pos));
        *outParams = params;
    } else {
        out->set(*pos);
        *outParams = params;
    }
}

// FIXME: 76.5. The original copies the 0x68-byte record into field_0x11c[slot]
// with a 13-iteration double-word loop; ours unrolls a word copy. Registers also
// differ (record and slot in r24 / r23, original r23 / r26).
void TileMap_804AA858::fn_802F53F4(s32 slot, const cls_804AA858Sub11C* record) {
    if (record->mValid == 0) {
        return;
    }
    if (record->mKind == 5 || record->mKind == 14) {
        *field_0x11c[slot] = *record;
        if (record->mMode == 2) {
            field_0x134[slot] = record->field_0x5_7;
        } else if (record->mMode == 3) {
            field_0x130[slot] = record->field_0x4_5;
        }
        if (record->mMode & 1) {
            for (s32 i = 0; i < 4; i++) {
                lbl_80506DA8[slot][i] = (record->field_0x6 >> (i * 2)) & 3;
                lbl_80506DB8[slot][i] = (record->field_0x7_4 >> i) & 1;
                lbl_80506DC8[slot][i] = (record->field_0x7_0 >> i) & 1;
                lbl_80506DD8[slot] = fn_8022EA0C(slot);
            }
        }
    }
    field_0x12c[slot] = 1;
    fn_803068D0(field_0x11c[slot]);
}

// FIXME: 99.6. The z > 0.0f flag is extracted to r0 and moved to r3 (one mr more;
// the original extracts straight into r3). Literals match.
// Sets up the GX state of a sprite at pos: the view matrix (params' matrix,
// scaled for the height like fn_802F5FF4, moved to pos) or the plain camera,
// the blend mode, the TEV colours and the z mode. out is where the sprite is
// drawn relative to the matrix.
void fn_802F5040(const JGeometry::TVec3f* pos, SpriteParams_801F5AE0* params, JGeometry::TVec3f* out) {
    f32 offset;
    if (params != NULL) {
        offset = getDrawOffset(params, pos);
    } else {
        offset = 0.0f;
    }
    if ((params == NULL || params->mMtx == NULL || !params->field_0x25) &&
        ((params != NULL && params->mMtx != NULL) ||
         (0.0f != pos->z && (params == NULL || params->field_0x1f))))
    {
        Mtx m;
        if (params != NULL && params->mMtx != NULL) {
            JMath::gekko_ps_copy12(m, params->mMtx);
        } else {
            PSMTXIdentity(m);
        }
        if (0.0f != pos->z && (params == NULL || params->field_0x1f)) {
            f32 scale = fn_802F5FF4(pos->z);
            m[0][0] *= scale;
            m[1][0] *= scale;
            m[2][0] *= scale;
            m[0][1] *= scale;
            m[1][1] *= scale;
            m[2][1] *= scale;
            m[0][2] *= scale;
            m[1][2] *= scale;
            m[2][2] *= scale;
            m[0][3] *= scale;
            m[1][3] *= scale;
            m[2][3] *= scale;
        }
        m[0][3] += pos->x;
        m[1][3] += pos->y + offset;
        m[2][3] += pos->z + offset;
        gGameRoot2_8053AB10->fn_8013B3CC(m);
        out->x = out->y = out->z = 0.0f;
    } else {
        gGameRoot2_8053AB10->fn_8013B3F0();
        out->set(pos->x, pos->y + offset, pos->z + offset);
    }
    gGameRoot2_8053AB10->fn_8013B1E8();
    if (params != NULL && params->field_0x34 == 1) {
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_NOOP);
    } else if (params != NULL && params->field_0x34 == 2) {
        GXSetBlendMode(GX_BM_BLEND, GX_BL_DSTALPHA, GX_BL_INVDSTALPHA, GX_LO_NOOP);
    } else {
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    }
    if (params != NULL) {
        GXSetTevColor(GX_TEVREG0, params->field_0x10);
        GXSetTevColor(GX_TEVREG1, params->field_0x14);
        GXSetZMode(params->field_0x26, GX_LEQUAL, params->field_0x2c);
    }
    if (params != NULL && params->field_0x2d) {
        GXSetAlphaUpdate(GX_TRUE);
    }
}
