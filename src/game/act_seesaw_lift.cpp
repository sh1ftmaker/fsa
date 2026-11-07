#include "game/GameAudio.h"
#include "game/act/act_seesaw_lift.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/BgRewrite.h"
#include "game/Map.h"
#include "game/Effect.h"
#include "game/GameRootSub20.h"
#include "JSystem/JMath/JMath.h"
#include <math.h>

// The state table in the unit's .data (0x804AD30C). It is zeros in the DOL and
// filled at runtime, so it is declared here and not defined.
extern void (SeesawLift_804AD348::*lbl_804AD30C[5])();

static const char* lbl_8053A1B8[2] = {"formation_lift.bti", "sand5.bti"};

// Writes tile `tile` over the lift's 6 x 6 tile area.
static inline void fillTiles(SeesawLift_804AD348* lift, u16 tile) {
    JGeometry::TVec3f p = lift->mPos;
    p.x += lift->mBox.i.x;
    p.y += lift->mBox.i.y;
    f32 baseY = p.y;
    lift->field_0x258 = 0;
    for (s32 row = 0; row < 6; row++) {
        for (s32 col = 0; col < 6; col++) {
            gBgRewrite_8053AC98->fn_802A381C(&p, lift->field_0x4, tile);
            p.y += 16.0f;
        }
        p.y = baseY;
        p.x += 16.0f;
    }
}

// FIXME(shotgun): 70.9 %, missing/extra: the tile-clearing loop start position and the box arithmetic differ (18 instructions); the two words past the 0x274 allocation (0x274, 0x278) are not stored (nor the first of its two stores to 0x26c)
void SeesawLift_804AD348::init() {
    field_0x234 = gGameRoot_8053AAF8->field_0xa4->getResource('????', lbl_8053A1B8[field_0x90 & 0x7F]);
    const u16* res = (const u16*)field_0x234;
    u16 width = res[1];
    u16 height = res[2];
    mBox.i.x = (f32)(-(s32)width / 2);
    mBox.i.y = (f32)(-(s32)height / 2);
    mBox.f.x = (f32)(width >> 1);
    mBox.f.y = (f32)(height >> 1);
    mLife = 0;
    field_0x230 = 0;
    field_0x238 = 0;
    field_0x23a = 0;
    field_0x23c = 0;
    field_0x23e = 0x18;
    field_0x240 = 0.0f;
    field_0x244 = 0;
    field_0x245 = 0;
    field_0x24e = 0;
    field_0x246[0] = 0;
    field_0x24a[0] = 0;
    field_0x246[1] = 0;
    field_0x24a[1] = 0;
    field_0x246[2] = 0;
    field_0x24a[2] = 0;
    field_0x246[3] = 0;
    field_0x24a[3] = 0;
    field_0x250 = 0;
    field_0x25c = 0;
    field_0x260 = 0;
    field_0x264 = 0;
    field_0x268 = 0;
    field_0x26c = 0;
    field_0x270 = 0;
    JGeometry::TVec3f p = mPos;
    p.x += mBox.i.x;
    p.y += mBox.i.y;
    f32 baseY = p.y;
    field_0x258 = 0;
    for (s32 row = 0; row < 6; row++) {
        for (s32 col = 0; col < 6; col++) {
            gBgRewrite_8053AC98->fn_802A381C(&p, field_0x4, 0x360);
            p.y += 16.0f;
        }
        p.y = baseY;
        p.x += 16.0f;
    }
}

// FIXME(shotgun): 81.1 %, missing/extra: scheduling of the player loop
s32 SeesawLift_804AD348::fn_8035AE9C(s32 player) {
    if (fn_8022D75C(player)) {
        return 0;
    }
    if (fn_80230188(player)) {
        return 0;
    }
    if (getPlayerLayer_8023DE58(player) != field_0x4) {
        return 0;
    }
    if (getPlayerPos_8023E724(player)->z > 0.0f) {
        fn_8022DF48(player);
        return 0;
    }
    for (s32 i = 0; i < 8; i++) {
        if ((field_0x230 >= 0x3C00 && field_0x230 <= 0x4400) || (field_0x230 <= -0x3C00 && field_0x230 >= -0x4400)) {
            return 0;
        }
        JGeometry::TVec3f rel;
        fn_8022E088(player, i, &rel);
        rel.x -= mPos.x;
        rel.y -= mPos.y;
        rel.z -= mPos.z;
        rel.z = 0.0f;
        f32 cosVal = __fabs(JMath::sincosTable_.cosShort(field_0x230));
        bool inside = false;
        if (rel.x >= mBox.i.x && rel.y >= mBox.i.y * cosVal && rel.x < mBox.f.x && rel.y < mBox.f.y * cosVal) {
            inside = true;
        }
        if (inside) {
            fn_8022DF48(player);
            f32 d = mPos.y - getPlayerPos_8023E724(player)->y;
            if (field_0x24e && d == 0.0f) {
                d = 0.1f;
            }
            if (!field_0x24e && __fabs(d) <= 8.0f) {
                d = 0.0f;
            }
            field_0x240 += d;
            fn_8035A860(d, player);
            if (d >= 0.0f) {
                return 1;
            }
            return -1;
        }
    }
    return 0;
}

// FIXME(shotgun): 79.6 %, missing/extra: scheduling of the player loop
s32 SeesawLift_804AD348::fn_8035AC84(s32 player) {
    if (fn_8022D75C(player)) {
        return 0;
    }
    if (fn_80230188(player)) {
        return 0;
    }
    if (getPlayerLayer_8023DE58(player) != field_0x4) {
        return 0;
    }
    if (getPlayerPos_8023E724(player)->z > 0.0f) {
        fn_8022DF48(player);
        return 0;
    }
    for (s32 i = 0; i < 8; i++) {
        if ((field_0x230 >= 0x3C00 && field_0x230 <= 0x4400) || (field_0x230 <= -0x3C00 && field_0x230 >= -0x4400)) {
            return 0;
        }
        JGeometry::TVec3f rel;
        fn_8022E088(player, i, &rel);
        rel.x -= mPos.x;
        rel.y -= mPos.y;
        rel.z -= mPos.z;
        rel.z = 0.0f;
        f32 cosVal = __fabs(JMath::sincosTable_.cosShort(field_0x230));
        bool inside = false;
        if (rel.x >= mBox.i.x && rel.y >= mBox.i.y * cosVal && rel.x < mBox.f.x && rel.y < mBox.f.y * cosVal) {
            inside = true;
        }
        if (inside) {
            fn_8022DF48(player);
            f32 d = mPos.y - getPlayerPos_8023E724(player)->y;
            if (d == 0.0f) {
                d = 0.1f;
            }
            field_0x240 += d;
            fn_8035A860(d, player);
            if (d >= 0.0f) {
                return 1;
            }
            return -1;
        }
    }
    return 0;
}

// FIXME(shotgun): 88.6 %, missing/extra: the branches on field_0x240
void SeesawLift_804AD348::fn_8035AA54() {
    bool near = false;
    bool moved = false;
    bool all = false;
    if (field_0x246[0] && field_0x246[1] && field_0x246[2] && field_0x246[3]) {
        all = true;
    }
    if (all && __fabs(field_0x240) < 33.0f) {
        near = true;
    }
    bool flag = near;
    if (!gGameRoot_8053AAF8->field_0x7e) {
        flag = near && fn_8022CAF8(0) == 1;
    }
    if (field_0x240 != 0.0f) {
        if (flag || field_0x250 > 0) {
            if (!gGameRoot_8053AAF8->field_0x7e) {
                if (field_0x240 > 0.0f) {
                    field_0x240 = -1500.0f;
                } else {
                    field_0x240 = 1500.0f;
                }
            } else {
                if (field_0x240 > 0.0f) {
                    field_0x240 = -10.0f;
                } else {
                    field_0x240 = 10.0f;
                }
            }
            moved = true;
        } else {
            if (!gGameRoot_8053AAF8->field_0x7e) {
                if (field_0x240 > 0.0f) {
                    field_0x240 = 1500.0f;
                } else {
                    field_0x240 = -1500.0f;
                }
            } else {
                if (field_0x240 > 0.0f) {
                    field_0x240 = 10.0f;
                } else {
                    field_0x240 = -10.0f;
                }
            }
        }
        field_0x23a = (s32)field_0x240;
    } else {
        field_0x23a = (s32)((-(0.1f * (f32)field_0x238) - 0.01f * (f32)field_0x230) / 15.0f);
    }
    field_0x238 += field_0x23a;
    if (moved) {
        field_0x240 = 0.0f;
    }
}

// FIXME(shotgun): 73.1 %, missing/extra: the inside test and the table lookups (sincosTable_ index form)
// Not auto-inlined: fn_8035AC84 and fn_8035AE9C call fn_8035A860 out of line in the
// original (docs/INLINING.md, pragma rule 1).
#pragma auto_inline off
void SeesawLift_804AD348::fn_8035A860(f32 weight, s32 player) {
    JGeometry::TVec3f rel = *getPlayerPos_8023E724(player);
    rel.x -= mPos.x;
    rel.y -= mPos.y;
    rel.z -= mPos.z;
    rel.z = 0.0f;
    bool inside = false;
    if (rel.x >= mBox.i.x + 7.0f && rel.y >= mBox.i.y && rel.x < mBox.f.x - 7.0f && rel.y < mBox.f.y) {
        inside = true;
    }
    if (inside) {
        f32 sinVal = JMath::sincosTable_.sinShort(field_0x230);
        f32 cosVal = JMath::sincosTable_.cosShort(field_0x230);
        f32 slide = 0.3f * __fabs(sinVal) * (__fabs(weight) / (mBox.f.y * __fabs(cosVal)));
        if (slide > 0.3f) {
            slide = 0.3f;
        }
        f32 scale;
        if (!field_0x244) {
            if ((f32)field_0x230 * weight >= 0.0f) {
                scale = 1.0f - slide;
            } else {
                scale = 1.0f + slide;
            }
        } else {
            if ((f32)field_0x230 * weight >= 0.0f) {
                scale = 1.0f + slide;
            } else {
                scale = 1.0f - slide;
            }
        }
        JGeometry::TVec3f v(scale, 1.0f, scale);
        fn_8022BE24(player, &v);
    }
}
#pragma auto_inline reset

// FIXME(shotgun): 88.9 %, missing/extra + reloc-name
void SeesawLift_804AD348::fn_8035A210() {
    field_0x240 = 0.0f;
    for (s32 p = 0; p < 4; p++) {
        field_0x246[p] = 0;
        field_0x24a[p] = 0;
        if (fn_8023F264(p)) {
            field_0x246[p] = fn_8035AE9C(p);
            JGeometry::TBox2f box;
            fn_801F317C(&box);
            f32 centerY = mPos.y;
            f32 c = __fabs(JMath::sincosTable_.cosShort(field_0x230));
            box.i.y = (box.i.y - centerY) * c + centerY;
            box.f.y = (box.f.y - centerY) * c + centerY;
            JGeometry::TVec2f out;
            if (fn_8023AAF8(p, &box, field_0x4, &out)) {
                f32 d = centerY - out.y;
                if (field_0x24e && d == 0.0f) {
                    d = 0.1f;
                }
                if (!field_0x24e && __fabs(d) <= 8.0f) {
                    d = 0.0f;
                }
                field_0x240 += 20.0f * d;
                if (d >= 0.0f) {
                    field_0x24a[p] = 1;
                } else {
                    field_0x24a[p] = -1;
                }
            } else {
                field_0x24a[p] = 0;
            }
        }
    }
    if ((field_0x24a[0] | field_0x24a[1] | field_0x24a[2] | field_0x24a[3]) != 0 && field_0x240 != 0.0f) {
        fn_801F0D20(3);
    }
    for (s32 p = 0; p < 4; p++) {
        if (fn_8023F264(p) && field_0x246[p] != 0 && !fn_8022D75C(p) && !fn_80230188(p) && !fn_8022F514(p)) {
            f32 sinVal = JMath::sincosTable_.sinShort(field_0x230);
            f32 push = 1.8f * (sinVal * __fabs(sinVal));
            if (gGameRoot_8053AAF8->field_0x7e != 0 && ((field_0x90 >> 12) & 0x1F) >= 2) {
                push *= 5.5f;
            }
            if (field_0x244 == 0) {
                if (field_0x258 == 1) {
                    JGeometry::TVec3f v(0.0f, -1.0f * push, 0.0f);
                    fn_8023DF68(p, &v);
                }
            } else if (field_0x258 == 1) {
                JGeometry::TVec3f v(0.0f, push, 0.0f);
                fn_8023DF68(p, &v);
            }
        }
    }
    for (s32 p = 0; p < 4; p++) {
        if (field_0x24a[p] != 0) {
            for (s32 q = 0; q < 4; q++) {
                if (!fn_8022F514(q) && field_0x24a[q] == 0 && field_0x246[q] != 0 && field_0x24a[p] * field_0x246[q] < 0) {
                    fn_802314D8(q, JGeometry::TVec3f(0.0f, 0.0f, 10.0f));
                }
            }
        }
    }
    if (field_0x244 == 0) {
        field_0x23a = (s16)((field_0x240 - 0.15f * (f32)field_0x238 - 0.008f * (f32)field_0x230) / 15.0f);
    } else if (field_0x230 > 0) {
        s16 mirrored = field_0x230 - 0x8000;
        field_0x23a = (s16)((field_0x240 - 0.15f * (f32)field_0x238 - 0.008f * (f32)mirrored) / 15.0f);
    } else {
        s16 mirrored = field_0x230 + 0x8000;
        field_0x23a = (s16)((field_0x240 - 0.15f * (f32)field_0x238 - 0.008f * (f32)mirrored) / 15.0f);
    }
    if (field_0x238 == 0 && field_0x23a != 0) {
        fillTiles(this, 12);
        field_0x258 = 1;
    }
    field_0x238 += field_0x23a;
    s16 oldAngle = field_0x230;
    field_0x230 += field_0x238;
    if (field_0x238 < 0) {
        if ((oldAngle > 0x4000 && field_0x230 <= 0x4000) || (oldAngle >= -0x4000 && field_0x230 < -0x4000)) {
            field_0x244 ^= 1;
        }
    } else if (field_0x238 > 0) {
        if ((oldAngle <= 0x4000 && field_0x230 > 0x4000) || (oldAngle < -0x4000 && field_0x230 >= -0x4000)) {
            field_0x244 ^= 1;
        }
    }
}

void SeesawLift_804AD348::update() {
    (this->*lbl_804AD30C[(field_0x90 >> 7) & 0x1F])();
}

// FIXME(shotgun): 68.8 %, missing/extra: the Euler matrix is the plain RotMatrix.h product with table sin/cos; written by hand here; the original also scales every element by a float (lbl_80542878) that is not modelled
void SeesawLift_804AD348::draw() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    mtx[0][0] = 0.0f;
    mtx[1][1] = 0.0f;
    mtx[2][2] = 0.0f;
    s32 type = (field_0x90 >> 7) & 0x1F;
    s16 angle = field_0x230;
    if (type == 4 && field_0x230 == 0) {
        angle = field_0x23c;
    }
    f32 sz = JMath::sincosTable_.sinShort(angle);
    f32 cz = JMath::sincosTable_.cosShort(angle);
    f32 sx = JMath::sincosTable_.sinShort(0);
    f32 cx = JMath::sincosTable_.cosShort(0);
    mtx[0][0] = cx * cx;
    mtx[1][0] = cx * sz;
    mtx[2][0] = -sx;
    mtx[0][1] = sx * sx * cz - cx * sz;
    mtx[1][1] = sx * sx * sz + cx * cz;
    mtx[2][1] = sx * cx;
    mtx[0][2] = cx * cz * sx + sx * sz;
    mtx[1][2] = cx * sz * sx - sx * cz;
    mtx[2][2] = cx * cx;
    params.mMtx = mtx;
    params.field_0x21 = 1;
    params.field_0x1d = -2;
    params.field_0x25 = 1;
    if ((field_0x90 & 0x7F) == 1) {
        params.field_0x24 = 1;
    }
    const char* name = lbl_8053A1B8[field_0x90 & 0x7F];
    if (field_0x4 < 8) {
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('TIMG', name);
        gGameRoot2_8053AB10->field_0x20->fn_802F745C(&mPos, res, &params);
    }
    if (type < 2) {
        params.field_0x1c = -1;
        params.field_0x1d = -2;
        mtx[0][0] = 1.5f;
        mtx[0][1] = 0.0f;
        mtx[0][2] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = 1.5f;
        mtx[1][2] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = 1.5f;
        params.mMtx = mtx;
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('TIMG', "otosiana_bg_s.bti");
        JGeometry::TVec3f pos(mPos.x, mPos.y, 0.0f);
        if (field_0x4 < 8) {
            gGameRoot2_8053AB10->field_0x20->fn_802F745C(&pos, res, &params);
        }
    }
}

// FIXME(shotgun): 79.8 %, missing/extra + reloc-name
void SeesawLift_804AD348::fn_803597EC() {
    field_0x240 = 0.0f;
    field_0x24e = 0;
    if (field_0x254 != 0) {
        field_0x254--;
        if (field_0x254 == 0) {
            fn_801F0D20(3);
        }
    }
    s32 mask = 0;
    for (s32 p = 0; p < 4; p++) {
        field_0x246[p] = 0;
        field_0x24a[p] = 0;
        if (fn_8023F264(p)) {
            field_0x246[p] = fn_8035AE9C(p);
            if (fn_80235448(p) && field_0x246[p] != 0) {
                field_0x24e |= 1 << p;
                JGeometry::TBox2f box;
                fn_801F317C(&box);
                f32 centerY = mPos.y;
                f32 c = __fabs(JMath::sincosTable_.cosShort(field_0x230));
                box.i.y = (box.i.y - centerY) * c + centerY;
                box.f.y = (box.f.y - centerY) * c + centerY;
                JGeometry::TVec2f out;
                if (fn_8023AAF8(p, &box, field_0x4, &out)) {
                    f32 d = centerY - out.y;
                    if (field_0x24e && d == 0.0f) {
                        d = 0.1f;
                    }
                    if (!field_0x24e && __fabs(d) <= 8.0f) {
                        d = 0.0f;
                    }
                    field_0x240 += 20.0f * d;
                    if (d >= 0.0f) {
                        field_0x24a[p] = 1;
                    } else {
                        field_0x24a[p] = -1;
                    }
                } else {
                    field_0x24a[p] = 0;
                }
            }
        }
        mask |= field_0x24e;
        mask |= field_0x24a[p] << p;
    }
    if (mask != 0) {
        if (field_0x238 == 0 && field_0x240 != 0.0f) {
            field_0x254 = 30;
            fillTiles(this, 12);
            field_0x258 = 1;
            if (field_0x240 > 0.0f) {
                field_0x238 = 0x200;
            } else if (field_0x240 < 0.0f) {
                field_0x238 = -0x200;
            }
        }
    }
    for (s32 p = 0; p < 4; p++) {
        if (fn_8023F264(p) && field_0x246[p] != 0 && !fn_8022D75C(p) && !fn_80230188(p) && !fn_8022F514(p)) {
            f32 sinVal = JMath::sincosTable_.sinShort(field_0x230);
            f32 push = 1.8f * (sinVal * __fabs(sinVal));
            if (gGameRoot_8053AAF8->field_0x7e != 0 && ((field_0x90 >> 12) & 0x1F) >= 2) {
                push *= 5.5f;
            }
            if (field_0x244 == 0) {
                if (field_0x258 == 1) {
                    JGeometry::TVec3f v(0.0f, -1.0f * push, 0.0f);
                    fn_8023DF68(p, &v);
                }
            } else if (field_0x258 == 1) {
                JGeometry::TVec3f v(0.0f, push, 0.0f);
                fn_8023DF68(p, &v);
            }
        }
    }
    for (s32 p = 0; p < 4; p++) {
        if (field_0x24a[p] != 0) {
            for (s32 q = 0; q < 4; q++) {
                if (!fn_8022F514(q) && field_0x24a[q] == 0 && field_0x246[q] != 0 && field_0x24a[p] * field_0x246[q] < 0) {
                    fn_802314D8(q, JGeometry::TVec3f(0.0f, 0.0f, 10.0f));
                }
            }
        }
    }
    s16 oldAngle = field_0x230;
    field_0x230 += field_0x238;
    if (field_0x238 < 0) {
        if ((oldAngle > 0x4000 && field_0x230 <= 0x4000) || (oldAngle >= -0x4000 && field_0x230 < -0x4000)) {
            field_0x244 ^= 1;
        }
    } else if (field_0x238 > 0) {
        if ((oldAngle <= 0x4000 && field_0x230 > 0x4000) || (oldAngle < -0x4000 && field_0x230 >= -0x4000)) {
            field_0x244 ^= 1;
        }
    }
    if (field_0x230 == 0 || field_0x230 == -0x8000) {
        if (field_0x238 != 0) {
            fillTiles(this, 864);
            field_0x238 = 0;
        }
    }
}

// FIXME(shotgun): 82.5 %, missing/extra + reloc-name
void SeesawLift_804AD348::fn_80358F98() {
    if (field_0x245) {
        GameRootSub20_801902C0* sub = fn_801902C0();
        if (sub->field_0x79c < 16) {
            GameRootSub20_801902C0::SandFall& fall = sub->mSandFalls[sub->field_0x79c];
            fall.mPos.x = mPos.x;
            fall.mPos.y = mPos.y;
            fall.mPos.z = mPos.z;
            sub->mSandFalls[sub->field_0x79c].field_0xc[0] = 0;
            sub->field_0x79c++;
        }
    }
    field_0x240 = 0.0f;
    for (s32 p = 0; p < 4; p++) {
        field_0x246[p] = 0;
        field_0x24a[p] = 0;
        if (fn_8023F264(p)) {
            field_0x246[p] = fn_8035AE9C(p);
            JGeometry::TBox2f box;
            fn_801F317C(&box);
            f32 centerY = mPos.y;
            f32 c = __fabs(JMath::sincosTable_.cosShort(field_0x230));
            box.i.y = (box.i.y - centerY) * c + centerY;
            box.f.y = (box.f.y - centerY) * c + centerY;
            JGeometry::TVec2f out;
            if (fn_8023AAF8(p, &box, field_0x4, &out)) {
                f32 d = centerY - out.y;
                if (field_0x24e && d == 0.0f) {
                    d = 0.1f;
                }
                if (!field_0x24e && __fabs(d) <= 8.0f) {
                    d = 0.0f;
                }
                field_0x240 += 20.0f * d;
                if (d >= 0.0f) {
                    field_0x24a[p] = 1;
                } else {
                    field_0x24a[p] = -1;
                }
            } else {
                field_0x24a[p] = 0;
            }
        }
    }
    if ((field_0x24a[0] | field_0x24a[1] | field_0x24a[2] | field_0x24a[3]) != 0 && field_0x240 != 0.0f) {
        fn_801F0D20(3);
    }
    for (s32 p = 0; p < 4; p++) {
        if (fn_8023F264(p) && field_0x246[p] != 0 && !fn_8022D75C(p) && !fn_80230188(p) && !fn_8022F514(p)) {
            f32 sinVal = JMath::sincosTable_.sinShort(field_0x230);
            f32 push = 1.8f * (sinVal * __fabs(sinVal));
            if (gGameRoot_8053AAF8->field_0x7e != 0 && ((field_0x90 >> 12) & 0x1F) >= 2) {
                push *= 5.5f;
            }
            if (field_0x244 == 0) {
                if (field_0x258 == 1) {
                    JGeometry::TVec3f v(0.0f, -1.0f * push, 0.0f);
                    fn_8023DF68(p, &v);
                }
            } else if (field_0x258 == 1) {
                JGeometry::TVec3f v(0.0f, push, 0.0f);
                fn_8023DF68(p, &v);
            }
        }
    }
    for (s32 p = 0; p < 4; p++) {
        if (field_0x24a[p] != 0) {
            for (s32 q = 0; q < 4; q++) {
                if (!fn_8022F514(q) && field_0x24a[q] == 0 && field_0x246[q] != 0 && field_0x24a[p] * field_0x246[q] < 0) {
                    fn_802314D8(q, JGeometry::TVec3f(0.0f, 0.0f, 10.0f));
                }
            }
        }
    }
    if (field_0x240 != 0.0f) {
        if (gGameRoot_8053AAF8->field_0x7e == 0) {
            if (field_0x240 > 0.0f) {
                field_0x240 = 1500.0f;
            } else {
                field_0x240 = -1500.0f;
            }
        } else if (field_0x240 > 0.0f) {
            field_0x240 = 10.0f;
        } else {
            field_0x240 = -10.0f;
        }
        field_0x23a = (s16)field_0x240;
    } else {
        field_0x23a = (s16)((-(0.1f * (f32)field_0x238) - 0.01f * (f32)field_0x230) / 15.0f);
    }
    field_0x238 += field_0x23a;
    s16 oldAngle = field_0x230;
    field_0x230 += field_0x238;
    if (((field_0x90 >> 12) & 0x1F) == 2) {
        if (field_0x230 <= -0x3E00) {
            field_0x230 = -0x3E00;
            field_0x238 = 0;
            field_0x23a = 0;
        } else if (field_0x230 >= 0) {
            field_0x230 = 0;
            field_0x238 = 0;
            field_0x23a = 0;
            field_0x245 = 0;
            fillTiles(this, 864);
        }
    } else {
        if (field_0x230 >= 0x3E00) {
            field_0x230 = 0x3E00;
            field_0x238 = 0;
            field_0x23a = 0;
        } else if (field_0x230 <= 0) {
            field_0x230 = 0;
            field_0x238 = 0;
            field_0x23a = 0;
            field_0x245 = 0;
            fillTiles(this, 864);
        }
    }
    if (oldAngle == 0 && field_0x230 != 0) {
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x3DE, 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x3DF, 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x3E0, 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x3E1, 0, 0, NULL, NULL, true, 1.0f);
        field_0x245 = 1;
        fillTiles(this, 12);
        field_0x258 = 1;
    }
}

SeesawLift_804AD348::~SeesawLift_804AD348() {}
