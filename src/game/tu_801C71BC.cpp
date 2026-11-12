#include "game/BgRewrite.h"
#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameMath.h"
#include "game/Map.h"
#include "game/ViewLayer.h"
#include "game/act/act_shadow_link_2.h"
#include "JSystem/JGeometry.h"
#include "JSystem/JMath/JMath.h"

// FIXME(shotgun): 71 %, missing/extra + reloc-name
u8 ShadowLink2_804A43E8::fn_801C976C() {
    u8 start = field_0x358.fn_80214244()->field_0x3;
    field_0x358.fn_80214244()->fn_800FD818(1);
    u8 next = field_0x358.fn_80214244()->field_0x3;
    cls_800FE3EC* a = field_0x358.fn_80214244();
    if (start < a->field_0x2) {
        a->field_0x3 = start;
    }
    field_0x358.fn_80214244()->fn_800FD818(-1);
    u8 prev = field_0x358.fn_80214244()->field_0x3;
    cls_800FE3EC* b = field_0x358.fn_80214244();
    if (start < b->field_0x2) {
        b->field_0x3 = start;
    }
    u8 opt[2];
    opt[1] = prev;
    opt[0] = next;
    u8 pick;
    if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.5f) {
        pick = opt[0];
        s32 layer = field_0x4;
        if (layer != field_0x358.fn_80214244()->fn_800FE25C(pick)) {
            pick = opt[1];
        }
    } else {
        pick = opt[1];
        s32 layer = field_0x4;
        if (layer != field_0x358.fn_80214244()->fn_800FE25C(pick)) {
            pick = opt[0];
        }
    }
    return pick;
}

// FIXME(shotgun): 98 %, scheduling+regalloc + reloc-name
void ShadowLink2_804A43E8::fn_801C9420(JGeometry::TVec2f* dir) {
    f32 startX = mPos.x;
    f32 startY = mPos.y;
    for (int side = 0; side < 2; side++) {
        dir->normalize();
        dir->x *= 8.0f;
        dir->y *= 8.0f;
        f32 stepX = dir->x;
        f32 stepY = dir->y;
        JGeometry::TVec2f cur(startX, startY);
        for (int i = 0; i < 30; i++) {
            JGeometry::TVec2f prev(cur.x, cur.y);
            cur.x += stepX;
            cur.y += stepY;
            if (fn_801CF888(&prev, (const JGeometry::TVec3f*)&cur, field_0x4)) {
                break;
            }
            JGeometry::TVec2f toGoal(field_0x32c.x - cur.x, field_0x32c.y - cur.y);
            toGoal.normalize();
            toGoal.x *= 14.0f;
            toGoal.y *= 14.0f;
            JGeometry::TVec2f probe(cur.x + toGoal.x, cur.y + toGoal.y);
            if (!fn_801CF888(&cur, (const JGeometry::TVec3f*)&probe, field_0x4)) {
                dir->x = cur.x - startX;
                dir->y = cur.y - startY;
                return;
            }
        }
        dir->x = -dir->x;
        dir->y = -dir->y;
    }
    mFlags |= 0x100000;
    dir->x = -dir->x;
    dir->y = -dir->y;
    dir->normalize();
    dir->x *= 8.0f;
    dir->y *= 8.0f;
}

// FIXME(shotgun): register order and scheduling differ; lbl_ constant reloc-name
void ShadowLink2_804A43E8::fn_801C9234(JGeometry::TVec2f* out) {
    f32 nx = mMapHitDir.x;
    f32 ny = mMapHitDir.y;
    s16 velAngle = (s32)(JMath::atanTable_.atan2_(field_0x3c.x, field_0x3c.y) * 10430.378f);
    f32 c1 = JMACosShort(0x4000);
    f32 s1 = JMASinShort(0x4000);
    f32 c2 = JMACosShort(-0x4000);
    f32 s2 = JMASinShort(-0x4000);
    f32 v1x = nx * c1 - ny * s1;
    f32 v1y = nx * s1 + ny * c1;
    f32 v2x = nx * c2 - ny * s2;
    f32 v2y = nx * s2 + ny * c2;
    s32 a1 = (s32)(JMath::atanTable_.atan2_(v1x, v1y) * 10430.378f);
    f32 d1 = __fabsf((f32)(s16)(velAngle - a1));
    s32 a2 = (s32)(JMath::atanTable_.atan2_(v2x, v2y) * 10430.378f);
    f32 d2 = __fabsf((f32)(s16)(velAngle - a2));
    JGeometry::TVec2f dir;
    if (0.000030517578f * d2 < 0.000030517578f * d1) {
        dir.x = v2x;
        dir.y = v2y;
    } else {
        dir.x = v1x;
        dir.y = v1y;
    }
    fn_801C9420(&dir);
    out->x = mPos.x + dir.x;
    out->y = mPos.y + dir.y;
}

// FIXME(shotgun): 59 %, missing/extra + reloc-name
void ShadowLink2_804A43E8::fn_801C8C84(s32 arg1, s32 arg2) {
    if ((u8)arg1 == 0) {
        JGeometry::TVec3f railPos;
        fn_800FE294(&railPos, field_0x358.fn_80214244(), 0xFF);
        field_0x32c.x = railPos.x;
        field_0x32c.y = railPos.y;
        field_0x32c.z = railPos.z;
        if (viewLayerFlag(field_0x4)) {
            if (field_0x4 == field_0x358.fn_80214244()->fn_800FE25C(0xFF) &&
                !field_0x358.fn_80214244()->fn_800FD548(0xFF)) {
                JGeometry::TVec3f pos2;
                fn_800FE294(&pos2, field_0x358.fn_80214244(), 0xFF);
                JGeometry::TVec3f p = pos2;
                if (fn_801CC9F8(&mPos, field_0x4) && fn_8022461C(&p, field_0x4) == 0) {
                    field_0x32c.x = p.x;
                    field_0x32c.y = p.y;
                    field_0x32c.z = p.z;
                } else {
                    field_0x32c.x = p.x;
                    field_0x32c.y = mPos.y;
                    field_0x32c.z = p.z;
                }
            }
            return;
        }
        if ((u8)arg2 != 0) {
            return;
        }
        bool skip = false;
        if ((mFlags & 0x200000) ||
            field_0x358.fn_80214244()->fn_800FD5E8(field_0x358.fn_80214244()->field_0x3)) {
            skip = true;
        } else if (field_0x524 & 0x100) {
            u8 start = field_0x358.fn_80214244()->field_0x3;
            field_0x358.fn_80214244()->fn_800FD818(1);
            u8 next = field_0x358.fn_80214244()->field_0x3;
            if (start < field_0x358.fn_80214244()->field_0x2) {
                field_0x358.fn_80214244()->field_0x3 = start;
            }
            field_0x358.fn_80214244()->fn_800FD818(-1);
            u8 prev = field_0x358.fn_80214244()->field_0x3;
            if (start < field_0x358.fn_80214244()->field_0x2) {
                field_0x358.fn_80214244()->field_0x3 = start;
            }
            u8 opt[2];
            opt[0] = prev;
            opt[1] = next;
            if (field_0x358.fn_80214244()->fn_800FD5E8(opt[0]) ||
                field_0x358.fn_80214244()->fn_800FD5E8(opt[1])) {
                skip = true;
            }
        }
        if (!skip) {
            JGeometry::TVec3f saved = field_0x32c;
            field_0x32c.x += 16.0f * (2.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f);
            field_0x32c.y += 16.0f * (2.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f);
            if (fn_801CAEF4(&field_0x32c, field_0x4)) {
                field_0x32c.x = saved.x;
                field_0x32c.y = saved.y;
                field_0x32c.z = saved.z;
            }
        }
    } else {
        bool ok = true;
        if (field_0x59b != 0 || viewLayerFlag(field_0x4) ||
            ((field_0x524 & 0x100) && field_0x358.fn_80214244()->fn_800FD5E8(0xFF))) {
            ok = false;
        }
        if (!ok) {
            return;
        }
        JGeometry::TVec2f detour;
        fn_801C9234(&detour);
        if ((field_0x524 & 0x800) && !(mFlags & 0x2000000) && !(mFlags & 0x100000) &&
            fn_801CF888(&detour, &field_0x32c, field_0x4)) {
            bool moved = false;
            u8 start = field_0x358.fn_80214244()->field_0x3;
            field_0x358.fn_80214244()->fn_800FD818(1);
            u8 next = field_0x358.fn_80214244()->field_0x3;
            if (start < field_0x358.fn_80214244()->field_0x2) {
                field_0x358.fn_80214244()->field_0x3 = start;
            }
            field_0x358.fn_80214244()->fn_800FD818(-1);
            u8 prev = field_0x358.fn_80214244()->field_0x3;
            if (start < field_0x358.fn_80214244()->field_0x2) {
                field_0x358.fn_80214244()->field_0x3 = start;
            }
            u8 opt[3];
            opt[0] = prev;
            opt[1] = start;
            opt[2] = next;
            if (field_0x358.fn_80214244()->fn_800FD4C0(opt[1], opt[0]) ||
                field_0x358.fn_80214244()->fn_800FD4C0(opt[1], opt[2])) {
                JGeometry::TVec3f railPos;
                mPathCount = 0;
                mFlags &= ~(0x1000 | 0x20000);
                mFlags |= 0x400000;
                fn_800FE294(&railPos, field_0x358.fn_80214244(), 0xFF);
                field_0x32c.x = railPos.x;
                field_0x32c.y = railPos.y;
                field_0x32c.z = railPos.z;
                moved = true;
            }
            if (moved) {
                return;
            }
        }
        if (mPathCount < 16) {
            mPath[mPathCount].x = detour.x;
            mPath[mPathCount].y = detour.y;
            mPathCount++;
        }
        if (mPathCount < 16) {
            mPath[mPathCount].x = field_0x32c.x;
            mPath[mPathCount].y = field_0x32c.y;
            mPathCount++;
        }
        field_0x32c.x = detour.x;
        field_0x32c.y = detour.y;
        field_0x32c.z = 0.0f;
        field_0x33c = fn_801D02E0(&field_0x32c);
        mFlags &= ~0x20000;
        mFlags |= 0x1000;
    }
}

void ShadowLink2_804A43E8::fn_801C8BC4(u8 flag) {
    if (flag != 0) {
        u8 buf[0x30];
        s32 result = field_0x358.fn_80214244()->fn_800FD894(field_0x4, buf, 0x30);
        if (result <= 3) {
            field_0x55c[1] = 1;
        } else {
            field_0x55c[1] = fn_801CD664(4) + 1;
            if (field_0x524 & 0x100) {
                field_0x55c[1]++;
            }
            u8 cap = result - 2;
            u8 cur = field_0x55c[1];
            if (cap >= cur) {
                cap = cur;
            }
            field_0x55c[1] = cap;
        }
    } else {
        field_0x55c[1] = fn_801CD664(0xC) + 1;
    }
}

// Not auto-inlined: fn_801C71BC calls fn_801C8B50 out of line in the original.
#pragma auto_inline off
// FIXME(shotgun): 99 %, scheduling
void ShadowLink2_804A43E8::fn_801C8B50() {
    mPathCount = 0;
    mFlags &= ~ENEMY_FLAG_NO_MAP_HIT;
    field_0x358.fn_80214244()->fn_800FDE38();
    fn_801C8C84(0, 0);
    field_0x33c = fn_801D02E0(&field_0x32c);
    mFlags &= ~0x20000;
}
#pragma auto_inline reset

// FIXME(shotgun): 71 %, missing/extra
void ShadowLink2_804A43E8::fn_801C887C() {
    JGeometry::TVec3f railPos;
    fn_800FE294(&railPos, field_0x358.fn_80214244(), 0xFF);
    if (fn_801CCB1C(&railPos, field_0x4)) {
        u8 start = field_0x358.fn_80214244()->field_0x3;
        field_0x358.fn_80214244()->fn_800FD818(1);
        u8 next = field_0x358.fn_80214244()->field_0x3;
        if (start < field_0x358.fn_80214244()->field_0x2) {
            field_0x358.fn_80214244()->field_0x3 = start;
        }
        field_0x358.fn_80214244()->fn_800FD818(-1);
        u8 prev = field_0x358.fn_80214244()->field_0x3;
        if (start < field_0x358.fn_80214244()->field_0x2) {
            field_0x358.fn_80214244()->field_0x3 = start;
        }
        u8 opt[3];
        opt[2] = prev;
        opt[1] = start;
        opt[0] = next;
        if (field_0x4 != field_0x358.fn_80214244()->fn_800FE25C(opt[2])) {
            u8 pick = opt[0];
            if (pick < field_0x358.fn_80214244()->field_0x2) {
                field_0x358.fn_80214244()->field_0x3 = pick;
            }
        } else if (field_0x4 != field_0x358.fn_80214244()->fn_800FE25C(opt[0])) {
            u8 pick = opt[2];
            if (pick < field_0x358.fn_80214244()->field_0x2) {
                field_0x358.fn_80214244()->field_0x3 = pick;
            }
        } else if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.5f) {
            JGeometry::TVec3f p2;
            fn_800FE294(&p2, field_0x358.fn_80214244(), opt[2]);
            if (fn_801CCB1C(&p2, field_0x4)) {
                u8 pick = opt[0];
                if (pick < field_0x358.fn_80214244()->field_0x2) {
                    field_0x358.fn_80214244()->field_0x3 = pick;
                }
            } else {
                u8 pick = opt[2];
                if (pick < field_0x358.fn_80214244()->field_0x2) {
                    field_0x358.fn_80214244()->field_0x3 = pick;
                }
            }
        } else {
            JGeometry::TVec3f p0;
            fn_800FE294(&p0, field_0x358.fn_80214244(), opt[0]);
            if (fn_801CCB1C(&p0, field_0x4)) {
                u8 pick = opt[2];
                if (pick < field_0x358.fn_80214244()->field_0x2) {
                    field_0x358.fn_80214244()->field_0x3 = pick;
                }
            } else {
                u8 pick = opt[0];
                if (pick < field_0x358.fn_80214244()->field_0x2) {
                    field_0x358.fn_80214244()->field_0x3 = pick;
                }
            }
        }
        if (field_0x4 != field_0x358.fn_80214244()->fn_800FE25C(0xFF)) {
            u8 pick = opt[1];
            if (pick < field_0x358.fn_80214244()->field_0x2) {
                field_0x358.fn_80214244()->field_0x3 = pick;
            }
        }
    }
}

// FIXME(shotgun): 80 %, missing/extra + reloc-name
void ShadowLink2_804A43E8::fn_801C844C() {
    JGeometry::TVec3f pos = mPos;
    field_0x358.fn_80214244()->fn_800FE2E4(&pos, field_0x4);
    if (mFlags & 0x2000000) {
        return;
    }
    if ((field_0x524 & 0x100) &&
        field_0x358.fn_80214244()->fn_800FD5E8(field_0x358.fn_80214244()->field_0x3)) {
        u8 start = field_0x358.fn_80214244()->field_0x3;
        field_0x358.fn_80214244()->fn_800FD818(1);
        u8 next = field_0x358.fn_80214244()->field_0x3;
        if (start < field_0x358.fn_80214244()->field_0x2) {
            field_0x358.fn_80214244()->field_0x3 = start;
        }
        field_0x358.fn_80214244()->fn_800FD818(-1);
        u8 prev = field_0x358.fn_80214244()->field_0x3;
        if (start < field_0x358.fn_80214244()->field_0x2) {
            field_0x358.fn_80214244()->field_0x3 = start;
        }
        u8 opt[2];
        opt[0] = prev;
        opt[1] = next;
        u8 pick;
        if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.5f) {
            pick = opt[1];
            s32 layer = field_0x4;
            if (layer != field_0x358.fn_80214244()->fn_800FE25C(pick)) {
                pick = opt[0];
            }
        } else {
            pick = opt[0];
            s32 layer = field_0x4;
            if (layer != field_0x358.fn_80214244()->fn_800FE25C(pick)) {
                pick = opt[1];
            }
        }
        if (pick < field_0x358.fn_80214244()->field_0x2) {
            field_0x358.fn_80214244()->field_0x3 = pick;
        }
    }
    if (mFlags & 0x200000) {
        return;
    }
    if (viewLayerFlag(field_0x4)) {
        fn_801C887C();
        return;
    }
    if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.4f) {
        u8 start = field_0x358.fn_80214244()->field_0x3;
        field_0x358.fn_80214244()->fn_800FD818(1);
        u8 next = field_0x358.fn_80214244()->field_0x3;
        if (start < field_0x358.fn_80214244()->field_0x2) {
            field_0x358.fn_80214244()->field_0x3 = start;
        }
        field_0x358.fn_80214244()->fn_800FD818(-1);
        u8 prev = field_0x358.fn_80214244()->field_0x3;
        if (start < field_0x358.fn_80214244()->field_0x2) {
            field_0x358.fn_80214244()->field_0x3 = start;
        }
        u8 opt[2];
        opt[1] = prev;
        opt[0] = next;
        if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.5f) {
            if (field_0x4 == field_0x358.fn_80214244()->fn_800FE25C(opt[1])) {
                JGeometry::TVec3f p1;
                fn_800FE294(&p1, field_0x358.fn_80214244(), opt[1]);
                JGeometry::TVec3f copy1 = p1;
                if (!fn_801CF888((const JGeometry::TVec2f*)&mPos, &copy1, field_0x4)) {
                    u8 pick = opt[1];
                    if (pick < field_0x358.fn_80214244()->field_0x2) {
                        field_0x358.fn_80214244()->field_0x3 = pick;
                    }
                }
            }
        } else {
            if (field_0x4 == field_0x358.fn_80214244()->fn_800FE25C(opt[0])) {
                JGeometry::TVec3f p0;
                fn_800FE294(&p0, field_0x358.fn_80214244(), opt[0]);
                JGeometry::TVec3f copy0 = p0;
                if (!fn_801CF888((const JGeometry::TVec2f*)&mPos, &copy0, field_0x4)) {
                    u8 pick = opt[0];
                    if (pick < field_0x358.fn_80214244()->field_0x2) {
                        field_0x358.fn_80214244()->field_0x3 = pick;
                    }
                }
            }
        }
    }
}

// FIXME(shotgun): 77 %, missing/extra + reloc-name
void ShadowLink2_804A43E8::fn_801C7F98() {
    fn_801C844C();
    fn_801C8C84(0, 0);
    s16 angle = fn_801D02E0(&field_0x32c);
    field_0x33c = angle;
    mFlags &= ~0x20000;
    u8 start = field_0x358.fn_80214244()->field_0x3;
    field_0x358.fn_80214244()->fn_800FD818(1);
    u8 next = field_0x358.fn_80214244()->field_0x3;
    if (start < field_0x358.fn_80214244()->field_0x2) {
        field_0x358.fn_80214244()->field_0x3 = start;
    }
    field_0x358.fn_80214244()->fn_800FD818(-1);
    u8 prev = field_0x358.fn_80214244()->field_0x3;
    if (start < field_0x358.fn_80214244()->field_0x2) {
        field_0x358.fn_80214244()->field_0x3 = start;
    }
    u8 opt[2];
    opt[1] = prev;
    opt[0] = next;
    if (mFlags & 0x2000000) {
        if (field_0x4 != field_0x358.fn_80214244()->fn_800FE25C(opt[1])) {
            field_0x358.fn_80214244()->field_0x1 = 1;
        } else if (field_0x4 != field_0x358.fn_80214244()->fn_800FE25C(opt[0])) {
            field_0x358.fn_80214244()->field_0x1 = 0;
        } else {
            field_0x358.fn_80214244()->field_0x1 = 0;
        }
    } else if (field_0x358.fn_80214244()->fn_800FD5E8(field_0x358.fn_80214244()->field_0x3)) {
        u8 start2 = field_0x358.fn_80214244()->field_0x3;
        field_0x358.fn_80214244()->fn_800FD818(1);
        u8 next2 = field_0x358.fn_80214244()->field_0x3;
        if (start2 < field_0x358.fn_80214244()->field_0x2) {
            field_0x358.fn_80214244()->field_0x3 = start2;
        }
        field_0x358.fn_80214244()->fn_800FD818(-1);
        u8 prev2 = field_0x358.fn_80214244()->field_0x3;
        if (start2 < field_0x358.fn_80214244()->field_0x2) {
            field_0x358.fn_80214244()->field_0x3 = start2;
        }
        u8 opt2[2];
        opt2[0] = prev2;
        opt2[1] = next2;
        u8 pick;
        if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.5f) {
            pick = opt2[1];
            s32 layer = field_0x4;
            if (layer != field_0x358.fn_80214244()->fn_800FE25C(pick)) {
                pick = opt2[0];
            }
        } else {
            pick = opt2[0];
            s32 layer = field_0x4;
            if (layer != field_0x358.fn_80214244()->fn_800FE25C(pick)) {
                pick = opt2[1];
            }
        }
        if (opt[0] == pick) {
            field_0x358.fn_80214244()->field_0x1 = 1;
        } else {
            field_0x358.fn_80214244()->field_0x1 = 0;
        }
    } else {
        JGeometry::TVec3f p0;
        fn_800FE294(&p0, field_0x358.fn_80214244(), opt[0]);
        JGeometry::TVec3f copy0 = p0;
        JGeometry::TVec3f p1;
        fn_800FE294(&p1, field_0x358.fn_80214244(), opt[1]);
        JGeometry::TVec3f copy1 = p1;
        f32 dx0 = copy0.x - field_0x32c.x;
        f32 dy0 = copy0.y - field_0x32c.y;
        f32 dx1 = copy1.x - field_0x32c.x;
        f32 dy1 = copy1.y - field_0x32c.y;
        s16 a1 = (s32)(JMath::atanTable_.atan2_(dx1, dy1) * 10430.378f);
        f32 d1 = __fabs((f32)(s16)(angle - a1));
        s16 a0 = (s32)(JMath::atanTable_.atan2_(dx0, dy0) * 10430.378f);
        f32 d0 = __fabs((f32)(s16)(angle - a0));
        if (0.000030517578f * d0 < 0.000030517578f * d1) {
            if (field_0x4 != field_0x358.fn_80214244()->fn_800FE25C(opt[1])) {
                field_0x358.fn_80214244()->field_0x1 = 1;
            } else {
                field_0x358.fn_80214244()->field_0x1 = 0;
            }
        } else {
            if (field_0x4 != field_0x358.fn_80214244()->fn_800FE25C(opt[0])) {
                field_0x358.fn_80214244()->field_0x1 = 0;
            } else {
                field_0x358.fn_80214244()->field_0x1 = 1;
            }
        }
    }
}

void ShadowLink2_804A43E8::fn_801C7EE4() {
    if (!(mFlags & 0x2000000)) {
        if (field_0x358.fn_80214244()->fn_800FD548(0xFF)) {
            if ((viewLayerFlag(field_0x4) && __fabsf(mPos.y - field_0x32c.y) > 1.0f) ||
                fn_801CF888((const JGeometry::TVec2f*)&mPos, &field_0x32c, field_0x4)) {
                mFlags |= 0x400000;
            }
        }
    }
}

// FIXME(shotgun): 72 %, missing/extra + reloc-name
u8 ShadowLink2_804A43E8::fn_801C7BD0() {
    u8 ret = 0;
    s32 count = mPathCount;
    if (count == 2) {
        f32 dx = field_0x32c.x - mPos.x;
        f32 dy = field_0x32c.y - mPos.y;
        f32 r = 0.001f + lbl_8053EDF0;
        if (dx * dx + dy * dy <= r * r) {
            s32 idx1 = (count > 1) ? 1 : 0;
            f32 ny = mPath[idx1].y;
            s32 idx0 = (count > 1) ? 1 : 0;
            f32 nx = mPath[idx0].x;
            field_0x32c.x = nx;
            field_0x32c.y = ny;
            field_0x32c.z = 0.0f;
            mPathCount = 0;
            mFlags &= ~ENEMY_FLAG_NO_MAP_HIT;
        }
        return ret;
    }
    f32 gx = field_0x32c.x;
    bool blocked = false;
    f32 gy = field_0x32c.y;
    f32 radius = 10.0f;
    if (viewLayerFlag(field_0x4)) {
        radius = 0.001f + lbl_8053EDF0;
        JGeometry::TVec3f p1;
        fn_800FE294(&p1, field_0x358.fn_80214244(), 0xFF);
        JGeometry::TVec3f copy1 = p1;
        if (!fn_801CCB1C(&copy1, field_0x4)) {
            gx = copy1.x;
            gy = copy1.y;
        } else {
            blocked = true;
        }
        if (field_0x4 == field_0x358.fn_80214244()->fn_800FE25C(0xFF) &&
            !field_0x358.fn_80214244()->fn_800FD548(0xFF)) {
            JGeometry::TVec3f p2;
            fn_800FE294(&p2, field_0x358.fn_80214244(), 0xFF);
            JGeometry::TVec3f copy2 = p2;
            if (fn_801CC9F8(&mPos, field_0x4) && fn_8022461C(&copy2, field_0x4) == 0) {
                field_0x32c.x = copy2.x;
                field_0x32c.y = copy2.y;
                field_0x32c.z = copy2.z;
            } else {
                field_0x32c.x = copy2.x;
                field_0x32c.y = mPos.y;
                field_0x32c.z = copy2.z;
            }
        }
    }
    f32 dx = gx - mPos.x;
    f32 dy = gy - mPos.y;
    if (dx * dx + dy * dy <= radius * radius) {
        if (blocked) {
            fn_801C8B50();
        } else {
            if (field_0x358.fn_80214244()->fn_800FD594(0xFF)) {
                mFlags |= 0x20000;
            } else {
                mFlags &= ~0x20000;
            }
            s32 dir = 1;
            if (field_0x358.fn_80214244()->field_0x1 != 0) {
                dir = -1;
            }
            field_0x358.fn_80214244()->fn_800FD818(dir);
            fn_801C8C84(0, 0);
        }
        ret = 1;
    }
    return ret;
}

void ShadowLink2_804A43E8::fn_801C79A0() {
    JGeometry::TVec3f railPos;

    mFlags &= ~0x02000000;
    fn_800FE294(&railPos, field_0x358.fn_80214244(), 0xFF);
    // Copies through set() where the original copies field by field;
    // operator= (the psq pairs) gives other code.
    field_0x32c.set(railPos);
    field_0x56c = field_0x4;
    field_0x570.set(mPos);
    field_0x4 = field_0x358.fn_80214244()->fn_800FE25C(0xFF);
    mPos = field_0x32c;
    mPrevPos = field_0x32c;
    field_0x57c = field_0x4;
    field_0x580.set(mPos);
    if (field_0x358.fn_80214244()->fn_800FD594(0xFF)) {
        mFlags |= 0x20000;
    } else {
        mFlags &= ~0x20000;
    }

    s32 dir = 1;
    if (field_0x358.fn_80214244()->field_0x1 != 0) {
        dir = -1;
    }
    field_0x358.fn_80214244()->fn_800FD818(dir);
    fn_801C8C84(0, 1);
    fn_801C8BC4(1);

    if (field_0x524 & 0x4) {
        field_0x4 = field_0x56c;
        mPos.set(field_0x570);
        if (fn_801CD2F8(field_0x56c)) {
            field_0x58c = 1;
        } else {
            field_0x58c = 0;
        }
        fn_801C3B7C(0x18);
    } else {
        fn_801C3B7C(0x17);
    }
}

// FIXME(shotgun): 80 %, missing/extra + reloc-name
void ShadowLink2_804A43E8::fn_801C7370(u8 flag) {
    if (!(mFlags & 0x2000000)) {
        if (fn_801C5340()) {
            mFlags |= 0x2000000;
            fn_801C3B7C(0x1E);
            return;
        }
        if (mFlags & 0x400000) {
            mFlags &= ~0x400000;
            fn_801C3B7C(0x12);
            return;
        }
        if (field_0x55c[0] >= 30 && field_0x55c[1] == 1) {
            if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.015f) {
                field_0x55c[1] = 0;
            }
        }
    }
    bool nearFlag = fn_801C53C4();
    if (!(mFlags & 0x2000000) && !nearFlag && field_0x55c[1] == 0) {
        if (fn_801D05F8(&mPos, field_0x5a8) < 0) {
            fn_801C3B7C(0xF);
            return;
        }
    }
    if (!(mFlags & 0x2000000) && !nearFlag && field_0x598 == 0) {
        if (fn_801D05F8(&mPos, 60.0f) >= 0) {
            fn_801C3B7C(0x1B);
            return;
        }
    }
    u8 stepped = fn_801C7BD0();
    if ((field_0x524 & 0x1000) && field_0x358.fn_80214244()->fn_800FD5E8(0xFF)) {
        if (!gBgRewrite_8053AC98->fn_8029E248(&mPos, field_0x4)) {
            JGeometry::TVec3f below(mPos.x, mPos.y - 16.0f, mPos.z);
            gBgRewrite_8053AC98->fn_8029E248(&below, field_0x4);
        }
    }
    if (field_0x4 != field_0x358.fn_80214244()->fn_800FE25C(0xFF)) {
        fn_801C79A0();
        return;
    }
    if (stepped != 0) {
        if (field_0x55c[1] != 0) {
            field_0x55c[1]--;
        }
        if (field_0x358.fn_80214244()->fn_800FD548(0xFF)) {
            fn_801C3B7C(0x12);
            return;
        }
        JGeometry::TVec3f railPos;
        fn_800FE294(&railPos, field_0x358.fn_80214244(), 0xFF);
        if (fn_801CCB1C(&railPos, field_0x4)) {
            fn_801C8B50();
            if (!fn_801D0DD4() || (field_0x524 & 0x40)) {
                return;
            }
            if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() >= 0.5f) {
                return;
            }
            u8 start = field_0x358.fn_80214244()->field_0x3;
            field_0x358.fn_80214244()->fn_800FD818(1);
            u8 next = field_0x358.fn_80214244()->field_0x3;
            if (start < field_0x358.fn_80214244()->field_0x2) {
                field_0x358.fn_80214244()->field_0x3 = start;
            }
            field_0x358.fn_80214244()->fn_800FD818(-1);
            u8 prev = field_0x358.fn_80214244()->field_0x3;
            if (start < field_0x358.fn_80214244()->field_0x2) {
                field_0x358.fn_80214244()->field_0x3 = start;
            }
            u8 opt[2];
            opt[0] = prev;
            opt[1] = next;
            if (field_0x358.fn_80214244()->field_0x1 != 0) {
                if (field_0x4 == field_0x358.fn_80214244()->fn_800FE25C(opt[0])) {
                    JGeometry::TVec3f p0;
                    fn_800FE294(&p0, field_0x358.fn_80214244(), opt[0]);
                    if (mPos.x < p0.x) {
                        field_0x33c = 0x4000;
                    } else {
                        field_0x33c = -0x4000;
                    }
                } else if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.5f) {
                    field_0x33c = 0x4000;
                } else {
                    field_0x33c = -0x4000;
                }
            } else {
                if (field_0x4 == field_0x358.fn_80214244()->fn_800FE25C(opt[1])) {
                    JGeometry::TVec3f p1;
                    fn_800FE294(&p1, field_0x358.fn_80214244(), opt[1]);
                    if (mPos.x < p1.x) {
                        field_0x33c = 0x4000;
                    } else {
                        field_0x33c = -0x4000;
                    }
                } else if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.5f) {
                    field_0x33c = 0x4000;
                } else {
                    field_0x33c = -0x4000;
                }
            }
            fn_801C3B7C(0x1C);
            return;
        }
    } else if (flag != 0) {
        fn_801C8C84(1, 0);
    }
    s16 targetAngle = fn_801D02E0(&field_0x32c);
    if (mFlags & 0x20000) {
        fn_801CD930(&field_0x33c, targetAngle, 0x500);
    } else {
        field_0x33c = targetAngle;
    }
    if (mStateTimer % 30 == 0 && !nearFlag) {
        bool doReset = false;
        s32 player = fn_801D05F8(&mPos, 100.0f);
        if (player >= 0) {
            s16 pAngle = fn_801D0280(player);
            f32 diff = __fabs((f32)(s16)(field_0x33c - pAngle));
            if (0.000030517578f * diff <= 0.3f) {
                doReset = true;
            }
        }
        if (doReset) {
            fn_801C8B50();
        }
    }
}

// FIXME(shotgun): 67 %, missing/extra + reloc-name
void ShadowLink2_804A43E8::fn_801C71BC() {
    mFlags &= ~0x100000;
    if (!(mFlags & 0x2000000)) {
        bool moved = false;
        u8 start = field_0x358.fn_80214244()->field_0x3;
        field_0x358.fn_80214244()->fn_800FD818(1);
        u8 next = field_0x358.fn_80214244()->field_0x3;
        cls_800FE3EC* buf = field_0x358.fn_80214244();
        if (start < buf->field_0x2) {
            buf->field_0x3 = start;
        }
        field_0x358.fn_80214244()->fn_800FD818(-1);
        u8 prev = field_0x358.fn_80214244()->field_0x3;
        buf = field_0x358.fn_80214244();
        if (start < buf->field_0x2) {
            buf->field_0x3 = start;
        }
        u8 opt[3];
        opt[0] = prev;
        opt[1] = start;
        opt[2] = next;
        if (field_0x358.fn_80214244()->fn_800FD4C0(opt[1], opt[0]) ||
            field_0x358.fn_80214244()->fn_800FD4C0(opt[1], opt[2])) {
            JGeometry::TVec3f railPos;
            mPathCount = 0;
            mFlags &= ~(0x1000 | 0x20000);
            mFlags |= 0x400000;
            fn_800FE294(&railPos, field_0x358.fn_80214244(), 0xFF);
            field_0x32c.x = railPos.x;
            field_0x32c.y = railPos.y;
            field_0x32c.z = railPos.z;
            moved = true;
        }
        if (moved) {
            return;
        }
    }
    fn_801C8B50();
    field_0x33c += fn_801CD664(0x4001) - 0x2000;
    if (!(field_0x524 & 0x40)) {
        fn_801C3B7C(0x1C);
    }
}
