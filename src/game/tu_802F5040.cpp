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

// The palettes of fn_802FF798's 16 colour-index textures per layer.
static const GXTlut lbl_8046F940[16] = {
    GX_TLUT0, GX_TLUT1, GX_TLUT2,  GX_TLUT3,  GX_TLUT4,  GX_TLUT5,  GX_TLUT6,  GX_TLUT7,
    GX_TLUT8, GX_TLUT9, GX_TLUT10, GX_TLUT11, GX_TLUT12, GX_TLUT13, GX_TLUT14, GX_TLUT15,
};

// The scale of a sprite at height z (fn_802F5FF4; fn_802F5040 inlines it).
static inline f32 getScale(f32 z) {
    f32 h;
    if (z >= 96.0f) {
        h = 96.0f;
    } else {
        h = z;
    }
    return 192.0f / (192.0f - h);
}

// The height under pos: the body of fn_802F5E9C, which the original inlined into
// fn_802F5570 (defined before it here, so it cannot be inlined from there; a
// call-through wrapper costs fn_802F5E9C 0.6 %, hence two copies).
inline f32 cls_804AA858::getHeight(const JGeometry::TVec3f* pos) {
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

// FIXME: 99.4. The z > 0.0f flag is extracted to r0 and moved to r3 (the original
// extracts straight into r3); its .sdata2 literals are @NNN until the four colour
// constants of fn_802F8F0C ... fn_802FC80C, which precede the floats, are emitted.
// Sets up the GX state of a sprite at pos: the view matrix (params' matrix,
// scaled for the height like fn_802F5FF4, moved to pos) or the plain camera,
// the blend mode, the TEV colours and the z mode. out is where the sprite is
// drawn relative to the matrix.
void fn_802F5040(const JGeometry::TVec3f* pos, SpriteParams_801F5AE0* params, JGeometry::TVec3f* out) {
    f32 offset;
    if (params != NULL) {
        u8 raised = lbl_8053AB10->field_0x20->field_0xe461;
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
        offset = (a << 10) + (b << 7);
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
            f32 scale = getScale(pos->z);
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
        lbl_8053AB10->fn_8013B3CC(m);
        out->x = out->y = out->z = 0.0f;
    } else {
        lbl_8053AB10->fn_8013B3F0();
        out->set(pos->x, pos->y + offset, pos->z + offset);
    }
    lbl_8053AB10->fn_8013B1E8();
    if (params != NULL && params->field_0x34 == 1) {
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_NOOP);
    } else if (params != NULL && params->field_0x34 == 2) {
        GXSetBlendMode(GX_BM_BLEND, GX_BL_DSTALPHA, GX_BL_INVDSTALPHA, GX_LO_NOOP);
    } else {
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    }
    if (params != NULL) {
        GXSetTevColor(GX_TEVREG0, params->field_0x10.mColor);
        GXSetTevColor(GX_TEVREG1, params->field_0x14.mColor);
        GXSetZMode(params->field_0x26, GX_LEQUAL, params->field_0x2c);
    }
    if (params != NULL && params->field_0x2d) {
        GXSetAlphaUpdate(GX_TRUE);
    }
}

// FIXME: 90.7. Register allocation only: the record and slot sit in r24 / r23
// (original r23 / r26) and the table loop keeps slot * 4 in r26 instead of a
// separate r25.
void cls_804AA858::fn_802F53F4(s32 slot, const cls_804AA858Sub11C* record) {
    if (record->mBits.mValid == 0) {
        return;
    }
    if (record->mBits.mKind == 5 || record->mBits.mKind == 14) {
        *field_0x11c[slot] = *record;
        if (record->mBits.mMode == 2) {
            field_0x134[slot] = record->mBits.field_0x5_7;
        } else if (record->mBits.mMode == 3) {
            field_0x130[slot] = record->mBits.field_0x4_5;
        }
        if (record->mBits.mMode & 1) {
            for (s32 i = 0; i < 4; i++) {
                lbl_80506DA8[slot][i] = (record->mBits.field_0x6 >> (i * 2)) & 3;
                lbl_80506DB8[slot][i] = (record->mBits.field_0x7_4 >> i) & 1;
                lbl_80506DC8[slot][i] = (record->mBits.field_0x7_0 >> i) & 1;
                lbl_80506DD8[slot] = fn_8022EA0C(slot);
            }
        }
    }
    field_0x12c[slot] = 1;
    fn_803068D0(field_0x11c[slot]);
}

// FIXME: 89.4. The copy of params into tmp moves the colour words 0x10 / 0x14 as
// words (SpriteParamsColor_801F5AE0 is a union with an s32); the original copies
// them byte by byte (four u8). In the inlined getHeight 8.0f and field_0x2368.y swap
// f5/f6 as in fn_802F5E9C. Its .sdata2 literals are @NNN until .sdata2 is in order.
// Where and how a sprite at pos is drawn: params' field_0x38 selects the
// ground's tilt (2, or no params: fn_802F58A8's matrix, concatenated with
// params' own one into the copy tmp), the ground's height (1) or pos as is.
void cls_804AA858::fn_802F5570(const JGeometry::TVec3f* pos, SpriteParams_801F5AE0* params,
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
        out->set(pos->x, pos->y, pos->z);
    } else if (params->field_0x38 == 1) {
        f32 z = pos->z + getHeight(pos);
        out->set(pos->x, pos->y, z);
        *outParams = params;
    } else {
        out->set(pos->x, pos->y, pos->z);
        *outParams = params;
    }
}

// FIXME: 88.2. Register allocation (the grid test uses f7-f11 for the original's
// f3-f9, the result flag r5 for r6) and the first interpolation step: the original
// scales v before loading p[1] and stores top's z before its x/y. Literals @NNN.
bool cls_804AA858::fn_802F58A8(const JGeometry::TVec3f* pos, Mtx mtx) {
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
// lbl_80541500 / lbl_805414FC: .sdata2 starts with four colour words of
// fn_802F8F0C, fn_802FAC10, fn_802FB69C and fn_802FC80C (in reverse), not written yet.
f32 fn_802F5FF4(f32 z) {
    return getScale(z);
}

// FIXME: 99.9. Instructions identical; the 32.0f / 0.0f / 0.5f / 3.0f loads of the
// inlined normalize are @NNN until .sdata2 is emitted in order (see fn_802F5FF4).
// Starts an effect of `kind` at pos in a free slot of field_0xbf9c, else in
// the one with the smallest timer; kind 3 also keeps the normalized dir.
void cls_804AA858::fn_802F601C(const JGeometry::TVec3f* pos, s32 kind, const JGeometry::TVec3f* dir,
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
    if (field_0x248 && field_0x24c.x1 != 0) {
        lbl_8053AB10->field_0x24->field_0x144 = field_0x24c;
    } else {
        ScreenRect_802FF3F0 rect;
        rect.x0 = 0;
        rect.y0 = 0;
        rect.x1 = JUTVideo::getManager()->getFbWidth();
        rect.y1 = JUTVideo::getManager()->getEfbHeight();
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
