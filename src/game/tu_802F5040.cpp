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

// The TEV register colours of the sprite and tile GX setups (the user of each
// in the comment). File-scope objects: they come before the unit's literal
// pool in .sdata2 (a function's own constants are pooled in first-use order,
// after the floats of fn_802F5040), and the two white ones are separate.
static const GXColor lbl_805414E8 = {0xFF, 0xFF, 0xFF, 0xFF}; // fn_802FC80C, TEVREG2
static const GXColor lbl_805414EC = {0x00, 0x00, 0x00, 0xFF}; // fn_802FB69C, TEVREG0 (user not written yet)
static const GXColor lbl_805414F0 = {0x00, 0x00, 0x00, 0x7E}; // fn_802FAC10, TEVREG0 (user not written yet)
static const GXColor lbl_805414F4 = {0xFF, 0xFF, 0xFF, 0xFF}; // fn_802F8F0C, TEVREG0

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

// FIXME: 99.6. The z > 0.0f flag is extracted to r0 and moved to r3 (one mr more;
// the original extracts straight into r3). Literals match.
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

// The scale for a height z (callers pass mPos.z).
f32 fn_802F5FF4(f32 z) {
    return getScale(z);
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
        GXSetTevColor(GX_TEVREG0, params->field_0x10.mColor);
        GXSetTevColor(GX_TEVREG1, params->field_0x14.mColor);
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
        *loaded = true;
    } else {
        lbl_8053AB10->fn_8013B3F0();
    }
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
            resSize = lbl_8053AAF8->field_0xa4->getResSize((u8*)res);
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

void TileMap_804AA858::fn_802F9974(s32 layer, s32 index, u16 value) {
    if (layer >= 0 && layer < 4) {
        mLayerGrid[layer][index] = value;
    } else if (layer >= 4 && layer < 8) {
        mLayerGrid2[layer - 4][index] = value;
    } else {
        mLayerGrid[layer][index] = value;
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
            resSize = lbl_8053AAF8->field_0xa4->getResSize((u8*)res);
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
            resSize = lbl_8053AAF8->field_0xa4->getResSize((u8*)res);
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
            resSize = lbl_8053AAF8->field_0xa4->getResSize((u8*)res);
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

// Draws the tile layers: indexed f32 positions / RGBA8 colours of the
// field_0x2388 height grid and the texture coordinates of field_0x110, the
// tile texture times the vertex colour; with field_0xe450 set, three more TEV
// stages put the player's field_0xe428 texture over it. Then each layer whose
// field_0x235c / field_0x2360 byte is set (layers 0-3 at field_0x2368, 4-7 at
// field_0x2374), and with field_0x2365 layer 0 a second time as pass 8.
void TileMap_804AA858::fn_802FC80C() {
    lbl_8053AB10->fn_8013B1E8();
    lbl_8053AB10->fn_8013B438();
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
        if (isGameModeSet() && lbl_8053AAF8->field_0x4 != NULL) {
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

void TileMap_804AA858::fn_802FF3B0() {
    if (field_0xe451 != 0 && lbl_8053AAF8->field_0x4 != NULL) {
        fn_802FA3FC();
    }
}

// FIXME: 88.7. The original copies the rectangles two words at a time (lwz, lwz,
// stw, stw) and builds the full-screen one on the stack while storing it; ours copies
// word by word and drops the stack copy. The loop registers swap as in fn_802FF2D8.
void TileMap_804AA858::fn_802FF3F0() {
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

void TileMap_804AA858::fn_802FF548() {
    fn_802FF798();
}

void TileMap_804AA858::fn_802FF568() {
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
        field_0x2398[i].field_0x0 = 0;
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
