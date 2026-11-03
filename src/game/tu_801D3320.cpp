#include "game/GameAudio.h"

#include "game/GameGlobals.h"
#include "game/GameMath.h"
#include "game/RailBase.h"
#include "game/act/act_shadow_link_2.h"

// Moves field_0x32c (the rail waypoint the actor is heading for) one step: with
// mFlags bit 0x2000000 to the entry of the rail (field_0x358) farthest from
// mPos, else to a random usable entry, then toward the entry nearest the
// actor (fn_801C976C), by a random fraction of the way.
//
// FIXME: 95.0 %. Only register allocation differs: the original keeps the
// loop counter, the u8 best/count and the entry pointer in r28/r30/r31/r26
// (counter copied from the best/numUsable zero with mr), ours picks other
// registers; the u8 `sel` and `player` are widened a second time (clrlwi)
// in the original. The stack slots (hoisted vectors, in reverse order) match.
void ShadowLink2_804A43E8::fn_801D3320() {
    // Declared up front: the original's stack slots follow this order.
    u8 usable[0x30];
    JGeometry::TVec3f from;
    JGeometry::TVec3f to;
    JGeometry::TVec3f step;
    JGeometry::TVec3f dir;
    JGeometry::TVec3f railA;
    JGeometry::TVec3f railB;
    JGeometry::TVec3f tmpTo;
    JGeometry::TVec3f tmpA;
    JGeometry::TVec3f pA;
    JGeometry::TVec3f pB;
    if (mFlags & 0x2000000) {
        u8 entries[0x30];
        u8 count = field_0x358.fn_80214244()->fn_800FD894(field_0x4, entries, 0x30);
        u8 best = 0;
        f32 bestDist = -1.0f;
        for (s32 i = 0; i < count; i++) {
            if (field_0x358.fn_80214244()->fn_800FD5E8(entries[i])) {
                fn_800FE294(&tmpA, field_0x358.fn_80214244(), entries[i]);
                pA.set(tmpA);
                bool ok = true;
                s32 layer = field_0x4;
                if (fn_801CCBD0(&pA, layer) || fn_801CCB1C(&pA, layer)) {
                    ok = false;
                }
                if (ok) {
                    f32 dist = square(pA.x - mPos.x) + square(pA.y - mPos.y);
                    if (dist > bestDist) {
                        bestDist = dist;
                        best = entries[i];
                    }
                }
            }
        }
        cls_800FE3EC* cur = field_0x358.fn_80214244();
        if (best < cur->field_0x2) {
            cur->field_0x3 = best;
        }
        fn_800FE294(&railA, field_0x358.fn_80214244(), 0xFF);
        field_0x32c.set(railA);
    } else {
        u8 entries[0x30];
        u8 count = field_0x358.fn_80214244()->fn_800FD894(field_0x4, entries, 0x30);
        u8 numUsable = 0;
        u8* out = usable;
        for (s32 i = 0; i < count; i++) {
            s32 layer = field_0x4;
            fn_800FE294(&pB, field_0x358.fn_80214244(), entries[i]);
            bool ok = true;
            if (fn_801CCBD0(&pB, layer) || fn_801CCB1C(&pB, layer)) {
                ok = false;
            }
            if (ok) {
                *out++ = entries[i];
                numUsable++;
            }
        }
        u8 sel = usable[fn_801CD664(numUsable)];
        cls_800FE3EC* cur = field_0x358.fn_80214244();
        if (sel < cur->field_0x2) {
            cur->field_0x3 = sel;
        }
        fn_800FE294(&railB, field_0x358.fn_80214244(), 0xFF);
        field_0x32c.set(railB);
        if (numUsable == 1) {
            return;
        }
        s32 layer = field_0x4;
        u32 player = fn_801C976C();
        if (layer != field_0x358.fn_80214244()->fn_800FE25C(player)) {
            return;
        }
        bool found = false;
        for (s32 i = 0; i < numUsable; i++) {
            if (player == usable[i]) {
                found = true;
                break;
            }
        }
        if (!found) {
            return;
        }
        from = field_0x32c;
        fn_800FE294(&tmpTo, field_0x358.fn_80214244(), player);
        to = tmpTo;
        if (field_0x358.fn_80214244()->fn_800FD4C0(sel, player) &&
            fn_801CF888((const JGeometry::TVec2f*)&from, &to, field_0x4)) {
            return;
        }
        step = to;
        step.sub(from);
        step.z = 0.0f;
        if (step.isZero()) {
            return;
        }
        dir = step;
        f32 rnd = lbl_8053AAF8->mRandom.get_ufloat_1();
        step.scale(rnd);
        bool selOk = field_0x358.fn_80214244()->fn_800FD5E8(sel);
        bool playerOk = field_0x358.fn_80214244()->fn_800FD5E8(player);
        if (selOk) {
            if (step.squared() < 1024.0f) {
                step.set(dir);
                step.normalize();
                step.scale(32.0f);
            }
        } else if (playerOk) {
            f32 full = dir.length();
            if (step.length() > full - 32.0f) {
                step.set(dir);
                step.normalize();
                step.scale(full - 32.0f);
            }
        }
        field_0x32c += step;
        bool ok = true;
        layer = field_0x4;
        if (fn_801CCBD0(&field_0x32c, layer) || fn_801CCB1C(&field_0x32c, layer)) {
            ok = false;
        }
        if (ok) {
            if (!fn_801CAEF4(&field_0x32c, field_0x4)) {
                return;
            }
        }
        if (playerOk) {
            field_0x32c.set(from);
        } else if (selOk) {
            cls_800FE3EC* c = field_0x358.fn_80214244();
            if (player < c->field_0x2) {
                c->field_0x3 = player;
            }
            field_0x32c.set(to);
        } else {
            field_0x32c.set(from);
            if (fn_801CAEF4(&field_0x32c, field_0x4)) {
                cls_800FE3EC* c = field_0x358.fn_80214244();
                if (player < c->field_0x2) {
                    c->field_0x3 = player;
                }
                field_0x32c.set(to);
            }
        }
    }
}
