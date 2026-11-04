#include "game/GameAudio.h"
#include "game/act/act_shadow_link_2.h"
#include "game/ViewLayer.h"
#include "game/GameGlobals.h"
#include "game/GameMath.h"
#include "JSystem/JGeometry.h"
#include "JSystem/JMath/JMath.h"

// Turns field_0x33c toward player field_0x590: a half turn to the side of
// the player mPos is on, or the angle to the player when the view flag
// holds and fn_801CC9F8 does not.
void ShadowLink2_804A43E8::fn_801C6784() {
    if (viewLayerFlag(field_0x4) && !fn_801CC9F8(&mPos, field_0x4)) {
        const JGeometry::TVec3f* playerPos = &field_0x1ac[field_0x590];
        if (mPos.x < playerPos->x) {
            field_0x33c = 0x4000;
        } else {
            field_0x33c = -0x4000;
        }
    } else {
        field_0x33c = fn_801D0280(field_0x590);
    }
}

// FIXME: 98.61 %. Same instructions except the copy of the angle temporary
// (the original keeps the (s16) result in r31 and copies it to r4 for
// fn_801CD930; ours computes it straight into r4) and the literal labels
// (@NNN vs lbl_ for lbl_8053EE70, EE78, EE80: data split, not code).
// The turn toward player `player`. With `flag` set, the turn is a side pick
// (or fn_801D11FC) and the counter field_0x595 restarts; otherwise the
// counter counts up to field_0x596 before the turn is redone.
void ShadowLink2_804A43E8::fn_801C5EBC(s32 player, bool flag) {
    u8 viewFlag = viewLayerFlag(field_0x4);
    if (flag) {
        field_0x595 = 0;
        if (viewFlag != 0 && !fn_801CC9F8(&mPos, field_0x4)) {
            switch (field_0x98) {
            case 0:
                field_0x33c = -0x4000;
                return;
            case 1:
                field_0x33c = 0x4000;
                return;
            }
            if (lbl_8053AAF8->mRandom.get_ufloat_1() < 0.5f) {
                field_0x33c = 0x4000;
            } else {
                field_0x33c = -0x4000;
            }
        } else {
            fn_801D11FC();
        }
        return;
    }
    field_0x595++;
    if (field_0x595 < field_0x596) {
        return;
    }
    field_0x595 = 0;
    field_0x596 = fn_801CD664(0x15) + 0x14;
    JGeometry::TVec3f* playerPos = &field_0x1ac[player];
    if (viewFlag != 0 && !fn_801CC9F8(&mPos, field_0x4)) {
        if (__fabsf((f32)(s16)(field_0x33c + 0x4000)) * (1.0f / 32768.0f) < 0.05f &&
            fn_801CFA70(&mPos, playerPos, field_0x4) == true) {
            field_0x33c = -0x4000;
            return;
        }
        if (__fabsf((f32)(s16)(field_0x33c - 0x4000)) * (1.0f / 32768.0f) < 0.05f &&
            fn_801CFA70(&mPos, playerPos, field_0x4) == true) {
            field_0x33c = 0x4000;
            return;
        }
        if (lbl_8053AAF8->mRandom.get_ufloat_1() < 0.3f) {
            if (lbl_8053AAF8->mRandom.get_ufloat_1() < 0.5f) {
                field_0x33c = 0x4000;
            } else {
                field_0x33c = -0x4000;
            }
        } else {
            if (mPos.x < playerPos->x) {
                field_0x33c = -0x4000;
            } else {
                field_0x33c = 0x4000;
            }
        }
        return;
    }
    s32 angle = field_0x33c;
    f32 dx = mPos.x - playerPos->x;
    f32 dy = mPos.y - playerPos->y;
    f32 sq = dx * dx + dy * dy;
    if (sq <= 32.0f * FLT_EPSILON) {
        // a player on top of the enemy keeps the current angle
    } else {
        angle = (s32)(JMath::atanTable_.atan2_(dx, dy) * (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180()));
    }
    angle = (s16)(angle + fn_801CD664(0x4001) - 0x2000);
    fn_801CD930(&field_0x33c, angle, 0xAAA);
}

// FIXME: 97.07 %. Register choice for the locals (the original holds the entry
// index in r26 and the rail index in r27, the player in r30, ours differs),
// and the literal labels (@NNN vs lbl_ for lbl_8053EE6C, EE70, EE74, EE80,
// EE88: data split).
// The per-tick driver of the state machine: picks the nearest player within
// field_0x5ac and, in state 0x1B, a rail or a turn toward it; otherwise it
// steers toward the player (fn_801C5EBC).
void ShadowLink2_804A43E8::fn_801C5B08(bool flag) {
    if (fn_801C5340()) {
        mFlags |= 0x2000000;
        fn_801C3B7C(0x1E);
        return;
    }
    s32 player = fn_801D05F8(&mPos, field_0x5ac);
    if (player < 0) {
        if (lbl_8053AAF8->mRandom.get_ufloat_1() < 0.6f) {
            fn_801C3B7C(0xF);
        } else {
            fn_801C3B7C(0x11);
        }
        return;
    }
    field_0x338 = player;
    fn_801C6260();
    if (mState != 0x1B) {
        return;
    }
    if (field_0x59a == 0 && mStateTimer >= 0xF0 && fn_801D0DD4() && !(field_0x524 & 0x40)) {
        s32 layer = field_0x4;
        if (viewLayerFlag(layer) != 0 && !fn_801CC9F8(&mPos, field_0x4)) {
            if (lbl_8053AAF8->mRandom.get_ufloat_1() < 0.5f) {
                field_0x33c = 0x4000;
            } else {
                field_0x33c = -0x4000;
            }
        } else {
            field_0x33c = fn_801D0280(player);
            s32 rnd = fn_801CD664(0x4001);
            field_0x33c = field_0x33c + rnd - 0x2000;
        }
        fn_801C3B7C(0x1C);
        field_0x59a = 0xB4;
        return;
    }
    bool found = false;
    if (field_0x599 == 0) {
        JGeometry::TVec3f railPos;
        JGeometry::TVec3f pos;
        u8 entry = field_0x358.fn_80214244()->field_0x3;
        pos = mPos;
        u8 index = field_0x358.fn_80214244()->fn_800FE2E4(&pos, field_0x4);
        cls_800FE3EC* rail = field_0x358.fn_80214244();
        if (entry < rail->field_0x2) {
            rail->field_0x3 = entry;
        }
        fn_800FE294(&railPos, field_0x358.fn_80214244(), index);
        JGeometry::TVec3f target = railPos;
        f32 dx = target.x - mPos.x;
        f32 dy = target.y - mPos.y;
        f32 sq = square(dx) + square(dy);
        if (sq <= 1024.0f) {
            if (field_0x358.fn_80214244()->fn_800FD5E8(index)) {
                found = true;
            } else {
                s16 angle = fn_801D02E0(&target);
                s16 delta = field_0x33c - angle;
                f32 d = __fabsf((f32)delta) * (1.0f / 32768.0f);
                if (d <= 0.3f) {
                    if (lbl_8053AAF8->mRandom.get_ufloat_1() < 0.05f) {
                        found = true;
                    }
                }
            }
        }
    }
    if (found) {
        mFlags |= 0x200000;
        fn_801C3B7C(0x11);
        mFlags &= ~0x200000;
        field_0x598 = 0x78;
    } else {
        fn_801C5EBC(player, flag);
    }
}
