#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "dolphin/gx.h"
#include "JSystem/JUtility/JUTTexture.h"
#include "JSystem/JUtility/TColor.h"
#include "JSystem/JKernel/JKRHeap.h"
#include <stdio.h>

// The palettes of fn_802FF798's 16 colour-index textures per layer.
static const GXTlut lbl_8046F940[16] = {
    GX_TLUT0, GX_TLUT1, GX_TLUT2,  GX_TLUT3,  GX_TLUT4,  GX_TLUT5,  GX_TLUT6,  GX_TLUT7,
    GX_TLUT8, GX_TLUT9, GX_TLUT10, GX_TLUT11, GX_TLUT12, GX_TLUT13, GX_TLUT14, GX_TLUT15,
};
#include "JSystem/JKernel/JKRArchive.h"
#include "JSystem/JKernel/JKRDecomp.h"
#include "dolphin/os.h"
#include "game/Camera.h"
#include "game/GameMode.h"
#include "game/Player.h"
#include "JSystem/JUtility/JUTVideo.h"

// FIXME: 98.4. The 8.0f literal and field_0x2368.y swap f5/f6 (and 0.125f is in f6, not f7); the
// ix * 0xC / iy * 0x30C products swap r0/r3. Its .sdata2 literals are @NNN until the
// functions before it (fn_802F5040 ...) emit the unit's literals in order.
f32 cls_804AA858::fn_802F5E9C(const JGeometry::TVec3f* pos) {
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

// The scale for a height z (callers pass mPos.z).
// FIXME: 99.0. Instructions identical; the 96.0f / 192.0f loads are @NNN, not
// lbl_80541500 / lbl_805414FC, until the earlier functions emit .sdata2 in order.
f32 fn_802F5FF4(f32 z) {
    f32 h;
    if (z >= 96.0f) {
        h = 96.0f;
    } else {
        h = z;
    }
    return 192.0f / (192.0f - h);
}

// FIXME: 84.4. In the unrolled nibble-swap loop the original loads the next
// source byte before storing the previous one (ours stores, then loads), and the
// computed length lives in r26 (ours r31; the parameters move up one register).
void cls_804AA858::fn_802F9714(void* res, u32 srcOffset, s32 slot, u32 dstOffset, u32 size) {
    u32 len;
    if (size == 0) {
        s32 resSize;
        if (JKRDecomp::checkCompressed((u8*)res)) {
            resSize = JKRDecompExpandSize((u8*)res);
        } else {
            resSize = lbl_8053AAF8->field_0xa4->getResSize((u8*)res);
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

// Layers 0-3 and 4-7 have their own grid arrays; any other layer indexes
// the first array as is.
u16 cls_804AA858::fn_802F9908(s32 layer, s32 index) {
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

void cls_804AA858::fn_802F9974(s32 layer, s32 index, u16 value) {
    if (layer >= 0 && layer < 4) {
        mLayerGrid[layer][index] = value;
    } else if (layer >= 4 && layer < 8) {
        mLayerGrid2[layer - 4][index] = value;
    } else {
        mLayerGrid[layer][index] = value;
    }
}

// FIXME: 95.5. The loop index and the element pointer swap r29/r30 (and the
// stw r29 save moves up one slot); the 1.0f is @NNN until .sdata2 is in order.
void cls_804AA858::fn_802FF2D8() {
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

void cls_804AA858::fn_802FF3B0() {
    if (field_0xe451 != 0 && lbl_8053AAF8->field_0x4 != NULL) {
        fn_802FA3FC();
    }
}

// FIXME: 88.7. The original copies the rectangles two words at a time (lwz, lwz,
// stw, stw) and builds the full-screen one on the stack while storing it; ours copies
// word by word and drops the stack copy. The loop registers swap as in fn_802FF2D8.
void cls_804AA858::fn_802FF3F0() {
    if (field_0x248 && field_0x24c.w != 0) {
        lbl_8053AB10->field_0x24->field_0x144 = field_0x24c;
    } else {
        ScreenRect_802FF3F0 rect;
        rect.x = 0;
        rect.y = 0;
        rect.w = JUTVideo::getManager()->getFbWidth();
        rect.h = JUTVideo::getManager()->getEfbHeight();
        lbl_8053AB10->field_0x24->field_0x144 = rect;
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

void cls_804AA858::fn_802FF548() {
    fn_802FF798();
}

void cls_804AA858::fn_802FF568() {
    fn_802FDDD4();
    if (isGameModeSet() == true) {
        fn_802FCE74();
    }
    for (s32 i = 0; i < 0x100; i++) {
        if (field_0x2398[i].field_0x0 > 0) {
            field_0x2398[i].field_0x0--;
        }
    }
    field_0xbf98 = 1;
    field_0xe461 = 1;
    if (lbl_8053AAF8->field_0x7f == 0) {
        for (s32 i = 0; i < 4; i++) {
            if (fn_8023F264(i)) {
                // tu_80441A1C reads these 2-bit per-player fields the same way.
                u32 mode = (lbl_8053AAF8->field_0x83 >> (i * 2)) & 3;
                if (mode == 2) {
                    field_0xe461 = 0;
                    break;
                }
            }
        }
    }
}

// FIXME: 87.7. Our 0x100- and 0x10-element clearing loops get an entry test
// (cmpwi / bge) the original lacks, and the original keeps the shared zero in r5 / r8
// across them; in the grid loop it loads the float constants before the outer loop
// and the registers differ. Its .sdata2 literals are @NNN until .sdata2 is in order.
void cls_804AA858::fn_802FF798() {
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
    for (s32 i = 0; i < 0x100; i++) {
        field_0x2398[i].field_0x0 = 0;
    }
    field_0xbf98 = 0;
    field_0x2388 = 0;
    for (s32 i = 0; i < 16; i++) {
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

// FIXME: 99.9. Instructions identical; the two file names are @NNN strings, not
// lbl_804711D4 / lbl_804711E4, until the unit's .rodata is emitted in order.
cls_804AA858::cls_804AA858(JKRHeap* heap) {
    heap->getTotalFreeSize();
    for (s32 i = 0; i < 2; i++) {
        field_0x4[i] = new (heap, 0x20) u8[0x8000];
    }
    for (s32 i = 0; i < 3; i++) {
        field_0xc[i] = new (heap, 0x20) u8[0x200];
        field_0x18[i] = new (heap, 0x20) u8[0x200];
    }
    field_0x24 = new (heap, 0x20) u8[0x2000];
    for (s32 i = 0; i < 4; i++) {
        mLayerGrid[i] = new (heap, 0x20) u16[0x1000];
        mLayerGrid2[i] = new (heap, 0x20) u16[0x1000];
    }
    for (s32 i = 0; i < 16; i++) {
        field_0x48[i] = new (heap, 0x20) u8[0x2000];
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
            (ResTIMG*)lbl_8053AAF8->field_0xa4->getResource('????', name), 0);
    }
    field_0xe454 = new (heap, 0) JUTTexture(
        (ResTIMG*)lbl_8053AAF8->field_0xa4->getResource('????', "circle2.bti"), 0);
    field_0xfc = new (heap, 0) cls_804B1668();
    field_0x238c = new (heap, 0x20) u8[0x100];
}

cls_8030029C::cls_8030029C() {}

cls_803003A8::cls_803003A8() {}

cls_803003AC::cls_803003AC() {
    field_0x94 = -1;
    field_0x98 = -1;
}

cls_803004C0::cls_803004C0() {}
