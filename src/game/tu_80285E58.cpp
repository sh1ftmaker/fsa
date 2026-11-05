#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Player.h"

// The length of v in the x/y plane (z is the height).
static inline f32 planarLength(const JGeometry::TVec3f& v) {
    return JGeometry::TUtil<f32>::sqrt(v.x * v.x + v.y * v.y);
}

// The player's per-frame update (docs/PLAYER.md, "Per-frame flow"): run by
// fn_80253E6C (update A) for each player.
void Player_804A6F38::fn_80285E58() {
    if (field_0x11f4 > 0) {
        field_0x11f4--;
        for (int i = 0; i < 0x50; i++) {
            field_0xdb0[i] = -1;
            field_0x1030[i] = -1;
        }
    } else {
        fn_802870C8();
    }
    fn_80265E88();
    fn_8024CA20();
}

// FIXME: 99.53. f4/f5 swapped for the two planar distances; the literals
// (0.5f, 3.0f, 8.0f) are @NNN until fn_80285F44, which pools 0, 6, 1, 0.5, 3,
// 0.3, 8 first, is written.
void Player_804A6F38::fn_8028676C() {
    JGeometry::TVec3f target;
    fn_8027FA08(&target);
    if (!fn_80240808()) {
        if (field_0x1216) {
            field_0x1228++;
            if (fn_8025197C(true) || field_0x1228 > 180) {
                mPos.set(field_0x38[0]);
                field_0x2c.set(mPos);
                field_0x1228 = 0;
            }
        } else {
            field_0x1228 = 0;
        }
        return;
    }

    if (!fn_802413E8() && !fn_80270A8C() && !fn_80250D30() && !fn_8024FEFC() && mPos.z <= 0.0f &&
        fn_8022BC38(fn_80240644(), field_0x3b4, &mPos)) {
        if (!fn_80234ECC(fn_80240644()) &&
            planarLength(*fn_8023E724(fn_80240644()) - target) <= planarLength(mPos - target)) {
            field_0x1228++;
            if (field_0x1238 > 0) {
                field_0x1238--;
            }
            if ((!(fn_80240DAC() || fn_80240D8C() || field_0x43e) && fn_8025197C(true)) ||
                (fn_802486A0() == 5 && fn_80240B84()) || field_0x1228 > 180) {
                mPos.set(*fn_8023E724(fn_80240644()));
                field_0x1228 = 0;
                field_0x1238 = 0;
            }
        } else {
            field_0x1228 = 0;
        }
    } else {
        field_0x1228 = 0;
    }

    if (fn_80234E30(fn_80240644())) {
        field_0x1228 = 0;
    }
    if (fn_802486A0() == 4) {
        field_0x1238 = 0;
        return;
    }
    if (planarLength(target - mPos) < 8.0f) {
        field_0x1238 = 0;
    }
}

// FIXME: 99.20. Register allocation: the original keeps the player loop
// counter, `player` and `start` in r27 and `other` in r29.
void Player_804A6F38::fn_80286E10() {
    if (field_0x1220 > 0) {
        return;
    }
    if (field_0x1216 == true) {
        return;
    }
    for (int i = 0; i < 4; i++) {
        if (fn_8022E438(field_0x4, i)) {
            if (fn_80236B50(i)) {
                return;
            }
            if (fn_8022D75C(i)) {
                return;
            }
        }
    }
    if (fn_80240808()) {
        return;
    }
    s32 player = field_0x4;
    if (fn_802486A0() == 4) {
        player = fn_8022EA0C(field_0x4);
    }
    if (fn_80230188(player)) {
        return;
    }
    if (!(fn_8023E724(player)->z <= 0.0f)) {
        return;
    }
    if (fn_8023EA28(player)) {
        return;
    }
    if (fn_80234ECC(player)) {
        return;
    }
    if (fn_80235274(player)) {
        return;
    }
    if (fn_80234CF8(player)) {
        return;
    }
    if (fn_80234C5C(player)) {
        return;
    }
    if (fn_80235744(player)) {
        return;
    }
    if (fn_802353AC(player)) {
        return;
    }
    if (fn_80233DA4(player)) {
        return;
    }
    if (fn_80233EDC(player)) {
        return;
    }
    if (fn_80234E30(player)) {
        return;
    }
    if (fn_80248970() < 2) {
        return;
    }
    if (field_0x121e) {
        return;
    }
    if (!lbl_8053AD30->fn_80309D6C(fn_8030AAF4(field_0x11ec), 0x400)) {
        return;
    }
    if (fn_802486A0() == 4) {
        s32 start = field_0x1224;
        s32 partner = fn_8022EA0C(field_0x4);
        s32 other;
        do {
            field_0x1224++;
            if (field_0x1224 >= fn_80248970()) {
                field_0x1224 = 0;
            }
            other = fn_8022EA0C(field_0x4);
            if (!fn_80234134(other) && !fn_8023EA28(other) &&
                fn_8022BC38(other, fn_8023DE58(partner), fn_8023E724(partner))) {
                break;
            }
        } while (start != field_0x1224);
        if (start != field_0x1224) {
            fn_80230FD0(other, 0x10010);
            if (fn_8023ED8C(partner)) {
                fn_8023105C(fn_8023ECF0(partner));
                fn_802310F8(partner);
            }
        }
    } else {
        fn_80248004(4, true);
    }
}
